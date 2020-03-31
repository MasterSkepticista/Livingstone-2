/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Implementation of an LTMS
// $Id: ltms.cpp,v 1.83 2004/01/28 23:31:54 lbrown Exp $

#include <tms/ltms.h>
// For testing timing
#include <time.h>

/***************************************************************************
  Implementing the fringe.
  We have two fringes, which is why we ferret it off to an inner class to
  do this work.
 ***************************************************************************/

/**
 * The fringe.
 * We used to use a list, but removing an arbitrary element from the
 * list was prohibitively expensive.  The problem is that STL iterators
 * become undefined when anything happens.  We need the behaviour to be
 * defined.
 * This defines an almost-stack.  Removing an arbitrary element is defined to
 * pop the top element, and replace the arbitrary element with the former top
 * one; thus the order is not necessarily LIFO.
 *
 * The macro INITIAL_FRINGE_SIZE is 10 by default; but can be set elsewhere
 * (i.e. as a -D flag) to change it.  It must be greater than zero.
 *
 * The macro DISABLE_DYNAMIC_MEMORY is not currently handled.
 */

#ifndef INITIAL_FRINGE_SIZE
#  define INITIAL_FRINGE_SIZE 16
#endif

LTMS::Fringe::Fringe() : super(INITIAL_FRINGE_SIZE) { }

// Remove an arbitrary fringe Clause. The final element is a special case.
// Called by LTMS::Fringe::remove_destroying()

void LTMS::Fringe::remove(Clause *pClause) {
  unsigned fringeIndex = pClause->get_fringe_index();
  if (fringeIndex == size() - 1) {
    pop();
  } else {
    super::remove(fringeIndex);
    pClause->take_from_fringe();
    // set the replacement's index
    operator[] (fringeIndex)->set_in_fringe(fringeIndex);
  }
}


// Remove all Clauses in the Fringe such that Clause::is_destroying() is true
// Called by LTMS::finish_destroying()

void LTMS::Fringe::remove_destroying() {
  for (unsigned i = 0; i < size(); /* iterate only if we don't remove */) {
    Clause* pClause = operator[](i);
    if (pClause->is_destroying()) {
      // Remove the Clause, not the index -- otherwise indices don't map
      // correctly anymore
      remove(pClause);
    } else {
      ++i;
    }
  }
}


/***************************************************************************
  Operations on the fringe.
 ***************************************************************************/

// If the Clause is not already in the Fringe, put it in
// Called by LTMS::add(), Ptheory::initialize(), LTMS::assign(),
// LTMS::unassign()

void LTMS::insert_into_fringe(Clause& clause) {
  if (!clause.is_in_fringe()) {
    if (clause.is_volatile()) { volatile_fringe.push(&clause); }
    else                      { fringe.push(&clause); }
  }
}


// If the Clause is in the Fringe, take it out
// Called by LTMS::remove(), LTMS::assign()

void LTMS::remove_from_fringe(Clause& clause) {
  if (clause.is_in_fringe()) {
    if (clause.is_volatile()) { volatile_fringe.remove(&clause); }
    else                      { fringe.remove(&clause); }
  }
}


// Get any Clause from the Fringe, trying non-volatile first
// Called ty LTMS::partial_propagate(), LTMS::propagate()

Clause* LTMS::get_fringe_clause() {
  if      (!fringe.empty())          { return fringe.pop(); }
  else if (!volatile_fringe.empty()) { return volatile_fringe.pop(); }
  else                               { return 0; }
}


/***************************************************************************
  Operations on the list of unsatisfied clauses.
 ***************************************************************************/

// If clause is marked satisfied, mark it and put it into the unsatisfied set
// Called by LTMS::add(), LTMS::assign()

void
LTMS::insert_into_unsat(Clause& clause) {
  if (!clause.is_unsatisfied()) {
    unsatisfied.push(&clause);
    clause.set_unsatisfied(true);
  }
}


// If clause is marked unsatisfied, unmark it and take it out of the
// unsatisfied set
// Called by LTMS::remove(), LTMS::unassign()

void
LTMS::remove_from_unsat(Clause& clause) {
  if (clause.is_unsatisfied()) {
    for (UnsatType::iterator it = unsatisfied.begin();
	it != unsatisfied.end(); ++it) {
      Clause *pClause = *it;
      if (pClause == &clause) {
	unsatisfied.erase(it);
	clause.set_unsatisfied(false);
	return;
      }
    }
    L2_throw(L2_not_found_error, ("clause not found in unsat list"));
  }
}


