/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Non-flight debugging/inspection code for LTMS

// This file includes the implementation of the LTMS_debug class and a large
// number of internal functions to implement the debugging/inspecting
// functionality.

#include <debuggers/L2_string_map.h>
#include <debuggers/ltms_debug.h>
#include <tms/ptheory_listener.h>
#ifdef VXWORKS
#include <sysLib.h>
#endif

/***************************************************************************
  Constructor
 ***************************************************************************/

LTMS_debug::LTMS_debug(LTMS& tms, L2_string_map& themap,
		       _STD_ ostream& outstream) :
  map(themap),
  ltms(tms),
  stream(outstream),
  start_time(0),
  end_time(0),
  timing_enabled(false) {
}


/***************************************************************************
  Static variable initialization
 ***************************************************************************/

size_t LTMS_debug::max_explanation_depth = 10;


/***************************************************************************
  Utility functions
 ***************************************************************************/

// Return a string of "indent" space characters
// Apparently not used

MBA_string LTMS_debug::get_indent_string(unsigned indent) {
  if (indent == 0) {
    return "";
  } else {
    char iarray[MBA_MAX_LINE_SIZE];
    unsigned i = 0;
    while (i < indent) iarray[i++] = ' ';
    iarray[i++] = 0;
    return iarray;
  }
}


// Store the current time; see end_timing()
// Used to time propagation

void LTMS_debug::begin_timing() {
  start_time = clock();
}


// Write the number of seconds since the clock was started with begin_timing()
// Used to time propagation

void LTMS_debug::end_timing() {
  if (timing_enabled) {
    unsigned int end_time = clock();
    unsigned int elapsed = end_time - start_time;
    float seconds = (float) elapsed / (float) CLOCKS_PER_SEC;
    stream << seconds << "s elapsed" << _STD_ endl;
  }
}


// Send a usage message to "stream". Presented at the beginning of the
// listener and in case there is a bad command

void LTMS_debug::listener_usage() const {
  stream
    << "TMS commands:\n"
    << "---------------------\n"
    << "  remove  <clause>            --> remove a clause from theory\n"
    << "  add  <clause>               --> put a removed clause back in\n"
    << "  propagate                   --> perform inference\n"
    << "  partial_propagate           --> perform partial inference\n"
    << "  props  [string]             <-- propositions containing string\n"
    << "  true  [string]              <-- propositions that are true\n"
    << "  clauses  <string>           <-- clauses containing string\n"
    << "  time                        <-- toggle printout of timing info\n"
    << "  explain <proposition> \n"
    << "  support <proposition> \n"
    << "  ltms-stats                  <-- print out TMS statistics\n"
    << _STD_ endl;
}


/**
 * A callback predictate that is always true of its Clause.
 * \param clause a const reference to a Clause
 * \return true
 */
static bool always_true(const Clause&) { return true; }


// Print all Clauses that support the Proposition

void LTMS_debug::show_proposition_support(Proposition& rProposition) {
  stream <<"Note: returns first level support rather than assumptions"
	 << _STD_ endl;
  // Get the set of Clauses that support the Proposition
  Clause::ListOfClause support;
  ltms.get_proposition_support(rProposition, support, &(always_true));
  for (Clause::ListOfClause::iterator it = support.begin();
       it != support.end(); ++it) {
    Clause *pClause = *it;
    pretty_print_clause(*pClause);
    stream << _STD_ endl;
  }
}


// Print all Clauses that support the Proposition with the given name

void LTMS_debug::show_proposition_support (MBA_string& propositionName)  {
  Proposition* pProposition = find_proposition(propositionName);
  if (pProposition == NULL) {
    stream << "No such proposition found: " << propositionName
	   << _STD_ endl;
  } else {
    show_proposition_support(*pProposition);
  }
}


