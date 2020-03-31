/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Transition system representation, NASA Ames Research Center
// See Copyright.txt
// $Id: transitioned.cpp,v 1.119 2004/02/10 21:47:28 lbrown Exp $

#include <livingstone/L2_assert.h>
#include <transition/transitioned.h>

/***************************************************************************
  Constructors
 ***************************************************************************/

Transitioned::Transitioned(T_system& t_system,
			   unsigned nvalues,
			   unsigned id,
			   unsigned transitionCount,
			   unsigned timeStep) :
  Dependent(t_system, nvalues, timeStep, id),
  now_has_failure_from_horizon(false),
  t_system(t_system),
  ntransitions(transitionCount)
{
  // The nominal transition costs nothing
  transition_costs.push_front(0);

  // Create a list with nx zero costs. As we call add_cost, we'll be fixing the
  // costs. This is somewhat of a hack...
  Slist<Assignment::weight_t> zeros;
  for (unsigned i = 0; i < transitionCount; ++i) { zeros.push_front(0); }

  // At the beginning, we have a singularity; the horizon and zenith are the
  // same.
  zenith_transition = horizon_transition =
    t_system.create_mode_transition(*this, transitionCount, zeros,
				    T_system::BEFORE_HORIZON);
}


/***************************************************************************
        Destructor
 ***************************************************************************/

Transitioned::~Transitioned() {
  // Deallocate indexed_transition_table
  {
    for (Table_type::iterator it1 = indexed_transition_table.begin();
	 it1 != indexed_transition_table.end() ; ++it1) {
      Slist<Proto_transition*>& list = (*it1).second;
      for (Slist<Proto_transition*>::iterator it2 = list.begin();
	   it2 != list.end(); ++it2) {
	const Proto_transition* const pProtoTransition = *it2;
	// Older compilers don't know that delete is const
	delete const_cast<Proto_transition*>(pProtoTransition);
      }
    }
    indexed_transition_table.erase();
  }

  // Deallocate unindexed_prototypes
  {
    for (Slist<Proto_transition*>::iterator it = unindexed_prototypes.begin();
	it != unindexed_prototypes.end(); ++it) {
      const Proto_transition* const pProtoTransition = *it;
      // Older compilers don't know that delete is const
      delete const_cast<Proto_transition*>(pProtoTransition);
    }
    unindexed_prototypes.erase();
  }

  // Deallocate autonomous_transitions
  {
    for (Slist<Proto_transition*>::iterator it = autonomous_transitions.begin();
	it != autonomous_transitions.end(); ++it) {
      const Proto_transition* const pProtoTransition = *it;
      // Older compilers don't know that delete is const
      delete const_cast<Proto_transition*>(pProtoTransition);
    }
    autonomous_transitions.erase();
  }

  transition_clauses.erase();
  transition_costs.erase();

  // Don't destroy the zenith or horizon variables; the C_db will.
}


void Transitioned::create_transition_variable(unsigned timestep) {
  // Copy this variable, as a Dependent.  That creates the mode variable.
  t_system.copy_in_past(*this);

  // Create the modeTransition variable that creates a constraint from the
  // previous mode variable to us (the NOW mode).  Keep the linked list
  // of modeTransition linked correctly.

  // Transitions are irrelevant to recovery
  if (!t_system.isPlanning()) {
    Assumption* const setting = get_zenith(); // it's falling to the horizon...
    zenith_transition =
      t_system.create_mode_transition(*this,
				       ntransitions,
				       transition_costs,
				       timestep);
    zenith_transition->insert_previous(setting);
  }

  // Move the support. That is, this Variable is constrained to have a
  // particular value by the constraints of the system.  Now as we move
  // this Variable one step into the future, those old constraints have
  // to be moved onto the object that will represent the previous value
  // of the Variable.
  t_system.splice(*this, *get_previous_dependent());

  // Now we can clear the Transitions list
  transition_clauses.erase();
  now_has_failure_from_horizon = false;
}


