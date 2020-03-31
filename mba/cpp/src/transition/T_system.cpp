/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Transition system representation, NASA Ames Research Center
// See Copyright.txt
// $Id: T_system.cpp,v 1.258 2004/02/10 21:39:16 lbrown Exp $
//
#include <conflict_db/candidate.h>
#include <debuggers/trajectory.h>
#include <transition/T_system.h>
#include <stdlib.h>
#include <livingstone/L2_assert.h>
#include <transition/transitioned.h>

// Allow easily disabling the listeners for flight
#ifndef DISABLE_T_SYS_LISTEN
#  include <transition/T_system_listener.h>
#  define t_sys_record(call_with_args) \
   do { \
    Slist<T_system_listener*>::iterator listen_it##__LINE__ \
        = listeners_.begin(); \
    while(listen_it##__LINE__ != listeners_.end()) { \
        (*listen_it##__LINE__)->call_with_args;  \
        ++listen_it##__LINE__; \
    } \
   } while(0)
#else
#  define t_sys_record(call_with_args)
#endif

// If we disabled full-progress, the default style is min-progress; otherwise,
// the default is full; or we don't have min anyway

#ifndef DISABLE_FULL_PROGRESS
const T_system::Progress_style T_system::DEFAULT_PROGRESS_STYLE = T_system::full;
#else
const T_system::Progress_style T_system::DEFAULT_PROGRESS_STYLE = T_system::min;
#endif

#ifdef DISABLE_FULL_PROGRESS
const bool T_system::DEFAULT_PROGRESS_USES_FULL=false;
#else
const bool T_system::DEFAULT_PROGRESS_USES_FULL=true;
#endif

/***************************************************************************
 ***************************************************************************/

#if defined(ENABLE_RECOVERY) || !defined(DISABLE_FULL_PROGRESS)
class T_system::Proto_constraint : public Pooled {
public:
  Proto_constraint(const Proto_assignment_list& pos,
		   const Proto_assignment_list& neg)
    : positive(pos.size()), negative(neg.size()) {
    {
      for (iterator it = pos.begin(); it != pos.end(); ++it) {
	const Proto_assignment& pProtoAssignment = *it;
	positive.push(pProtoAssignment);
      }
    }
    {
      for (iterator it = neg.begin(); it != neg.end(); ++it) {
	const Proto_assignment& pProtoAssignment = *it;
	negative.push(pProtoAssignment);
      }
    }
  }


  /** No subclasses ==> non-virtual. */
  ~Proto_constraint() { }


  /// Instantiate the constraint by creating a constraint that has all of the
  /// same value indices and signs, but that replaces the variables with those
  /// from the previous time step.
  void instantiate_in_past(T_system &system) const {
    system.do_create_constraint(positive, negative,
				/* is_volatile */ false,
				/* in_theory   */ false,
				/* get_previous*/ true);
  }

private:
  typedef Proto_assignment_list::iterator iterator;
  Proto_assignment_list positive;
  Proto_assignment_list negative;
};
#endif


/***************************************************************************
 * Destructor
 ***************************************************************************/

T_system::~T_system() {
  t_sys_record(destroying_container(*this));
#if defined(ENABLE_RECOVERY) || !defined(DISABLE_FULL_PROGRESS)
  // Deallocate all Proto_constraint objects
  {
    for (Slist<Proto_constraint *>::iterator it =
	   state_constraint_prototypes.begin();
	 it != state_constraint_prototypes.end(); ++it) {
      const Proto_constraint* const pProtoConstraint = *it;
      // Older compilers don't know that delete is const
      delete const_cast<Proto_constraint*>(pProtoConstraint);
    }
  }
  state_constraint_prototypes.erase();
#endif

  // take care of the trajectories
#if (0)
  delete [] state_var_trajectories;
  delete [] old_trajectories;
#else
  L2_free_array(state_var_trajectories, n_trajectories);
  L2_free_array(old_trajectories, n_old_trajectories);
#endif
}


/***************************************************************************
 ***************************************************************************/

// Advance to the next time step, with the command specified by an Assignment

void T_system::progress(Assignment* pCommandAssignment) {
#if defined(DISABLE_MIN_PROGRESS)
  full_progress(pCommandAssignment);
#elif defined(DISABLE_FULL_PROGRESS)
  minimal_progress(pCommandAssignment);
#else
  if (get_progress_uses_full()) { full_progress(pCommandAssignment); }
  else                          { minimal_progress(pCommandAssignment); }
#endif
}


#ifndef DISABLE_MIN_PROGRESS
void T_system::minimal_progress(Assignment *pCommandAssignment) {
  truncate_if_needed();
  if (pCommandAssignment != NULL) {
    // Create the Command Variable in the past Assignment. We should be able
    // to optimize the Command variable out: Once we have given the Command, we
    // introduce only Clauses that contain that Assignment.  Thus the
    // Assignment itself is redundant. This would be a savings of one Variable.

    Command* const pCurrentCommand =
      static_cast<Command *>(pCommandAssignment->get_variable());
    Command* const pPastCommand = copy_in_past(*pCurrentCommand);

    bool transitioned = false;
    const unsigned variableCount = npresent_variables();
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[i];
      if (pVariable->is_transitioned()) {
	Transitioned* const pTransitioned =
	  static_cast<Transitioned *>(pVariable);
	// To use the minimal representation, we analyze the model off line and
	// index each Transition Variable by the commands that can cause it to
	// transition.
      
	// We need to process this Variable only if it's indexed to change on
	// this Command.
	if (pTransitioned->has_indexed_transitions(*pCommandAssignment)) {
	  // This mode can change as a consequence of this Command
	  pTransitioned->transition(*pCommandAssignment, timestep);
	  transitioned = true;
	} else {
#ifdef TRANSITIONED_DEBUG
	  _STD_ cerr << "Command " << pCommandAssignment << " does not affect "
		    << pTransitioned->get_name() << _STD_ endl;
#endif
	}
      }
    }

#ifdef TRANSITIONED_DEBUG
    if (!transitioned) {
      _STD_ cerr << "Warning: Command " << (unsigned)pCommandAssignment
		<< " did not transition any variables" << _STD_ endl;
    }
#endif

    // We have to do the assignment to the past var after we have done the
    // progress, which introduces the constraints that the pPastCommand
    // participates in. Otherwise the truth maintenance system won't know
    // what constraints the assignment effects.
    pPastCommand->assign(pCommandAssignment->get_index());
    pPastCommand->set_exogenously_assigned(true);
    propagate();
  } else {
    // Idling min-progress does nothing. We represent only new Commands; so
    // with no Commands to represent, we've nothing to do!
  }
  next_timestep();
}
#endif