void LTMS_debug::doAdd(const char* args) {
  MBA_string propositionName(args);
  Clause* pClause = find_literal_clause(propositionName);
  if (pClause) {
    ltms.add(*pClause);
  } else {
    stream << "No such clause" << _STD_ endl;
  }
}


void LTMS_debug::doRemove(const char* args) {
  MBA_string propositionName(args);
  Clause* pClause = find_literal_clause(propositionName);
  if (pClause) {
    ltms.remove(*pClause);
  } else {
    _STD_ cout << "No such clause" << _STD_ endl;
  }
}


void LTMS_debug::doPartialPropagate(const char* args) {
  bool consistent;
  bool done = false;
  size_t steps = 50;
  size_t calls = 0;
    
  while (!done) {
    begin_timing();
    done = ltms.partial_propagate(steps, consistent);
    end_timing();
    calls++;
  }
  stream << "Used " << calls << " calls of " << steps << " steps."
	 << _STD_ endl;
  stream << (consistent ? "Consistent" : "Inconsistent") << _STD_ endl;
}


void LTMS_debug::doPropagate(const char* args) {
  begin_timing();
  bool consistent = ltms.propagate();
  end_timing();
  stream << (consistent ? "Consistent" : "Inconsistent") << _STD_ endl;
}


void LTMS_debug::doUnsatisfied(const char* args) {
  for(LTMS::UnsatType::iterator it = ltms.begin_unsatisfied();
      it != ltms.end_unsatisfied(); ++it) {
    Clause *pClause = *it;
    print_clause(*pClause);
    stream << _STD_ endl;
  }
}


void LTMS_debug::doStatus(const char* args) {
  MBA_string propositionName(args);
  Clause* pClause = find_literal_clause(propositionName);
  if (pClause) {
    print_clause(*pClause);
    stream << _STD_ endl;
  }
}

/// Returns true if cmd was known to the debugger

bool LTMS_debug::parse_cmd(const char* cmd, const char* args) {
  if (!strcmp("add", cmd)) {
    doAdd(args);
  } else if (!strcmp("clauses", cmd))  {
    cclauses(args,false);
  } else if (!strcmp("time", cmd))  {
    toggle_timing();
  } else if (!strcmp("support", cmd)) {
    MBA_string temp = MBA_string(args);
    show_proposition_support(temp);
  } else if (!strcmp("props", cmd)) {
    cprops(args,true);
  } else if (!strcmp("true", cmd))  {
    // Print true propositions whose name contains string "args"
    cprops(args,false);
  } else if (!strcmp("remove", cmd)) {
    doRemove(args);
  } else if (!strcmp("explain", cmd)) {
    MBA_string temp = MBA_string(args);
    explain_proposition(temp, 0);
  } else if (!strcmp("support", cmd)) {
    MBA_string temp = MBA_string(args);
    show_proposition_support(temp);
  } else if (!strcmp("partial_propagate",cmd)) {
    doPartialPropagate(args);
  } else if (!strcmp("propagate",cmd)) {
    doPropagate(args);
  } else if (!strcmp("ltms-stats", cmd) || !strcmp("stats", cmd)) {
    // Print the Proposition and Clause counts
    print_stats();
  } else if (!strcmp("unsat", cmd)) {
    // Print unsatisfied clauses
    doUnsatisfied(args);
  } else if (!strcmp("status", cmd)) {
    doStatus(args);
  } else {
    // Didn't understand the command
    return false;
  }
  return true;
}


// The read-execute loop. This has been superseded by cascading parse_cmd()

void LTMS_debug::listener() {
  char cmd[MBA_MAX_LINE_SIZE];
  char args[MBA_MAX_LINE_SIZE];

  listener_usage();
  for (;;) {
    stream << "LTMS> ";                    // prompt
    stream.flush();
    _STD_ cin >> cmd;
    if (!strcmp("exit",cmd)) {             // done listening
      return;
    } else {
      _STD_ cin.getline(args, MBA_MAX_LINE_SIZE);
      if (!parse_cmd(cmd, args)) {         // try to execute
	listener_usage();                  // if not, a little reminder
      }
    }
  }
}


