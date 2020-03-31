/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Transition system representation, NASA Ames Research Center
// See Copyright.txt
// $Id: T_system_debug.cpp,v 1.70 2004/02/05 22:36:18 lbrown Exp $

#include <debuggers/T_system_debug.h>
#include <debuggers/L2_string_map.h>
#include <conflict_db/candidate.h>
#include <livingstone/L2_assert.h>
#include <livingstone/L2_fstream.h>
#include <stdlib.h>

void
T_system_debug::time_variable_iterator(unsigned int time,
  Conflict_db_debug::VariableCallbackObject& func) {
  for (Variable::ListOfVariable::iterator it = t_system.begin_variables();
       it != t_system.end_variables(); ++it) {
    Variable *pVariable = *it;
    if (pVariable->get_time_step() == time) {
      func(*pVariable);
    }
  }
}


Variable* T_system_debug::find_at_timestep(const MBA_string name,
					   unsigned timestep) {
  MBA_string past_name = L2_string_map::form_name_at_time(name, timestep);
  return find_variable(past_name);
}


// Assume that rVariable is the NOW Variable; search backward for the Variable
// at the given time step

Variable* T_system_debug::find_at_timestep(Variable& rVariable,
					   unsigned timestep) {
  for (Variable *pVariable = &rVariable; pVariable ;
       pVariable = pVariable->get_previous()) {
    if (pVariable->get_time_step() == timestep) { return pVariable; }
  }
  return 0;
}


void
T_system_debug::variable_time_iterator(const char* name,
  Conflict_db_debug::VariableCallbackObject& func) {

  // If the user passes a Variable name with a time signature
  // in it (i.e., <time>#<variable-name>, strip that off
  const char* variable_string = strchr(name, '#');
  if (variable_string) {
    variable_string++;      // Increment to one past the #
  } else {
    variable_string = name;
  }

  Variable* pVariable = find_variable(variable_string, true);

  unsigned int last_time = t_system.get_time_step();
  
  // First call the function on the Variable for all the previous time steps
  for (unsigned time = t_system.get_oldest_timestep(); time < last_time;
       time++) {
    Variable* pOldVariable = find_at_timestep(variable_string, time);
    if (pOldVariable) {
      func(*pOldVariable);
    }
  }
  // Finally call the function on the Variable for the NOW time step
  func(*pVariable);
}


/**
 * This class is used as a callback. The callback function is operator().
 * For the Variable parameter, print "<name>=<value>"
 */

class PrintVariable : public Conflict_db_debug::VariableCallbackObject {
public:
  virtual void operator() (Variable& var) {
    _STD_ cout<< db_debugger.get_var_name(var)
	     << "="
	     << db_debugger.get_variable_value(var) << _STD_ endl;
  }
  
  PrintVariable(Conflict_db_debug &d) : db_debugger(d) {};

private:
  Conflict_db_debug &db_debugger;
};


static Conflict_db_debug * sort_db; // HACK!!!
extern "C" int var_alpha_comp(const void *v1, const void *v2) {
  Variable *varL = *(Variable**)v1;
  Variable *varR = *(Variable**)v2;
  MBA_string nameL = sort_db->get_var_name(*varL);
  MBA_string nameR = sort_db->get_var_name(*varR);
  return nameL.compare(nameR);
}


template <class Iterator>
void print_variable_values(Conflict_db_debug& db_debugger,
			   Iterator it,
			   const Iterator& end,
			   bool (*accept)(Variable*,void*),
			   void *acceptdata = 0) {
  // an array; allocated size is 50 elements, used size is n
  unsigned alloc = 50;
  unsigned n;
  Variable **vars = new Variable* [alloc];

  // collect the variables
  for (n = 0; it != end; ++it) {
    Variable *pVariable = *it;
    if (accept(pVariable, acceptdata)) {
      // realloc if needed
      if (n == alloc) {
	unsigned oldalloc = alloc;
	Variable **oldvars = vars;
	alloc *= 2;
	vars = new Variable* [alloc];
	memcpy(vars, oldvars, oldalloc*sizeof(Variable*));
	delete[] oldvars;
      }
      vars[n] = pVariable;
      n++;
    }
  }

  // sort the variables alphabetically
  sort_db = &db_debugger;
  qsort(vars, n, sizeof(Variable*), var_alpha_comp);
  sort_db = 0;

  // print them out
  for (unsigned i = 0; i < n; i++) {
    Variable *pVariable = vars[i];
    _STD_ cout << db_debugger.get_var_name(*pVariable)
	      << " = "
	      << db_debugger.get_variable_value(*pVariable)
	      << _STD_ endl;
  }
  delete[] vars;
}