/***************************************************************************
  Add/remove a clause.
 ***************************************************************************/

// Undelete a deleted Clause
// Called by client

void
LTMS::add(Clause& clause) {
  // Only a currently deleted Clause can be added.  If it isn't already
  // deleted, it is already in the theory, and nothing need be done.
  if (clause.is_deleted()) {
    // Put the Clause back into the theory. This sets Clause::is_deleted(),
    // Clause::satisfying_prop, and Clause::num_unassigned
    Ptheory::add(clause);
    // It supports nothing yet
    clause.set_supports(NULL);
    // Not yet known to be unsatisfied
    clause.set_unsatisfied(false);
    // It should not be in the fringe at this point
    L2_assert(!clause.is_in_fringe(),
	      L2_fatal_error,
	      ("found deleted clause that was already in the fringe"));
    // Put it in the fringe or unsatisfied set if applicable
    if (clause.get_satisfying_prop() == NULL) {
      // If it is a unit Clause, add it to the fringe
      if (clause.get_unassigned_count() == 1) {
	insert_into_fringe(clause);
	// If it is completely assigned but there's no satisfying Proposition,
	// then it's unsatisfied.
      } else if (clause.get_unassigned_count() == 0) {
	insert_into_unsat(clause);
      }
    }
  }
}


// Delete an undeleted Clause

void
LTMS::remove(Clause& clause) {
  if (!clause.is_deleted()) {
    Ptheory::remove(clause);

    // Make sure the Clause isn't left behind in either the fringe or the set
    // of unsatisfied Clauses

    if      (clause.is_in_fringe())   { remove_from_fringe(clause); }
    else if (clause.is_unsatisfied()) { remove_from_unsat(clause); }
    else { // It is satisfied
      Proposition* pSupportedProposition = clause.get_supports();
      if (pSupportedProposition != NULL) {
	// undo any propagations from the Clause
	enqueue_for_undo(pSupportedProposition);
	// It no longer supports anything
	clause.set_supports(NULL);
	undo_propagations();
      }
    }
  }
}

/***************************************************************************
  Permanently remove marked clauses and propositions.
 ***************************************************************************/

// Helper to remove Clauses from the unsatisfied list. Filters the parameter.
// Called by LTMS::finish_destroying()

static void remove_marked_from_array(Array<Clause*, false>& list) {
  for (Array<Clause*, false>::iterator it = list.begin(); it != list.end(); ) {
    Clause *pClause = *it;
    if (pClause->is_destroying()) { list.erase(it); }
    else                          { ++it; }
  }
}


// After Clauses are marked for destruction, deallocate them
// Called by client

void
LTMS::finish_destroying() {
  // remove Clauses marked as destroying from the fringe and unsatisfied list
  fringe.remove_destroying();
  volatile_fringe.remove_destroying();
  remove_marked_from_array(unsatisfied);

  // foreach marked clause
  //  if it supports something that isn't marked
  //    remove our support

  // This is done in two steps because we need to call
  // Ptheory::finish_destroying to remove the effect of the Clauses we're
  // destroying, but since it deallocates the Clauses, we need to figure out
  // what to remove the support from first.

  // First step of `foreach marked clause': collect Propositions to unassign
        
  for (clause_list::iterator it = begin_clauses(); it != end_clauses(); ++it) {
    Clause *pClause = *it;
    if (pClause->is_destroying()) {
      Proposition *pSupportedProposition = pClause->get_supports();
      if (pSupportedProposition != NULL &&
	  !pSupportedProposition->is_destroying()) {
	enqueue_for_undo(pSupportedProposition);
      }
    }
  }

  // Remove Clauses and Propositions from the theory
  Ptheory::finish_destroying();

  // Second step of `foreach marked clause': unassign Propositions on the list
  undo_propagations();

  // All that probably put stuff on the fringe; fix it
  propagate();
}


// Propagate until done or iterations reached, whichever comes first
// Called by client

