/* Copyright 2015-2024
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

extern val struct_type_s, meth_s, print_s, make_struct_lit_s;
extern val init_k, postinit_k;
extern val slot_s, slotset_s, static_slot_s, static_slot_set_s, derived_s;
extern val lambda_set_s;
extern val iter_begin_s, iter_more_s, iter_item_s, iter_step_s, iter_reset_s;

extern struct cobj_ops struct_inst_ops;
extern struct cobj_class *struct_cls;

enum special_slot {
  equal_m, copy_m, nullify_m, from_list_m, lambda_m, lambda_set_m,
  length_m, length_lt_m, car_m, cdr_m, rplaca_m, rplacd_m,
  iter_begin_m, iter_more_m, iter_item_m, iter_step_m, iter_reset_m,
  plus_m, slot_m, slotset_m, static_slot_m, static_slot_set_m,
  num_special_slots
};

val make_struct_type(val name, val super,
                     val static_slots, val slots,
                     val static_initfun, val initfun, val boactor,
                     val postinitfun);
val struct_type_p(val obj);
val struct_get_initfun(val type);
val struct_set_initfun(val type, val fun);
val struct_get_postinitfun(val type);
val struct_set_postinitfun(val type, val fun);
val super(val type, val idx);
val make_struct(val type, val plist, varg );
val struct_from_plist(val type, varg plist);
val struct_from_args(val type, varg boa);
val make_lazy_struct(val type, val argfun);
val make_struct_lit(val type, val plist);
val allocate_struct(val type);
val copy_struct(val strct);
val clear_struct(val strct, val value);
val replace_struct(val target, val source);
val reset_struct(val strct);
val find_struct_type(val sym);
val slot(val strct, val sym);
val maybe_slot(val strct, val sym);
val slotset(val strct, val sym, val newval);
val static_slot(val stype, val sym);
val static_slot_set(val stype, val sym, val newval);
val static_slot_ensure(val stype, val sym, val newval, val no_error_p);
val static_slot_home(val stype, val sym);
val test_dirty(val strct);
val test_clear_dirty(val strct);
val clear_dirty(val strct);
val slotp(val type, val sym);
val static_slot_p(val type, val sym);
val slots(val stype);
val structp(val obj);
val struct_type(val strct);
val struct_type_name(val stype);
val struct_subtype_p(val sub, val sup);
val method(val strct, val slotsym);
val method_args(val strct, val slotsym, varg );
val super_method(val strct, val slotsym);
val uslot(val slot);
val umethod(val slot, varg );
val method_name(val fun);
val slot_types(val slot);
val static_slot_types(val slot);
val slot_type_reg(val slot, val strct);
val static_slot_type_reg(val slot, val strct);
val get_special_slot(val obj, enum special_slot spidx);
val get_special_required_slot(val obj, enum special_slot spidx);
val get_special_slot_by_type(val stype, enum special_slot spidx);
INLINE int obj_struct_p(val obj) { return obj->co.ops == &struct_inst_ops; }
void struct_init(void);
void struct_compat_fixup(int compat_ver);
