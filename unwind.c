/* Copyright 2009-2024
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
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include "config.h"
#if HAVE_VALGRIND
#include <valgrind/memcheck.h>
#endif
#include "lib.h"
#include "gc.h"
#include "args.h"
#include "stream.h"
#include "txr.h"
#include "signal.h"
#include "eval.h"
#include "struct.h"
#include "cadr.h"
#include "alloca.h"
#include "unwind.h"
#include "debug.h"

#define UW_CONT_FRAME_BEFORE (32 * sizeof (val))
#define UW_CONT_FRAME_AFTER (16 * sizeof (val))

static uw_frame_t *uw_stack;
static uw_frame_t *uw_menv_stack;
static uw_frame_t *uw_exit_point;
static uw_frame_t toplevel_env;
static uw_frame_t unhandled_ex;

static val unhandled_hook_s, types_s, jump_s, desc_s;
#if CONFIG_DEBUG_SUPPORT
static val args_s, form_s;
#endif
static val sys_cont_s, sys_cont_poison_s;
static val sys_cont_free_s, sys_capture_cont_s;

val catch_frame_s;

static val frame_type, catch_frame_type, handle_frame_type;
#if CONFIG_DEBUG_SUPPORT
static val fcall_frame_type, eval_frame_type, expand_frame_type;
#endif

static val deferred_warnings, tentative_defs;

static struct cobj_class *sys_cont_cls;

#if CONFIG_EXTRA_DEBUGGING
static int uw_break_on_error;
#endif

static int reentry_count;

static void uw_unwind_to_exit_point(void)
{
  uw_frame_t *orig_stack = uw_stack;
  assert (uw_exit_point);

  for (; uw_stack && uw_stack != uw_exit_point; uw_stack = uw_stack->uw.up) {
    switch (uw_stack->uw.type) {
    case UW_CATCH:
      /* If a catch block is not visible, do
         not run its unwind stuff. This
         would cause infinite loops if
         unwind blocks trigger a nonlocal exit. */
      if (!uw_stack->ca.visible)
        continue;
      /* Catches catch everything, so that they
         can do "finally" or "unwind protect" logic.
         If a catch is invoked with a nil exception
         and symbol, it must execute the
         mandatory clean-up code and then
         continue the unwinding by calling uw_continue,
         passing it the ca.cont value. */
      uw_stack->ca.sym = nil;
      uw_stack->ca.args = nil;
      uw_stack->ca.cont = uw_exit_point;
      /* 1 means unwind only. */
      extended_longjmp(uw_stack->ca.jb, 1);
      abort();
    case UW_MENV:
      /* Maintain consistency of unwind stack pointer */
      uw_menv_stack = uw_menv_stack->ev.up_env;
      break;
    case UW_GUARD:
      if (uw_stack->gu.uw_ok)
        break;
      ++reentry_count;
      format(top_stderr, lit("~a: cannot unwind across foreign stack frames\n"),
             prog_string, nao);
      abort();
    default:
      break;
    }
  }

  if (uw_exit_point == &unhandled_ex) {
    val sym = unhandled_ex.ca.sym;
    val args = unhandled_ex.ca.args;

    gc_stack_limit = 0;
    ++reentry_count;

    dyn_env = nil;

    if (opt_loglevel >= 1) {
      val prefix = scat2(prog_string, lit(":"));

      flush_stream(std_output);
      format(top_stderr, lit("~a unhandled exception of type ~a:\n"),
             prefix, sym, nao);

      uw_stack = orig_stack;
      error_trace(sym, args, top_stderr, prefix);
    }
    if (uw_exception_subtype_p(sym, query_error_s) ||
        uw_exception_subtype_p(sym, file_error_s)) {
      if (opt_print_bindings)
        put_line(lit("false"), std_output);
    }

    exit(EXIT_FAILURE);
  }

  uw_exit_point = 0;

  if (!uw_stack)
    abort();

  switch (uw_stack->uw.type) {
  case UW_BLOCK:
    extended_longjmp(uw_stack->bl.jb, 1);
    abort();
  case UW_MENV: /* env frame cannot be exit point */
    abort();
  case UW_CATCH:
    /* 2 means actual catch, not just unwind */
    extended_longjmp(uw_stack->ca.jb, 2);
  default:
    abort();
  }
}

static void uw_abscond_to_exit_point(void)
{
  assert (uw_exit_point);

  for (; uw_stack && uw_stack != uw_exit_point; uw_stack = uw_stack->uw.up) {
    switch (uw_stack->uw.type) {
    case UW_MENV:
      uw_menv_stack = uw_menv_stack->ev.up_env;
      break;
    default:
      break;
    }
  }

  if (!uw_stack)
    abort();

  uw_exit_point = 0;

  switch (uw_stack->uw.type) {
  case UW_BLOCK:
    extended_longjmp(uw_stack->bl.jb, 1);
    abort();
  default:
    abort();
  }
}

uw_snapshot_t uw_snapshot(void)
{
  uw_snapshot_t snap = {
    dyn_env, uw_stack, uw_menv_stack
  };

  return snap;
}

void uw_restore(const uw_snapshot_t *psnap)
{
  dyn_env = psnap->de;
  uw_stack = psnap->stack;
  uw_menv_stack = psnap->menv_stack;
}

