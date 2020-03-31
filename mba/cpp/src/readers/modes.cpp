/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: modes.cpp,v 1.7 2001/11/16 19:32:36 lbrown Exp $ */

#include <readers/modes.h>
#include <readers/transition.h>

// Delete and erase all nominal and failure transitions

L2rMode::~L2rMode() {
  {
    for (iterator it = begin_nominal(); it != end_nominal(); ) {
      // Cast to allow delete
      L2rTransition *pL2rTransition = const_cast<L2rTransition*>(*it);
      delete pL2rTransition;
      nominal_.erase(it);
    }
  }
  {
    for (iterator it = begin_failure(); it != end_failure(); ) {
      // Cast to allow delete
      L2rTransition *pL2rTransition = const_cast<L2rTransition*>(*it);
      delete pL2rTransition;
      failure_.erase(it);
    }
  }
}


// Insert the transition in increasing order of rank. Note that, since rank
// is -log(prior probability), these are in decreasing order of probability.

void L2rMode::add_transition(const L2rTransition *x) {
  if (x->isNominal()) {
    L2_assert(!nominal_.contains(x),
	      L2_fatal_error,
	      ("duplicate copy of transition in nominal modes"));
    nominal_.push_front(x);
  } else {
    for (Slist<const L2rTransition*>::iterator it = failure_.begin();
	it!=failure_.end(); ++it) {
      const L2rTransition *pL2rTransition = *it;
      if (x->rank() < pL2rTransition->rank()) {
	failure_.insert_before(it, x);
	return;
      }
    }
    // if we get here, we are > than anything.
    failure_.push_back(x);
  }
}