// Like substring on a Proposition's name, but returns boolean, with true on
// null or empty string
// The Proposition* parameter can't yet be made const

bool LTMS_debug::prop_contains_string(Proposition* pProposition,
				      const char* substring) const {
  // Every proposition contains the empty string
  if (substring == NULL || strlen(substring) == 0) {
    return true;
  } else {
    return (get_prop_name(*pProposition).find(substring) != MBA_string::npos);
  }
}


// Print all true propositions (all propositions if verbose) whose names
// contain the given string

void LTMS_debug::cprops(const char* substring, bool verbose) {
  bool print_truth_value = verbose;
  for (Ptheory::prop_list::iterator it = ltms.begin_propositions();
       it != ltms.end_propositions(); ++it) {
    Proposition *pProposition = *it;
    if ((verbose || pProposition->isTrue()) &&
        (prop_contains_string(pProposition, substring))) {
      print_proposition(pProposition, print_truth_value);
      stream << _STD_ endl;
    }
  }
  stream << _STD_ endl;
}


// A disjunction with only positive literals means at least one must be true

void LTMS_debug::pretty_print_clause_no_negative(const Clause& clause) {
  bool print_truth_value = false;
  // A disjunction with only a single positive literal forces that literal to
  // be true
  if (1 == clause.number_of_positive_literals())  {
    stream << "Assertion: " ;
    Clause::iterator it = clause.begin_positive(); // The first and only
    Proposition *pProposition = *it;
    print_proposition(pProposition, print_truth_value);
  } else  {
    stream << "At least one: ";
    for (Clause::iterator it = clause.begin_positive();
	 it != clause.end_positive(); ++it) {
      Proposition *pProposition = *it;
      print_proposition(pProposition, print_truth_value);
    }
  }
  stream << _STD_ endl;
}


// A disjunction with only negations in it means that not all of the literals
// are true

void LTMS_debug::pretty_print_clause_no_positive(const Clause& clause) {
  bool print_truth_value = false;
  if (2 == clause.number_of_negative_literals()) {
    stream << "Only one: ";
  } else {
    stream << "Not all: ";
  }
  for (Clause::iterator it = clause.begin_negative();
       it != clause.end_negative(); ++it) {
    print_proposition(*it,print_truth_value);
  }
  stream << _STD_ endl;
}


// Print in the form of an implication

void LTMS_debug::pretty_print_clause_positive_and_negative(const Clause& clause) {
  bool print_truth_value = false;
  {
    bool printed_first = false; // Controls printing '&' before all but first
    for (Clause::iterator it = clause.begin_negative();
	 it != clause.end_negative(); ++it) {
      Proposition *pProposition = *it;
      if (printed_first) {
	stream << "& ";
	print_proposition(pProposition, print_truth_value);
      } else {
	printed_first = true;
	print_proposition(pProposition, print_truth_value);
      }
    }
  }
  stream << "==> ";
  {
    bool printed_first = false; // Controls printing '|' before all but first
    for (Clause::iterator it = clause.begin_positive();
	 it != clause.end_positive(); ++it) {
      Proposition *pProposition = *it;
      if (printed_first) {
	stream << "| ";
	print_proposition(pProposition, print_truth_value);
      } else  {
	printed_first = true;
	print_proposition(pProposition, print_truth_value);
      }
    }
  }
  stream << _STD_ endl;
}


void LTMS_debug::pretty_print_clause (const Clause& clause) {
  // This does not currently print the clause's datum, as that is not yet
  // supported by the model reader.

  // A DNF clause of the form ~a or ~b  or ~c  or d or e,
  // which is equivalent to ~(a & b & c) or (d or e)
  // which is of course equivalent to (a & b & c) -> (d or e).
  // The degenerate form nil -> (d or e) is an assertion and
  // the form (a & b & c) -> nil means it's inconsistent for
  // a, b and c to be true simultaneously.

  if (0 == clause.number_of_negative_literals()) {
    pretty_print_clause_no_negative(clause);
  } else if (0 == clause.number_of_positive_literals())  {
    pretty_print_clause_no_positive(clause);
  } else {
    pretty_print_clause_positive_and_negative(clause);
  }
}


