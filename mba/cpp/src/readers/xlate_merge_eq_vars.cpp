/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_merge_eq_vars.cpp,v 1.26 2004/02/05 22:36:20 lbrown Exp $ */

#include <readers/xlate_merge_eq_vars.h>
#include <readers/L2_file.h>
#include <readers/clause.h>

#ifdef ENABLE_L2_DEBUG_SECTIONS
#include <debuggers/livingstone_debug.h>
#endif

// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) do { if(isVerbose()) { expr; } } while(0)
#else
#  define verbose(expr)
#endif


/**
 * This inner class represents the equivalence class of L2rVariables based on
 * a "representative" supplied to its constructor.  Needed by the xlate dtor.
 */

class xlate_merge_eq_vars::EqClass {
public:
  typedef Slist<const L2rVariable*>::iterator iterator;

  EqClass (const L2rVariable *rep) : representative(rep) {
    members.push_front(rep);
  }

  iterator begin() const {
    return const_cast<EqClass*>(this)->members.begin();
  }

  iterator end() const {
    return const_cast<EqClass*>(this)->members.end();
  }

  const L2rVariable* get_representative() const {
    return representative;
  }

  /**
   * Does the member list contain the given variable?
   */

  bool contains(const L2rVariable *pTargetL2rVariable) const {
    for (iterator it = begin(); it != end(); ++it) {
      const L2rVariable *pL2rVariable = *it;
      if (pL2rVariable->id() == pTargetL2rVariable->id())
	return true;
    }
    return false;
  }

  /**
   * Add the supplied variable to the equivalence class
   */

  void add(const L2rVariable *var) {
    // The representative should point to some kind of interesting Variable if
    // there is any in the class.  If there isn't, it will point to the first
    // Variable (an arbitrary choice).
    if (var->kind() != vk_unknown) {
      L2_assert(representative->kind() == vk_unknown,
		L2_optimizer_error,
		("Multiple representatives (" +
		 MBA_string(representative->id()) + ", " +
		 MBA_string(var->id()) + ") "
		 "found for an equivalence class"));
      representative = var;
    }
    members.push_front(var);
  }

  /**
   * this->members <- this->members UNION other.members
   */

  void absorb(const EqClass& other) {
    for (iterator it = other.begin(); it != other.end(); ++it) {
      const L2rVariable *pL2rVariable = *it;
      add(pL2rVariable);
    }
  }

private:
  // The L2rVariable objects that are in the equivalence class
  Slist<const L2rVariable*> members;
  // The representative L2rVariable object of the equivalence class
  const L2rVariable* representative;

};


/***************************************************************************
  Destructor.
 ***************************************************************************/

// Delete all the equivalence classes
xlate_merge_eq_vars::~xlate_merge_eq_vars() {
  for (Slist<EqClass*>::iterator it = eq_classes.begin();
       it != eq_classes.end(); ++it ) {
    EqClass *pEqClass = *it;
    delete pEqClass;
  }
  eq_classes.erase(); // not needed
}


/***************************************************************************
  Compute the equivalences classes according to the unconditional equivalences
  passed in.

  This could be much faster (linear-time rather than quadratic) with a
  union-find set, but this is ground-only code anyway.
 ***************************************************************************/

xlate_merge_eq_vars::EqClass*
xlate_merge_eq_vars::get_equivalence_class(const L2rVariable *pL2rVariable) {

  // Look for it in all existing equivalence classes
  for (Slist<EqClass*>::iterator it = eq_classes.begin();
       it != eq_classes.end() ; ++it) {
    EqClass *pEqClass = *it;
    if (pEqClass->contains(pL2rVariable)) { return pEqClass; }
  }

  // If not found, create its own class. This occurs if the Variable is did
  // not appear in the set of Variables but appears in a Variable1=Variable2
  // Proposition.
  EqClass* pNewEqClass = new EqClass(pL2rVariable);
  eq_classes.push_front(pNewEqClass);
  return pNewEqClass;
}


// Each element of parameter equivalences is a background Proposition asserting
// that Variable1=Variable2. Use this set to prune redundancies from the set of
// equivalence classes.
// Auxiliary to find_equivalences()

void
xlate_merge_eq_vars::compute_equivalence_classes(Slist<const L2rPropVarVar*>&
						 equivalences) {
    
  for (Slist<const L2rPropVarVar*>::iterator it = equivalences.begin();
       it != equivalences.end(); ++it)  {
    const L2rPropVarVar *pL2rPropVarVar = *it;
    EqClass* pEqClass1 = get_equivalence_class(pL2rPropVarVar->var());
    EqClass* pEqClass2 = get_equivalence_class(pL2rPropVarVar->otherVar());
    
    if (pEqClass1 != pEqClass2) {
      // Not the same object; merge them
      pEqClass1->absorb(*pEqClass2);
      // pEqClass2 is no longer needed; remove it
      eq_classes.remove(pEqClass2);
      delete pEqClass2;
    }
  }
}


// If the L2rClause contains a single variable=variable L2rProposition,
// return it
// Auxiliary to find_equivalences()

