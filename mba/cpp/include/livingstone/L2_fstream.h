/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: L2_fstream.h,v 1.2 2004/01/28 23:55:56 lbrown Exp $

#ifndef L2_FSTREAM_H
#define L2_FSTREAM_H

#include <livingstone/L2_stream.h>

#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ > 1) || defined(_MSC_VER)
// <*.h> is deprecated, so to avoid warnings don't use it
#include <fstream>
#else 
// <*.h> is not deprecated, so use it
#include <fstream.h>
#endif

#endif // L2_FSTREAM_H