// Is the Variable's timestep the one supplied (as a void*)?

static bool accept_var_at_time(Variable *pVariable, void *timep) {
  unsigned time = *(unsigned int*)timep;
  return pVariable->get_time_step() == time;
}


// A predicate that is true of all Variables

static bool accept_all(Variable*, void*) {
  return true;
}


// A predicate that is true of Command Variables at the NOW time step

static bool accept_present_command(Variable *pVariable, void*) {
  return
    pVariable->is_command() && 
    pVariable->get_time_step() == T_system::NOW; 
}

// A predicate that is true of Observable Variables at the NOW time step

static bool accept_present_observable(Variable *pVariable, void*) {
  return
    pVariable->is_observable() && 
    pVariable->get_time_step() == T_system::NOW; 
}

// A predicate that is true of Transitioned Variables at the NOW time step

static bool accept_present_transitioned(Variable *pVariable, void*) {
  return
    pVariable->is_transitioned() && 
    pVariable->get_time_step() == T_system::NOW; 
}


void
T_system_debug::print_variables_at_time(unsigned int time) {
  print_variable_values(*this,
			t_system.begin_variables(),
			t_system.end_variables(),
			accept_var_at_time, &time);
}


void
T_system_debug::print_all_variables() {
    print_variable_values(*this,
			  t_system.begin_variables(),
			  t_system.end_variables(),
			  accept_all);
}


class PrintValueAndTime : public Conflict_db_debug::VariableCallbackObject {
public:
  virtual void operator() (Variable& var) {
    unsigned int time = var.get_time_step();
    if (T_system::NOW == time)
      _STD_ cout << "now";
    else
      _STD_ cout << time;
    _STD_ cout << "\t" << db_debugger.get_variable_value(var) << _STD_ endl;
  }

  PrintValueAndTime(Conflict_db_debug& d) : db_debugger(d) { };

private:
  Conflict_db_debug  &db_debugger;
};


void
T_system_debug::print_variable_at_times(const char* name) {
  if(!name || name[0]=='\0') {
    stream
      << "Earliest time: " << t_system.get_oldest_timestep() << _STD_ endl
      << "Current time:  " << t_system.get_time_step() << _STD_ endl;
  } else {
    stream << _STD_ endl
	   << " Time  \tValue" << _STD_ endl;
    stream << " ----  \t----------" << _STD_ endl;
    
    PrintValueAndTime pv(*this);
    variable_time_iterator(name,pv);
    _STD_ cout << _STD_ endl;
  }
}


/***************************************************************************
  Print assignments knowing about summarization.
 ***************************************************************************/

unsigned T_system_debug::print_assignment(_STD_ ostream& outstream,
        const Assignment& assignment, unsigned indent) {
  if (long_state_var &&
     assignment.get_variable() == t_system.get_state_variable()) {
    return print_start_state_assignment(outstream, assignment, indent);
  } else {
    return Conflict_db_debug::print_assignment(outstream, assignment, indent);
  }
}


unsigned T_system_debug::print_start_state_assignment(_STD_ ostream& outstream,
        const Assignment& assignment, unsigned indent) {
  MBA_string istr;
  if (indent < 3) {
    istr = "*  ";
  } else {
    istr = get_indent_string(indent - 3) + "*  ";
  }

  const Trajectory& traj = t_system.get_trajectory(assignment.get_index());

  unsigned nlines = 0;
  for (Trajectory::iterator it = traj.begin(); it != traj.end(); ++it) {
    const Trajectory::Parts& trajectoryParts = *it;
    unsigned variableID = trajectoryParts.get_id();
    unsigned timeStep   = trajectoryParts.get_time();
    unsigned valueIndex = trajectoryParts.get_index();
    Variable* pVariable = t_system.get_present_variable(variableID);
    if (pVariable->is_transitioned()) {
      // Use the corresonding Transition instead
      Transitioned* pTransitioned = static_cast<Transitioned*>(pVariable);
      pVariable = pTransitioned->get_zenith();
    }
    outstream
      << istr
      << map.get_variable_name(*pVariable, timeStep)
      << "="
      << map.get_variable_values(*pVariable)[valueIndex]
      << " :"
      << trajectoryParts.get_rank()
      << _STD_ endl;
    nlines++;
  }
#ifdef DEBUG_PRINT_START_STATE_STATS
  _STD_ cout << "state=s" << assignment.get_index()
	    << " with " << traj.get_parts_count() << " parts"
	    << " weight " << assignment.get_weight()
	    << _STD_ endl;
#endif
  return nlines;
}


