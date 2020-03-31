/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: candidate.cpp,v 1.57 2004/02/05 22:36:17 lbrown Exp $

#include <conflict_db/candidate.h>

// the symbol definition ; this is a static field.
unsigned Candidate::global_id = 0;


// Copy constructor

Candidate::Candidate(const Candidate &init) :
  Pooled(),  // make sure super is init
  id(++global_id) {
  for (Array<const Assignment*, false>::iterator it = init.assignments.begin();
       it != init.assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    assignments.push(pAssignment);
  }
}


// operator=
Candidate& Candidate::operator=(const Candidate &rhs) {
  // if (&init != this)
  assignments.erase();
  for (Array<const Assignment*, false>::iterator it = rhs.assignments.begin();
       it != rhs.assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    assignments.push(pAssignment);
  }
  // Give it a new ID, since it has a new identity
  id = ++global_id;
  return *this;
}



// For each Assignment, call its assign() method

void Candidate::assign() const {
  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    pAssignment->assign();
  }
}


// Remove the first instance of Assignment from the set; return true if removed

bool Candidate::remove(const Assignment* pAssignmentToRemove) {
  for (Array<const Assignment *, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    if (pAssignment == pAssignmentToRemove) {
      assignments.erase(it);
      return true; // assume only one instance to remove
    }
  }
  return false;
}


// Remove all Assignments whose Time Step is older than the horizon

void Candidate::truncate(Theory::time_step_t horizon,
			 Assignment& state_eq_si) {
  // Conflict_db shouldn't know about the T_system. There must be a better way.
  const unsigned BEFORE_HORIZON = (unsigned)-1;
  // The State Variable will have no more Assignments than this Candidate
  Array<const Assignment*, false> stateVariableAssignments(assignments.size());

  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); /* don't increment here */) {
    const Assignment* pAssignment = *it;
    if (pAssignment->get_time_step() < horizon ||
	pAssignment->get_time_step() == BEFORE_HORIZON) {
      // Truncate: add to weight, but delete the Assignment
      stateVariableAssignments.push(pAssignment);
      assignments.erase(it);
      // Don't increment it; it now points to an item not yet seen
    } else {
      // Don't truncate it
      ++it;
    }
  }
  // Add the state variable Assignment, with the given weight
  add(&state_eq_si);
  state_eq_si.get_variable()->set_weight(state_eq_si,
					 get_weight(stateVariableAssignments));
}


// Loop over assignments to find smallest unresolved conflict

const Conflict* Candidate::smallest_unresolved_conflict() const {

  // Unfortunately, some Conflicts may be resolved by default values of
  // Assignments (they may have resulted from other candidates) so we must
  // check for these
  // In fact, must check whether any other candidate Assignments resolve a
  // given Conflict

  const Conflict *pCurrentConflict = NULL; // the return value
  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    // Assuming Conflicts are ordered -- check if list empty first
    if (!pAssignment->get_conflicts().empty()) {
      const Conflict *pConflict = pAssignment->get_conflicts().front();
      if (pCurrentConflict == NULL) {
	// && conflict_not_resolved(pConflict) ??
	// Initialize the smallest conflict to date
	pCurrentConflict = pConflict;
      } else {
	if (conflict_not_resolved(pConflict) &&
	    *pConflict < *pCurrentConflict) {
	  pCurrentConflict = pConflict;
	}
      }
    } else {
      // No conflicts; return NULL
    }
  }
  return pCurrentConflict;
}


// Return all Assignments in the Candidate which involve the given Conflict.
// The return list is sorted by weight

void
Candidate::get_conflict_assignments(Conflict *const & pConflict,
				    Slist<const Assignment *> & conflictList) const {
  if (pConflict != NULL) {
    for (Array<const Assignment*, false>::iterator it = assignments.begin();
	 it != assignments.end(); ++it) {
      const Assignment* pAssignment = *it;
      if (pAssignment->get_conflicts().contains(pConflict) ) {
	// The Assignment contains the Conflict
	// Insert the Assignment into the return list in order of weight
	Assignment::weight_t weight = pAssignment->get_weight();
	// Declare outside loop because it will be used outside loop
	Slist<const Assignment*>::iterator assign_it = conflictList.begin();
	for (; (assign_it != conflictList.end()) &&
	       (weight > (*assign_it)->get_weight()); ++assign_it) { }
	conflictList.insert(assign_it, pAssignment);
      }
    }
  } else {
    // Ignore a null Conflict
  }
}


