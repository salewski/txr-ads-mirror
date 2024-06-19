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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <assert.h>
#include <wchar.h>
#include <signal.h>
#include "config.h"
#include "alloca.h"
#if HAVE_VALGRIND
#include <valgrind/memcheck.h>
#endif
#if HAVE_RLIMIT
#include <sys/resource.h>
#endif
#include "lib.h"
#include "stream.h"
#include "hash.h"
#include "eval.h"
#include "gc.h"
#include "signal.h"
#include "unwind.h"
#include "args.h"

#define PROT_STACK_SIZE         1024

#if CONFIG_SMALL_MEM
#define HEAP_SIZE               4096
#define CHECKOBJ_VEC_SIZE       HEAP_SIZE
#define MUTOBJ_VEC_SIZE         HEAP_SIZE
#define FULL_GC_INTERVAL        20
#define FRESHOBJ_VEC_SIZE       (2 * HEAP_SIZE)
#define DFL_MALLOC_DELTA_THRESH (16L * 1024 * 1024)
#define DFL_STACK_LIMIT         (128 * 1024L)
#else
#define HEAP_SIZE               16384
#define CHECKOBJ_VEC_SIZE       (2 * HEAP_SIZE)
#define MUTOBJ_VEC_SIZE         (2 * HEAP_SIZE)
#define FULL_GC_INTERVAL        40
#define FRESHOBJ_VEC_SIZE       (8 * HEAP_SIZE)
#define DFL_MALLOC_DELTA_THRESH (64L * 1024 * 1024)
#define DFL_STACK_LIMIT         (16384 * 1024L)
#endif

#define MIN_STACK_LIMIT         32768

#if HAVE_MEMALIGN || HAVE_POSIX_MEMALIGN
#define OBJ_ALIGN (sizeof (obj_t))
#else
#define OBJ_ALIGN 8
#endif

typedef struct heap {
  obj_t block[HEAP_SIZE];
  struct heap *next;
#if CONFIG_NAN_BOXING_STRIP_TAG
  ucnum tag;
#endif
} heap_t;

typedef struct mach_context {
  struct jmp buf;
} mach_context_t;

#define save_context(X) jmp_save(&(X).buf)

int opt_gc_debug;
#if HAVE_VALGRIND
int opt_vg_debug;
#endif

val *gc_stack_bottom;
val *gc_stack_limit;

static val *prot_stack[PROT_STACK_SIZE];
static val **prot_stack_limit = prot_stack + PROT_STACK_SIZE;
val **gc_prot_top = prot_stack;

static val free_list, *free_tail = &free_list;
static heap_t *heap_list;
static val heap_min_bound, heap_max_bound;

alloc_bytes_t gc_bytes;
static alloc_bytes_t prev_malloc_bytes;
alloc_bytes_t opt_gc_delta = DFL_MALLOC_DELTA_THRESH;

int gc_enabled = 1;
static int inprogress;

static struct fin_reg {
  struct fin_reg *next;
  val obj;
  val fun;
  int reachable;
} *final_list, **final_tail = &final_list;

#if CONFIG_GEN_GC
static val checkobj[CHECKOBJ_VEC_SIZE];
static int checkobj_idx;
static val mutobj[MUTOBJ_VEC_SIZE];
static int mutobj_idx;
static val freshobj[FRESHOBJ_VEC_SIZE];
static int freshobj_idx;
int full_gc;
#endif

#if CONFIG_EXTRA_DEBUGGING
val break_obj;
#endif

struct prot_array {
  cnum size;
  val self;
  val arr[FLEX_ARRAY];
};

val gc_prot_array_s;
struct cobj_class *prot_array_cls;

val prot1(val *loc)
{
  assert (gc_prot_top < prot_stack_limit);
  assert (loc != 0);
  *gc_prot_top++ = loc;
  return nil; /* for use in macros */
}

void protect(val *first, ...)
{
  val *next = first;
  va_list vl;
  va_start (vl, first);

  while (next) {
    prot1(next);
    next = va_arg(vl, val *);
  }

  va_end (vl);
}