// Print the Clause's negative and then positive Propositions

void LTMS_debug::print_clause(const Clause& rClause)
{
  // Are these mutually exclusive?
  if (rClause.is_deleted())   { stream << "D:"; }
  if (rClause.is_in_fringe()) { stream << "F:"; }

  { // Print the negated Propositions
    for (Clause::iterator it = rClause.begin_negative();
	 it != rClause.end_negative(); ++it)  {
      Proposition *pProposition = *it;
      stream << "!" ;
      print_proposition(pProposition);
      stream << " ";
    }
  }
  { // Print the non-negated Propositions
    for (Clause::iterator it = rClause.begin_positive();
	 it != rClause.end_positive(); ++it)  {
      Proposition *pProposition = *it;
      print_proposition(pProposition);
      stream << " ";
    }
  }
}


// Auxiliary to explain_proposition()

void LTMS_debug::explain_clause (Clause& clause, size_t indent)	{
  // We don't want to explain the prop this clause is supporting,
  // otherwise we will end up explaining this clause again.
  const Proposition* pSatisfyingProposition = clause.get_satisfying_prop();

  print_trace_indent(indent);
  stream << "Via ";
  pretty_print_clause(clause);

  // and trace back from the Clause's literals
  if (max_explanation_depth < (1 + indent)) {
    // then nothing more is to be printed.  Merely print some ellipses
    print_trace_indent(1 + indent);
    stream <<  "........." << _STD_ endl;
  }

  //	stream << _STD_ endl << _STD_ endl;;

  // Tracks whether there is another Proposition that supports this Clause, or
  // if the supporting Clause is the satisfying Clause.  In the latter case,
  // this is an assertion.

  bool other_support = false;

  { // Do non-negated Propositions first
    for (Clause::iterator   it = clause.begin_positive();
	 it != clause.end_positive(); ++it) {
      Proposition *pProposition = *it;
      if (pSatisfyingProposition != pProposition)  {
	if (!other_support) {
	  stream << _STD_ endl;
	  print_trace_indent(indent);
	  stream << "Support: " << _STD_ endl;
	  other_support = true;
	}
	explain_proposition(*pProposition, 1 + indent); }
    }
  }
  { // Do negated Propositions second
    for (Clause::iterator it = clause.begin_negative();
	 it != clause.end_negative(); ++it) {
      Proposition *pProposition = *it;
      if (pSatisfyingProposition != pProposition) {
	if (!other_support) {
	  stream << _STD_ endl;
	  print_trace_indent(indent);
	  stream << "Support: " << _STD_ endl;
	  other_support = true;
	}
	explain_proposition(*pProposition, 1 + indent);
      }
    }
  }
  stream << _STD_ endl;
}


void LTMS_debug::explain_proposition (MBA_string& propositionName,
				      size_t indent) {
  Proposition* pProposition= find_proposition(propositionName);
  if (pProposition == NULL) {
    stream << "No such proposition found: " << propositionName << _STD_ endl;
  } else {
    explain_proposition(*pProposition, indent);
  }
}


void LTMS_debug::explain_proposition (Proposition& rProposition,
				      size_t indent) {
  // First print out the truth value of the proposition
  print_trace_indent(indent);
  print_proposition(&rProposition);
  
  // Proceed only if the proposition is set by unit propagation.
  Clause* pSupportingClause = rProposition.get_supported_by();
  if (pSupportingClause) {
    explain_clause (*pSupportingClause, indent);
  }
}


