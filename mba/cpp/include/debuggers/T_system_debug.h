/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// A nonflight class that contains debugging functions
// $Id: T_system_debug.h,v 1.37 2004/02/05 22:36:13 lbrown Exp $

#ifndef T_SYSTEM_DEBUG_H
#define T_SYSTEM_DEBUG_H

#include <debuggers/conflict_db_debug.h>
#include <debuggers/trajectory.h> // for Trajectory::Parts
#include <transition/transitioned.h>

class L2_string_map;

/**
 * Enable command-line interaction with the T_system
 */

class T_system_debug : public Conflict_db_debug {
 public:

/***************************************************************************
  Constructor
 ***************************************************************************/

  /**
   * Interact with the given T_system, sending output to the given ostream.
   * The string map allows referring to a Variable by its name.
   */
  T_system_debug(T_system& t_sys, L2_string_map& map, _STD_ ostream& outstream)  :
  Conflict_db_debug(t_sys, map, outstream),
  t_system(t_sys),
  stream(outstream),
  long_state_var(true) { }

  /// Interpret the command line, delegating Conflict_db_debug if unable.
  virtual bool parse_cmd(const char* cmd, const char* args);

  /// Write a usage message to the ostream.
  virtual void listener_usage() const;

  /**
   * Print the recipe used for creating a Transition at a given time step.
   * It is a set of positive antecedets, a set of negative antecendents, and
   * a consequent.
   */
  void print_proto_transition(Transitioned::Proto_transition&
			      proto_transition);

  /**
   * Print the recipe used for creating an Assignment at a given time step.
   * It is a (Variable, value) pair.
   */
  void print_proto_assignment(const T_system::Proto_assignment&
			      proto_assignment);

  /**
   * Print a Transitioned (mode) Variable.
   * It is a set of Proto_transition elements.
   */
  void print_transitioned(Transitioned* pTransitioned);

  /**
   *  Iterates over all Variables at the passed time, calling the callback
   *  function object on each Variable.
   */
  void time_variable_iterator(unsigned int time,
			      Conflict_db_debug::VariableCallbackObject&
			      variableCallbackObject);

  /**
   *  For the named Variable, this function iterates over all times at which
   *  the Variable appears.   The callback function object is called on the
   *  Variable that represents the named Variable at each point in time.
   */
  void variable_time_iterator(const char* name,
			      Conflict_db_debug::VariableCallbackObject&
			      variableCallbackObject);

  /**
   * Prints out only those Variables that appear at the given time step
   * (use T_system::NOW for the current time).
   */
  void print_variables_at_time(unsigned int time);

  /// Prints out all Variables.
  void print_all_variables();

  /**
   * Prints the value of the named Variable at all time steps.
   * Also an example of how to use variable_time_iterator.
   */
  void print_variable_at_times(const char* variableName);

  /**
   * Same as Conflict_db::print_assignment(const Assignment&, unsigned), but
   * explain the state=si.
   * The Conflict_db doesn't know about the State_variable
   */
  virtual unsigned print_assignment(const Assignment& assignment,
				    unsigned indent = 0) {
    return print_assignment(stream, assignment, indent);
  }
  /**
   * Same as Conflict_db::print_assignment(_STD_ ostream&, const Assignment&,
   * unsigned), but explain the state=si.
   * The Conflict_db doesn't know about the State_variable
   */
  virtual unsigned print_assignment(_STD_ ostream& outstream,
				    const Assignment& assignment, 
				    unsigned indent=0);

  /**
   * Print to the given ostream the Trajectory that corresponds to the given
   * Assignment.
   * \param outstream the output stream to which the information is printed
   * \param assignment the Assignment to the State Variable
   * \param indent the indentation level
   */
  unsigned print_start_state_assignment(_STD_ ostream& outstream,
					const Assignment& assignment,
					unsigned indent=0);

