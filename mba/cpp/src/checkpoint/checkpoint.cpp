/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: checkpoint.cpp,v 1.48 2004/02/05 22:36:17 lbrown Exp $ */
#include <checkpoint/checkpoint.h>

#include <transition/tracker.h>
#include <transition/transitioned.h>
#include <api/livingstone.h>
#ifdef ENABLE_CHECKPOINT_SERIALIZE
#include <assert.h>
#include <parsers/SAXParser.hpp>
#include <sax/AttributeList.hpp>
#include <sax/HandlerBase.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>
#include <livingstone/L2_fstream.h>
#include <util/PlatformUtils.hpp>
#endif

/***************************************************************************
  Implementation.
 ***************************************************************************/
class L2_checkpoint::data : public Pooled {
  friend class L2_checkpoint::CheckpointHandler;
    public:
        data(Tracker *tracker) : valid(true) {
            tracker_ = tracker;
            store_times();
	    store_command_history();
            store_cmds_obs();
            store_state_var();
            store_candidates();
            store_conflicts();
        }

        virtual ~data() {
            // nothing to do; the arrays go away on their own
        }

        bool restore() {
            T_system *tsys = get_tsys();
            // check the engine is ok.
            if((tsys->get_time_step() - tsys->get_oldest_timestep())
	       !=ntimesteps)
                return false;
            // later: more sanity checks

            reset_times();
            restore_cmds_obs();
	    delete_outdated_constraints();
	    restore_command_constraints();
            restore_state_var();
            restore_candidates();
            restore_conflicts();
            if(!tsys->getCandidates().empty())
                get_tracker()->get_t_system().install(0);
            else {
                tsys->get_state_variable()->assign(0u);
                tsys->propagate();
            }
            return true;
        }

  // LSB begin
#ifdef ENABLE_CHECKPOINT_SERIALIZE
  /** Construct a data object from the contents of a checkpoint XML file */
  data(Tracker* tracker, const MBA_string& xmlCheckpointFilePath);

  /** Deserialize the checkpoint from the input stream */
  bool deserialize(const MBA_string& xmlCheckpointFilePath);

  /** Serialize the checkpoint to the file whose pathname is given */
  bool serialize(const MBA_string& xmlCheckpointFilePath);

  /** Serialize the checkpoint to the output stream */
  bool serialize(_STD_ ostream& os);

  bool isValid() const { return valid; }
#endif
  // LSB end

    private:

        Tracker *get_tracker() {
            return tracker_;
        }
        T_system *get_tsys() {
            return &get_tracker()->get_t_system();
        }

        /************************************************************
          Stuff for creating the checkpoint.
         ************************************************************/

  /**
   * Get the the Command variable at the time step whose value is not 0
   * (no command, for idle transitions). If there is none, return the null
   * pointer. The null pointer means either (1) there are no Commands for
   * that time step or (2) there are Commands for that time step, but all
   * of them have 0 values.
   */

