/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: enumeration.cpp,v 1.10 2001/11/16 19:32:34 lbrown Exp $ */
#include <readers/enumeration.h>

#ifdef ENABLE_L2_DEBUG_SECTIONS
// Allocate and initialize the array of element names

dbg_L2rEnumeration::dbg_L2rEnumeration(unsigned eid,
				       unsigned n,
				       const MBA_string& ename,
				       const MBA_string *memnames)
  : L2rEnumeration(eid,n), enum_name_(ename)
{
    member_name_ = L2_alloc_array(MBA_string, n);
    for (size_t i = 0; i < n; i++) {
      member_name_[i] = memnames[i];
    }
}

// Delete the array of element names

dbg_L2rEnumeration::~dbg_L2rEnumeration()
{
  L2_free_array(member_name_, nmembers());
}
#endif
