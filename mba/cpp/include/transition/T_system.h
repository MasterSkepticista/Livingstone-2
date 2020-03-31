/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Transition system representation, NASA Ames Research Center
// The main class representing a transition system
// $Id: T_system.h,v 1.177 2004/02/10 22:16:38 lbrown Exp $

#ifndef T_SYSTEM_H
#define T_SYSTEM_H

#include <conflict_db/conflict_db.h>
#include <conflict_db/past_command.h>
#include <conflict_db/state_variable.h>
#include <conflict_db/transition.h>

#include <mba_utils/pair.h>          // for pair

#ifndef DISABLE_T_SYS_LISTEN
class T_system_listener;
#endif

class Candidate;
class Trajectory;
class Transitioned;

/**
 * The transition system is a specialization of the deductive conflict
 * database, Conflict_db, that represents discrete time.
 * 
 * <p>In the transition system, or T_system, the Assignable and Dependent
 * Variables are divided into sets that represent the state of the system
 * at different points in time as described in
 * <A href=
 * "http://ic-www.arc.nasa.gov/ic/projects/mba/abstracts/aaai-2000.html">the
 * L2 AAAI paper</A></p>
 * 
 * <p>For example, if the quantity FLOW would be represented at time steps
 * 0, 1 and 2 by the separate Variables 0#FLOW, 1#FLOW and 2#FLOW.  The
 * unadorned Variable FLOW always represents the quantity at the latest
 * time step.  The constraints in the system are also divided, into state
 * constraints and Transition constraints.  A copy of the state constraints
 * constrains the values of Variables within each time step. For example, the
 * state constraints would constrain 0#FLOW and 0#PRESSURE, 1#FLOW and
 * 1#PRESSURE, 2#FLOW and 2#PRESSURE, and finally FLOW and PRESSURE.</p>
 * 
 * <p>The Transition constraints constrain the value of certain Variables in
 * time step t+1 based upon the values of Variables in time step t. For each
 * of these Transitioned Variables, generally representing the mode of some
 * component that may be commanded, the Transition constraints specify what
 * value the Variable will have in the next time step given the conditions in
 * the previous time step.</p>
 * 
 * <p>This is a sample picture:
 * 
 * \image html solutions.jpg</p>
 *
 * Major issue:  The transition system should be able to automatically
 * truncate itself when it becomes full.  However, in order to truncate,
 * the list of current candidates is needed.  This is only available in
 * Tracker.  So the truncation functionality is split across T_system and
 * Tracker.  One solution is to move the candidate list into T_system or
 * Conflict_db.
 *  \todo  Update conflicts when truncating/substitute start state var
*/

class T_system : public Conflict_db {
  friend class Transitioned;
  friend class Tracker;
  friend class to_t_system; // so that it can use register_new_variable()
public:

  /************************************************************
   * Public types.
   ************************************************************/

  enum public_constants {
    /// The maximum number of time steps stored, unless otherwise specified.
    DEFAULT_HISTORY_LENGTH = 3,
    /// The maximum number of truncated Candidates, unless otherwise specified.
    DEFAULT_NUM_HISTORIES = 5,
    /// The minimum value for history length
    MIN_HISTORY_LENGTH = 1,
    /// The time step that Variables representing the current state of the
    /// system occupy.   This is larger than all intervening time steps.
    NOW = (unsigned)-1,
    /// The ID of the state variable.
    STATE_VAR_ID = (unsigned)-1,
    /// The time step for Transitions at the horizon
    BEFORE_HORIZON = (unsigned)-1
  };

  /**
   * A Proto_assignment specifies a Variable and a value index. This will
   * allow us to create a copy of the Variable at a particular time step, then
   * ask the new Variable to assign itself to its iTh value.
   */

  typedef MBApair<Variable*, unsigned> Proto_assignment;
  typedef Array<Proto_assignment, false> Proto_assignment_list;

  /// The T_system progress styles as symbolic constants.
  enum Progress_style {
    min,
    full,
    invalid_progress
  };