// This is like operator==(). Two Candidates are equivalent if they have the
// same sets of Assignments. Assuming that the Candidate::assignments contains
// no duplicates, test for equal cardinalities and subsethood.

bool Candidate::equivalent(const Candidate& other) const {
  // This assumes every unique assignment is represented by exactly one
  // Assignment object, which is an overall system assumption.
  // If the assignments sets have different cardinality, they can not be equal
  if (assignments.size() != other.assignments.size()) {
    return false;
  } else {
    // Is other a superset of this?
    for (Array<const Assignment*, false>::iterator it = assignments.begin();
	 it != assignments.end(); ++it) {
      const Assignment* pAssignment = *it;
      if (!other.contains(pAssignment)) { return false; }
    }
    return true;
  }
}


// Checks candidate for an inconsistent Assignment and swaps, if necessary

void Candidate::add_consistently(const Assignment *const & pNewAssignment) {
  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    if (pAssignment->is_inconsistent(*pNewAssignment)) {
      // Replace the existing Assignment with the new one
      *it = pNewAssignment;
      return;
    }
  }
  // No inconsistent Assignment, so just add normally
  add(pNewAssignment);
}


// Returns true if every Assignment of the sub-Candidate is contained in myself

bool Candidate::superset_of(Candidate *const pSubCandidate) const {
  const Candidate * const pSuperCandidate = this;
  // Assume the Assignment values are identified uniquely by name
  for (Array<const Assignment*, false>::iterator it =
	 pSubCandidate->assignments.begin();
       it != pSubCandidate->assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    if (!pSuperCandidate->contains(pAssignment)) {
      return false;
    }
  }
  return true;
}


// Returns true if the argument Conflict is not resolved by any of the
// candidate Assignments (explicit and implicit)

bool Candidate::conflict_not_resolved(const Conflict *pConflict) const {
  // iterate over Conflict Assignments, assuming nondefault-only representation
  for (Slist<Assignment * const>::const_iterator it = pConflict->begin();
       it != pConflict->end(); ++it) {
    // If conflicting assignment is not best weight value (is failure)
    // and it's not in Candidate, then it's resolved
    Assignment* const pAssignment = *it;
    Assignment* pBestWeightAssignment = 
      *(pAssignment->get_variable()->assignments_begin());
    if (contains(pAssignment)) {
      // If Assignment is best weight value and it's represented in Candidate,
      // it's resolved.
      if (pAssignment == pBestWeightAssignment) { return false; }
    } else {
      if (pAssignment != pBestWeightAssignment) { return false; }
    }
  }
  return true;
}


// Returns true if no Assignment has same Variable as testAssignment

bool Candidate::consistent_assignment(const Assignment& testAssignment) const {
  const Assignable *pTestAssignable = testAssignment.get_variable();
  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    if (pAssignment->get_variable() == pTestAssignable) {
      return false;
    }
  }
  return true;
}


bool Candidate::contains(const Assignment *pTestAssignment) const {
  for (Array<const Assignment*, false>::iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    const Assignment* pAssignment = *it;
    if (pAssignment == pTestAssignment) { // shudder.....
      return true;
    }
  }
  return false;
}


// Return the weight as a function of the set of Assignments

Assignment::weight_t Candidate::get_weight(const Array<const Assignment*,
					   false>& assignmentArray) const {
  // Take a simple sum. This is subject to change.
  return sum_assignment_weights(assignmentArray);
}


// Return the sum of the weights over all Assignments

Assignment::weight_t
Candidate::sum_assignment_weights(const Array<const Assignment*, false>&
				  assignmentArray) const {
  Assignment::weight_t result = Assignment::weight_t(0);
  for (Array<const Assignment*, false>::iterator it = assignmentArray.begin();
       it != assignmentArray.end(); ++it) {
    const Assignment* pAssignment = *it;
    result += pAssignment->get_weight();
  }
  return result;
}