bool
LTMS::partial_propagate(size_t iterations, bool &consistent) {
  for (size_t iteration = 0; unsatisfied.empty() && iteration < iterations;
       iteration++) {
    Clause *pFringeClause = get_fringe_clause();
    if (pFringeClause != NULL) {
      // These are here rather than at the top to make it clearer that the out
      // loop is stateless (the state is held in the LTMS) and we can split it
      bool is_positive;
      Proposition *pProposition =
	pFringeClause->get_unassigned_literal(is_positive);
      assign(*pProposition, is_positive, *pFringeClause);
    } else {
      // We have propagated through all the fringe clauses and satisfied them.
      consistent = true;
      return true;
    }
  }
  if (!unsatisfied.empty()) { // We have encountered an unsatisfied clause
    consistent = false;
    return true;
  } else { // unsatisfied.empty() && (iteration >= iterations)
    // The user should not be paying any attention to consistent
    // since finished (the return value) is false.  We won't set it here.
    return false;
  }
}


// Propagate until done
// Called by client

bool
LTMS::propagate() {
  while (unsatisfied.empty()) {
    Clause *pFringeClause = get_fringe_clause();
    if (pFringeClause != NULL) {
      bool is_positive;
      Proposition *pProposition =
	pFringeClause->get_unassigned_literal(is_positive);
      assign(*pProposition, is_positive, *pFringeClause);
      // Move this into assign.
      pFringeClause->set_supports(pProposition);
    } else {
      // We have propagated through all of the fringe Clauses & satisfied them
      return true;
    }
  }
  // We have encountered an unsatisfied Clause
  return false;
}


// Return all ancestor Clauses satisfying the predicate.
// Auxiliary to LTMS::last_conflict()

void
LTMS::get_clause_support(Clause& clause,
			 Clause::ListOfClause& result,
			 bool (*predicate)(const Clause&))
{
  // Start at 16 since we often will need at least that many
  Array<Clause*, false> stack(16);
  // In case time_stamp is incremented until overflow, start from the beginning
  if (time_stamp == Clause::MAX_TIME_STAMP) {
    initialize_time_stamps();
  }
  // Starting a new cohort. No Clause should now have this time stamp.
  ++time_stamp;
  // Initialize the stack with the input Clause
  stack.push(&clause);
  do {
    // The next Clause to examine
    Clause *pCurrentClause = stack.pop();
    // For each Proposition in each Clause, collect its support
    for (Clause::iterator it = pCurrentClause->begin();
	 it != pCurrentClause->end(); ++it) {
      Proposition* pProposition = *it;
      Clause* pParentClause = pProposition->get_supported_by();
#ifdef LTMS_DEBUG
      if (pParentClause == NULL) {
	_STD_ cerr << "PARENTLESS PROP IS " << *pProposition   << _STD_ endl
		  << "pCurrentClause is "  << *pCurrentClause << _STD_ endl
		  << "clause is "          << clause          << _STD_ endl
		  << _STD_ flush;
      }
#endif
      L2_assert(pParentClause,
		L2_fatal_error,
		("unsupported clause was supporting something"));

      // Don't add the Proposition's supporting Clause to the stack of Clauses
      // to be processed if it is the Clause we are already processing or if
      // the supporting Clause has already been visited because it supports
      // another Proposition.

      if ((pParentClause != pCurrentClause) &&
	  (pParentClause->get_time_stamp() != time_stamp)) {
	// Mark it as having been visited in this cohort
	pParentClause->set_time_stamp(time_stamp);

	// Note that we keep tracing back until we hit a Clause for which the
	// predicate holds.  We do not add the support for the Clause that
	// satisfied the predicate, nor do any Clauses that don't satisfy the
	// predicate appear in the result.

#ifdef LTMS_DEBUG
 	_STD_ cerr << "Support clause is " << (*pParentClause) << _STD_ endl;
#endif
	if (predicate(*pParentClause)) {
	  // It passes the filter; return it
	  result.push_front(pParentClause);
	} else {
	  // It doesn't pass the filter; keep looking
	  stack.push(pParentClause);
	}
      } // end if
    } // end for
  } while (!stack.empty());
}


void LTMS::initialize() {
  Ptheory::initialize();
  // Clear containers
  fringe.erase();
  volatile_fringe.erase();
  unsatisfied.erase();
  undo_list.erase();
  // Put all unit Clauses into the fringe
  for (clause_list::iterator it = begin_clauses();
       it != end_clauses(); it++) {
    Clause* pClause = *it;
    if (pClause->get_unassigned_count() == 1) {
      insert_into_fringe(*pClause);
    }
  }
}


