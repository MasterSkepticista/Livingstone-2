/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: array_allocator.cpp,v 1.21 2004/02/11 01:02:56 lbrown Exp $ */
#include <mba_utils/array_allocator.h>
#include <stdlib.h> // free
#include <string.h> // memset
#include <assert.h> // assert
//#if defined(ENABLE_COUNT_MAX_POOL_SIZE) || defined(DISABLE_DYNAMIC_MEMORY)
#  include <livingstone/L2_iostream.h>
//#endif


/**
 * Under purify, we need to disable the special-purpose allocators
 * to get any useful information out of bounds checking.
 * We ignore attempts to reclaim memory, for now.
 */
#ifdef DISABLE_L2_ALLOCATORS
void *L2_alloc(size_t sz) {
    return malloc(sz);
}
void L2_free(void *p, size_t) {
    free(p);
}
void L2_alloc_reclaim(void) {
}
#else

/**
 * lg2 returns ceil(lg(x))
 * exp2 returns 2^x
 * Under gcc, tell the optimizer that the function is a function
 * in the mathematical sense, and thus only needs to be called
 * once for each input.
 * Really, we could make them be tables.
 */
# if __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 5)
static unsigned lg2(unsigned) __attribute__ ((const));
static unsigned exp2(unsigned) __attribute__ ((const));
static unsigned ensure_minimal_block_size(unsigned sz) __attribute__ ((const));
# else
static unsigned lg2(unsigned);
static unsigned exp2(unsigned);
static unsigned ensure_minimal_block_size(unsigned sz);
# endif



/**
 * A number of checks, in order of computational cost.
 */
# ifdef ENABLE_MEMORY_CHECKS_1
// Return the size of the chunk containing the pointer.
// We assert that the chunk is larger than the pointer.
static unsigned size_of_chunk(void*);
static bool is_from_our_chunks(void*);
# endif

# ifdef ENABLE_MEMORY_CHECKS_2
// No checks so far.
# endif

# ifdef ENABLE_MEMORY_CHECKS_3
// Return whether the pointer is already on the freelist.
// We assert that it's not, since otherwise it's a duplicate free.
static bool is_on_freelist(unsigned i, void *ptr);
# endif



/**
 * False initially; set to true by L2_alloc.
 */
static bool is_ready = false;


/**
 * The maximum size of an item on a free list, expressed as the log base 2
 * of the number of 4-byte words.
 */

static const unsigned MAX_ITEM_SIZE = 30; /* meaning 2^30 words = 2^32 bytes */

/**
 * The type of a block on a free list.
 * The free lists are objects of arbitrary size, the first word of which points
 * to the next object on the freelist.
 */
struct free_item {
public:
  /// The first word of the free_item points to the successor free_item.
  free_item *next;
  /**
   * Bogus: we never call it, just quash a warning about all functions being
   * private.
   */
  free_item() { }
private:
  // Declared private to make this illegal
  void *operator new(size_t);
};

/// A free list for each power of 2 block size up to MAX_ITEM_SIZE
static free_item *freelists[MAX_ITEM_SIZE];

# ifdef ENABLE_COUNT_MAX_POOL_SIZE
/**
  * Parallel to the freelists, we keep track of how many of each has been
  * allocated, how many are currently on the freelist, and how many
  * are currently outstanding.
  * Use 'dec' to decrement.  This ensures our counts don't fall below zero.
  * Also, use 'inc' which will refuse to increment pointers.
  */
static unsigned allocation_count[MAX_ITEM_SIZE];
static unsigned freelist_size[MAX_ITEM_SIZE];
static unsigned num_outstanding[MAX_ITEM_SIZE];
static void dec(unsigned& i) {
    assert(i>0);
    --i;
}
static void inc(unsigned& i) {
    ++i;
}
# endif


/**
 * We statically allocate an array of size 2^20 = 1 Mb
 * At present, IBB _must_ be a power of two.  It would be easy
 * to fix that (i.e. for flight).
 */
static const unsigned INITIAL_BLOCK_LG2 = 20;
static const unsigned INITIAL_BLOCK_BYTES = 1 << INITIAL_BLOCK_LG2;
static char initial_block[INITIAL_BLOCK_BYTES];


/**
 * This makes sure no garbage gets on the freelist.  If deallocating
 * something not allocated from L2_alloc, it won't be in the range
 * [base , base+nbytes]
 * We statically allocate one chunk ; we may dynamically allocate more.
 */
