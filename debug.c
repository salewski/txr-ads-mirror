#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <setjmp.h>
#include <stdarg.h>
#include <wchar.h>
#include "config.h"
#include "lib.h"
#include "debug.h"
#include "gc.h"
#include "unwind.h"
#include "stream.h"
#include "parser.h"

int opt_debugger;
int debug_depth;
val debug_block_s;
static int step_mode;
static int next_depth = -1;
val breakpoints;
val last_command;

static void help(val stream)
{
  put_string(stream,
             lit("commands:\n"
                 "? - help                               s - step into form\n"
                 "h - help                               n - step over form\n"
                 "c - continue                           f - finish form\n"
                 "v - show variable binding environment  s - show current form\n"
                 "b - set breakpoint by line number      i - show current data\n"
                 "d - delete breakpoint                  w - backtrace\n"
                 "l - list breakpoints\n"));
}

static void show_bindings(val env, val stream)
{
  val level = zero;
  put_string(stream, lit("bindings:\n"));

  for (;; level = plus(level, one)) {
    if (nullp(env))
      break;
    else if (consp(env)) {
      format(stream, lit("~s: ~s\n"), level, env, nao);
      break;
    } else if (type(env) == ENV) {
      format(stream, lit("~s: ~s\n"), level, env->e.vbindings, nao);
      env = env->e.up_env;
    } else {
      format(stream, lit("invalid environment object: ~s\n"), env, nao);
      break;
    }
  }
}

val debug(val form, val bindings, val data, val line, val pos, val base)
{
  uses_or2;
  val lineno = source_loc(form);

  if (!step_mode && !memqual(lineno, breakpoints) 
      && (debug_depth > next_depth))
  {
    return nil;
  } else {
    val print_form = t;
    val print_data = t;

    for (;;) {
      val input, command;

      if (print_form) {
        format(std_output, lit("stopped at line ~a\n"), lineno, nao);
        format(std_output, lit("form: ~s\n"), form, nao);
        format(std_output, lit("depth: ~s\n"), num(debug_depth), nao);
        print_form = nil;
      }

      if (print_data) {
        if (data && pos) {
          val prefix = sub_str(data, zero, pos);
          val suffix = sub_str(data, pos, nil);

          format(std_output, lit("data (~s:~s):\n~s . ~s\n"),
                 line, plus(pos, base), prefix, suffix, nao);
        } else {
          format(std_output, lit("data (~s):\n~s\n"), line, data, nao);
        }
        print_data = nil;
      }

      format(std_output, lit("txr> "), nao);
      flush_stream(std_output);

      input = split_str_set(or2(get_line(std_input), lit("q")), lit("\t "));
      command = if3(equal(first(input), null_string), 
                    or2(last_command, lit("")), first(input));
      last_command = command;

      if (equal(command, lit("?")) || equal(command, lit("h"))) {
        help(std_output);
        continue;
      } else if (equal(command, null_string)) {
        continue;
      } else if (equal(command, lit("c"))) {
        step_mode = 0;
        next_depth = -1;
        return nil;
      } else if (equal(command, lit("s"))) {
        step_mode = 1;
        return nil;
      } else if (equal(command, lit("n"))) {
        step_mode = 0;
        next_depth = debug_depth;
        return nil;
      } else if (equal(command, lit("f"))) {
        step_mode = 0;
        next_depth = debug_depth - 1;
        return nil;
      } else if (equal(command, lit("v"))) {
        show_bindings(bindings, std_output);
      } else if (equal(command, lit("s"))) {
        print_form = t;
      } else if (equal(command, lit("i"))) {
        print_data = t;
      } else if (equal(command, lit("b")) || equal(command, lit("d"))) {
        if (!rest(input)) {
          format(std_output, lit("b needs argument\n"), nao);
          continue;
        } else {
          long n = wcstol(c_str(second(input)), NULL, 10);
          if (equal(command, lit("b")))
            push(num(n), &breakpoints);
        }
      } else if (equal(command, lit("l"))) {
        format(std_output, lit("breakpoints: ~s\n"), breakpoints, nao);
      } else if (equal(command, lit("w"))) {
        format(std_output, lit("backtrace:\n"), nao);
        {
          uw_frame_t *iter;

          for (iter = uw_current_frame(); iter != 0; iter = iter->uw.up) {
            if (iter->uw.type == UW_DBG) {
              if (iter->db.ub_p_a_pairs)
                format(std_output, lit("(~s ~s ~s)\n"), iter->db.func,
                       iter->db.args, iter->db.ub_p_a_pairs, nao);
              else
                format(std_output, lit("(~s ~s)\n"), iter->db.func,
                       iter->db.args, nao);
            }
          }
        }
      } else if (equal(command, lit("q"))) {
        uw_throwf(query_error_s, lit("terminated via debugger"), nao);
      } else {
        format(std_output, lit("unrecognized command: ~a\n"), command, nao);
      }
    }

    return nil;
  }
}

void debug_init(void)
{
  step_mode = 1;
  protect(&breakpoints, &last_command, (val *) 0);
  debug_block_s = intern(lit("debug-block"), system_package);
}