  Command* command_at_time_step(unsigned timeStep) {
    T_system *tsys = get_tsys();
    // For each Command
    for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
      Variable *pVariable = tsys->get_present_variable(i);
      if (!pVariable->is_command()) continue;
      Command *pCommand = static_cast<Command *>(pVariable);
      // For each time step for the Command
      for (; pCommand != 0; pCommand =
	     static_cast<Command *>(pCommand->get_previous())) {
	if (pCommand->get_time_step() == timeStep &&
	    pCommand->get_value_index() != 0) {
	  // Found the command at this time step
	  return pCommand;
	}
      }
    }
    // No Command known at this time step
    return 0;
  }

        /**
         * For each time step in the history, save the Command Variable index
	 * and the value index. Ignore before T_system::get_oldest_timestep()
	 * because that is not known. Ignore at NOW, since it will always be
	 * NO_COMMAND.
	 */

        void store_command_history() {
	  T_system *tsys = get_tsys();
	  for (unsigned ts = tsys->get_oldest_timestep();
	       ts < tsys->get_time_step();
	       ts++) {
	    Command *pCommand = command_at_time_step(ts);
	    if (pCommand) {
	      // It was a commanded progress
	      int valueIndex = pCommand->get_value_index();
	      if (valueIndex == Variable::NOT_FOUND) {
		// The LTMS does not know the value; try the Assignable
		const Assignment* pAssignment = pCommand->get_current();
		if (pAssignment != NULL) {
		  valueIndex = static_cast<int>(pAssignment->get_index());
		}
	      }
	      MBApair<int, unsigned> p(pCommand->get_id(),
				       static_cast<unsigned>(valueIndex));
	      commandHistory.grow_and_set(ts - tsys->get_oldest_timestep(), p);
	    } else {
	      // It was an idle progress
	      MBApair<int, unsigned> p(0, 0);
	      commandHistory.grow_and_set(ts - tsys->get_oldest_timestep(), p);
	    }	      
	  }
	}


        /**
         * Store the initial time and number of timesteps.
         */
        void store_times() {
            T_system *tsys  = get_tsys();
            oldest_timestep = tsys->get_oldest_timestep();
            ntimesteps      = tsys->get_time_step()-tsys->get_oldest_timestep();
        }

        /**
         * Store the commands and observations that are assigned.
         * We make the assumption that the only assignables that
         * are neither commands nor observations are assumptions.
         */
        void store_cmds_obs() {
            T_system *tsys  = get_tsys();
            Assignable::ListOfAssignable::iterator it(tsys->begin_assignables());
            for( ; it != tsys->end_assignables(); ++it) {
                Assignable *a = *it;
                if( a->is_assumption() || !a->is_assigned()) continue;
                // store it
                CPAssignment *stored = new CPAssignment(a->get_id(),
                        a->get_time_step(), a->get_current()->get_index());
                cmds_obs_.push_back(stored);
            }
        }


        /**
         * Store the initial mode assignments.
         * To do so, we unassign all observations to avoid any conflicts ;
         * then assign the state variable to each of its values and look
         * at the values of the initial modes.
         */
        void store_state_var() {
            T_system *tsys  = get_tsys();

            {
	        for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
		    Variable *pVariable = tsys->get_present_variable(i);
		    if (!pVariable->is_observable()) continue;
		    Assignable *a = static_cast<Assignable *>(pVariable);
                    a->save();
                    a->unassign();
                }
            }

            // find the initial modes
            Array<Dependent*, false> initial_modes;
            {
	        for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
		    Variable *pVariable = tsys->get_present_variable(i);
		    if (!pVariable->is_transitioned()) continue;
		    Dependent *mode = static_cast<Dependent *>(pVariable);
                    while(mode->get_previous_dependent())
                        mode = mode->get_previous_dependent();
                    initial_modes.push_back(mode);
                }
            }

            // for each state assignment, store all the initial modes
            {
                Assignable *state = tsys->get_state_variable();
                state->save();

                for(unsigned i=0 ; i<state->get_nvalues(); ++i) {
                    state->assign(i);
                    // now look at all the mode variables
                    InitialModeAssignments *init = new InitialModeAssignments
                        (state->get_current()->get_weight());
                    Array<Dependent*, false>::iterator it =
		      initial_modes.begin();
                    for( ; it != initial_modes.end(); ++it) {
                        Dependent *d = *it;
                        // only put in ones that are known values
                        if(d->get_value_index() >= 0)
                            init->add_mode(d->get_id(), d->get_value_index());
                    }
                    initial_modes_.push_back(init);
                }
                // put the state back where it was
                state->restore();
            }


            {
		for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
		    Variable *pVariable = tsys->get_present_variable(i);
		    if (!pVariable->is_observable()) continue;
		    Assignable *a = static_cast<Assignable *>(pVariable);
                    a->restore();
                }
            }
	    // The Assignable::save() and Assignable::restore() actions
	    // require propagation
	    tsys->propagate();
        }


        /**
         * Store all the candidates.  Foreach candidate, figure out the
         * state=si assignment and add the failures.
         */
        void store_candidates() {
            Tracker *tracker = get_tracker();
            T_system *tsys   = get_tsys();
            Variable *svar   = tsys->get_state_variable();

            Tracker::iterator it =
	      tracker->get_t_system().getCandidates().begin();
            for( ; it != tracker->get_t_system().getCandidates().end(); ++it) {
                CPCandidate *cand=0; // into which we'll store our results
                ::Candidate *c = *it; // from which we're reading
                // find its state=si assignment ; later, the Candidate
                // structure will be redone to have it easy to find
                unsigned i;
                for(i=0; i<c->size(); ++i) {
                    const ::Assignment *a = c->get_assignment(i);
		    const Variable *pVariable = a->get_variable();
                    if(pVariable == svar) {
                        cand = new CPCandidate(a->get_index());
                        break;
                    }
                }
                // if we found none, what to do?
                if(!cand) {
                    assert(c->size()==0);
                    // empty candidate, state=s0
                    cand = new CPCandidate(unsigned(0));
                }

                // now, loop over the failures and add them all
                // (skipping the state variable)
                for(i=0; i<c->size(); ++i) {
                    const ::Assignment *a = c->get_assignment(i);
                    const ::Variable *var = a->get_variable();
                    if(var == svar) continue; // skip it
                    cand->add_failure(var->get_id(), var->get_time_step(),
                            a->get_index());
                }

                // done; add the candidate to the list
                candidates_.push_back(cand);
            }
        }


        void store_conflicts() {
            T_system *tsys = get_tsys();

            Conflict::ListOfConflict::iterator it;
            for (it = tsys->begin_conflicts();
		 it != tsys->end_conflicts(); ++it) {
                CPConflict *conf = new CPConflict;
                const ::Conflict *c = *it;
                for (::Conflict::const_iterator ait = c->begin();
		    ait != c->end(); ++ait) {
                    const ::Assignment * const a = *ait;
                    conf->add_assign(a->get_variable()->get_id(),
                            a->get_variable()->get_time_step(),
                            a->get_index());
                }
                conflicts_.push_back(conf);
            }
        }

  // LSB begin
  class CPAssignment;
  class InitialModeAssignments;
  class InitialValue;
  class CPCandidate;
  class CPConflict;
  void indent(_STD_ ostream& os);
  void serializeTimes(_STD_ ostream& os);
  void serializeAssignment(_STD_ ostream& os, const CPAssignment& assignment);
  void serializeCommandsAndObservations(_STD_ ostream& os);
  void serializeStateVariables(_STD_ ostream& os);
  void serializeInitialModeAssignments(_STD_ ostream& os,
    const InitialModeAssignments& initialModeAssignments);
  void serializeInitialValue(_STD_ ostream& os,
    const InitialValue& initialValue);
  void serializeCandidates(_STD_ ostream& os);
  void serializeCandidate(_STD_ ostream& os,
    const CPCandidate& candidate);
  void serializeFailure(_STD_ ostream& os, const CPAssignment& assignment);
  void serializeConflicts(_STD_ ostream& os);
  void serializeConflict(_STD_ ostream& os,
    const CPConflict& conflict);
  void serializeCommandHistory(_STD_ ostream& os);
  // LSB end

        /************************************************************
          Stuff for restoring the checkpoint.
         ************************************************************/


        /**
         * Reset the times back to what they were at the checkpoint.
         * Note this is a delta, not an absolute setting, and thus
         * isn't idempotent as you might expect it to be.  That's why
         * we do this first.  Note also we don't change the NOW timestep.
         */
        void reset_times() {
            T_system *tsys = get_tsys();

            Variable::ListOfVariable::iterator it;
            unsigned time_delta = oldest_timestep - tsys->get_oldest_timestep();
            for(it = tsys->begin_variables(); it!=tsys->end_variables(); ++it) {
                Variable *var = *it;
                if(var->get_time_step()!=static_cast<unsigned>(T_system::NOW))
                    var->set_time_step( var->get_time_step() + time_delta );
            }
            // do this last, since we call get_oldest above and expect to
            // get the time stored from before.
            tsys->set_oldest_timestep(oldest_timestep);
        }

  bool hasTransitionClauseCounts(const Clause& clause) const {
    const int pos = clause.number_of_positive_literals();
    const int neg = clause.number_of_negative_literals();
    return
      // An idle Transition Clause that is unindexed by a Command.
      (pos == 1 && neg == 2) ||
      // An idle Transition Clause that is indexed by a Command to another
      // component.
      (pos == 2 && neg == 2) ||
      // A commanded Transition Clause.
      (pos == 1 && neg == 3);
  }


  /**
   * The Clause contains the modeTransition=nominal Proposition and it
   * has the correct number of positive and negative literals. But is it a
   * Transition Clause (and hence not to be used after restoration)?
   */

  bool containsNegative(const Clause& clause,
			const Proposition& proposition) const {
    // The Proposition must be negative (appear on the LHS of an implication)
    const Clause::iterator end = clause.end_negative();
    for (Clause::iterator it = clause.begin_negative(); it != end; ++it) {
      const Proposition* pProposition = *it;
      // If the modeTransition=nominal Proposition is positive, it's false
      if (pProposition == &proposition) return true;
    }
    return false;
  }


 /**
   * On each progress after the L2_checkpoint object was created, indexed
   * clauses of the form N#state=Si & transition=nominal => N+1#state=Sj have
   * been added. During restoration, these indexed clauses have to be removed.
   */

  void delete_outdated_constraints() {
    T_system *tsys = get_tsys();
    LTMS *ltms = tsys->get_solver();
    for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
      Variable *pVariable = tsys->get_present_variable(i);
      if (pVariable->is_transitioned()) {
	Transitioned *pTransitioned = static_cast<Transitioned *>(pVariable);
	Variable *pTVariable = pTransitioned->get_zenith();
	// pTVariable points to a Transition at the NOW time step
	for ( ; pTVariable != 0; pTVariable = pTVariable->get_previous()) {
	  // The predecessor of a Transition is always a Transition
	  Transition *pTransition = static_cast<Transition *>(pTVariable);
	  // The Proposition that modeTransition=nominal
	  Proposition *pNominalProposition = pTransition->get_prop(0);
	  // Find all the Clauses for this Transition
          // Iterate over the Transitioned::all_transition_clauses
	  Clause::ListOfClause::iterator end = pTransitioned->clauses_end();
          for (Clause::ListOfClause::iterator cit = 
                 pTransitioned->clauses_begin(); cit != end; ++cit) {
	    Clause *pClause = *cit;
	    // Make sure it is deleted if it contains the Proposition and is
	    // not a background Clause
	    if (!pClause->is_deleted() &&
		containsNegative(*pClause, *pNominalProposition) &&
		pClause->is_transition()) {
	      // Remove it from Transitioned::transition_clauses()
	      pTransitioned->remove_transition_clause(pClause);
	      // Remove it from the LTMS
	      ltms->remove(*pClause);
	    } // if
	  } // for
	} // for
      } // if
    } // for
  }


  /// A brute-force way to get the mode Transition given ID and time step.

  Transition* getTransition(unsigned variableID, unsigned timeStep) {
    for (Variable::ListOfVariable::iterator it =
	   get_tsys()->begin_variables();
	 it != get_tsys()->end_variables(); ++it) {
      Variable* pVariable = *it;
      if (pVariable->is_transition() &&
	  pVariable->get_id() == variableID &&
	  pVariable->get_time_step() == timeStep) {
	return static_cast<Transition *>(pVariable);
      }
    }
    return NULL;
  }

  /**
   * An unindexed Clause represents a nominal Transition that is not commanded.
   * Either (1) another component was commanded or (2) no command was issued.
   * The Clauses say that, in the absence of a Command, the nominal Transition
   * is for the mode to stay the same.
   * \param transitioned the mode
   * \param toTimeStep the consequent time step
   */

  void restore_unindexed_clauses(Transitioned& transitioned,
				 unsigned toTimeStep) {
    // component.mode[t]=value[i] &
    // component.modeTransition[t]=nominal =>
    // component.mode[t+1]=value[i]
    // Get the mode Transition with the same ID as the Transitioned
    Transition *pTransition = getTransition(transitioned.get_id(),
					    toTimeStep);
    assert(pTransition);
    transitioned.add_idle_transitions(toTimeStep, *pTransition);
  }


  /**
   * An indexed Clause represents a nominal Transition that is Commanded.
   * The Clauses say that, in the presence of a Command, the nominal Transition
   * is for the mode to go from the "from" state to the "to" state.
   * \param transitioned the mode at the NOW time step
   * \param commandVariableID the variableID of the Command
   * \param commandValueIndex the valueIndex of the Command
   * \param commandTimeStep the time step of the Command
   */

  void restore_indexed_clauses(Transitioned& transitioned,
			       unsigned commandVariableID,
			       unsigned commandValueIndex,
			       unsigned commandTimeStep) {
    // component.modeTransition[t]=nominal &
    // component.cmdIn=c &
    // component.mode[t]=value[i] =>
    // component.mode[t+1]=value[j]
    // This function constrains from timeStep - 1 to timeStep, so increment.
    unsigned fromTimeStep = commandTimeStep;
    unsigned toTimeStep =
      (fromTimeStep + 1 == get_tsys()->get_time_step()) ?
      T_system::NOW :
      fromTimeStep + 1;
    transitioned.add_commanded_transitions(commandVariableID,
					   commandValueIndex,
					   fromTimeStep,
					   toTimeStep);
  }


  /**
   * L2_checkpoint::data::delete_outdated_constraints() removed all the
  * potentially incorrect transition Clauses. This method restores the
  * correct ones.
  */

  void restore_command_constraints() {
    T_system& t_system = *get_tsys();
    for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
      Variable *pVariable = t_system.get_present_variable(i);
      // Interested only in modes
      if (pVariable->is_transitioned()) {
	Transitioned *pTransitioned = static_cast<Transitioned *>(pVariable);
	// The predecessors of a Transitioned are Dependent, not Transitioned
	for ( ; pVariable != 0; pVariable = pVariable->get_previous()) {
	  unsigned commandTimeStep = pVariable->get_time_step();
	  if (commandTimeStep != T_system::NOW) {
	    MBApair<int, unsigned> commandPair =
	      commandHistory[commandTimeStep - t_system.get_oldest_timestep()];
	    int commandVariableID      = commandPair.first;
	    unsigned commandValueIndex = commandPair.second;
	    if (commandVariableID == 0) {
	      // An idle progress
	      restore_unindexed_clauses(*pTransitioned, commandTimeStep);
	    } else {
	      // A commanded progress
	      restore_indexed_clauses(*pTransitioned,
				      commandVariableID,
				      commandValueIndex,
	      			      commandTimeStep);
	    }
	  } // end if
	 } // end for
      } // end if
    } // end for
  }

        /**
         * Restore all commands and observables to their old values.
         * This requires unsetting the current ones.
         */
        void restore_cmds_obs() {
            T_system *tsys = get_tsys();

            // unset commands and obs
            {
                Assignable::ListOfAssignable::iterator it = tsys->begin_assignables();
                for( ; it != tsys->end_assignables(); ++it) {
                    Assignable *var = *it;
                    if(var->is_assumption()) continue;
                    else var->unassign();
                }
            }

            // set those we stored
            {
                assign_iterator it;
                for(it = cmds_obs_.begin(); it != cmds_obs_.end(); ++it)
                    (*it)->assign(tsys);
            }
        }


  /**
   * This callback object has method accept(const Variable&) which returns true
   * if the Variable is the State_variable with which the callback was created.
   */
  // Used in restore_state_var to find and delete the n#state variable.
  class accept_state_var : public Conflict_db::Variable_acceptor {
  public:
    /// Create the callback object with a pointer to the State_variable
    accept_state_var(Variable* pStateVariable) : state_var(pStateVariable) { }
    /// Accept the Variable if it is the State_variable supplied at creation
    virtual bool accept(const Variable& rVariable) const {
      return (state_var == &rVariable);
    }
  private:
    /// The State_variable supplied at creation
    const Variable *const state_var;
  };

        /**
         * Destroy the current state variable, and create a new one of the
         * right size.
         */
        void restore_state_var() {
            T_system *tsys = get_tsys();

            // Destroy the old state_var, cleaning up all the clauses
            // involved -- notably including those for mode variables.
            accept_state_var asv(tsys->get_state_variable());
            tsys->destroy_variables(&asv, tsys->begin_variables(),
                    tsys->end_variables(), false /* don't clean up */);
            {
	      for (unsigned i = 0; i < tsys->npresent_variables(); i++) {
		  Variable *pVariable = tsys->get_present_variable(i);
		  if (!pVariable->is_transitioned()) continue;
		  Transitioned *pTransitioned =
		    static_cast<Transitioned *>(pVariable);
		  pTransitioned->remove_destroyed_clauses(); // this is why
	      }
            }
            tsys->get_solver()->finish_destroying();

            // Create the new one.
            tsys->create_state_var(oldest_timestep, initial_modes_.size());

            // set the initial modes
            for(unsigned i=0; i<initial_modes_.size(); ++i)
                initial_modes_[i]->set_initial_values(tsys, oldest_timestep, i);
        }

        /**
         * Destroy the old candidates, and add new ones.
         */
        void restore_candidates() {
            Tracker *tracker = get_tracker();

            // destroy the current set of candidates and start anew
            tracker->get_t_system().eraseCandidates();
	    // The partition will be rendered obsolete
	    tracker->eraseCandidatePartition();

            // create all the new candidates
            for(unsigned i=0; i<candidates_.size(); ++i)
                candidates_[i]->create_in_tracker(tracker);

	    // Don't re-partition; that can be done when the user requests it
        }

        /**
          * Restore the conflicts to their former state.
          * First, remove the newer conflicts.
          *     -> remove all those from all Assignables
          *     -> remove all those from all Assignments (done by Assignable)
          *     -> remove all those in the Conflict_db, and delete them
          * Next, add the original conflicts.
          */
        void restore_conflicts() {
            T_system *tsys = get_tsys();

            {
                // removing from variables
                Assignable::ListOfAssignable::iterator it = tsys->begin_assignables();
                for( ; it != tsys->end_assignables(); ++it)
                    (*it)->erase_conflicts();
            }

            {
                // remove from c_db and delete
                Conflict::ListOfConflict *c_list = tsys->get_conflicts();
                Conflict::ListOfConflict::iterator it = c_list->begin();
                while(it != c_list->end()) {
                    delete *it;
                    c_list->erase(it); // also iterates
                }
            }

            // add the original ones
            {
                conf_iterator it = conflicts_.begin();
                for( ; it != conflicts_.end() ; ++it)
                    (*it)->create_in_db(tsys);
            }
        } // end restore_conflicts


  /**
   *  This class represents an Assignment: a triple (Assignable, value,
   *  time step).
   */

  class CPAssignment : public Pooled {
  public:
    CPAssignment(unsigned id, unsigned t, unsigned val)
      : varID_(id), value_(val), time_(t) { }
    unsigned varID() const { return varID_; }
    unsigned time()  const { return time_ ; }
    unsigned value() const { return value_; }

    /** Return the Assignment for this object's Assignable Variable */

    ::Assignment *find_assignment(T_system *tsys) const {
      Assignable *pAssignable = find_variable(tsys);
      if (pAssignable != 0) {
	return find_variable(tsys)->get_assignment(value());
      } else {
	_STD_ cerr << "No variable " << varID_ << _STD_ endl;
	return 0;
      }
    }

    /** Assign the Assignment for this object's Assignable Variable */

    void assign(T_system *tsys) const {
      Assignment *pAssignment = find_assignment(tsys);
      if (pAssignment != 0) {
	find_assignment(tsys)->assign();
      } else {
	_STD_ cerr << "No assigment " << varID_ << _STD_ endl;
      }
    }

  protected:
    /** Return the Assignable Variable for this object's time step and ID */

    Assignable *find_variable(T_system* tsys) const {
      Assignable *pAssignable = find_a_variable(tsys);
      while (pAssignable && pAssignable->get_time_step() != time()) {
	pAssignable = pAssignable->get_previous_assignable();
      }
      return pAssignable; // may be null!
    }

    /** Return the current Assignable Variable for this object's ID */

    Assignable *find_a_variable(T_system *tsys) const {
      Variable *pVariable =
	(varID() == T_system::STATE_VAR_ID) ?
	tsys->get_state_variable() :
	tsys->get_present_variable(varID());
      if (pVariable->is_state_variable() ||
	  pVariable->is_command() ||
	  pVariable->is_observable()) {
	return static_cast<Assignable *>(pVariable);
      } else if (pVariable->is_transitioned()) {
	return static_cast<Transitioned *>(pVariable)->get_zenith();
      } else {
	L2_throw(L2_not_found_error,
		 ("id " + MBA_string(varID()) +
		  " is not an observation, command, or modeTransition"));
      }
    }
    
  private:
    unsigned varID_; // index of Assignable in conflict_db::present_variables
    unsigned value_; // index of value
    unsigned time_;  // time step
  };

        class InitialValue : public Pooled {
            public:
                InitialValue() { } // undetermined value
                InitialValue(unsigned id, unsigned val)
                    : varID_(id), value_(val) { }
                unsigned varID() const { return varID_; }
                unsigned value() const { return value_; }

            private:
                unsigned varID_;
                unsigned value_;
        };

        class InitialModeAssignments : public Pooled {
            public:
                InitialModeAssignments (Assignment::weight_t w) {
		  weight_ = w;
		}
                void add_mode(unsigned id, unsigned value) {
                    modes_.push_back(new InitialValue(id, value));
                }

                typedef Array<InitialValue*, false>::iterator iterator;
                iterator begin_modes() const { return modes_.begin(); }
                iterator end_modes()   const { return modes_.end()  ; }
                Assignment::weight_t weight() const { return weight_; }

                // set the initial value of all modes here ; the 'true'
                // means to put the clause in the theory immediately
                void set_initial_values(T_system *tsys, unsigned time,
                        unsigned si) {
                    tsys->get_state_variable()->get_assignment(si)
                        ->set_weight(weight());
                    for(iterator it = begin_modes(); it!=end_modes(); ++it) {
                        Transitioned *x=tsys->get_transitioned((*it)->varID());
                        x->set_initial_mode(si, (*it)->value(), time, true);
                    }
                }

            private:
                Array<InitialValue*, false> modes_;
                Assignment::weight_t weight_;
        };

        class CPCandidate : public Pooled {
            public:
                CPCandidate(unsigned i) : si_(i) { }
                void add_failure(unsigned id, unsigned t, unsigned val) {
                    failures_.push_back(new CPAssignment(id, t, val));
                }

                typedef Array<CPAssignment*, false>::iterator failure_iterator;
                failure_iterator begin_failures() const {
                    return failures_.begin();
                }
                failure_iterator end_failures() const {
                    return failures_.end();
                }

                // get the value of the state variable we use
                unsigned si() const {
                    return si_;
                }


                // create the equivalent in the tracker
                void create_in_tracker(Tracker *tracker) const {
                    T_system *tsys = &tracker->get_t_system();

                    // create the tracker candidate
                    ::Candidate *c = new ::Candidate;

                    // add state=si
                    c->add(tsys->get_state_variable()->get_assignment(si()));

                    // and all the failures
                    failure_iterator fit = begin_failures();
                    for( ; fit != end_failures(); ++fit) {
                        // get the assignment and add it
                        c->add((*fit)->find_assignment(tsys));
                    }

                    // finally, tell the tracker about it
                    tracker->get_t_system().addCandidate(c);
                }

            private:
                unsigned si_;
                Array<CPAssignment*, false> failures_;
        };


        class CPConflict : public Pooled {
            public:
                typedef Array<CPAssignment*, false>::iterator iterator;
                iterator begin_assign() const {
                    return assignments_.begin();
                }
                iterator end_assign() const {
                    return assignments_.end();
                }
                void add_assign(unsigned varID, unsigned time, unsigned value) {
                    assignments_.push_back(new CPAssignment(varID, time, value));
                }

                void create_in_db(T_system *tsys) const {
                    Slist< ::Assignment* > list;
                    for(iterator it=begin_assign(); it!=end_assign(); ++it)
                        list.push_front((*it)->find_assignment(tsys));
                    tsys->add_conflict(list);
                }

            private:
                Array<CPAssignment*, false> assignments_;
        };


        // list of the values of observations and commands
        typedef Array<CPAssignment*, false>::iterator assign_iterator;
        Array<CPAssignment*, false> cmds_obs_;

        // list of initial mode assignments
        typedef Array<InitialModeAssignments*, false>::iterator init_iterator;
        Array<InitialModeAssignments*, false> initial_modes_;

        // list of the candidates
        typedef Array<CPCandidate*, false>::iterator cand_iterator;
        Array<CPCandidate*, false> candidates_;

        // list of the conflicts
        typedef Array<CPConflict*, false>::iterator conf_iterator;
        Array<CPConflict*, false> conflicts_;

        Tracker *tracker_;

        // how many timesteps do we have, including the NOW timestep
        unsigned ntimesteps;

        // what the oldest timestep is
        unsigned oldest_timestep;

        // (Command Variable ID, Value Index) by time step
        // Command Variable ID == -1 if there was no Command
        Array< MBApair<int, unsigned>, true > commandHistory;

  int indentLevel;
  bool valid;

};