static void more(void)
{
#if CONFIG_NAN_BOXING_STRIP_TAG
  ucnum tagged_ptr = coerce(cnum, chk_malloc_gc_more(sizeof (heap_t)));
  heap_t *heap = coerce(heap_t *, tagged_ptr & ~TAG_BIGMASK);
#else
  heap_t *heap = coerce(heap_t *, chk_malloc_gc_more(sizeof *heap));
#endif
  obj_t *block = heap->block, *end = heap->block + HEAP_SIZE;

#if CONFIG_NAN_BOXING_STRIP_TAG
  heap->tag = tagged_ptr >> TAG_BIGSHIFT;
#endif

  if (free_list == 0)
    free_tail = &heap->block[0].t.next;

  if (!heap_max_bound || end > heap_max_bound)
    heap_max_bound = end;

  if (!heap_min_bound || block < heap_min_bound)
    heap_min_bound = block;

  while (block < end) {
    block->t.next = free_list;
    block->t.type = convert(type_t, FREE);
#if CONFIG_EXTRA_DEBUGGING
      if (block == break_obj) {
#if HAVE_VALGRIND
        VALGRIND_PRINTF_BACKTRACE("object %p newly added to free list\n", convert(void *, block));
#endif
        breakpt();
      }
#endif
    free_list = block++;
  }

  heap->next = heap_list;
  heap_list = heap;

#if HAVE_VALGRIND
  if (opt_vg_debug)
    VALGRIND_MAKE_MEM_NOACCESS(&heap->block, sizeof heap->block);
#endif
}

val make_obj(void)
{
  int tries;
  alloc_bytes_t malloc_delta = malloc_bytes - prev_malloc_bytes;
  assert (!async_sig_enabled);

#if CONFIG_GEN_GC
  if ((opt_gc_debug || freshobj_idx >= FRESHOBJ_VEC_SIZE ||
       malloc_delta >= opt_gc_delta) &&
      gc_enabled)
  {
    gc();
  }

  if (freshobj_idx >= FRESHOBJ_VEC_SIZE)
    full_gc = 1;
#else
  if ((opt_gc_debug || malloc_delta >= opt_gc_delta) && gc_enabled) {
    gc();
  }
#endif

  for (tries = 0; tries < 3; tries++) {
    if (free_list) {
      val ret = free_list;
#if HAVE_VALGRIND
      if (opt_vg_debug)
        VALGRIND_MAKE_MEM_DEFINED(free_list, sizeof *free_list);
#endif
      free_list = free_list->t.next;

      if (free_list == 0)
        free_tail = &free_list;
#if HAVE_VALGRIND
      if (opt_vg_debug)
        VALGRIND_MAKE_MEM_UNDEFINED(ret, sizeof *ret);
#endif
#if CONFIG_GEN_GC
      ret->t.gen = 0;
      ret->t.fincount = 0;
      if (!full_gc)
        freshobj[freshobj_idx++] = ret;
#endif
      gc_bytes += sizeof (obj_t);
#if CONFIG_EXTRA_DEBUGGING
      if (ret == break_obj) {
#if HAVE_VALGRIND
        VALGRIND_PRINTF_BACKTRACE("object %p allocated\n", convert(void *, ret));
#endif
        breakpt();
      }
#endif
      return ret;
    }

#if CONFIG_GEN_GC
    if (!full_gc && freshobj_idx < FRESHOBJ_VEC_SIZE) {
      more();
      continue;
    }
#endif

    switch (tries) {
    case 0:
      if (gc_enabled) {
        gc();
        break;
      }
      /* fallthrough */
    case 1:
      more();
      break;
    }
  }

  abort();
}

val copy_obj(val orig)
{
  val copy = make_obj();
  *copy = *orig;
#if CONFIG_GEN_GC
  copy->t.fincount = 0;
  copy->t.gen = 0;
#endif
  return copy;
}

static void finalize(val obj)
{
  switch (convert(type_t, obj->t.type)) {
  case NIL:
  case CONS:
  case CHR:
  case NUM:
  case LIT:
  case PKG:
  case FUN:
  case LCONS:
  case ENV:
  case FLNUM:
  case RNG:
  case TNOD:
    return;
  case SYM:
    free(obj->s.slot_cache);
    obj->s.slot_cache = 0;
    return;
  case STR:
    free(obj->st.str);
    obj->st.str = 0;
    return;
  case LSTR:
    free(obj->ls.props);
    obj->ls.props = 0;
    return;
  case VEC:
    free(obj->v.vec-2);
    obj->v.vec = 0;
    return;
  case COBJ:
  case CPTR:
    obj->co.ops->destroy(obj);
    obj->co.handle = 0;
    return;
  case BGNUM:
    mp_clear(mp(obj));
    return;
  case BUF:
    if (obj->b.size) {
      free(obj->b.data);
      obj->b.data = 0;
    }
    return;
  case DARG:
    free(obj->a.args);
    obj->a.args = 0;
    return;
  }

  assert (0 && "corrupt type field");
}