/***************************************************************************
  Return what a candidate means in terms of truncated assignments.
 ***************************************************************************/

void T_system_debug::candidate_details(Candidate& candidate,
				       Slist<Trajectory::Parts>& list,
				       Array<MBA_string, true>& variableNames,
				       Array<MBA_string, true>& valueNames) {
  // foreach assignment
  for (unsigned i = 0; i < candidate.size(); ++i) {
    const Assignment *pAssignment = candidate.get_assignment(i);
    if (long_state_var &&
       pAssignment->get_variable() != t_system.get_state_variable()) {
      // get info from still-extant assignment
      const Assignable* pVariable = pAssignment->get_variable();
      unsigned valueIndex = pAssignment->get_index();
      Trajectory::Parts parts(pAssignment);
      list.push_front(parts); // A reference to an automatic object!
      variableNames.push(map.get_variable_name(*pVariable));
      valueNames.push(map.get_variable_values(*pVariable)[valueIndex]);
    } else {
      // get info from trajectory corresponding to state=si
      const Trajectory& t = t_system.get_trajectory(pAssignment->get_index());
      for (Trajectory::iterator it = t.begin(); it != t.end(); ++it) {
	const Trajectory::Parts& trajectoryParts = *it;
	list.push_front(trajectoryParts);
	unsigned variableID = trajectoryParts.get_id();
	unsigned timeStep   = trajectoryParts.get_time();
	unsigned valueIndex = trajectoryParts.get_index();
	Variable* pVariable = t_system.get_present_variable(variableID);
	if (pVariable->is_transitioned()) {
	  // If it is a mode, use the corresonding horizon Transition instead
	  pVariable = static_cast<Transitioned*>(pVariable)->get_horizon();
	}
	variableNames.push(map.get_variable_name(*pVariable, timeStep));
	valueNames.push(map.get_variable_values(*pVariable)[valueIndex]);
      }
    }
  }
}


void T_system_debug::doStateVariableStyle(const char *cmd, const char *args) {
  if (args[0]!='\0') {
    if (!strcmp(args, "toggle")) {
      long_state_var = !long_state_var;
    } else if (!strcmp(args, "long")) {
      long_state_var = true;
    } else if (!strcmp(args, "short")) {
      long_state_var = false;
    } else {
      _STD_ cerr << "Unknown state-var style `" << args << "'\n"
		<< "Allowable values are: toggle, long, short\n";
    }
  }
  stream << "Using " << (long_state_var?"long":"short")
	 << " format for state=si\n";
}


void T_system_debug::doDiscrepancy(const char *cmd, const char *args) {
  bool consistent = t_system.propagate();
  if (consistent) {
    stream  << "System is consistent" << _STD_ endl;
  } else {
    Assignment::ListOfAssignment assignments;
    find_discrepant_observations(assignments);
    find_discrepant_commands(assignments);
    if (assignments.begin() == assignments.end()) {
      stream 
	<< "No commands or observations in explanation of inconsistency"
	<< _STD_ endl;
    } else {
      for (Assignment::ListOfAssignment::iterator it = assignments.begin();
	   it != assignments.end();  ++it) {
	Assignment *pAssignment = *it;
	print_assignment(*pAssignment);
	stream << _STD_ endl;
      }
    }
  }
}


void T_system_debug::doTransitions(const char *cmd, const char *args) {
  Transitioned* var = find_current_mode(args);
  if (var) {
    print_transitioned(var);
  } else {
    stream << "No such mode variable " << args << _STD_ endl;
  }
}


void T_system_debug::doValues(const char *cmd, const char *args) {
  if (args[0] == '\0') {
    print_all_variables();
  } else {
    unsigned int time;
    if(!strcmp(args, "now")) {
      time = T_system::NOW;
    } else {
      time = atoi(args);
      // a bit of error-checking
      if (time == 0 && args[0] != '0') {
	_STD_ cerr << "Non-integer `" << args << "'\n";
	return;
      }
    }
    print_variables_at_time(time);
  }
}