const Transition* Transitioned::idle(unsigned timestep) {
  // We are transitioning this variable because it supports the cmded
  // transition, not because we are commanding it.  Just add the idle and
  // failure transitions.

  // Now that we have an object that has taken the place of this variable
  // at the previous time step, we can represent how this variable evolved
  // from the previuos time step to the next time step.  We do this by
  // introducing a set of constraints between them.

  create_transition_variable(timestep);
  add_idle_transitions();
  instantiate_autonomous_clauses();
  return get_zenith();
}


// Go through the transition_clauses and remove them all from the TMS.

void Transitioned::remove_transition_clauses(LTMS *tms) {
  for (Clause::ListOfClause::iterator it = transition_clauses.begin();
       it != transition_clauses.end(); ++it) {
    Clause* const pClause = *it;
    tms->remove(*pClause);
  }
}


#ifdef ENABLE_CHECKPOINT
// Go through the all_transition_clauses and remove them all from the TMS.

void Transitioned::remove_all_transition_clauses(LTMS *tms) {
  for (Clause::ListOfClause::iterator it = all_transition_clauses.begin();
       it != all_transition_clauses.end(); ++it) {
    Clause* const pClause = *it;
    tms->remove(*pClause);
  }
}
#endif


void Transitioned::set_mode(LTMS *tms, unsigned value) {
  remove_transition_clauses(tms);
  // Create the clause mode=value ; not volatile, in theory.
  Proposition::ListOfProposition posPropositions;
  // This will always be empty, so allocate it statically
  static Proposition::ListOfProposition emptyPropositionList;
  Proposition* const m_eq_v = get_prop(value);
  posPropositions.push_front(m_eq_v);
  Clause* const pClause = tms->create_clause(NULL, posPropositions,
					     emptyPropositionList,
					     false, true);
  // add the clause to the transition_clauses so we know to move it
  transition_clauses.push_front(pClause);
}


const Transition* Transitioned::transition(const Assignment& commandAssignment,
					   unsigned timestep) {

  /// We are transitioning this Variable by a Command (as opposed to
  /// using the idle Transition to support some other Variable's Command)
  create_transition_variable(timestep);

  // Now that we have an object that has taken the place of this Variable
  // at the previous time step, we can represent how this Variable evolved
  // from the previous time step to the next time step.  We do this by
  // introducing a set of constraints between them.
  instantiate_clauses(commandAssignment, T_system::NOW);

  return get_zenith();
}


// Instantiate clauses that constrain the Transitioned between time steps

void Transitioned::instantiate_clauses(const Assignment& commandAssignment,
				       unsigned toTimeStep) {

  // Create the Clauses that control the assignment if the expected happens.
  instantiate_indexed_clauses(commandAssignment,
			      this->get_time_step(),
			      toTimeStep);

  // Create the Clauses that we will need if the device unexpectly idles
  // because the Command did not get through due to failure, or because the
  // user gives a Command that isn't specified to move the device out of
  // its current state.
  instantiate_unindexed_clauses(this->get_time_step(), toTimeStep);

  /// Always add in the non-commanded Transitions (e.g., failures), if any
  instantiate_autonomous_clauses(toTimeStep);
}


// Used in restoring checkpoints. Within-time-step constraint Clauses and
// autonomous Transition Clauses are already there.

void Transitioned::add_commanded_transitions(unsigned commandVariableID,
					     unsigned commandValueIndex,
					     unsigned fromTimeStep,
					     unsigned toTimeStep) {
  // Look up the Assignment for (commandVariableID, commandValueIndex)
  Assignment* const pAssignment =
    t_system.get_command(commandVariableID)->get_assignment(commandValueIndex);
  assert(pAssignment != NULL);
  instantiate_indexed_clauses(*pAssignment, fromTimeStep, toTimeStep);
  instantiate_unindexed_clauses(fromTimeStep, toTimeStep);
  // The autonomous Clauses were never removed
}


// This is an autonomous Transition

// If there are autonomous Transitions which has a current value (e.g., can
// occur from some start states but not others) we should be adding a
// Proposition representing the current_value as a guard clause for the
// Transition.  Currently we assume all autonomous Transitions are failures
// that can occur from any state, and thus have no guard condition.

