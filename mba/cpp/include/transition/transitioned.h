/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Transition system representation, NASA Ames Research Center
// $Id: transitioned.h,v 1.79 2003/05/14 15:49:21 lbrown Exp $

#ifndef TRANSITIONED_H
#define TRANSITIONED_H

#include <transition/T_system.h>

class Transition;

/**
 * This class represents a Variable in the current state whose value is
 * inferred from its value in the previous state and the outcome of a
 * Transition.
 * In effect, it is the mode of operation of a component. It is a subclass of
 * the Dependent Variable of the Conflict_db.
 */

class Transitioned : public Dependent {
  friend class T_system;
  friend class T_system_debug;

public:

  enum public_constants { ANY_MODE = -1 };

  /**
   * Establish the cost of the nominal Transition (0) and create a horizon
   * (oldest) Transition, which is initially the same as the zenith (most
   * recent) Transition.
   * Save a back pointer to the T_system. The transitionCount is the number of
   * Transition entities that affect this mode.
   * \param t_system the T_system that is governs the Transitions
   * \param nvalues the mode's domain cardinality
   * \param id the Variable ID
   * \param transitionCount the Transition's domain cardinality
   * \param timeStep the time step for the mode
   */
  Transitioned(T_system& t_system,
	       unsigned nvalues,
	       unsigned id,
	       unsigned transitionCount,
	       unsigned timeStep);

  /// Deallocate tables: indexed Transition, unindexed Transition, autonomous
  /// Transition. Also erase pointers to the clauses and the set of costs.

  virtual ~Transitioned();

  // **************************************************************************
  // The following are for transitioning a Transitioned Variable to the next
  // time step
  // **************************************************************************

  /**
   * Perform a commanded Transition (a progress).
   * Create a new mode Variable to represent this mode Variable one time step
   * ago, thus moving this mode Variable one time step forward upon receipt of
   * the Command. The zenith Transition Variable is returned.
   * \param commandAssignment the Assignment cmdIn=c for the Command
   * \param timestep the current time step
   * \return the newly-created Transition Variable
   */
  const Transition* transition(const Assignment& commandAssignment,
			       unsigned timestep);

  /**
   * Perform an idle Transition.
   * Create a new mode Variable to represent this mode Variable one time step
   * ago, thus moving this mode Variable one time step forward if no Command
   * is given. The zenith Transition Variable is returned.
   * \param timestep the current time step
   * \return the newly-created Transition Variable
   */
  const Transition* idle(unsigned timestep);

  /**
   * Ignore the Transitions and force the Variable to take on the given value.
   * This will continually add new Clauses to transition_clauses and thus
   * should be called only rarely. We need access to the LTMS to manage this.
   */
  void set_mode(LTMS* pLTMS, unsigned value);

  /**
   * Does the Transitioned Variable have at least one Transition that is
   * indexed by the Command Assignment?
   * \param commandAssignment the Assignment cmdIn=c for the Command
   * \return whether any Proto_transitions are stored for this Command
   * Assignment
   */
  bool has_indexed_transitions(const Assignment& commandAssignment) {
    return (find_transition_prototypes(commandAssignment) != NULL);
  }

  /**
   * Constrain the initial value of the mode variable.
   * The constraint is
   * state=si & -#modeTransition=nominal ==> horizon#mode=value
   * Note that "initial" here means at the horizon, the start of the window of
   * time which is currently in the engine.
   * \param stateVariableValueIndex the value of the State Variable
   * \param modeValueIndex the value index of the initial mode; if negative,
   * discover it (the value of the oldest Transitioned Variable on or after
   * the horizon)
   * \param horizon the horizon time step
   * \param in_theory whether to add to the prositional theory now
   * \return a pointer to the newly-created Clause or NULL if the initial
   * value was not supplied but could not be discovered
   */

  Clause* set_initial_mode(unsigned stateVariableValueIndex,
			   int modeValueIndex,
			   Theory::time_step_t horizon,
			   bool in_theory);

  // **************************************************************************
  // The following are for reading in Transitions
  // **************************************************************************