void T_system_debug::doTimes(const char *cmd, const char *args) {
  print_variable_at_times(args);
}


void T_system_debug::doOldestTime(const char *cmd, const char *args) {
  _STD_ cout << "Oldest timestep in theory: "
	    << t_system.get_oldest_timestep()
	    << _STD_ endl;
}


void T_system_debug::doCommands(const char *cmd, const char *args) {
  print_variable_values(*this,
			t_system.begin_variables(),
			t_system.end_variables(),
			accept_present_command);
}


void T_system_debug::doObservables(const char *cmd, const char *args) {
  print_variable_values(*this,
			t_system.begin_variables(),
			t_system.end_variables(),
			accept_present_observable);
}


void T_system_debug::doTransitioned(const char *cmd, const char *args) {
  print_variable_values(*this,
			t_system.begin_variables(),
			t_system.end_variables(),
			accept_present_transitioned);
}


void T_system_debug::doExportIniFile(const char* cmd, const char* args) {
  // Assume that args is a file pathname. Try to make an output file stream.
  _STD_ ofstream os(args);
  if (!os) { // Couldn't open _STD_ ofstream
    // Do nothing for now
  } else {
    os << "<?xml version=\"1.0\"?>" << _STD_ endl;
    os << "<mplInit name=\"ignore\" version=\"1.00\">" << _STD_ endl;
    for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
      const Variable* const pVariable = t_system.get_present_variable(i);
      if (pVariable->is_transitioned() ||
	  pVariable->is_observable()) {
	os << "    <assign eq=\""
	   << get_var_name(*pVariable)
	   << "="
	   << get_variable_value(*pVariable)
	   << "\"/>"
	   << _STD_ endl;
      }
    }
    os << "</mplInit>" << _STD_ endl;
  }
}


#ifdef ENABLE_RECOVERY
void T_system_debug::doPlanSteps(const char *cmd, const char *args) {
  int steps = atoi(args);
  if (steps <= 0) {
    _STD_ cerr << "Please supply a number of steps >= 0" << _STD_ endl;
  } else {
    for( ; steps > 0; steps--) {
      t_system.planner_progress();
    }
  }
}
#endif


bool T_system_debug::parse_cmd(const char* cmd, const char* args) {
  if (!strcmp("state-var-style", cmd)) {
    doStateVariableStyle(cmd, args);
  } else if (!strcmp("discrepancy", cmd)) {
    doDiscrepancy(cmd, args);
  } else if (!strcmp("transitions", cmd)) {
    doTransitions(cmd, args);
  } else if (!strcmp("values", cmd)) {
    doValues(cmd, args);
  } else if (!strcmp("times", cmd)) {
    doTimes(cmd, args);
  } else if (!strcmp("oldest-time", cmd)) {
    doOldestTime(cmd, args);
  } else if (!strcmp("cmds", cmd)) {
    doCommands(cmd, args);
  } else if (!strcmp("observables", cmd)) {
    doObservables(cmd, args);
  } else if (!strcmp("transitioned", cmd)) {
    doTransitioned(cmd, args);
  } else if (!strcmp("assign-mode", cmd)) {
    assign_mode(args);
  } else if (!strcmp("reset-conflicts", cmd)) {
    t_system.deleteNewestConflicts();
  } else if (!strcmp("export-ini-file", cmd)) {
    doExportIniFile(cmd, args);
  }
#ifdef ENABLE_RECOVERY
  else if (!strcmp("plan-steps", cmd)) {
    doPlanSteps(cmd, args);
    return true;
  }
#endif
  else {
    // Check to see if the lower level debugger can handle the command.
    return Conflict_db_debug::parse_cmd(cmd, args);
  }
  return true;
}


void T_system_debug::listener_usage() const {
  stream
    << "Transition commands:\n"
    << "---------------------\n"
    << "  values [time | now]         <-- values of all variables at time 'time'\n"
    << "  times  <var-name>           <-- values of variable at all times\n"
    << "  oldest-time                 <-- oldest timestep in theory\n"
    << "  cmds                        <-- print commanded variables\n"
    << "  observables                 <-- print observed variables\n"
    << "  transitioned                <-- print mode variables (they transition across time)\n"
    << "  transitions <mode-var-name> <-- print transitions on the mode\n"
    << "  discrepancy                 <-- cmds & obs in current inconsistency\n"
    << "  state-var-style [long|short] <-> show/set how to display state=si\n"
    << "  assign-mode <var>=<value>   --> set the value of a mode variable\n"

#ifdef ENABLE_RECOVERY
    << "  plan-steps n               --> set max number of actions in recovery plans\n"
#endif
    << _STD_ endl;
  Conflict_db_debug::listener_usage();
}