#ifndef DISABLE_FULL_PROGRESS

// Make copies of the Dependent, Assignable and Transitioned Variables in the
// current time step.
// Auxiliary to full_progress()

void
T_system::full_progress_copy_variables(const Assignment* pCommandAssignment) {

  // Copy all of the Assignable and non-mode Dependent Variables into the new
  // past time step.
  // The Transitioned Variables are constrained from one time step to the 
  // next, so we can't just copy them. We have to transition them if we are
  // processing a Command or idle them if we are not.

  const unsigned variableCount = npresent_variables();
  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[i];
      if (pVariable->is_assignable()) {    // assignable
	Assignable* const pAssignable = static_cast<Assignable *>(pVariable);
	Assignable* const pPastAssignable = copy_in_past(*pAssignable);
	// If current was exogenously assigned, so was past
	pPastAssignable->set_exogenously_assigned(
	 pAssignable->is_exogenously_assigned());
      } else if (pVariable->is_dependent() &&    // dependent
		 !pVariable->is_transitioned()) {  // but not mode
	Dependent* const pDependent = static_cast<Dependent *>(pVariable);
	copy_in_past(*pDependent);
      }
    }
  }

  /// The Transitioned Variables are constrained from one time step to the 
  /// next, so we can't just copy them.   We have to transition them if we are
  /// processing a Command or idle them if we are not.

  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[i];
      if (pVariable->is_transitioned()) {
	Transitioned* const pTransitioned =
	  static_cast<Transitioned *>(pVariable);
	(pCommandAssignment != NULL) ?
	  pTransitioned->transition(*pCommandAssignment, timestep) :
	  pTransitioned->idle(timestep);
      }    
    }
  }
}


// Now that we have all of the Variables for the last time step, instantiate
// all of the constraints that will constrain them within that time step.
// Auxliary to full_progress()

void
T_system::full_progress_create_constraints() {
  for (Slist<Proto_constraint *>::iterator it =
	 state_constraint_prototypes.begin();
       it != state_constraint_prototypes.end(); ++it) {
    const Proto_constraint* const pProtoConstraint = *it;
    pProtoConstraint->instantiate_in_past(*this);
  }
}


// This step copies the Assignments of all Variables except for the Command
// Variable that we are progressing on.  That Variable is set by the Command
// Assignment passed to progress.
// Note we cannot assign the new Variables until the constraints that involve
// them are created (STEP 2 above).
// Auxliary to full_progress()

void
T_system::full_progress_make_assignments(const Assignment* pCommandAssignment) 
{

  // The Command from the Command Assignment parameter
  const Assignable* pCurrCommand = NULL;
  if (pCommandAssignment != NULL) {
    pCurrCommand =
      static_cast<const Assignable*>(pCommandAssignment->get_variable());
  }
  
  /// Iterate over all Assignable Variables.  Assign each to the same as the
  /// value in the current time step, except the Command variable we are
  /// progressing on.  That Variable must be given the value from the Command
  /// Assignment we are implementing.

  const unsigned variableCount = npresent_variables();
  for (unsigned i = 0; i < variableCount; i++) {
    Variable* const pVariable = present_variables[i];
    if (pVariable->is_assignable()) {
      // The Assignable in the current time step
      Assignable* const pCurrAssignable = static_cast<Assignable *>(pVariable);
      // The Assignable in the previous time step
      Assignable* const pPastAssignable =
	static_cast<Assignable*>(pCurrAssignable->get_previous());
      L2_assert(pPastAssignable,
		L2_fatal_error,
		("Failed to find variable during progress"));
      if ((pCurrCommand != NULL) && (pCurrAssignable == pCurrCommand)) {
	// The Assignable is the Command; assign the Command
	pPastAssignable->assign(pCommandAssignment->get_index());
	pPastAssignable->set_exogenously_assigned(true);
      } else {
	// Either idle or Assignable is not the Command; just copy
	pPastAssignable->assign(*pCurrAssignable);
	pPastAssignable->set_exogenously_assigned(false);
      }
    }
  }
}


void T_system::full_progress(const Assignment* pCommandAssignment) {
  truncate_if_needed();
  /// STEP 1:  Create the variables for the new time step
  full_progress_copy_variables(pCommandAssignment);
  /// STEP 2:  Create the constraints for the new time step
  full_progress_create_constraints();
  /// STEP 3:  Make the assignments for the new time step
  full_progress_make_assignments(pCommandAssignment);
  propagate();
  /// All done.  Go to the next time step.
  next_timestep();
}
#endif


// For each value of the Transitioned Variable, splice in the Dependent
// Variable
// We are going to move the Clauses that support now_var onto past_var
// and then resupport old_var
// There are two classes of Clauses: those that constrain Variables within
// a timestep and those that constrain Variables across time steps.  We
// want to move only those that go across time steps, so we track them.

