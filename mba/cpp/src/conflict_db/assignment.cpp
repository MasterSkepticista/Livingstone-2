/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: assignment.cpp,v 1.46 2004/01/28 22:02:25 lbrown Exp $ */
#include <conflict_db/assignment.h>
#include <conflict_db/assignable.h>
#include <tms/clause.h> // clause.get_datum

// This implementation is hokey.  See the .h file
void Assignment::assign() const { variable->assign(this); }


void Assignment::add_conflict(const Conflict& conflict) {
  if (!conflicts.contains(&conflict)) {
    conflicts.push_front(&conflict);
  }

  // If this Assignment is in the Conflict, all other Assignments to the
  // Variable resolve the Conflict.
  // Note that this does not preserve the order in
  // Assignment::resolved_conflicts
  Assignable* pAssignable = get_variable();
  for (ListOfAssignment::iterator it = pAssignable->assignments_begin();
       it != pAssignable->assignments_end(); ++it) {
    Assignment *pAssignment = *it;
    if (this != pAssignment) {
      pAssignment->resolved_conflicts.push_front(&conflict);
    }
  }
}


void Assignment::remove_conflict(const Conflict& conflict) {
  conflicts.remove(&conflict);
  Assignable* pAssignable = get_variable();
  for (ListOfAssignment::iterator it = pAssignable->assignments_begin();
       it != pAssignable->assignments_end(); ++it) {
    Assignment *pAssignment = *it;
    if (this != pAssignment) {
      pAssignment->resolved_conflicts.remove(&conflict);
    }
  }
}


// This is defined here so that assignment.h needn't #include assignable.h
bool Assignment::is_assumption() const { return variable->is_assumption(); }


// This is defined here so that assignment.h needn't #include assignable.h
Theory::time_step_t Assignment::get_time_step() const {
  return get_variable()->get_time_step();
}


void Assignment::append_other_values(ListOfAssignment& assignments) {
  // The Assignment's Assignable Variable
  Assignable* pAssignable = get_variable();
  ListOfAssignment increment;
  for (ListOfAssignment::iterator it = pAssignable->assignments_begin();
       it != pAssignable->assignments_end(); ++it) {
    Assignment *pAssignment = *it;
    if (this != pAssignment) { increment.push_front(pAssignment); }
  }
  assignments += increment;
}

