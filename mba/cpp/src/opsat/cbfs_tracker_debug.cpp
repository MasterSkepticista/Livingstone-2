/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// cbfs_tracker_debug.cpp

#include <opsat/cbfs_tracker_debug.h>

bool CBFS_tracker_debug::parse_cmd(const char *cmd, const char* args) {
  if (!strcmp("nc",cmd)) {
    Candidate *cand = get_cbfs_tracker()->next_consistent_candidate();
    if (cand != 0) { get_ostream() << cand << _STD_ endl; }
    else           { get_ostream() << "No candidate found" << _STD_ endl; }
    return true;
  } else if (!strcmp("extend", cmd)) {
    start_find_candidates();
    get_tracker()->extend_candidates();
    end_find_candidates();
  }

  // See if the parser common to all trackers can handle the
  // command for us.
  return this->Tracker_debug::parse_cmd(cmd,args);
}


void CBFS_tracker_debug::listener_usage() const {
  get_ostream()
    << "CBFS Tracker commands:\n"
    << "----------------------\n"
    << "  nc                          --> next candidates\n"
    << "  extend                      --> find extensions to existing candidates\n"
    << _STD_ endl;
  Tracker_debug::listener_usage();
}


MBA_string CBFS_tracker_debug::search_termination() const {
  char reason_string[MBA_MAX_LINE_SIZE];
  SearchTermination reason = get_tracker()->search_termination();
  const CBFS_tracker& cbfs_tracker = *(get_cbfs_tracker());
  const unsigned candidateCount =
    cbfs_tracker.get_t_system().getCandidates().size();
  const unsigned partitionSize = cbfs_tracker.getCandidatePartition().size();
  const unsigned searchedNodeCount = cbfs_tracker.get_candidates_considered();

  // in case we reach default
  reason_string[0] = '\0';

  switch (reason) {
  case SEARCH_CONTINUE:
    if ((cbfs_tracker.get_max_candidate_classes() > 0) &&
	(cbfs_tracker.get_max_candidate_classes() == partitionSize) &&
	(candidateCount < cbfs_tracker.get_number_tracked())) {
      sprintf(reason_string,
	      "CBFS: search found %d candidate(s) in %d class(es), \
more possible (searched %u)",
	      candidateCount,
	      partitionSize,
	      searchedNodeCount);
    } else {
      sprintf(reason_string,
	      "CBFS: search found %d candidate(s), more possible (searched %u)",
	      candidateCount,
	      searchedNodeCount);
    }
    break;
  case EMPTY_AGENDA:
    sprintf(reason_string,
	    "CBFS: exhaustive search, returned fewer than %d candidate(s) \
(searched %u)",
	    cbfs_tracker.get_number_tracked(),
            searchedNodeCount);
    break;
  case WEIGHT_CUTOFF:
    sprintf(reason_string,
	    "CBFS: search candidate(s) limited by max rank = %d (searched %u)",
	    cbfs_tracker.get_cutoff_weight(),
            searchedNodeCount);
    break;
  case MAX_OVERRUN:
    sprintf(reason_string,
	    "CBFS: search candidate(s) limited by max search space = %u",
	    cbfs_tracker.get_max_candidates());
    break;
    
    // no default; therefore we get a warning if new reasons
    // are added.
  }
  
  return
    (reason_string[0] == 0) ? "CBFS: unknown termination!" : reason_string;
}
