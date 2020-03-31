/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: variable.cpp,v 1.12 2002/06/20 20:10:28 lbrown Exp $ */

#include <readers/variable.h>

void L2rVariable::setKind(VarKind vk) {
  // Forbidden to call this method twice with different non-UNKNOWN parameters 
  L2_assert(kind_ == vk_unknown || kind_ == vk,
	    L2_fatal_error,
            ("Resetting kind of " + MBA_string(id())+" to " +
             VarKind_to_string(vk) + " from " + VarKind_to_string(kind_)));
  kind_ = vk;
  if (kind() == vk_mode && !mode()) { mode_ = new L2rMode; } 
}


void L2rVariable::setInitial(int i) {
  // Forbidden to call this method twice with different non-UNKNOWN parameters
  L2_assert(initial_== NO_INITIAL_VALUE || initial_ == i,
	    L2_fatal_error,
            ("Resetting initial value of " + MBA_string(id()) + " to " +
             MBA_string(i) + " from " + MBA_string(initial_)));
  initial_ = i;
}

void L2rVariable::shadowInitial(int newInitialValueIndex) {
  // Save the exiting initial value. This can be done only once.
  savedInitial_ = initial_;
  // Set the new initial value.
  initial_ = newInitialValueIndex;
}

void L2rVariable::restoreInitial() {
  if (savedInitial_ != initial_) {
    initial_ = savedInitial_;
  }
}