void LTMS::assignUpdateSameSense(const Proposition& proposition,
				 bool becomingTrue) {
  Proposition::iterator it     =
    becomingTrue ? proposition.begin_positive() : proposition.begin_negative();
  Proposition::iterator it_end =
    becomingTrue ? proposition.end_positive() : proposition.end_negative();
  for (; it != it_end; ++it) {
    Clause *pClause = *it;
    if (!pClause->is_deleted()) {
      // One of its Propositions is being assigned
      pClause->decr_num_unassigned();
      // If it wasn't already satisfied, now it is
      if (pClause->get_satisfying_prop() == NULL) {
	pClause->set_satisfying_prop(&proposition);
	// Now that it is satisfied, it can't be in the fringe
	if (pClause->is_in_fringe()) {
	  remove_from_fringe(*pClause);
	}
      }
    }
  }
}


void LTMS::assignUpdateOppositeSense(const Proposition& proposition,
				     bool becomingTrue) {
  Proposition::iterator it     =
    becomingTrue ? proposition.begin_negative() : proposition.begin_positive();
  Proposition::iterator it_end =
    becomingTrue ? proposition.end_negative() : proposition.end_positive();

  for (; it != it_end; ++it) {
    Clause *pClause = *it;
    if (!pClause->is_deleted()) {
      // One fewer unassigned Proposition
      pClause->decr_num_unassigned();
      if (pClause->get_satisfying_prop() == NULL) {
	// The Clause was not satisfied
	if (pClause->is_in_fringe()) {
	  // The number of unassigned literals was 1 but now must be 0
	  remove_from_fringe(*pClause);
	  // All its literals are assigned but it is not satisfied
	  insert_into_unsat(*pClause);
	} else if (pClause->get_unassigned_count() == 1) {
	  // It has one unassigned literal and it is not satisfied: fringe
	  insert_into_fringe(*pClause);
	}
      }
    }
  }
}


// Now that Proposition has a truth assignment, we need to update all of the
// Clauses in which it appears.  If the Clause contains the Proposition with
// the same truth value as becomingTrue, then Proposition is the satisfying
// Proposition for the Clause, if it doesn't already have one.
//
// If the Clause contains the Proposition with the opposite truth value, then
// the Proposition does not satisfy the Clause.  In addition, if the Clause
// was in the fringe (that is, wasn't satisfied and had only one unassigned
// Proposition left, the one we just assigned to the wrong sign) it is now
// screwed. The Clause will be added to the unsatisfied Propositions of the
// theory.
// If becomingTrue is true, then the Positive Clauses are satisfied.
// Otherwise the negative Clauses are satisfied.
// Called by LTMS::move_support()

void
LTMS::assign(Proposition& proposition, bool becomingTrue, Clause& clause) {
  proposition.set_value((becomingTrue ?
			 Proposition::PTRUE :
			 Proposition::PFALSE));
  proposition.set_supported_by(&clause);
  assignUpdateSameSense(proposition, becomingTrue);
  assignUpdateOppositeSense(proposition, becomingTrue);
}


/***************************************************************************
  Unassigning propositions.
 ***************************************************************************/

// Called by LTMS::remove(), LTMS::finish_destroying(), LTMS::unassign()

void
LTMS::enqueue_for_undo(Proposition *pProposition) {
  undo_list.push(pProposition);
}


// Unassign each Proposition whose Clause had been removed
// Called by LTMS::remove(), LTMS::finish_destroying(), LTMS::move_support()

void
LTMS::undo_propagations() {
  while (!undo_list.empty()) {
    Proposition *pProposition = undo_list.pop();
    unassign(*pProposition); // this may add more to undo_list
  }
}


void LTMS::unassignUpdateSameSense(const Proposition& proposition,
				   bool wasTrue) {
  Proposition::iterator it     = 
    wasTrue ? proposition.begin_positive() : proposition.begin_negative();
  Proposition::iterator it_end = 
    wasTrue ? proposition.end_positive() : proposition.end_negative();    
  for (; it != it_end; ++it) {
    Clause *pClause = *it;
    if (!pClause->is_deleted()) {
      // One more literal is unassigned
      pClause->incr_num_unassigned();
      if (pClause->get_satisfying_prop() == &proposition) {
	// The Proposition being unassigned used to satisfy it
	const Proposition* pSatisfyingProposition =
	  pClause->find_satisfying_prop();
	// Update the satisfying Proposition now that we know it
	pClause->set_satisfying_prop(pSatisfyingProposition);
	if (pSatisfyingProposition == NULL) {
	  // There is no other literal that satisfies the Clause
	  if (pClause->get_unassigned_count() == 1) {
	    // One unassigned literal and unsatisfied: fringe
	    insert_into_fringe(*pClause);
	  }
	}
      }
    }
  }
}