const L2rPropVarVar *
xlate_merge_eq_vars::get_equivalence(const L2rClause* pL2rClause) {
  if (1 == pL2rClause->nprops()) {
    // The L2rClause has one L2rProposition; retrieve it
    const L2rProposition *pL2rProposition = pL2rClause->prop(0);
    if (pL2rProposition->isEquality()) {
      // It is a variable=variable proposition; return it
      return static_cast<const L2rPropVarVar*>(pL2rProposition);
    } else {
      // Not a variable=variable proposition
      return NULL;
    }
  } else {
    // Not even a single-proposition clause
    return NULL;
  }
}


// Auxiliary to copy_all_vars()

void xlate_merge_eq_vars::find_equivalences() {

  // First create an equivalence class for each L2rVariable in the source
  for (unsigned i = 0; i < source()->nvars(); ++i) {
    const L2rVariable *pL2rVariable = source()->getVar(i);
    eq_classes.push_front(new EqClass(pL2rVariable));
  }

  // Now, find all unconditional equalities in the model
  Slist<const L2rPropVarVar*> equivalences;
  for (unsigned int index = 0;index < source()->nclauses(); ++index) {
    const L2rClause* pL2rClause = source()->getClause(index);
    if (pL2rClause->isInBackground()) {
      const L2rPropVarVar* pL2rPropVarVar = get_equivalence(pL2rClause);
      if (pL2rPropVarVar != 0)  {
	// The Clause asserts Variable1=Variable2
	equivalences.push_front(pL2rPropVarVar);
      }
    }
  }

  // Compute the equivalence classes.  That is, if A=B and B=C, then {A,B,C}
  // is an equivalence class and all Variables must have the same value.
  compute_equivalence_classes(equivalences);
}


/***************************************************************************
  The only work: redefine copying the variables.
 ***************************************************************************/

void xlate_merge_eq_vars::copy_all_vars() {
  // Construct the set of equivalence classes. Equivalence of variable1 and
  // variable2 is defined by there being a clause with a single proposition
  // that asserts that variable1==variable2
  find_equivalences();

  // For each equivalence class, copy the representative, and add a mapping for
  // all others to the copy of the representative. All Variables belong to an
  // equivalence class (many of which are singletons), so there as many
  // equivalence classes as will be Variables in the output model.
  dest()->allocVars( eq_classes.size(), source()->dbg_vars() );

  // Serial enumerator
  unsigned variableID = 0;
  // Each equivalence class will result in one Variable
  for (Slist<EqClass*>::iterator it = eq_classes.begin();
      it != eq_classes.end(); ++it, ++variableID) {
    const EqClass *pEqClass = *it;
    // The variable that represents the equivalence class
    const L2rVariable *pRepresentative = pEqClass->get_representative();
    L2rVariable *to = copy_var(pRepresentative, variableID);
    verbose(get_output()
	    << "Representative " << *pRepresentative
            << " mapped to "     << *to << _STD_ endl);

    // Map from each Variable in the class to the representative (this maps
    // the representative twice, but that's ok)
    for (EqClass::iterator eq_it = pEqClass->begin(); eq_it != pEqClass->end();
	 ++eq_it) {
      const L2rVariable *pL2rVariable = *eq_it;
      verbose(get_output()
	      << "    " << *pL2rVariable << "  ==>  " << *to << _STD_ endl);
      add_mapping(pL2rVariable, to);
#ifdef ENABLE_L2_DEBUG_SECTIONS
      if (pL2rVariable->hasDebug()) {
	// We know that the L2rVariable objects are dbg_L2rVariable
	const dbg_L2rVariable *dbgFrom =
	  static_cast<const dbg_L2rVariable *>(pL2rVariable);
	const dbg_L2rVariable *dbgTo =
	  static_cast<const dbg_L2rVariable *>(to);
	MBA_string fromName = dbgFrom->name();
	MBA_string toName   = dbgTo->name();
	if (fromName != toName) {
	  // To get from fromName to toName
	  setVariableMapping(fromName, toName);
	  // To store which variables have been optimized away
	  addOptimizedAwayVariableName(fromName);
	}
      }
#endif
    }
  }
}


#ifdef ENABLE_L2_DEBUG_SECTIONS
void
xlate_merge_eq_vars::getOptimizedAwayVariableNames(Array<MBA_string, true>&
						   array) {
  // Start afresh
  array.erase();
  for (Array<MBA_string, true>::iterator it =
	 optimizedAwayVariableNames.begin();
       it != optimizedAwayVariableNames.end(); ++it) {
    MBA_string variableName = *it;
    array.push(variableName);
  }
}


void
xlate_merge_eq_vars::getVariableHashMap(Hash_table<MBA_string, MBA_string>&
					hash_table) {
  hash_table.erase();
  for (Hash_table<MBA_string, MBA_string>::iterator it =
	 variableHashMap.begin(); it != variableHashMap.end(); ++it) {
    MBA_string key = it.key();
    MBA_string value = *it;
    hash_table.insert(key, value);
  }
}
#endif