void T_system::splice(Transitioned &now_var, Dependent &past_var) {
  // Copy the Clauses from the NOW Transition Variable
  Clause::ListOfClause old_transition_clauses = now_var.transition_clauses;

  // Erase the Clauses from the NOW Transition Variable
  now_var.transition_clauses.erase();

  // If they have the same number of Variable values, they have the same
  // number of Propositions
  L2_assert(now_var.get_nvalues() == past_var.get_nvalues(),
	    L2_argument_error,
            ("splicing variables of different-size domains"));

  // Replace the now Proposition with the new Proposition in the now
  // Proposition's transition Clauses, and fix the support in the TMS
  for (unsigned i = 0; i < now_var.get_nvalues(); i++) {
    get_solver()->move_support(now_var.get_prop(i),
			       old_transition_clauses,
			       past_var.get_prop(i));
  }
}


Dependent* T_system::copy_in_past(Dependent& now_var) {
  Dependent* const past_var =
    create_dependent(now_var.get_nvalues(), timestep, now_var.get_id());
  t_sys_record(progressed_variable(now_var, *past_var));
  now_var.insert_previous(past_var);
  return past_var;
}


Assignable* T_system::copy_in_past(Assignable& now_var) {
  Assignable* const past_var =
    create_assignable(now_var.get_nvalues(), timestep, now_var.get_id());
  t_sys_record(progressed_variable(now_var, *past_var));
  now_var.insert_previous(past_var);
  return past_var;
}


Command *T_system::copy_in_past(Command& now_var) {
  Command* const past_var =
    create_command(now_var.get_nvalues(), timestep, now_var.get_id());
  t_sys_record(progressed_variable(now_var, *past_var));
  now_var.insert_previous(past_var);
  return past_var;
}


Observable *T_system::copy_in_past(Observable& now_var) {
  Observable* const past_var =
    create_observable(now_var.get_nvalues(), timestep, now_var.get_id());
  t_sys_record(progressed_variable(now_var, *past_var));
  now_var.insert_previous(past_var);
  return past_var;
}


/***************************************************************************
        Remove variables we've truncated off.
 ***************************************************************************/

// A conflict is old if any of its Assignments is from before the horizon

static bool conflict_is_old(const Conflict* pConflict,
			    Theory::time_step_t horizon) {
  for (Conflict::const_iterator it = pConflict->begin();
       it != pConflict->end() ; ++it) {
    const Assignment* const pAssignment = *it;
    if (pAssignment->get_time_step() < horizon) { return true; }
  }
  return false;
}


/**
 * This callback object has method accept(const Variable&) which returns true
 * if the Variable is "old" -- its time step is before the horizon with which
 * the callback object was created.
 */

class T_system::Old_variable_acceptor : public Conflict_db::Variable_acceptor {
public:
  /// Create the callback object with the horizon time step.
  Old_variable_acceptor(Theory::time_step_t horizon) : h(horizon) { }

  /// Accept the Variable if its time step is before the horizon.
  virtual bool accept(const Variable& rVariable) const {
    return rVariable.get_time_step() < h;
  }
private:
  /// The horizon before which Variables are deemed to be old
  const unsigned h;
};


void T_system::remove_old_variables(Theory::time_step_t horizon) {

  // destroy the conflicts
  // Should probably be in Conflict_db::destroy_variable.
  // Should at least be better than O(n) in number of conflicts,
  // except we (usually) don't have many.
    
  for (Conflict::ListOfConflict::iterator it = begin_conflicts(); 
       it != end_conflicts(); ) {
    const Conflict* const pConflict = *it;
    if (conflict_is_old(pConflict, horizon)) {
      destroy_conflict(it); // iterates it
    } else {
      // no obsolete variable; iterate it
      ++it;
    }
  }

  // destroy the variables
  {
    Old_variable_acceptor ova(horizon);
    const unsigned variableCount = npresent_variables();
    // Remove any links to modeTransitions being removed
    {
      for (unsigned i = 0; i < variableCount; i++) {
	Variable* const pVariable = present_variables[i];
	if (pVariable->is_transitioned()) {
	  Transitioned* const pTransitioned =
	    static_cast<Transitioned *>(pVariable);
	  pTransitioned->remove_old_modeTransition(horizon);
	}
      }
    }

    // Assumes sorted by time:
    // t ... horizon ... NOW   (horizon possibly missing, i.e. initially)

    // Set start to the first variable predating the horizon;
    // set end just after the last old variable, i.e. the first
    // variable of the NOW timestep
    Variable::ListOfVariable::iterator start = begin_variables();
    while ((*start)->get_time_step() >= horizon) {
      oldest_timestep = (*start)->get_time_step();
      ++start;
    }

    Variable::ListOfVariable::iterator end = start;
    while ((*end)->get_time_step() != static_cast<unsigned>(T_system::NOW)) {
      ++end;
    }

    destroy_variables(&ova, start, end, false);

    // Fix the transitioned variables' clauses, in case some are out of date.
    {
      for (unsigned i = 0; i < variableCount; i++) {
	Variable* const pVariable = present_variables[i];
	if (pVariable->is_transitioned()) {
	  Transitioned* const pTransitioned =
	    static_cast<Transitioned *>(pVariable);
	  pTransitioned->remove_destroyed_clauses();
	}
      }
    }

    get_solver()->finish_destroying();
  }
}