  // The default for progress_style_ is conditionally compiled.
  static const Progress_style DEFAULT_PROGRESS_STYLE;

  /**
   * Is full-progress the default progress style?
   * \todo Rename this.
   */
  static const bool DEFAULT_PROGRESS_USES_FULL; // true = full-progress

  /************************************************************
   * Constructor/destructor.
   ************************************************************/

  /**
   * Creates a transition system that tracks the evolution of a system,
   * keeping a history of the length specified by length.
   * \param historyLength the number of time steps of history to keep
   */
  T_system(unsigned historyLength, unsigned max_truncated_candidates,
	   bool progress_uses_full) :
    Conflict_db(), timestep(0), oldest_timestep(0),
    max_history_length(historyLength),
    state_var_trajectories(0),
    n_trajectories(0),
    old_trajectories(0),
    n_old_trajectories(0),
    planning(false),
    max_truncated_candidates(max_truncated_candidates),
    pTracker(NULL)
    {
      // Does some validity checking
      set_progress_uses_full(progress_uses_full);
    };

  /**
   * Deallocate the state constraint prototypes.
   */
  virtual ~T_system();

#ifndef DISABLE_T_SYS_LISTEN
  /************************************************************
   *    Listeners.
   ************************************************************/

  /**
   * Subscribe the listener
   * \param pT_system_listener pointer to the listener to add
   */
  void add_listener(T_system_listener *pT_system_listener) {
    listeners_.push_front(pT_system_listener);
  }

  /**
   * Unsubscribe the listener
   * \param pT_system_listener pointer to the listener to remove
   */
  void remove_listener(T_system_listener *pT_system_listener) {
    listeners_.remove_first(pT_system_listener);
  }
#endif

  /**
   * Progress with the Command specified as an Assignment.
   * \param pCommandAssignment pointer to the value Assignment for a Command
   * Variable; idle if null.
   */
  void progress(Assignment *pCommandAssignment = 0);

#ifndef DISABLE_MIN_PROGRESS
  /**
   * Transition the minimal number of Variables needed to represent the
   * evolution of the system based on the passed Command.  Adds to transitions
   * the new Transition Variables that were added to the transition system.
   * Presumably these will be of interest to algorithms doing searches over
   * the T_system.  If pCommandAssignment is NULL, then the idle transition is
   * taken. Transition the Variables with transition() then increments the
   * time step. This copies only the Transitioned (i.e., mode) Variables
   * involved in the Command and works only if your transition Clauses have
   * been reduced to prime implicate form. It results in very small growth per
   * model step, however.
   * \param pCommandAssignment pointer to the value assignment for a Command
   * Variable; idle if null
   */
  void minimal_progress(Assignment *pCommandAssignment = NULL);
#endif

#ifndef DISABLE_FULL_PROGRESS
  /**
   * First copies the entire now model to a new, previous time step, then
   * transitions the commanded Variables with transition().  This results
   * in a complete copy of the system model at each time step.  If 
   * pCommandAssignment = NULL, then the idle transition is taken rather than
   * a commanded transition.
   * \param pCommandAssignment pointer to the value assignment for a Command
   * Variable; idle if null
   */
  void full_progress(const Assignment *pCommandAssignment = NULL);
#endif

  /// Is Livingstone being used for planning?
  bool isPlanning() const { return planning; }
  /// Declare, irreversibly, that Livingstone is being used for planning.
  void setPlanning() { planning = true; }

#ifdef ENABLE_RECOVERY
  void planner_progress(void);
#endif

/********************************************************************
  Commands and observations

  The following functionality is to a user or external program to interact
  with the transition system by specifying Commands and Observations (both
  just collections of Assignable Variables).

  Functions from going from an enumeration value (i.e., an index) to
  a Command or Observation Variable are for the Real Time Interface.

 ********************************************************************/

  /**
   * Assigns the indexed value to the indexed Observable Variable.
   * \param observableIndex the ID of the Observable Variable
   * \param valueIndex the index of the value; if negative, unassign
   */
  void observe(unsigned observableIndex, signed valueIndex) const {
    Observable* const pObservable = get_observable(observableIndex);
    if (pObservable) { pObservable->observe(valueIndex); }
  }

