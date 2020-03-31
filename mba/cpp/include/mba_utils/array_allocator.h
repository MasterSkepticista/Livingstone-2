/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: array_allocator.h,v 1.13 2006/05/02 00:04:55 taylor Exp $ */
#ifndef array_allocator_HEADER
#define array_allocator_HEADER

/**
 * The main defines we use are:
 *     - ENABLE_COUNT_ALLOCATED, which keeps track of how many objects we've
 *              allocated, how many are on the freelists, etc.  Use
 *              L2_alloc_print_stats.
 *     - DISABLE_DYNAMIC_MEMORY, in which case we don't malloc when we run
 *              out of memory.  Make sure the INITIAL_BLOCK_LG2 variable
 *              if large enough.  As is now, this may need some modification
 *              for flight, i.e. to statically allocate something other than a
 *              power of two.
 *     - ENABLE_MEMORY_CHECKS_1, enable the cheapest memory checks
 *     - ENABLE_MEMORY_CHECKS_2, enable higher computational cost checks
 *     - ENABLE_MEMORY_CHECKS_3, enable impractically expensive checks
 */

#include <livingstone/l2conf.h>

#include <stddef.h> // size_t

/**
 * Older systems don't have this header file.
 */
#if defined(HAVE_NEW) || defined(_MSC_VER)
#  include <new>
#endif

/***************************************************************************
  Allocating one element.
 ***************************************************************************/

/**
 * Allocate sz bytes of memory.  The memory must be deallocated by L2_free.
 */
extern void *L2_alloc(size_t sz);

/**
  * Deallocate sz bytes of memory in a block that starts at ptr.
  * The memory must have been allocated by L2_alloc.
  * This is a no-op if ptr is null.
  */
extern void L2_free(void *ptr, size_t sz);




/***************************************************************************
  Allocating an array of elements.
 ***************************************************************************/

/**
 * Allocate nel elements of Type.  The memory must be deallocated by
 * L2_free_array.  If the type has no constructor (i.e. pointers, integers,
 * etc), consider using L2_alloc_array_no_ctor instead.
 */
#define L2_alloc_array(Type, nel) L2_initialize_array( \
        (Type*)L2_alloc_array_no_ctor(Type, nel), nel )

/**
 * Deallocate an array of nel elements, the first element of which is at ptr.
 * The memory must have been gotten from L2_alloc_array.  If the type has no
 * destructor (i.e. pointers, integers, etc), consider using
 * L2_free_array_no_dtor instead.
 */
template <class T> void L2_free_array(T *ptr, unsigned nel) {
    // call the destructors
    for(unsigned i=0; i<nel; ++i) {
        // ptr[i].T::~T();
        T t = ptr[i];
	t.~T();    
    }
    // then deallocate the array
    L2_free_array_no_dtor(ptr, nel);
}

/**
 * Allocate nel elements of Type ; the constructor will not be called.
 * This saves time on types that have no constructor.
 */
#define L2_alloc_array_no_ctor(Type, nel) (Type*)L2_alloc(sizeof(Type)*nel)

/**
 * Deallocate an array of nel elements of Type ; the destructor will not be
 * called.  This saves time on types that have no destructor.
 */
template <class T> void L2_free_array_no_dtor(T *ptr, unsigned nel) {
    L2_free(ptr, nel*sizeof(T));
}



/***************************************************************************
  Observing the state of allocations.
  This part of the interface should be used with caution.
 ***************************************************************************/

/**
 * Print statistics about allocations.
 * What's collected depends on configure switches.
 * More details later.
 */
extern void L2_alloc_print_stats();

/**
  * Reclaim all the memory.
  * This is obviously quite dangerous -- if anything outstanding still
  * wants to use it!  You should make sure you've popped out of all
  * functions before doing this.
  */
extern void L2_alloc_reclaim();


/**
 * Set static flag is_ready to false so that L2_alloc() re-initializes the
 * memory allocator.
 */
extern void reset_memory_flag();


/***************************************************************************
  For internal use only.
  We need this to appear in a publicly accessable place for C++ to do the
  right thing ; but you shouldn't be using this directly.
 ***************************************************************************/

/**
 * Define a placement-new operator on platforms that don't have it.
 */
#if (!defined(_MSC_VER) && !defined(HAVE_PLACEMENT_NEW)) || \
    ( defined(_MSC_VER) && !defined(__PLACEMENT_NEW_INLINE))
inline void *operator new(size_t, void *p) { return p ; }
#endif

/**
 * Don't use this directly.
 * This initializes all elements in 'base' using the placement new operator,
 * in effect an explicit call to the constructor.
 * This is used by L2_alloc_array.  It returns base to make the macro
 * easier to write.
 */
template <class T> T *L2_initialize_array(T *base, unsigned nel) {
    for(unsigned i=0; i<nel; ++i)
        ::new (base+i) T;
    return base;
}

#endif
