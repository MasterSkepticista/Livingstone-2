/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Non-flight debugging/inspection code for Conflict_db
//

#include <ctype.h> // isspace
#include <conflict_db/candidate.h>
#include <debuggers/domain.h>
#include <debuggers/L2_string_map.h>
#include <debuggers/conflict_db_debug.h>
#include <livingstone/L2_assert.h>

/***************************************************************************
  Constructor.
 ***************************************************************************/

Conflict_db_debug::Conflict_db_debug(Conflict_db &system,
				     L2_string_map &themap,
				     _STD_ ostream& outstream) :
  LTMS_debug(*(system.get_solver()), themap, outstream), db(system),
  stream(outstream) { }


// Return the name string of the Variable's value

MBA_string Conflict_db_debug::get_variable_value(const Variable& variable) {
  return get_variable_value(variable, variable.get_value_index());
}


// Return the indexed member of the Variable's values

MBA_string Conflict_db_debug::get_variable_value(const Variable& variable,
						 int idx) {
  return
    (idx < 0) ?
    MBA_string("unknown") :
    map.get_variable_values(variable) [idx] ;
}


// Assign the Assignable the value specified by its name

void
Conflict_db_debug::assign(Assignable& assignable,
			  const MBA_string& value_name) {
  Assignment *pAssignment = find_assignment(assignable, value_name, true);
  assignable.assign(pAssignment);
  assignable.set_exogenously_assigned(true);
}


static bool isAllWhiteSpace(const char* s) {
  for (unsigned i = 0; i < strlen(s); i++) {
    if (!isspace(s[0])) {
      // It contains a non-whitespace character
      return false;
    }
  }
  // It is all white space
  return true;
}


// Carry out the <variable>=<value> command in the input string

void
Conflict_db_debug::assign(const char* string) {
  // Copy argument so it can be freely modified
  char* assignment_str = static_cast<char *>(malloc(strlen(string) + 1));
  strcpy(assignment_str, string);
    char *remaining_str = assignment_str;
    const char* const end=assignment_str + strlen(assignment_str);

    while (remaining_str!=NULL && !isAllWhiteSpace(remaining_str)) {

        char * const first_equal = strchr(remaining_str,'=');

        L2_assert(first_equal, L2_parse_scenario_error,
                (MBA_string("No '=' in assignment `")+remaining_str+"'"));
        L2_assert(first_equal != (end-1), L2_parse_scenario_error,
                (MBA_string("No values in assignment `")+remaining_str+"'"));

        /// Characters before the = are the first variable.
        char *variable_str = remaining_str;

        /// skip any spaces and check to see that something's left
        for(;variable_str!=first_equal;variable_str++)
            if (*variable_str != ' ')
                break;

        L2_assert(first_equal != variable_str, L2_parse_scenario_error,
                (MBA_string("No variable in assignment `")+remaining_str+"'"));

        //  Characters after the = are the rest of the command.
        remaining_str=first_equal+1;

        /// skip any spaces and check to see that something's left
        for(;remaining_str!=end;remaining_str++)
            if (*remaining_str != ' ')
                break;

        L2_assert(remaining_str != end, L2_parse_scenario_error,
                (MBA_string("No values in assignment `")+remaining_str+"'"));

        /// Split off the variable name by placing a NULL.
        *first_equal=0;

        for(char* cursor1 = variable_str; cursor1 != first_equal; cursor1++)
            if ((' ' == *cursor1) || ('=' == *cursor1)) {
                *cursor1=0;
                break;
            }


        MBA_string varname(variable_str);
        Assignable* var = find_assignable(varname, true);

        /// Next, collect up values separated by | sign, up to the first
        /// space or the end of line.

        char *value_strings[20];
        unsigned int number_values = 0;

        char* cursor=remaining_str;
        while(cursor<=end) {

            /// Something has demarcated the end of the current value we
            /// are scanning over
            if ((cursor==end) ||  (' '==*cursor) ||  ('|'==*cursor))  {

                // Record the start of the value and insert a marker
                // for the end of the value.
                value_strings[number_values++]=remaining_str;

                /// Found the end of command. This is also the end of the
                /// current set of values. Break out of the loop
                /// that collects up values.
                if(cursor==end)  {
                    remaining_str=NULL;
                    break;
                }

                // Not at the end of the command string, but at the end of
                // a value.  This is caused by a space or a |.
                // Update where the rest of the command starts.
                const char terminator = *cursor;
                *cursor=0;
                remaining_str=cursor+1;

                // If we found a space, this marks the end of the current set
                // of values. Break out of the loop that collects up values.
                if (' '==terminator)
                    break;

            }
            cursor++;
        }

        /// Assign the 1 or more values we collected
        if (number_values == 1) {
            assign(*var, value_strings[0]);
        } else {
            /// Do the trickier thing of assigning the disjunction
            _STD_ cout << "Disjunctive assignment not yet implemented"
		      << _STD_ endl;
        }

    }
    free(assignment_str);
}


