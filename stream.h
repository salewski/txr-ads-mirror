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

enum strm_whence {
  strm_start = SEEK_SET,
  strm_cur = SEEK_CUR,
  strm_end = SEEK_END
};

enum indent_mode {
  indent_off,
  indent_data,
  indent_code,
  indent_foff
};

struct strm_ctx {
  val obj_hash;
  val obj_hash_prev;
  val counter;
  cnum depth;
  struct strm_base *strm;
};

struct strm_base {
  enum indent_mode indent_mode;
  cnum data_width;
  cnum code_width;
  cnum indent_chars;
  cnum column;
  unsigned force_break;
  cnum max_length;
  cnum max_depth;
  val close_result;
  struct strm_ctx *ctx;
};

struct strm_ops {
  struct cobj_ops cobj_ops;
  const wchli_t *name;
  val (*put_string)(val, val);
  val (*put_char)(val, val);
  val (*put_byte)(val, int);
  val (*get_line)(val);
  val (*get_char)(val);
  val (*get_byte)(val);
  val (*unget_char)(val, val);
  val (*unget_byte)(val, int);
  ucnum (*put_buf)(val, mem_t *, ucnum len, ucnum pos);
  ucnum (*fill_buf)(val, mem_t *, ucnum len, ucnum pos);
  val (*close)(val, val);
  val (*flush)(val);
  val (*seek)(val, val, enum strm_whence);
  val (*truncate)(val, val);
  val (*get_prop)(val, val ind);
  val (*set_prop)(val, val ind, val);
  val (*get_error)(val);
  val (*get_error_str)(val);
  val (*clear_error)(val);
  val (*get_fd)(val);
  val (*get_sock_family)(val);
  val (*get_sock_type)(val);
  val (*get_sock_peer)(val);
  val (*set_sock_peer)(val, val);
};

#define strm_ops_init(cobj_init_macro, name, put_string, put_char, put_byte, \
                      get_line, get_char, get_byte, unget_char, unget_byte,  \
                      put_buf, fill_buf,                                     \
                      close, flush, seek, truncate, get_prop, set_prop,      \
                      get_error, get_error_str, clear_error, get_fd)         \
{                                                                            \
    cobj_init_macro, name, put_string, put_char, put_byte, get_line,         \
    get_char, get_byte, unget_char, unget_byte, put_buf, fill_buf,           \
    close, flush, seek, truncate, get_prop, set_prop,                        \
    get_error, get_error_str, clear_error, get_fd, 0, 0, 0, 0                \
}

#define STDIO_MODE_NREDIRS 4

struct stdio_mode {
  unsigned malformed : 1;
  unsigned read : 1;
  unsigned write : 1;
  unsigned create : 1;
  unsigned append : 1;
  unsigned binary : 1;
  unsigned notrunc : 1;
  unsigned excl : 1;
  unsigned nonblock : 1;
  unsigned interactive : 1;
  unsigned unbuf : 1;
  unsigned linebuf : 1;
  unsigned gzip : 1;
  unsigned gzlevel : 4;
  unsigned tmpfile : 1;
  int buforder : 5;
  int redir[STDIO_MODE_NREDIRS][2];
  int streamfd;
};

#define stdio_mode_init_blank { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, { { 0 } }, -1 }
#define stdio_mode_init_r     { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, { { 0 } }, -1 }
#define stdio_mode_init_rpb   { 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, { { 0 } }, -1 }

#define std_input (deref(lookup_var_l(nil, stdin_s)))
#define std_output (deref(lookup_var_l(nil, stdout_s)))
#define std_debug (deref(lookup_var_l(nil, stddebug_s)))
#define std_error (deref(lookup_var_l(nil, stderr_s)))
#define std_null (deref(lookup_var_l(nil, stdnull_s)))

enum json_fmt {
  json_fmt_default,
  json_fmt_standard
};

struct json_opts {
  enum json_fmt fmt : 4;
  unsigned flat : 1;
};

loc lookup_var_l(val env, val sym);

extern val from_start_k, from_current_k, from_end_k;
extern val real_time_k, name_k, addr_k, fd_k, byte_oriented_k;
extern val standard_k;
extern val format_s;

extern val stdio_stream_s;

extern val stdin_s, stdout_s, stddebug_s, stderr_s, stdnull_s;

extern val put_string_s, put_char_s, put_byte_s, get_line_s, get_char_s;
extern val get_byte_s, unget_char_s, unget_byte_s, put_buf_s, fill_buf_s;
extern val close_s, flush_s, seek_s, truncate_s, get_prop_s, set_prop_s;
extern val get_error_s, get_error_str_s, clear_error_s, get_fd_s;

extern val print_flo_precision_s, print_flo_digits_s, print_flo_format_s;
extern val pprint_flo_format_s, print_base_s, print_circle_s;
extern val print_json_format_s;

#if HAVE_SOCKETS
extern val socket_error_s;
#endif

extern const wchli_t *path_sep_chars;
extern wchar_t path_var_sep_char;

extern val top_stderr;