  /**
   * Return the current time step, a positive integer starting with 1 and
   * increasing by 1 with each commanded or idle progress.
   * \return the current time step
   */

  unsigned get_time_step() const { return timestep;}

  /**
   * Return the earliest non-truncated time step in the history, or 0 if
   * there has been no truncation yet.
   * \return the earliest non-truncated time step in the history
   */
  unsigned get_horizon() const {
    return
      // can only happen at early time steps
      (max_history_length > timestep) ?
      0 :
      // timestep-max_history_length is all the room we have; the
      // horizon is one past the last one so we have room to grow.
      (timestep - max_history_length + 1);
  }

  /**
   * Returns the oldest timestep represented by the transition system.
   * This is computed by truncate_history().
   * \return the oldest time step represented in the transition system
   */
  unsigned get_oldest_timestep() const { return oldest_timestep; }

  /**
   * Set the time step.
   * Note this doesn't affect what the Variables reflect; the reason is that
   * this isn't used for flight, only for checkpointing. This is a very small
   * amount of code here, so I won't bother with ifdefs.
   * \param newoldest what the oldest time step is to be
   */
  void set_oldest_timestep(unsigned newoldest) {
    const unsigned curlen = get_time_step() - get_oldest_timestep();
    oldest_timestep = newoldest;
    timestep = newoldest + curlen;
  }

  /**
   * Is the history is full? If so,we can't progress without first truncating.
   * \return whether the history is full
   */
  // oldest time  = 0;
  // max history  = 3;
  // current time = 3  -> we're full
  bool history_full() const {
    return
      (max_history_length != 0) && // truncating enabled
      ((get_oldest_timestep() + max_history_length) <= get_time_step());
  }

  /**
   * Return the maximum history length, established at construction
   * \return the maximum history length
   */
  unsigned get_max_history_length() const { return max_history_length; }

  /**
   * Set the maximum history length.
   * Maximum history length must not be changed after the first progress.
   * \param the maximum history length
   */
  void set_max_history_length(unsigned length) { max_history_length = length; }

  /**
   * Set the maximum number of Candidates to retain upon progressing.
   * \param the maximum number of Candidates to retain upon progressing
   */
  void set_max_truncated_candidates(unsigned maxcand) {
    max_truncated_candidates = maxcand;
  }

  /**
   * Is the progress style full progress?
   * \return whether the progress style is full progress
   */
  bool get_progress_uses_full() const {
#if defined (DISABLE_FULL_PROGRESS)
    return false;
#elif defined(DISABLE_MIN_PROGRESS)
    return true;
#else
    return progress_uses_full;
#endif
  }

  /**
   * Set whether the progress style is full progress.
   * \param b whether the progress style is to be full progress
   */
  void set_progress_uses_full(bool progress_uses_full) {
#if defined (DISABLE_FULL_PROGRESS)
    L2_assert(!progress_uses_full, L2_disabled_error, ("full-progress"));
#elif defined(DISABLE_MIN_PROGRESS)
    L2_assert(progress_uses_full, L2_disabled_error, ("min-progress"));
#else
  /// Only set it in this case; the others are obvious.
    this->progress_uses_full = progress_uses_full;
#endif
  }


  /************************************************************
   *    Access to Variables.
   ************************************************************/

  /**
   * Return the number of "present" Variables (Variables at the NOW time step).
   * \return the number of Variables at the NOW time step
   */
  unsigned npresent_variables() const { return present_variables.size(); }

  /**
   * Return the "present" Variable indexed by varID. If the index is out of
   * range, return NULL.
   * \param varID the ID of the Variable at the NOW time step
   * \return a pointer to the specified Variable; NULL if it does not exist
   */
  Variable *get_present_variable(unsigned varID) const {
    return
      (varID < present_variables.size()) ? present_variables[varID] : 0;
  }