void LTMS::unassignUpdateOppositeSense(const Proposition& proposition,
				       bool wasTrue) {
  Proposition::iterator it     =
    wasTrue ? proposition.begin_negative() : proposition.begin_positive();
  Proposition::iterator it_end =
    wasTrue ? proposition.end_negative() : proposition.end_positive();
  for (; it != it_end; ++it) {
    Clause *pClause = *it;
    if (!pClause->is_deleted()) {
      pClause->incr_num_unassigned();
      if (pClause->is_unsatisfied()) {
	// A hitherto unsatisfied Clause is no longer known to be unsatisfied
	remove_from_unsat(*pClause);
	insert_into_fringe(*pClause);
      } else if (pClause->is_in_fringe()) {
	// A hitherto fringe Clause is no longer fringe
	remove_from_fringe(*pClause);
      } else {
	// A hitherto supported Clause is no longer supported
	Proposition* pSupportedProposition = pClause->get_supports();
	if (pSupportedProposition != NULL) {
	  enqueue_for_undo(pSupportedProposition);
	  pClause->set_supports(NULL);
	}
      }
    }
  }
}


// Unassigning a Proposition's truth value involves the following:
//  o resetting the Proposition's truth value to UNKNOWN
//  o updating the support and unassigned literal counts of all
//    Clauses that it occurs in, and making sure that the update on the
//    Clauses correctly updates the fringe and the unsatisfied Clauses
//  o undoing the propagations from any Clauses that did unit propagations
//    based on the truth value of this Proposition.
// Called by LTMS::undo_propagations(), LTMS::move_support()

void
LTMS::unassign(Proposition& proposition)
{
  if (proposition.isUnknown()) {
    // Integrity test
    L2_assert(!proposition.get_supported_by(),
	      L2_fatal_error,
	      ("prop has unknown value but is supported"));
    // Nothing further to do
    return;
  } else {
    bool wasTrue = proposition.isTrue();
    proposition.initialize();
    unassignUpdateSameSense(proposition, wasTrue);
    unassignUpdateOppositeSense(proposition, wasTrue);
  }
}


#ifdef LTMS_DEBUG
/**
 * Send to _STD_ cerr the address of the chosen unsatisfied Clause and the
 * pointers in the set of unsatisfied Clauses. The unsatisfield Clause should
 * be the first element of the set.
 * Debugging only
 * Auxiliary to LTMS::last_conflict()
 * \param pUnsatisfiedClause pointer the the first element of unsat
 * \param unsatisfiedClauseSet
 */

static void
last_conflict_debug1(Clause *pUnsatisfiedClause,
		     Array<Clause *, false>& unsatisfiedClauseSet) {
  _STD_ cout << "Unsatisfied clause is " << pUnsatisfiedClause << _STD_ endl;
  _STD_ cout << _STD_ endl << "     When checking conflict UNSAT is: "
	    <<  _STD_ endl;
  for (Array<Clause*, false>::iterator it = unsatisfiedClauseSet.begin();
       it != unsatisfiedClauseSet.end(); ++it) {
    Clause *pClause = *it;
    _STD_ cout << "               " << pClause << _STD_ endl;
  }
  _STD_ cout << _STD_ endl;
}


/**
 * Send to _STD_ cerr the addresses of the elements of the Clause list.
 * Debugging only
 * Auxiliary to LTMS::last_conflict()
 * \param conflict a list of Clauses
 */

static void last_conflict_debug2(Clause::ListOfClause& conflict) {
  _STD_ cerr << "Conflict is " << _STD_ endl;
  for (Clause::ListOfClause::iterator it = conflict.begin();
       it != conflict.end(); ++it) {
    Clause *pClause = *it;
    _STD_ cerr << pClause << " ";
  }
  _STD_ cerr << _STD_ endl;
}
#endif


// Called by client

