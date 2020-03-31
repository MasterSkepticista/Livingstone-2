/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: conflict_db_debug.h,v 1.28 2004/01/28 23:31:34 lbrown Exp $
// Non-flight debugging/inspection code for Conflict_db

#ifndef CONFLICT_DB_DEBUG_H
#define CONFLICT_DB_DEBUG_H

#include <conflict_db/conflict_db.h>
#include <debuggers/ltms_debug.h>

class L2_string_map;
class Assignment;
class Candidate;
class Domain;

/**
 * Enable command-line interaction with the Conflict_db
 * \todo Everything except parse_cmd() should be made private.
 */

class Conflict_db_debug : public LTMS_debug {
 public:

  enum public_constants { NOT_FOUND = -1 };
  /**
   * Interact with the given conflict_db, sending output to the given ostream.
   * The string map allows referring to a Variable by its name.
   * \param conflict_db the Conflict_db being reported on
   * \param symbolTable the mapping between object and print name
   * \param os the default stream to which to print
   */

  Conflict_db_debug(Conflict_db& conflict_db,
		    L2_string_map& symbolTable,
		    _STD_ ostream& os);

  /// Nothing special to do.
  virtual ~Conflict_db_debug() { }

  /**
   * Interpret the command line, delegating to LTMS_debug if unable
   * \param cmd the command name
   * \param args the command's arguments
   * \return whether the command was handled
   */
  virtual bool parse_cmd(const char* cmd, const char* args);

  /// Write a usage message to the ostream
  virtual void listener_usage() const;

  /// An abstract callback class with an operator() taking a Variable argument.
  class VariableCallbackObject {
  public:
    /// An operation on the Variable
    virtual void operator() (Variable&) = 0;
  };

  // --------------------------------------------------------------------------
  // Methods for querying/modifying the DB in ways that would not be used in
  // flight.
  // --------------------------------------------------------------------------

  /**
   * Assign a value to an Assignable.
   * The string is assumed to be "assign {<variable>=<value>}*"
   * \param string a string for the form &lt;variable&gt;=&lt;value&gt;
   * \todo This breaks if there is trailing white space
   */
  // This munges the assignment string.
  void assign(const char* string);

  /**
   * An auxiliary to assign(char *)
   * \param assignable the Assignable to which to assign a value
   * \param value_name the print name of the value to assign
   */
  void assign(Assignable& assignable, const MBA_string& value_name);

  /**
   * Set the value of the named Variable to be UNKNOWN, if it exists.
   * \param variableName the print name of the Assignable
   */
  void unassign(const MBA_string& variableName);

  /**
   * Returns the string representation of the value of the Variable that is
   * currently inferred to be true in the db, or "unknown" if no value is known
   * to hold.
   * \param variable the Variable the value of which is sought
   * \return the print name of the Variable
   */
  MBA_string get_variable_value(const Variable& variable);

  /**
   * Auxiliary to get_variable_value(const Variable& variable)
   * \param variable the Variable the value of which is sought
   * \param valueIndex the index of the value
   * \return the print name of the Variable
   */
  MBA_string get_variable_value(const Variable& variable, int valueIndex);

  // -------------------------------------------------
  //   Methods for printing various items
  // -------------------------------------------------

  /**
   * For each Variable, print its name and the name of its value to the
   * default ostream.
   * \todo Implement this in terms of print_all_variable_values(_STD_ ostream&)
   */
  void print_all_variable_values();

  /**
   * For each Variable, print its name and the name of its value to the
   * supplied ostream.
   * \param os where to print the Variable values
   */
  void print_all_variable_values(_STD_ ostream& os);

  /**
   * Print to the default ostream the members of every Domain whose name
   * contains the substring.
   * \param substring The substring of the domain name
   */
  void cdomains(const MBA_string& substring);

  /// Print to the default ostream the members of the supplied Domain
  void print_domain_members(const Domain* pDomain);

  /// Print to the default ostream the members of all Domains.
  void print_domains();

  /// Find and prints the "best" Conflict in the current DB context, if any
  void print_conflict();

  /// Print all conflicts discovered in any DB context
  void print_conflicts();

  /// Print all Assumptions and their Assignments in the current context
  void print_assumptions();

  /// Print all Assignables and the Assignments in the current context
  void print_assignables();

  /**
   * Print all constraints (Clauses) that contain the substring
   * \param substring a substring of the constraint Clauses to print
   */
  void print_constraints(const char *substring);

  /**
   * Print to the default ostream a Variable and its value.
   * \param variable the Variable being described
   */
  void print_variable_and_value(const Variable& variable);

  /**
   * Print to the supplied ostream a Variable and its value.
   * \param variable the Variable to print
   * \param stream where to print the Variable
   */
  void print_variable_and_value(const Variable& variable,
				_STD_ ostream& stream);

  /// Print some stats: the number of Variables
  void print_stats();

  /**
   * Return the print name of the Variable.
   * \param variable the Variable the print name of which is sought
   * \return the print name of the Variable
   */
  MBA_string get_var_name(const Variable& variable);

  ///////////// Various queries by name.