void Transitioned::add_failure_prototype(unsigned transition_value,
					 // from is implicitly ANY_MODE
					 unsigned int to_value) {
  autonomous_transitions.push_front(new Proto_transition(transition_value,
							 *this, to_value));
}


void
Transitioned::add_commanded_prototype(unsigned transition_value,
				      int from_value,
				      unsigned int to_value,
				      const Slist<T_system::Proto_assignment>&
				      pos,
				      const Slist<T_system::Proto_assignment>&
				      neg) {

  // This first part is analogous to the above.
  Proto_transition* const ptrans =
    new Proto_transition(transition_value, *this, to_value, pos, neg,
			 from_value);

  const Assignment* const cmd_assignment = ptrans->get_command_assignment();

  // IMPORTANT NOTE:
  //
  // If the Transition has no "command in" it used to be considered it a
  // frame axiom. These would be inserted into the model to specify what
  // happens if none of the Transitions that mention a Command are enabled
  // (i.e., none of the guard conditions are satisfied).  Calling these
  // frame axioms was relevant when the model was compiled into prime
  // implicate form.
  //
  // If we haven't compiled the transition model into prime implicate form,
  // not every Transition will explicitly mention a Command.  A Transition
  // might mention the output of a bus, which transmits the Command value.
  // So, the Transitions that do not contain Command Assignments are not
  // frame axioms per se.

  if (cmd_assignment == NULL) {
    // assert(current_value == next_value);
#ifdef TRANSITIONED_DEBUG
    _STD_ cerr
      << "   Adding as a frame axiom because there is no command assignment"
      << _STD_ endl;
#endif
    unindexed_prototypes.push_front(ptrans);
  }

  // If the Transition is an idle Transition, we consider it a frame axiom
  // for now, and it will be added on every
  //
  // We should be checking if this has any Command Assignments in it and
  // indexing the Clauses by Command. However, often these are negative, so
  // we'd have to index them by every other Command Assignment.

  else if ( (unsigned)from_value == to_value)  {
#ifdef TRANSITIONED_DEBUG
    _STD_ cerr << "   Adding as a frame axiom because from=to" << _STD_ endl;
#endif
    unindexed_prototypes.push_front(ptrans);
  } else {
    add_transition_prototype_to_table(*cmd_assignment, ptrans);
  }
}


void 
Transitioned::add_transition_prototype_to_table(const Assignment&
						commandAssignment,
						Transitioned::Proto_transition*
						pProto_transition) {
#ifdef TRANSITIONED_DEBUG
  _STD_ cerr << "   Adding trans for " << commandAssignment << " to table"
	    << _STD_ endl << _STD_ endl;
#endif

  for (Table_type::iterator it = indexed_transition_table.begin();
       it != indexed_transition_table.end(); ++it) {
    Entry_type& thePair = *it;
    if (thePair.first == &commandAssignment) {
      thePair.second.push_front(pProto_transition);
      return;
    }
  }
  Transitioned::Entry_type new_entry;
  new_entry.first = &commandAssignment;
  new_entry.second.push_front(pProto_transition);
  indexed_transition_table.push_front(new_entry);
}


Clause *Transitioned :: set_initial_mode(unsigned stateVariableValueIndex,
					 int modeValueIndex,
					 Theory::time_step_t horizon,
					 bool in_theory) {
  // The Clause is
  // !state=si !x=nominal mode=value

  Proposition::ListOfProposition positive, negative;
  // !state=si
  negative.push_front(
    t_system.get_state_variable()->get_prop(stateVariableValueIndex)
  );

  // Transitions are irrelevant to recovery
  if (!t_system.isPlanning()) {
    // !modeTransition=nominal
    negative.push_front(get_horizon()->get_prop(0));
  }

  // find the oldest mode on or after the horizon
  Variable *v = this;
  Variable *pPrevious = v->get_previous();
  while(pPrevious && pPrevious->get_time_step() >= horizon) {
    v = pPrevious;
    pPrevious = v->get_previous();
  }
  // Now pPrevious is before the horizon (or null); v is on or after it

  // Set the value to what was passed in; or, if we passed in a negative,
  // discover the value
  if (modeValueIndex < 0) {
    Proposition* const propval = v->get_value();
    if (!propval) { // The mode value could not be discovered; return failure
      return NULL;
    } else {
      positive.push_front(propval);
    }
  } else {
    positive.push_front(v->get_prop(modeValueIndex));
  }

  // Create the Clause. It is not volatile because it is not an assumption
  Clause* const pClause =
    t_system.create_clause(positive, negative, false, in_theory);
  add_transition_clause(pClause);

  // Add the possibility of failure if we haven't already done so
  // A failure Clause does not depend on the State Variable and is of the form
  // !-#modeTransition=t horizon#mode=m
  // Failure is irrelevant to recovery
  if (!t_system.isPlanning() && !now_has_failure_from_horizon) {
    instantiate_autonomous_clauses(horizon);
  }
  return pClause;
}


