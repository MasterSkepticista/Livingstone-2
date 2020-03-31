/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: cbfs_tracker.cpp,v 1.38 2002/11/08 00:34:53 lbrown Exp $ */

#include <opsat/cbfs_tracker.h>

// Constructor with only number of candidates to track specified
// Assumes start with the empty initial candidate

CBFS_tracker::CBFS_tracker(T_system& t_system,
			   unsigned N,
			   unsigned max_class,
			   unsigned max_cands,
			   unsigned int max_cutoff_weight) :
  Tracker(t_system),
  number_tracked(N),
  maxClassCount(max_class),
  opsat(&get_t_system(),
	0,
	static_cast<Assignment::weight_t>(max_cutoff_weight),
	max_cands)
{
  extend_candidates();
}


// Clears the current candidates and restarts the search with an empty agenda
// It looks like it uses Tracker::candidates() to hold the temporary set of
// Candidates only because Tracker::erase() can clean it up.

void CBFS_tracker::find_fresh_candidates()
{
  // Make Tracker:candidates the empty set so that it can be used to collect
  // the Candidates from the State Variable
  get_t_system().eraseCandidates();

  // Add each Assignment in the State Variable to the set of Candidates
  Assumption *pStateVariable = get_t_system().get_state_variable();
  for (Assignable::iterator it = pStateVariable->assignments_begin();
       it != pStateVariable->assignments_end(); ++it) {
    Assignment *pAssignment = *it;
    Candidate *pStateCandidate = new Candidate;
    pStateCandidate->add(pAssignment);
    get_t_system().getCandidates().push_front(pStateCandidate);
  }

  extend_candidates();
}


// proceeds finding candidates starting with current set
void CBFS_tracker::extend_candidates()
{
  // Clear the candidatePartition
  eraseCandidatePartition();
  Candidate::ListOfCandidate& candidates = get_t_system().getCandidates();
  // Insert the current Candidate set into the CBFSAgenda
  opsat.initialize_search(candidates);
  // When the constructor is called, the T_system might not exist
  opsat.set_t_system(&get_t_system());
  // Make Tracker::candidates the empty set
  get_t_system().eraseCandidates();
  // The central "find candidates" operation
  opsat.find_consistent_candidates(candidates, number_tracked);
}