void cobj_destroy_stub_op(val obj)
{
  (void) obj;
}

void cobj_destroy_free_op(val obj)
{
  free(obj->co.handle);
}

static void mark_obj(val obj)
{
  val self = lit("gc");
  type_t t;

tail_call:
#define mark_obj_tail(o) do { obj = (o); goto tail_call; } while (0)

  if (!is_ptr(obj))
    return;

  t = obj->t.type;

  if ((t & REACHABLE) != 0)
    return;

#if CONFIG_GEN_GC
  if (!full_gc && obj->t.gen > 0)
    return;
#endif

  if ((t & FREE) != 0)
    abort();

#if CONFIG_GEN_GC
  if (obj->t.gen == -1)
    obj->t.gen = 0;  /* Will be promoted to generation 1 by sweep_one */
#endif

  obj->t.type = convert(type_t, t | REACHABLE);

#if CONFIG_EXTRA_DEBUGGING
  if (obj == break_obj) {
#if HAVE_VALGRIND
    VALGRIND_PRINTF_BACKTRACE("object %p marked\n", convert(void *, obj));
#endif
    breakpt();
  }
#endif

  switch (t) {
  case NIL:
  case CHR:
  case NUM:
  case LIT:
  case BGNUM:
  case FLNUM:
    return;
  case CONS:
    mark_obj(obj->c.car);
    mark_obj_tail(obj->c.cdr);
  case STR:
    mark_obj_tail(obj->st.len);
  case SYM:
    mark_obj(obj->s.name);
    mark_obj_tail(obj->s.package);
  case PKG:
    mark_obj(obj->pk.name);
    mark_obj(obj->pk.hidhash);
    mark_obj_tail(obj->pk.symhash);
  case FUN:
    switch (obj->f.functype) {
    case FINTERP:
      mark_obj(obj->f.f.interp_fun);
      break;
    case FVM:
      mark_obj(obj->f.f.vm_desc);
      break;
    }
    mark_obj_tail(obj->f.env);
  case VEC:
    {
      val alloc_size = obj->v.vec[vec_alloc];
      val len = obj->v.vec[vec_length];
      cnum i, fp = c_num(len, self);

      mark_obj(alloc_size);
      mark_obj(len);

      for (i = 0; i < fp; i++)
        mark_obj(obj->v.vec[i]);
    }
    return;
  case LCONS:
    mark_obj(obj->lc.func);
    mark_obj(obj->lc.car);
    mark_obj_tail(obj->lc.cdr);
  case LSTR:
    mark_obj(obj->ls.prefix);
    mark_obj(obj->ls.props->limit);
    mark_obj(obj->ls.props->term);
    mark_obj_tail(obj->ls.list);
  case COBJ:
    obj->co.ops->mark(obj);
    return;
  case CPTR:
    obj->co.ops->mark(obj);
    mark_obj_tail(obj->cp.cls);
  case ENV:
    mark_obj(obj->e.vbindings);
    mark_obj(obj->e.fbindings);
    mark_obj_tail(obj->e.up_env);
  case RNG:
    mark_obj(obj->rn.from);
    mark_obj_tail(obj->rn.to);
  case BUF:
    mark_obj(obj->b.len);
    mark_obj_tail(obj->b.size);
  case TNOD:
    mark_obj(obj->tn.left);
    mark_obj(obj->tn.right);
    mark_obj_tail(obj->tn.key);
  case DARG:
    {
      varg args = obj->a.args;
      cnum i, n = args->fill;
      val *arg = args->arg;

      mark_obj(obj->a.car);
      mark_obj(obj->a.cdr);

      for (i = 0; i < n; i++)
        mark_obj(arg[i]);

      mark_obj_tail(args->list);
    }
  }

  assert (0 && "corrupt type field");
}

