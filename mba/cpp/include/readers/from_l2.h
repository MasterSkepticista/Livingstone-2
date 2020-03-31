/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_l2.h,v 1.16 2004/01/28 23:31:39 lbrown Exp $ */
#ifndef from_l2_HEADER
#define from_l2_HEADER

#include <readers/L2_file_reader.h>
#include <readers/L2_file.h>

/**
 * The maximum length (in characters) of a line in a *.l2 model file.
 * The line of observed variables has been known to be quite large.
 */

#define L2_MODEL_FILE_MAX_LINE_SIZE 4096

// Forward declarations; these are mostly the data types that we parse.
// We include enumeration.h, et al., in parser.cpp.
class L2rEnumeration;
class L2rVariable;
class L2rProposition;
class L2rClause;
class L2rTransition;

/**
 * Interface from the L2 model file format into memory.
 * The result of reading the model file is an L2_file object.
 */

class from_l2 : public L2_file_reader {
 private:

  // The value must be different from that of L2rTranstion::ANY_MODE
  enum private_constants { NOT_AN_INTEGER = -2 };

  /// The file we're parsing.
  FILE *the_file;

  /// The line being parsed, read in with readline().
  char line[L2_MODEL_FILE_MAX_LINE_SIZE];

  /// Line number in parsing, starting at 1.
  size_t line_number;

  /// Character index in parsing, with respect to the current line.
  unsigned index;

  // Flags to make sure we don't parse the same thing twice.

  /// Whether the "background" section has been parsed
  bool didBackground_:1;
  /// Whether the "clauses" section has been parsed
  bool didClauses_:1;
  /// Whether the "commandss" section has been parsed
  bool didCommands_:1;
  /// Whether the "enums" section has been parsed
  bool didEnums_:1;
  /// Whether the "modess" section has been parsed
  bool didModes_:1;
  /// Whether the "observed" section has been parsed
  bool didObserved_:1;
  /// Whether the "propositions" section has been parsed
  bool didPropositions_:1;
  /// Whether the "transitions" section has been parsed
  bool didTransitions_:1;
  /// Whether the "variables" section has been parsed
  bool didVariables_:1;
  /// Whether the "initial" section has been parsed
  bool didInitial_:1;

  /// The pathname of the input model file, supplied in the constructor.
  MBA_string filename_;

 public:
  /**
   * This object reads from the file with pathname "fname" into the destination
   * L2_file object.
   * Call from_l2::read() to read the model file.
   */
  from_l2(L2_file *f, MBA_string fname) : L2_file_reader(f), the_file(0),
    line_number(0), index(0), filename_(fname) {
    // This could be done in the initializer list, but it would be gross.
    didBackground_    =
      didClauses_     =
      didCommands_    =
      didEnums_       =
      didModes_       =
      didObserved_    =
      didPropositions_=
      didTransitions_ =
      didVariables_   =
      didInitial_     =
      false;
  }
  
  /**
   * Close the model file.
   */
  virtual ~from_l2();

  /**
   * The API action to read the model file into the destination object.
   * This method must be called no more than once. The first line of the file
   * must be a "magic cookie" string, currently "L211". After that come several
   * "sections" for the different kinds of specifications.
   * \return whether the model file has been successfully read in.
   */ 
  bool read();

 private:
  /**
   * Return the ith L2rEnumeration in the L2_file.
   * \param the index of the enum
   * \return the ith enum
   */
  const L2rEnumeration *getEnum(unsigned i) const {
    return get_dest()->getEnum(i);
  }

  /**
   * Return the ith L2rVariable in the L2_file.
   * \param the index of the Variable
   * \return the ith Variable
   */
  const L2rVariable *getVar(unsigned i) const {
    return get_dest()->getVar(i);
  }

  /**
   * Return the ith L2rProposition in the L2_file.
   * \param the index of the Proposition
   * \return the ith Proposition
   */
  const L2rProposition *getProp(unsigned i) const {
    return get_dest()->getProp(i);
  }

  /**
   * Return the ith L2rClause in the L2_file.
   * \param the index of the Clause
   * \return the ith Clause
   */
  const L2rClause *getClause(unsigned i) const {
    return get_dest()->getClause(i);
  }

