/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: ltms_debug.h,v 1.24 2004/01/28 23:31:35 lbrown Exp $
// Non-flight debugging/inspection code for LTMS

#ifndef LTMS_DEBUG_H
#define LTMS_DEBUG_H

#include <time.h>
#include <livingstone/L2_iostream.h>
#include <mba_utils/hash_table.h>
#include <tms/ltms.h>

class L2_string_map;

/**
 * Enable command-line interaction with the LTMS.
 */

class LTMS_debug : public Pooled {
 public:

  //----------------------------------------------------------------------
  // Constructors and destructors
  //----------------------------------------------------------------------

  /// Save the parameters.
  LTMS_debug(LTMS& ltms, L2_string_map& stringMap, _STD_ ostream& stream);

  /// Nothing special to do.
  virtual ~LTMS_debug() { }

  /// Trap LTMS-level command lines, returning false if it can't be handled.
  virtual bool parse_cmd(const char* cmd, const char* args);

  /// Prints a usage message to the object's ostream.
  virtual void listener_usage() const;

  /**
   * Obsolete, but still used by dlltest-mof.cpp and testMOFVxWorks.cpp.
   * \todo Remove.
   */
  void listener();

  /// Used by several subclasses; creates a string with i spaces.
  static MBA_string get_indent_string(unsigned);

  /// Does the Proposition's name string contain the supplied substring?
  bool prop_contains_string(Proposition* prop, const char* substring) const;

  /// Returns the print name that was recorded for a Proposition.
  MBA_string get_prop_name(Proposition& proposition) const;

  /**
   * Prints out every Proposition whose name contains the substring, i.e.,
   * "see props", and whose truth value is TRUE.
   * If "verbose" is true, all Propositions containing the string are printed.
   * Note that the empty string and NULL are both defined to be contained in
   * every proposition name, so passing either will cause all Propositions to
   * be printed.
   */
  void cprops(const char* substring, bool verbose);

  /**
   * Prints out every non-deleted Clause that contains the substring.
   * If "show_deleted" is true, both deleted and non-deleted Clauses that
   * contain the string are shown.
   */
  void cclauses(const char* substring, bool show_deleted = false);

  /// Print out every Proposition in the LTMS.
  void print_propositions();

  /// Return the maximum explanation depth.
  static size_t get_max_explanation_depth() { return max_explanation_depth; }

  /// Set the maximum explanation depth.
  static void set_max_explanation_depth(size_t depth)
    { max_explanation_depth = depth; }

  /// Write out an indented "explanation" (backtrace) of the named Proposition.
  void explain_proposition (MBA_string& propositionName, size_t indent);

  /**
   * Prints out a Clause that is internally stored in disjunctive normal form.
   * The output is rendered as an implication to make it more understandable.
   */
  void pretty_print_clause (const Clause& clause);

  /**
   * Prints out a Clause that is internally stored in disjunctive normal form.
   */
  void print_clause(const Clause& clause);

  /**
   * This inner class is the base class for a filter callback for iterating
   * over Clauses. Users must subclass this and define operator().
   */

  class ClauseCallbackObject {
  public:
    virtual void operator() (Clause&) = 0;
  };

  /**
   * Iterates over all clauses, calling the callback function object on
   * each variable.
   */
  void clause_iterator(LTMS_debug::ClauseCallbackObject& clauseCallbackObject);

  /// Returns the Proposition with the passed name, if it exists.
  Proposition* find_proposition(MBA_string& propositionName) const;

  /// Controls whether timing information is printed out. Initially false.
  void toggle_timing() { timing_enabled = !timing_enabled; }

 protected:
  /**
   * Resets the clock used for timing. This should be called before a
   * debugger starts the function to be timed.
   */
  void begin_timing();

  /**
   * Prints out the time elapsed since the last begin_timing(), if timing
   * is enabled.
   */
  void end_timing();
  
  /// The mapping between print names and LTMS objects.
  // protected so subclasses can also use it
  L2_string_map &map;

 private:
  void print_trace_indent(size_t indent);
  
  void print_proposition(Proposition* prop, bool print_truth_value=true);

  /*
   * This function explains a CLAUSE. It traces back through the clause's
   * literals and prints them out with the appropriate indentation.  It
   * makes sure that it does not trace back through the proposition that
   * it propagates a unit clause to (propn in the code below).
   *
   *  It traces back only when INDENT is less than MAX_EXPLANATION_DEPTH
   */
  void explain_clause (Clause& clause, size_t indent);
  
  /// This function prints out an explanation for proposition PROPN. 
  /// INDENT specifies the current level of indentation. The function
  /// assumes that the proposition's truth value has been set
  /// appropriately, and any dependencies are still active.
  void explain_proposition (Proposition& propn, size_t indent );

  void show_proposition_support(MBA_string& prop_name);
  void show_proposition_support(Proposition& propn);

  /// Returns the clause that represents the assertion of the literal
  /// into the theory.  This currently doesn't support negative literals.
  Clause* find_literal_clause(MBA_string& prop_name);
  void print_stats();
  void pretty_print_clause_no_negative(const Clause& clause);
  void pretty_print_clause_no_positive(const Clause& clause);
  void pretty_print_clause_positive_and_negative(const Clause& clause);
  void doAdd(const char* args);
  void doRemove(const char* args);
  void doPartialPropagate(const char* args);
  void doPropagate(const char* args);
  void doUnsatisfied(const char* args);
  void doStatus(const char* args);

  /**
   * This inner class is a callback object. Its only method, operator(),
   * iterates over each Proposition in the Clause parameter and, if its print
   * name contains the object's string, send the print name of the Clause to
   * the object's ostream.
   */

  class ClauseStringChecker : public ClauseCallbackObject {
  public:

    /**
     * Create a callback object to search for the supplied string, sending
     * output the the supplied ostream. The pointer to the LTMS_debug is
     * needed to generate the print name of a Proposition. If show_deleted is
     * true, the Clause is processed even when deleted.
     */
    ClauseStringChecker(LTMS_debug* db,
			_STD_ ostream &stream,
			const char* string,
			bool show_deleted = false) :
      db(db), stream(stream), substring(string), show_deleted(show_deleted) { }

    /**
     * Write the first Clause that contains a Proposition whose name contains
     * the substring given in the object's constructor.
     */
    void operator() (Clause& rClause);

  private:
    /// Needed for finding the print name of a Proposition.
    LTMS_debug *db;

    /// Where to write the print name of the Clause.
    _STD_ ostream &stream;

    /**
     * Write the Clause if a Proposition's print name contains this
     * Proposition.
     */
    const char *substring;

    /// If true, process a Clause even if it is deleted.
    bool show_deleted;
  };

 private:
  /// Instead of inheriting from the LTMS, it refers to it
  LTMS &ltms;

  /// The stream to which verbose output is sent
  _STD_ ostream &stream;

  /// The cutoff for expanding nodes
  static size_t max_explanation_depth;

  // For timing function calls via begin_timing, end_timing

  /// Set at the beginning of a timed operation.
  clock_t start_time;

  /// Set at the end of a timed operation.
  clock_t end_time;

  /// Is timing information printed out?
  bool timing_enabled;
};

#endif // LTMS_DEBUG_H