static void mark_obj_norec(val obj)
{
  type_t t;

  if (!is_ptr(obj))
    return;

  t = obj->t.type;

  if ((t & REACHABLE) != 0)
    return;

#if CONFIG_GEN_GC
  if (!full_gc && obj->t.gen > 0)
    return;
#endif

  if ((t & FREE) != 0)
    abort();

#if CONFIG_GEN_GC
  if (obj->t.gen == -1)
    obj->t.gen = 0;  /* Will be promoted to generation 1 by sweep_one */
#endif

  obj->t.type = convert(type_t, t | REACHABLE);

#if CONFIG_EXTRA_DEBUGGING
  if (obj == break_obj) {
#if HAVE_VALGRIND
    VALGRIND_PRINTF_BACKTRACE("object %p marked\n", convert(void *, obj));
#endif
    breakpt();
  }
#endif
}

void cobj_mark_op(val obj)
{
  (void) obj;
}

static int in_heap(val ptr)
{
  heap_t *heap;

  if (!is_ptr(ptr))
    return 0;

  if (coerce(uint_ptr_t, ptr) % OBJ_ALIGN != 0)
    return 0;

  if (ptr < heap_min_bound || ptr >= heap_max_bound)
    return 0;

  for (heap = heap_list; heap != 0; heap = heap->next) {
    if (ptr >= heap->block && ptr < heap->block + HEAP_SIZE) {
#if HAVE_MEMALIGN || HAVE_POSIX_MEMALIGN
      return 1;
#else
      if ((coerce(char *, ptr) -
           coerce(char *, heap->block)) % sizeof (obj_t) == 0)
        return 1;
#endif
    }
  }

  return 0;
}

static void mark_obj_maybe(val maybe_obj)
{
#if HAVE_VALGRIND
  VALGRIND_MAKE_MEM_DEFINED(&maybe_obj, sizeof maybe_obj);
#endif
  if (in_heap(maybe_obj)) {
    type_t t;
#if HAVE_VALGRIND
    if (opt_vg_debug)
      VALGRIND_MAKE_MEM_DEFINED(maybe_obj, SIZEOF_PTR);
#endif
    t = maybe_obj->t.type;
    if ((t & FREE) == 0) {
      mark_obj(maybe_obj);
    } else {
#if HAVE_VALGRIND
      if (opt_vg_debug)
        VALGRIND_MAKE_MEM_NOACCESS(maybe_obj, sizeof *maybe_obj);
#endif
    }
  }
}

static void mark_mem_region(val *low, val *high)
{
  if (low > high) {
    val *tmp = high;
    high = low;
    low = tmp;
  }

  for (; low < high; low++)
    mark_obj_maybe(*low);
}

NOINLINE static void mark(val *gc_stack_top)
{
  val **rootloc;

  /*
   * First, scan the officially registered locations.
   */
  for (rootloc = prot_stack; rootloc != gc_prot_top; rootloc++)
    mark_obj(**rootloc);

#if CONFIG_GEN_GC
  /*
   * Mark the additional objects indicated for marking.
   */
  if (!full_gc)
  {
    int i;
    for (i = 0; i < checkobj_idx; i++)
      mark_obj(checkobj[i]);
    for (i = 0; i < mutobj_idx; i++)
      mark_obj(mutobj[i]);
  }
#endif

  /*
   * Finally, the stack.
   */
  mark_mem_region(gc_stack_top, gc_stack_bottom);
}

static int sweep_one(obj_t *block)
{
#if HAVE_VALGRIND
  const int vg_dbg = opt_vg_debug;
#else
  const int vg_dbg = 0;
#endif

#if CONFIG_EXTRA_DEBUGGING
  if (block == break_obj && (block->t.type & FREE) == 0) {
#if HAVE_VALGRIND
    VALGRIND_PRINTF_BACKTRACE("object %p swept (type = %x)\n",
                              convert(void *, block),
                              convert(unsigned int, block->t.type));
#endif
    breakpt();
  }
#endif

#if CONFIG_GEN_GC
  if (!full_gc && block->t.gen > 0)
    abort();
#endif

  if ((block->t.type & (REACHABLE | FREE)) == (REACHABLE | FREE))
    abort();

  if (block->t.type & REACHABLE) {
#if CONFIG_GEN_GC
    block->t.gen = 1;
#endif
    block->t.type = convert(type_t, block->t.type & ~REACHABLE);
    return 0;
  }

  if (block->t.type & FREE) {
#if HAVE_VALGRIND
    if (vg_dbg)
      VALGRIND_MAKE_MEM_NOACCESS(block, sizeof *block);
#endif
    return 1;
  }

  finalize(block);
  block->t.type = convert(type_t, block->t.type | FREE);

  /* If debugging is turned on, we want to catch instances
     where a reachable object is wrongly freed. This is difficult
     to do if the object is recycled soon after.
     So when debugging is on, the free list is FIFO
     rather than LIFO, which increases our chances that the
     code which is still using the object will trip on
     the freed object before it is recycled. */
  if (vg_dbg || opt_gc_debug) {
#if HAVE_VALGRIND
    if (vg_dbg && free_tail != &free_list)
      VALGRIND_MAKE_MEM_DEFINED(free_tail, sizeof *free_tail);
#endif
    *free_tail = block;
    block->t.next = nil;
#if HAVE_VALGRIND
    if (vg_dbg) {
      if (free_tail != &free_list)
        VALGRIND_MAKE_MEM_NOACCESS(free_tail, sizeof *free_tail);
      VALGRIND_MAKE_MEM_NOACCESS(block, sizeof *block);
    }
#endif
    free_tail = &block->t.next;
  } else {
    block->t.next = free_list;
    free_list = block;
  }

  return 1;
}