struct chunk_descriptor /* not pooled! */ {
  /// Base address of the chunk
  void *base;
  /// Size of the chunk in bytes
  size_t nbytes;
  /// Pointer to the successor chunk; NULL at terminus
  chunk_descriptor *next;
};
/// Unique statically-allocated chunk
static chunk_descriptor initial_chunk;
/// Linked list of dynamically-allocated chunks
static chunk_descriptor *chunks;







/***************************************************************************
  Translate from bytes to exponential notation in words and back.
  These are first, because gcc-2.95.2 only seems to inline functions
  that are defined before they are used.  Doesn't usually matter, but
  we allocate a lot, and we call these every time we do so.
 ***************************************************************************/

/**
 * Returns ceil[lg(x)]
 * Note: we define lg(0) to be 0.  In this file, it turns out we never
 * in fact call lg2 with x=0, so the point is moot (but it saves a few
 * instructions to write it this way).
 */
static unsigned lg2(unsigned x) {
    // x is sum(bi*2^i) for i=0 to n-1
    unsigned sum_bi = 0; // sum(bi), the number of bits that are high
    unsigned n = 0;      // find n, the number of bits
    do {
        ++n;
        sum_bi += x & 1; // bi is the current LSB
        x >>= 1;
    } while(x);

    // ceil(lg(x)) is k if x=2^k,
    //                k+1 if x = 2^k + smaller terms
    // We know there are smaller terms if there was more than one bi.
    // Also, k is n-1 -- the largest i.
    unsigned k = n-1;
    if(sum_bi > 1)
        return k+1;
    else
        return k;
}

static unsigned exp2(unsigned x) {
    return 1 << x;
}

// This is not so pretty -- we want to make sure we can put things
// on the freelists without overwriting the next block, so we
// ensure a minimal size.
static unsigned ensure_minimal_block_size(unsigned sz) {
    if(sz<sizeof(free_item)) return sizeof(free_item);
    else return sz;
}




/***************************************************************************
  Checks, if chosen.
  Again, we put these near the top so they can be inlined.
 ***************************************************************************/
# ifdef ENABLE_MEMORY_CHECKS_1
static unsigned size_of_chunk(void *ptr) {
    unsigned pval = (unsigned)ptr;
    if(!ptr) return true;
    for(chunk_descriptor *d = chunks; d ; d = d->next) {
        if(pval >= unsigned(d->base) && pval < unsigned(d->base) + d->nbytes)
            return d->nbytes;
    }
    // if here, we didn't find it ==> not in our chunks!
    return 0;
}
static bool is_from_our_chunks(void *ptr) { return size_of_chunk(ptr)!=0; }
# endif

# ifdef ENABLE_MEMORY_CHECKS_3
static bool is_on_freelist(unsigned i, void *ptr) {
    free_item *f = freelists[i];
    while(f) {
        if(f==ptr) return true;
        else f = f->next;
    }
    return false;
}
# endif






/***************************************************************************
  Allocation.
 ***************************************************************************/
static void *L2_allocate_exp2(unsigned i);
static void get_one_block_from_malloc(unsigned i);

/**
 * Use malloc to get a new block of size 2^i, put it on the freelist.
 * This can be disabled, i.e. for flight; it's the only place where
 * we use malloc.
 */
static void get_one_block_from_malloc(unsigned
#ifndef DISABLE_DYNAMIC_MEMORY
i
#endif
) {
# ifndef DISABLE_DYNAMIC_MEMORY
    free_item *block = static_cast<free_item*>(malloc(exp2(i)));
    assert(!freelists[i]);
    block->next = 0;
    freelists[i] = block;
#  ifdef ENABLE_COUNT_MAX_POOL_SIZE
    inc(freelist_size[i]);
#  endif

    // use malloc, in case new is defined to call L2_alloc which would be
    // recursive
    chunk_descriptor *cd = new chunk_descriptor;
    cd->base = block;
    cd->nbytes = exp2(i);
    cd->next = chunks;
    chunks = cd;

# else
    _STD_ cerr << "Array_allocator: out of memory\n";
#ifdef ENABLE_ABORT
    // Note: --enable-abort is not yet a configure option
    abort();
#else
    exit(1);
#endif
# endif
}


/** Push a block onto the free list */

static void push_free_block(unsigned i, free_item* block) {
  assert(block != 0);        // precondition
  block->next = freelists[i];
  freelists[i] = block;
#ifdef ENABLE_COUNT_MAX_POOL_SIZE
  inc(freelist_size[i]);
#endif
}

/** Pop a block from the free list */

static void* pop_free_block(unsigned i) {
  assert(freelists[i] != 0);             // precondition
  free_item* result = freelists[i];
  freelists[i] = result->next;
#ifdef ENABLE_COUNT_MAX_POOL_SIZE
  dec(freelist_size[i]);
#endif
  return static_cast<void *>(result);
}