  /**
   * Return the Transitioned (mode) Variable indexed by varID.
   * \param varID the ID of the Transitioned Variable
   * \return a pointer to the specified Transitioned Variable; NULL if it does
   * not exist
   */
  Transitioned *get_transitioned(unsigned varID) const;

  /**
   * Return the Transition Variable indexed by varID.
   * Since Transitioned (mode) and Transition Variables share the same
   * Variable ID, only one of them (Transitioned) can be retrieved via
   * T_system::get_present_variable(unsigned)
   * \param varID the ID of the Transition Variable
   * \return a pointer to the specified Transition Variable; NULL if it does
   * not exist
   */
  Transition* get_transition(unsigned varID) const;

  /**
   * Return the Command Variable indexed by varID.
   * \param varID the ID of the Command Variable
   * \return a pointer to the specified Command Variable; NULL if it does not
   * exist
   */
  Command *get_command(unsigned varID) const;

  /**
   * Return the Observable Variable indexed by varID
   * \param varID the ID of the Observable Variable
   * \return a pointer to the specified Observable Variable; NULL if it does
   * not exist
   */
  Observable *get_observable(unsigned varID) const;

  /**
   * Returns the Variable representing the start state.
   * Useful for debugging.
   * \return a pointer to the State_variable
   */
  State_variable* get_state_variable() const { return state_var; }

  /**
   * Creates a constraint that is the disjunction of the assignments
   * represented by taking each value of the Variable, value index and sign
   * list.
   * \param positive Array of (Variable*, valueIndex) pairs for each positive
   * Variable=value Proposition
   * \param negative Array of (Variable*, valueIndex) pairs for each negative
   * Variable=value Proposition
   * \param is_volatile whether the Clause is volatile
   * \param in_theory whether the Clause is placed in the theory
   * \param state_constraint whether this is considered a constraint that must
   * be instantiated every time a new time step is introduced by full-progress
   * or plan-progress.
   * \return a pointer to the newly-created Clause
   */
  Clause* create_constraint(const Proto_assignment_list& positive,
			    const Proto_assignment_list& negative,
			    bool is_volatile,
			    bool in_theory,
			    bool state_constraint = false);

  // moved to public from protected by bhudson;
  // this allows writing new debug and reader classes

  /**
   * Creates a new Transition Variable we'll use to control a mode Variable.
   * Same arguments as create_assumption(), except we also pass in the mode
   * Variable being controlled.
   * \param modeVariable the mode that is being transitioned
   * \param domainCardinality the cardinality of the domain
   * \costs Slist of the respective Transition cost/rank/weight values
   * \param timeStep the time step at which the transition occurs
   * \return a pointer to the newly-created Transition Varaible
   */
  Transition *create_mode_transition(Dependent& modeVariable,
				     unsigned domainCardinality,
				     const Assignment::ListOfWeight& costs,
				     unsigned timeStep);

  /**
   * Create a new Variable at time horizon which has candidateCount values.
   * The reader needs to call this with (0,1) to set the initial state of the
   * system.
   * Truncation calls with with a new horizon and updated set of Candidates to
   * represent multiple Candidates.
   * \param horizon the horizon time step
   * \param candidateCount the number of Candidates
   * \return a pointer to the newly-created State_variable
   */
  State_variable *create_state_var(Theory::time_step_t horizon,
				   unsigned candidateCount);

  
  /**************************************************
   *    Truncation support
   **************************************************/

  /// Identify what function to map over all Observables at all time steps
  enum map_command {
    /// Save the values of all Observables
    save,
    /// Unassign the values of all Observables
    unassign,
    /// Restore the values of all Observables
    restore };

  /**
   * Map the specified operation over all Observables at all time steps
   * \param command the operation to map (save, unassign or restore)
   */
  void map_over_observations(map_command command);

  /**
   * Truncate the last time step if the current size of the system is equal to
   * the history length passed at creation time.
   * This is called automatically by the progress methods.
   */
  void truncate_if_needed() {
    if (history_full()) { truncate(get_horizon()); }
  }