#ifdef ENABLE_CHECKPOINT_SERIALIZE
/***************************************************************************
 The class that handles the SAX callbacks
***************************************************************************/

class L2_checkpoint::CheckpointHandler : public HandlerBase {
public:
  // Constructor and destructor
  CheckpointHandler(L2_checkpoint::data& theData) :
    theData(theData),
    inCommandsAndObservations(false),
    inConflict(false),
    initialModeAssignments(0),
    candidate(0),
    conflict(0)
  {
  }

  ~CheckpointHandler() { }

  // Handlers for the SAX DocumentHandler interface

  void startElement(const XMLCh* const name, AttributeList& attributes) {
    const char* s = XMLString::transcode(name);
    if        (XMLString::compareString(s, "cp:L2checkpoint") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:Times") == 0) {
      handleStartTimes(attributes);
    } else if (XMLString::compareString(s, "cp:CommandsAndObservations") == 0) {
      inCommandsAndObservations = true;
    } else if (XMLString::compareString(s, "cp:Assignment") == 0) {
      handleStartAssignment(attributes);
    } else if (XMLString::compareString(s, "cp:StateVariable") == 0) {
      handleStartStateVariable(attributes);
    } else if (XMLString::compareString(s, "cp:InitialModeAssignmentSet") == 0) {
      handleStartInitialModeAssignments(attributes);
    } else if (XMLString::compareString(s, "cp:InitialValue") == 0) {
      handleStartInitialValue(attributes);
    } else if (XMLString::compareString(s, "cp:CandidateSet") == 0) {
      handleStartCandidateSet(attributes);
    } else if (XMLString::compareString(s, "cp:Candidate") == 0) {
      handleStartCandidate(attributes);
    } else if (XMLString::compareString(s, "cp:Failure") == 0) {
      handleStartFailure(attributes);
    } else if (XMLString::compareString(s, "cp:ConflictSet") == 0) {
      handleStartConflictSet(attributes);
    } else if (XMLString::compareString(s, "cp:Conflict") == 0) {
      handleStartConflict(attributes);
    } else if (XMLString::compareString(s, "cp:CommandHistory") == 0) {
      handleStartCommandHistory(attributes);
    } else if (XMLString::compareString(s, "cp:Command") == 0) {
      handleStartCommand(attributes);
    } else {
      _STD_ cerr << "Unknown element [" << s << "]" << _STD_ endl;
    }
    delete [] const_cast<char *>(s);
  }