NOINLINE static int_ptr_t sweep(void)
{
  int_ptr_t free_count = 0;
  heap_t **pph;
  val hminb = nil, hmaxb = nil;
#if HAVE_VALGRIND
  const int vg_dbg = opt_vg_debug;
#endif

#if CONFIG_GEN_GC
  if (!full_gc) {
    int i;

    /* No need to mark block defined via Valgrind API; everything
       in the freshobj is an allocated node! */
    for (i = 0; i < freshobj_idx; i++)
      free_count += sweep_one(freshobj[i]);

    /* Generation 1 objects that were indicated for dangerous
       mutation must have their REACHABLE flag flipped off,
       and must be returned to gen 1. */
    for (i = 0; i < mutobj_idx; i++)
      sweep_one(mutobj[i]);

    return free_count;
  }

#endif

  for (pph = &heap_list; *pph != 0; ) {
    obj_t *block, *end;
    heap_t *heap = *pph;
    int_ptr_t old_count = free_count;
    val old_free_list = free_list;

#if HAVE_VALGRIND
    if (vg_dbg)
        VALGRIND_MAKE_MEM_DEFINED(&heap->block, sizeof heap->block);
#endif

    for (block = heap->block, end = heap->block + HEAP_SIZE;
         block < end;
         block++)
    {
      free_count += sweep_one(block);
    }

    if (free_count - old_count == HEAP_SIZE) {
      val *ppf;

      free_list = old_free_list;
#if HAVE_VALGRIND
      if (vg_dbg) {
        val iter;
        for (iter = free_list; iter; iter = iter->t.next)
          VALGRIND_MAKE_MEM_DEFINED(iter, sizeof *iter);
      }
#endif
      for (ppf = &free_list; *ppf != nil; ) {
        val block = *ppf;
        if (block >= heap->block && block < end) {
          if ((*ppf = block->t.next) == 0)
            free_tail = ppf;
        } else {
          ppf = &block->t.next;
        }
      }
      *pph = heap->next;
#if CONFIG_NAN_BOXING_STRIP_TAG
      free(coerce(heap_t *, coerce(ucnum, heap) | (heap->tag << TAG_BIGSHIFT)));
#else
      free(heap);
#endif

#if HAVE_VALGRIND
      if (vg_dbg) {
        val iter, next;
        for (iter = free_list; iter; iter = next) {
          next = iter->t.next;
          VALGRIND_MAKE_MEM_NOACCESS(iter, sizeof *iter);
        }
      }
#endif
    } else {
      if (!hmaxb || end > hmaxb)
        hmaxb = end;
      if (!hminb || heap->block < hminb)
        hminb = heap->block;
      pph = &(*pph)->next;
    }
  }

  heap_min_bound = hminb;
  heap_max_bound = hmaxb;
  return free_count;
}

static int is_reachable(val obj)
{
  type_t t;

#if CONFIG_GEN_GC
  if (!full_gc && obj->t.gen > 0)
    return 1;
#endif

  t = obj->t.type;

  return (t & REACHABLE) != 0;
}

NOINLINE static void prepare_finals(void)
{
  struct fin_reg *f;

  if (!final_list)
    return;

  for (f = final_list; f; f = f->next)
    f->reachable = is_reachable(f->obj);

  for (f = final_list; f; f = f->next) {
    if (!f->reachable) {
      mark_obj(f->obj);
    }
    mark_obj(f->fun);
  }
}