void T_system_debug::print_transitioned(Transitioned* var) {
  for (Transitioned::Table_type::iterator table_it =
	 var->indexed_transition_table.begin();
       table_it != var->indexed_transition_table.end(); ++ table_it) {
    Transitioned::Entry_type& entry = *table_it;

    // print out the command
    const Assignment *pAssignment = /* const_cast<Assignment*> */(entry.first);

    stream << "On cmd ";
    print_assignment(*pAssignment);
    stream << ":" << _STD_ endl;

    // Entry is a list of trans prototypes
    Slist<Transitioned::Proto_transition*>& transitionList = entry.second;

    {
      for (Slist<Transitioned::Proto_transition*>::iterator it =
	     transitionList.begin();
	   it != transitionList.end(); ++it) {
	Transitioned::Proto_transition *pProtoTransition = *it;
	print_proto_transition(*pProtoTransition);
	stream << _STD_ endl;
      }
    }
    stream << _STD_ endl;
  }

  stream << "Unindexed transitions" << _STD_ endl;
  stream << "---------------------" << _STD_ endl;
  
  {
    for (Slist<Transitioned::Proto_transition*>::iterator it =
	   var->unindexed_prototypes.begin();
	 it != var->unindexed_prototypes.end(); ++it) {
      Transitioned::Proto_transition *pProtoTransition = *it;
      print_proto_transition(*pProtoTransition);
      stream << _STD_ endl;
    }
  }
  stream << _STD_ endl;

//   return;

  stream << "Currently constrained by" << _STD_ endl;
  stream << "------------------------" << _STD_ endl;

  {
    for (Clause::ListOfClause::iterator it  = var->transition_clauses.begin();
	 it != var->transition_clauses.end(); ++it) {
      Clause *pClause = *it;
      if (!pClause->is_deleted()) {
	pretty_print_clause(*pClause);
	stream << _STD_ endl;
      }
    }
  }
  stream << _STD_ endl;
}


void T_system_debug::print_proto_assignment(const T_system::Proto_assignment& proto) {
  Variable* var      = proto.first;
  unsigned int index = proto.second;
  stream
    << get_var_name(*var) << "=" << get_variable_value(*var, index) << " ";
}


void T_system_debug::print_proto_transition(Transitioned::Proto_transition& proto) {

  // counter-intuitive: we print 'negatives' as-is, and 'positives' with
  // a '!' as if they were negative.  The reason: we're printing
  //     p & !q  ==>  consequent
  // Which in CNF is turned into
  //    !p | q | consequent.
  // The consequent is dealt with specially; that means we assume all
  // positive and negative literals in the formula are on the antecedant side
  // of the implication, and thus to print them in a more natural (yet
  // logically equivalent) way, we negate them.

  {    
    for (Slist<T_system::Proto_assignment>::iterator it =
	   proto.negatives.begin(); it != proto.negatives.end(); ++it) {
      T_system::Proto_assignment& protoAssignment = *it;
      print_proto_assignment(protoAssignment);
    }
  }

  {
    for (Slist<T_system::Proto_assignment>::iterator it =
	   proto.positives.begin(); it != proto.positives.end(); ++it) {
      T_system::Proto_assignment& protoAssignment = *it;
      stream << "!";
      print_proto_assignment(protoAssignment);
    }
  }

  stream << "=> ";
  print_proto_assignment(proto.consequent);
  stream << _STD_ endl;
}


Command* T_system_debug::find_current_command(const MBA_string name) {
  for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
    Variable *pVariable = t_system.get_present_variable(i);
    if (pVariable->is_command() && get_var_name(*pVariable) == name) {
      return static_cast<Command *>(pVariable);
    }
  }
  return NULL;
}


// Nobody calls this
Observable* T_system_debug::find_current_observable(const MBA_string name) {
  for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
    Variable *pVariable = t_system.get_present_variable(i);
    if (pVariable->is_observable() && get_var_name(*pVariable) == name) {
      return static_cast<Observable *>(pVariable);
    }
  }
  return NULL;
}