  /**
   * Add a template for a commanded (indexed) Transition upon reading in the
   * model.
   * The Transition will contain
   * !t#mode=m1 !t#cmdIn=command !t#modeTransition=nominal (t+1)#mode=m2
   * \param transition_value value index of the Transition
   * \param from_value value index of the "from" mode or -1 for wild card
   * \param to_value value index of the "to" mode
   * \param pos list of positive antecedants
   * \param neg list of negative antecedants
   */
  void add_commanded_prototype(unsigned transition_value,
			       int from_value,
			       unsigned to_value,
			       const Slist<T_system::Proto_assignment>& pos,
			       const Slist<T_system::Proto_assignment>& neg);

  /**
   * Add a template for a failure (autonomous) Transition upon reading in the
   * model.
   * There is no "from" mode because an autonomous (failure) Transition is the
   * same for all "from" modes. There are no antecedents (guards) because 
   * failure Transitions can not be constrained.
   * The Transition will be
   * !t#modeTransition=x (t+1)#mode=m
   * \param transition_value value index of the Transition
   * \param to_value value index of the "to" mode
   */
  void add_failure_prototype(unsigned transition_value,
			     unsigned int to_value);

  /**
   * Add a template for an idle (unindexed) Transition upon reading in the
   * model.
   * This is a frame axiom; it asserts that, in the absence of a Command, a
   * nominal Transition keeps the mode constant. Thus, the "from" mode is the
   * same as the "to" mode, and there are no antecedents (guards).
   * \param to_value value index of the "from" mode and "to" mode
   */
  void add_idle_prototype(unsigned from_and_to_value) {
    static const Slist<T_system::Proto_assignment> empty;
    add_commanded_prototype(0,
			    from_and_to_value,
			    from_and_to_value,
			    empty,
			    empty);
  }

  /// Return the oldest Transition that affected this mode.
  Transition *get_horizon() { return horizon_transition; }

  /// Return the most recent Transition that affected this mode.
  Transition *get_zenith()  { return zenith_transition;  }

  /// Add the transition cost as an order of magnitude.
  void add_transition_cost(Assignment::weight_t cost);

  /**
   * Record the Clauses from Transitions that are constraining the value of
   * this Variable.  This is used when progressing the system and by the
   * reader, in order to enforce the initial state constraints.
   */
  void add_transition_clause(Clause *pClause) {
    // Tell the Clause that it is a transition Clause; used in checkpointing
    pClause->set_transition(true);
    transition_clauses.push_back(pClause);
#ifdef ENABLE_CHECKPOINT
    all_transition_clauses.push_back(pClause);
#endif
  }

  /// Return the number of Transitions that affect this mode.
  unsigned get_ntransitions() const { return ntransitions; }

  /// Remove things being destroyed.
  void remove_old_modeTransition(Theory::time_step_t horizon);
  void remove_destroyed_clauses();

  /// Is this Variable a Transitioned? Yes, it is.
  virtual bool is_transitioned() const { return true; }

#ifdef ENABLE_CHECKPOINT
  /**
   * Remove from the theory all elements of all_transition_clauses
   * \param pLTMS pointer to the LTMS from which the Clauses are to be removed
   * \todo This is not called; remove it
   */
  void remove_all_transition_clauses(LTMS *pLTMS);
#endif

  /**
   * A variant of add_idle_transitions(void) used in restoring checkpoints.
   * Called by L2_checkpoint::data::restore_unindexed_clauses()
   * \param fromTimeStep the antecedent time step
   * \param transition the Transition that appears in the Clauses
   */
  void add_idle_transitions(unsigned fromTimeStep,
			    const Transition& transition);

  /**
   * A variant of instantiate_clauses(...) used in restoring checkpoints.
   * Called by L2_checkpoint::data::restore_indexed_clauses()
   * \param commandVariableID the variableID of the Command
   * \param commandValueIndex the index of the Command's value
   * \param fromTimeStep the antecedent time step
   * \param toTimeStep the consequent time step
   */
  void add_commanded_transitions(unsigned commandVariableID,
				 unsigned commandValueIndex,
				 unsigned fromValueIndex,
				 unsigned toTimeStep);