State_variable *T_system::create_state_var(Theory::time_step_t horizon,
					   unsigned nstates) {
  assert(nstates > 0);

  // Insert before the iterator that points to the first Variable in the
  // horizon.
  
  for (Variable::ListOfVariable::iterator it = begin_variables();
      it != end_variables(); ++it) {
    const Variable* const pVariable = *it;
    // Assumes sorted by time:
    // t ... horizon ... NOW   (horizon possibly missing, i.e., initially)
    const unsigned var_time = pVariable->get_time_step();
    if (var_time <= horizon || var_time == NOW) {
      // Tell the Conflict_db to insert new variables there temporarily
      set_new_variable_iterator(it);
      break;
    }
  }

  // Create a list of nstates zeroes. These are the weights.
  Assignment::ListOfWeight zeroes;
  for (unsigned i = 0; i < nstates; ++i) { zeroes.push_front(0); }

  // Create the State Variable
  // state_var is an instance variable
  state_var = create_state_variable(nstates, zeroes, horizon, STATE_VAR_ID);
  if (state_var->get_time_step() == T_system::NOW) {
    register_new_variable(state_var);
  }

  // Tell the Conflict_db to go back to prepending Variables
  set_new_variable_iterator(begin_variables());

  // Record the action.
  t_sys_record(created_state_variable(*state_var));
  // Create a data structure to describe what the State Variable represents
  start_new_trajectories_map(state_var->get_nvalues());

  return state_var;
}


void T_system::truncate(Theory::time_step_t horizon) {
  // No work to do if we already have less than that
  if (horizon <= get_oldest_timestep()) {
    return;
  }

  // Save all the Observations; T_system::summarize_variables may unassign
  // inconsistent observations. They will be restored before returning.
  map_over_observations(save);

  // The current number of Candidates
  const unsigned initialSize = getCandidates().size();

  if (initialSize == 0) {
    // No consistent Candidates; so truncate based on the start states
    map_over_observations(unassign);
    const unsigned count = get_state_variable()->get_nvalues();
    State_variable* const pStateVariable = create_state_var(horizon, count);
    for (unsigned i = 0; i < count; ++i) {
      pStateVariable->assign(i);
      summarize_variables(horizon, i);
    }
#ifdef ENABLE_SUMMARIZE_CONFLICTS
    summarize_conflicts(horizon);
#endif
  } else {
    // Truncation limit is min(size(), max_truncated_candidates) if max != 0
    unsigned truncationLimit =
      (max_truncated_candidates != 0 &&
       initialSize > max_truncated_candidates) ?
      max_truncated_candidates :
      initialSize;
    State_variable* const pStateVariable =
      create_state_var(horizon, truncationLimit);
    Candidate::ListOfCandidate::iterator it = getCandidates().begin();
    for (unsigned i = 0; it != getCandidates().end() && i < truncationLimit;) {
      Candidate* const pCandidate = *it;
      install(*pCandidate);
      if (get_solver()->propagate()) {
	// The Candidate leaves the system consistent; summarize it as state=si
	put_candidate_in_trajectory(horizon, i, *pCandidate);
	summarize_variables(horizon, i);
	pCandidate->truncate(horizon, *pStateVariable->get_assignment(i)); 
	++i;   // count one more toward truncationLimit
	++it;  // move to next Candidate
      } else {
	// The Candidate makes the system inconsistent; delete it
	delete pCandidate;
	getCandidates().erase(it);
      }
    }
#ifdef ENABLE_SUMMARIZE_CONFLICTS
    summarize_conflicts(horizon);
#endif
    // delete Candidates we can't fit, if any
    for ( ; it != getCandidates().end(); ) {
      const Candidate* const pCandidate = *it;
      // Older compilers don't know that delete is const
      delete const_cast<Candidate*>(pCandidate);
      getCandidates().erase(it);
    }
  }

  // Summaries have been made; it's safe to remove the summarized stuff
  remove_old_variables(horizon);
  install_new_state();

  // Restore the observations
  map_over_observations(restore);

  if (getCandidates().size() == 0) { get_solver()->propagate(); }
  else                             { install(0u); }
}


/***************************************************************************
 Looping over all observations to avoid truncating while in an inconsistent
 state.
 ***************************************************************************/

// For each Observable in the T_system, for each time step, carry out the
// map_command, which is one of { save, unassign, restore }

void T_system::map_over_observations(map_command cmd) {
  const unsigned variableCount = npresent_variables();
  // For each Observable
  for (unsigned i = 0; i < variableCount; i++) {
    // get_previous() doesn't know about Observable
    Variable* pVariable = get_present_variable(i);
    if (pVariable->is_observable()) {
      // For each time step
      do {
	// This switch is done for each observable for each time step, when
	// it should be done once; but I haven't yet figured out how
	switch (cmd) {
	  // These functions are no-ops if pVariable points to a non-Observable
	case save:     pVariable->save();     break;
	case unassign: pVariable->unassign(); break;
	case restore:  pVariable->restore();  break;
	}
	pVariable = pVariable->get_previous();
      } while (pVariable != NULL);
    }
  }
}


/***************************************************************************
 * Truncation: summarize the values of the variables, currently calculated
 * via inference, into a clause ; replace obsolete conflicts.
 ***************************************************************************/

// For each Transition Variable, set its initial mode appropriately
// Don't immediately add the clauses to the theory however.

void T_system::summarize_variables(Theory::time_step_t horizon,
				   unsigned stateVariableValueIndex) {
  const unsigned variableCount = npresent_variables();
  for (unsigned i = 0; i < variableCount; i++) {
    Variable* const pVariable = present_variables[i];
    if (pVariable->is_transitioned()) {
      // It is a mode
      Transitioned* const pTransitioned =
	static_cast<Transitioned *>(pVariable);
      Clause* const pClause =
	pTransitioned->set_initial_mode(stateVariableValueIndex,
					-1,       // discover the value
					horizon,
					false);   // do not add to theory
      if (pClause != NULL) {
	// Collect the Clauses for adding to the theory in a batch
	new_clauses.push_front(pClause);
      }
    }
  }
}


