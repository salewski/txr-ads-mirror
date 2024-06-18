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

typedef enum hash_weak_opt {
  hash_weak_none = 0,
  hash_weak_keys = 1,
  hash_weak_vals = 2,
  hash_weak_or = 3,
  hash_weak_and = 4,
} hash_weak_opt_t;

struct hash_iter {
  struct hash_iter *next;
  val hash;
  val table;
  ucnum mask;
  ucnum index;
};

extern val weak_keys_k, weak_vals_k, weak_and_k, weak_or_k, userdata_k;
extern val equal_based_k, eql_based_k, eq_based_k;

extern struct cobj_class *hash_cls;

ucnum equal_hash(val obj, int *count, ucnum);
val make_seeded_hash(val weak_keys, val weak_vals, val equal_based, val seed);
val make_hash(hash_weak_opt_t, val equal_based);
val make_eq_hash(hash_weak_opt_t);
val make_similar_hash(val existing);
val copy_hash(val existing);
val gethash_c(val self, val hash, val key, loc new_p);
val gethash_e(val self, val hash, val key);
val gethash_d(val hash, val key);
val gethash(val hash, val key);
val inhash(val hash, val key, val init);
val gethash_n(val hash, val key, val notfound_val);
val sethash(val hash, val key, val value);
val pushhash(val hash, val key, val value);
val remhash(val hash, val key);
val clearhash(val hash);
val hash_count(val hash);
val us_hash_count(val hash);
val get_hash_userdata(val hash);
val set_hash_userdata(val hash, val data);
val hashp(val obj);
val maphash(val func, val hash);
void hash_iter_init(struct hash_iter *hi, val hash, val self);
void us_hash_iter_init(struct hash_iter *hi, val hash);
val hash_iter_next(struct hash_iter *hi);
val hash_iter_peek(struct hash_iter *hi);
val hash_begin(val hash);
val hash_next(val iter);
val hash_peek(val iter);
val hash_reset(val iter, val hash);
val copy_hash_iter(val iter);
val hash_eql(val obj);
val hash_equal(val obj, val seed);
val hashv(struct args *args);
val hashl(val args);
val hash_construct(val hashl_args, val pairs);
val hash_from_pairs_v(val pairs, struct args *hashv_args);
val hash_from_alist_v(val alist, struct args *hashv_args);
val hash_map(val fun, val seq, struct args *hashv_args);
val hash_props(struct args *plist);
val hash_list(val keys, struct args *hashv_args);
val hash_zip(val keys, val vals, struct args *hashv_args);
val group_by(val func, val seq, struct args *hashv_args);
val group_map(val by_fun, val filter_fun, val seq, struct args *hashv_args);
val group_reduce(val hash, val by_fun, val reduce_fun, val seq,
                 val initval, val filter_fun);
val hash_keys(val hash);
val hash_values(val hash);
val hash_pairs(val hash);
val hash_alist(val hash);
val hash_uni(val hash1, val hash2, val joinfun, val map1fun, val map2fun);
val hash_join(val hash1, val hash2, val joinfun, val h1dfl, val h2dfl);
val hash_diff(val hash1, val hash2);
val hash_symdiff(val hash1, val hash2);
val hash_isec(val hash1, val hash2, val joinfun);
val hash_subset(val hash1, val hash2);
val hash_proper_subset(val hash1, val hash2);
val hash_update(val hash, val fun);
val hash_update_1(val hash, val key, val fun, val init);
val hash_revget(val hash, val value, val test, val keyfun);
val hash_keys_of(val hash, val value, val test, val keyfun);
val hash_invert(val hash, val joinfun, val unitfun, struct args *hashv_args);

void hash_process_weak(void);

INLINE loc gethash_l(val self, val hash, val key, loc new_p)
{
  return cdr_l(gethash_c(self, hash, key, new_p));
}

void hash_early_init(void);
void hash_init(void);
