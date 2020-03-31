/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_remove_duplicates.cpp,v 1.15 2004/01/28 23:31:51 lbrown Exp $ */
#include <readers/xlate_remove_duplicates.h>

#include <readers/L2_file.h>
#include <readers/clause.h>

// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) \
   do { if (isVerbose()) { get_output() << expr; } } while(0)
#else
#  define verbose(expr)
#endif

/**
 * This inner class is a wrapper for an L2rProposition that has a hash code.
 * It is used as a key in a hash table that maps L2rProposition objects to
 * the "representative" of their equalivance class.
 */

class xlate_remove_duplicates::Hashable_prop {
public:

  /**
   * Construct from an existing L2rProposition and the enclosing
   * L2_file_translator.
   * L2rProposition is an abstract class, so parameter pL2rProposition must be
   * of type either L2rPropVarVar or L2rPropVarValue. However, we cannot
   * dispatch on the type of that parameter. Subclassing is probably best, but
   * that is a major redesign.
   */

  Hashable_prop(const L2rProposition *pL2rProposition,
		L2_file_translator *x) {
    if (pL2rProposition->isEquality()) {
      // Class L2rPropVarVar (variable1 == variable2)
      const L2rPropVarVar *pL2rPropVarVar =
	static_cast<const L2rPropVarVar*>(pL2rProposition);
      initialize(x->get_dest_var(pL2rPropVarVar->var()),
		 x->get_dest_var(pL2rPropVarVar->otherVar()),
		 pL2rPropVarVar->isPositive());
    } else {
      // Class L2rPropVarValue (variable == value)
      const L2rPropVarValue *pL2rPropVarValue =
	static_cast<const L2rPropVarValue*>(pL2rProposition);
      initialize(x->get_dest_var(pL2rPropVarValue->var()),
		 pL2rPropVarValue->value(),
		 pL2rPropVarValue->isPositive());
    }
  }


  /**
   * Construct variable1 == variable2
   * This apparently isn't used; comment it out for now.
  Hashable_prop(const L2rVariable *pL2rVariable1,
		const L2rVariable *pL2rVariable2,
		bool isPositive) {
    initialize(pL2rVariable1, pL2rVariable2, isPositive);
  }
  */
  

  /**
   * Construct variable == value
   * This apparently isn't used; comment it out for now.
  Hashable_prop(const L2rVariable *pL2rVariable,
                unsigned valueIndex,
		bool isPositive) {
    initialize(pL2rVariable, valueIndex, isPositive);
  }
  */


  /**
   * Auxiliary to constructors for L2rPropVarValue (variable == value)
   */

  void initialize(const L2rVariable *pL2rVariable, unsigned valueIndex,
		  bool isPositive) {
    var = pL2rVariable;
    value = valueIndex;
    is_equality = false;
    is_positive = isPositive;
  }


  /**
   * Auxiliary to constructors for L2rPropVarVar (variable1 == variable2).
   * Put the two variables in a canonical order -- increasing IDs.
   */

  void initialize(const L2rVariable *pL2rVariable1,
		  const L2rVariable *pL2rVariable2,
		  bool isPositive) {
    if (pL2rVariable1->id() < pL2rVariable2->id()) {
      // Increasing; as is
      var      = pL2rVariable1;
      otherVar = pL2rVariable2;
    } else if (pL2rVariable1->id() > pL2rVariable2->id()) {
      // Decreasing; swap
      var      = pL2rVariable2;
      otherVar = pL2rVariable1;
    } else {
      // Equal; we should check and not create these
      L2_assert(pL2rVariable1->id() != pL2rVariable2->id(),
		L2_fatal_error,
		("creating var=var prop with only one var=" +
		 MBA_string(pL2rVariable1)));
    }
    is_equality = true;
    is_positive = isPositive;
  }


  /**
   * Two Hashable_prop objects are equal if (1) their two variables are
   * equal; (2) their senses (positive vs. negative) are equal; (3) they
   * are both variable1 == variable2 or both variable == value; and (4) if
   * variable1 == variable2, the variable2's must be the same and if 
   * variable == value, the two values are the same.
   */