Transitioned* T_system_debug::find_current_mode(const MBA_string name) {
  for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
    Variable *pVariable = t_system.get_present_variable(i);
    if (pVariable->is_transitioned() && get_var_name(*pVariable) == name) {
      return static_cast<Transitioned *>(pVariable);
    }
  }
  return NULL;
}


void T_system_debug::find_discrepant_observations(Assignment::ListOfAssignment&
						  assignments) {
  // Collect up all clauses involved in the inconsistency
  Clause::ListOfClause clauses;
  t_system.get_solver()->last_conflict(clauses, Conflict_db::is_observable);

  for (Clause::ListOfClause::iterator it = clauses.begin();
       it != clauses.end(); ++it) {
    Clause *pClause = *it;
    // For each clause, check if it has a datum, which will be the
    // assignment it represents
    void* datum = pClause->get_datum();
    if (datum != NULL) {
      /// If there is an assignment, it has a variable.
      Assignment * pAssignment = (Assignment*) datum;
      Variable * pVariable = pAssignment->get_variable();
      // If the variable associated with the assignment is marked as an
      // observation, put that assignment conflicting assignment list.
      for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
	Variable *pCandidateVariable = t_system.get_present_variable(i);
	if (pCandidateVariable == pVariable && // faster test, better filter
	    pCandidateVariable->is_observable()) {
	  assignments.push_front(pAssignment);
	}
      }
    }
  }
}


void T_system_debug::find_discrepant_commands(Assignment::ListOfAssignment&
					      assignments) {
  // Collect up all clauses involved in the inconsistency
  Clause::ListOfClause clauses;
  t_system.get_solver()->last_conflict(clauses, Conflict_db::is_command);

  for (Clause::ListOfClause::iterator it = clauses.begin();
      it != clauses.end(); ++it) {
    Clause *pClause = *it;
    // For each clause, check if it has a datum, which will be the assignment
    // it represents
    void* datum = pClause->get_datum();
    if (datum != NULL) {
      // If there is an assignment, it has a variable.
      Assignment *pAssignment = static_cast<Assignment *>(datum);
      Variable *pVariable = pAssignment->get_variable();

      // If the variable associated with the assignment has the same name as a
      // command, put the assignment in the assignment list.

      MBA_string full_name = get_var_name(*pVariable);
      const char* name_chars = strchr(full_name.c_str(), '#');
      Command* current_cmd = NULL;

      if (name_chars) {
	// Move past the '#';
	current_cmd = find_current_command(MBA_string(name_chars + 1));
      } else {
	current_cmd = find_current_command(full_name);
      }

      if (current_cmd) {
	assignments.push_front(pAssignment);
      }
    }
  }
};


bool T_system_debug :: assign_mode(MBA_string assign_name) {
  MBA_string varname, valuename;
  split_assignment(assign_name, varname, valuename);

  // Map the name onto a Variable
  Variable *pVariable = find_variable(varname, true);

  if (pVariable == NULL) {
    _STD_ cerr << "No variable named '" << varname << "'" << _STD_ endl;
    return false;
  } else {

    // Map the value onto its index
    int valueIndex = find_prop_index(*pVariable, valuename, true);

    if (valueIndex == Conflict_db_debug::NOT_FOUND) {
      _STD_ cerr << "No value named '" << valuename << "'" << _STD_ endl;
      return false;
    } else {

      // Make sure it's a mode
      L2_assert(pVariable->is_transitioned(),
		L2_argument_error,
		("v" + MBA_string(pVariable->get_id()) +
		 " is not a mode variable"));
    
      // Ask the transitioned variable to forget its old value, ignore the
      // modeTransition, and take on the new value.
      static_cast<Transitioned*>(pVariable)->set_mode(t_system.get_solver(),
						      valueIndex);

      // See if it conflicts
      if (!t_system.get_solver()->propagate()) {
	_STD_ cerr
	  << "Inconsistent: unassign or change conflicting observations\n";
	return false;
      } else {
	return true;
      }
    }
  }
}

// Assume pVariable != NULL (caller does that checking)
bool T_system_debug::is_exogenously_assigned(Variable *pVariable) {
  if (!pVariable->is_assignable()) {
    // By definition, only an Assignable can be assigned exogenously
    return false;
  } else {
    const Assignable *pAssignable = static_cast<const Assignable *>(pVariable);
    return pAssignable->is_exogenously_assigned();
  }
}