void
Transitioned::instantiate_indexed_clauses(const Assignment& commandAssignment,
					  unsigned fromTimeStep,
					  unsigned toTimeStep) {
  // Map the Command Assignment onto the Proto_transitions for it
  Slist<Proto_transition*>* cmd_clauses =
    find_transition_prototypes(commandAssignment);
  if (cmd_clauses != NULL) {
    for (Slist<Proto_transition*>::iterator it  = cmd_clauses->begin();
	 it != cmd_clauses->end(); ++it) {
      Proto_transition* const pProtoTransition = *it;
      Clause* const pClause =
	pProtoTransition->instantiate(t_system, fromTimeStep, toTimeStep);
      add_transition_clause(pClause);
    }
  }
}


void Transitioned::instantiate_autonomous_clauses(unsigned timestep){
  for (Slist<Proto_transition*>::iterator it = autonomous_transitions.begin();
       it != autonomous_transitions.end(); ++it) {
    Proto_transition* const pProtoTransition = *it;
    Clause* const pClause =
      pProtoTransition->instantiate(t_system, this->get_time_step(), timestep);
    add_transition_clause(pClause);
  }

  // If we added failure Transitions from the pre-horizon Transitions to the
  // NOW variable, note the fact so we don't add them twice. The flag is
  // cleared
  if (get_previous() == NULL) {
    now_has_failure_from_horizon = true;
  }
}


void Transitioned::instantiate_unindexed_clauses(unsigned fromTimeStep,
						 unsigned toTimeStep){
  for (Slist<Proto_transition*>::iterator it = unindexed_prototypes.begin();
       it != unindexed_prototypes.end(); ++it) {
    Proto_transition* const pProtoTransition = *it;
    Clause* const pClause =
      pProtoTransition->instantiate(t_system, fromTimeStep, toTimeStep);
    add_transition_clause(pClause);
  }
}


/**
 * We want to introduce a Clause of the form
 * var[t]=value & zenith=nominal -> var[t+1]=value
 * for every value of the Variable.
 */

void Transitioned::add_idle_transitions() {
  // Get the previous mode. When this method is called, there will be one.
  const Variable* const pPreviousTransitioned = get_previous();
  assert(pPreviousTransitioned != NULL);

  // The Proposition that modeTransition=nominal for the most recent Transition
  Proposition* const pTransitionProposition = get_zenith()->get_prop(0);

  // The negative Propositions constitute the antecedant.
  Proposition::ListOfProposition negatives;
  // All the idle Transitions have "modeTransition=nominal" as an antecedant.
  negatives.push_front(pTransitionProposition);

  // For each possible mode (e.g., on, off, broken, unknown)
  for (unsigned int index = 0; index < get_nvalues(); ++index) {
    // Consequent: var[t+1]=value
    Proposition* const pNowProposition = this->get_prop(index);
    // Antecedent: var[t]=value
    Proposition* const pPreviousProposition =
      pPreviousTransitioned->get_prop(index);
    assert(pPreviousProposition != NULL);

    // The positive Proposition (there will be only one) is the consequent.
    Proposition::ListOfProposition positives;

    // Consequent
    positives.push_front(pNowProposition);
    // Second Proposition of antecedant
    negatives.push_front(pPreviousProposition);

    // Create Clause: var[t]=value & zenith=nominal -> var[t+1]=value
    // In CNF, that's ~(var[t]=value) \/ ~(zenith=nominal) \/ (var[t+1]=value)
    Clause* const pClause =
      t_system.create_clause(positives, negatives, false, true);

    // Pop the previous value out of the antecedant, but leave the Proposition
    // representing "modeTransition=nominal" in there.
    negatives.pop_front();
    
    // Store away the Clauses that are constraining the now Variable based on
    // the past Variable, for the next progress.
    add_transition_clause(pClause);
  }
}


