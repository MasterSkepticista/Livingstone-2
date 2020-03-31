/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone_debug.h,v 1.18 2004/02/05 22:36:13 lbrown Exp $ */

#ifndef livingstone_debug_HEADER
#define livingstone_debug_HEADER

#include <api/livingstone.h>

class Tracker_debug;
class to_t_system;
class L2_string_map;
class L2_string_map_listener;

/**
 * This is the API for applications that interact with Livingstone via a
 * command-line interpreter.
 * The application instantiates this class, initializes the Livingstone
 * engine, and runs the command-line interpreter by calling
 * Livingstone_debug::run().
 */

class Livingstone_debug : public Livingstone {
 public:
  /// Initialize a command-line-enabled Livingstone API object.
  Livingstone_debug();

  /**
   * Deallocate the command-line buffer, the command-line enabled Tracker, the 
   * string map, and the listener that populates it.
   */
  virtual ~Livingstone_debug();

  /**
   * Create the command-line enabled Tracker of the previously-specified
   * subclass.
   */
  void create_debugger();

  /**
   * Return the command-line enabled Tracker
   * This should not be necessary for applications.
   */
  Tracker_debug* get_debugger() { return debugger_; }

  /**
   * Return the map between Livingstone entities and their print names.
   * This should not be necessary for applications.
   */
  L2_string_map* get_string_mapping() { return map_; }

  /**
   * Run the command-line interpreter interactively from standard input.
   * Prompt the user for input. Returns true if user asked to restart.
   */
  bool run() { return run(_STD_ cin, true); }

  /**
   * Run the command-line interpreter.
   * Run the scenario in the 'input' stream until we get to the end of it.
   * If the stream is interactive, the 'restart' command will make run() quit
   * and return true.  Otherwise, run() returns false.
   */

  bool run(_STD_ istream& input, bool is_interactive = false);

  /**
   * Map the SearchTermination enum onto print names.
   * \param searchTermination an enum specifying why or whether the last
   * search terminated
   * \return a human-readable representation of the input parameter
   */
  const MBA_string search_termination_string(SearchTermination
					     searchTermination) const {
    switch (searchTermination) {
    case SEARCH_CONTINUE: return "in progress";
    case EMPTY_AGENDA:    return "empty agenda";
    case WEIGHT_CUTOFF:   return "weight cutoff";
    case MAX_OVERRUN:     return "max overrun";
    default:              return "unknown SearchTermination";
    }              
  }

#ifdef ENABLE_L2_DEBUG_SECTIONS
#ifndef DISABLE_OPTIMIZER
  /**
   * Optimize the file, if needed or requested (inherited from
   * Livingstone_reader)
   * \return whether the model was optimized
   */
  virtual bool optimize() {
    return getReader().optimize(variableHashMap, optimizedAwayVariableNames);
  }
#endif

  /**
   * Get the L2rVariable name that the from L2rVariable name maps to, or ""
   * \param from the name of the Variable
   * \return the name of the Variable
   */
  const MBA_string getVariableMapping(const MBA_string& from);

  /**
   * Return the Variable in the Theory that represents the from L2rVariable, in
   * case the from L2rVariable was optimized away.
   * \param from the name of the Variable
   */
  const MBA_string getRepresentativeVariable(const MBA_string& from);

  /**
   * Add the name of the L2rVariable to allVariableNames; this is called by
   * collectAllVariableNames() for all L2rVariables
   */
  void addVariableName(MBA_string& variableName) {
    allVariableNames.grow_and_set(allVariableNames.size(), variableName);
  }

  /**
   * Set allVariableNames to the set of names of all model Variables, whether
   * or not optimized away.
   */
  void collectAllVariableNames();

  /// Begin iterator for the names of Variables optimized away.
  Array<MBA_string, true>::iterator beginOptimizedAwayVariableNames() {
    return optimizedAwayVariableNames.begin();
  }

  /// End iterator for the names of Variables optimized away.
  Array<MBA_string, true>::iterator endOptimizedAwayVariableNames() {
    return optimizedAwayVariableNames.end();
  }

  /// The begin iterator for allVariableNames
  Array<MBA_string, true>::iterator beginAllVariableNames() {
    return allVariableNames.begin();
  }

  /// The end iterator for allVariableNames
  Array<MBA_string, true>::iterator endAllVariableNames() {
    return allVariableNames.end();
  }

  /**
   * Get the named Variable or a member of its equivalence class.
   * \param the print name of a Variable
   * \return a pointer to either the named Variable or a member of its
   * equivalence class
   */
  const Variable* getRepresentative(const MBA_string& variableName);

#endif

 protected:
  /// Create a string map, given a writer.
  virtual void init_tracker_created_writer(to_t_system* writer);

 private:
  /// Associates model elements and their name strings.
  L2_string_map *map_;

  /// Callback that populates the string map.
  L2_string_map_listener *listener_;

  /**
   * The command-line-enabled Tracker, created by
   * Livingstone_debug::create_debugger().
   */
  Tracker_debug *debugger_;

  /**
   * The length of the buffer used to parse the command-line input.
   * Passed to the constructor.
   */
  unsigned line_alloc_;

  /**
   * Buffer used to parse the command-line input; malloc'd, so we can realloc.
   * Its length is Livingstone_debug::line_alloc_
   */
  char *line_;

#ifdef ENABLE_L2_DEBUG_SECTIONS
  /**
   * The set of all Variable names before optimization, built by
   * collectAllVariableNames()
   */
  Array<MBA_string, true> allVariableNames;

  /// Associates optimized-away Variables with their representatives
  Hash_table<MBA_string, MBA_string> variableHashMap;

  /// The set of names of all Variables optimized away
  Array<MBA_string, true> optimizedAwayVariableNames;
#endif

};

#endif