  /**
   * Remove the Clause pointer from the list of Transition Clause pointers.
   * \param pClause a pointer to the Clause to remove
   */
  void remove_transition_clause(Clause* pClause) {
    transition_clauses.remove(pClause);
#ifdef ENABLE_CHECKPOINT
    all_transition_clauses.remove(pClause);
#endif
  }

#ifdef ENABLE_CHECKPOINT
  /// Start of list of all Transition Clauses.
  Clause::ListOfClause::iterator clauses_begin() {
    return all_transition_clauses.begin();
  }

  /// End of list of all Transition Clauses.
  Clause::ListOfClause::iterator clauses_end() {
    return all_transition_clauses.end();
  }
#endif

protected:

  /**
   * That portion of transition() that instantiates Clauses
   * \param commandAssignment the Assignment cmdIn=c for the Command
   * \param toTimeStep the consequent time step
   */
  void instantiate_clauses(const Assignment& commandAssignment,
			   unsigned toTimeStep);

  /**
   * Currently we do not explictly store the idle Transitions.
   * This method creates all the Clauses we need to add when we are not
   * commanding a Variable, but need to Transition it because Transitions that
   * are being commanded depend upon it. It forms a set of Clauses of the form
   * modeTransition=nominal & t#mode=m => t+1#mode=m
   */
  void add_idle_transitions();

  /**
   * Remove from the theory all elements of transition_clauses
   * \param pLTMS pointer to the LTMS from which the Transition Clauses are to
   * be removed
   */
  void remove_transition_clauses(LTMS *pLTMS);

#ifdef ENABLE_RECOVERY
  void planner_transition(int timestep);
  void instantiate_all_indexed_clauses(unsigned t = T_system::NOW);
#endif

  /**
   * This inner class represents a Transition Clause. It's called a
   * "proto-transition" because its antecedents can be instantiated for any
   * timestep.
   */
  class Proto_transition : public Pooled {
    friend class T_system_debug;

  public:

    /**
     * Construct a prototype for a failure (autonomous) Transition.
     * There are no antecedents and no "from" value (implicitly any mode).
     * \param transitionValueIndex index of the Transition value
     * \param transitioned the mode Variable being transitioned
     * \param toModeValueIndex index of the mode Variable being transitioned to
     */
    Proto_transition(unsigned transitionValueIndex,
		     Transitioned& transitioned,
		     unsigned toModeValueIndex) :
      consequent(&transitioned, toModeValueIndex),
      command_assignment(NULL),
      mode_var(transitioned),
      transition_value(transitionValueIndex) {
      // No antecedent to set
      // No Command assignment to set
    }

    /**
     * Construct a prototype for a nominal Transition.
     * There are antecedents as well as a "from" value.
     * If fromModeIndex is a value index, add literal !t#mode=m.
     * If one of the literals is for a Command, store the Command's Assignment
     * in a data member.
     * \param transitionValueIndex index of the Transition value
     * \param transitioned the mode Variable being transitioned
     * \param toModeValueIndex index of the mode Variable being transitioned to
     * \param pos list of positive literal antecedents
     * \param neg list of negative literal antecedents
     * \param fromModeIndex index of the mode Variable being transitioned from
     */
    Proto_transition(unsigned transitionValueIndex,
		     Transitioned &transitioned,
		     unsigned toModeValueIndex,
		     const Slist<T_system::Proto_assignment>& pos,
		     const Slist<T_system::Proto_assignment>& neg,
		     int fromModeIndex);

    /// No subclasses ==> non-virtual 
    ~Proto_transition() { }

    /**
     * Instantiate the Transition.
     * Antecedents (if any) are from the time step before the argument; the
     * consequent is from the time step passed in (or nearest thereafter).
     * The modeTransition is the zenith if constraining the current mode,
     * the horizon if constraining the initial mode. Return the Clause.
     * \param t_system the T_system that will receive the Clause
     * \param fromTimeStep the antecedent time step
     * \param toTimeStep the consequent time step
     * \todo this does not change the Proto_transition, so should be made const
     */
    Clause* instantiate(T_system& t_system,
			unsigned fromTimeStep,
			unsigned toTimeStep);

