/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_t_system.cpp,v 1.48 2004/01/28 23:31:51 lbrown Exp $ */

#include <readers/L2_file.h>
#include <readers/to_t_system.h>
#include <readers/transition.h>
#include <transition/transitioned.h>

// verbose output
// The do-while(0) is the only portable way to block
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) do { if (isVerbose()) { expr; } } while(0)
#else
#  define verbose(expr)
#endif

// record via the listener
// The do-while(0) is the only portable way to block
#ifndef DISABLE_TO_T_SYSTEM_LISTEN
#  include <readers/to_t_system_listener.h>
#  define record(call_with_args) \
   do { \
    Slist<to_t_system_listener*>::iterator listen_it##__LINE__ \
        = listeners_.begin(); \
    while(listen_it##__LINE__ != listeners_.end()) { \
        (*listen_it##__LINE__)->call_with_args;  \
        ++listen_it##__LINE__; \
    } \
   } while(0)
#else
#  define record(call_with_args)
#endif


// Needed to implement created_clause: a listener into the TMS that catches the
// created_clause event and translates it to the user's to_t_system_listener
// call.  We also need to pass call on to the other listener.

#ifndef DISABLE_TO_T_SYSTEM_LISTEN
#include <tms/ptheory_listener.h>
class to_t_system::tms_listener : public Pooled,
		   public virtual Ptheory_listener
{
private:
  // can't be null
  Slist<to_t_system_listener*>& listeners_;
  Ptheory& theory;
  friend class to_t_system;

public:
  tms_listener(Slist<to_t_system_listener*>& l, Ptheory& p)
    : listeners_(l), theory(p) {
    theory.add_listener(this);
  }

  virtual ~tms_listener() {
    theory.remove_listener(this);
  }

  // translate into a call on the user's to_t_system_listener
  virtual void created_clause(Clause& newclause) {
    if (creating_from_variable)
      record(created_clause(var, newclause));
    else
      record(created_clause(cls, newclause));
  }

  // we ignore all these
  virtual void created_proposition(Proposition&) { }
  virtual void destroying_proposition(Proposition&) { }
  virtual void destroying_clause(Clause&) { }
  virtual void destroying_container(Ptheory&) {
    L2_throw(L2_fatal_error,
	     ("Expected theory to outlast the reader!"));
  }

private:
  // We could use a union, but we'd only save all of 4 bytes,
  // and unions are ugly.
  const L2rVariable *var;
  const L2rClause   *cls;
  bool creating_from_variable; // if false, creating from clause
};
#endif


/***************************************************************************
        Constructor
 ***************************************************************************/

to_t_system::to_t_system(const L2_file *f, T_system *tsys)
    : L2_file_writer (f), t_system(tsys) {
}

to_t_system::~to_t_system() {
  record(destroying_container(*this));
}

/***************************************************************************
        The main function
 ***************************************************************************/

// Take info from the l2_file and put it into the T_system

bool to_t_system::write() {
#ifndef DISABLE_TO_T_SYSTEM_LISTEN
  // Set a listener which will allow us to keep track of where clauses come
  // from.  The internal_listen just translates TMS listener calls into
  // to_t_system_listener calls.
  if (!listeners_.empty()) {
    internal_tms_listen =
      new tms_listener(listeners_, *t_system->get_solver());
  } else {
    internal_tms_listen = 0;
  }
#endif

  // The model from the reader
  const L2_file *source = get_source();

  // Some checks that the model has needed elements
  L2_assert(source->nenums() != 0,
	    L2_empty_model_error,
            ("model has no enumerations"));
  L2_assert(source->nvars() != 0,
	    L2_empty_model_error,
            ("model has no variables"));
  L2_assert(source->nclauses() != 0,
	    L2_empty_model_error,
            ("model has no clauses"));

  // Create the 3 tyeps of elements (variables, clauses, transitions)
  {
    for (unsigned i = 0; i < source->nvars(); i++)
      createVariable(source->getVar(i));
  }
  {
    for (unsigned i = 0; i < source->nclauses(); i++)
      createBackground(source->getClause(i));
  }
  {
    for (unsigned i = 0; i < source->nvars(); ++i) {
      const L2rVariable* pL2rVariable = source->getVar(i);
      if (pL2rVariable->kind() == vk_mode) {
	// It is a mode
	createTransitions(source->getVar(i)); // ignored if not a mode
      }
    }
  }

#ifndef DISABLE_TO_T_SYSTEM_LISTEN
  // Undo the work we did at the top of the file.
  if (!listeners_.empty()) {
    delete internal_tms_listen;
  }
#endif

  return true;
}


// Turn an L2rVariable (from the l2_file) into a Variable (in the T_system)

void to_t_system::createVariable(const L2rVariable *pL2rVariable) {
#ifndef DISABLE_TO_T_SYSTEM_LISTEN
  if (internal_tms_listen) {
    internal_tms_listen->creating_from_variable = true;
    internal_tms_listen->var = pL2rVariable;
  }
#endif
  unsigned domainCardinality = pL2rVariable->type()->nmembers();
  unsigned variableID = pL2rVariable->id();
  Variable *pVariable = NULL;
  switch(pL2rVariable->kind()) {
  case vk_commanded:
    {
      Command* pCommand =
	t_system->create_command(domainCardinality, T_system::NOW, variableID);
      // It's OK to assign the present state Command because the present state
      // Command doesn't constrain anything. The previous Command constrains
      // the current state Variables, but the current Commands don't constrain
      // anything and are always noCommand (index 0) -- nothing will ever set
      // them to anything else!
      pCommand->assign(0u);
      pVariable = pCommand;
    }
    break;
  case vk_observed:
    pVariable =
      t_system->create_observable(domainCardinality, T_system::NOW, variableID);
    break;
  case vk_mode:
    pVariable =
      new Transitioned(*t_system, domainCardinality, variableID,
		       pL2rVariable->mode()->domain_size(), T_system::NOW);
    break;
  default:
    pVariable =
      t_system->create_dependent(domainCardinality, T_system::NOW, variableID);
    break;
  }
  t_system->register_new_variable(pVariable);
  verbose(_STD_ cout << "Created variable " << pVariable->get_id() << " `"
	  << pL2rVariable << "'\n");
  record(created_variable(pL2rVariable, *pVariable));
}


// Create the Transition objects that go from one mode to another

void to_t_system::createTransitions(const L2rVariable *pL2rVariable) {
  const L2rMode *pL2rMode = pL2rVariable->mode();
  // Map from L2rVariable (l2_file) to Variable (T_system) and cast to
  // Transitioned*
  Transitioned *pTransitioned =
    static_cast<Transitioned*>(findVar(pL2rVariable));

  // Create the nominal Transition. The index of the nominal value is zero.
  // Note there must be exactly one of them.
  {
    for (L2rMode::iterator it = pL2rMode->begin_nominal();
	 it != pL2rMode->end_nominal(); ++it) {
      const L2rTransition *pL2rTransition = *it;
      createTransition(pTransitioned, pL2rTransition, 0); // 0 is nominal index
    }
  }

  // Create the failure Transitions. Each failure has its own index.
  // The first failure has index 1, not 0 (0 is nominal)
  
  // When doing recovery instead of diagnosis, failures are irrelevant.
  if (!t_system->isPlanning()) {
    unsigned i = 1;
    for (L2rMode::iterator it = pL2rMode->begin_failure();
	 it != pL2rMode->end_failure() ; ++it) {
      const L2rTransition *pL2rTransition = *it;
      createTransition(pTransitioned, pL2rTransition, i++);
    }
    L2_assert(i == pTransitioned->get_ntransitions(),
	      L2_writer_error,
	      ("Mode v" + MBA_string(pTransitioned->get_id()) +
	       " has the wrong number of transitions"));
  }
}


// Create a clause if it is in the background

void to_t_system::createBackground(const L2rClause *pL2rClause) {
  if (pL2rClause->isInBackground()) {
    // background clauses are 'state' constraints by definition
    expandClause(pL2rClause, true);
  }
}

// Handle a Clause with any number of Variable1 == Variable2 Propositions

void to_t_system::expandClauseEquality(const Slist<const L2rPropVarVar*>&
				       equality,
				       T_system::Proto_assignment_list& pos,
				       T_system::Proto_assignment_list& neg,
				       bool state_constr) {
  Slist<Partition*> stack; // stack of Partition to avoid recursion
  stack.push_front(new Partition(pos, neg, equality)); // the initial Partition
  while (!stack.empty()) {
    // Pop the top element from the stack; note that this takes two operations.
    Partition* pPartition = stack.front(); stack.pop_front();
    // Extract the lists of Propositions
    T_system::Proto_assignment_list& positiveSingles =
      pPartition->getPositiveSingles();
    T_system::Proto_assignment_list& negativeSingles =
      pPartition->getNegativeSingles();
    const Slist<const L2rPropVarVar*>& duals = pPartition->getDuals();
    if (duals.empty()) { // the base step; create the Clause
      t_system->create_constraint(positiveSingles, negativeSingles, false,
				  true, state_constr);
    } else { // a recursive step
      // Non-destructively retrieve the first L2rPropVarVar
      const L2rPropVarVar* pL2rPropVarVar = duals.front();
      // Take the "cdr" of the duals list; note that this takes two operations.
      Slist<const L2rPropVarVar*> tail(duals); tail.pop_front();
      // The two Variables must be of the same type
      L2_assert(pL2rPropVarVar->var()->type() ==
		pL2rPropVarVar->otherVar()->type(),
		L2_reader_error,
		("Type mismatch in equality"));
      // Get the Variables as known in the T_system; and the domain
      Variable* pVariable1 = findVar(pL2rPropVarVar->var());
      Variable* pVariable2 = findVar(pL2rPropVarVar->otherVar());
      const L2rEnumeration* pL2rEnumeration = pL2rPropVarVar->var()->type();
      const unsigned domainCardinality = pL2rEnumeration->nmembers();
      if (pL2rPropVarVar->isPositive()) {
	// positive: create clauses (c is the current list in neg/pos)
	//   foreach i in domain[v],
	//      c  !v1=i v2=i               (v1=i implies v2=i)
	//      c  !v2=i v1=i               (v2=i implies v1=i)
	for (unsigned i = 0 ; i < domainCardinality; i++) {
	  unsigned j = domainCardinality - i - 1; // to pass regression tests
	  {
	    // c !v1=i v2=i; v1 == i ==> v2 == i 
	    Partition* p = new Partition(pos, neg, tail); // delete when popped
	    p->getNegativeSingles().push(
	      T_system::Proto_assignment(pVariable1, j)
	    );
	    p->getPositiveSingles().push(
	      T_system::Proto_assignment(pVariable2, j)
	    );
	    stack.push_front(p);
	  }
	  {
	    // c  !v2=i v1=i; v2 == i ==> v1 == i
	    Partition* p = new Partition(pos, neg, tail); // delete when popped
	    p->getNegativeSingles().push(
	      T_system::Proto_assignment(pVariable2, j)
	    );
	    p->getPositiveSingles().push(
	      T_system::Proto_assignment(pVariable1, j)
	    );
	    stack.push_front(p);
	  }
	}
      } else {
	// negative:
	//   foreach i in domain[v],
	//      c  !v1=i !v2=i              (v1=i implies v2!=i and v-v)
	for (unsigned i = 0; i < domainCardinality; i++) {
	  unsigned j = domainCardinality - i - 1; // to pass regression tests
	  // c  !v1=i !v2=i; v1 == i ==> v2 != i
	  Partition* p = new Partition(pos, neg, tail); // delete when popped 
	  p->getNegativeSingles().push(T_system::Proto_assignment(pVariable1,
								  j));
	  p->getNegativeSingles().push(T_system::Proto_assignment(pVariable2,
								  j));
	  stack.push_front(p);
	} // end for
      } // end else
    } // end else
    delete pPartition; // done with this "stack frame"
  }
}


// Create a Clause from an L2rClause

void to_t_system::expandClause(const L2rClause *pL2rClause,
			       bool state_constr) {
#ifndef DISABLE_TO_T_SYSTEM_LISTEN
  if (internal_tms_listen) {
    internal_tms_listen->creating_from_variable = false;
    internal_tms_listen->cls = pL2rClause;
  }
#endif

  T_system::Proto_assignment_list pos(pL2rClause->nprops());
  T_system::Proto_assignment_list neg(pL2rClause->nprops());

  // A Variable1 == Variable2 L2rProposition in the L2rClause, if there is any
  Slist<const L2rPropVarVar*> equality;

  for (unsigned i = 0; i < pL2rClause->nprops(); i++) {
    const L2rProposition *pL2rProposition = pL2rClause->prop(i);
    if (pL2rProposition->isEquality()) {
      equality.push_front(static_cast<const L2rPropVarVar*>(pL2rProposition));
    } else {
      // Variable == Value
      // Downcast
      const L2rPropVarValue *pL2rPropVarValue =
	static_cast<const L2rPropVarValue*>(pL2rProposition);
      // Find the Proposition's Variable and value index
      Variable *pVariable = findVar(pL2rPropVarValue->var());
      int valueIndex = pL2rPropVarValue->value();
      if (pL2rPropVarValue->isPositive()) {
	pos.push(T_system::Proto_assignment(pVariable, valueIndex));
      } else {
	neg.push(T_system::Proto_assignment(pVariable, valueIndex));
      }
    }
  }

  expandClauseEquality(equality, pos, neg, state_constr);
}


// Create a Transitioned from an L2rTransition

void to_t_system::createTransition(Transitioned *pTransitioned,
				   const L2rTransition *pL2rTransition,
				   unsigned transition_value) {
  unsigned rank = pL2rTransition->rank();
  if (rank != 0) { // Non-default cost/rank
    pTransitioned->add_transition_cost(static_cast<Assignment::weight_t>(rank));
  }

  if (pL2rTransition->nclauses() == 0) {
    if (pL2rTransition->isFailure()) {
      // Add an autonomous transition; no from
      pTransitioned->add_failure_prototype(transition_value,
					   pL2rTransition->to());
    } else {
      // Add an idle transition; from == to
      L2_assert(pL2rTransition->from() == pL2rTransition->to(),
		L2_reader_error,
		("non-idle non-failure uncommanded transition"));
      pTransitioned->add_idle_prototype(pL2rTransition->to());
    }
  } else {
    // Add a nominal transition
    for (unsigned i = 0; i < pL2rTransition->nclauses(); ++i) {
      const L2rClause *pL2rClause = pL2rTransition->clause(i);

      // Collect the positive and negative assignments
      Slist<T_system::Proto_assignment> pos;
      Slist<T_system::Proto_assignment> neg;
      for (unsigned j = 0; j < pL2rClause->nprops(); ++j) {
	const L2rProposition *pL2rProposition = pL2rClause->prop(j);
	// A Transition can not assert Variable1 == Variable2
	L2_assert(!pL2rProposition->isEquality(),
		  L2_reader_error,
		  ("found equality as guard condition"));
	// Cast as L2rPropVarValue (Variable == Value)
	const L2rPropVarValue *pL2rPropVarValue =
	  static_cast<const L2rPropVarValue*>(pL2rProposition);
	Variable *pVariable = findVar(pL2rPropVarValue->var());
	int valueIndex = pL2rPropVarValue->value();
	T_system::Proto_assignment pa(pVariable, valueIndex);
	if (pL2rPropVarValue->isPositive()) {
	  pos.push_front(pa);
	} else {
	  neg.push_front(pa);
	}
      }

      pTransitioned->add_commanded_prototype(transition_value,
					     pL2rTransition->from(),
					     pL2rTransition->to(),
					     pos, neg);
    }
  }
  verbose(_STD_ cout << "Created transition " << pL2rTransition << _STD_ endl);
}


/***************************************************************************
  Creating the initial state.  Quite likely we don't actually want this
  encoded in the model, but in some other file somewhere.  But it's here
  for now.
 ***************************************************************************/

void to_t_system::createInitialState() {
  State_variable* pStateVariable =
    t_system->create_state_var(0,  // horizon time step
			       1); // state count
  for (unsigned i = 0; i < get_source()->nvars(); i++) {
    const L2rVariable *pL2rVariable = get_source()->getVar(i);
    // if not set, ignore
    if (pL2rVariable->initial() != L2rVariable::NO_INITIAL_VALUE) {
      // Only mode or Observable Variables can have initial values
      L2_assert((pL2rVariable->kind() == vk_mode) ||
		(pL2rVariable->kind() == vk_observed),
		L2_reader_error,
		("v" + MBA_string(pL2rVariable->id()) +
		 " is neither mode nor observation yet has an initial value"));
      if (pL2rVariable->kind() == vk_mode) {
	Transitioned *pTransitioned =
	  static_cast<Transitioned*>(findVar(pL2rVariable));
	pTransitioned->set_initial_mode(0,                       // state=s0
					pL2rVariable->initial(),
					T_system::NOW,
					true);                   // inTheory
      } else if (pL2rVariable->kind() == vk_observed) {
	Observable* pObservable =
	  static_cast<Observable*>(findVar(pL2rVariable));
	pObservable->observe(pL2rVariable->initial());
      } else {
	// Shouldn't get here
      }
    }
  }

  pStateVariable->assign(unsigned(0)); // 0 is ambiguous unsigned or pointer

  // no need to progress; this does all we need
  t_system->next_timestep();
  t_system->propagate();
}


/***************************************************************************
  Fast lookup from L2_file data types to T_system types.
  This is likely a duplicate of some information we'd keep anyway,
  but this way, the to_t_system class is independent of who's using
  it (flight, GPU, etc).
 ***************************************************************************/
Variable *to_t_system::findVar(const L2rVariable *var) const {
    return t_system->get_present_variable(var->id());
}