// Look up the Variable with the given name and unassign it

void
Conflict_db_debug::unassign(const MBA_string& name) {
  Assignable* pAssignable = find_assignable(name, true);
  if (pAssignable != 0) {
    pAssignable->unassign(); // fails silently
    pAssignable->set_exogenously_assigned(false);
  }
}


// Print each value in the Domain as a comma-separated list

void
Conflict_db_debug::print_domain_members(const Domain* pDomain) {
  Domain::iterator it = pDomain->begin();
  stream << *it;
  for (++it ; it != pDomain->end() ; ++it)
    stream << ", " << *it;
  stream << _STD_ endl;
}


// Prints out every Domain whose name contains the substring.
// The name means "see domains"

void
Conflict_db_debug::cdomains(const MBA_string& substring) {
  for (Slist<const Domain*>::iterator it = map.begin_domains();
       it != map.end_domains(); ++it) {
    const Domain *pDomain = *it;
    if (pDomain->get_name().find(substring)!= MBA_string::npos)  {
      stream << pDomain->get_name() << ": " ;
      print_domain_members(pDomain);
    }
  }
}


// Print all Domains

void
Conflict_db_debug::print_domains() { cdomains(""); }


// Prints all Assumptions

void
Conflict_db_debug::print_assumptions()
{
  for (Assumption::ListOfAssumption::iterator it = db.begin_assumptions();
       it != db.end_assumptions(); ++it) {
    Assumption *pAssumption = *it;
    print_variable_and_value(*pAssumption);
  }
}


// Prints all Variables that are Assignable

void
Conflict_db_debug::print_assignables()
{
  for (Assignable::ListOfAssignable::iterator it = db.begin_assignables();
       it != db.end_assignables(); ++it) {
    Assignable *pAssignable = *it;
    print_variable_and_value(*pAssignable);
  }
}


// Print all Variables and their values to the default _STD_ ostream

void
Conflict_db_debug::print_all_variable_values()
{
  for (Variable::ListOfVariable::iterator it =
	 db.begin_variables(); it != db.end_variables(); ++it) {
    Variable *pVariable = *it;
    print_variable_and_value(*pVariable);
  }
}


// Print all Variables and their values to the specified _STD_ ostream

void Conflict_db_debug::print_all_variable_values(_STD_ ostream & outstream)
{
  for (Variable::ListOfVariable::iterator it =
	 db.begin_variables(); it != db.end_variables(); ++it) {
    Variable *pVariable = *it;
    print_variable_and_value(*pVariable, outstream);
  }
}


// Print all Conflicts

void
Conflict_db_debug::print_conflicts() {
  for(Conflict::ListOfConflict::iterator it = db.begin_conflicts();
      it != db.end_conflicts(); ++it) {
    const Conflict *pConflict = *it;
    print_conflict(*pConflict);
  }
}


// Print the "best" Conflict if there is one

void
Conflict_db_debug::print_conflict() {
  stream << "Known conflicts in any DB state: "
	 << db.get_conflicts()->size()
	 << _STD_ endl;
  stream << "Searching for conflict in the current state..."
	 << _STD_ endl;
  const Conflict* pConflict = db.conflict();
  if (NULL == pConflict) {
    stream << "DB is consistent" << _STD_ endl;
    return;
  }
  print_conflict(*pConflict);
}


// Forwarded from the T_system to handle a command line

bool Conflict_db_debug::parse_cmd(const char* cmd, const char* args) {
  if (!strcmp("domains", cmd)) {
    if (0 == strlen(args)) { print_domains();} // All domains
    else { cdomains(args); }                   // Specified domains
  } else if (!strcmp("variables", cmd)) {
    unsigned n = 0;
    for (Variable::ListOfVariable::iterator it = db.begin_variables();
	 it != db.end_variables(); ++it) {
      Variable *pVariable = *it;
      ++n;
      print_variable_and_value(*pVariable);
    }
    stream << "Printed " << n << " variables\n";
  } else if (!strcmp("assign", cmd)) {
    assign(args);
  } else if (!strcmp("unassign", cmd)) {
    unassign(args);
  } else if (!strcmp("constraints", cmd))  {
    print_constraints(args);
  } else if (!strcmp("conflict", cmd)) {
    print_conflict();
  } else if (!strcmp("assumptions", cmd)) {
    print_assumptions();
  } else if (!strcmp("assignables", cmd)) {
    print_assignables();
  } else if (!strcmp("conflicts", cmd)) {
    print_conflicts();
  } else if (!strcmp("stats", cmd)) {
    print_stats();
    // allow the tms to print its own stats, too
    LTMS_debug::parse_cmd(cmd, args);
  } else if (!strcmp("cdb-stats", cmd)) {
    print_stats();
  } else {
    // Check to see if the lower level debugger can handle the command.
    return LTMS_debug::parse_cmd(cmd, args);
  }
  return true;
}