#ifdef ENABLE_SUMMARIZE_CONFLICTS
/**
 * Does the Conflict have any Assignment older than the horizon?
 * \param pConflict pointer to the Conflict
 * \param horizon the horizon time step
 * \return whether the Conflict contains an Assignment the time step of which
 * is older than the horizon
 */
static bool conflict_has_old_active_assignments(const Conflict *pConflict,
						Theory::time_step_t horizon) {
  for (Conflict::const_iterator it = pConflict->begin();
       it != pConflict->end(); ++it) {
    const Assignment* const pAssignment = *it;
    // does not assume time ordering
    if ((pAssignment->get_time_step() < horizon) &&
	pAssignment->get_variable()->is_current(*pAssignment))
      return true;
  }
  return false;
}


bool T_system::covers(const Trajectory& trajectory,
		      const Assignment& assignment) const {
  for (Trajectory::iterator it = trajectory.begin(); it != trajectory.end();
       ++it) {
    const Trajectory::Parts& parts = *it;
    if ((parts.get_id()    == assignment.get_variable()->get_id()) &&
	(parts.get_time()  == assignment.get_variable()->get_time_step()) &&
	(parts.get_index() == assignment.get_index())) {
      // The Trajectory's Parts elements represents the Assignment
      return true;
    }
  }
  // None of the Trajectory's Parts elements represents the Assignment
  return false;
}


bool T_system::consistentWith(const Trajectory& trajectory,
			      const Assignment& assignment) const {
  for (Trajectory::iterator it = trajectory.begin(); it != trajectory.end();
       ++it) {
    const Trajectory::Parts& parts = *it;
    if ((parts.get_id()    == assignment.get_variable()->get_id()) &&
        (parts.get_time()  == assignment.get_variable()->get_time_step()) &&
	(parts.get_index() != assignment.get_index())) {
      // The Trajectory's Parts elements contradicts the Assignment
      return false;
    }
  }
  // None of the Trajectory's Parts elements contradicts the Assignment
  return true;
}


bool T_system::summarizes(const Trajectory& trajectory,
			  const Assignment::ListOfAssignment& beforeHorizon,
			  unsigned stateVariableValueIndex) const {
  // Does the Trajectory cover the prior State Variable value?
  if (stateVariableValueIndex == static_cast<unsigned>(-1)) {
    // No State Variable value to test; it succeeds by default
  } else {
    // Does this Trajectory subsume the specified Trajectory from the previous
    // time step?
    assert(stateVariableValueIndex < n_old_trajectories);
    Trajectory& priorTrajectory = old_trajectories[stateVariableValueIndex];
    if (!trajectory.subsumes(priorTrajectory)) { return false; }
  }
  // Does the Trajectory cover the Assignments before the horizon
  for (Assignment::ListOfAssignment::const_iterator it = beforeHorizon.begin();
       it != beforeHorizon.end(); ++it) {
    const Assignment* const pAssignment = *it;
    if (pAssignment->get_index() == 0) {
      // The Assignment is modeTransition=nominal
      // The Assignment must not be contradicted by anything in the Trajectory
      if (!consistentWith(trajectory, *pAssignment)) {
	return false;
      }
    } else {
      // The Assignment is modeTransition!=nominal
      // The Assignment must be contained in the Trajectory
      if (!covers(trajectory, *pAssignment)) {
	return false;
      }
    }
  }
  // No violations, so it is summarizable
  return true;
}


/**
 * Partition the Conflict's Assignments into pre-horizon and all others
 * \param conflict the Conflict
 * \param horizon the horizon time step
 * \param beforeHorizon the Conflict's Assignments before the horizon, except
 * \param onOrAfterHorizon the Conflict's Assignments on or after the horizon;
 * an out parameter
 * the State Variable; an out parameter
 */
static unsigned splitConflict(const Conflict& conflict,
			  Theory::time_step_t horizon,
			  Assignment::ListOfAssignment& beforeHorizon,
			  Assignment::ListOfAssignment& onOrAfterHorizon) {
  // The default value means "no State Variable in the Conflict"
  unsigned stateVariableValueIndex = static_cast<unsigned>(-1);
  for (Conflict::const_iterator it = conflict.begin(); it != conflict.end();
       ++it) {
    Assignment* const pAssignment = *it;
    if (pAssignment->get_variable()->is_state_variable()) {
      assert(stateVariableValueIndex == static_cast<unsigned>(-1));
      stateVariableValueIndex = pAssignment->get_index();
    } else if (pAssignment->get_time_step() >= horizon) {
      // Assignment is on or after the horizon; retain
      onOrAfterHorizon.push_front(pAssignment);
    } else {
      // Assignment is before the horizon; summarize before truncating
      beforeHorizon.push_front(pAssignment);
    }
  }
  return stateVariableValueIndex;
}


void
T_system::summarize_conflict(const Assignment::ListOfAssignment&
			     beforeHorizon,
			     const Assignment::ListOfAssignment&
			     onOrAfterHorizon,
			     unsigned stateVariableValueIndex) {
  // The Candidate doesn't conflict with itself, so if we've truncated
  // off all the conflicting Assignments, we've lost the Conflict
  // if (!onOrAfterHorizon.empty()) {
  Array<unsigned, false> summaries;
  // Seek State Variable values that are consistent with the prefix
  for (unsigned i = 0; i < n_trajectories; i++) {
    if (summarizes(get_trajectory(i), beforeHorizon,
		   stateVariableValueIndex)) {
      summaries.push(i);
    }
  }
  // summarizedConflicts is an Array of summarizing State Variable values
  if (summaries.size() == n_trajectories) {
    // Every single value of the State Variable summarizes the Conflict. The
    // Conflict being truncated was not minimal; it is still a Conflict with
    // the prefix removed. Make only one Conflict with no State Variable
    // Assignment
    if (!onOrAfterHorizon.empty()) { new_conflicts.push(onOrAfterHorizon); }
  } else {
    // Not all values of the State Variable summarize the Conflict. For each
    // value that does, create a Conflict in which an Assignment to the State
    // Variable replaces the truncated prefix.
    for (unsigned i = 0; i < summaries.size(); i++) {
      // This State Variable Assignment will go into a summary Conflict
      Assignment* const pStateVariableValueAssignment =
	state_var->get_assignment(summaries[i]);
      // Copy the suffix
      Assignment::ListOfAssignment newConflictSpec = onOrAfterHorizon;
      // Add the State Variable Assignment to the conflict spec
      newConflictSpec.push_front(pStateVariableValueAssignment);
      // Queue the spec for creation of a Conflict
      new_conflicts.push(newConflictSpec);
    }
  }
}