void uw_push_block(uw_frame_t *fr, val tag)
{
  memset(fr, 0, sizeof *fr);
  fr->bl.type = UW_BLOCK;
  fr->bl.tag = tag;
  fr->bl.result = nil;
  fr->bl.up = uw_stack;
  fr->bl.cont_bottom = 0;
  uw_stack = fr;
}

static uw_frame_t *uw_find_env(void)
{
  return uw_menv_stack ? uw_menv_stack : &toplevel_env;
}

void uw_push_match_env(uw_frame_t *fr)
{
  uw_frame_t *prev_env = uw_find_env();
  memset(fr, 0, sizeof *fr);
  fr->ev.type = UW_MENV;
  fr->ev.up_env = prev_env;
  fr->ev.func_bindings = nil;
  fr->ev.match_context = nil;
  fr->ev.up = uw_stack;
  uw_stack = fr;
  uw_menv_stack = fr;
}

val uw_get_func(val sym)
{
  uw_frame_t *env;

  for (env = uw_find_env(); env != 0; env = env->ev.up_env) {
    if (env->ev.func_bindings) {
      val found = assoc(sym, env->ev.func_bindings);
      if (found)
        return cdr(found);
    }
  }

  return nil;
}

val uw_set_func(val sym, val value)
{
  uw_frame_t *env = uw_find_env();
  env->ev.func_bindings = acons_new(sym, value, env->ev.func_bindings);
  return value;
}

val uw_get_match_context(void)
{
  uw_frame_t *env = uw_find_env();
  return env->ev.match_context;
}

val uw_set_match_context(val context)
{
  uw_frame_t *env = uw_find_env();
  env->ev.match_context = context;
  return context;
}

void uw_push_guard(uw_frame_t *fr, int uw_ok)
{
  memset(fr, 0, sizeof *fr);
  fr->uw.type = UW_GUARD;
  fr->uw.up = uw_stack;
  fr->gu.uw_ok = uw_ok;
  uw_stack = fr;
}

void uw_pop_frame(uw_frame_t *fr)
{
  assert (fr == uw_stack);
  uw_stack = fr->uw.up;
  if (fr->uw.type == UW_MENV) {
    assert (fr == uw_menv_stack);
    uw_menv_stack = fr->ev.up_env;
  }
}

void uw_pop_block(uw_frame_t *fr, val *pret)
{
  if (uw_stack->uw.type == UW_CAPTURED_BLOCK) {
    assert (fr->uw.type == UW_BLOCK || fr->uw.type == UW_CAPTURED_BLOCK);
    assert (uw_stack->bl.tag == fr->bl.tag);

    uw_stack = fr->uw.up;
    uw_block_return(uw_stack->bl.tag, *pret);
    abort();
  }

  uw_pop_frame(fr);
}

void uw_pop_until(uw_frame_t *fr)
{
  while (uw_stack != fr)
    uw_pop_frame(uw_stack);
}

uw_frame_t *uw_current_frame(void)
{
  return uw_stack;
}

uw_frame_t *uw_current_exit_point(void)
{
  return uw_exit_point;
}

val uw_get_frames(void)
{
  uw_frame_t *ex;
  list_collect_decl (out, ptail);

  for (ex = uw_stack; ex != 0; ex = ex->uw.up) {
    switch (ex->uw.type) {
    case UW_CATCH:
      if (ex->ca.matches && ex->ca.visible) {
        val cf = allocate_struct(catch_frame_type);
        slotset(cf, types_s, ex->ca.matches);
        slotset(cf, jump_s, cptr(coerce(mem_t *, ex)));
        ptail = list_collect(ptail, cf);
      }
      break;
    case UW_HANDLE:
      if (ex->ha.visible) {
        val hf = allocate_struct(handle_frame_type);
        slotset(hf, types_s, ex->ha.matches);
        slotset(hf, fun_s, ex->ha.fun);
        ptail = list_collect(ptail, hf);
      }
    default:
      break;
    }
  }

  return out;
}

static val uw_find_frames_impl(val extype, val frtype, val just_one)
{
  uw_frame_t *ex;
  uw_frtype_t et;
  list_collect_decl (out, ptail);

  extype = default_null_arg(extype);
  frtype = default_arg_strict(frtype, catch_frame_type);

  if (symbolp(frtype)) {
    frtype = find_struct_type(frtype);
    if (!frtype)
      return nil;
  }

  if (frtype == catch_frame_type)
    et = UW_CATCH;
  else if (frtype == handle_frame_type)
    et = UW_HANDLE;
  else
    return nil;

  if (frtype != catch_frame_type && frtype != handle_frame_type)
    return nil;

  for (ex = uw_stack; ex != 0; ex = ex->uw.up) {
    if (ex->uw.type == et && ex->ca.visible) {
      val match;
      for (match = ex->ca.matches; match; match = cdr(match))
        if (uw_exception_subtype_p(extype, car(match)))
          break;
      if (match) {
        val fr = allocate_struct(frtype);
        slotset(fr, types_s, ex->ca.matches);
        if (et == UW_CATCH) {
          slotset(fr, desc_s, ex->ca.desc);
          slotset(fr, jump_s, cptr(coerce(mem_t *, ex)));
        } else {
          slotset(fr, fun_s, ex->ha.fun);
        }
        if (just_one)
          return fr;
        ptail = list_collect(ptail, fr);
      }
    }
  }

  return out;
}

