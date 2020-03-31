/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_strip.cpp,v 1.10 2002/01/02 23:13:13 lbrown Exp $ */
#include <readers/xlate_strip.h>

#include <readers/L2_file.h>
#include <readers/transition.h>

// For each L2rEnumeration, possibly a dbg_L2rEnumeration, make a new
// L2rEnumeration from it
// Furthermore, all L2rEnumerations of the same cardinality can be merged
// Note that the latter is not stripping; it is merging.

void xlate_strip::copy_all_enums() {
  // Maps from the cardinality to the destination enum for that cardinality.
  Hash_table<unsigned, L2rEnumeration*> sizes (hash_integral_key);
  // Number of distinct cardinalities; used as 0-based enum ID as well
  unsigned cardinalityCount = 0; 
  // Set of destinatation L2rEnumerations to create
  Slist<L2rEnumeration*> stripped_enums;

  for (unsigned i = 0; i < get_source()->nenums() ; ++i) {
    const L2rEnumeration *pFromL2rEnumeration = get_source()->getEnum(i);
    unsigned cardinality = pFromL2rEnumeration->nmembers();
    L2rEnumeration *pToL2rEnumeration;
    Hash_table<unsigned, L2rEnumeration*>::iterator it =
      sizes.find(cardinality);
    if (it == sizes.end()) {
      // No L2rEnumeration in the hash table with this cardinality; create one
      pToL2rEnumeration = new L2rEnumeration(cardinalityCount++, cardinality);
      sizes.insert(cardinality, pToL2rEnumeration);      // hash table
      stripped_enums.push_front(pToL2rEnumeration);      // destination
    } else {
      // The iterator points to the L2rEnumeration with this cardinality
      pToL2rEnumeration = *it;
    }
    add_mapping(pFromL2rEnumeration, pToL2rEnumeration);
  }

  // Put the new L2rEnumerations into the L2_file.  No debug info of course.
  dest()->allocEnums(cardinalityCount, false);
  for (Slist<L2rEnumeration*>::iterator it = stripped_enums.begin();
      it != stripped_enums.end() ; ++it) {
    L2rEnumeration *pL2rEnumeration = *it;
    dest()->setEnum( pL2rEnumeration->id(), pL2rEnumeration );
  }
}


// Variables are easy; just copy them all; make L2rVariable,
// which don't have debug info.
// For each L2rVariable, possibly a dbg_L2rVariable, make a new L2rVariable
// from it

void xlate_strip::copy_all_vars() {
  unsigned variableCount = source()->nvars();
  dest()->allocVars(variableCount, false);

  for (unsigned i = 0; i < variableCount; ++i) {
    const L2rVariable *pFromL2rVariable = source()->getVar(i);
    const L2rEnumeration *dest_type = get_dest_enum(pFromL2rVariable->type());
    L2_assert(dest_type,
	      L2_reader_error,
	      ("no matching enumeration found for source var " +
	       MBA_string(pFromL2rVariable->id()) + " of type " +
	       MBA_string(pFromL2rVariable->type()->id())));
    L2rVariable *pToL2rVariable = new L2rVariable(i, dest_type);
    pToL2rVariable->setInitial(pFromL2rVariable->initial());
    pToL2rVariable->setKind(pFromL2rVariable->kind());
    dest()->setVar(i, pToL2rVariable);
    add_mapping(pFromL2rVariable, pToL2rVariable);
  }
}


// Quite easy: just like the variables code, in fact.
// For each L2rClause, possibly a dbg_L2rClause, copy the L2rPropositions and
// make a new L2rClause from it.

void xlate_strip::copy_all_clauses() {
  unsigned clauseCount = source()->nclauses();
  dest()->allocClauses(clauseCount, false);

  for (unsigned i = 0; i < clauseCount; ++i) {
    const L2rClause *pFromL2rClause = source()->getClause(i);
    unsigned propositionCount = pFromL2rClause->nprops();
    const L2rProposition **propositions =
      L2_alloc_array_no_ctor(const L2rProposition*, propositionCount);
    for (unsigned j = 0; j < propositionCount; ++j) {
      propositions[j] = get_dest_prop(pFromL2rClause->prop(j));
    }
    L2rClause *pToL2rClause =
      new L2rClause(i, propositionCount, propositions, false);
    if (pFromL2rClause->isInBackground()) { pToL2rClause->putInBackground(); }
    dest()->setClause(i, pToL2rClause);
    add_mapping(pFromL2rClause, pToL2rClause);
  }
}


// For the given L2rTransition, possible a dbg_L2rTransition, copy the
// L2rClauses and make a new L2rTransition from them.

L2rTransition *
xlate_strip::copy_transition(const L2rTransition *pFromL2rTransition,
			     bool add_to_dest) {
  unsigned clauseCount = pFromL2rTransition->nclauses();
  const L2rClause **clauses =
    L2_alloc_array_no_ctor(const L2rClause*, clauseCount);
  for (unsigned j = 0; j < clauseCount; ++j) {
    clauses[j] = get_dest_clause(pFromL2rTransition->clause(j));
  }
  L2rTransition *pToL2rTransition =
    new L2rTransition(get_dest_var(pFromL2rTransition->mode()),
		      pFromL2rTransition->from(),
		      pFromL2rTransition->to(),
		      clauseCount,
		      clauses,
		      false,              // don't copy
		      pFromL2rTransition->isNominal(),
		      pFromL2rTransition->rank());

  // Add the destination L2rTransition to the destination L2_file
  if (add_to_dest) { dest()->addTransition(pToL2rTransition); }
  return pToL2rTransition;
}
