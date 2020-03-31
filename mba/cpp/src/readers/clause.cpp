/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: clause.cpp,v 1.11 2004/02/11 00:54:51 lbrown Exp $ */
#include <readers/clause.h>

// If copy, allocate new memory for the array of L2rProposition objects
L2rClause::L2rClause(unsigned clauseID,
		     size_t propositionCount,
		     const L2rProposition* const * propositions,
		     bool copyPropositions) :
  clauseID_(clauseID), nprops_(propositionCount), isInBackground_(false)
{
  if (copyPropositions) {
    // Make a copy of propsitions and store it
    const L2rProposition **pcopy =
      L2_alloc_array_no_ctor(const L2rProposition*, propositionCount);
    for (unsigned i = 0; i < propositionCount; i++) {
      assert(i < propositionCount);
      pcopy[i] = propositions[i];
    }
    props_ = pcopy;
  } else {
    // Store the propositions as given
    props_  = propositions;
  }
}


// Delete the array of L2rProposition objects
L2rClause::~L2rClause() {
  // cast away a const
  L2_free_array_no_dtor(const_cast<const L2rProposition* *>(props_),
			nprops());
}


#ifdef ENABLE_L2_DEBUG_SECTIONS
// The only thing that is different is the "from" parameter
dbg_L2rClause::dbg_L2rClause(MBA_string from,
			     unsigned clauseID,
			     size_t propositionCount,
			     const L2rProposition* const * propositions,
			     bool copyPropositions)
  : L2rClause(clauseID, propositionCount, propositions, copyPropositions),
    fromComponent_(from)
{
}
#endif
