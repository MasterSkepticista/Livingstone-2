/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file_translator.cpp,v 1.23 2003/10/27 22:49:56 lbrown Exp $ */

#include <readers/L2_file_translator.h>
#include <readers/L2_file.h>
#include <readers/transition.h>

/***************************************************************************
  Templates to do all the real work for us.
 ***************************************************************************/

// Look up the destination corresponding to the supplied source

template <class T>
T get_destination(T src, Hash_table<T, T>& mapping) {
  typename Hash_table<T, T>::iterator it = mapping.find(src);
  return (it == mapping.end()) ? 0 : *it;
}


// Look up all the list of sources corresponding to the supplied destination

template <class T>
void get_sources(T dest, Slist<T>& srcs, Hash_table< T, Slist<T> >& mapping) {
  typename Hash_table<T, Slist<T> >::iterator it = mapping.find(dest);
  if (it == mapping.end()) {
    srcs.erase();  // return an empty list
  } else {
    srcs = *it; // copy the list
  }
}


// Add a (from, to) pair to the many-to-one mapping

template <class T>
void add_mapping(T from,
		 T to,
		 Hash_table<T, T>& foremap,
		 Hash_table<T, Slist<T> >& backmap) {
  // If already mapped, ignore (but assert we're just mapping to the same
  // thing twice)
  T search = get_destination(from, foremap);
  if (search) {
    L2_assert(search == to,
	      L2_reader_error,
	      ("mapping " + MBA_string(from->id()) + " twice ("
	       + MBA_string(search->id()) + ", "
	       + MBA_string(to->id()) + ")"));
    return;
  } else {
    // Just insert into the forward mapping
    foremap.insert(from, to);

    // If not already in the backward mapping, add it;
    // if already there, add to its list
    typename Hash_table<T, Slist<T> >::iterator it = backmap.find(to);
    if (it == backmap.end()) {
      Slist<T> list (from); // singleton list
      backmap.insert(to, list);
    } else {
      // add to the list.  It won't let us get away with this very easily.
      const_cast<Slist<T>&>(*it).push_front(from);
    }
  }
}


/***************************************************************************
  Translation.  This may be overridden.
 ***************************************************************************/

bool
L2_file_translator::translate() {
  L2_assert(get_source(),
	    L2_initialization_error,
            ("no model to translate from"));
  L2_assert(get_dest(),
	    L2_initialization_error,
            ("no model to translate to"));

  copy_all_enums();
  copy_all_vars();
  copy_all_props();
  copy_all_clauses();
  copy_all_transitions();
  return true;
}


/***************************************************************************
  Enumeration mapping
 ***************************************************************************/

const L2rEnumeration *
L2_file_translator::get_dest_enum(const L2rEnumeration *src) {
  return ::get_destination(src, fore_enum_map_);
}


void
L2_file_translator::get_source_enums(const L2rEnumeration *dest,
				     Slist<const L2rEnumeration*>& srcs) {
  ::get_sources(dest, srcs, back_enum_map_);
}


void
L2_file_translator::add_mapping(const L2rEnumeration *from,
				const L2rEnumeration *to) {
  ::add_mapping(from, to, fore_enum_map_, back_enum_map_);
}


/***************************************************************************
  Variable mapping
 ***************************************************************************/

const L2rVariable *
L2_file_translator::get_dest_var(const L2rVariable *src) {
  return ::get_destination(src, fore_var_map_);
}


void
L2_file_translator::get_source_vars(const L2rVariable *dest,
				    Slist<const L2rVariable*>& srcs) {
  ::get_sources(dest, srcs, back_var_map_);
}


void
L2_file_translator::add_mapping(const L2rVariable *from,
				L2rVariable *to) {
  // If the source has an interesting kind, set it
  if (from->kind() != vk_unknown) {
    to->setKind(from->kind());
  }
  // If the source has a non-default initial value, set it
  if (from->initial() != L2rTransition::ANY_MODE) {
    to->setInitial(from->initial());
  }
  add_mapping_unchecked(from, to);
}


void
L2_file_translator::add_mapping_unchecked(const L2rVariable *from,
					  const L2rVariable *to) {
  ::add_mapping(from, to, fore_var_map_, back_var_map_);
}


/***************************************************************************
  Proposition mapping
 ***************************************************************************/

const L2rProposition *
L2_file_translator::get_dest_prop(const L2rProposition *src) {
  return ::get_destination(src, fore_prop_map_);
}


void
L2_file_translator::get_source_props(const L2rProposition *dest,
				     Slist<const L2rProposition*>& srcs) {
  ::get_sources(dest, srcs, back_prop_map_);
}