  /**
   * Truncate the theory before time 'horizon' such that the first time step
   * we're aware of is now at time horizon.
   * \param horizon the new horizon
   */
  void truncate(Theory::time_step_t horizon);

  /**
   * A Candidate is being turned into a value of the State Variable.
   * Its index in the list of Candidates becomes a State Variable value.
   * For each Transitioned (mode) Variable, create a Clause that constrains
   * its value at the horizon time step for this value of the State Variable.
   * If the Candidate index is i, the Clause is of the form:
   * state=si & -#modeTransition=nominal => horizon#mode=m
   * \param horizon the time step before which Variables are summarized
   * \param stateVariableValueIndex the i in state=si
   */
  void summarize_variables(Theory::time_step_t horizon,
			   unsigned stateVariableValueIndex);

#ifdef ENABLE_SUMMARIZE_CONFLICTS
  /**
   * For truncation: summarize the Conflicts
   * \param horizon the time step before which Conflicts are summarized
   * \param stateVariableValueIndex the i in state=si
   */
  void summarize_conflicts(Theory::time_step_t horizon);
#endif

  /**
   * Checks candidates for inconsistency and drops any that are inconsistent.
   * This should be called after progress() and after Observable Variables
   * have been Assigned. This is called automatically by prune_and_search()
   * but is public in case the user wants to control the two steps.
   */
  void prune_inconsistent_candidates();
  
  /**
   * Remove part of the theory that we no longer need.
   * This destroys all Variables at time < horizon. Used by truncate_history().
   * \param horizon the time step before which Variables are removed
   */
  void remove_old_variables(Theory::time_step_t horizon);

  /**
   * Increment the time step.
   * This does not modify the history. The reason for this is even if the
   * history buffer is full, we don't need to dump history until we progress
   * again. We don't want to throw out information before we are absolutely
   * forced to.
   * \return the new time step
   */
  unsigned int next_timestep() { return ++timestep; }


  // Map the State_variable values to the Trajectories they represent.
  // A Trajectory is an array of strings which corresponds to a truncated
  // Assignment.

  /**
   * Return the ith Trajectory of the State_variable.
   * Throw an exception if i is out of range.
   * \param i the index of the Trajectory
   * \return a reference to the ith Trajectory
   */
  const Trajectory& get_trajectory(unsigned) const;


  /**
   * Upon truncation, the Trajectory mapping has to be rebuilt.
   * This rings out the old and rings in the new by deleting the old Trajectory
   * map, making the current Trajectory map the old one; allocating a blank
   * current Trajectory map; and replacing the State Variable's domain if it
   * has changed cardinality.
   * \param the new Trajectory count.
   */
  void start_new_trajectories_map(unsigned size);
  
  /**
   * Re-create the oldith pre-truncation Trajectory as the ith post-truncation
   * Trajectory.
   * The new Trajectory gets all members of assignments, and those of the 
   * oldith Trajectory are prepended. Throw an exception if either newi or
   * oldi is out of range.
   * \param newi index of the current Trajectory
   * \param oldi index of the previous Trajectory
   * \param oldAssignments Assignments of the old Trajectory
   */
  void set_trajectory(unsigned newi,
		      unsigned oldi,
		      Slist<const Assignment*>& assignments);

  /**
   * The Candidate is being truncated. Take all of its Assignments and
   * combine them with the Assignments stored in the existing Trajectory for
   * this Candidate. Then make a new Trajectory for the valueIndex.
   */

  void put_candidate_in_trajectory(Theory::time_step_t horizon,
				   unsigned value,
				   const Candidate& candidate);


  /**
   * Delete all Conflicts that occurred in the current time step
   */

  void deleteNewestConflicts();

  /** Return a pointer to the Tracker object. It can be null. */
  Tracker* getTracker() const { return pTracker; }

  /** Set the pointer to the Tracker object. It can be null. */
  void setTracker(Tracker* pTracker) { this->pTracker = pTracker; }
  

protected:
  /************************************************************
   *    Protected types.
   ************************************************************/