  /**
   * Get a Variable by name.
   * If assert_found is true and the Variable is not found, throw an
   * L2_error; otherwise return NULL.
   * \param variableName the name of the Variable sought
   * \param assert_found if true, assert that the return value is not null
   * \return a pointer to the named Variable, or NULL if not found
   */
  Variable *find_variable(const MBA_string& variableName,
			  bool assert_found = false);

  /**
   * Find the index of the Variable's Proposition with the given name.
   * If assert_found is true and the Proposition is not found, throw an
   * L2_error; otherwise return NOT_FOUND.
   * \param variable the Variable the Propositions of which are scanned
   * \param propositionName the print name of the Proposition sought
   * \assert_found if true, assert that the Proposition was found
   * \return the index of the Proposition, or NULL if not found
   */
  int find_prop_index(const Variable& variable,
		      const MBA_string& propositionName,
		      bool assert_found = false);

  /**
   * Get an Assignable Variable by name.
   * If assert_found is true and the Assignable is not found, throw an
   * L2_error; otherwise return NULL.
   * \param assignableName the name of the Assignable
   * \assert_found if true, assert that the Assignable was found
   * \return a pointer to the Assignable, or NULL if not found
   */
  Assignable* find_assignable(const MBA_string& assignableName,
			      bool assert_found = false);

  ////////// Find an assignment.

  /**
   * Split a user-input string into Variable name and value name substrings
   * \param assign_str A string of the form &lt;variable&gt;=&lt;value&gt;
   * \param variableName (out) the name of the variable
   * \param valueName (out) the print name of the value
   */
  void split_assignment(const MBA_string& assign_str,
			MBA_string& variableName,
			MBA_string& valueName);

  /**
   * Find an Assignment given a user-input string.
   * If assert_found is true and the Assignment is not found, throw an
   * exception.
   * \param assign_str A string of the form &lt;variable&gt;=&lt;value&gt;
   * \param assert_found if true, assert that the Assignment has been found
   * \return a pointer to the Assignment, or NULL if not found
   */
  Assignment *find_assignment(const MBA_string& assign_str,
			      bool assert_found = false);

  /**
   * Find an Assignment given well-formed Variable name and value name strings.
   * If assert_found is true and the Assignment is not found, throw an
   * exception.
   * \param variableName the print name of the Variable
   * \param valueName the print name of the value
   * \param assert_found if true, assert that the Assignment has been found
   * \return a pointer to the Assignment, or NULL if not found
   */
  Assignment *find_assignment(const MBA_string& variableName,
			      const MBA_string& valueName,
			      bool assert_found = false);

  /**
   * Find an Assignment given Assignable and well-formed value name string.
   * If assert_found is true and the Assignment is not found, throw an
   * Exception.
   * \param assignable the Assignable that may contain the Assignment
   * \param value the name of the Assignable Variable's value
   * \param assert_found if true, assert that the Assignment has been found
   * \return a pointer to the Assignment, or NULL if not found
   * \todo The Assignable& arg can't be const because the return value isn't.
   */
  Assignment *find_assignment(Assignable& assignable,
			      const MBA_string& valueName,
			      bool assert_found = false);

  // We don't provide finding an Assignment given Assignable and
  // index because you can get that directly from the Assignable.

  ////////// Print things

  /**
   * Print the Assignment to the default ostream.
   * \param assignment the Assignment to print
   * \param indent the number of spaces to indent
   * \return 1
   */
  virtual unsigned print_assignment(const Assignment& assignment,
				    unsigned indent = 0) {
    return print_assignment(stream, assignment, indent);
  }

  /**
   * Print the Assignment to the specified ostream.
   * \param os the stream to which the Assignment is printed
   * \param assignment the Assignment to print
   * \param indent the number of spaces to indent
   * \return 1
   */
  virtual unsigned print_assignment(_STD_ ostream& os,
				    const Assignment& assignment,
				    unsigned indent = 0);

  /**
   * Print the Candidate to the default ostream.
   * \param candidate the Candidate to print
   * \param indent the number of spaces to indent
   */
  void print_candidate(const Candidate& candidate,
		       unsigned indent = 0) {
    print_candidate(stream, candidate, indent);
  }

  /**
   * Print the Candidate to the specified ostream with the supplied
   * indentation.
   * \param os the stream to which the Candidate is printed
   * \param candidate the Candidate to print
   * \param indent the number of spaces to indent
   */
  void print_candidate(_STD_ ostream& os,
		       const Candidate& candidate,
		       unsigned indent = 0);

  /**
   * Print the Conflict to the default ostream.
   * \param conflict the conflict to print
   */
  void print_conflict(const Conflict& conflict) {
    print_conflict(stream, conflict);
  }

  /**
   * Print the Conflict to the specified ostream.
   * \param os the output stream to which the conflict is to be printed
   * \param conflict the Conflict to print
   */
  void print_conflict(_STD_ ostream& os, const Conflict& conflict);

 protected:
  /// The Conflict_db with which this class interacts
  Conflict_db& db;

 private:
  /// The default ostream to which output is written.
  _STD_ ostream &stream;
};


#endif // CONFLICT_DB_DEBUG_H