val uw_find_frame(val extype, val frtype)
{
  return uw_find_frames_impl(extype, frtype, t);
}

val uw_find_frames(val extype, val frtype)
{
  return uw_find_frames_impl(extype, frtype, nil);
}

#if CONFIG_DEBUG_SUPPORT

val uw_find_frames_by_mask(val mask_in)
{
  val self = lit("find-frames-by-mask");
  ucnum mask = c_unum(mask_in, self);
  list_collect_decl (out, ptail);
  uw_frame_t *fr;

  for (fr = uw_stack; fr != 0; fr = fr->uw.up) {
    uw_frtype_t type = fr->uw.type;
    if (((1U << type) & mask) != 0) {
      val frame = nil;
      switch (type) {
      case UW_CATCH:
        {
          frame = allocate_struct(catch_frame_type);
          slotset(frame, types_s, fr->ca.matches);
          slotset(frame, desc_s, fr->ca.desc);
          slotset(frame, jump_s, cptr(coerce(mem_t *, fr)));
          break;
        }
      case UW_HANDLE:
        {
          frame = allocate_struct(handle_frame_type);
          slotset(frame, types_s, fr->ha.matches);
          slotset(frame, fun_s, fr->ha.fun);
          break;
        }
      case UW_FCALL:
        {
          varg frargs = fr->fc.args;
          args_decl(acopy, frargs->argc);
          args_copy(acopy, frargs);
          frame = allocate_struct(fcall_frame_type);
          slotset(frame, fun_s, fr->fc.fun);
          slotset(frame, args_s, args_get_list(acopy));
          break;
        }
      case UW_EVAL:
        {
          frame = allocate_struct(eval_frame_type);
          slotset(frame, form_s, fr->el.form);
          slotset(frame, env_s, fr->el.env);
          break;
        }
      case UW_EXPAND:
        {
          frame = allocate_struct(expand_frame_type);
          slotset(frame, form_s, fr->el.form);
          slotset(frame, env_s, fr->el.env);
          break;
        }
      default:
        break;
      }

      if (frame)
        ptail = list_collect(ptail, frame);
    }
  }

  return out;
}

#endif

#if CONFIG_DEBUG_SUPPORT

val uw_last_form_expanded(void)
{
  uw_frame_t *fr;

  for (fr = uw_stack; fr != 0; fr = fr->uw.up) {
    if (fr->uw.type == UW_EXPAND)
      return fr->el.form;
  }

  return nil;
}

#endif

val uw_invoke_catch(val catch_frame, val sym, varg args)
{
  uw_frame_t *ex, *ex_point;

  if (struct_type(catch_frame) != catch_frame_type)
    uw_throwf(type_error_s, lit("invoke-catch: ~s isn't a catch frame"),
              catch_frame, nao);

  ex_point = coerce(uw_frame_t *, cptr_get(slot(catch_frame, jump_s)));

  for (ex = uw_stack; ex != 0; ex = ex->uw.up)
    if (ex == ex_point && ex->uw.type == UW_CATCH)
      break;

  if (!ex)
    uw_throwf(type_error_s, lit("invoke-catch: ~s no longer exists"),
              catch_frame, nao);

  ex->ca.sym = sym;
  ex->ca.args = args_get_list(args);
  uw_exit_point = ex;
  uw_unwind_to_exit_point();
  abort();
}

val uw_muffle_warning(val exc, varg args)
{
  (void) exc;
  (void) args;
  return uw_rthrow(continue_s, nil);
}

val uw_trace_error(val ctx, val exc, varg args)
{
  cons_bind (stream, prefix, ctx);
  error_trace(exc, args_get_list(args), stream, prefix);
  return nil;
}

void uw_push_cont_copy(uw_frame_t *fr, mem_t *ptr,
                       void (*copy)(mem_t *ptr))
{
  memset(fr, 0, sizeof *fr);
  fr->cp.type = UW_CONT_COPY;
  fr->cp.ptr = ptr;
  fr->cp.copy = copy;
  fr->cp.up = uw_stack;
  uw_stack = fr;
}

val uw_block_return_proto(val tag, val result, val protocol)
{
  uw_frame_t *ex;

  for (ex = uw_stack; ex != 0; ex = ex->uw.up) {
    if (ex->uw.type == UW_BLOCK && ex->bl.tag == tag)
      break;
  }

  if (ex == 0)
    return nil;

  ex->bl.result = result;
  ex->bl.protocol = protocol;
  uw_exit_point = ex;
  uw_unwind_to_exit_point();
  abort();
}

val uw_block_abscond(val tag, val result)
{
  uw_frame_t *ex;

  for (ex = uw_stack; ex != 0; ex = ex->uw.up) {
    if (ex->uw.type == UW_BLOCK && ex->bl.tag == tag)
      break;
    if (ex->uw.type == UW_GUARD)
      uw_throwf(error_s, lit("~a: cannot abscond via foreign stack frames"),
                prog_string, nao);

  }

  if (ex == 0)
    return nil;

  ex->bl.result = result;
  ex->bl.protocol = nil;
  uw_exit_point = ex;
  uw_abscond_to_exit_point();
  abort();
}

