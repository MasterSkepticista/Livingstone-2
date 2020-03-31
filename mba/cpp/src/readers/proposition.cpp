/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: proposition.cpp,v 1.11 2004/01/29 00:39:21 lbrown Exp $ */
#include <readers/proposition.h>

_STD_ ostream& operator<<(_STD_ ostream& os,
			  const L2rPropVarVar& l2rPropVarVar) {
  os << "[ID1="
     << l2rPropVarVar.var()->id()
     << ", ID2="
     << l2rPropVarVar.otherVar()->id();
  os << "]";
  return os;
}
