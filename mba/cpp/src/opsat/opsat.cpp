/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: opsat.cpp,v 1.49 2003/01/06 23:15:22 lbrown Exp $ */

#include <opsat/opsat.h>
#include <debuggers/trajectory.h>
#include <opsat/cbfs_tracker.h>
#include <transition/CandidateEqClass.h>

/**
 * 'best_weight' is the weight of the empty element or all-default candidate.
 * 'cutoff' is the weight below which the search will be terminated.
 * 'maximum_candidates' is the number of candidates which may be checked for
 * consistency before the search is terminated.
 */

Opsat_CBFS::Opsat_CBFS(T_system* pT_system,
		       Assignment::weight_t best_weight,
		       Assignment::weight_t cutoff,
		       unsigned maximum_candidates) :
  pT_system(pT_system),
  candidates_considered(0),
  cutoff_weight(cutoff),
  max_candidates(maximum_candidates),
  agenda(best_weight, LEASTFIRST) {
}


/**
 * This allows explicit search initialization with multiple candidates. The
 * agenda is emptied and elements corresponding to those in the initialization
 * list are added. The weight of the empty element or all-default candidate
 * must still be specified.
 */

void Opsat_CBFS::initialize_search(const Candidate::ListOfCandidate&
				   seedCandidates) {
  // empty the agenda and reinitialize
  agenda.initialize_agenda(seedCandidates);
}


/**
 * Attempts to return "number" consistent candidates. The search may be
 * terminated prematurely due to reasons found in the 'Termination' enum.
 */

void Opsat_CBFS::find_consistent_candidates(Candidate::ListOfCandidate&
					    consistentCandidates,
					    unsigned number) {
  candidates_considered = 0;
  while ( consistentCandidates.size() < number ) {
    Candidate* pCandidate = next_consistent_candidate();
    if (pCandidate == NULL) {
      return; // should send back reason
    } else {
      // First check if the Candidate is a superset of some other Candidate
      // already in the list. If so, reject it.
      if ( superset_candidate(*pCandidate, consistentCandidates) ) {
	delete pCandidate; // reject (and don't leak)
      } else {
	consistentCandidates.push_back(pCandidate);
	if (pT_system->getTracker() != NULL) {
	  // Not being called via CBFS_tracker::CBFS_tracker()
	  CBFS_tracker& tracker =
	    static_cast<CBFS_tracker&>(*pT_system->getTracker());
	  Array<CandidateEqClass*, false>& candidatePartition =
	    tracker.getCandidatePartition();
	  const unsigned classLimit = tracker.get_max_candidate_classes();
	  const unsigned classIndex =
	    tracker.equivalenceClassIndex(*pCandidate);
	  if (classIndex < candidatePartition.size()) {
	    // Add to the existing equivalence class
	    candidatePartition[classIndex]->add(*pCandidate);
	  } else {
	    // Create a new equivalence class
	    candidatePartition.push(new CandidateEqClass(*pCandidate));
	    if (classLimit > 0 && candidatePartition.size() >= classLimit) {
	      return;
	    }
	  }
	}
      }
    }
  }
}


/**
 * This function returns the next consistent Candidate.  It returns a
 * consistent Candidate as the data of the values in the current Candidate. If
 * the agenda is in decreasing (increasing) order, the CUTOFF-WEIGHT is the
 * least (greatest) weight considered. If no consistent Candidate is found,
 * either because the agenda is empty, or because of the cutoff weight, the
 * function returns NULL.
 * \return a pointer to the next Candidate, or NULL if there is none
 */