void uw_push_catch(uw_frame_t *fr, val matches)
{
  memset(fr, 0, sizeof *fr);
  fr->ca.type = UW_CATCH;
  fr->ca.matches = matches;
  fr->ca.args = nil;
  fr->ca.cont = 0;
  fr->ca.visible = 1;
  fr->ca.up = uw_stack;
  uw_stack = fr;
}

void uw_push_handler(uw_frame_t *fr, val matches, val fun)
{
  memset(fr, 0, sizeof *fr);
  fr->ha.type = UW_HANDLE;
  fr->ha.matches = matches;
  fr->ha.fun = fun;
  fr->ha.visible = 1;
  fr->ha.up = uw_stack;
  fr->ha.package = cur_package;
  fr->ha.package_alist = deref(cur_package_alist_loc);
  uw_stack = fr;
}

#if CONFIG_DEBUG_SUPPORT

void uw_push_fcall(uw_frame_t *fr, val fun, varg args)
{
  memset(fr, 0, sizeof *fr);
  fr->fc.type = UW_FCALL;
  fr->fc.fun = fun;
  fr->fc.args = args;
  fr->fc.up = uw_stack;
  uw_stack = fr;
}

void uw_push_eval(uw_frame_t *fr, val form, val env)
{
  memset(fr, 0, sizeof *fr);
  fr->el.type = UW_EVAL;
  fr->el.form = form;
  fr->el.env = env;
  fr->el.up = uw_stack;
  uw_stack = fr;
}

void uw_push_expand(uw_frame_t *fr, val form, val env)
{
  memset(fr, 0, sizeof *fr);
  fr->el.type = UW_EXPAND;
  fr->el.form = form;
  fr->el.env = env;
  fr->el.up = uw_stack;
  uw_stack = fr;
}

#endif

static val exception_subtypes;

val uw_exception_subtype_p(val sub, val sup)
{
  if (sub == nil || sup == t || sub == sup) {
    return t;
  } else {
    val entry = assoc(sub, exception_subtypes);
    return memq(sup, entry) ? t : nil;
  }
}

static void invoke_handler(uw_frame_t *fr, varg args)
{
  val saved_dyn_env = dyn_env;
  val cur_pkg_alist = deref(cur_package_alist_loc);
  val cur_pkg = cur_package;

  fr->ha.visible = 0;

  uw_simple_catch_begin;

  if (cur_pkg_alist != fr->ha.package_alist || cur_pkg != fr->ha.package) {
    dyn_env = make_env(nil, nil, dyn_env);
    env_vbind(dyn_env, package_s, fr->ha.package);
    env_vbind(dyn_env, package_alist_s, fr->ha.package_alist);
  }

  generic_funcall(fr->ha.fun, args);

  uw_unwind {
    fr->ha.visible = 1;
    dyn_env = saved_dyn_env;
  }

  uw_catch_end;
}

val uw_rthrow(val sym, val args)
{
  uw_frame_t *ex;
  val errorp = uw_exception_subtype_p(sym, error_s);

  if (++reentry_count > 1 && errorp) {
    fprintf(stderr, "txr: invalid re-entry of exception handling logic\n");
    abort();
  }

#if CONFIG_EXTRA_DEBUGGING
  if (uw_break_on_error && errorp)
    breakpt();
#endif

  if (!listp(args))
    args = cons(args, nil);

  for (ex = uw_stack; ex != 0; ex = ex->uw.up) {
    if (ex->uw.type == UW_CATCH && ex->ca.visible) {
      /* The some_satisfy would require us to
         cons up a function; we want to
         avoid consing in exception handling, if we can. */
      val matches = ex->ca.matches;
      val match;
      for (match = matches; match; match = cdr(match))
        if (uw_exception_subtype_p(sym, car(match)))
          break;
      if (match)
        break;
    }
    if (ex->uw.type == UW_HANDLE && ex->ha.visible) {
      val matches = ex->ha.matches;
      val match;
      for (match = matches; match; match = cdr(match))
        if (uw_exception_subtype_p(sym, car(match)))
          break;
      if (match) {
        args_decl_list(gf_args, ARGS_MIN, cons(sym, args));
        --reentry_count;
        invoke_handler(ex, gf_args);
        ++reentry_count;
      }
    }
  }

  if (ex == 0) {
    if (uw_exception_subtype_p(sym, warning_s)) {
      --reentry_count;
      if (uw_exception_subtype_p(sym, defr_warning_s))
        uw_defer_warning(args);
      else if (top_stderr != 0)
        format(top_stderr, lit("~a\n"), car(args), nao);
      if (!opt_compat || opt_compat >= 234) {
        uw_rthrow(continue_s, nil);
        return nil;
      }
      uw_throw(continue_s, nil);
    }

    if (!opt_compat || opt_compat >= 234) {
      if (!errorp) {
        --reentry_count;
        return nil;
      }
    }

    if (top_stderr == 0) {
      fprintf(stderr, "txr: unhandled exception in early initialization\n");
      abort();
    }

    {
      loc pfun = lookup_var_l(nil, unhandled_hook_s);
      val fun = deref(pfun);

      set(pfun, nil);

      if (fun) {
        if (functionp(fun))
          funcall3(fun, sym, args, last_form_evaled);
        else
          format(top_stderr, lit("~a: *unhandled-hook* ~s isn't a function\n"),
                 prog_string, fun, nao);
      }
    }

    ex = &unhandled_ex;
  }

  ex->ca.sym = sym;
  ex->ca.args = args;
  uw_exit_point = ex;
  reentry_count--;
  uw_unwind_to_exit_point();
  abort();
}