void
L2_file_translator::add_mapping(const L2rProposition *from,
				const L2rProposition *to) {
  ::add_mapping(from, to, fore_prop_map_, back_prop_map_);
}


/***************************************************************************
  Clause mapping
 ***************************************************************************/

const L2rClause *
L2_file_translator::get_dest_clause(const L2rClause *src) {
  return ::get_destination(src, fore_clause_map_);
}


void
L2_file_translator::get_source_clauses(const L2rClause *dest,
				       Slist<const L2rClause*>& srcs) {
  ::get_sources(dest, srcs, back_clause_map_);
}


void
L2_file_translator::add_mapping(const L2rClause *from,
				L2rClause *to) {
  if (from->isInBackground()) {
    to->putInBackground();
  }
  add_mapping_unchecked(from, to);
}


void
L2_file_translator::add_mapping_unchecked(const L2rClause *from,
					  const L2rClause *to) {
  ::add_mapping(from, to, fore_clause_map_, back_clause_map_);
}


/***************************************************************************
  Utility functions to copy a data member without changing it
  (i.e. same amount of debug info, etc).
 ***************************************************************************/

L2rEnumeration *
L2_file_translator::copy_enum(const L2rEnumeration *from,
			      int newID, bool add_to_dest) {
  L2_assert(newID == GET_FROM_SOURCE || newID >= 0,
	    L2_reader_error,
	    ("newID negative but not GET_FROM_SOURCE"));
  L2rEnumeration *to;
  unsigned id = (newID == GET_FROM_SOURCE) ? from->id() : newID ;

#ifdef ENABLE_L2_DEBUG_SECTIONS
  if (from->hasDebug()) {
    const dbg_L2rEnumeration *fdbg =
      static_cast<const dbg_L2rEnumeration*>(from);
    to = new dbg_L2rEnumeration(id, from->nmembers(),
				fdbg->name(),
				fdbg->names());
  } else
#endif
    // this is either an else clause, or just a random scope,
    // depending on whether the thing is defined
    {
        to = new L2rEnumeration(id, from->nmembers());
    }

  add_mapping(from, to);
  if (add_to_dest) dest()->setEnum(id, to);
  return to;
}


L2rVariable *
L2_file_translator::copy_var(const L2rVariable *from,
			     int newID,
			     bool add_to_dest) {
  L2_assert(newID == GET_FROM_SOURCE || newID >= 0,
	    L2_reader_error,
	    ("newID negative but not GET_FROM_SOURCE"));
  L2rVariable *to;
  unsigned id = (newID == GET_FROM_SOURCE) ? from->id() : newID ;

  const L2rEnumeration *dest_enum = get_dest_enum(from->type());

#ifdef ENABLE_L2_DEBUG_SECTIONS
  if (from->hasDebug()) {
    const dbg_L2rVariable *fdbg =
      static_cast<const dbg_L2rVariable*>(from);
    to = new dbg_L2rVariable(id, dest_enum, fdbg->name());
  } else
#endif
    // this is either an else clause, or just a random scope,
    // depending on whether the thing is defined
    {
        to = new L2rVariable(id, dest_enum);
    }

  to->setKind(from->kind());
  to->setInitial(from->initial());

  add_mapping(from, to);
  if (add_to_dest) dest()->setVar(id, to);
  return to;
}


L2rProposition *
L2_file_translator::copy_prop(const L2rProposition *from,
			      int newID,
			      bool add_to_dest) {
  L2_assert(newID == GET_FROM_SOURCE || newID >= 0,
	    L2_reader_error,
	    ("newID negative but not GET_FROM_SOURCE"));
  L2rProposition *to;
  unsigned id = (newID == GET_FROM_SOURCE) ? from->id() : newID ;

  if (from->isEquality()) {
    const L2rPropVarVar *f =
      static_cast<const L2rPropVarVar*>(from);
    to = new L2rPropVarVar(id,
			   get_dest_var(f->var()),
			   f->isPositive(),
			   get_dest_var(f->otherVar()));
  } else {
    const L2rPropVarValue *f =
      static_cast<const L2rPropVarValue*>(from);
    to = new L2rPropVarValue(id,
			     get_dest_var(f->var()),
			     f->isPositive(),
			     f->value());
  }

  add_mapping(from, to);
  if (add_to_dest) dest()->setProp(id, to);
  return to;
}


