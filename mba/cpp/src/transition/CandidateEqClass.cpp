/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: CandidateEqClass.cpp,v 1.5 2003/01/09 19:17:40 lbrown Exp $ */

#include <transition/CandidateEqClass.h>
#include <debuggers/trajectory.h>
#include <transition/T_system.h>

/**
 * Do the two assignments have the same variableID, same valueIndex and
 * different timeStep?
 * \param assignment1 an Assignment
 * \param assignment2 an Assignment
 */
static bool isTimePermutation(const Assignment& assignment1,
			      const Assignment& assignment2) {
  // It probably won't help to explicitly test &assignment1==&assignment2
  const Assignable& assignable1 = *(assignment1.get_variable());
  const Assignable& assignable2 = *(assignment2.get_variable());
  return
    assignable1.get_id()        == assignable2.get_id() &&
    assignable1.get_time_step() != assignable2.get_time_step() &&
    assignment1.get_index()     == assignment2.get_index();
}


bool CandidateEqClass::isEquivalent (const Candidate& candidate1,
				     const Candidate& candidate2) const {
  unsigned size = candidate1.size();
  if (size != candidate2.size()) { return false; } // must be same size
  else {
    for (unsigned i = 0; i < size; i++) {
      // For each Assignment in Candidate1
      const Assignment* const pAssignment1 = candidate1.get_assignment(i);
      // Does Candidate2 contain an "equivalent" Assignment?
      bool found = false;
      for (unsigned j = 0; j < size; j++) {
	const Assignment* const pAssignment2 = candidate2.get_assignment(j);
	if ((pAssignment1 == pAssignment2) ||
	    isTimePermutation(*pAssignment1, *pAssignment2)) {
	  found = true;
	  break;
	}
      }
      if (!found) {
	// Candidate1 contains an Assignment such that Candidate2 does not
	// contain a time permutation of it
	return false;
      }
    }
    // For each Assignment in Candidate1, Candidate2 contains an Assignment
    // that is a time permutation of it
    return true;
  }
}


bool CandidateEqClass::isEquivalent (const Slist<const Triple*>& ct1,
				     const Slist<const Triple*>& ct2) const {
  unsigned size = ct1.size();
  if (size != ct2.size()) { return false; } // must be same size
  else {
    for (Slist<const Triple*>::const_iterator it1 = ct1.begin();
	 it1 != ct1.end(); ++it1) {
      // For each Triple in ct1
      const Triple* const pTriple1 = *it1;
      // Does ct2 contain an "equivalent" Triple?
      bool found = false;
      for (Slist<const Triple*>::const_iterator it2 = ct2.begin();
	   it2 != ct2.end(); ++it2) {
	const Triple* const pTriple2 = *it2;
	if ((pTriple1->variableID == pTriple2->variableID) &&
	    (pTriple1->valueIndex == pTriple2->valueIndex)) {
	  found = true;
	  break;
	}
      }
      if (!found) {
	// Candidate1 contains an Assignment such that Candidate2 does not
	// contain a time permutation of it
	return false;
      }
    }
    // For each Assignment in Candidate1, Candidate2 contains an Assignment
    // that is a time permutation of it
    return true;
  }
}


void CandidateEqClass::createTriples(Slist<const Triple*>& triples,
				     const Candidate& candidate,
				     const T_system& t_system) const {
  for (unsigned i = 0; i < candidate.size(); i++) {
    const Assignment* const pAssignment = candidate.get_assignment(i);
    const Assignable* const pAssignable = pAssignment->get_variable();
    if (pAssignable->is_state_variable()) {
      // Get the trajectory that corresponds to the State Variable's value
      unsigned valueIndex = pAssignment->get_index();
      const Trajectory& trajectory = t_system.get_trajectory(valueIndex);
      // Add each Parts to the triples list
      for (Trajectory::iterator it = trajectory.begin();
	   it != trajectory.end(); ++it) {
	const Trajectory::Parts& parts = *it;
	triples.push_back(new Triple(parts.get_id(),
				     parts.get_time(),
				     parts.get_index()));
      }
    } else {
      // Add the Assignment to the triples list
      triples.push_back(new Triple(pAssignable->get_id(),
				   pAssignable->get_time_step(),
				   pAssignment->get_index()));
    }
  }
}


void CandidateEqClass::deleteTriples(Slist<const Triple*>& triples) const {
  for (Slist<const Triple*>::iterator it = triples.begin();
       it != triples.end(); ++it) {
    const Triple* pTriple = *it;
    delete const_cast<Triple *>(pTriple);
  }
}


bool CandidateEqClass::belongs(const Candidate& candidate,
			       const T_system& t_system) const {
  bool result = true; // the default
  // Convert Candidates to Slist of Triple ptrs and call isEquivalent() on them
  Slist<const Triple*> ct1;
  createTriples(ct1, candidate, t_system);
  for (unsigned i = 0; i < size(); i++) {
    const Candidate* const pComparisonCandidate = get(i);
    Slist<const Triple*> ct2;
    createTriples(ct2, *pComparisonCandidate, t_system);
    if (!isEquivalent(ct1, ct2)) {
      // It is not equivalent to one tested element of candidateEqClass
      result = false;
    }
    deleteTriples(ct2);
    if (!result) { break; }
  }
  // On normal exit it is equivalent to all tested elements of candidateEqClass
  deleteTriples(ct1);
  return result;
}


bool CandidateEqClass::contains(const Candidate& candidate) const {
  for (unsigned i = 0; i < size(); i++) {
    if (candidates[i] == &candidate) { return true; }
  }
  return false;
}