val uw_rthrowv(val sym, varg arglist)
{
  return uw_rthrow(sym, args_get_list(arglist));
}

val uw_rthrowfv(val sym, val fmt, varg args)
{
  val stream = make_string_output_stream();
  (void) formatv(stream, fmt, args);
  return uw_rthrow(sym, get_string_from_stream(stream));
  abort();
}

val uw_throw(val sym, val args)
{
  uw_rthrow(sym, args);
  abort();
}

val uw_throwf(val sym, val fmt, ...)
{
  va_list vl;
  val stream = make_string_output_stream();

  va_start (vl, fmt);
  (void) vformat(stream, fmt, vl);
  va_end (vl);

  uw_throw(sym, get_string_from_stream(stream));
  abort();
}

val uw_ethrowf(val sym, val fmt, ...)
{
  va_list vl;
  val eno = num(errno);
  val stream = make_string_output_stream();

  va_start (vl, fmt);
  (void) vformat(stream, fmt, vl);
  va_end (vl);

  uw_throw(sym, string_set_code(get_string_from_stream(stream), eno));
  abort();
}

val uw_errorfv(val fmt, varg args)
{
  val stream = make_string_output_stream();
  (void) formatv(stream, fmt, args);
  uw_throw(error_s, get_string_from_stream(stream));
  abort();
}

val uw_warningf(val fmt, ...)
{
  va_list vl;

  val stream = make_string_output_stream();
  va_start (vl, fmt);
  put_string(lit("warning: "), stream);
  (void) vformat(stream, fmt, vl);
  va_end (vl);

  uw_catch_begin (cons(continue_s, nil), exsym, exvals);

  uw_throw(warning_s, get_string_from_stream(stream));

  uw_catch(exsym, exvals) { (void) exsym; (void) exvals; }

  uw_unwind;

  uw_catch_end;

  return nil;
}

val type_mismatch(val fmt, ...)
{
  va_list vl;
  val stream = make_string_output_stream();

  va_start (vl, fmt);
  (void) vformat(stream, fmt, vl);
  va_end (vl);

  uw_throw(type_error_s, get_string_from_stream(stream));
  abort();
}

NORETURN void invalid_ops(val self, val obj1, val obj2)
{
  uw_throwf(type_error_s, lit("~a: invalid operands ~s ~s"), self,
            obj1, obj2, nao);
}

NORETURN void invalid_op(val self, val obj)
{
  uw_throwf(type_error_s, lit("~a: invalid operand ~s"), self, obj, nao);
}

val uw_defer_warning(val args)
{
  val msg = car(args);
  val tag = cadr(args);
  if (!memqual(tag, tentative_defs))
    push(cons(msg, tag), &deferred_warnings);
  return nil;
}

val uw_warning_exists(val tag)
{
  return member(tag, deferred_warnings, equal_f, cdr_f);
}

val uw_register_tentative_def(val tag)
{
  uw_purge_deferred_warning(tag);
  push(tag, &tentative_defs);
  return nil;
}

val uw_tentative_def_exists(val tag)
{
  return memqual(tag, tentative_defs);
}

val uw_dump_deferred_warnings(val stream)
{
  val wl = nreverse(zap(&deferred_warnings));

  for (; wl; wl = cdr(wl)) {
    val args = car(wl);
    format(stream, lit("~a\n"), car(args), nao);
  }

  return nil;
}

val uw_release_deferred_warnings(void)
{
  val wl = nreverse(zap(&deferred_warnings));

  for (; wl; wl = cdr(wl)) {

    uw_catch_begin (cons(continue_s, nil), exsym, exvals);

    uw_rthrow(warning_s, caar(wl));

    uw_catch(exsym, exvals) { (void) exsym; (void) exvals; }

    uw_unwind;

    uw_catch_end;
  }

  return nil;
}

val uw_purge_deferred_warning(val tag)
{
  deferred_warnings = remqual(tag, deferred_warnings, cdr_f);
  tentative_defs = remqual(tag, tentative_defs, nil);
  return nil;
}

val uw_register_subtype(val sub, val sup)
{
  val t_entry = assoc(t, exception_subtypes);
  val sub_entry = assoc(sub, exception_subtypes);
  val sup_entry = assoc(sup, exception_subtypes);

  assert (t_entry != 0);

  if (sub == nil)
    return sup;

  if (sub == t) {
    if (sup == t)
      return sup;
    uw_throwf(type_error_s,
              lit("cannot define ~s as an exception subtype of ~s"),
              sub, sup, nao);
  }

  if (sup == nil) {
    uw_throwf(type_error_s,
              lit("cannot define ~s as an exception subtype of ~s"),
              sub, sup, nao);
  }

  if (uw_exception_subtype_p(sup, sub))
    uw_throwf(type_error_s, lit("~s is already an exception supertype of ~s"),
              sub, sup, nao);

  /* If sup symbol not registered, then we make it
     an immediate subtype of t. */
  if (!sup_entry) {
    sup_entry = cons(sup, t_entry);
    exception_subtypes = cons(sup_entry, exception_subtypes);
  }

  /* Make sub an immediate subtype of sup.
     If sub already registered, we just repoint it. */
  if (sub_entry) {
    rplacd(sub_entry, sup_entry);
  } else {
    sub_entry = cons(sub, sup_entry);
    exception_subtypes = cons(sub_entry, exception_subtypes);
  }
  return sup;
}