  void endElement(const XMLCh* const name) {
    const char* s = XMLString::transcode(name);
    if        (XMLString::compareString(s, "cp:L2checkpoint") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:Times") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:CommandsAndObservations") == 0) {
      inCommandsAndObservations = false;
    } else if (XMLString::compareString(s, "cp:Assignment") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:StateVariable") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:InitialModeAssignmentSet") == 0) {
      assert(initialModeAssignments != 0);
      theData.initial_modes_.push_back(initialModeAssignments);
      initialModeAssignments = 0;
    } else if (XMLString::compareString(s, "cp:InitialValue") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:CandidateSet") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:Candidate") == 0) {
      theData.candidates_.push_back(candidate);
      candidate = 0;
    } else if (XMLString::compareString(s, "cp:Failure") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:ConflictSet") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:Conflict") == 0) {
      theData.conflicts_.push_back(conflict);
      conflict = 0;
      inConflict = false;
    } else if (XMLString::compareString(s, "cp:CommandHistory") == 0) {
      // Nothing to be done
    } else if (XMLString::compareString(s, "cp:Command") == 0) {
      // Nothing to be done
    } else {
      _STD_ cerr << "Unknown element [" << s << "]" << _STD_ endl;
    }
    delete [] const_cast<char *>(s);
  }

  // Handlers for the SAX ErrorHandler interface
  void warning(const SAXParseException& e) {
    const char * systemId = XMLString::transcode(e.getSystemId());
    const char * message  = XMLString::transcode(e.getMessage());
    _STD_ cerr << _STD_ endl << "Warning at (file " << systemId
	      << ", line " << e.getLineNumber()
	      << ", char " << e.getColumnNumber()
	      << "): " << message << _STD_ endl;
    delete [] const_cast<char *>(systemId);
    delete [] const_cast<char *>(message);
  }
  void error(const SAXParseException& e) {
    const char * systemId = XMLString::transcode(e.getSystemId());
    const char * message  = XMLString::transcode(e.getMessage());
    _STD_ cerr << _STD_ endl << "Error at (file " << systemId
	      << ", line " << e.getLineNumber()
	      << ", char " << e.getColumnNumber()
	      << "): " << message << _STD_ endl;
    delete [] const_cast<char *>(systemId);
    delete [] const_cast<char *>(message);
  }
  void fatalError(const SAXParseException& e) {
    const char * systemId = XMLString::transcode(e.getSystemId());
    const char * message  = XMLString::transcode(e.getMessage());
    _STD_ cerr << _STD_ endl << "Fatal Error at (file " << systemId
	      << ", line " << e.getLineNumber()
	      << ", char " << e.getColumnNumber()
	      << "): " << message << _STD_ endl;
    delete [] const_cast<char *>(systemId);
    delete [] const_cast<char *>(message);
  }