/**
 * This is like add_idle_transitions(void), except instead of assuming that
 * the constraint is between previous and NOW, specify that it is between
 * fromTimeStep and its successor. It is used in checkpointing.
 */

void Transitioned::add_idle_transitions(unsigned fromTimeStep,
					const Transition& transition) {
  // component.mode[t]=value[i] &
  // component.modeTransition[t]=nominal =>
  // component.mode[t+1]=value[i]
  // Get the "from" mode for the specified time step.
  const Variable *pFromMode = this;
  for ( ; pFromMode != 0; pFromMode = pFromMode->get_previous()) {
    if (pFromMode->get_time_step() == fromTimeStep) { break; }
  }
  assert(pFromMode != NULL);

  // Get the "to" mode; this is the mode on the successor time step
  Variable *pToMode = pFromMode->get_next();
  assert(pToMode != NULL);

  // The Proposition that modeTransition=nominal for the most recent Transition
  Proposition* const pTransitionProposition = transition.get_prop(0);

  // The negative Propositions constitute the antecedant.
  Proposition::ListOfProposition negatives;
  // All the idle Transitions have "modeTransition=nominal" as an antecedant.
  negatives.push_front(pTransitionProposition);

  // For each possible mode (e.g., on, off, broken, unknown)
  for (unsigned int index = 0; index < get_nvalues(); ++index) {
    // Consequent: var[t+1]=value
    Proposition* const pNowProposition = pToMode->get_prop(index);
    // Antecedent: var[t]=value
    Proposition* const pPreviousProposition = pFromMode->get_prop(index);
    assert(pPreviousProposition != NULL);

    // The positive Proposition (there will be only one) is the consequent.
    Proposition::ListOfProposition positives;

    // Consequent
    positives.push_front(pNowProposition);
    // Second Proposition of antecedant
    negatives.push_front(pPreviousProposition);

    // Create Clause: var[t]=value & zenith=nominal -> var[t+1]=value
    // In CNF, that's ~(var[t]=value) \/ ~(zenith=nominal) \/ (var[t+1]=value)
    Clause* const pClause =
      t_system.create_clause(positives, negatives, false, true);

    // Pop the previous value out of the antecedant, but leave the Proposition
    // representing "modeTransition=nominal" in there
    negatives.pop_front();
    
    // Store away the Clauses that are constraining the now Variable based on
    // the past Variable, for the next progress.
    add_transition_clause(pClause);
  }
}


// ******************************************************
//
//  Proto_transitions
//
// ******************************************************

// Moving this to transitioned.h for inlining harmed performance

Transitioned::Proto_transition::Proto_transition(unsigned transitionValueIndex,
    Transitioned &transitioned,
    unsigned toModeValueIndex,
    const Slist<T_system::Proto_assignment>& pos,
    const Slist<T_system::Proto_assignment>& neg,
    int fromModeIndex) : 
  positives(pos), negatives(neg),
  consequent(&transitioned, toModeValueIndex),
  command_assignment(findCommandAssignment()), mode_var(transitioned),
  transition_value(transitionValueIndex) {
  // Create "from" Proto_assignment. If the Transition is from all
  // Assignments (i.e., Transitioned::ANY_MODE or "*"), there is no
  // current value. The literal is !t#mode=m
  if (fromModeIndex >= 0) {
    negatives.push_front(T_system::Proto_assignment(&mode_var, fromModeIndex));
  }
}