static val call_finalizers_impl(val ctx,
                                int (*should_call)(struct fin_reg *, val))
{
  val ret = nil;

  for (;;) {
    struct fin_reg *f, **tail;
    struct fin_reg *found = 0, **ftail = &found;

    for (f = final_list, tail = &final_list; f; ) {
      struct fin_reg *next = f->next;

      if (should_call(f, ctx)) {
        *ftail = f;
        ftail = &f->next;
      } else {
        *tail = f;
        tail = &f->next;
      }

      f = next;
    }

    *ftail = 0;
    *tail = 0;
    final_tail = tail;

    if (!found)
      break;

    do {
      struct fin_reg *next = found->next;
      val obj = found->obj;
      funcall1(found->fun, obj);
#if CONFIG_GEN_GC
      if (--obj->t.fincount == 0 && inprogress &&
          !full_gc && !found->reachable)
      {
        if (freshobj_idx < FRESHOBJ_VEC_SIZE) {
          obj->t.gen = 0;
          freshobj[freshobj_idx++] = obj;
        } else {
          full_gc = 1;
        }
      }
#endif
      free(found);
      found = next;
      ret = t;
    } while (found);
  }

  return ret;
}

static int is_unreachable_final(struct fin_reg *f, val ctx)
{
  (void) ctx;
  return !f->reachable;
}

NOINLINE static void call_finals(void)
{
  (void) call_finalizers_impl(nil, is_unreachable_final);
}

void gc(void)
{
#if CONFIG_GEN_GC
  int exhausted = (free_list == 0);
  int full_gc_next_time = 0;
  static int gc_counter;
#endif
  int swept;
  mach_context_t *pmc = convert(mach_context_t *, alloca(sizeof *pmc));

  assert (gc_enabled);

  if (inprogress++)
    assert(0 && "gc re-entered");

#if CONFIG_GEN_GC
  if (malloc_bytes - prev_malloc_bytes >= opt_gc_delta)
    full_gc = 1;
#endif

  save_context(*pmc);
  gc_enabled = 0;
  rcyc_empty();
  iobuf_list_empty();
  mark(coerce(val *, pmc));
  hash_process_weak();
  prepare_finals();
  swept = sweep();
#if CONFIG_GEN_GC
  if (++gc_counter >= FULL_GC_INTERVAL ||
      freshobj_idx >= FRESHOBJ_VEC_SIZE)
  {
    full_gc_next_time = 1;
    gc_counter = 0;
  }

  if (exhausted && full_gc && swept < 3 * HEAP_SIZE / 4)
    more();
#else
  if (swept < 3 * HEAP_SIZE / 4)
    more();
#endif

#if CONFIG_GEN_GC
  checkobj_idx = 0;
  mutobj_idx = 0;
  freshobj_idx = 0;
  full_gc = full_gc_next_time;
#endif
  call_finals();
  gc_enabled = 1;
  prev_malloc_bytes = malloc_bytes;

  inprogress--;
}

int gc_state(int enabled)
{
  int old = gc_enabled;
  gc_enabled = enabled;
  return old;
}

int gc_inprogress(void)
{
  return inprogress;
}

void gc_init(val *stack_bottom)
{
  gc_stack_bottom = stack_bottom;
  gc_stack_limit = gc_stack_bottom - DFL_STACK_LIMIT / sizeof (val);
#if HAVE_RLIMIT
  {
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
      rlim_t lim = rl.rlim_cur;
      if (lim != RLIM_INFINITY) {
        ptrdiff_t delta = (lim >= MIN_STACK_LIMIT
                           ? (lim - lim / 16)
                           : MIN_STACK_LIMIT) / sizeof (val);
        gc_stack_limit = gc_stack_bottom - delta;
      }
    }
  }
#endif
}

void gc_mark(val obj)
{
  mark_obj(obj);
}

void gc_mark_norec(val obj)
{
  mark_obj_norec(obj);
}

void gc_conservative_mark(val maybe_obj)
{
  mark_obj_maybe(maybe_obj);
}

void gc_mark_mem(val *low, val *high)
{
  mark_mem_region(low, high);
}

int gc_is_reachable(val obj)
{
  return is_ptr(obj) ? is_reachable(obj) : 1;
}

