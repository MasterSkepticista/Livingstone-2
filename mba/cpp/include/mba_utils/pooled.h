/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: pooled.h,v 1.11 2002/01/03 00:07:03 lbrown Exp $ */
#ifndef pooled_HEADER
#define pooled_HEADER

#include "array_allocator.h"

/**
 * A superclass for all objects in Livingstone.
 * This provides new and delete operators that use the
 * allocation routines of Array_allocator.
 */

class Pooled {
    public:
        /**
         * Allocate from the L2 pools.
         */
        void *operator new  (size_t sz) { return L2_alloc(sz); }

        /**
          * Deallocate back into the L2 pools.
          */
        void operator delete  (void *v, size_t sz) { L2_free(v, sz); }

    protected:
        /**
         * Protected constructor.
         * The function does nothing; it just makes it impossible to
         * allocate instances of class Pooled -- you have to create a subclass
         * instead.
         */
        Pooled() { }

        /**
         * Protected destructor.
         * The function does nothing; it just makes it impossible to
         * deallocate something only known as class Pooled -- you must
         * call delete on a subclass.
         * Non-virtual to avoid creating a virtual function table if it's
         * not needed, thus saving time and space.
         */
        ~Pooled() { }

};


#endif
