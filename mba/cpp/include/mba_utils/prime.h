/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Code for primality testing
// $Id: prime.h,v 1.8 2001/11/16 19:32:11 lbrown Exp $
//

#ifndef PRIME_H
#define PRIME_H

#include <livingstone/l2conf.h>


#include <assert.h>
#include <limits.h>

inline unsigned int
modular_exponentiation(unsigned int a, unsigned int b, unsigned int n)
{
  unsigned int d = 1;

#ifdef WIN32
#pragma warning( disable : 4307 )
#endif

  unsigned int flag = (unsigned int)INT_MAX + 1;
// most significant bit should now be 1
// Explicit cast to avoid VwWorks integer overflow warning on expression

#ifdef WIN32
#pragma warning( default : 4307 )
#endif

  assert(n > 0);

  while (flag) {
    d = (d * d) % n;
    if (flag & b) {
      d = (d * a) % n;
    }
    flag >>= 1;
  }
  return d;
}

inline bool
pseudoprime(unsigned int n)
{
  if (modular_exponentiation(2, (n - 1), n) != 1)
    return false;
  else
    return true;
}


#endif // PRIME_H