  bool operator == (const Hashable_prop& other) const {
    return
      (var         == other.var) &&
      (is_positive == other.is_positive) &&
      (is_equality == other.is_equality) &&
      ((is_equality  && (otherVar == other.otherVar)) ||
       (!is_equality && (value    == other.value)));
  }


  /**
   * Definition of the hash code
   * For variable1 == variable2 propositions: the sum of variable pointers
   * For variable == value propositions :the sum of the variable pointer and
   * the value index.
   * For both propositions, if the sense is positive, multiply by two
   */

  unsigned hashcode() const {
    unsigned ret = unsigned(var);
    if (is_equality) {
      ret += unsigned(otherVar);
    } else {
      ret += value;
    }
    if (is_positive)
      ret <<= 1;
    return ret;
  }


  /**
   * Return the hash code for this object. This is passed to the constructor
   * for a Hash_table with Hashable_prop key in copy_all_clauses().
   */

  static unsigned hash(const Hashable_prop& hp) {
    return hp.hashcode();
  }


  /**
   * Create an L2rProposition using the supplied propositionID and the
   * object's data members.
   */

  L2rProposition *create_prop(unsigned id) const {
    // note: no safeguard against creating several.
    if (is_equality) {
      return new L2rPropVarVar(id, var, is_positive, otherVar);
    } else {
      return new L2rPropVarValue(id, var, is_positive, value);
    }
  }

private:
  // If variable1 == variable2, variable1; if variable == value, variable
  const L2rVariable *var;
  // If variable1 == variable2, variable2
  const L2rVariable *otherVar;
  // If variable == value, value
  unsigned value;
  // If true, variable1 == variable2; else variable == value
  bool is_equality;
  // Whether the proposition is negated
  bool is_positive;
};


/** This inner class is a wrapper for an L2rClause that has a hash code.
 *  It is used as a key in a hash table that maps L2rClause objects onto the
 *  "representative" of their equivalence class.
 */

class xlate_remove_duplicates::Hashable_clause {
public:
  Hashable_clause(const L2rClause *pL2rClause, L2_file_translator *x) {
    hash_cache = 0;  // The clause's hash code
    nprops_ = 0;     // The number of L2rPropositions in the L2rClause
    
    const L2rVariable *unsatisfiable = 0;
    
    // For each L2rProposition in the L2rClause
    for (unsigned i = 0; i < pL2rClause->nprops(); ++i) {
      // Retrieve the L2rProposition
      const L2rProposition *original = pL2rClause->prop(i);
      if (original->isEquality()) {
	// L2rPropVarVar (variable1 == variable2)
	const L2rPropVarVar *pL2rPropVarVar =
	  static_cast<const L2rPropVarVar*>(original);
	if (pL2rPropVarVar->var() == pL2rPropVarVar->otherVar()) {
	  // v1=v1 is trivially true ;
	  // ! v1=v1 is trivially false.
	  if (original->isPositive()) {
	    // v1=v1 is true, so the entire clause is already satisfied by
	    // this true disjunct, so don't bother.
	    nprops_ = 0;
	    return;
	  } else {
	    // ! v1=v1 is false, so this false disjunct proposition doesn't
	    // affect the truth value -- except if this is a singleton clause.
	    unsatisfiable = pL2rPropVarVar->var();
	    continue;
	  }
	} else {
	  // L2rPropVarValue (variable == value); can't be optimized away
	}
      }

      // The general case, nothing weird is up.
      const L2rProposition *translated = x->get_dest_prop(original);
      L2_assert(translated,
		L2_fatal_error,
		("failed to map prop " + MBA_string(original->id())));
      add_prop(translated);
    }
    
    L2_assert(!unsatisfiable || nprops_ > 0,
	      L2_reader_error,
	      ("unsatisfiable clause asserting v" +
	       MBA_string(unsatisfiable->id()) + "!=v" +
	       MBA_string(unsatisfiable->id())));

#ifdef ENABLE_L2_DEBUG_SECTIONS
    if (pL2rClause->hasDebug()) {
      component_name =
	static_cast<const dbg_L2rClause*>(pL2rClause)->component();
    }
#endif
  }


  /**
   * Two Hashable_clause objects are equal if (1) their hashcodes are equal;
   * (2) they have the same number of propositions; and (3) their proposition
   * lists have the same elements
   */

