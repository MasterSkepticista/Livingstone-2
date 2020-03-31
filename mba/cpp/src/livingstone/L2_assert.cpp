/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_assert.cpp,v 1.6 2004/01/28 23:31:47 lbrown Exp $ */

#include <livingstone/L2_assert.h>

#if !defined(ENABLE_EXCEPTIONS) && !defined(ENABLE_PRINT_ERROR)

#include <stdlib.h>   // abort

extern "C" void L2_error_function(const char *xcp_name,
        const char *filename,
        unsigned linenum) {
  _STD_ cout.flush();
  _STD_ cerr << xcp_name << " at " << filename << ':' << linenum << _STD_ endl;
#ifdef ENABLE_ABORT
  // Note: --enable-abort is not yet a configure option
  abort();
#else
  exit(1);
#endif
}

#endif