Candidate* Opsat_CBFS::next_consistent_candidate() {
  Candidate *pCandidate = new Candidate();
  // Loop until a candidate is found or the search must be terminated
  while (SEARCH_CONTINUE == terminate_search()) {
    // Copies the popped node into pCandidate; this clears old Assignments
    agenda.pop_candidate(*pCandidate);
    ++candidates_considered;
    pT_system->restore_assumption_defaults();
    pCandidate->assign();
    // First check whether Candidate has any previously-discovered unresolved
    // Conflicts. Note below will not generate Conflicts of all defaults
    const Conflict* pConflict =
      pT_system->conflict(pT_system->get_time_step());
    // Check if first consistent Candidate found. In Lisp engine the search
    // holds the weight of the first consistent Candidate -- not sure if
    // necessary here.
    agenda.expand_element(pConflict);
    if (pConflict == NULL) {
      // The theory is consistent; return this element's Candidate
      return pCandidate;
    } else {
      // pT_system->restore();
    }
  }
  // no candidate returned so free memory here
  delete pCandidate;
  return NULL;
} // end "next_consistent_candidate"


/**
 * .
 * The search is terminated if either the agenda is empty, we
 * have examined as many elements as allowed, or the leading
 * element's weight is less (greater) than the cutoff weight when the
 * agenda is in decreasing (increasing) order.
 */

SearchTermination Opsat_CBFS::terminate_search() const {
  if (agenda.empty_agenda())
    return EMPTY_AGENDA;
  else if ( (max_candidates != 0) && (candidates_considered > max_candidates) )
    return MAX_OVERRUN;
  else if ( (cutoff_weight != 0) && (agenda.top_weight() > cutoff_weight) )
    return WEIGHT_CUTOFF;
  else
    return SEARCH_CONTINUE;
}


const Assignment*
Opsat_CBFS::getStateVariableAssignment(const Candidate& candidate) const {
  for (unsigned i = 0; i < candidate.size(); i++) {
    const Assignment* const pAssignment = candidate.get_assignment(i);
    if (pAssignment->get_variable()->is_state_variable()) {
      return pAssignment;
    }
  }
  // Every Candidate in the Tracker must have a State Variable Assignment,
  // but during search Candidates can be lacking them.
  return NULL;
}


bool
Opsat_CBFS::subsumes(const Assignment* const pSuperStateVariableAssignment,
		     const Assignment* const pSubStateVariableAssignment) 
  const {
  if (pSuperStateVariableAssignment == NULL) { return false; }
  else if (pSubStateVariableAssignment == NULL) { return false; }
  else if (pSuperStateVariableAssignment == pSubStateVariableAssignment)
    { return true; }
  else {
    const Trajectory& superTrajectory = 
      pT_system->get_trajectory(pSuperStateVariableAssignment->get_index());
    const Trajectory& subTrajectory =
      pT_system->get_trajectory(pSubStateVariableAssignment->get_index());
    return superTrajectory.subsumes(subTrajectory);
  }
}


bool Opsat_CBFS::supersetWithState(const Candidate& super,
				   const Candidate& sub) const {
  const Assignment* const pSuperStateAssignment =
    getStateVariableAssignment(super);
  for (unsigned i = 0; i < sub.size(); i++) {
    const Assignment* pAssignment = sub.get_assignment(i);
    if (!pAssignment->get_variable()->is_state_variable()) {
      // If the Assignment is not the State Variable, super must have it
      if (!super.contains(pAssignment)) { return false; }
    } else {
      // If the Assignment is the State Variable, super must subsume sub
      if (!subsumes(pSuperStateAssignment, pAssignment)) { return false; }
    }
  }
  return true;  
}


/**
 * Returns true if the pTestCandidate completely contains any of the
 * Candidates in the "cands" list.
 */

bool
Opsat_CBFS::superset_candidate(const Candidate& testCandidate, 
			       const Candidate::ListOfCandidate&
			       candidates) const {
  for (Candidate::ListOfCandidate::const_iterator it = candidates.begin();
       it != candidates.end(); ++it) {
    Candidate *pCandidate = *it;
    if (supersetWithState(testCandidate, *pCandidate)) { return true; }
  }
  return false;
}