Assignment* Transitioned::Proto_transition::findCommandAssignment() {
  {
    for (iterator it = begin_positive(); it != end_positive(); ++it) {
      T_system::Proto_assignment& protoAssignment = *it;
      Variable* const pVariable = protoAssignment.first;
      if (pVariable->is_command()) {
	Command* const pCommand = static_cast<Command *>(pVariable);
	return pCommand->get_assignment(protoAssignment.second);
      }
    }
  }
  {
    for (iterator it = begin_negative(); it != end_negative(); ++it) {
      T_system::Proto_assignment& protoAssignment = *it;
      Variable* const pVariable = protoAssignment.first;
      if (pVariable->is_command()) {
	Command* const pCommand = static_cast<Command *>(pVariable);
	return pCommand->get_assignment(protoAssignment.second);
      }
    }
  }
  return NULL;
}


Slist<Transitioned::Proto_transition*> * 
Transitioned::find_transition_prototypes(const Assignment& commandAssignment)
{
  for (Table_type::iterator it = indexed_transition_table.begin();
       it != indexed_transition_table.end(); ++it) {
    Entry_type& thePair = *it;
    if (thePair.first == &commandAssignment) {
      // Found an entry whose Assignment is the one supplied; return its list
      return &(thePair.second);
    }
  }
  // Not found; return a null pointer to list
  return NULL;
}


/**
  * Return the Proposition corresponding to the Proto_assignment at a given
  * time step.  Returns null if we can't find a Variable at that time step.
  */

Proposition *
Transitioned::Proto_transition::prop_closest_before(const 
						    T_system::Proto_assignment&
						    assign,
						    unsigned timeStep) {
  const Variable *pVariable       = assign.first;
  const unsigned propositionIndex = assign.second;
  while (pVariable != 0) {
    if (pVariable->get_time_step() < timeStep) {
      return pVariable->get_prop(propositionIndex);
    } else {
      pVariable = pVariable->get_previous();
    }
  }
  // If here, no variable in the list satisfied the time constraint
  // This happens i.e. when searching for modeTransitions, since NOW
  // is the most-previous in the list, but has time > any other time.
  return NULL;
}


/**
  * Use the variable either at the given timestep, or the first one after.
  * Always returns non-null.
  */
Proposition *
Transitioned::Proto_transition::prop_closest_after(const 
						   T_system::Proto_assignment&
						   assign,
						   unsigned time) {
  const Variable *cur             = assign.first;
  const unsigned propositionIndex = assign.second;
  const Variable *next = cur; // in case cur is acceptable already
  while(cur && cur->get_time_step() >= time) {
    next = cur;
    cur = cur->get_previous();
  }
  return next->get_prop(propositionIndex);
}


/**
  * Collect all the antecedants, the Propositions on the LHS of an implication.
  * Antecedents might be "mode(timeStep=2)=off" and "cmdIn(timeStep=2)=turnOn".
  * The consequent (RHS of the implication) might be "mode(timeStep=3)=on".
  */


void
Transitioned::Proto_transition::set_antecedants(Proposition::ListOfProposition&
						positive,
						Proposition::ListOfProposition&
						negative,
						unsigned t) {
  // Collect all the ones that deal with the guard clause.
  {
    for (iterator it = begin_positive(); it != end_positive(); ++it)  {
      const T_system::Proto_assignment& protoAssignment = *it;
      Proposition* const pProposition =
	prop_closest_before(protoAssignment, t);
      assert(pProposition != NULL);
      positive.push_front(pProposition);
    }
  }
  {
    for (iterator it = begin_negative(); it != end_negative(); ++it)  {
      const T_system::Proto_assignment& protoAssignment = *it;
      Proposition* const pProposition =
	prop_closest_before(protoAssignment, t);
      assert(pProposition != NULL);
      negative.push_front(pProposition);
    }
  }

  // Add the one that deals with the mode transition.
  //   p & q ==> r
  //   ~p | ~q | r
  // This is p, so we add to negatives.

  // Tricky thing here: the horizon is at time BEFORE_HORIZON, which is > any
  // other time (makes truncation easier).  But really it's logically at a
  // time < any other time.  So if we find nothing earlier than time t,
  // we use the horizon.
}