// The "help" message

void Conflict_db_debug::listener_usage() const {
  stream
    << "Conflict db commands:\n"
    << "---------------------\n"
    << "  assign <var>=<value>        --> assign variable to value\n"
    << "  unassign <var>              --> unassign the variable\n"
    << "  conflict                    <-- a conflict in the current state\n"
    << "  conflicts                   <-- all known conflicts\n"
    << "  constraints [string]        <-- constraints containing the string\n"
    << "  domains  [string]           <-- domains whose name contains string\n"
    << "  variables                   <-- list all variables, unsorted\n"
    << "  assumptions                 <-- variables that are assumptions\n"
    << "  assignables                 <-- variables that are assignable\n"
    << "  cdb-stats                   <-- number of variables and conflicts\n"
    << _STD_ endl;
  LTMS_debug::listener_usage();
}


// For each Clause, print the Clause's first Proposition with the substring

void
Conflict_db_debug::print_constraints(const char* substring) {
  LTMS* ltms = db.get_solver();
  for (Ptheory::clause_list::iterator clause_it = ltms->begin_clauses();
       clause_it != ltms->end_clauses(); ++clause_it) {
    Clause* pClause = *clause_it;
    bool found = false;
    if (!pClause->is_deleted()) {
      {
	for (Clause::iterator it = pClause->begin_negative();
	     it != pClause->end_negative(); ++it) {
	  Proposition *pProposition = *it;
	  if (LTMS_debug::prop_contains_string(pProposition, substring) ) {
	    found = true;
	    LTMS_debug::pretty_print_clause(*pClause);
	    stream << _STD_ endl;
	    break;
	  }
	}
      }
      if (!found) {
	for (Clause::iterator it = pClause->begin_positive();
	     it != pClause->end_positive(); ++it) {
	  Proposition *pProposition = *it;
	  if (LTMS_debug::prop_contains_string(pProposition, substring) ) {
	    LTMS_debug::pretty_print_clause(*pClause);
	    stream << _STD_ endl;
	    break;
	  }
	}
      }
    }
  }
  stream << _STD_ endl;
}


// Print the number of Variables and Conflicts

void Conflict_db_debug::print_stats() {
  stream << "Conflict_db has "
	 << db.size()
	 << " variables, "
	 << db.num_conflicts()
	 << " conflicts\n";
}


/***************************************************************************
  Used by tracker_debug
 ***************************************************************************/

// Print the Variable and its value to the default _STD_ ostream

void Conflict_db_debug::print_variable_and_value(const Variable& variable) {
  stream << get_var_name(variable) << '=' << get_variable_value(variable);
  if (variable.is_assignable()) {
    const Assignable& ass = static_cast<const Assignable&>(variable);
    if (!ass.get_current())
      stream << "\t  (inferred)";
  }
  stream << _STD_ endl;
}


// Print the Variable and its value to the specified _STD_ ostream

void Conflict_db_debug::print_variable_and_value(const Variable& var,
						 _STD_ ostream &outstream) {
  outstream << get_var_name(var) << '=' << get_variable_value(var);
  if(var.is_assignable()) {
    const Assignable& ass = static_cast<const Assignable&> (var);
    if (!ass.get_current())
      outstream << "\t  (inferred)";
  }
  outstream << _STD_ endl;
}


/***************************************************************************
  Handling variable names.
 ***************************************************************************/
MBA_string Conflict_db_debug::get_var_name(const Variable& variable) {
  return map.get_variable_name(variable);
}


Variable *Conflict_db_debug::find_variable(const MBA_string& name,
					   bool assert_found) {
  if (map.has_variable_named(name)) {
    return &map.get_variable_by_name(name);
  } else {
    L2_assert(!assert_found,
	      L2_not_found_error,
	      ("No variable named `"+name+"'"));
    return 0;
  }
}


int Conflict_db_debug::find_prop_index(const Variable& var,
				       const MBA_string& name,
				       bool assert_found) {
  const MBA_string *value_names = map.get_variable_values(var);
  for (unsigned i = 0; i < var.get_nvalues(); ++i) {
    if (value_names[i] == name) { return i; }
  }
  // not found
  L2_assert(!assert_found,
	    L2_not_found_error,
            ("Variable " + map.get_variable_name(var) +
	     " has no value `" + name + "'"));
  return NOT_FOUND;
}