void
LTMS::last_conflict(Clause::ListOfClause& conflicts,
		    bool (*predicate)(const Clause&)) {
  if (unsatisfied.empty()) {
    // The theory is consistent, so there is no conflict; leave conflicts empty
    return;
  } else {
    // The theory is inconsistent, so there is a conflict.
    // If the model is well formed, all unsatisfied Clauses will be suitable.
    // We will trace the inconsistency causing the first unsatisfied Clause.
    Clause *pUnsatisfiedClause = unsatisfied[0];

#ifdef LTMS_DEBUG
    last_conflict_debug1(pUnsatisfiedClause, unsatisfied);
#endif

    // If the Clause is unsatisfied, that means that propagation attempted to
    // propagate an Assignment to a Proposition through the Clause, and the
    // Clause required a different value for that Proposition.

    // Suppose we have the Clause (A or B) and we have propagated a false truth
    // value for A. If we then attempt to propagate a false truth value to B,
    // then the Clause is unsatisfied, and its support is left as Proposition B.

    // We then want an explanation, in terms of Clauses that satisfy predicate,
    // of what is supporting the conflicting truth values.

    // If the unsatisfied Clause itself satisfies the predicate, then it is part
    // of the explanation of the conflict, since it is placing one constraint
    // on "B" (using the above example).

    if (predicate(*pUnsatisfiedClause)) {
      conflicts.push_front(pUnsatisfiedClause);
    }

    // The support for the unsatisfied Clause represents the propagation that
    // had been done to deliver the conflicting "B" value into the unsatified
    // Clause. That represents the remainder of the explaination.

    get_clause_support(*pUnsatisfiedClause, conflicts, predicate);

#ifdef LTMS_DEBUG
    last_conflict_debug2(conflicts);
#endif
  }
}


#ifdef LTMS_DEBUG
// Debugging only
// Called by LTMS::move_support()

static void move_support_debug1(Proposition* pOldProposition,
				Clause::ListOfClause& trans_clauses,
				Proposition* pNewProposition) {
  _STD_ cerr << "Moving support from "
       << *pOldProposition
       << " to " << *pNewProposition
       << _STD_ endl;
  _STD_ cerr << "Clauses are " << _STD_ endl;
  for (Clause::ListOfClause::iterator it = trans_clauses.begin();
       it != trans_clauses.end(); ++it) {
    Clause* pClause = *it;
    _STD_ cerr << "      " << pClause << _STD_ endl;
  }
}


// Debugging only
// Called by LTMS::move_support()

static void move_support_debug2(Proposition* pOldProposition,
				Clause::ListOfClause& trans_clauses,
				Proposition* pNewProposition) {
  _STD_ cerr << "Moved support from "
	    << *pOldProposition
	    << " to "
	    << *pNewProposition
	    << _STD_ endl;
  _STD_ cerr << "Clauses are now" << _STD_ endl;
  for (Clause::ListOfClause::iterator it = trans_clauses.begin();
      it != trans_clauses.end(); ++it) {
    Clause* pClause = *it;
    _STD_ cerr << "      " << pClause << _STD_ endl;
  }
  _STD_ cerr << _STD_ endl;
}
#endif


/**
 * \TODO Since pOldProposition should appear in the Clause exactly once, it
 * should be possible to stop iterating as soon as it is found. In fact, there
 * can be only one loop, over all literals. When the literals match, it should
 * be possible to determine from the index whether it is a positive or negative
 * literal.
 */