    /**
     * Return the Assignment from a guarded (i.e., non-failure)
     * Proto_transition that is the Command Assignment.
     * This will be a negative antecedent in the Transition:
     * !t#mode=m1 !t#cmdIn=c !t#modeTransition=nominal (t+1)#mode=m2
     * If the guard does not mention a Command, then NULL is returned.
     */
    const Assignment* get_command_assignment() const {
      return command_assignment;
    }

    /// Iterator over sets of antecedents, whether positive or negative
    typedef Slist<T_system::Proto_assignment>::iterator iterator;

    /// Begin iterator for positive antecedents.
    iterator begin_positive() { return positives.begin(); }

    /// End iterator for positive antecedents.
    iterator end_positive()   { return positives.end();   }

    /// Begin iterator for negative antecedents.
    iterator begin_negative() { return negatives.begin(); }

    /// End iterator for negative antecedents.
    iterator end_negative()   { return negatives.end();   }

  private:
    /**
     * Helper for the constructors.
     * If one of the literals is for a Command, return it
     * \return a pointer to the Assignment object corresponding to a literal in
     * the constraint
     */
    Assignment* findCommandAssignment();

    /// Return Proposition at time strictly less than timestep (for antecedent)
    static Proposition* prop_closest_before(const T_system::Proto_assignment&,
					    unsigned);

    /// Return Propositon at time greater or equal to timestep (for consequent)
    static Proposition* prop_closest_after(const T_system::Proto_assignment&,
					   unsigned);

    /**
     * Look up and return by reference the Propositions for the Transitioned's
     * Proto_assignments, separated by positive and negative, for the most
     * recent time step earlier than that specified.
     * \param positive the positive literals from the Proto_assignment an
     * out parameter
     * \param negative the negative literals from the Proto_assignment; an
     * out parameter
     * \todo this does not change the Proto_transition, so should be made const
     */

    void set_antecedants(Proposition::ListOfProposition& positive,
			 Proposition::ListOfProposition& negative,
			 unsigned time_step);

    // All antecedents are expected to be negative, making the Clauses Horn
    // clauses.
    // That is,
    // !antecedent1 ||... || !antecentN || consequent
    // is equivalent to
    // !(antecedent1 && ... && antecedentN) || consequent
    // is equivalent to
    // (antecedent1 && ... && antecedentN) => consequent

    /**
     * Positive antecedent Proto_assignments representing the Assignments
     * without respect to a timepoint.
     * When we create a Transition from this prototype, we will instantiate it
     * for a particular time step.
     */
    Slist<T_system::Proto_assignment> positives;

    /**
     * Negative antecedent Proto_assignments representing the Assignments
     * without respect to a timepoint.
     * When we create a Transition from this prototype, we will instantiate it
     * for a particular time step.
     */

    Slist<T_system::Proto_assignment> negatives;

    /**
     * If all the Propositions (positive and negative, and the transition) are
     * correctly set, this is the consequence.
     */
    const T_system::Proto_assignment consequent;

    // If we have a Command in our guard Clause, we store it; this allows for a
    // faster lookup of the effects of a Command.
    const Assignment* const command_assignment;

    // Reference to the mode Variable to which this Transition corresponds.
    Transitioned& mode_var;

    /// The value of the Transition. This is cached from the zenith Transition.
    const unsigned transition_value;
  };

  /**
   * An element of a mapping from what Command was given to a list of
   * Transition Clauses that might be enabled.
   */
  typedef MBApair<const Assignment*, Slist<Proto_transition*> > Entry_type;

  /**
   * The mapping from what command was given to a list of Transition Clauses
   * that might be enabled.
   * We could have used a hash table, but we will have an average of about 3-4
   * entries per table.
   */
  typedef  Slist<Entry_type> Table_type;

  /// Holds prototype Transitions indexed by the Command that invokes them.
  Table_type indexed_transition_table;

  /**
   * This is for non-autonomous Transitions that do not include an Assignment
   * to a Command Variable.
   * These include Transitions that are guarded by a value that is inferred
   * from a Command (e.g., if a Command is given to a bus, which transmits it
   * to a device). They also include frame axioms that say what happens if no
   * Command is given; that is, for idle Transitions.
   */
  Slist<Proto_transition*> unindexed_prototypes;