void T_system::summarize_conflicts(Theory::time_step_t horizon) {
  // foreach Conflict
  for (Conflict::ListOfConflict::iterator it = begin_conflicts();
      it != end_conflicts(); ++it) {
    const Conflict* const pConflict = *it;
    if (conflict_has_old_active_assignments(pConflict, horizon)) {
      // The Conflict is at time < horizon and at least one Assignment is true,
      // Create a replacement Conflict that has state=s_i and all the
      // Assignments at time >= h ; note: no assumption about time ordering.
      Assignment::ListOfAssignment beforeHorizon;
      Assignment::ListOfAssignment onOrAfterHorizon;
      // Partition the Assignments into < horizon and >= horizon
      // Return the State Variable value index if present; otherwise -1
      const unsigned stateVariableValueIndex = 
	splitConflict(*pConflict, horizon, beforeHorizon, onOrAfterHorizon);
      // Create the new summarized Conflict(s)
      summarize_conflict(beforeHorizon, onOrAfterHorizon,
			 stateVariableValueIndex);
    }
  }
}
#endif


/***************************************************************************
        Install the new clauses and conflicts.
 ***************************************************************************/

void T_system::install_new_state() {
  // Install the new clauses we just created.
  {
    for (Clause::ListOfClause::iterator it = new_clauses.begin();
	 it != new_clauses.end(); ++it) {
      Clause* const pClause = *it;
      get_solver()->add(*pClause);
    }
  }
  new_clauses.erase();

  // Also add all the new conflicts.
  {
    for (Array<Assignment::ListOfAssignment, true>::iterator it =
	   new_conflicts.begin(); it != new_conflicts.end(); ++ it) {
      Assignment::ListOfAssignment& conflictSet = *it;
      add_conflict(conflictSet);
    }
  }
  new_conflicts.erase();

  // Finally, sort the state_var assignables by weight.

  Assignment::ListOfAssignment vals;

  // Use insertion sort, in reverse order because push_front is
  // cheaper than push_back:
  // for i=n down to 1
  //      max = 1
  //      for j=1 to i
  //          if A[j] >= A[max]
  //              max = j
  //      A[i] = A[max]
  // Checking A[j] >= makes the sort stable.
  for (unsigned i = state_var->get_nvalues(); i >= 1; --i) {

    // max = 1
    Assignable::iterator max_ait = state_var->assignments_begin();;
    Assignment::weight_t max_weight = 0;
    
    // for j=1 to i
    Assignable::iterator ait = state_var->assignments_begin();
    for (unsigned j = 1; j <= i; ++j, ++ait) {
      const Assignment* const pAssignment = *ait;
      if (pAssignment->get_weight() >= max_weight) {
	max_ait = ait;
      }
    }
    assert(ait == state_var->assignments_end());
    vals.push_front(*max_ait);
    state_var->assignments.erase(max_ait);
  }

  // Change the assignables list in a most heinous fashion.
  state_var->assignments = vals;
}

/***************************************************************************
  Creating variables.
 ***************************************************************************/
void
T_system::register_new_variable(Variable* pVariable) {
  // Are these tests sufficient?
  // We put in variables that are current, but not modeTransitions since they
  // share ID with the mode.
  L2_assert(pVariable->get_time_step() == T_system::NOW,
	    L2_fatal_error,
	    ("Attempt to register non-NOW Variable"));
  L2_assert(!pVariable->is_transition(),
	    L2_fatal_error,
	    ("Attempt to register Transition Variable"));
  const unsigned ID = pVariable->get_id();
  L2_assert(present_variables.size() <= ID || present_variables[ID] == NULL,
	    L2_fatal_error,
	    ("index " + MBA_string(ID) + " already occupied"));
  present_variables.grow_and_set(ID, pVariable);
}


Past_command*
T_system::create_past_command(unsigned valueCount,
			      const Assignment::ListOfWeight& costs,
			      Theory::time_step_t timeStep,
			      unsigned ID) {
  Past_command* const pPastCommand =
    new Past_command(*this, valueCount, costs, timeStep, ID);
  registerAssumption(pPastCommand);
  return pPastCommand;
}


// Note we set the new transition variable to its default (i.e., nominal) value

Transition*
T_system::create_mode_transition(Dependent& modevar,
				 unsigned domain,
				 const Assignment::ListOfWeight& costs,
				 Theory::time_step_t time_step) {
  Transition* const pTransition =
    create_transition(domain, costs, time_step, modevar.get_id());
  pTransition->restore_default();
  t_sys_record(created_mode_transition(modevar, *pTransition));
  return pTransition;
}


State_variable*
T_system::create_state_variable(unsigned nvalues,
				const Assignment::ListOfWeight& costs,
				Theory::time_step_t time_step,
				unsigned id) {
  State_variable* const pStateVariable =
    new State_variable(*this, nvalues, costs, time_step, id);
  registerAssumption(pStateVariable);
  return pStateVariable;
}