void LTMS::transfer_clauses(Proposition* pOldProposition,
			    Proposition* pNewProposition,
			    Clause::ListOfClause& transferredClauses,
			    bool losing_assignment) {
  // For each Clause in the list of Clauses
  for (Clause::ListOfClause::iterator it = transferredClauses.begin();
       it != transferredClauses.end(); ++it) {
    Clause* pClause = *it;
    // Move every occurrence of the Proposition in the Clause's positive
    // literals. Let the Propositions know that the Clauses they appear in have
    // changed.

    // Contrary to appearances, positive_props is an array of Proposition*
    Clause::iterator positive_props = pClause->begin_positive();
    // For each positive literal of the Clause being transferred
    for (int i = 0; i < pClause->number_of_positive_literals(); ++i) {
      if (positive_props[i] == pOldProposition) {
	positive_props[i] = pNewProposition;
	pNewProposition->add_positive(pClause);
	pOldProposition->remove_positive(pClause);
	if (losing_assignment) { pClause->incr_num_unassigned(); }
      }
    }

    // Move every occurrence of the Proposition in the Clause's negative
    // literals. Let the Propositions know that the Clauses they appear in have
    // changed.

    // Contrary to appearances, negative_props is an array of Proposition*
    Clause::iterator negative_props = pClause->begin_negative();
    // For each negative literal of the Clause being transferred
    for (int ii = 0; ii < pClause->number_of_negative_literals(); ++ii) {
      if (negative_props[ii] == pOldProposition) {
	negative_props[ii] = pNewProposition;
	pNewProposition->add_negative(pClause);
	pOldProposition->remove_negative(pClause);
	if (losing_assignment) { pClause->incr_num_unassigned(); }
      }
    }
    
    // If the moved Clause was satisfied by the old Propsition, now it is
    // satisfied by the new Proposition.
    if (pClause->get_satisfying_prop() == pOldProposition) {
      pClause->set_satisfying_prop(pNewProposition);
    }
  }
}


/*
 * There are 3 cases:
 *  (1) pOldProposition is not supported: simply transfer the Clauses
 *  (2) pOldProposition is supported by a Clause not in transferredClauses:
 *      transfer the Clauses but adjust the assignment count
 *  (3) pOldProposition is supported by a Clause in transferredClauses:
 *      assign newProposition, transfer the Clauses, unassign oldProposition,
 *      undo propagations
 */

void
LTMS::move_support(Proposition* pOldProposition,
		   Clause::ListOfClause& transferredClauses,
		   Proposition* pNewProposition) {
#ifdef LTMS_DEBUG
  move_support_debug1(pOldProposition, transferredClauses, pNewProposition);
#endif
  Clause *pSupportingClause = pOldProposition->get_supported_by();
  if (pSupportingClause == NULL) {
    // Just transfer the Clauses; no problem with support
    transfer_clauses(pOldProposition, pNewProposition, transferredClauses,
		     false);
  } else if (!transferredClauses.contains(pSupportingClause)) {
    // If pOldProposition is supported by some other Clause than the ones we are
    // moving, pNewProposition will not yet have support. In this case, the
    // Clauses that swap pNewProposition for pOldProposition will lose one
    // Assignment.
    // If pOldProposition is supported but we cannot move that support to
    // pNewProposition because it's not in the Clauses we are moving, we have
    // lost the Assignment. We'll have to adjust the unassigned literal counts
    // of all Clauses that swap pNewProposition for pOldProposition.
    transfer_clauses(pOldProposition, pNewProposition, transferredClauses,
		     true);
  } else {
#ifdef LTMS_DEBUG
    _STD_ cerr << "Supporting transition clause moved " << _STD_ endl;
#endif
    // If pOldProposition was constrained to a value by the transition Clauses,
    // we want pNewProposition to take on its value and support. Otherwise,
    // pNewProposition is not yet assigned. This will be important when we swap
    // pNewProposition for pOldProposition.
    // Assign pNewProposition while it is pointing only to Clauses in its time
    // step (e.g., its domain constraints).
    // For all of the Clauses across time steps, we will copy the support from
    // pOldProposition to pNewProposition. We want to make this Assignment
    // first so we do not mangle the existing support on the time step
    // Clauses.
    assign(*pNewProposition, !pOldProposition->isFalse(), *pSupportingClause);

    transfer_clauses(pOldProposition, pNewProposition, transferredClauses,
		     false);

    // We have now removed all of the time-step-related Clauses from
    // pOldProposition and put them onto pNewProposition.  If pOldProposition
    // had a value and supporting Clause, then we'll want to unassign it. We can
    // do that at the end so we do not disturb any of the propagations involving
    // the time-step Clauses.
    // As in the use of assign() above, do we want to put in the correct
    // manipulations of the TMS here so we do not end up doing extra work?
    unassign(*pOldProposition);

    // We will have to call undo_propagations() before propagating to ensure we
    // have undone all of the propagation that pOldProposition was supporting.
    // Since we are going to call move_support() a great many times, we might
    // want to call the undo_propagations() that cleans up after the unassigns
    // just once. I haven't investigated whether it saves anything.  JAK.
    undo_propagations();
  }
#ifdef LTMS_DEBUG
  move_support_debug2(pOldProposition, transferredClauses, pNewProposition);
#endif
}
