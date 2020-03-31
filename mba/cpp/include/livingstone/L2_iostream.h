/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: L2_iostream.h,v 1.2 2004/01/28 23:55:56 lbrown Exp $

#ifndef L2_IOSTREAM_H
#define L2_IOSTREAM_H

#include <livingstone/L2_stream.h>

#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ > 1) || defined(_MSC_VER)
// <*.h> is deprecated, so to avoid warnings don't use it
#include <iostream>
#else 
// <*.h> is not deprecated, so use it
#include <iostream.h>
#endif

#endif // L2_IOSTREAM_H