extern struct cobj_class *stream_cls, *stdio_stream_cls;

void strm_base_init(struct strm_base *s);
void strm_base_cleanup(struct strm_base *s);
void strm_base_mark(struct strm_base *s);
void fill_stream_ops(struct strm_ops *ops);
void stream_print_op(val stream, val out, val pretty, struct strm_ctx *);
void stream_mark_op(val stream);
void stream_destroy_op(val stream);
int w_open_mode(const wchar_t *wname, const struct stdio_mode m);
struct stdio_mode parse_mode(val mode_str, struct stdio_mode m_dfl, val self);
val normalize_mode(struct stdio_mode *m, val mode_str,
                   struct stdio_mode m_dfl, val self);
val normalize_mode_no_bin(struct stdio_mode *m, val mode_str,
                          struct stdio_mode m_dfl, val self);
val set_mode_props(const struct stdio_mode m, val stream);
val generic_get_line(val stream);
val errno_to_string(val err);
val make_null_stream(void);
val make_stdio_stream(FILE *, val descr);
val make_tail_stream(FILE *, val descr);
val pipe_close_status_helper(val stream, val throw_on_error,
                             int status, val self);
val stream_fd(val stream);
#if HAVE_SOCKETS
val make_sock_stream(FILE *f, val family, val type);
val sock_family(val stream);
val sock_type(val stream);
val sock_peer(val stream);
val sock_set_peer(val stream, val peer);
#endif
val make_string_input_stream(val);
val make_string_byte_input_stream(val);
val make_strlist_input_stream(val);
val make_string_output_stream(void);
val get_string_from_stream(val);
val make_strlist_output_stream(void);
val get_list_from_stream(val);
val record_adapter(val regex, val stream, val include_match);
val streamp(val obj);
val real_time_stream_p(val obj);
val stream_set_prop(val stream, val ind, val prop);
val stream_get_prop(val stream, val ind);
val close_stream(val stream, val throw_on_error);
val get_error(val stream);
val get_error_str(val stream);
val clear_error(val stream);
val get_line(val);
val get_char(val);
val get_byte(val);
val get_bytes(val self, val, mem_t *ptr, ucnum len);
val unget_char(val ch, val stream);
val unget_byte(val byte, val stream);
val put_buf(val buf, val pos, val stream);
val fill_buf(val buf, val pos, val stream);
val fill_buf_adjust(val buf, val pos, val stream);
val get_line_as_buf(val stream);
val vformat(val stream, val string, va_list);
val vformat_to_string(val string, va_list);
val format(val stream, val string, ...);
val formatv(val stream, val string, struct args *args);
val fmt(val string, struct args *args);
val put_string(val string, val stream);
val put_line(val string, val stream);
val put_char(val ch, val stream);
val put_byte(val byte, val stream);
val put_strings(val strings, val stream);
val put_lines(val lines, val stream);
val flush_stream(val stream);
val seek_stream(val stream, val offset, val whence);
val truncate_stream(val stream, val len);
val get_indent_mode(val stream);
val test_set_indent_mode(val stream, val compare, val mode);
val test_neq_set_indent_mode(val stream, val compare, val mode);
val set_indent_mode(val stream, val mode);
val get_indent(val stream);
val set_indent(val stream, val indent);
val inc_indent(val stream, val delta);
val inc_indent_abs(val stream, val delta);
val width_check(val stream, val alt);
val force_break(val stream);
val set_max_length(val stream, val length);
val set_max_depth(val stream, val depth);
struct strm_ctx *get_set_ctx(val stream, struct strm_ctx *);
struct strm_ctx *get_ctx(val stream);
val get_string(val stream, val nchars, val close_after_p);
val open_directory(val path);
val open_file(val path, val mode_str);
val open_fileno(val fd, val mode_str, val pid_opt);
val open_tail(val path, val mode_str, val seek_end_p);
val open_command(val path, val mode_str);
val path_search(val name, val path_in);
val open_process(val path, val mode_str, val args);
val make_catenated_stream(val stream_list);
val make_catenated_stream_v(struct args *streams);
val catenated_stream_p(val obj);
val catenated_stream_push(val new_stream, val cat_stream);
val remove_path(val path, val throw_on_error);
val rename_path(val from, val to);
val portable_abs_path_p(val path);
val abs_path_p(val path);
val pure_rel_path_p(val path);
val base_name(val path, val suff);
val dir_name(val path);
val short_suffix(val name, val alt_in);
val long_suffix(val name, val alt_in);
val trim_short_suffix(val name);
val trim_long_suffix(val name);
val trim_path_seps(val name);
val path_cat(val dir_name, val base_name);
val add_suffix(val name, val suffix);
val make_byte_input_stream(val obj);
val iobuf_get(void);
void iobuf_put(val buf);
void iobuf_list_empty(void);
val tmpfile_wrap(void);
val mkdtemp_wrap(val prefix);
val mkstemp_wrap(val prefix, val suffix);
void stream_init(void);
void stream_compat_fixup(int compat_ver);