static val register_exception_subtypes(varg args)
{
  val types = args_copy_to_list(args);
  reduce_left(func_n2(uw_register_subtype), types, nil, nil);
  return nil;
}

static val me_defex(val form, val menv)
{
  val types = cdr(form);

  (void) menv;

  if (!all_satisfy(types, func_n1(symbolp), nil))
    eval_error(form, lit("defex: arguments must all be symbols"), nao);

  return cons(intern(lit("register-exception-subtypes"), user_package),
              mapcar(pa_12_2(list_f, quote_s), types));
}

static val exception_subtype_map(void)
{
  return exception_subtypes;
}

void uw_continue(uw_frame_t *cont)
{
  uw_exit_point = cont;
  uw_unwind_to_exit_point();
}

struct cont {
  uw_frame_t *orig;
  cnum size;
  val tag;
  mem_t *stack;
};

static void cont_destroy(val obj)
{
  struct cont *cont = coerce(struct cont *, obj->co.handle);
  free(cont->stack);
  free(cont);
}

static void cont_mark(val obj)
{
  struct cont *cont = coerce(struct cont *, obj->co.handle);
  val *mem = coerce(val *, cont->stack);
  if (mem)
    gc_mark_mem(mem, mem + cont->size / sizeof *mem);
  gc_mark(cont->tag);
}

static struct cobj_ops cont_ops = cobj_ops_init(eq,
                                                cobj_print_op,
                                                cont_destroy,
                                                cont_mark,
                                                cobj_eq_hash_op,
                                                0);

static void call_copy_handlers(uw_frame_t *upto)
{
  uw_frame_t *fr;

  for (fr = uw_stack; fr != 0 && fr != upto; fr = fr->uw.up) {
    if (fr->uw.type == UW_CONT_COPY)
      fr->cp.copy(fr->cp.ptr);
  }
}

static val revive_cont(val dc, val arg)
{
  val self = lit("revive-cont");
  struct cont *cont = coerce(struct cont *, cobj_handle(self, dc, sys_cont_cls));

  if (arg == sys_cont_free_s) {
    free(cont->stack);
    cont->stack = 0;
    return nil;
  } else if (cont->stack) {
    mem_t *space = coerce(mem_t *, alloca(cont->size));
    uint_ptr_t orig_start = coerce(uint_ptr_t, cont->orig);
    uint_ptr_t orig_end = orig_start + cont->size;
    cnum delta = space - coerce(mem_t *, cont->orig);
    mem_t *ptr;
    uw_frame_t *new_uw_stack = coerce(uw_frame_t *, space + UW_CONT_FRAME_BEFORE), *fr;
    int env_set = 0;
    val result = nil;

    memcpy(space, cont->stack, cont->size);

    for (ptr = space; delta && ptr < space + cont->size; ptr += sizeof (cnum))
    {
      uint_ptr_t *wordptr = coerce(uint_ptr_t *, ptr);
      uint_ptr_t word;
#if HAVE_VALGRIND
      uint_ptr_t vbits = 0;

      if (opt_vg_debug) {
        VALGRIND_GET_VBITS(wordptr, &vbits, sizeof *wordptr);
        VALGRIND_MAKE_MEM_DEFINED(wordptr, sizeof *wordptr);
      }
#endif
      word = *wordptr;

      if (word >= orig_start - UW_CONT_FRAME_BEFORE &&
          word <= orig_end && is_ptr(coerce(val, word)))
      {
        *wordptr = word + convert(uint_ptr_t, delta);
      }

#if HAVE_VALGRIND
      if (opt_vg_debug)
        VALGRIND_SET_VBITS(wordptr, &vbits, sizeof *wordptr);
#endif
    }

    uw_block_beg (cont->tag, result);

    for (fr = new_uw_stack; ; fr = fr->uw.up) {
      if (!env_set && fr->uw.type == UW_MENV) {
        uw_menv_stack = fr;
        env_set = 1;
      }
      if (fr->uw.up == 0) {
        bug_unless (fr->uw.type == UW_CAPTURED_BLOCK);
        bug_unless (fr->bl.tag == cont->tag);
        fr->uw.up = uw_stack;
        break;
      }
    }

    uw_stack = new_uw_stack;

    bug_unless (uw_stack->uw.type == UW_BLOCK);

    if (arg != sys_cont_poison_s)
      call_copy_handlers(&uw_blk);

    uw_stack->bl.result = arg;
    uw_exit_point = if3(arg == sys_cont_poison_s, &uw_blk, uw_stack);
    uw_unwind_to_exit_point();
    abort();

    uw_block_end;

    return result;
  } else {
    uw_throwf(error_s, lit("cannot revive freed continuation"), nao);
  }
}