  bool operator == (const Hashable_clause& other) const {
    return
      // a couple constant-time checks first
      (hashcode() == other.hashcode()) &&
      (nprops_ == other.nprops_) &&
      hasSamePropositions(other);
  }


  /** Define the hash code: the sum of the L2rProposition pointers */
  unsigned hashcode() const { return hash_cache; }

  
  /** Return the number of L2rPropositions in the optimized L2rClause */
  unsigned nprops() const { return nprops_; }


  /** Return the hash code for this object. This is passed to the constructor
   *  for a Hash_table with Hashable_clause key in copy_all_props().
   */

  static unsigned hash(const Hashable_clause& hc) {
    return hc.hashcode();
  }


  typedef Slist<const L2rProposition*>::iterator iterator;


  /** The begin iterator of the L2rClause's L2rProposition list */
  iterator begin() const { // begin is not const, somehow
    return const_cast<Slist<const L2rProposition*>&>(props).begin();
  }


  /** The end iterator of the L2rClause's L2rProposition list */
  iterator end() const {   // end is not const, somehow
    return const_cast<Slist<const L2rProposition*>&>(props).end();
  }


  /**
   * Create an L2rClause using the supplied clauseID and the object's data
   * members.
   */

  L2rClause *create_clause(unsigned id) const {
    // Allocate memory for the L2rClause's array of L2rProposition pointers
    const L2rProposition **propositions =
      L2_alloc_array_no_ctor(const L2rProposition*, nprops_);
    unsigned i = 0; // Apparently not used
    // Copy the clause's propositions
    for (iterator it = begin(); it != end(); ++it, ++i) {
      const L2rProposition *pL2rProposition = *it;
      propositions[i] = pL2rProposition;
    }
    // Create the L2rClause object
#ifdef ENABLE_L2_DEBUG_SECTIONS
    if (component_name != "") {
      return new dbg_L2rClause(component_name, id, nprops_, propositions,
			       false);
    } else
#endif
      {
	return new L2rClause(id, nprops_, propositions, false);
      }
  }


private:
  /**
   * Does the parameter Hashable_clause have the same L2rProposition elements?
   * Assume both have the same number of elements and are sorted in the same
   * order.
   */

  bool hasSamePropositions(const Hashable_clause& other) const {
    iterator it1 = begin();
    iterator it2 = other.begin();
    for ( ; it1 != end(); ++it1, ++it2) {
      const L2rProposition *pL2rProposition1 = *it1;
      const L2rProposition *pL2rProposition2 = *it2;
      if (pL2rProposition1 != pL2rProposition2) { return false; }
    }
    return true;
  }

  /**
   * Insert the L2rProposition into Slist of propositions in increasing order
   * of their pointers, if it is not already there.
   */

  void add_prop(const L2rProposition *p) {
    // sort, and ensure no duplicates
    Slist<const L2rProposition*>::iterator it  = props.begin();
    Slist<const L2rProposition*>::iterator pit = props.begin();
    for( ; it != props.end(); ++it ) {
      if (p == *it) {
	// The proposition is already in the list; don't add it
	return;
      } else if (p < *it) {
	// Found insertion point; insert before "previous" iterator
	break;
      } else {
	// Keep going
	pit = it;
      }
    }

    // Not found, so insert it before "previous" iterator
    props.insert_before(pit, p);
    // Increment the number of propositions
    ++nprops_;
    // Add the proposition pointer to the hash code
    hash_cache += unsigned(p);
  }

private:
  // All L2rPropostions in the L2rClause; sorted in increasing order
  Slist<const L2rProposition*> props;
  // The number of L2rPropositions in the L2rClause
  unsigned nprops_;
  unsigned hash_cache;
#ifdef ENABLE_L2_DEBUG_SECTIONS
  // Debugging info ; "" for no info
  MBA_string component_name;
#endif
};


/***************************************************************************
  Constructor
 ***************************************************************************/

xlate_remove_duplicates::xlate_remove_duplicates(const L2_file *s, L2_file *d)
    : L2_file_translator(s,d)
{
}


/***************************************************************************
  Translation
 ***************************************************************************/

// Remove trivially true (variable1 == variable1) L2rPropositions and merge
// L2rPropositions that have the same hash code (based on addresses of
// variables, indices of values, and whether it is an equality or inequality)