  /**
   * Recorda a newly-instantiated Variable as one of the Variables that must be
   * copied at each time step if we are to have a full model for each time
   * step.
   * Call this once upon creating a new Variable to add it to the set of
   * present Variables. Its offset is its ID, so IDs must be unique for
   * present Variables.
   * \param pVariable pointer to a newly-created variable
   */
  void register_new_variable(Variable* pVariable);

  /**
   * Inserts a new Variable, representing a new time step that has just passed,
   * between the present Variable and the Variables that previously supported
   * its value. The value of each Transitioned Variable in the present time
   * step is inferred based upon Assignments in the previous state and some
   * Clauses in the transition model. When we wish to move time forward one
   * step, we insert a new Variable, representing the most recent previous
   * time step, between the present Variable and the old Variables that
   * supported it. All of the Clauses that caused the present Variable to have
   * its value now cause the new most recent past Variable to have the same
   * value. We then add constraints based on the Command that cause the
   * present variable to take on a new value.
   * \param present_var the "now" Variable
   * \param new_past_var the newly-created past version of the "now" Variable
   */
  void splice(Transitioned& present_var, Dependent& new_past_var);

  /**
   * Installs the new state by adding the new Clauses to the theory and the
   * new Conflicts to the database.
   */
  void install_new_state();

private:
  
  /**
   * Create and return a Past_command Variable.
   * \param domainCardinality the cardinality of the domain
   * \param time_step the time step of the Past_command Variable
   * \param costs an Slist of respective cost/rank/weight values
   * \param id the ID of the Past_command Variable
   * \return a pointer to the newly-created Past_command Variable
   */
  Past_command *create_past_command(unsigned domainCardinality,
				    const Assignment::ListOfWeight& costs,
				    Theory::time_step_t timeStep,
				    unsigned ID);
 
  /**
   * Create and return a State_variable. A State_variable is also an
   * Assumption, which in turn is an Assignable.
   * \param domainCardinality the cardinality of the domain
   * \param time_step the time step of the State_variable
   * \param id the ID of the State_variable
   * \return a pointer to the newly-created State_variable
   */
  State_variable *create_state_variable(unsigned domainCardinality,
					const Assignment::ListOfWeight& costs,
					Theory::time_step_t time_step,
					unsigned id);

  /**
   * Create and return a Transition Variable.
   * \param domainCardinality the cardinality of the domain
   * \param costs an Slist of respective Transition cost/rank/weight values
   * \param time_step the time step of the Transition Variable
   * \param id the ID of the Transition Variable
   * \return a pointer to the newly-created Transition Variable
   */
  Transition *create_transition(unsigned domainCardinality,
				const Assignment::ListOfWeight& costs,
				Theory::time_step_t timeStep,
				unsigned ID);

  /**
   * Given arrays of assignments arranged in CNF, create a clause.
   * \param positive Array of (Variable*, valueIndex) pairs for each positive
   * Variable=value Proposition
   * \param negative Array of (Variable*, valueIndex) pairs for each negative
   * Variable=value Proposition
   * \param is_volatile whether the Clause is volatile
   * \param in_theory whether the Clause is placed in the theory
   * \param get_previous whether to use the predecessor Variable in the
   * Propositions
   * \return a pointer to the newly-created Clause
   */
  Clause *do_create_constraint(const Proto_assignment_list & positive,
			       const Proto_assignment_list & negative,
			       bool  is_volatile,
			       bool  in_theory,
			       bool  get_previous);

  /**
   * Create a Dependent of the same domain and similar name to the now_var,
   * to represent the now_var at the most recent past time step.  This also
   * adds the new variable to the list of Variables for the time step.
   * This method is used for dependent Variables, including Transitioned vars.
   * \param now_var the Dependent Variable at the NOW time step
   * \return the copy of the Dependent Variable at the current time step
   */
  Dependent* copy_in_past(Dependent& now_var);