  /// Return the number of enums.
  size_t nenums()   const { return get_dest()->nenums();   }
  /// Return the number of Variables.
  size_t nvars()    const { return get_dest()->nvars();    }
  /// Return the number of Propositions.
  size_t nprops()   const { return get_dest()->nprops();   }
  /// Return the number of Clauses.
  size_t nclauses() const { return get_dest()->nclauses(); }

  /**
   * Parse the section that specifies which of the Clauses are "background"
   * (i.e., non-transition) Clauses.
   * Preconditions: Clauses must have been parsed; background Clauses must not
   * have been parsed.
   * The section consists of one line containing a count of the number of
   * background Clauses, followed by that number of Clause ID lines.
   * \param has_debug_info unused
   * \return whether the "background" section was successfully parsed
   */
  bool parse_background (bool);

  /**
   * Parse the section that specifies the Clauses.
   * Preconditions: Propositions must have been parsed; Clauses must not have
   * been parsed.
   * The section contains two lines, each containing a count: nominal
   * Transitions and failure Transitions, followed by those numbers of
   * Transition declaration lines. A Transition declaration line consists of a
   * Variable ID, the value index of the "from" state and the value index of
   * the "to" state, preceded, if the section name was prefixed with "dbg_",
   * by the print name of the Transition.
   * \param has_debug_info whether print names are associated with the entities
   * \return whether the "propositions" section was successfully parsed
   */
  bool parse_clauses	(bool);

  /**
   * Parse the section that specifies which of the Variables are Command
   * Variables.
   * Preconditions: Variables must have been parsed.
   * \param has_debug_info unused
   * \return whether the "variables" section was successfully parsed
   */
  bool parse_commands	(bool);

  /**
   * Parse the section that specifies the "enums" (data types for Variables).
   * Preconditions: none.
   * The section contains one line containing a count of the number of enums,
   * followed by that number of enum declaration lines. An enum declaration
   * line consists of the cardinality of the data type followed, if the
   * section name was prefixed with "dbg_", by the print name of the enum and
   * the print name of its members. The number of members must equal the
   * cardinality.
   * \param has_debug_info whether print names are associated with the entities
   * \return whether the "enums" section was successfully parsed
   */
  bool parse_enums	(bool);

  /**
   * Parse the section that specifies which of the Variables are mode (i.e.,
   * Transitioned) Variables.
   * Preconditions: Variables must have been parsed.
   * \param has_debug_info unused
   * \return whether the "modes" section was successfully parsed
   */
  bool parse_modes   	(bool);

  /**
   * Parse the section that specifies which of the Variables are Observable
   * Variables.
   * Preconditions: Variables must have been parsed.
   * \param has_debug_info unused
   * \return whether the "observed" section was successfully parsed
   */
  bool parse_observed	(bool);

  /**
   * Parse the section that specifies the Propositions.
   * Preconditions: Enums and Variables must have been parsed; Propositions
   * must not have been parsed.
   * The section contains one line containing four counts: positive
   * Propositions, negative Propositions, same Propositions and different
   * Propositions, followed by those numbers of Proposition declaration lines.
   * A Proposition declaration line depends on what kind it is. For positive
   * and negative Propositions, it consists of a Variable ID followed by a
   * value index. For same and different Propositions, it consists of two
   * Variable IDs.
   * \param has_debug_info unused
   * \return whether the "propositions" section was successfully parsed
   */
  bool parse_propositions(bool);

  /**
   * Parse the section that specifies the Transitions.
   * Preconditions: Enums, Variables and Clauses must have been parsed;
   * Transitions must not have been parsed.
   * The section contains one line containing a count of the number of Clauses,
   * followed by that number of Clause declaration lines. A Clause declaration
   * line consists of the IDs of the Propositions that are the Clause's
   * disjuncts, preceded, if the section name was prefixed with "dbg_", by the 
   * print name of the component.
   * \param has_debug_info whether print names are associated with the entities
   * \return whether the "propositions" section was successfully parsed
   */
  bool parse_transitions(bool);

  /**
   * Parse the section that specifies the Variables.
   * Preconditions: Enums must have been parsed; Variables must not have been
   * parsed.
   * The section contains one line containing a count of the number of
   * Variables, followed by that number of Variable declaration lines. A
   * Variable declaration line consists of the index of the Variable's enum
   * followed, if the section name was prefixed with "dbg_", by the print name
   * of the Variable.
   * \param has_debug_info whether print names are associated with the entities
   * \return whether the "variables" section was successfully parsed
   */
  bool parse_variables	(bool);

