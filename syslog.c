/* Copyright 2013-2024
 * Kaz Kylheku <kaz@kylheku.com>
 * Vancouver, Canada
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <syslog.h>
#include "config.h"
#include "alloca.h"
#include "lib.h"
#include "stream.h"
#include "gc.h"
#include "args.h"
#include "utf8.h"
#include "eval.h"
#include "syslog.h"

struct syslog_strm {
  struct strm_base a;
  val prio;
  val strstream;
};

val prio_k;

static_forward(struct strm_ops syslog_strm_ops);

void syslog_init(void)
{
  reg_varl(intern(lit("log-pid"), user_package), num_fast(LOG_PID));
  reg_varl(intern(lit("log-cons"), user_package), num_fast(LOG_CONS));
  reg_varl(intern(lit("log-ndelay"), user_package), num_fast(LOG_NDELAY));
  reg_varl(intern(lit("log-odelay"), user_package), num_fast(LOG_ODELAY));
  reg_varl(intern(lit("log-nowait"), user_package), num_fast(LOG_NOWAIT));
#ifdef LOG_PERROR
  reg_varl(intern(lit("log-perror"), user_package), num_fast(LOG_PERROR));
#endif
  reg_varl(intern(lit("log-user"), user_package), num_fast(LOG_USER));
  reg_varl(intern(lit("log-daemon"), user_package), num_fast(LOG_DAEMON));
  reg_varl(intern(lit("log-auth"), user_package), num_fast(LOG_AUTH));
#ifdef LOG_AUTHPRIV
  reg_varl(intern(lit("log-authpriv"), user_package), num_fast(LOG_AUTHPRIV));
#endif
  reg_varl(intern(lit("log-emerg"), user_package), num_fast(LOG_EMERG));
  reg_varl(intern(lit("log-alert"), user_package), num_fast(LOG_ALERT));
  reg_varl(intern(lit("log-crit"), user_package), num_fast(LOG_CRIT));
  reg_varl(intern(lit("log-err"), user_package), num_fast(LOG_ERR));
  reg_varl(intern(lit("log-warning"), user_package), num_fast(LOG_WARNING));
  reg_varl(intern(lit("log-notice"), user_package), num_fast(LOG_NOTICE));
  reg_varl(intern(lit("log-info"), user_package), num_fast(LOG_INFO));
  reg_varl(intern(lit("log-debug"), user_package), num_fast(LOG_DEBUG));
  reg_fun(intern(lit("openlog"), user_package), func_n3o(openlog_wrap, 1));
  reg_fun(intern(lit("closelog"), user_package), func_n0(closelog_wrap));
  reg_fun(intern(lit("setlogmask"), user_package), func_n1(setlogmask_wrap));
  reg_fun(intern(lit("syslog"), user_package), func_n2v(syslog_wrapv));

  prio_k = intern(lit("prio"), keyword_package);

  reg_var(intern(lit("*stdlog*"), user_package), make_syslog_stream(num_fast(LOG_INFO)));

  fill_stream_ops(&syslog_strm_ops);
}

val openlog_wrap(val wident, val optmask, val facility)
{
  val self = lit("openlog");
  static char *ident;
  cnum coptmask = c_num(default_arg(optmask, zero), self);
  cnum cfacility = c_num(default_arg(facility, num_fast(LOG_USER)), self);
  char *old_ident = ident;

  ident = utf8_dup_to(c_str(wident, self));

  openlog(ident, coptmask, cfacility);

  free(old_ident);

  return nil;
}

val setlogmask_wrap(val mask)
{
  val self = lit("setlogmask");
  return num(setlogmask(c_num(mask, self)));
}

val syslog_wrapv(val prio, val fmt, varg args)
{
  val self = lit("syslog");
  val text = formatv(nil, fmt, args);
  cnum cprio = c_num(prio, self);
  char *u8text = utf8_dup_to(c_str(text, self));
  syslog(cprio, "%s", u8text);
  return nil;
}

val syslog_wrap(val prio, val fmt, val arglist)
{
  args_decl_list(args, ARGS_MIN, arglist);
  return syslog_wrapv(prio, fmt, args);
}

val closelog_wrap(void)
{
  closelog();
  return nil;
}

static void syslog_mark(val stream)
{
  struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
  strm_base_mark(&s->a);
  gc_mark(s->prio);
  gc_mark(s->strstream);
}

static val syslog_put_string(val stream, val str)
{
  struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
  val strstream = s->strstream;

  for (;;) {
    val length = length_str(str);
    val span_to_newline = compl_span_str(str, lit("\n"));

    if (zerop(length))
      break;

    put_string(sub_str(str, nil, span_to_newline), strstream);

    if (equal(span_to_newline, length))
      break;

    str = sub_str(str, plus(span_to_newline, num(1)), nil);
    syslog_wrap(s->prio, lit("~a"), list(get_string_from_stream(strstream), nao));
    strstream = make_string_output_stream();
  }

  set(mkloc(s->strstream, stream), strstream);
  return t;
}

static val syslog_put_char(val stream, val ch)
{
  struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
  val strstream = s->strstream;

  if (ch == chr('\n')) {
    syslog_wrap(s->prio, lit("~a"), list(get_string_from_stream(strstream), nao));
    strstream = make_string_output_stream();
  } else {
    put_char(ch, strstream);
  }

  set(mkloc(s->strstream, stream), strstream);
  return t;
}

static val syslog_put_byte(val stream, int ch)
{
  struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
  val strstream = s->strstream;

  if (ch == '\n') {
    syslog_wrap(s->prio, lit("~a"), list(get_string_from_stream(strstream), nao));
    strstream = make_string_output_stream();
  } else {
    put_byte(num(ch), strstream);
  }

  set(mkloc(s->strstream, stream), strstream);
  return t;
}

static val syslog_get_prop(val stream, val ind)
{
  if (ind == prio_k) {
    struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
    return s->prio;
  } else if (ind == name_k) {
    return lit("syslog");
  }
  return nil;
}

static val syslog_set_prop(val stream, val ind, val prop)
{
  if (ind == prio_k) {
    struct syslog_strm *s = coerce(struct syslog_strm *, stream->co.handle);
    set(mkloc(s->prio, stream), prop);
    return t;
  }
  return nil;
}

static_def(struct strm_ops syslog_strm_ops =
  strm_ops_init(cobj_ops_init(eq,
                              stream_print_op,
                              cobj_destroy_free_op,
                              syslog_mark,
                              cobj_eq_hash_op,
                              0),
                wli("syslog-stream"),
                syslog_put_string,
                syslog_put_char,
                syslog_put_byte,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                syslog_get_prop,
                syslog_set_prop,
                0, 0, 0, 0));

val make_syslog_stream(val prio)
{
  struct syslog_strm *s = coerce(struct syslog_strm *, chk_malloc(sizeof *s));
  val stream;
  val strstream = make_string_output_stream();
  strm_base_init(&s->a);
  s->prio = prio;
  s->strstream = nil;
  stream = cobj(coerce(mem_t *, s), stream_cls, &syslog_strm_ops.cobj_ops);
  s->strstream = strstream;
  return stream;
}