private:
  void handleStartTimes(AttributeList& attributeList) {
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "oldest") == 0) {
	theData.oldest_timestep = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "count") == 0) {
	theData.ntimesteps = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
  }

  void handleStartAssignment(AttributeList& attributeList) {
    unsigned int id = 0, timeStep = 0, currentValueIndex = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "id") == 0) {
	id = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "timeStep") == 0) {
	timeStep = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "currentValueIndex") == 0) {
	currentValueIndex = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    if (inCommandsAndObservations) {
      L2_checkpoint::data::CPAssignment *assignment =
	new L2_checkpoint::data::CPAssignment(id, timeStep, currentValueIndex);
      theData.cmds_obs_.push_back(assignment);
    } else if (inConflict) {
      conflict->add_assign(id, timeStep, currentValueIndex);
    }
  }

  void handleStartStateVariable(AttributeList& attributeList) {
    // Nothing to be done
  }

  void handleStartInitialModeAssignments(AttributeList& attributeList) {
    Assignment::weight_t weight = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "weight") == 0) {
	weight = static_cast<Assignment::weight_t>(atof(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    assert(initialModeAssignments == 0);
    initialModeAssignments =
      new L2_checkpoint::data::InitialModeAssignments(weight);
  }

  void handleStartInitialValue(AttributeList& attributeList) {
    unsigned id = 0, valueIndex = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "id") == 0) {
	id = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "value") == 0) {
	valueIndex = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    initialModeAssignments->add_mode(id, valueIndex);
  }

  void handleStartCandidateSet(AttributeList& attributeList) {
    // Nothing to be done
  }

  void handleStartCandidate(AttributeList& attributeList) {
    unsigned int index = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "index") == 0) {
	index = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    assert(candidate == 0);
    candidate = new L2_checkpoint::data::CPCandidate(index);
  }

  void handleStartFailure(AttributeList& attributeList) {
    unsigned id = 0, timeStep = 0, valueIndex = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "id") == 0) {
	id = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "timeStep") == 0) {
	timeStep = static_cast<unsigned int>(atoi(value));
      } else if (XMLString::compareString(name, "value") == 0) {
	valueIndex = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    assert(candidate != 0);
    candidate->add_failure(id, timeStep, valueIndex);
  }

  void handleStartConflictSet(AttributeList& attributeList) {
    // Nothing to be done
  }

  void handleStartConflict(AttributeList& attributeList) {
    assert(conflict == 0);
    conflict = new L2_checkpoint::data::CPConflict();
    inConflict = true;
  }

  void handleStartCommandHistory(AttributeList& attributeList) {
    // Nothing to be done
  }

  void handleStartCommand(AttributeList& attributeList) {
    int id = 0;
    unsigned valueIndex = 0;
    for (unsigned int i = 0; i < attributeList.getLength(); i++) {
      const char* name  = XMLString::transcode(attributeList.getName(i));
      const char* value = XMLString::transcode(attributeList.getValue(i));
      if (XMLString::compareString(name, "id") == 0) {
	id = atoi(value);
      } else if (XMLString::compareString(name, "valueIndex") == 0) {
	valueIndex = static_cast<unsigned int>(atoi(value));
      } else {
	// Illegal attribute
      }
      delete [] const_cast<char *>(name);
      delete [] const_cast<char *>(value);
    }
    // Stack-allocate; pass by value a temp copy
    MBApair<int, unsigned> command(id, valueIndex);
    theData.commandHistory.push_back(command);
  }