  /**
   * Parse the section that assigns inital values to Variables.
   * Preconditions: enums and Variables must have been parsed; initial values
   * must not have been parsed.
   * The section consists of one line containing a count of the number of
   * assignments, followed by that number of (Variable ID, value index) pair
   * lines.
   * \param has_debug_info unused
   * \return whether the "initial" section was successfully parsed
   */
  bool parse_initial  	(bool);

  /// Return whether the "background" section was parsed.
  bool was_background_parsed() 	{ return didBackground_;   }
  /// Return whether the "clauses" section was parsed.
  bool was_clauses_parsed() 	{ return didClauses_;      }
  /// Return whether the "commands" section was parsed.
  bool was_commands_parsed()    { return didCommands_;     }
  /// Return whether the "enums" section was parsed.
  bool was_enums_parsed() 	{ return didEnums_;        }
  /// Return whether the "modes" section was parsed.
  bool was_modes_parsed()       { return didModes_;        }
  /// Return whether the "observed" section was parsed.
  bool was_observed_parsed()    { return didObserved_;     }
  /// Return whether the "propositions" section was parsed.
  bool was_propositions_parsed(){ return didPropositions_; }
  /// Return whether the "transitions" section was parsed.
  bool was_transitions_parsed() { return didTransitions_;  }
  /// Return whether the "variables" section was parsed.
  bool was_variables_parsed() 	{ return didVariables_;    }
  /// Return whether the "initial" section was parsed.
  bool was_initial_parsed() 	{ return didInitial_;      }

  /**
   * Read a line from the input file into data member "line".
   * The terminal newline is stripped off. Data member "line_number" is
   * advanced by one.
   * \return whether there are more lines
   */
  bool readline();

  /**
   * Interpret the next "word" in the current line as a nonnegative decimal
   * integer.
   * \param msg the name of what is being read, used in error messages
   * \param acceptStar if true and the next character is '*', return
   * L2rTransition::ANY_MODE; if false and the next character is '*', return
   * from_l2::NOT_AN_INTEGER
   * \return L2rTransition::ANY_MODE if acceptStar is true and the next
   * character is '*'; from_l2::NOT_AN_INTEGER if the line could not be parsed
   * as an integer; otherwise the integer value
   */
  int read_integer(const char *msg, bool acceptStar = false);
  
  /**
   * Skip over all characters in a word boundary (whitespace).
   * \param print_error whether to print an error message if the string
   * terminator is encountered.
   * \return whether the string terminator is encountered.
   */
  bool skip_word_boundary(bool print_error = true);

#ifdef ENABLE_L2_VERBOSE
  /**
   * Print a parsing error message to _STD_ cerr.
   * This is conditionally compiled either to print the line and character in
   * line or to do nothing.
   */
  void print_parse_error();
#endif

  /**
   * If a section has been parsed when it shouldn't have, or the reverse, print
   * an error message.
   * \param condition has the section named in msg already been parsed?
   * \param expected should the section named in msg have already been parsed?
   * \param msg the name of the section being checked
   * \return whether condition == expected
   */
  bool check_parsed(bool condition, bool expected, const char*);

  /**
   * Parse a Proposition declaration line.
   * This is an auxiliary to from_l2::parse_propositions().
   * \param prop_index unique serial enumerator (ID) of the Proposition
   * \param equality whether the Proposition is "same" or "different"
   * \param positive whether the Proposition is "positive" or "same"
   * \return whether the Proposition declaration line was successfully parsed
   */
  bool parse_one_prop(unsigned, bool, bool);

  /**
   * Parse a Transition declaration line
   * This is an auxiliary to from_l2::parse_transitions()
   * \param has_debg_info whether print names are associated with the entities
   * \param isNominal whether the Transition is nominal
   * \return whether the Transition declaration line was successfully parsed
   */
  bool parse_one_transition(bool, bool);

  /**
   * Parse one declaration of a Variable as being a Command, Observable or
   * mode.
   * This is an auxiliary to from_l2::parse_observed(),
   * from_l2::parse_commands() and from_l2::parse_modes().
   * \param kind whether the declaration is for Commands, Observables or modes
   * \param whether the declaration was successfuly parsed
   */
  bool parse_var_modifier(VarKind);
};

#endif