Transition*
T_system::create_transition(unsigned valueCount,
			    const Assignment::ListOfWeight& costs,
			    Theory::time_step_t timeStep,
			    unsigned ID) {
  Transition* const pTransition =
    new Transition(*this, valueCount, costs, timeStep, ID);
  registerAssumption(pTransition);
  return pTransition;
}


/********************************************************************
    The following functionality is to allow the full single-state
    model (eg not the transition model) to be copied at each
    time point in the system rather than being represented sparsely.
 ********************************************************************/


Clause*
T_system::create_constraint(const Proto_assignment_list & positive,
			    const Proto_assignment_list & negative,
			    bool  is_volatile,
			    bool  in_theory,
			    bool  state_constraint) {

#if defined(ENABLE_RECOVERY) || !defined(DISABLE_FULL_PROGRESS)
  if (state_constraint) {
    Proto_constraint* const proto = new Proto_constraint(positive, negative);
    state_constraint_prototypes.push_front(proto);
  }
#endif

  return do_create_constraint(positive, negative, is_volatile, in_theory,
			      /*get_previous*/ false);
}

Clause
*T_system::do_create_constraint(const Proto_assignment_list & positive,
				const Proto_assignment_list & negative,
				bool  /* is_volatile */,
				bool  /* in_theory */,
				bool  get_previous) {
  Proposition::ListOfProposition posPropositions, negPropositions;
  // Collect the positive Propositions from the positive Proto_assignments
  {
    for (unsigned int i = 0; i < positive.size(); ++i) {
      const Variable* pVariable = positive[i].first;
      if (get_previous) pVariable = pVariable->get_previous();
      Proposition* const pProposition =
	pVariable->get_prop(positive[i].second);
      posPropositions.push_front(pProposition);
    }
  }
  // Collect the negative Propositions from the negative Proto_assignments
  {
    for (unsigned int i = 0; i < negative.size(); ++i) {
      const Variable* pVariable = negative[i].first;
      if (get_previous) pVariable = pVariable->get_previous();
      Proposition* const pProposition =
	pVariable->get_prop(negative[i].second);
      negPropositions.push_front(pProposition);
    }
  }

  // Note args 3 and 3 are literals; this function's parameters are ignored
  return
    Conflict_db::create_clause(posPropositions, negPropositions, false, true);
}


/***************************************************************************
  API by variable index
 ***************************************************************************/

Command *T_system::get_command(unsigned varID) const {
  Variable* const pVariable = get_present_variable(varID);
  L2_assert(pVariable != 0 && pVariable->is_command(),
	    L2_not_found_error,
            ("v" + MBA_string(varID) +" is not commandable"));
  return static_cast<Command *>(pVariable);
}


Observable *T_system::get_observable(unsigned varID) const {
  Variable* const pVariable = get_present_variable(varID);
  L2_assert(pVariable != 0 && pVariable->is_observable(),
	    L2_not_found_error,
	    ("v"+MBA_string(varID)+" is not observable"));
  return static_cast<Observable *>(pVariable);
}


Transitioned *T_system::get_transitioned(unsigned varID) const {
  Variable* const pVariable = get_present_variable(varID);
  L2_assert(pVariable != 0 && pVariable->is_transitioned(),
	    L2_not_found_error,
            ("v" + MBA_string(varID) + " is not a mode variable"));
  return static_cast<Transitioned*>(pVariable);
}

Transition* T_system::get_transition(unsigned varID) const {
  Transitioned* const transitioned = get_transitioned(varID);
  return (transitioned == NULL) ? NULL : transitioned->get_zenith();
}

#ifdef ENABLE_RECOVERY
void T_system::planner_progress(void) {

  const unsigned variableCount = npresent_variables();
  /// STEP 1:  Create the variables for the new time step

  /// Copy all of the dependent variables into the new past time step
  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[variableCount - i - 1];
      if (!pVariable->is_assignable() &&    // dependent
	  !pVariable->is_transitioned()) {  // but not mode
	Dependent* const pDependent = static_cast<Dependent *>(pVariable);
	copy_in_past(*pDependent);
      }
    }
  }

  /// Ditto for the assignable variables
  /// This code assumes there are no assignable variables that are not
  /// either observations or commands.  We copy observations first,
  /// then commands.
  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[variableCount - i - 1];
      if (pVariable->is_observable()) {
	Observable* const pObservable = static_cast<Observable *>(pVariable);
	Observable* const pPastObservable = copy_in_past(*pObservable);
	pPastObservable->set_exogenously_assigned(
	  pObservable->is_exogenously_assigned()
	);
      }
    }
  }

  /// While planning, Commands are Assumptions.  Once we
  /// integrate planning and diagnosis, some Commands will have
  /// to be Assignables (in the past) and some will be assumptions
  /// (in the future).   We will have to sort that out in order
  /// to integrate planning & diagnosis.  For now we just
  /// create Commands as Assumptions and store them in the Command list
  /// as Assignables.  We cast them when we pull them back out.
  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[variableCount - i - 1];
      if (pVariable->is_command()) {
	Command* const pCommand = static_cast<Command *>(pVariable);
	create_cmd_assumption_in_past(*pCommand);
      }
    }
  }

  /// The transitioned variables are constrained from one time step
  /// to the next, so we can't just copy them.   We have to transition
  /// them if we are processing a command or idle them if we are not.
  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[variableCount - i - 1];
      if (pVariable->is_transitioned()) {
	Transitioned* const pTransitioned =
	  static_cast<Transitioned *>(pVariable);
	pTransitioned->planner_transition(timestep);
      }
    }
  }

  /// STEP 2:  Create the constraints for the new time step

  /// Now that we have all of the variables for the last time
  /// step, instantiate all of the constraints that
  /// will constrain them within that time step.
  {
    for (Slist<Proto_constraint *>::iterator it =
	   state_constraint_prototypes.begin();
	 it != state_constraint_prototypes.end(); ++it) {
      const Proto_constraint* const pProtoConstraint = *it;
      pProtoConstraint->instantiate_in_past(*this);
    }
  }

  /// When progressing for diagnosis, we copy the commands
  /// and observations.  This makes sense because we'll be
  /// progressing, making some observations, etc.
  ///
  /// With planning, we'll just blast out the number of time
  /// steps we want in one go, so there won't be any observations
  /// to copy.  If the user wants observations in the plan model,
  /// they can be made once the full model is constructed.

  {
    for (unsigned i = 0; i < variableCount; i++) {
      Variable* const pVariable = present_variables[variableCount - i - 1];
      if (pVariable->is_command()) {
	Command* const pCommand = static_cast<Command *>(pVariable);
	Command* const past_var = static_cast<Command *>(pCommand->get_previous());
	L2_assert(past_var,
		  L2_fatal_error,
		  ("Failed to find variable during planner progress"));
	past_var->assign(*pCommand);
      }
    }
  }

  propagate();
  /// All done.  Go to the next time step.
  next_timestep();
}