L2rClause *
L2_file_translator::copy_clause(const L2rClause *from,
				int newID, bool
				add_to_dest) {
  L2_assert(newID == GET_FROM_SOURCE || newID >= 0,
	    L2_reader_error,
	    ("newID negative but not GET_FROM_SOURCE"));
  L2rClause *to ;
  unsigned id = (newID == GET_FROM_SOURCE) ? from->id() : newID ;

  unsigned propositionCount = from->nprops();
  const L2rProposition **props =
    L2_alloc_array_no_ctor(const L2rProposition*, propositionCount);
  for (unsigned j = 0; j < propositionCount; ++j) {
    props[j] = get_dest_prop(from->prop(j));
  }

#ifdef ENABLE_L2_DEBUG_SECTIONS
  if (from->hasDebug()) {
    const dbg_L2rClause *fdbg = static_cast<const dbg_L2rClause*>(from);
    to =
      new dbg_L2rClause(fdbg->component(), id, propositionCount, props, false);
  } else
#endif
    {
      to = new L2rClause(id, propositionCount, props, false);
    }

  if (from->isInBackground()) {
    to->putInBackground();
  }
  if (add_to_dest) {
    dest()->setClause(id, to);
  }
  add_mapping(from, to);
  
  return to;
}


L2rTransition *
L2_file_translator::copy_transition(const L2rTransition *from,
				    bool add_to_dest) {
  unsigned clauseCount = from->nclauses();
  const L2rClause **cls = L2_alloc_array_no_ctor(const L2rClause*, clauseCount);
  for (unsigned j = 0; j < clauseCount; ++j) {
    cls[j] = get_dest_clause(from->clause(j));
  }

  L2rTransition *to;

#ifdef ENABLE_L2_DEBUG_SECTIONS
  if (from->hasDebug()) {
    const dbg_L2rTransition *fdbg = static_cast<const dbg_L2rTransition*>(from);
    to = new dbg_L2rTransition(fdbg->name(),
			       get_dest_var(from->mode()),
			       from->from(),
			       from->to(),
			       clauseCount,
			       cls,
			       false, // don't copy
			       from->isNominal(),
			       from->rank());
  } else
#endif
    {
      to = new L2rTransition(get_dest_var(from->mode()),
			     from->from(),
			     from->to(),
			     clauseCount,
			     cls,
			     false, // don't copy
			     from->isNominal(),
			     from->rank());
    }

  if (add_to_dest) dest()->addTransition(to);
  return to;
}


/***************************************************************************
  Utility functions to copy all of a section without changing it at all
  (same amount of debug info, etc).
 ***************************************************************************/

void L2_file_translator::copy_all_enums() {
  unsigned enumCount = source()->nenums();
  dest()->allocEnums(enumCount, source()->dbg_enums());
  for (unsigned i = 0; i < enumCount; ++i) {
    copy_enum(source()->getEnum(i));
  }
}


void L2_file_translator::copy_all_vars() {
  unsigned variableCount = source()->nvars();
  dest()->allocVars(variableCount, source()->dbg_vars());
  for (unsigned i = 0; i < variableCount; ++i) {
    copy_var(source()->getVar(i));
  }
}


void L2_file_translator::copy_all_props() {
  unsigned propositionCount = source()->nprops();
  dest()->allocProps(propositionCount);
  for (unsigned i = 0; i < propositionCount; ++i) {
    copy_prop(source()->getProp(i));
  }
}


void L2_file_translator::copy_all_clauses() {
  unsigned clauseCount = source()->nclauses();
  dest()->allocClauses(clauseCount, source()->dbg_clauses());
  for (unsigned i = 0; i < clauseCount; ++i) {
    dest()->setClause(i, copy_clause(source()->getClause(i)));
  }
}


void L2_file_translator::copy_all_transitions() {
  unsigned variableCount = source()->nvars();
  for (unsigned i = 0; i < variableCount; ++i) {
    const L2rVariable *pL2rVariable = source()->getVar(i);
    // Only bother with mode variables
    if (pL2rVariable->kind() == vk_mode) {
      // Get a non-const destination variable
      const L2rVariable *var2_k = get_dest_var(pL2rVariable);
      L2rVariable *var2 = dest()->getVar_friend(var2_k->id());

      // Get the variable's mode
      const L2rMode *pL2rMode = pL2rVariable->mode();
      L2rMode *m2 = var2->mode_friend();

      L2_assert(m2->domain_size() == 1,
		L2_optimizer_error,
		("transitions for " + MBA_string(pL2rVariable->id()) +
		 " already copied"));
      // Copy nominal transitions
      {
	for (L2rMode::iterator it = pL2rMode->begin_nominal();
	     it != pL2rMode->end_nominal(); ++it) {
	  const L2rTransition *pL2rTransition = *it;
	  copy_transition(pL2rTransition);
	}
      }
      // Copy failure transitions
      {
	for (L2rMode::iterator it = pL2rMode->begin_failure();
	     it != pL2rMode->end_failure(); ++it) {
	  const L2rTransition *pL2rTransition = *it;
	  copy_transition(pL2rTransition);
	}
      }
    }
  }
}