Clause* LTMS_debug::find_literal_clause(MBA_string& propositionName)  {
  // HAVE TO MODIFY TO SUPPORT NEGATIVE LITERALS
  Proposition* pProposition = find_proposition(propositionName);
  if (pProposition == NULL) {
    stream << "No such proposition found: " << propositionName << _STD_ endl;
    return NULL;
  } else {
    // Find the clause of length 1 where this prop appears with a positive sign
    for (Proposition::iterator it = pProposition->begin_positive();
	 it != pProposition->end_positive(); ++it) {
      Clause *pClause = *it;
      if (pClause->number_of_literals() == 1) {
	return (pClause);
      }
    }
    return NULL;
  }
}


// This is fairly brain dead.

void LTMS_debug::print_trace_indent(size_t indent) {
  for (size_t i = 0; i < indent; ++i) {
    stream << "   ";
  }
  stream << indent << ": ";
}


// Print the name of the proposition and, if enabled, its truth value
// The Proposition parameter can't yet be made const

void LTMS_debug::print_proposition(Proposition* pProposition,
				   bool print_truth_value) {
  stream << get_prop_name(*pProposition) << " ";
  if (print_truth_value) {
    const char *truthValueStr;
    if      (pProposition->isFalse()) { truthValueStr = ":FALSE"; }
    else if (pProposition->isTrue())  { truthValueStr = ":TRUE"; }
    else                              { truthValueStr = ":UNKNOWN"; }
    stream << truthValueStr << " ";
  }
}


// Call print_proposition() for each Proposition

void LTMS_debug::print_propositions() {
  for (Ptheory::prop_list::iterator it = ltms.begin_propositions();
       it != ltms.end_propositions(); ++it) {
    Proposition *pProposition = *it;
    print_proposition(pProposition);
  }
}


// Write the number of Propositions and Clauses

void LTMS_debug::print_stats() {
  stream << "LTMS has "
	 << ltms.num_propositions()
	 << " propositions, "
	 << ltms.num_clauses()
	 << " clauses\n";
}


// Map the function over all Clauses

void LTMS_debug::clause_iterator(ClauseCallbackObject& func) {
  for (Ptheory::clause_list::iterator it = ltms.begin_clauses();
       it != ltms.end_clauses(); ++it) {
    Clause *pClause = *it;
    func(*pClause);
  }
}


/**
 * The sole method for the callback object; print Clause it has a Proposition
 * whose print name contains a given substring.
 */

void LTMS_debug::ClauseStringChecker::operator() (Clause& rClause) {
  if (!rClause.is_deleted() || show_deleted) {
    { // Look in negated Propositions first
      for (Clause::iterator it = rClause.begin_negative();
	   it != rClause.end_negative(); ++it) {
	Proposition *pProposition = *it;
	if (db->prop_contains_string(pProposition, substring) ) {
	  db->print_clause(rClause);
	  stream << _STD_ endl;
	  return;
	}
      }
    }
    { // Look in non-negated propositions second
      for (Clause::iterator it = rClause.begin_positive();
	   it != rClause.end_positive(); ++it) {
	Proposition *pProposition = *it;
	if (db->prop_contains_string(pProposition, substring) ) {
	  db->print_clause(rClause);
	  stream << _STD_ endl;
	  return;
	}
      }
    }
  }
};


// Map the checker function over all Clauses

void LTMS_debug::cclauses(const char* substring, bool show_deleted) {
  ClauseStringChecker callback =
    ClauseStringChecker(this, stream, substring, show_deleted);
  clause_iterator(callback);
}


// Map the name string onto its Proposition

Proposition* LTMS_debug::find_proposition(MBA_string& propositionName) const {
  return map.get_prop_by_name(propositionName);
}


// Map the Proposition onto its name string
// The parameter can't yet be made const

MBA_string LTMS_debug::get_prop_name(Proposition& rProposition) const {
  return map.get_name_of_prop(&rProposition);
}