  /**
   * Holds a list of autonomous Transitions that are always enabled.
   * Currently the only autonomous Transitions we consider are failures, but
   * recoveries could be autonomous as well.
   */
  Slist<Proto_transition*> autonomous_transitions;

  /**
   * The Transition's Clauses that are currently being used to constrain this
   * mode Variable.  This will include Clauses from unindexed prototypes,
   * autonomous Transitions, and whatever Transitions from the table were
   * indexed by the last Command.
   */
  Clause::ListOfClause transition_clauses;

#ifdef ENABLE_CHECKPOINT 
  /** 
   * A list of all currently active Transition Clauses w.r.t. some timestep of
   * this Transitioned Variable.  This list is used to erase all Transition
   * Clauses before re-generating fresh ones upon checkpoint restoration.
   */
  Clause::ListOfClause all_transition_clauses;
#endif

  /**
   * Have we added any autonomous Transitions that constrain the
   * current-timestep mode from the pre-horizon modeTransition?
   */
  bool now_has_failure_from_horizon;

private:
  /**
   * Instantiate the commanded (indexed) Clauses that constrain the mode from
   * the previous historical time step to now.
   * \param commandAssignment the Assignment cmdIn=c for the Command
   * \param fromTimeStep the antecedent time step
   * \param toTimeStep the consequent time step
   */
  void instantiate_indexed_clauses(const Assignment& commandAssignment,
				   unsigned fromTimeStep,
				   unsigned toTimeStep = T_system::NOW);

  /**
   * Instantiate the failure (autonomous) Clauses that constrain the mode from
   * the previous historical time step to now.
   * \param time_step the current time step
   */
  void instantiate_autonomous_clauses(unsigned time_step = T_system::NOW);

  /**
   * Instantiate the idle (unindexed) Clauses that constrain the mode from
   * the previous historical time step to now.
   * \param fromTimeStep the antecedent time step
   * \param toTimeStep the consequent time step
   */
  void instantiate_unindexed_clauses(unsigned fromTimeStep,
				     unsigned toTimeStep = T_system::NOW);

  /**
   * Copy this NOW mode Variable into the past and create a Transition for it.
   * Move the Clauses that supported the NOW Variable so that they support the
   * newly-created past mode. Support function for idle() and transition().
   * \time_step the current time step
   */
  void create_transition_variable(unsigned time_step);

  /**
   * Allows us to look up Transitions that could be enabled by the Command
   * that was given.
   * \param commandAssignment the Assignment cmdIn=c to which the
   * Proto_transition Transition template is being attached
   * \param pProto_transition pointer to the Proto_transition that refers to
   * the Command Assignment
   */
  void add_transition_prototype_to_table(const Assignment& commandAssignment,
					 Proto_transition *pProto_transition);

  /**
   * Search list Transitioned::indexed_transition_table for a pair element, the
   * first element of which is a pointer to the Command Assignment. If found,
   * return it; otherwise return NULL.
   * \param commandAssignment the Assignment cmdIn=c for which the list of
   * Proto_transitions is being sought
   * \return a pointer to a list of Proto_transitions that refer to the Command
   * Assignment; if not found, return a NULL pointer rather than an empty list
   */
  Slist<Proto_transition*>*
    find_transition_prototypes(const Assignment& commandAssignment);

 private:

  /// Reference to the T_system that holds this Transitioned Variable.
  T_system& t_system;

  /**
   * Initial modes are constrained by a Clause set of the following type:
   * horizon#state=si and horizon#modeTransition=nominal ==> horizon#mode=x
   * This Variable represents horizon#modeTransition.  It does not change
   * throughout the execution of the program; instead of deleting
   * (horizon-1)#modeTransition and creating horizon#modeTransition, we just
   * keep this at time BEFORE_HORIZON.
   */
  Transition* horizon_transition;

  /**
   * We keep around the two ends of a list of Transitions.  The horizon is the
   * old end.  Clearly, the zenith is the opposite (newest) end.
   */
  Transition* zenith_transition;

  /// The costs of the Transition Variable Assignments
  Assignment::ListOfWeight transition_costs;

  /// The number of Transitions that affect this mode
  const unsigned ntransitions;
};

#endif /* TRANSITIONED_H */