  /**
   * Create an Assignable of the same domain and similar name to the now_var,
   * to represent the now_var at the most recent past time step.  This also
   * adds the new Variable to the list of Variables for the time step.
   * This method is used for Assignable vars when the subtype is unknown.
   * \param now_var the Assignable Variable at the NOW time step
   * \return the copy of the Assignable Variable at the current time step
   */
  Assignable* copy_in_past(Assignable& now_var);

  /**
   * Create a Command of the same domain and similar name to the now_var,
   * to represent the now_var at the most recent past time step.  This also
   * adds the new Variable to the list of Variables for the time step.
   * This method is used for Command Variables.
   * \param now_var the Command Variable at the NOW time step
   * \return the copy of the Command Variable at the current time step
   */
  Command* copy_in_past(Command& now_var);

  /**
   * Create an Observable of the same domain and similar name to the now_var,
   * to represent the now_var at the most recent past time step.  This also
   * adds the new Variable to the list of Variables for the time step.
   * This method is used for Observable Variables.
   * \param now_var the Observable Variable at the NOW time step
   * \return the copy of the Observable Variable at the current time step
   */
  Observable* copy_in_past(Observable& now_var);

#ifdef ENABLE_SUMMARIZE_CONFLICTS
  /**
   * Can the Trajectory summarize the Conflict's pre-horizon Assignments?
   * This is so if
   * (1) each non-nominal Assignment in the prefix exists in the Trajectory and
   * (2) each nominal Assignment in the prefix is not contradicted by a
   * different Assignment to the same Variable.
   * \param trajectory the Trajectory
   * \param beforeHorizon for some Conflict, the set of Assignments before the
   * horizon
   * \param stateVariableValueIndex the value of the State Variable from the
   * Conflict, or (unsigned)-1 if the Conflict didn't contain it
   * \return whether the Trajectory can replace the truncatablePrefix in the
   * Conflict
   */
  bool summarizes(const Trajectory& trajectory,
		  const Assignment::ListOfAssignment& beforeHorizon,
		  unsigned stateVariableValueIndex) const;

  /**
   * The Trajectory "covers" the non-nominal Assignment if one of its Parts
   * elements represents that Assignment.
   * \param trajectory the Trajectory
   * \param assignment the Assignment
   * \return whether one of the Trajectory's Parts elements is for the same
   * value of the Assignment's Variable
   */
  bool covers(const Trajectory& trajectory,
	      const Assignment& assignment) const;

  /**
   * The Trajectory "is consistent with" the nominal Assignment if none of its
   * Parts elements represents a different Assignment to the same Variable
   * \param trajectory the Trajectory
   * \param assignment the Assignment
   * \return whether none of the Trajectory's Parts elements is for a
   * different value of the Assignment Variable
   */
  bool consistentWith(const Trajectory& trajectory,
		      const Assignment& assignment) const;

  /**
   * Create Conflict(s) in which a value of the State Variable replaces the
   * Assignments in the truncatablePrefix
   * \param beforeHorizon the Assignments with time step < horizon
   * (== horizon-1)
   * \param onOrAfterHorizon the Assignments with time step >= horizon
   */
  void summarize_conflict(const Assignment::ListOfAssignment& beforeHorizon,
			  const Assignment::ListOfAssignment& onOrAfterHorizon,
			  unsigned stateVariableValueIndex);
#endif			  
  
#ifndef DISABLE_FULL_PROGRESS
  /**
   * Auxiliary to full_progress()
   * \param transitions an Slist of pointers to Transition Variables created
   * \param pCommandAssignment pointer to the value assignment for a Command
   * Variable; idle if null
   */
  void full_progress_copy_variables(const Assignment *pCommandAssignment =
				    NULL);
  /**
   * Auxiliary to full_progress()
   */
  void full_progress_create_constraints();

  /**
   * Auxiliary to full_progress()
   * \param pCommandAssignment pointer to the value assignment for a Command
   * Variable; idle if null
   */
  void full_progress_make_assignments(const Assignment* pCommandAssignment);
#endif

#ifdef ENABLE_RECOVERY
  /**
   * Creates an Assumption Variable to represent the current Command in a past
   * time step.
   * When planning, the past Commands are assumptions. The current Command is
   * useful for determining the domain of the past Commands, but is otherwise
   * useless.
   * \param now_cmd the current Command
   * \return a pointer to the newly-created Past_command
   */
  Past_command* create_cmd_assumption_in_past(Command& now_cmd);
#endif