private:
  L2_checkpoint::data& theData;
  bool inCommandsAndObservations;
  bool inConflict;
  L2_checkpoint::data::InitialModeAssignments* initialModeAssignments;
  L2_checkpoint::data::CPCandidate* candidate;
  L2_checkpoint::data::CPConflict* conflict;
};

L2_checkpoint::data::data(Tracker* tracker,
			  const MBA_string& xmlCheckpointFilePath) {
  tracker_ = tracker;
  valid = deserialize(xmlCheckpointFilePath);
}

/**
 * This does the majority of the work of the deserializing constructor for
 * L2_checkpoint::data.
 */

bool L2_checkpoint::data::deserialize(const MBA_string& xmlCheckpointFilePath) {
  // Initialize the XML4C2 system
  L2_TRY {
    XMLPlatformUtils::Initialize();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (const XMLException& e) {
    const char* message = XMLString::transcode(e.getMessage());
    _STD_ cerr << "Error during initialization! Message:" << _STD_ endl
	      << message << _STD_ endl;
    delete [] const_cast<char *>(message);
    return false;
  }
#endif
  // Create a SAX parser object.
  SAXParser parser;
  parser.setDoValidation(true);
  parser.setDoNamespaces(true);
  // Create our SAX handler object and install it on the parser, as the
  // document and error handler
  CheckpointHandler handler(*this);
  parser.setDocumentHandler(&handler);
  parser.setErrorHandler(&handler);
  // Kick off the parse of the indicated file. Catch any exceptions that
  // might propagate out of it
  L2_TRY {
    parser.parse(xmlCheckpointFilePath.c_str());
  }
#ifdef ENABLE_EXCEPTIONS
  catch (const XMLException& e) {
    const char* message = XMLString::transcode(e.getMessage());
    _STD_ cerr << _STD_ endl << "Error during parsing: ["
	      << xmlCheckpointFilePath
	      << "]" << _STD_ endl
	      << "Exception message is: " << _STD_ endl
	      << message << _STD_ endl << _STD_ endl;
    delete [] const_cast<char *>(message);
    return false;
  }
#endif
  return true;
}
#endif

// There may be a bug here

void L2_checkpoint::data::indent(_STD_ ostream& os) {
  /*
  const int gain = 2;
  for (int i = 0; i < indentLevel * gain; i++) os << " ";
  */
}

#ifdef ENABLE_CHECKPOINT_SERIALIZE
/// Serialize to the output stream

bool L2_checkpoint::data::serialize(_STD_ ostream& os) {
  os << "<?xml version=\"1.0\"?>" << _STD_ endl;
  os << "<!DOCTYPE cp:L2checkpoint SYSTEM \"checkpoint.dtd\">" << _STD_ endl;
  os << "<cp:L2checkpoint xmlns:cp=\"http://www.w3.org/1999/XSL/Format\">"
     << _STD_ endl;
  serializeTimes(os);
  serializeCommandsAndObservations(os);
  serializeStateVariables(os);
  serializeCandidates(os);
  serializeConflicts(os);
  serializeCommandHistory(os);
  os << "</cp:L2checkpoint>" << _STD_ endl;
  return true;
}


/// Serialize to the file

bool L2_checkpoint::data::serialize(const MBA_string& xmlCheckpointFilePath) {
  _STD_ ofstream os(xmlCheckpointFilePath.c_str());
  if (!os) {
    return false;
  } else {
    return serialize(os);
  }
}

void L2_checkpoint::data::serializeTimes(_STD_ ostream& os) {
  indentLevel++;
  indent(os);
  os << "<cp:Times" 
     << " oldest=\"" << oldest_timestep << "\"" 
     << " count=\""  << ntimesteps      << "\""
     << """/>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeAssignment(_STD_ ostream& os,
  const L2_checkpoint::data::CPAssignment& assignment) {
  indentLevel++;
  indent(os);
  os << "<cp:Assignment id=\""  << assignment.varID() << "\""
     << " timeStep=\""          << assignment.time()  << "\""
     << " currentValueIndex=\"" << assignment.value() << "\""
     << "/>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeCommandsAndObservations(_STD_ ostream& os) {
  indentLevel++;
  indent(os); os << "<cp:CommandsAndObservations>"  << _STD_ endl;
  for (Array<CPAssignment*, false>::iterator it = cmds_obs_.begin();
       it != cmds_obs_.end(); ++it) {
    CPAssignment *assignment = *it;
    serializeAssignment(os, *assignment);
  }
  indent(os); os << "</cp:CommandsAndObservations>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeStateVariables(_STD_ ostream& os) {
  indentLevel++;
  indent(os); os << "<cp:StateVariable>" << _STD_ endl;
  for (Array<InitialModeAssignments*, false>::iterator it =
	 initial_modes_.begin(); it != initial_modes_.end(); ++it) {
    InitialModeAssignments* initialModeAssignments = *it;
    serializeInitialModeAssignments(os, *initialModeAssignments);
  }
  indent(os); os << "</cp:StateVariable>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeInitialModeAssignments(
  _STD_ ostream& os, const InitialModeAssignments& initialModeAssignments) {
  indentLevel++;
  indent(os); os << "<cp:InitialModeAssignmentSet weight=\""
		 << initialModeAssignments.weight()
		 << "\">" << _STD_ endl;
  for (Array<InitialValue*, false>::iterator it =
	 initialModeAssignments.begin_modes();
       it != initialModeAssignments.end_modes(); ++it) {
    InitialValue* initialValue = *it;
    serializeInitialValue(os, *initialValue);
  }
  indent(os); os << "</cp:InitialModeAssignmentSet>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeInitialValue(_STD_ ostream& os,
  const L2_checkpoint::data::InitialValue& initialValue) {
  indentLevel++;
  indent(os);
  os << "<cp:InitialValue id=\""
     << initialValue.varID()
     << "\" value=\""
     << initialValue.value()
     << "\"/>"
     << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeCandidates(_STD_ ostream& os) {
  indentLevel++;
  indent(os); os << "<cp:CandidateSet>" << _STD_ endl;
  for (Array<CPCandidate*, false>::iterator it = candidates_.begin();
       it != candidates_.end(); ++it) {
    CPCandidate* candidate = *it;
    serializeCandidate(os, *candidate);
  }
  indent(os); os << "</cp:CandidateSet>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeCandidate(_STD_ ostream& os,
  const L2_checkpoint::data::CPCandidate& candidate) {
  indentLevel++;
  indent(os);
  os << "<cp:Candidate index=\""
     << candidate.si()
     << "\">" << _STD_ endl;
  for (Array<CPAssignment*, false>::iterator it = candidate.begin_failures();
       it != candidate.end_failures(); ++it) {
    CPAssignment* assignment = *it;
    serializeFailure(os, *assignment);
  }
  indent(os); os << "</cp:Candidate>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeFailure(_STD_ ostream& os,
  const L2_checkpoint::data::CPAssignment& failure) {
  indentLevel++;
  indent(os);
  os << "<cp:Failure id=\""     << failure.varID() << "\""
     << " timeStep=\""          << failure.time()  << "\""
     << " currentValueIndex=\"" << failure.value() << "\""
     << "/>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeConflicts(_STD_ ostream& os) {
  indentLevel++;
  indent(os); os << "<cp:ConflictSet>" << _STD_ endl;
  for (Array<CPConflict*, false>::iterator it = conflicts_.begin();
       it != conflicts_.end(); it++) {
    CPConflict* conflict = *it;
    serializeConflict(os, *conflict);
  }
  indent(os); os << "</cp:ConflictSet>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeConflict(_STD_ ostream& os,
  const L2_checkpoint::data::CPConflict& conflict) {
  indentLevel++;
  indent(os); os << "<cp:Conflict>" << _STD_ endl;
  for (Array<CPAssignment*, false>::iterator it = conflict.begin_assign();
       it != conflict.end_assign(); ++it) {
    CPAssignment* assignment = *it;
    serializeAssignment(os, *assignment);
  }
  indent(os); os << "</cp:Conflict>" << _STD_ endl;
  indentLevel--;
}

void L2_checkpoint::data::serializeCommandHistory(_STD_ ostream& os) {
  indentLevel++;
  indent(os); os << "<cp:CommandHistory>" << _STD_ endl;
  for (Array< MBApair<int, unsigned>, true >::iterator it =
	 commandHistory.begin(); it != commandHistory.end(); ++it) {
    MBApair<int, unsigned> command = *it;
    indentLevel++;
    indent(os);
    os << "<cp:Command id=\"" << command.first  << "\""
       << " valueIndex=\""   << command.second << "\"" 
       << "/>" << _STD_ endl;
    indentLevel--;
  }
  indent(os); os << "</cp:CommandHistory>" << _STD_ endl;
  indentLevel--;
}
#endif

/***************************************************************************
  Public interface.
 ***************************************************************************/

L2_checkpoint::L2_checkpoint(Livingstone* engine) : valid(true) {
    data_ = new data(engine->get_tracker());
}
L2_checkpoint::L2_checkpoint(Tracker* tracker) : valid(true) {
    data_ = new data(tracker);
}


L2_checkpoint::~L2_checkpoint() {
    delete data_;
}


bool L2_checkpoint::restore() {
    return data_->restore();
}


#ifdef ENABLE_CHECKPOINT_SERIALIZE
bool L2_checkpoint::serialize(const MBA_string& xmlCheckpointFilePath) {
  return data_->serialize(xmlCheckpointFilePath);
}


bool L2_checkpoint::serialize(_STD_ ostream& os) {
  return data_->serialize(os);
}


L2_checkpoint::L2_checkpoint(Tracker* tracker,
			     const MBA_string& xmlCheckpointFilePath) {
    data_ = new data(tracker, xmlCheckpointFilePath);
    // The creation of the data object may have failed
    valid = data_->isValid();
}
#endif
