/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: L2_stream.h,v 1.4 2004/01/29 00:17:00 lbrown Exp $

#ifndef L2_STREAM_H
#define L2_STREAM_H

/*
 * g++ 3.2 and later deprecates <*.h> header files to be ANSI compliant, but
 * the C++ compiler for VxWorks 5.3.1, which uses g++ 2.7.2, does not support
 * namespaces, notably the STD__ namespace
 * <iOSTREAM>
 * For maximum compatibility with old compilers, don't use namespaces if they
 * can be avoided
 */

#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ > 1) || defined(_MSC_VER)
// <*.h> is deprecated, so don't use it
#define _STD_ std::
#else 
// <*.h> is not deprecated, so use it
#define _STD_ 
#endif

#endif // L2_STREAM_H
