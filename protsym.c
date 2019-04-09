/* This file is generated by genprotsym.txr */

/* Copyright 2009-2019
 * Kaz Kylheku <kaz@kylheku.com>
 * Vancouver, Canada
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include "config.h"
#include "lib.h"

extern val abs_s, accept_s, acos_s, addr_k, align_s;
extern val all_s, alloc_error_s, and_s, append_each_s, append_each_star_s;
extern val append_k, append_s, apply_s, args_k, array_s;
extern val ash_s, asin_s, assert_s, atan2_s, atan_s;
extern val atime_k, atime_s, atom_s, auto_k, bchar_s;
extern val be_double_s, be_float_s, be_int16_s, be_int32_s, be_int64_s;
extern val be_uint16_s, be_uint32_s, be_uint64_s, bignum_s, bind_s;
extern val bit_s, blksize_k, blksize_s, block_s, block_star_s;
extern val blocks_k, blocks_s, bool_s, bstr_d_s, bstr_s;
extern val buf_d_s, buf_s, byte_oriented_k, call_s, car_s;
extern val carray_s, caseq_s, caseq_star_s, caseql_s, caseql_star_s;
extern val casequal_s, casequal_star_s, cases_s, cat_s, catch_s;
extern val cdigit_k, cdr_s, ceil1_s, ceil_s, char_s;
extern val chars_k, choose_s, chr_s, chset_s, circref_s;
extern val clear_error_s, close_s, closure_s, cobj_s, coll_s;
extern val collect_each_s, collect_each_star_s, collect_s, colon_k, compile_only_s;
extern val compl_s, compound_s, cond_s, cons_s, continue_k;
extern val continue_s, cos_s, counter_k, cptr_s, cset_s;
extern val cspace_k, ctime_k, ctime_s, cword_char_k, data_s;
extern val day_s, decline_k, defex_s, deffilter_s, define_s;
extern val defmacro_s, defparm_s, defparml_s, defr_warning_s, defsymacro_s;
extern val defun_s, defvar_s, defvarl_s, dev_k, dev_s;
extern val digit_k, div_s, do_s, dohash_s, double_s;
extern val downcase_k, dst_s, dvbind_s, dwim_s, each_op_s;
extern val each_s, each_star_s, elif_s, else_s, empty_s;
extern val enum_s, enumed_s, env_k, env_s, eof_s;
extern val eol_s, eq_s, eql_based_k, eql_s, equal_based_k;
extern val equal_s, error_s, eval_error_s, eval_only_s, evenp_s;
extern val exp_s, expr_s, expt_s, exptmod_s, fail_s;
extern val fbind_s, fd_k, ffi_call_desc_s, ffi_closure_s, ffi_type_s;
extern val file_error_s, fill_buf_s, filter_k, filter_s, filters_s;
extern val finally_s, finish_k, first_s, fixnum_s, flatten_s;
extern val flet_s, float_s, floor1_s, floor_s, flush_s;
extern val for_op_s, for_s, for_star_s, force_s, forget_s;
extern val form_k, format_s, freeform_s, from_current_k, from_end_k;
extern val from_list_s, from_start_k, frombase64_k, fromhtml_k, frompercent_k;
extern val fromurl_k, fun_k, fun_s, fuzz_s, gap_k;
extern val gather_s, ge_s, gen_s, generate_s, gensym_counter_s;
extern val get_byte_s, get_char_s, get_error_s, get_error_str_s, get_fd_s;
extern val get_line_s, get_prop_s, gid_k, gid_s, gmtoff_s;
extern val greedy_k, gt_s, gun_s, handler_bind_s, hash_construct_s;
extern val hash_iter_s, hash_lit_s, hash_s, hash_seed_s, hextoint_k;
extern val hour_s, iapply_s, identity_s, if_s, iflet_s;
extern val in_package_s, inc_s, include_s, init_k, ino_k;
extern val ino_s, int16_s, int32_s, int64_s, int8_s;
extern val int_s, integer_s, internal_error_s, into_k, intr_s;
extern val inv_div_s, inv_minus_s, isqrt_s, keyword_package_s, labels_s;
extern val lambda_s, lambda_set_s, last_s, lbind_s, lcons_s;
extern val le_double_s, le_float_s, le_int16_s, le_int32_s, le_int64_s;
extern val le_s, le_uint16_s, le_uint32_s, le_uint64_s, length_s;
extern val let_s, let_star_s, lfilt_k, line_s, lines_k;
extern val list_k, list_s, list_star_s, listener_greedy_eval_s, listener_hist_len_s;
extern val listener_multi_line_p_s, listener_pprint_s, listener_sel_inclusive_p_s, lists_k, lit_s;
extern val load_path_s, load_recursive_s, load_s, load_time_lit_s, load_time_s;
extern val local_s, log10_s, log2_s, log_s, logand_s;
extern val logcount_s, logior_s, lognot1_s, lognot_s, logtrunc_s;
extern val logxor_s, long_s, longest_k, lstr_s, lt_s;
extern val mac_param_bind_s, macro_s, macro_time_s, macrolet_s, make_struct_lit_s;
extern val mandatory_k, maxgap_k, maxtimes_k, maybe_s, mdo_s;
extern val memq_s, memql_s, memqual_s, merge_s, meth_s;
extern val min_s, mingap_k, mintimes_k, minus_s, minusp_s;
extern val mod_s, mode_k, mode_s, modlast_s, month_s;
extern val mtime_k, mtime_s, mul_s, name_k, name_s;
extern val named_k, neg_s, next_s, next_spec_k, nlink_k;
extern val nlink_s, none_s, nongreedy_s, not_s, nothrow_k;
extern val noval_s, null_s, nullify_s, number_s, numeq_s;
extern val numeric_error_s, oddp_s, oneplus_s, op_s, optional_s;
extern val or_s, output_s, package_alist_s, package_s, panic_s;
extern val parser_s, path_s, pkg_s, plus_s, plusp_s;
extern val postinit_k, pprint_flo_format_s, print_base_s, print_circle_s, print_flo_digits_s;
extern val print_flo_format_s, print_flo_precision_s, print_s, process_error_s, prof_s;
extern val prog1_s, progn_s, promise_forced_s, promise_inprogress_s, promise_s;
extern val ptr_in_d_s, ptr_in_s, ptr_out_d_s, ptr_out_s, ptr_out_s_s;
extern val ptr_s, put_buf_s, put_byte_s, put_char_s, put_string_s;
extern val qquote_s, qref_s, quasi_s, quasilist_s, query_error_s;
extern val quote_s, r_atan2_s, r_ceil_s, r_expt_s, r_floor_s;
extern val r_lognot_s, r_logtrunc_s, r_mod_s, r_round_s, r_trunc_s;
extern val random_state_s, random_state_var_s, random_warmup_s, range_error_s, range_s;
extern val rcons_s, rdev_k, rdev_s, real_time_k, rebind_s;
extern val rec_source_loc_s, recip_s, reflect_k, regex_s, rep_s;
extern val repeat_s, repeat_spec_k, require_s, resolve_k, rest_s;
extern val restart_s, return_from_s, return_s, rfilt_k, round1_s;
extern val round_s, rplaca_s, rplacd_s, sbit_s, sec_s;
extern val seek_s, seq_iter_s, sequence_s, set_prop_s, set_s;
extern val setq_s, setqf_s, short_s, shortest_k, sign_extend_s;
extern val signum_s, sin_s, single_s, size_k, size_s;
extern val skip_s, slot_s, some_s, space_k, special_s;
extern val splice_s, sqrt_s, square_s, stat_s, stddebug_s;
extern val stderr_s, stdin_s, stdio_stream_s, stdnull_s, stdout_s;
extern val str_d_s, str_s, stream_s, string_k, string_s;
extern val struct_lit_s, struct_s, struct_type_s, switch_s, sym_s;
extern val symacro_k, symacrolet_s, syntax_error_s, sys_abscond_from_s, sys_apply_s;
extern val sys_catch_s, sys_l1_setq_s, sys_l1_val_s, sys_lisp1_setq_s, sys_lisp1_value_s;
extern val sys_mark_special_s, sys_qquote_s, sys_splice_s, sys_unquote_s, system_error_s;
extern val system_package_s, tan_s, text_s, throw_s, time_local_s;
extern val time_parse_s, time_s, time_string_s, time_utc_s, timeout_error_s;
extern val times_k, tlist_k, tobase64_k, tofloat_k, tohtml_k;
extern val tohtml_star_k, toint_k, tonumber_k, topercent_k, tourl_k;
extern val trailer_s, tree_bind_s, tree_case_s, trunc1_s, trunc_s;
extern val truncate_s, try_s, type_error_s, ubit_s, uchar_s;
extern val uid_k, uid_s, uint16_s, uint32_s, uint64_s;
extern val uint8_s, uint_s, ulong_s, unbound_s, unget_byte_s;
extern val unget_char_s, union_s, unique_s, unquote_s, until_s;
extern val until_star_s, upcase_k, uref_s, user_package_s, userdata_k;
extern val ushort_s, usr_var_s, uw_protect_s, val_s, var_k;
extern val var_s, vars_k, vec_list_s, vec_s, vecref_s;
extern val vector_lit_s, vm_closure_s, vm_desc_s, void_s, warning_s;
extern val wchar_s, weak_keys_k, weak_vals_k, when_s, while_s;
extern val while_star_s, whole_k, width_s, wild_s, word_char_k;
extern val wrap_k, wstr_d_s, wstr_s, year_s, zap_s;
extern val zarray_s, zerop_s, zeroplus_s, zone_s;

#if CONFIG_DEBUG_SUPPORT
extern val debug_quit_s;
#endif
#if HAVE_DLOPEN
extern val dlhandle_s, dlsym_s;
#endif
#if HAVE_GRGID
extern val group_s, mem_s;
#endif
#if HAVE_PWUID
extern val dir_s, gecos_s, passwd_s, shell_s;
#endif
#if HAVE_SOCKETS
extern val addr_s, addrinfo_s, canonname_s, family_s, flags_s;
extern val flow_info_s, port_s, protocol_s, scope_id_s, sockaddr_in6_s;
extern val sockaddr_in_s, sockaddr_un_s, socket_error_s, socktype_s;
#endif
#if HAVE_SYSLOG
extern val prio_k;
#endif
#if HAVE_TERMIOS
extern val cc_s, cflag_s, iflag_s, ispeed_s, lflag_s;
extern val oflag_s, ospeed_s, termios_s;
#endif
#if HAVE_UNAME
extern val domainname_s, machine_s, nodename_s, release_s, sysname_s;
extern val utsname_s, version_s;
#endif

val *protected_sym[] = {
  &abs_s, &accept_s, &acos_s, &addr_k, &align_s,
  &all_s, &alloc_error_s, &and_s, &append_each_s, &append_each_star_s,
  &append_k, &append_s, &apply_s, &args_k, &array_s,
  &ash_s, &asin_s, &assert_s, &atan2_s, &atan_s,
  &atime_k, &atime_s, &atom_s, &auto_k, &bchar_s,
  &be_double_s, &be_float_s, &be_int16_s, &be_int32_s, &be_int64_s,
  &be_uint16_s, &be_uint32_s, &be_uint64_s, &bignum_s, &bind_s,
  &bit_s, &blksize_k, &blksize_s, &block_s, &block_star_s,
  &blocks_k, &blocks_s, &bool_s, &bstr_d_s, &bstr_s,
  &buf_d_s, &buf_s, &byte_oriented_k, &call_s, &car_s,
  &carray_s, &caseq_s, &caseq_star_s, &caseql_s, &caseql_star_s,
  &casequal_s, &casequal_star_s, &cases_s, &cat_s, &catch_s,
  &cdigit_k, &cdr_s, &ceil1_s, &ceil_s, &char_s,
  &chars_k, &choose_s, &chr_s, &chset_s, &circref_s,
  &clear_error_s, &close_s, &closure_s, &cobj_s, &coll_s,
  &collect_each_s, &collect_each_star_s, &collect_s, &colon_k, &compile_only_s,
  &compl_s, &compound_s, &cond_s, &cons_s, &continue_k,
  &continue_s, &cos_s, &counter_k, &cptr_s, &cset_s,
  &cspace_k, &ctime_k, &ctime_s, &cword_char_k, &data_s,
  &day_s, &decline_k, &defex_s, &deffilter_s, &define_s,
  &defmacro_s, &defparm_s, &defparml_s, &defr_warning_s, &defsymacro_s,
  &defun_s, &defvar_s, &defvarl_s, &dev_k, &dev_s,
  &digit_k, &div_s, &do_s, &dohash_s, &double_s,
  &downcase_k, &dst_s, &dvbind_s, &dwim_s, &each_op_s,
  &each_s, &each_star_s, &elif_s, &else_s, &empty_s,
  &enum_s, &enumed_s, &env_k, &env_s, &eof_s,
  &eol_s, &eq_s, &eql_based_k, &eql_s, &equal_based_k,
  &equal_s, &error_s, &eval_error_s, &eval_only_s, &evenp_s,
  &exp_s, &expr_s, &expt_s, &exptmod_s, &fail_s,
  &fbind_s, &fd_k, &ffi_call_desc_s, &ffi_closure_s, &ffi_type_s,
  &file_error_s, &fill_buf_s, &filter_k, &filter_s, &filters_s,
  &finally_s, &finish_k, &first_s, &fixnum_s, &flatten_s,
  &flet_s, &float_s, &floor1_s, &floor_s, &flush_s,
  &for_op_s, &for_s, &for_star_s, &force_s, &forget_s,
  &form_k, &format_s, &freeform_s, &from_current_k, &from_end_k,
  &from_list_s, &from_start_k, &frombase64_k, &fromhtml_k, &frompercent_k,
  &fromurl_k, &fun_k, &fun_s, &fuzz_s, &gap_k,
  &gather_s, &ge_s, &gen_s, &generate_s, &gensym_counter_s,
  &get_byte_s, &get_char_s, &get_error_s, &get_error_str_s, &get_fd_s,
  &get_line_s, &get_prop_s, &gid_k, &gid_s, &gmtoff_s,
  &greedy_k, &gt_s, &gun_s, &handler_bind_s, &hash_construct_s,
  &hash_iter_s, &hash_lit_s, &hash_s, &hash_seed_s, &hextoint_k,
  &hour_s, &iapply_s, &identity_s, &if_s, &iflet_s,
  &in_package_s, &inc_s, &include_s, &init_k, &ino_k,
  &ino_s, &int16_s, &int32_s, &int64_s, &int8_s,
  &int_s, &integer_s, &internal_error_s, &into_k, &intr_s,
  &inv_div_s, &inv_minus_s, &isqrt_s, &keyword_package_s, &labels_s,
  &lambda_s, &lambda_set_s, &last_s, &lbind_s, &lcons_s,
  &le_double_s, &le_float_s, &le_int16_s, &le_int32_s, &le_int64_s,
  &le_s, &le_uint16_s, &le_uint32_s, &le_uint64_s, &length_s,
  &let_s, &let_star_s, &lfilt_k, &line_s, &lines_k,
  &list_k, &list_s, &list_star_s, &listener_greedy_eval_s, &listener_hist_len_s,
  &listener_multi_line_p_s, &listener_pprint_s, &listener_sel_inclusive_p_s, &lists_k, &lit_s,
  &load_path_s, &load_recursive_s, &load_s, &load_time_lit_s, &load_time_s,
  &local_s, &log10_s, &log2_s, &log_s, &logand_s,
  &logcount_s, &logior_s, &lognot1_s, &lognot_s, &logtrunc_s,
  &logxor_s, &long_s, &longest_k, &lstr_s, &lt_s,
  &mac_param_bind_s, &macro_s, &macro_time_s, &macrolet_s, &make_struct_lit_s,
  &mandatory_k, &maxgap_k, &maxtimes_k, &maybe_s, &mdo_s,
  &memq_s, &memql_s, &memqual_s, &merge_s, &meth_s,
  &min_s, &mingap_k, &mintimes_k, &minus_s, &minusp_s,
  &mod_s, &mode_k, &mode_s, &modlast_s, &month_s,
  &mtime_k, &mtime_s, &mul_s, &name_k, &name_s,
  &named_k, &neg_s, &next_s, &next_spec_k, &nlink_k,
  &nlink_s, &none_s, &nongreedy_s, &not_s, &nothrow_k,
  &noval_s, &null_s, &nullify_s, &number_s, &numeq_s,
  &numeric_error_s, &oddp_s, &oneplus_s, &op_s, &optional_s,
  &or_s, &output_s, &package_alist_s, &package_s, &panic_s,
  &parser_s, &path_s, &pkg_s, &plus_s, &plusp_s,
  &postinit_k, &pprint_flo_format_s, &print_base_s, &print_circle_s, &print_flo_digits_s,
  &print_flo_format_s, &print_flo_precision_s, &print_s, &process_error_s, &prof_s,
  &prog1_s, &progn_s, &promise_forced_s, &promise_inprogress_s, &promise_s,
  &ptr_in_d_s, &ptr_in_s, &ptr_out_d_s, &ptr_out_s, &ptr_out_s_s,
  &ptr_s, &put_buf_s, &put_byte_s, &put_char_s, &put_string_s,
  &qquote_s, &qref_s, &quasi_s, &quasilist_s, &query_error_s,
  &quote_s, &r_atan2_s, &r_ceil_s, &r_expt_s, &r_floor_s,
  &r_lognot_s, &r_logtrunc_s, &r_mod_s, &r_round_s, &r_trunc_s,
  &random_state_s, &random_state_var_s, &random_warmup_s, &range_error_s, &range_s,
  &rcons_s, &rdev_k, &rdev_s, &real_time_k, &rebind_s,
  &rec_source_loc_s, &recip_s, &reflect_k, &regex_s, &rep_s,
  &repeat_s, &repeat_spec_k, &require_s, &resolve_k, &rest_s,
  &restart_s, &return_from_s, &return_s, &rfilt_k, &round1_s,
  &round_s, &rplaca_s, &rplacd_s, &sbit_s, &sec_s,
  &seek_s, &seq_iter_s, &sequence_s, &set_prop_s, &set_s,
  &setq_s, &setqf_s, &short_s, &shortest_k, &sign_extend_s,
  &signum_s, &sin_s, &single_s, &size_k, &size_s,
  &skip_s, &slot_s, &some_s, &space_k, &special_s,
  &splice_s, &sqrt_s, &square_s, &stat_s, &stddebug_s,
  &stderr_s, &stdin_s, &stdio_stream_s, &stdnull_s, &stdout_s,
  &str_d_s, &str_s, &stream_s, &string_k, &string_s,
  &struct_lit_s, &struct_s, &struct_type_s, &switch_s, &sym_s,
  &symacro_k, &symacrolet_s, &syntax_error_s, &sys_abscond_from_s, &sys_apply_s,
  &sys_catch_s, &sys_l1_setq_s, &sys_l1_val_s, &sys_lisp1_setq_s, &sys_lisp1_value_s,
  &sys_mark_special_s, &sys_qquote_s, &sys_splice_s, &sys_unquote_s, &system_error_s,
  &system_package_s, &tan_s, &text_s, &throw_s, &time_local_s,
  &time_parse_s, &time_s, &time_string_s, &time_utc_s, &timeout_error_s,
  &times_k, &tlist_k, &tobase64_k, &tofloat_k, &tohtml_k,
  &tohtml_star_k, &toint_k, &tonumber_k, &topercent_k, &tourl_k,
  &trailer_s, &tree_bind_s, &tree_case_s, &trunc1_s, &trunc_s,
  &truncate_s, &try_s, &type_error_s, &ubit_s, &uchar_s,
  &uid_k, &uid_s, &uint16_s, &uint32_s, &uint64_s,
  &uint8_s, &uint_s, &ulong_s, &unbound_s, &unget_byte_s,
  &unget_char_s, &union_s, &unique_s, &unquote_s, &until_s,
  &until_star_s, &upcase_k, &uref_s, &user_package_s, &userdata_k,
  &ushort_s, &usr_var_s, &uw_protect_s, &val_s, &var_k,
  &var_s, &vars_k, &vec_list_s, &vec_s, &vecref_s,
  &vector_lit_s, &vm_closure_s, &vm_desc_s, &void_s, &warning_s,
  &wchar_s, &weak_keys_k, &weak_vals_k, &when_s, &while_s,
  &while_star_s, &whole_k, &width_s, &wild_s, &word_char_k,
  &wrap_k, &wstr_d_s, &wstr_s, &year_s, &zap_s,
  &zarray_s, &zerop_s, &zeroplus_s, &zone_s,

#if CONFIG_DEBUG_SUPPORT
  &debug_quit_s,
#endif
#if HAVE_DLOPEN
  &dlhandle_s, &dlsym_s,
#endif
#if HAVE_GRGID
  &group_s, &mem_s,
#endif
#if HAVE_PWUID
  &dir_s, &gecos_s, &passwd_s, &shell_s,
#endif
#if HAVE_SOCKETS
  &addr_s, &addrinfo_s, &canonname_s, &family_s, &flags_s,
  &flow_info_s, &port_s, &protocol_s, &scope_id_s, &sockaddr_in6_s,
  &sockaddr_in_s, &sockaddr_un_s, &socket_error_s, &socktype_s,
#endif
#if HAVE_SYSLOG
  &prio_k,
#endif
#if HAVE_TERMIOS
  &cc_s, &cflag_s, &iflag_s, &ispeed_s, &lflag_s,
  &oflag_s, &ospeed_s, &termios_s,
#endif
#if HAVE_UNAME
  &domainname_s, &machine_s, &nodename_s, &release_s, &sysname_s,
  &utsname_s, &version_s,
#endif
  convert(val *, 0)
};