Past_command* T_system::create_cmd_assumption_in_past(Command& now_cmd) {
  Assignment::ListOfWeight weights;
  for (unsigned i = 0; i < now_cmd.get_nvalues(); ++i) {
    weights.push_front(1);
  }
  weights.reverse(); // what good does this do?
  
  Past_command* const pPastCommand =
    create_past_command(now_cmd.get_nvalues(), weights, timestep,
			now_cmd.get_id());
  if (timestep == T_system::NOW) {
    register_new_variable(pPastCommand);
  }
  
  t_sys_record(progressed_variable(now_cmd, *pPastCommand));
  now_cmd.insert_previous(pPastCommand);
  
  return pPastCommand;
}

#endif

const Trajectory& T_system::get_trajectory(unsigned i) const {
  L2_assert(i < n_trajectories,
	    L2_bounds_error,
	    ("state var value", i, n_trajectories));
  return state_var_trajectories[i];
}


void T_system::start_new_trajectories_map(unsigned size) {
  // clear the old
#if (0)
  delete [] old_trajectories;
#else
  L2_free_array(old_trajectories, n_old_trajectories);
#endif
  
  // shift the new to the old
  old_trajectories = state_var_trajectories;
  n_old_trajectories = n_trajectories;

  // make room for new
#if (0)
  state_var_trajectories = new Trajectory[size];
#else
  state_var_trajectories = L2_alloc_array(Trajectory, size);
#endif
  n_trajectories = size;
}

  
void T_system::set_trajectory(unsigned newi,
			      unsigned oldi,
			      Slist<const Assignment*>& old) {
  L2_assert(newi < n_trajectories,
	    L2_bounds_error,
	    ("setting new state var value", newi, n_trajectories));
  L2_assert(oldi < n_old_trajectories,
	    L2_bounds_error,
	    ("getting old state var value", oldi, n_old_trajectories));

  Trajectory& newTrajectory = state_var_trajectories[newi];
  Trajectory& oldTrajectory = old_trajectories[oldi];

  // All Assignments of old become parts of the new Trajectory
  for (Slist<const Assignment*>::iterator it = old.begin();
       it != old.end(); ++it) {
    const Assignment* const pAssignment = *it;
    // The Assignment along with a back pointer to the L2_string_map
    const Trajectory::Parts p(pAssignment /*, *this */);
    // This copies the Parts object
    newTrajectory.insert(p);
  }
  // All Assignments of the old Trajectory are prepended to the new Trajectory
  newTrajectory.prepend(oldTrajectory);
}

void T_system::put_candidate_in_trajectory(Theory::time_step_t horizon,
					   unsigned value,
					   const Candidate& candidate) {
  // To collect the Assignments in the Candidate being truncated
  Slist<const Assignment*> existingAssignments;
  // If it doesn't appear in any Candidate, the old state assignment is 
  // state = s0 (with rank 0 -- the nominal case)
  unsigned previousValue = 0; 
  for (unsigned i = 0; i < candidate.size(); ++i) {
    const Assignment* const pAssignment = candidate.get_assignment(i);
    if (pAssignment->get_time_step() < horizon ||
	pAssignment->get_time_step() == T_system::BEFORE_HORIZON) {
      // To be truncated
      if (pAssignment->get_variable()->is_state_variable()) {
	// Only one of the Assignments can be to the State Variable
	assert(previousValue == 0);
	previousValue = pAssignment->get_index();
      } else {
	existingAssignments.push_front(pAssignment);
      }
    }
  }
  set_trajectory(value, previousValue, existingAssignments);
}


void T_system::deleteNewestConflicts() {
  for (Conflict::ListOfConflict::iterator it = begin_conflicts();
       it != end_conflicts(); ) {
    const Conflict* const pConflict = *it;
    if (pConflict->getTag() == timestep) { getConflicts().erase(it); }
    else { ++it; }
  }
}


/***************************************************************************
        Truncation support
 ***************************************************************************/

// For each Candidate, reset the Assumption defaults, install the Candidate
// and look for a Conflict. If no Conflict is found, delete the Candidate and
// erase it from the Tracker's set of Candidates.

void T_system::prune_inconsistent_candidates() {
  for (Candidate::ListOfCandidate::iterator it = getCandidates().begin();
       it != getCandidates().end(); ) {
    const Candidate* const pCandidate = *it;
    install(*pCandidate);
    const Conflict* const pConflict = conflict(get_time_step());
    if (pConflict != NULL) { // Erase and increment iterator
      // Older compilers don't know that delete is const
      delete const_cast<Candidate*>(pCandidate);
      getCandidates().erase(it);
    } else { // Just increment iterator
      ++it;
    }
  }
}

