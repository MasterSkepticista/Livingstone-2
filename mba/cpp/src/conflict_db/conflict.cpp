/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// A conflict between assignments
// $Id: conflict.cpp,v 1.24 2004/01/28 22:02:26 lbrown Exp $

#include <conflict_db/conflict.h>

// Copy the Assignments and give each one a back pointer to this Conflict

Conflict::Conflict(const Assignment::ListOfAssignment& new_assignments,
		   unsigned tag) :
  assignments(new_assignments), tag(tag) {
  // Give each Assignment a back pointer to this Conflict
  for (Assignment::ListOfAssignment::iterator it = assignments.begin();
      it != assignments.end(); ++it) {
    Assignment *pAssignment = *it;
    pAssignment->add_conflict(*this);
  }
}


// Fills in a list of all possible Assignments which may resolve this Conflict

void Conflict::resolving_assignments(Assignment::ListOfAssignment& new_items)
  const {
  for (Conflict::const_iterator it = begin(); it != end(); ++it) {
    Assignment* const pAssignment = *it;
    pAssignment->append_other_values(new_items);
  }
}


bool
Conflict::subsumes(const Assignment::ListOfAssignment& assignments) const {
  for (Conflict::const_iterator it = begin(); it != end(); ++it) {
    Assignment* const pAssignment = *it;
    if (!assignments.contains(pAssignment)) {
      // Conflict's Assignment not in the set; the Conflict is not a subset
      return false;
    }
  }
  // All Conflict's Assignments in the set; the Conflict is a subset
  return true;
}


bool
Conflict::subsumedBy(const Assignment::ListOfAssignment& assignments) const {
  for (Assignment::ListOfAssignment::const_iterator it = assignments.begin();
       it != assignments.end(); ++it) {
    Assignment* const pAssignment = *it;
    if (!this->assignments.contains(pAssignment)) {
      // Set's Assignment not in the Conflict; the Conflict is not a superset
      return false;
    }
  }
  // All set's Assignments in the Conflict; the Conflict is a superset
  return true;
}