#if CONFIG_GEN_GC

void gc_assign_check(val p, val c)
{
  if (p && is_ptr(c) && p->t.gen == 1 && c->t.gen == 0 && !full_gc) {
    if (checkobj_idx < CHECKOBJ_VEC_SIZE) {
      c->t.gen = -1;
      checkobj[checkobj_idx++] = c;
    } else if (gc_enabled) {
      gc();
      /* c can't be in gen 0 because there are no baby objects after gc */
    } else {
      /* We have no space to in checkobj record this backreference, and gc is
         not available to promote obj to gen 1. We must schedule a full gc. */
      full_gc = 1;
    }
  }
}

val gc_set(loc lo, val obj)
{
  gc_assign_check(lo.obj, obj);
  *valptr(lo) = obj;
  return obj;
}

val gc_mutated(val obj)
{
  /* We care only about mature generation objects that have not
     already been noted. And if a full gc is coming, don't bother. */
  if (full_gc || obj->t.gen <= 0)
    return obj;
  /* Store in mutobj array *before* triggering gc, otherwise
     baby objects referenced by obj could be reclaimed! */
  if (mutobj_idx < MUTOBJ_VEC_SIZE) {
    obj->t.gen = -1;
    mutobj[mutobj_idx++] = obj;
  } else if (gc_enabled) {
    gc();
  } else {
    full_gc = 1;
  }

  return obj;
}

val gc_push(val obj, loc plist)
{
  return gc_set(plist, cons(obj, deref(plist)));
}

#endif

static val gc_set_delta(val delta)
{
  val self = lit("gc");
  opt_gc_delta = c_num(delta, self);
  return nil;
}

static val set_stack_limit(val limit)
{
  val self = lit("set-stack-limit");
  val *gsl = gc_stack_limit;

  if (limit == nil || limit == zero) {
    gc_stack_limit = 0;
  } else {
    ucnum lim = c_unum(limit, self);
    gc_stack_limit = gc_stack_bottom - lim / sizeof (val);
  }

  return if2(gsl, num((gc_stack_bottom - gsl) * sizeof (val)));
}

static val get_stack_limit(void)
{
  val *gsl = gc_stack_limit;
  return if2(gsl, num((gc_stack_bottom - gsl) * sizeof (val)));
}

static val gc_wrap(val full)
{
  if (gc_enabled) {
#if CONFIG_GEN_GC
    if (!null_or_missing_p(full))
      full_gc = 1;
#else
    (void) full;
#endif
    gc();
    return t;
  }
  return nil;
}

val gc_finalize(val obj, val fun, val rev_order_p)
{
  val self = lit("gc-finalize");
  type_check(self, fun, FUN);

  rev_order_p = default_null_arg(rev_order_p);

  if (is_ptr(obj)) {
    struct fin_reg *f = coerce(struct fin_reg *, chk_malloc(sizeof *f));
    f->obj = obj;
    f->fun = fun;
    f->reachable = 1;

#if CONFIG_GEN_GC
    if (++obj->t.fincount == 0) {
      obj->t.fincount--;
      free(f);
      uw_throwf(error_s,
                lit("~a: too many finalizations registered against object ~s"),
                self, obj, nao);
    }
#endif

    if (rev_order_p) {
      if (!final_list)
        final_tail = &f->next;
      f->next = final_list;
      final_list = f;
    } else {
      f->next = 0;
      *final_tail = f;
      final_tail = &f->next;
    }
  }
  return obj;
}

static int is_matching_final(struct fin_reg *f, val obj)
{
  return f->obj == obj;
}

val gc_call_finalizers(val obj)
{
  return call_finalizers_impl(obj, is_matching_final);
}

val valid_object_p(val obj)
{
  if (!is_ptr(obj))
    return t;

  if (!in_heap(obj))
    return nil;

  if (obj->t.type & (REACHABLE | FREE))
    return nil;

  return t;
}

void gc_late_init(void)
{
  reg_fun(intern(lit("gc"), system_package), func_n1o(gc_wrap, 0));
  reg_fun(intern(lit("gc-set-delta"), system_package), func_n1(gc_set_delta));
  reg_fun(intern(lit("finalize"), user_package), func_n3o(gc_finalize, 2));
  reg_fun(intern(lit("call-finalizers"), user_package),
          func_n1(gc_call_finalizers));
  reg_fun(intern(lit("set-stack-limit"), user_package), func_n1(set_stack_limit));
  reg_fun(intern(lit("get-stack-limit"), user_package), func_n0(get_stack_limit));

  gc_prot_array_s = intern(lit("gc-prot-array"), system_package);
  prot_array_cls = cobj_register(gc_prot_array_s);
}

