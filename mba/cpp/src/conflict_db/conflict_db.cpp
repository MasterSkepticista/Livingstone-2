/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: conflict_db.cpp,v 1.114 2004/02/10 00:50:01 lbrown Exp $ */
#include <conflict_db/conflict_db.h>

#include <livingstone/L2_assert.h>
#include <mba_utils/algos.h>

// Allow easily disabling the listeners for flight
// The do-while(0) is the only portable way to block.
#ifndef DISABLE_CDB_LISTEN
#  include <conflict_db/conflict_db_listener.h>
#  define cdb_record(call_with_args) \
   do { \
    Slist<Conflict_db_listener*>::iterator listen_it##__LINE__ \
        = listeners_.begin(); \
    while(listen_it##__LINE__ != listeners_.end()) { \
        (*listen_it##__LINE__)->call_with_args;  \
        ++listen_it##__LINE__; \
    } \
   } while(0)
#else
#  define cdb_record(call_with_args)
#endif

/***************************************************************************
  Destructor.
 ***************************************************************************/

Conflict_db::~Conflict_db() {
  cdb_record(destroying_container(*this));

  // Destroy all the Candidates
  eraseCandidates();

  // Destroy all the Variables ; must use destroy_variables() to clear out the
  // TMS correctly
  accept_all a;
  destroy_variables(&a, begin_variables(), end_variables(), true);
  
  // Destroy all the Conflicts
  for (Conflict::ListOfConflict::iterator it = conflicts.begin();
       it != conflicts.end(); ++it) {
    const Conflict* const pConflict = *it;
    // Older compilers don't know that delete is const
    delete const_cast<Conflict*>(pConflict);
  }
  conflicts.erase();
}


// Called by Variable::Variable
void Conflict_db::insert_variable(Variable* pVariable) {
  insert_before = variables.insert_before(insert_before, pVariable);
  cdb_record(created_variable(*pVariable));
}


void Conflict_db::destroy_variables(Variable_acceptor *a,
				    Variable::ListOfVariable::iterator start,
				    Variable::ListOfVariable::iterator end,
				    bool cleanup) {
  // first clear the Assignables
  {
    for (Assignable::ListOfAssignable::iterator it = begin_assignables();
	 it != end_assignables(); ) {
      Assignable *pAssignable = *it;;
      if (a->accept(*pAssignable)) { assignables.erase(it); }
      else                         { ++it; }
    }
  }

  // next, the Assumptions
  {
    for (Assumption::ListOfAssumption::iterator it = begin_assumptions();
	 it != end_assumptions(); ) {
      Assumption *pAssumption = *it;
      if (a->accept(*pAssumption)) { assumptions.erase(it); }
      else                         { ++it; }
    }
  }

  // finally, the variables
  for (Variable::ListOfVariable::iterator it = start; it != end; ) {
    Variable *pVariable = *it;
    if (a->accept(*pVariable)) {
      cdb_record(destroying_variable(*pVariable));
      
      // don't clobber the insertion point
      if (insert_before == it) {
	variables.erase(it);
	insert_before = it;
      } else {
	variables.erase(it);
      }

      // deallocate the variable; this does the right thing
      // with respect to the list of variables, the TMS, etc.
      delete pVariable;
    } else { ++it; }
  }

  // clean up the tms
  if (cleanup) { get_solver()->finish_destroying(); }
}


// Iterate over all Variables and restore each one; this is meaningful only for
// Assignables

void Conflict_db::restore() {
  for (Variable::ListOfVariable::iterator it = begin_variables();
       it != end_variables(); ++it) {
    Variable *pVariable = *it;
    // This is a no-op unless pVariable points to an Assignable
    pVariable->restore();
  }
}


// Iterator over all Variables and assign each one; this is meaningful only for
// Assignables

void Conflict_db::save() {
  for (Variable::ListOfVariable::iterator it = begin_variables();
       it != end_variables(); ++it) {
    Variable *pVariable = *it;
    // This is a no-op unless pVariable points to an Assignable
    pVariable->save();
  }
}


// This isn't a method on class Clause because a Clause doesn't know about
// Assignment; it just stores a void pointer to it as a "datum".

// When we create the Clause representing an Assignment, we install the
// Assignment. This is used to determine which Clauses will be returned when
// we are tracing a Conflict.

// This assumes the only datum of a Clause is the Assignment it represents.
// This is true in this conflict DB, but in the original Livingstone we stuck
// all kinds of junk (e.g., where the Clause came from in the model) in there.
// We might want to have a different attribute for that.

// This function should be elsewhere -- for instance, in the same file as where
// we create this situation we're checking for (namely,in assignable.cpp)

bool Conflict_db::is_assumption(const Clause& clause) {
  const void * const pClauseDatum = clause.get_datum();
  if (pClauseDatum == NULL) {
    // If the Clause doesn't have an Assignment datum, it is not a unit Clause
    return false;
  } else {
    // If the Clause has a datum, the datum must be an Assignment*
    const Assignment * const pAssignment =
      static_cast<const Assignment * const>(pClauseDatum);
    const Assignable * const pAssignable = pAssignment->get_variable();
    return pAssignable->is_assumption();
  }
}


bool Conflict_db::is_command(const Clause& clause) {
  const void* const pClauseDatum = clause.get_datum();
  if (pClauseDatum == NULL) {
    // If the Clause doesn't have an Assignment datum, it is not a unit Clause
    return false;
  } else {
    // If the Clause has a datum, the datum must be an Assignment*
    const Assignment * const pAssignment =
      static_cast<const Assignment * const>(pClauseDatum);
    const Assignable * const pAssignable = pAssignment->get_variable();
    return pAssignable->is_command();
  }
}


bool Conflict_db::is_observable(const Clause& clause) {
  const void* const pClauseDatum = clause.get_datum();
  if (pClauseDatum == NULL) {
    // If the Clause doesn't have an Assignment datum, it is not a unit Clause
    return false;
  } else {
    // If the Clause has a datum, the datum must be an Assignment*
    const Assignment * const pAssignment =
      static_cast<const Assignment * const>(pClauseDatum);
    const Assignable * const pAssignable = pAssignment->get_variable();
    return pAssignable->is_observable();
  }
}


/***************************************************************************
  Handling conflicts: finding or creating; destroying.
 ***************************************************************************/

// A parial-ordering predicate on Conflict objects based on size

static bool conflict_is_smaller(const Conflict *c1, const Conflict *c2) {
  return c1->size() < c2->size();
}


const Conflict *Conflict_db::add_conflict(const Assignment::ListOfAssignment&
					  conflictingAssignments,
					  unsigned tag) {
  const Conflict *pSubsumingConflict =
    find_subsuming_conflict(conflictingAssignments);
  if (pSubsumingConflict) {
    return pSubsumingConflict;
  } else {
    // No Conflict subsumes the current one; so create a new Conflict and
    // insert it
    const Conflict *pConflict = new Conflict(conflictingAssignments, tag);
    // Insert the new conflict into the list in increasing order of size
    insert_sorted(conflicts, pConflict, conflict_is_smaller);
    return pConflict;
  }
}


/**
 * We want to find a Conflict that has a subset of the Assignments of what's
 * passed in.  The reason is that a Conflict is "active" only if _all_
 * Assignments in it are true (if all Assignments are true, there's a conflict;
 * otherwise, there's no conflict due to those Assignments).  So if we find
 * a conflict between fewer Assignments, resolving it will resolve the larger
 * assignment.
 */

const Conflict*
Conflict_db::find_subsuming_conflict(const Assignment::ListOfAssignment&
				     conflicting_assignments) const {
  // For each Conflict
  for (Slist<const Conflict * const>::iterator it = conflicts.begin();
       it != conflicts.end(); ++it) {
    const Conflict *pConflict = *it;
    if (pConflict->subsumes(conflicting_assignments)) {
      // All of the Assignments in this Conflict are found in the passed
      // Assignments. The passed set of conflicting Assignments are implicitly
      // represented by this Conflict. Return it.
      return pConflict;
    }
    // Otherwise, start checking the next Conflict
  }
  // There is no existing Conflict that subsumes the set of Assignments
  return NULL;
}


// Delete the Conflict to which the iterator points and erase it from its
// container.

void Conflict_db::destroy_conflict(Conflict::ListOfConflict::iterator& it) {
  const Conflict* const pConflict = *it;
  for (Slist<Assignment * const>::const_iterator ait = pConflict->begin();
       ait != pConflict->end(); ++ait) {
    Assignment * const pAssignment = *ait;
    pAssignment->remove_conflict(*pConflict);
  }
  // Older compilers don't know that delete is const
  delete const_cast<Conflict*>(pConflict);
  conflicts.erase(it);
}


// If the LTMS is consistent, return NULL; else return a Conflict pointer.

const Conflict* Conflict_db::conflict(unsigned tag) {
  if (solver.propagate()) {  // it is consistent
    return NULL;
  } else {                   // there is a conflict
    // Get a set of conflicting (unsatisfied) unit Clauses for Assumptions
    Clause::ListOfClause conflictingClauses;
    solver.last_conflict(conflictingClauses, &(Conflict_db::is_assumption));
    // Map the conflicting Clauses onto their Assignments
    Assignment::ListOfAssignment conflictingAssignments;
    for (Clause::ListOfClause::iterator it = conflictingClauses.begin();
	 it != conflictingClauses.end(); ++it) {
      Clause *pClause = *it;
      // The Clause's datum is a non-NULL Assignment*
      Assignment *pAssignment = static_cast<Assignment *>(pClause->get_datum());
      assert(pAssignment != NULL);
      conflictingAssignments.push_front(pAssignment);
    }
    // Make sure there is a Conflict
    return add_conflict(conflictingAssignments, tag);
  }
}


// Iterates over all the Assumption variables and assigns default values

void Conflict_db::restore_assumption_defaults() {
  for (Assumption::ListOfAssumption::iterator it = assumptions.begin();
       it != assumptions.end(); ++it) {
    Assumption *pAssumption = *it;
    pAssumption->restore_default();
  }
}


/***************************************************************************
  Dealing with the list of Candidates
 ***************************************************************************/

/***************************************************************************
  Get the ith Candidate, in the most moronic way.
 ***************************************************************************/

Candidate* Conflict_db::getCandidate(unsigned i) {
  L2_assert(i < candidates.size(),
	    L2_bounds_error,
	    ("candidate", i, candidates.size()));
  Candidate::ListOfCandidate::iterator it = candidates.begin();
  while (i != 0) { --i; ++it; }
  return *it;
}


const Candidate *Conflict_db::getCandidate(unsigned i) const {
  L2_assert(i < size(),
	    L2_bounds_error,
	    ("candidate", i, size()));
  Candidate::ListOfCandidate::const_iterator it = candidates.begin();
  while (i != 0) { --i; ++it; }
  return *it;
}


unsigned Conflict_db::getIndex(const Candidate& candidate) const {
  unsigned i = 0;
  for (Candidate::ListOfCandidate::const_iterator it = candidates.begin();
       it != candidates.end(); ++it, i++) {
    const Candidate* const pCandidate = *it;
    if (pCandidate == &candidate) { return i; }
  }
  // It is an error for candidate not to be in the candidates list
  return static_cast<unsigned>(-1);
}


// Deallocate each Candidate pointer and then empty the container

void Conflict_db::eraseCandidates() {
  for (Candidate::ListOfCandidate::iterator it = candidates.begin();
       it != candidates.end(); ++it) {
    const Candidate* const pCandidate = *it;
    // Older compilers don't know that delete is const
    delete const_cast<Candidate*>(pCandidate);
  }
  candidates.erase();
}


// Install the given candidate and propagate if specified

void Conflict_db::install(const Candidate& candidate, bool propagate) {
  restore_assumption_defaults();
  candidate.assign();
  // We can track the installed candidate here if desired.
  if (propagate) { solver.propagate(); }
}


// Insert given candidate in decreasing order of summed weight

void Conflict_db::addCandidate(Candidate *pCandidateToAdd) {
  Assignment::weight_t weightOfCandidateToAdd = pCandidateToAdd->get_weight();
  for (Candidate::ListOfCandidate::iterator it = candidates.begin();
       it != candidates.end(); ++it ) {
    Candidate *pCandidate = *it;
    if (pCandidate->get_weight() > weightOfCandidateToAdd) {
      candidates.insert_before(it, pCandidateToAdd);
      return;
    }
  }
  candidates.push_back(pCandidateToAdd);
}


// Add an Assumption to all Candidates

void  Conflict_db::addAssumption(const Assignment *pAssignment) {
  for (Candidate::ListOfCandidate::iterator it = candidates.begin();
       it != candidates.end(); ++it ) {
    Candidate *pCandidate = *it;
    pCandidate->add_consistently(pAssignment);
  }
 }

