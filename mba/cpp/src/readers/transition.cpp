/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: transition.cpp,v 1.12 2004/02/11 00:54:52 lbrown Exp $ */
#include <readers/transition.h>

/**
 * The rank is ignored for a nominal Transition
 */

// If copy, allocate new memory for the array of L2rClause objects
L2rTransition::L2rTransition(const L2rVariable *mode,
			     int fromStateIndex,
			     int toStateIndex,
			     size_t clauseCount,
			     const L2rClause * const *clauses,
			     bool copyClauses,
			     bool isNominal,
			     unsigned rank) :
  nclauses_(clauseCount), mode_(mode), from_(fromStateIndex),
  to_(toStateIndex), rank_(rank), isNominal_(isNominal)
{
  if (copyClauses) {
    const L2rClause **ccopy =
      L2_alloc_array_no_ctor(const L2rClause*, clauseCount);
    for (unsigned i = 0; i < clauseCount; i++) {
      assert(i < clauseCount);
      ccopy[i] = clauses[i];
    }
    clauses_ = ccopy;
  } else {
    clauses_ = clauses;
  }
}


// Deletethe array of L2rClause objects
L2rTransition::~L2rTransition() 
{
  // cast away a const
  L2_free_array_no_dtor(const_cast<const L2rClause**>(clauses_), nclauses());
}