void xlate_remove_duplicates :: copy_all_props() {
  Hash_table<Hashable_prop, L2rProposition*> newprops(Hashable_prop::hash);
  // For lookup; initial value is not used
  Hash_table<Hashable_prop, L2rProposition*>::iterator it = newprops.begin();

  // foreach prop,
  // look it up in the table;
  // if we find it, add the mapping
  // if we don't,
  //    put it in the table
  //    add the mapping
  // keep a count of how many propositions we've created
  unsigned nprops = 0;

  for (unsigned i = 0; i < source()->nprops(); ++i) {
    const L2rProposition *pSourceL2rProposition = source()->getProp(i);
    if (pSourceL2rProposition->isEquality()) {
      const L2rPropVarVar *pvv =
	static_cast<const L2rPropVarVar*>(pSourceL2rProposition);
      if (pvv->var() == pvv->otherVar()) {
	// Skip if this is a variable1 == variable1 [sic] proposition
	verbose("Ignoring useless proposition " << pSourceL2rProposition
		<< _STD_ endl);
	continue;
      }
    }

    L2rProposition *pDestinationL2rProposition;
    Hashable_prop hp(pSourceL2rProposition, this);
    it = newprops.find(hp);
    if (it == newprops.end()) {
      // The L2rProposition is not yet in the hash table; add it
      pDestinationL2rProposition = hp.create_prop(nprops++);
      newprops.insert(hp, pDestinationL2rProposition);
    } else {
      // The L2rProposition is already in the hash table; it maps onto itself
      verbose("Ignoring duplicate proposition " << pSourceL2rProposition
	      << _STD_ endl);
      pDestinationL2rProposition = *it;
    }
    // Augment many-to-one mapping
    add_mapping(pSourceL2rProposition, pDestinationL2rProposition);
  }

  // Allocate the L2rProposition arrays inside the destination L2_file
  dest()->allocProps(nprops);
  for (it = newprops.begin() ; it != newprops.end() ; ++it) {
    L2rProposition *pL2rProposition = *it;
    dest()->setProp(pL2rProposition->id(), pL2rProposition);
  }
}


// Remove L2rClauses with no L2rPropositions and merge L2rClauses that have the
// same hash code (the sum of the addresses of the L2rPropositions).

void xlate_remove_duplicates :: copy_all_clauses() {
  Hash_table<Hashable_clause, L2rClause*> newclauses (Hashable_clause::hash);
  // For lookup; initial value is not used
  Hash_table<Hashable_clause, L2rClause*>::iterator it = newclauses.begin();

  unsigned destinationClauseCount = 0; // also the id of the next clause
  for (unsigned i = 0; i < source()->nclauses(); ++i) {
    const L2rClause *pSourceL2rClause = source()->getClause(i);
    Hashable_clause hashableClause(pSourceL2rClause, this);
    if (hashableClause.nprops() == 0) {
      // This clause turned out to be a tautology; ignore it.
      verbose("Ignoring tautological clause "<< pSourceL2rClause << _STD_ endl);
    } else {
      L2rClause *pDestinationL2rClause;
      it = newclauses.find(hashableClause);
      if (it == newclauses.end()) {
	// The L2rClause is not yet in the hash table; add it
	pDestinationL2rClause =
	  hashableClause.create_clause(destinationClauseCount++);
	newclauses.insert(hashableClause, pDestinationL2rClause);
      } else {
	// The L2rClause is already in the hash table; it maps onto itself
	verbose("Ignoring duplicate clause "<< pSourceL2rClause << _STD_ endl);
	pDestinationL2rClause = *it;
      }
      // Augment many-to-one mapping
      add_mapping(pSourceL2rClause, pDestinationL2rClause);
    }
  }

  // Allocate the L2rClause array inside the destination L2_file.
  // Note if we have multiple source clauses mapping to the same destination
  // one, only one component name will be given
  dest()->allocClauses(destinationClauseCount,
		       source()->dbg_clauses());
  for (it = newclauses.begin(); it != newclauses.end(); ++it) {
    L2rClause *pL2rClause = *it;
    dest()->setClause(pL2rClause->id(), pL2rClause);
  }
}
