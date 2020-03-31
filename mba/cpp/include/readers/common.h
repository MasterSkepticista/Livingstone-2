/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: common.h,v 1.17 2004/02/18 23:50:16 lbrown Exp $ */
#ifndef COMMON_H
#define COMMON_H

#define L2_READER_MAGIC "L211"

#include <livingstone/L2_assert.h>

/**
 * Return array[i], checking that i < size.
 * The template ensures we can't use this as an l-value.
 * Gcc-3.0 requires both templates in order to work with either
 * passing in T const * const * or T**.
 */
template <class T>
const T *bounds_check_access(unsigned i,
			     unsigned size,
			     const T *const * array,
			     const char *name) {
  L2_assert(i<size, L2_bounds_error, (MBA_string(name)+" array", i, size));
  return array[i];
}

template <class T>
const T *bounds_check_access(unsigned i,
			     unsigned size,
			     T *const * array,
			     const char *name) {
  L2_assert(i<size, L2_bounds_error, (MBA_string(name)+" array", i, size));
  return array[i];
}

/**
 * Similarly, except array is only T**, and return is T*
 * We don't need to worry about the consts.
 */
template <class T>
T *bounds_check_access_friend(unsigned i,
			      unsigned size,
			      T ** array,
			      const char *name) {
  L2_assert(i<size, L2_bounds_error, (MBA_string(name)+" array", i, size));
  return array[i];
}

/***************************************************************************
  Several of the methods only need to be virtual if we have the debug
  versions of the objects.
 ***************************************************************************/
#ifdef ENABLE_L2_DEBUG_SECTIONS
#  define l2_virtual virtual
#else
#  define l2_virtual
#endif


#endif


