/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: variable.cpp,v 1.52 2002/10/24 16:40:23 lbrown Exp $ */

#include <conflict_db/variable.h>
#include <conflict_db/conflict_db.h>

Variable::Variable(Conflict_db& conflict_db,
		   unsigned domainCardinality,
		   Theory::time_step_t time,
		   unsigned variableID) : 
  id(variableID), time_step(time), previous(NULL), next(NULL) {
  LTMS* pLTMS = conflict_db.get_solver();

  Proposition::ListOfProposition all_props;
  // This will always be empty, so allocate it statically
  static Proposition::ListOfProposition emptyPropositionList;
  Proposition::ListOfProposition negPropositions;

  // For each member i in domain, create the proposition v=i. This asserts that
  // the value is some member of the Variable's domain.
  {
    for (unsigned i = 0; i < domainCardinality; ++i) {
      Proposition* pProposition = pLTMS->create_proposition();
      all_props.push_front(pProposition);
      propositions.grow_and_set(domainCardinality - i - 1, pProposition);
    }
  }

  /// Create the clauses that ensure values are mutex and at least one holds.

  // Create Clause that says Variable must have at least one value.  It's a
  // disjunction of all values in the positive sign. The Clause is not
  // volatile and is in the theory.
  pLTMS->create_clause(NULL, all_props, emptyPropositionList, false, true);

  // For each pair of value Propositions, create a Clause that ensures at
  // least one is false.
  {
    for (unsigned i = 0; i < propositions.size(); i++) {
      negPropositions.push_front(propositions[i]); // push v=i onto negatives
      for (unsigned j = i + 1; j < propositions.size(); j++) {
	negPropositions.push_front(propositions[j]); // push v=j onto negatives
	pLTMS->create_clause(NULL, emptyPropositionList, negPropositions,
			     false, true);
	negPropositions.pop_front(); // pop v=j from negatives
      }
      negPropositions.pop_front(); // pop v=i from negatives
    }
  }

  // All done; the Conflict_db can do with us as it wishes now
  conflict_db.insert_variable(this);
}


Variable::~Variable() {
  // Clear propositions and remove the effects of this variable on the TMS
  // This is leaveTMS in the algorithm.
  for (unsigned i = 0; i < propositions.size(); i++) {
    propositions[i]->set_destroying();
  }
  if (previous) previous->next = next;
  if (next) next->previous = previous;

#ifndef NDEBUG
  // for debugging, crash if we follow the previous/next pointers
  previous = next = (Variable*)-1;
#endif
}


/***************************************************************************
        Maintain the doubly-linked list of variables.
 ***************************************************************************/

// This is called in two circumstances:
//  a) this is an old Variable; var is new and being added before this
//  b) this is a new Variable, being appended to the list after var
//
// In either case, we want:
//          prev->next = var
//          var->previous = prev
//          var->next = this
//          this->previous = var
//
// Where 'prev' is either (a) this->previous; or (b) var->previous
// One thing to notice: in case (a), var->previous is always null; in case
// (b), this->previous is always null.  It's also possible for prev
// to be null, in either case, if the final list will have just two
// variables in it (var, then this).

void Variable::insert_previous(Variable* var) {
  L2_assert(var,
	    L2_fatal_error,
	    ("trying to insert a null variable"));

  // Written naively because it's confusing enough as-is, yet presumably easy
  // for the optimizer to deal with.

  Variable *prev = NULL;
  if (this->previous) prev = this->previous;
  if (var->previous)  prev = var->previous;

  if (prev) prev->next = var;
  var->previous = prev;
  var->next = this;
  this->previous = var;
}


/***************************************************************************
        Queries about propositions.
 ***************************************************************************/

// Return the Proposition at the specified index

Proposition *Variable::get_prop(unsigned index) const {
  L2_assert(index < get_nvalues(),
	    L2_bounds_error,
            ("variable propositions", index, get_nvalues()));
  return propositions[index];
}


// Return the first true Proposition, or NULL if none is true

Proposition* Variable::get_value() {
  for (unsigned i = 0; i < propositions.size(); i++) {
    if (propositions[i]->isTrue()) { return propositions[i]; }
  }
  return NULL;
}


// Return the index of the first true Proposition, or NOT_FOUND if none is true

int Variable::get_value_index() const {
  for (unsigned i = 0; i < propositions.size(); i++) {
    if (propositions[i]->isTrue()) { return i; }
  }
  return NOT_FOUND;
}
