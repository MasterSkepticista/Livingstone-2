/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Implementation of the hash table class.
// $Id: hash_table.cpp,v 1.6 2001/12/28 19:46:58 lbrown Exp $

#include <mba_utils/hash_table.h>

/**
 * An implementation of the hashing function used by P.J. Weinberger's C
 * compiler.
 * Taken from the Compilers book by Aho, Sethi, Ullman (page 436).  
 * The function is computed by starting with h=0. For each character c, shift
 * the bits of h left 4 positions and add in c. If any of the four high-order
 * bits of h is 1, shift the four bits right 24 positions, exclusive-or them
 * into h, and reset to 0 any of the four high order bits that was 1. The
 * function assumes that unsigned ints are 32 bits long.
 * \param str the MBA_string to be hashed
 * \return the hash code for the string
 */

unsigned int hashpjw(const MBA_string& str) {
  const char * p = str.c_str();
  unsigned int h = 0;

  for ( ; *p != '\0'; ++p) {
    h = (h << 4) + *p;
    unsigned int g = h & 0xf0000000;
    if (g) {
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }
  return h;
}