static void *L2_allocate_exp2(unsigned logN) {
  assert(INITIAL_BLOCK_LG2 <= MAX_ITEM_SIZE); // Invariant
  assert(logN < MAX_ITEM_SIZE);               // Precondition
  unsigned isf;
  for (isf = logN; freelists[isf] == 0 && isf < INITIAL_BLOCK_LG2; isf++) { }
  assert(INITIAL_BLOCK_LG2 <= isf || 0 != freelists[isf]); // postcondition
  if (0 == freelists[isf]) { // isf == INITIAL_BLOCK_LG2
    get_one_block_from_malloc(isf);
  }
  assert(freelists[isf] != 0); // Invariant if the program didn't exit
  // isf is the index of the smallest freelist whose size is >= logN.
  // Cascade splits down from i to logN
  unsigned full = isf;
  for (full = isf; full > logN; full--) {
    unsigned half = full - 1;
    // toSplit must be some kind of char for splitting to work
    char* toSplit = static_cast<char*>(pop_free_block(full));
    push_free_block(half, (free_item*)toSplit);
    push_free_block(half, (free_item*)&toSplit[exp2(half)]);
  }
  assert(full == logN && freelists[logN] != 0); // postcondition
  void* result = pop_free_block(logN);
#ifdef ENABLE_COUNT_MAX_POOL_SIZE
  inc(allocation_count[logN]);
  inc(num_outstanding[logN]);
#endif
#ifdef ENABLE_MEMORY_CHECKS_1
  assert(is_from_our_chunks(result));
#endif
  return result;
}

/**
 * The public function, allocate sz bytes.
 */
void * L2_alloc(size_t sz) {
    if(!is_ready) {
        // set up the list of chunks.  We have only the one chunk.
        initial_chunk.base = initial_block;
        initial_chunk.next = NULL;
        initial_chunk.nbytes = INITIAL_BLOCK_BYTES;
        chunks = &initial_chunk;

        // now, just the same as if we were reclaiming all the memory!
        L2_alloc_reclaim();
        is_ready = true;
    }
    // allocate 2^(ceil(lg(sz))), which is sz bytes or the next power of two
    // Ensure sz is large enough to hold a free_item; otherwise, we can't
    // keep the freelists.
    sz = ensure_minimal_block_size(sz);
    return L2_allocate_exp2(lg2(sz));
}


/***************************************************************************
  Deallocation.
 ***************************************************************************/

void L2_free(void* ptr, size_t sz) {
    if(!ptr) return;
    sz = ensure_minimal_block_size(sz);
    unsigned i = lg2(sz);

    assert(i<MAX_ITEM_SIZE);
# ifdef ENABLE_COUNT_MAX_POOL_SIZE
    assert(num_outstanding[i]>0);
# endif
# ifdef ENABLE_MEMORY_CHECKS_1
    assert(size_of_chunk(ptr) >= sz);
# endif
# ifdef ENABLE_MEMORY_CHECKS_3
    assert(!is_on_freelist(i, ptr));
# endif

    free_item *p = (free_item*)ptr;
    p->next = freelists[i];
    freelists[i] = p;

# ifdef ENABLE_COUNT_MAX_POOL_SIZE
    inc(freelist_size[i]);
    dec(num_outstanding[i]);
# endif
}


/***************************************************************************
  Others.
 ***************************************************************************/

void L2_alloc_reclaim() {
    // set up the freelists again, based on the chunks we have allocated
    memset(freelists, 0, MAX_ITEM_SIZE * sizeof(free_item*));

# ifdef ENABLE_COUNT_MAX_POOL_SIZE
    memset(allocation_count, 0, MAX_ITEM_SIZE * sizeof(unsigned));
    memset(freelist_size   , 0, MAX_ITEM_SIZE * sizeof(unsigned));
    memset(num_outstanding , 0, MAX_ITEM_SIZE * sizeof(unsigned));
# endif

    // foreach chunk, put it on the freelists.
    // TODO: handle chunks of off-sizes -- not power of two
    // At least check for them!
    for(chunk_descriptor *d = chunks ; d ; d = d->next) {
        unsigned i = lg2(d->nbytes);
        freelists[i] = static_cast<free_item*>(chunks->base);
        freelists[i]-> next = NULL;
# ifdef ENABLE_COUNT_MAX_POOL_SIZE
        inc(freelist_size[i]);
# endif
    }
}


void reset_memory_flag() { is_ready = false; }

#endif /* DISABLE_L2_ALLOCATORS */
