/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file.cpp,v 1.33 2004/01/22 01:21:40 lbrown Exp $ */

#include <readers/L2_file.h>
#include <readers/transition.h>

// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) do { if (isVerbose()) { expr; } } while(0)
#else
#  define verbose(expr)
#endif


// Delete each member of the array and then delete the array

template <class T>
void free_array(T **array, unsigned size) {
  for (unsigned i = 0; i < size; ++i) {
    if (array[i]) delete array[i];
  }
  L2_free_array_no_dtor(array, size);
}


/***************************************************************************
      Constructor/destructor.
 ***************************************************************************/

// Clear all the data structures ; we disallow setting them twice. Set all the
// counts to zero for the benefit of the destructor (in case of a parse error)

L2_file::L2_file() :
  nenums_(0), enums_(NULL),
  nvars_(0),  vars_(NULL),
  nprops_(0), props_(NULL),
  nclauses_(0), clauses_(NULL), 
  isVerbose_(false) { }


L2_file::~L2_file() {
    free_array(enums_,   nenums());
    free_array(vars_,    nvars());
    free_array(props_,   nprops());
    free_array(clauses_, nclauses());
}

/***************************************************************************
 * Allocating the arrays. Only allowed to do it once.
 ***************************************************************************/

// The do-while(0) is the only portable way to block
#define alloc_and_zero(array_name, array_type, array_size) \
    do { \
        n##array_name = array_size; \
        array_name = L2_alloc_array_no_ctor(array_type,array_size); \
        memset(array_name, 0, array_size * sizeof(array_type)); \
    } while(0)


void L2_file::allocEnums(unsigned n, bool dbg) {
  L2_assert(!enums_, L2_reader_error, ("enums already allocated"));
  alloc_and_zero(enums_, L2rEnumeration*, n);
  dbg_enums_ = dbg;
}


void L2_file::allocVars(unsigned n, bool dbg) {
  L2_assert(!vars_, L2_reader_error, ("vars already allocated"));
  alloc_and_zero(vars_, L2rVariable*, n);
  dbg_vars_ = dbg;
}


void L2_file::allocProps(unsigned nprops) {
  L2_assert(!props_, L2_reader_error, ("props already allocated"));
  alloc_and_zero(props_, L2rProposition*, nprops);
}


void L2_file::allocClauses(unsigned n, bool dbg) {
  L2_assert(!clauses_, L2_reader_error, ("clauses already allocated"));
  alloc_and_zero(clauses_, L2rClause*, n);
  dbg_clauses_ = dbg;
}

#undef alloc_and_zero


/***************************************************************************
      Setting the members.
 ***************************************************************************/

// Here because L2rPropsition::setID() requires declaration to be included

void L2_file::setProp  (unsigned i, L2rProposition *p) {
  getProp(i);
  p->setID(i);
  props_[i] = p;
}


// Here because L2rClause::putInBackground() requires declaration to be included

void L2_file::addBackground(const L2rClause* c) {
  getClause(c->id()); clauses_[c->id()]->putInBackground();
}

void L2_file::addTransition(const L2rTransition* x) {
  const L2rVariable *modevar_k = x->mode();
  L2rVariable *modevar = getVar_friend(modevar_k->id());
  L2rMode *modes = modevar->mode_friend();
  modes->add_transition(x);
}