 private:
#if defined(ENABLE_RECOVERY) || !defined(DISABLE_FULL_PROGRESS)
  // Each member represents one state constraint in the state model. We will
  // use these to quickly instantiate a new copy of the state model when
  // desired. Note that min-progress doesn't need this, but recovery and
  // full-progress do.
  class Proto_constraint;
  friend class Proto_constraint;
  Slist<Proto_constraint*> state_constraint_prototypes;
#endif

  /**
   * Used to decide which Variables to destroy.
   * This is a bit of a mess. Declared here so we can be a friend to it.
   */
  class Old_variable_acceptor;
  friend class Old_variable_acceptor;

  /**
   * Record all the Variables at the NOW timestep, with the exception of the
   * modeTransitions.
   * These are the Variables that the modeler thinks about. We keep them to be
   * able to have constant-time lookups by ID.
   */
  Array<Variable*, false> present_variables;

  /**
   * The current time step.
   * If we run for more than MAXINT time steps, we will need to figure out how
   * to wrap this.
   */
  unsigned int timestep;

  /**
   * The earliest timestep currently represented in the system.
   * This is computed when we truncate the history.  See truncate()
   */
  unsigned int oldest_timestep;

  /**
   * How large our history can be.
   * For flight, a history array will have to be pre-allocated to a certain
   * size. For now we allow a history size to be specified in the constructor
   * and we manage the history dynamically.
   */
  unsigned int max_history_length;

  ////////////////////////////////////////////////////////
  /// Start state functionality.
  ////////////////////////////////////////////////////////

  /**
   * The Variable that will support the initial values of the state Variables
   * when we instantiate the transition system, and will support values at the
   * time horizon if we truncate the Variables before the horizon.
   * A new one is created at every truncate.
   */
  State_variable* state_var;

  /**
   * Holds new, deleted clauses while we are transitioning from one one start
   * state to another when truncating the history.
   * There's no problem with adding these immediately except that we'd then
   * be propagating more clauses than we need: it's for speed. This list holds
   * the new ones until the old ones are deleted.
   */
  Clause::ListOfClause new_clauses;

  /**
   * Holds new conflicts to add to the Conflict_db at the end of truncate.
   * We don't check whether the lists subsume each other, but instead use
   * Conflict_db::add_conflict() to do that. As a heuristic, we use an Array
   * since that makes it easier to go through the Conflicts in size order (add
   * small Conflicts first, which are more likely to subsume the large
   * Conflicts later).
   */
  Array<Assignment::ListOfAssignment, true> new_conflicts;

#ifndef DISABLE_T_SYS_LISTEN
  /// The set of listeners
  Slist<T_system_listener*> listeners_;
#endif

  /// The set of Trajectories, implemented as an array.
  Trajectory* state_var_trajectories;

  /// The current Trajectory count.
  unsigned n_trajectories;
  
  /// The set of Trajectories before truncation, implemented as an array.
  Trajectory* old_trajectories;

  /// The Trajectory count before truncation.
  unsigned n_old_trajectories;

  /// Whether the T_system is being used for planning
  bool planning;

  /**
   * The maximum number of Candidates to carry through a truncate.
   * 0 means to always carry all of them through.
   */
  unsigned max_truncated_candidates;

#if !defined(DISABLE_FULL_PROGRESS) && !defined(DISABLE_MIN_PROGRESS)
  /**
   * Does progress() call full-progress()?
   * If not, it calls minimal-progress(). Needed only if we can do both.
   * \return whether the progress style if full progress
   */
  bool progress_uses_full;
#endif

  /**
   * A pointer to the Tracker. Since the Tracker object is created after the
   * T_system object is, it can't be a reference.
   */
  Tracker* pTracker;
};

#endif // T_SYSTEM_H
