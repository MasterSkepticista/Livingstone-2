/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <cover/cover.h>
#include <cover/cover_tracker.h>
#include <livingstone/L2_assert.h>
#include <livingstone/L2_iostream.h>

#undef TRACKER_DEBUG

void Cover_tracker::extend_candidates() {
  L2_throw(L2_disabled_error,
	   ("Cover tracker does not support extend_candidates"));
}


void Cover_tracker::find_fresh_candidates() {
  Candidate::ListOfCandidate& candidates = get_t_system().getCandidates();
  // This should only be called when we have blown out all of the current
  // candidates we were tracking.  If not, we just erase them.
  if (!candidates.empty()) { candidates.erase(); }
  if (get_t_system().get_conflicts()->empty()) {
    // If there are no conflicts, then there are no candidates that cover them.
    _STD_ cerr << "Cover_tracker::find_fresh_candidates() called when"
	      << " there are no conflicts" << _STD_ endl;
  } else if (gamma > gamma_limit) {
#ifdef TRACKER_DEBUG
    _STD_ cout << "Exceeded rho limit " << gamma_limit << _STD_ endl;
#endif
  } else {
    while (candidates.empty()) {
      // Since all candidates at the last likelihood level have been
      // eliminated, there's no sense searching at that level.
      ++gamma;
      if (gamma > gamma_limit) {
#ifdef TRACKER_DEBUG
	_STD_ cout << "Exceeded rho limit " << gamma_limit << _STD_ endl;
#endif
	return;
      }
#ifdef TRACKER_DEBUG
      _STD_ cout << "Calling rho at " << gamma << _STD_ endl;
#endif
      rho_coverage (&get_t_system(), candidates, gamma);
    }
  }
}


// This is deprecated. It is identical to Tracker::prune_and_search().

void Cover_tracker::find_candidates() {
  get_t_system().prune_inconsistent_candidates();
  if (get_t_system().getCandidates().empty()) { find_fresh_candidates(); }
}