  /**
   * Print to the default ostream the Trajectory that corresponds to the given
   * Assignment.
   * \param assignment the Assignment to the State Variable
   * \param indent the indentation level
   */
  unsigned print_start_state_assignment(const Assignment& assignment,
					unsigned indent=0){
    return print_start_state_assignment(stream, assignment, indent);
  }

  /// Explain a Candidate, including truncated state, by its details.
  void candidate_details(Candidate&, Slist<Trajectory::Parts>&,
			 Array<MBA_string, true>& variableNames,
			 Array<MBA_string, true>& valueNames);

 public:
  /**
   * Finds the Variable with the given time-independent name at the given
   * timestep.   E.g., name=var1, timestep=3 finds the var 3#var1. Returns 0
   * if not found.
   * Note that this will not find Transition Variables.
   * \param variableName the name not including the time step or '#'
   * \param timeStep the time step
   * \return a pointer the the name Variable at the given time step, or NULL
   * if it doesn't exist
   */
  Variable* find_at_timestep(const MBA_string variableName, unsigned timeStep);
  
  /**
   * Finds the Variable at time 'timestep' that represents the same Variable
   * as passed in.
   */
  Variable* find_at_timestep(Variable& variable, unsigned timestep);

  /**
   * Given a Proposition in the "now" timestep, this method returns the
   * corresponding Proposition from the passed timestep.
   */
  Proposition* find_at_timestep(Proposition& proposition, int timestep);

  /**
   * Finds the Assignable Variable that represents the named Command in the
   * current time step
   */
  Command* find_current_command(const MBA_string commandName);

  /**
   * Finds the Observable Variable that represents the named Observable in
   * the current time step.
   */
  Observable* find_current_observable(const MBA_string observableName);

  /**
   * Finds the Transitioned (mode) Variable that represents the named mode
   * Variable in the current time step.
   */
  Transitioned *find_current_mode(const MBA_string modeName);

  /**
   * If the system is inconsistent, the Observation Assignments involved in
   * detecting the inconsistency are added to the list of Assignments.
   */
  void find_discrepant_observations(Assignment::ListOfAssignment& assignments);

  /**
   * If the system is inconsistent, the Command Assignments involved in
   * detecting the inconsistency are added to the list of Assignments.
   */
  void find_discrepant_commands(Assignment::ListOfAssignment& assignments);

  /// Parse the modeVariable=value string and make the assignment.
  bool assign_mode(MBA_string);

  /**
   * Was the specified Variable, at the specified time step, assigned its
   * value as a result of an observation or progress?
   */
  bool is_exogenously_assigned(Variable *pVariable);

 protected:
	
  // Auxiliaries to parse_cmd()

  /// Handle the "state-var-style" command line.
  void doStateVariableStyle(const char *cmd, const char *args);
  /// Handle the "discrepancy" command line.
  void doDiscrepancy       (const char *cmd, const char *args);
  /// Handle the "transitions" command line.
  void doTransitions       (const char *cmd, const char *args);
  /// Handle the "values" command line.
  void doValues            (const char *cmd, const char *args);
  /// Handle the "times" command line.
  void doTimes             (const char *cmd, const char *args);
  /// Handle the "oldest-time" command line.
  void doOldestTime        (const char *cmd, const char *args);
  /// Handle the "cmds" command line.
  void doCommands          (const char *cmd, const char *args);
  /// Handle the "observables" command line.
  void doObservables       (const char *cmd, const char *args);
  /// Handle the "transitioned" command line.
  void doTransitioned      (const char *cmd, const char *args);
  /// Handle the "export-ini-file" command line
  void doExportIniFile     (const char *cmd, const char *args);
#ifdef ENABLE_RECOVERY
  /// Handle the "plan-steps" command line.
  void doPlanSteps         (const char *cmd, const char *args);
#endif

 protected:
  /// A reference to the T_system upon which this class reports.
  T_system &t_system;

 private:
  /// Where to send output.
  _STD_ ostream &stream;
  
  /// Whether to print a long description of the state_var; starts false.
  bool long_state_var;
};

#endif // T_SYSTEM_DEBUG_H
