/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: tracker.cpp,v 1.113 2004/01/22 01:21:44 lbrown Exp $ */

#include <transition/tracker.h>
#include <livingstone/L2_assert.h>

// The do-while(0) is the only portable way to block.
#ifdef DISABLE_TRACKER_LISTEN
#  define record(X)
#else
#  include <transition/tracker_listener.h>
#  define record(call_with_args) \
   do { \
     Slist<Tracker_listener*>::iterator listen_it##__LINE__ \
         = listeners_.begin(); \
     while(listen_it##__LINE__ != listeners_.end()) { \
       (*listen_it##__LINE__)->call_with_args;  \
        ++listen_it##__LINE__; \
     } \
   } while(0)
#endif


#ifndef DISABLE_CBFS
const Tracker::Search_style Tracker::DEFAULT_SEARCH_STYLE = Tracker::cbfs;
#else
const Tracker::Search_style Tracker::DEFAULT_SEARCH_STYLE = Tracker::cover;
#endif


// Set member variables and do some validity checking

Tracker::Tracker(T_system& t_system) :
  t_system(t_system), fc_style(tracker_default_fc) {

  // The CBFS tracker does not seem to like this so we've put it just in the
  // cover tracker. It should placed here once the issues with CBFS are fixed.
  // candidates.push_front(new Candidate());

  L2_assert(t_system.get_max_history_length() == 0 ||
	    t_system.get_max_history_length() >=
	    unsigned(T_system::MIN_HISTORY_LENGTH),
	    L2_argument_error,
	    ("history " + MBA_string(t_system.get_max_history_length()) +
	     " less than minimum " + MBA_string(T_system::MIN_HISTORY_LENGTH)));
}


void Tracker::eraseCandidatePartition() {
  for (unsigned i = 0; i < candidatePartition.size(); i++) {
    delete candidatePartition[i]; // delete the equivalence class object
  }
  candidatePartition.erase(); // make the partition of size 0
}


unsigned Tracker::equivalenceClassIndex(const Candidate& candidate) const {
  const unsigned count = candidatePartition.size();
  for (unsigned i = 0; i < count; i++) {
    const CandidateEqClass* const pCandidateEqClass = candidatePartition[i];
    if (pCandidateEqClass->belongs(candidate, t_system)) { return i; }
  }
  return count; // Need a new equivalence class
}


void Tracker::partition() {
  eraseCandidatePartition(); // Clear the old partition
  for (Tracker::iterator it = begin(); it != end(); ++it) {
    const Candidate* const pCandidate = *it;
    unsigned classIndex = equivalenceClassIndex(*pCandidate);
    if (classIndex < candidatePartition.size()) { // equivalence class exists
      candidatePartition[classIndex]->add(*pCandidate);
    } else { // equivalence class has to be created
      // There is no equivalence class for the Candidate; create a new one
      candidatePartition.push(new CandidateEqClass(*pCandidate));
    }
  }
}