static val capture_cont(val tag, val fun, uw_frame_t *block)
{
  volatile val cont_obj = nil;
  uw_block_begin (nil, result);
  mem_t *stack = coerce(mem_t *, uw_stack) - UW_CONT_FRAME_BEFORE;

  bug_unless (uw_stack < block);

  {
    mem_t *basic_lim = coerce(mem_t *, block + 1) + UW_CONT_FRAME_AFTER;
    mem_t *lim = block->bl.cont_bottom
                 ? (block->bl.cont_bottom > basic_lim
                    ? block->bl.cont_bottom : basic_lim)
                 : basic_lim;
    cnum bloff = coerce(mem_t *, block) - coerce(mem_t *, stack);
    cnum size = coerce(mem_t *, lim) - coerce(mem_t *, stack);
    mem_t *stack_copy = chk_malloc(size);
    uw_frame_t *blcopy = coerce(uw_frame_t *, stack_copy + bloff);
    struct cont *cont = coerce(struct cont *, chk_malloc(sizeof *cont));

    cont->orig = coerce(uw_frame_t *, stack);
    cont->size = size;
    cont->stack = stack_copy;
    cont->tag = nil;

    memcpy(stack_copy, stack, size);

    blcopy->uw.up = 0;
    blcopy->uw.type = UW_CAPTURED_BLOCK;

    cont_obj = cobj(coerce(mem_t *, cont), sys_cont_cls, &cont_ops);

    cont->tag = tag;

    result = nil;
  }

  uw_block_end;

  if (cont_obj) {
    call_copy_handlers(block);
    result = funcall1(fun, func_f1(cont_obj, revive_cont));
  }

  return result;
}

val uw_capture_cont(val tag, val fun, val ctx_form)
{
  uses_or2;
  uw_frame_t *fr;

  for (fr = uw_stack; fr != 0; fr = fr->uw.up) {
    switch (fr->uw.type) {
    case UW_BLOCK:
    case UW_CAPTURED_BLOCK:
      if (fr->bl.tag != tag)
        continue;
      break;
    case UW_GUARD:
      {
        val sym = or2(car(default_null_arg(ctx_form)), sys_capture_cont_s);
        eval_error(ctx_form, lit("~s: cannot capture continuation "
                                 "spanning external library stack frames"),
                   sym, nao);
      }
    default:
      continue;
    }

    break;
  }

  if (!fr) {
    val sym = or2(car(default_null_arg(ctx_form)), sys_capture_cont_s);

    if (tag)
      eval_error(ctx_form, lit("~s: no block ~s is visible"), sym, tag, nao);
    else
      eval_error(ctx_form, lit("~s: no anonymous block is visible"), sym, nao);
    abort();
  }

  return capture_cont(tag, fun, fr);
}

void extjmp_save(extended_jmp_buf *ejb)
{
   ejb->de = dyn_env;
   ejb->gc = gc_enabled;
   ejb->gc_pt = gc_prot_top;
#if HAVE_POSIX_SIGS
   ejb->se = async_sig_enabled;
   ejb->blocked.set = sig_blocked_cache.set;
#endif
#if CONFIG_DEBUG_SUPPORT
   ejb->ds = debug_state;
#endif
}

void extjmp_restore(extended_jmp_buf *ejb)
{
   dyn_env = ejb->de;
   gc_enabled = ejb->gc;
   gc_prot_top = ejb->gc_pt;
#if HAVE_POSIX_SIGS
   async_sig_enabled = ejb->se;
   sig_mask(SIG_SETMASK,
            strip_qual(small_sigset_t *, &ejb->blocked), 0);

#endif
#if CONFIG_DEBUG_SUPPORT
   ejb->ds = debug_state;
#endif
}

void uw_init(void)
{
  protect(&toplevel_env.ev.func_bindings,
          &toplevel_env.ev.match_context,
          &exception_subtypes, convert(val *, 0));
  exception_subtypes = cons(cons(t, nil), exception_subtypes);
  uw_register_subtype(type_error_s, error_s);
  uw_register_subtype(internal_error_s, error_s);
  uw_register_subtype(panic_s, error_s);
  uw_register_subtype(numeric_error_s, error_s);
  uw_register_subtype(range_error_s, error_s);
  uw_register_subtype(query_error_s, error_s);
  uw_register_subtype(file_error_s, error_s);
  uw_register_subtype(process_error_s, error_s);
  uw_register_subtype(system_error_s, error_s);
  uw_register_subtype(alloc_error_s, error_s);
  uw_register_subtype(stack_overflow_s, error_s);
  uw_register_subtype(timeout_error_s, error_s);
  uw_register_subtype(assert_s, error_s);
  uw_register_subtype(syntax_error_s, error_s);
  uw_register_subtype(path_not_found_s, file_error_s);
  uw_register_subtype(path_exists_s, file_error_s);
  uw_register_subtype(path_permission_s, file_error_s);
}

