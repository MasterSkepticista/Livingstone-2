/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Incremental Truth Maintenance System, NASA Ames Research Center
// $Id: clause.cpp,v 1.52 2004/01/28 22:02:37 lbrown Exp $

#include <tms/ptheory.h>

// Initializes a new Clause with a set of positive and negative literals
// (Propositions). The Clause starts out deleted, so many of the other fields
// in the Clause are not initialized.  Duplicate literals in the Clause are
// checked for and removed.

Clause::Clause(void *datum,
	       Proposition::ListOfProposition& posPropositions,
	       Proposition::ListOfProposition& negPropositions,
	       bool isVolatile /*=false*/) :
  num_unassigned(0), supports(NO_PROPOSITION), 
  flags(DELETED), // sets all other flags false
  fringe_index(0), time_stamp(0), satisfying_prop(NULL), clause_datum(datum)
{
  if (isVolatile) {
    flags |= VOLATILE;
  }

  // First remove duplicate literals from the clause.  Duplicate literals
  // cause problems with the unassigned literal count.  The essential point
  // is that when a duplicate literal is assigned a truth value, the
  // unassigned literal count changes by 2 (or more), rather than by 1.
  
  // By default, duplicates are allowed; however, remove_duplicates
  // is O(n^2).  We can save time by disallowing duplicates; define
  // CLAUSE_ASSUME_NO_DUPLICATE_PROPS.
  //
  // To assert that the assumption is valid, define
  // CLAUSE_CHECK_NO_DUPLICATE_PROPS.  This is just as expensive as allowing
  // duplicate props, but it allows use to be sure the assumption is valid
  // in later runs.
#ifdef CLAUSE_DISALLOW_DUPLICATE_PROPS
#  ifdef CLAUSE_CHECK_NO_DUPLICATE_PROPS
  L2_assert(positive.remove_duplicates() == 0,
	    L2_error("positive list had duplicates"));
  L2_assert(negative.remove_duplicates() == 0,
	    L2_error("negative list had duplicates"));
#  endif
#else
  posPropositions.remove_duplicates();
  negPropositions.remove_duplicates();
#endif

  // Does either positive.size() or negative.size() ever exceed 255?
  num_pos_lits = static_cast<unsigned char>(posPropositions.size());
  num_lits = num_pos_lits + static_cast<unsigned char>(negPropositions.size());

  // Next construct the array holding the propositions, fill them up, and
  // add this clause to the corresponding propositions.

  literals = L2_alloc_array_no_ctor(Proposition*, num_lits);

  int i = 0;
  // Insert each positive Proposition into literals and associate with Clause
  {
    for (Proposition::ListOfProposition::iterator it = posPropositions.begin();
	 it != posPropositions.end(); ++it) {
      Proposition *pProposition = *it;
      literals[i++] = pProposition;
      pProposition->add_positive(this);
    }
  }

  // Insert each negative proposition into literals and associate with Clause
  {
    for (Proposition::ListOfProposition::iterator it = negPropositions.begin();
	 it != negPropositions.end(); ++it) {
      Proposition *pProposition = *it;
      literals[i++] = pProposition;
      pProposition->add_negative(this);
    }
  }

  // num_unassigned, satisfying_prop, and supports are not initialized at this
  // time. Rather, they are initialized when this Clause is added to the theory.
}


// Deallocate the array of Propositions and reset the member variables

Clause::~Clause() {
  L2_free_array_no_dtor(literals, num_lits);

  // for debugging, mark this clause as deleted
  num_lits = num_pos_lits = num_unassigned = -1;
  flags = 0;
  time_stamp = 0x0505u;
  satisfying_prop = (Proposition*)0x50505;
  clause_datum = (void*)0x50505;
  literals = (Proposition**)0x50505;
  supports = 0; // so no one can follow it
}


void Clause::initialize() {
  // For the Ptheory
  set_unassigned_count(number_of_literals());
  set_satisfying_prop(NULL);
  set_time_stamp(0);
  // For the LTMS
  set_supports(NULL);
  take_from_fringe();
  set_unsatisfied(false);
}


// Having supported Proposition represented as an index into the array of
// Propositions is sort of a nuisance. This does ensure you cannot support a
// Proposition that's not in the Clause.

void Clause::set_supports(Proposition* pTargetProposition) {
  if (pTargetProposition == 0) {
    supports = NO_PROPOSITION;
  } else {
    length_t index = 0;
    for (Clause::iterator it = begin(); it != end(); ++it, ++index) {
      Proposition *pProposition = *it;
      if (pProposition == pTargetProposition) {
	supports = index; // it - begin();
	return;
      }
    }
    // If a Proposition isn't in the Clause and we are claiming the Clause
    // supports it, something is seriously wrong.
    L2_throw(L2_fatal_error,
	     ("clause being supported by a prop not in the clause"));
  }
}


Proposition* Clause::get_supports() const {
  if (supports == Clause::NO_PROPOSITION)
    return NULL;
  else {
    // Literals is the start of the literals.  Supports is an index into the
    // literals.
    assert(0 <= supports && supports < num_lits);
    return literals[supports];
  }
}


// Return a Proposition whose value is UNKNOWN; set is_positive as a side effect

Proposition*
Clause::get_unassigned_literal(bool& is_positive) const {
  for (int i = 0; i < num_lits; i++) {
    Proposition *pProposition = literals[i];
    if (pProposition->isUnknown()) {
      is_positive = (i < num_pos_lits);
      return pProposition;
    }
  }
  // Should never come here since this function should always to be called
  // with a bonafide fringe clause.
  L2_throw(L2_fatal_error,
	   ("called get_unassigned_literal on non-fringe clause"));
  return 0; // Added to suppress compiler warning.
}


const Proposition* Clause::find_satisfying_prop() const {
  for (int i = 0; i < num_lits; i++) {
    Proposition *pProposition = literals[i];
    if ((i <  num_pos_lits && pProposition->isTrue()) ||
	(i >= num_pos_lits && pProposition->isFalse())) {
	return pProposition;
    }
  }
  return NULL;
}


// Increment num_unassigned with a bounds check

Clause::length_t
Clause::incr_num_unassigned() {
  L2_assert(num_unassigned < num_lits,
	    L2_fatal_error,
	    ("number of unassigned literals exceeds total number ("+
	     MBA_string(num_unassigned + 1) + ">=" +
	     MBA_string(num_lits)+")"));
  return ++num_unassigned;
}


// Decrement num_unassigned with a bounds check

Clause::length_t
Clause::decr_num_unassigned() {
  L2_assert(num_unassigned > 0,
	    L2_fatal_error,
	    ("decreasing unassigned count below zero"));
  return --num_unassigned;
}

  
bool Clause::contains(const Proposition& proposition) const {
  for (iterator it = begin(); it != end(); ++it) {
    Proposition *pProposition = *it;
    if (pProposition == &proposition) { return true; }
  }
  return false;
}


Clause::length_t Clause::count_unassigned() const {
  length_t count = 0;
  for (length_t i = 0; i < num_lits; i++) {
    if (literals[i]->isUnknown()) { count++; }
  }
  return count;
}