/*
 * Useful functions for gdb'ing.
 */
void unmark(void)
{
  heap_t *heap;

  for (heap = heap_list; heap != 0; heap = heap->next) {
    val block, end;
    for (block = heap->block, end = heap->block + HEAP_SIZE;
         block < end;
         block++)
    {
      block->t.type = convert(type_t, block->t.type & ~REACHABLE);
    }
  }
}

void gc_cancel(void)
{
  unmark();
#if CONFIG_GEN_GC
  checkobj_idx = 0;
  mutobj_idx = 0;
  freshobj_idx = 0;
  full_gc = 1;
#endif
  inprogress = 0;
}

void dheap(heap_t *heap, int start, int end);

void dheap(heap_t *heap, int start, int end)
{
  int i;
  for (i = start; i < end; i++)
    format(std_output, lit("(~a ~s)\n"), num(i), &heap->block[i], nao);
}

/*
 * This function does nothing.
 * gc_hint(x) just takes the address of local variable x
 * and passes it to this function. This prevents the compiler
 * from caching the value across function calls.
 * This is needed for situations where
 * - a compiler caches a variable in a register, but not entirely (the variable
 *   has a backing memory location); and
 * - that location contains a stale old value of the variable, which cannot be
 *   garbage-collected as a result; and
 * - this causes a problem, like unbounded memory growth.
 */
void gc_hint_func(val *val)
{
  (void) val;
}

void gc_report_copies(val *pvar)
{
  val *opvar = pvar;
  val obj = *pvar++;

  for (; pvar < gc_stack_bottom; pvar++) {
    if (*pvar == obj)
      printf("%p found at %p (offset %d)\n",
             convert(void *, obj), convert(void *, pvar),
             convert(int, pvar - opvar));
  }
}

void gc_free_all(void)
{
  {
    heap_t *iter = heap_list;

    while (iter) {
      heap_t *next = iter->next;
      obj_t *block, *end;

#if HAVE_VALGRIND
      if (opt_vg_debug)
        VALGRIND_MAKE_MEM_DEFINED(&iter->block, sizeof iter->block);
#endif

      for (block = iter->block, end = iter->block + HEAP_SIZE;
           block < end;
           block++)
      {
        type_t t = block->t.type;
        if ((t & FREE) != 0)
          continue;
        finalize(block);
      }

#if CONFIG_NAN_BOXING_STRIP_TAG
      free(coerce(heap_t *, coerce(ucnum, iter) | (iter->tag << TAG_BIGSHIFT)));
#else
      free(iter);
#endif
      iter = next;
    }
  }

  {
    struct fin_reg *iter = final_list;

    while (iter) {
      struct fin_reg *next = iter->next;
      free(iter);
      iter = next;
    }
  }
}

void gc_stack_overflow(void)
{
  uw_throwf(stack_overflow_s, lit("computation exceeded stack limit"), nao);
}

static void prot_array_mark(val obj)
{
  struct prot_array *pa = coerce(struct prot_array *, obj->co.handle);

  if (pa) {
    cnum i;
    for (i = 0; i < pa->size; i++)
      gc_mark(pa->arr[i]);
  }
}

static struct cobj_ops prot_array_ops = cobj_ops_init(eq,
                                                      cobj_print_op,
                                                      cobj_destroy_free_op,
                                                      prot_array_mark,
                                                      cobj_eq_hash_op,
                                                      0);

val *gc_prot_array_alloc(cnum size, val *obj)
{
  struct prot_array *pa = coerce(struct prot_array *,
                                 chk_calloc(offsetof(struct prot_array, arr) +
                                            size * sizeof(val), 1));
  pa->size = size;
  *obj = pa->self = cobj(coerce(mem_t *, pa), prot_array_cls, &prot_array_ops);

  return pa->arr;
}

void gc_prot_array_free(val *arr)
{
  if (arr) {
    struct prot_array *pa = container(arr, struct prot_array, arr);
    val obj = pa->self;
    obj->co.handle = 0;
    free(pa);
  }
}