Assignable* Conflict_db_debug::find_assignable(const MBA_string& name,
					       bool assert_found) {
  // Rather than looking for a variable and casting it if it
  // is assignable, a function for which only members of the
  // Assignable class should return true, we could look through
  // the Conflict_db's list of assignable variables.

  Variable* pVariable = find_variable(name, assert_found);
  // Would have thrown already if assert_found were true
  if (pVariable == NULL) {
    // There is no Variable with the given name
    return NULL;
  } else {
    if (!pVariable->is_assignable()) {
      L2_assert(!assert_found,
		L2_argument_error,
		("`"+name+"' is not assignable"));
      // The Variable was found but is not Assignable
      return NULL;
    } else {
      // The Variable was found and is assignable
      return static_cast<Assignable*>(pVariable);
    }
  }
}


/***************************************************************************
  Finding an assignment given some of the information.
 ***************************************************************************/

// Split a user-input string; allows for any amount of whitespace
// but throws an exception if it doesn't look like an assignment.
void Conflict_db_debug::split_assignment(const MBA_string& assign_str,
					 MBA_string& varname,
					 MBA_string& valuename)
{
    const char *str = assign_str.c_str();
    const char *namep;

    /////// Find the variable name
    // skip leading whitespace
    while(*str && isspace(*str)) ++str;

    // that's the start of the variable name
    namep = str;

    // skip until an equal sign or whitespace
    while(*str && *str!='=' && !isspace(*str)) ++str;
    L2_assert(*str, L2_parse_scenario_error,
            ("no '=' found in `" + assign_str + "'"));

    // end of variable name; create the string
    // abc=d  ; namep=0, str=3, str-namep=3, strlen("abc") = 3
    varname = MBA_string(namep, (str-namep));

    /////// Find the value name
    // skip until equal sign
    while(*str && *str!='=') ++str;
    L2_assert(*str, L2_parse_scenario_error,
            ("no '=' found in `" + assign_str + "'"));

    // skip equal sign, then whitespace
    ++str;
    while(*str && isspace(*str)) ++str;

    // that's the start of the value name
    namep = str;

    // skip until whitespace or end of string
    while(*str && !isspace(*str)) ++str;
    // don't assert; either is ok ; note this allows garbage at the end.

    // abc=d  ; namep=4, str=5, str-namep=1, strlen("d")=1
    valuename = MBA_string(namep, (str-namep));
}


Assignment* Conflict_db_debug::find_assignment(const MBA_string& assign_str,
					       bool assert_found) {
  MBA_string varname, valuename;
  // Parse <variable>=<value> into <variable> and <value>
  split_assignment(assign_str, varname, valuename);
  return find_assignment(varname, valuename, assert_found);
}


Assignment* Conflict_db_debug::find_assignment(const MBA_string& variableName,
					       const MBA_string& valueName,
					       bool assert_found) {
  Assignable *pAssignable = find_assignable(variableName, assert_found);
  // would have thrown if assert_found
  return
    (pAssignable == NULL) ?
    NULL :
    find_assignment(*pAssignable, valueName, assert_found);
}


Assignment* Conflict_db_debug::find_assignment(Assignable& assignable,
					       const MBA_string& valueName,
					       bool assert_found) {
  int idx = find_prop_index(assignable, valueName, assert_found);
  // would have thrown if assert_found
  return
    (idx == NOT_FOUND) ?
    NULL :
    assignable.get_assignment(idx);
}



/***************************************************************************
  Printing things.
 ***************************************************************************/

// Print the Variable's name, the value and the weight and return 1

unsigned Conflict_db_debug::print_assignment(_STD_ ostream& outstream,
					     const Assignment& assignment,
					     unsigned indent) {
  const Variable *pVariable = assignment.get_variable();
  outstream
    << get_indent_string(indent)
    << get_var_name(*pVariable)
    << "="
    << get_variable_value(*pVariable, assignment.get_index())
    << " :"
    << assignment.get_weight()
    << _STD_ endl;
  return 1;
}


// Print each Assignment of the Candidate

void Conflict_db_debug::print_candidate(_STD_ ostream& outstream,
					const Candidate& candidate,
					unsigned indent) {
  int lineCount = 0;
  for (unsigned index = 0; index < candidate.size(); index++)  {
    // print_assignment may return 0
    lineCount += print_assignment(outstream, 
				  *candidate.get_assignment(index),
				  indent);
  }
  if (lineCount == 0) {
    outstream << get_indent_string(indent)
	      << "<the empty candidate>"
	      << _STD_ endl;
  }
  outstream << _STD_ endl;
}


// Print each Assignment of the Conflict

void Conflict_db_debug::print_conflict(_STD_ ostream& outstream,
				       const Conflict& conflict) {
  outstream << "Conflict: " ;
  for (Conflict::const_iterator it = conflict.begin();
       it != conflict.end(); ++it) {
    const Assignment *pAssignment = *it;
    print_assignment(*pAssignment);
    if (it != conflict.end()) { outstream << ' '; }
  }
  outstream << _STD_ endl;
}