void uw_late_init(void)
{
  protect(&frame_type, &catch_frame_type, &handle_frame_type,
          &deferred_warnings, &tentative_defs,
          &unhandled_ex.ca.sym, &unhandled_ex.ca.args,
          convert(val *, 0));
#if CONFIG_DEBUG_SUPPORT
  protect(&fcall_frame_type, &eval_frame_type, convert(val *, 0));
#endif
  types_s = intern(lit("types"), user_package);
  jump_s = intern(lit("jump"), user_package);
  desc_s = intern(lit("desc"), user_package);
#if CONFIG_DEBUG_SUPPORT
  args_s = intern(lit("args"), user_package);
  form_s = intern(lit("form"), user_package);
#endif
  sys_cont_s = intern(lit("cont"), system_package);
  sys_cont_poison_s = intern(lit("cont-poison"), system_package);
  sys_cont_free_s = intern(lit("cont-free"), system_package);
  catch_frame_s = intern(lit("catch-frame"), user_package);

  sys_cont_cls = cobj_register(sys_cont_s);

  frame_type = make_struct_type(intern(lit("frame"), user_package),
                                nil, nil, nil, nil, nil, nil, nil);
  catch_frame_type = make_struct_type(catch_frame_s,
                                      frame_type, nil,
                                      list(types_s, desc_s, jump_s, nao),
                                      nil, nil, nil, nil);
  handle_frame_type = make_struct_type(intern(lit("handle-frame"),
                                              user_package),
                                       frame_type, nil,
                                       list(types_s, fun_s, nao),
                                       nil, nil, nil, nil);
#if CONFIG_DEBUG_SUPPORT
  fcall_frame_type = make_struct_type(intern(lit("fcall-frame"), user_package),
                                      frame_type, nil,
                                      list(fun_s, args_s, nao),
                                      nil, nil, nil, nil);
  eval_frame_type = make_struct_type(intern(lit("eval-frame"), user_package),
                                     frame_type, nil,
                                     list(form_s, env_s, nao),
                                     nil, nil, nil, nil);
  expand_frame_type = make_struct_type(intern(lit("expand-frame"), user_package),
                                       frame_type, nil,
                                       list(form_s, env_s, nao),
                                       nil, nil, nil, nil);
#endif
  reg_mac(intern(lit("defex"), user_package), func_n2(me_defex));
  reg_var(unhandled_hook_s = intern(lit("*unhandled-hook*"),
          user_package), nil);
  reg_fun(throw_s, func_n1v(uw_rthrowv));
  reg_fun(intern(lit("throwf"), user_package), func_n2v(uw_rthrowfv));
  reg_fun(error_s, func_n1v(uw_errorfv));
  reg_fun(intern(lit("purge-deferred-warning"), user_package),
          func_n1(uw_purge_deferred_warning));
  reg_fun(intern(lit("register-tentative-def"), user_package), func_n1(uw_register_tentative_def));
  reg_fun(intern(lit("tentative-def-exists"), user_package), func_n1(uw_tentative_def_exists));
  reg_fun(intern(lit("defer-warning"), user_package), func_n1(uw_defer_warning));
  reg_fun(intern(lit("dump-deferred-warnings"), user_package), func_n1(uw_dump_deferred_warnings));
  reg_fun(intern(lit("release-deferred-warnings"), user_package), func_n0(uw_release_deferred_warnings));
  reg_fun(intern(lit("register-exception-subtypes"), user_package),
          func_n0v(register_exception_subtypes));
  reg_fun(intern(lit("exception-subtype-p"), user_package),
          func_n2(uw_exception_subtype_p));
  reg_fun(intern(lit("exception-subtype-map"), user_package), func_n0(exception_subtype_map));
  reg_fun(intern(lit("get-frames"), user_package), func_n0(uw_get_frames));
  reg_fun(intern(lit("find-frame"), user_package), func_n2o(uw_find_frame, 0));
  reg_fun(intern(lit("find-frames"), user_package), func_n2o(uw_find_frames, 0));
  reg_fun(intern(lit("invoke-catch"), user_package),
          func_n2v(uw_invoke_catch));
  reg_fun(sys_capture_cont_s = intern(lit("capture-cont"), system_package),
          func_n3o(uw_capture_cont, 2));
#if CONFIG_DEBUG_SUPPORT
  reg_varl(intern(lit("uw-block"), system_package), num_fast(1U << UW_BLOCK));
  reg_varl(intern(lit("uw-captured-block"), system_package), num_fast(1U << UW_CAPTURED_BLOCK));
  reg_varl(intern(lit("uw-menv"), system_package), num_fast(1U <<UW_MENV));
  reg_varl(intern(lit("uw-catch"), system_package), num_fast(1U <<UW_CATCH));
  reg_varl(intern(lit("uw-handle"), system_package), num_fast(1U <<UW_HANDLE));
  reg_varl(intern(lit("uw-cont-copy"), system_package), num_fast(1U <<UW_CONT_COPY));
  reg_varl(intern(lit("uw-guard"), system_package), num_fast(1U <<UW_GUARD));
  reg_varl(intern(lit("uw-fcall"), system_package), num_fast(1U <<UW_FCALL));
  reg_varl(intern(lit("uw-eval"), system_package), num_fast(1U <<UW_EVAL));
  reg_varl(intern(lit("uw-expand"), system_package), num_fast(1U <<UW_EXPAND));
  reg_fun(intern(lit("find-frames-by-mask"), user_package), func_n1(uw_find_frames_by_mask));
#endif
  uw_register_subtype(continue_s, restart_s);
  uw_register_subtype(intern(lit("retry"), user_package), restart_s);
  uw_register_subtype(skip_s, restart_s);
  uw_register_subtype(warning_s, t);
  uw_register_subtype(defr_warning_s, warning_s);
}