Clause*
Transitioned::Proto_transition::instantiate(T_system& t_system,
					    unsigned fromTimeStep,
					    unsigned toTimeStep) {
  Proposition::ListOfProposition positive;
  Proposition::ListOfProposition negative;
  // Antecedants are stricly before the time step
  set_antecedants(positive, negative, toTimeStep);

  // If constraining a NOW Variable, we're using the zenith (most recent
  // transition).  If constraining an initial mode, use the horizon.

  // Transitions are irrelevant to recovery
  if (!t_system.isPlanning()) {
    const Transition* pTransition;
    if (fromTimeStep == mode_var.get_time_step()) {
      // Either progressing or restoring a checkpoint
      pTransition =
	(toTimeStep == T_system::NOW) ?
	mode_var.get_zenith() :
	mode_var.get_horizon();
    } else {
      // Can only be restoring a checkpoint
      // Get the Transition for fromTimeStep
      for (pTransition = mode_var.get_zenith();
	   pTransition->get_time_step() != fromTimeStep;
	   pTransition = (const Transition*) pTransition->get_previous()) { }
      assert(pTransition != NULL);
    }
    negative.push_front(pTransition->get_prop(transition_value));
  }

  // Consequent is on or after the timestep
  positive.push_front(prop_closest_after(consequent, toTimeStep));

  return t_system.create_clause(positive, negative, false, true);
}


void Transitioned::add_transition_cost(Assignment::weight_t cost) {
  const unsigned idx = transition_costs.size();
  L2_assert(idx < ntransitions,
	    L2_fatal_error,
            ("adding too many transition costs to v" + MBA_string(get_id())));
  transition_costs.push_back(cost);
  horizon_transition->set_weight(*horizon_transition->get_assignment(idx),
				 cost);
}


/***************************************************************************
  Remove dead wood in truncation.
 ***************************************************************************/

void Transitioned::remove_old_modeTransition(Theory::time_step_t horizon) {
  // If the zenith is out of date, all modeTransitions are being removed,
  // so set it back to the initial-time-step one.  If only some mode
  // Transitions are being removed, they'll fix up the list themselves.

  if (get_zenith()->get_time_step() < horizon) {
    zenith_transition = get_horizon();
  }
}


/**
 * Remove from our local lists Clauses that are about to be destroyed.
 * The is_destroying() flag is set as part of the Variable destructor.  Then
 * is_destroying() Clauses are removed by LTMS::finish_destroying() later on.
 */
void Transitioned::remove_destroyed_clauses() {
  {
    for (Clause::ListOfClause::iterator it = transition_clauses.begin(); 
	 it != transition_clauses.end(); ) {
      const Clause* const pClause = *it;
      if (pClause->is_destroying()) {
	transition_clauses.erase(it);
      } else {
	++it;
      }
    }
  }

#ifdef ENABLE_CHECKPOINT
  {
    for (Clause::ListOfClause::iterator it = all_transition_clauses.begin(); 
	 it != all_transition_clauses.end(); ) {
      const Clause* const pClause = *it;
      if (pClause->is_destroying()) {
	all_transition_clauses.erase(it);
      } else {
	++it;
      }
    }
  }
#endif
}


/***************************************************************************
  Support for using T_system to do recovery planning
 ***************************************************************************/

#ifdef ENABLE_RECOVERY
void Transitioned::planner_transition(int toTimeStep) {
  create_transition_variable(toTimeStep);
  instantiate_all_indexed_clauses();
  instantiate_unindexed_clauses(this->get_time_step());
  instantiate_autonomous_clauses();
}


void Transitioned::instantiate_all_indexed_clauses(unsigned timestep){
  for (Table_type::iterator table_it = indexed_transition_table.begin();
      table_it != indexed_transition_table.end(); ++ table_it) {
    Entry_type& thePair = *table_it;
    Slist<Proto_transition*> cmd_clauses = thePair.second;
    for (Slist<Transitioned::Proto_transition*>::iterator it =
	   cmd_clauses.begin(); it != cmd_clauses.end(); ++it) {
      Proto_transition* const pProtoTransition = *it;
      Clause* const pClause =
	pProtoTransition->instantiate(t_system, this->get_time_step(),
				      timestep);
      assert(pClause != NULL);
      add_transition_clause(pClause);
    }
  }
}
#endif
