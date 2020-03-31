/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// A nonflight class that contains debugging functions
// $Id: tracker_debug.h,v 1.41 2004/01/28 23:31:35 lbrown Exp $

#ifndef TRACKER_DEBUG_H
#define TRACKER_DEBUG_H

#include <debuggers/T_system_debug.h>
#include <transition/tracker.h>

#ifdef ENABLE_CHECKPOINT
#  include <checkpoint/checkpoint.h>
#endif

/**
 * When Livingstone_debug::create_debugger() is called, this is the base class
 * of the "debugger" that is created.
 * It contains a Tracker rather than deriving from it.
 */

class Tracker_debug : public T_system_debug {

public:
  /// Save the parameters.
  Tracker_debug(Tracker*, L2_string_map&, _STD_ ostream&);

  /// Set the Find Candidates style.
  // void set_fc_style(Tracker::FC_style style) { tracker->set_fc_style(style); }

  /// Return the current Find Candidates style.
  // Tracker::FC_style get_fc_style() const { return tracker->get_fc_style(); }

  /// Traps Tracker-level command lines. 
  virtual bool parse_cmd(const char* cmd, const char* args);

  /// Prints a usage message to the object's ostream
  virtual void listener_usage() const;

  /// Print statistics (right now, just the number of Candidates)
  void print_stats();

  /**
   * Deprecated for applications; only Livingstone::command() should use this.
   * Use Livingstone::command(unsigned, unsigned) to progress.
   */
  void progress(Assignment* cmd = 0);

  /**
   * Deprecated.
   * Use Livingstone::set_progress_style(L2_parameters::min) to specify
   * Progress Style "min-progress" and then
   * Livingstone::command(unsigned, unsigned) to progress.
   */
  void minimal_progress(Assignment* cmd = 0);

  /**
   * Deprecated.
   * Use Livingstone::set_progress_style(L2_parameters::full) to specify
   * Progress Style "full-progress" and then
   * Livingstone::command(unsigned, unsigned) to progress.
   */
  void full_progress(Assignment* cmd = 0);

  /**
   * Deprecated for applications; only
   * Livingstone:get_search_termination_reason() should call this.
   * Return the reason the last search terminated (if indeed it did
   * terminate).
   */
  virtual MBA_string search_termination() const = 0;
  
  /**
   * Deprecated for applications; only methods of class Livingstone should call
   * this.
   * Return a pointer to the Tracker (the Livingstone engine).
   */
  Tracker *get_tracker() { return tracker; }

  /**
   * Deprecated for applications; only methods on class Livingstone should call
   * this.
   * Return a read-only pointer to the Tracker (the Livingstone engine).
   */
  const Tracker *get_tracker() const { return tracker; }

  /// Print Candidates to the current ostream.
  void print_candidates();

  /// Print Candidates to the specified ostream.
  void print_candidates(_STD_ ostream &);

#ifdef ENABLE_CHECKPOINT
  /**
   * Create a checkpoint with the given name or number.
   * Return whether it was successful.
   */
  bool create_checkpoint (const MBA_string& name);

  /**
   * Restore the checkpoint specified by its name or number. 
   * Return whether it was successful.
   */
  bool restore_checkpoint(const MBA_string& name);

  /// Delete the checkpoint specified by its name or number.
  void delete_checkpoint (const MBA_string& name);

  /// Print the ID numbers and names of all checkpoints.
  void list_checkpoints();
  
  /**
   * This links together a checkpoint, its name, and its numeric ID.
   */
  struct ckpt_descriptor {
    L2_checkpoint *ckpt;
    unsigned index;
    MBA_string name;
    ckpt_descriptor(L2_checkpoint *c, unsigned i, const MBA_string& n)
      : ckpt(c), index(i), name(n) { }
    bool is_valid() const { return ckpt != 0; }
    static ckpt_descriptor invalid() { return ckpt_descriptor(0, 0, ""); }
  };

  /// Iterator over the checkpoint association list.
  typedef Hash_table<unsigned,ckpt_descriptor>::iterator ckpt_iterator;

  /// Begin iterator over the checkpoints.
  ckpt_iterator begin_checkpoints() const {
    return const_cast<Tracker_debug*> (this)->ckpt_by_id.begin();
  }

  /// End iterator over the checkpoints.
  ckpt_iterator end_checkpoints() const {
    return const_cast<Tracker_debug*> (this)->ckpt_by_id.end();
  }
#endif

  /// Map the Find Candidates style print name onto its dictionary index.
  static Tracker::FC_style find_style(MBA_string);

  /// Map the Find Candidates style dictionary index onto its print name.
  static MBA_string find_style(Tracker::FC_style);

protected:
  /**
   * Called before calling the applicable Tracker find-Candidates method to
   * begin timing and to save the number of Conflicts before
   */
  void start_find_candidates();

  /**
   * Called after  calling the applicable Tracker find-Candidates method to
   * print statistics.
   */
  void end_find_candidates();

  /// Map the Command=value string onto an Assignment object.
  Assignment* getCommandAssignment(const char* args);

  /// Handle the "progress-style" command line
  void doProgressStyle(const char* cmd, const char* args);

  /// Handle the "candidates" command line
  void doPrintCandidates(const char* cmd, const char* args);

  /// Handle the "classes" command line
  void doPrintClasses(const char* cmd, const char* args);

  /// Handle the "install" command line
  void doInstall(const char* cmd, const char* args);

  /// Handle the "truncate" command line
  void doTruncate(const char* cmd, const char* args);

  /// Handle the "fc-style" command line
  void doFindCandidatesStyle(const char *cmd, const char *args);

  /// Handle the "find-candidates" and "fc" command lines
  void doFindCandidates(const char *cmd, const char *args);

  /// Handle the "prune-search" and "ps" command lines
  void doPruneAndSearch(const char *cmd, const char *args);

  /// Handle the "find-fresh" and "ff" command lines
  void doFindFreshCandidates(const char *cmd, const char *args);

  /// Handle the "tracker-stats" command line
  void doPrintAllStats(const char *cmd, const char *args);

  /// Handle the "add-assumption" command line
  void doAddAssumption(const char *cmd, const char *args);

#ifdef ENABLE_CHECKPOINT
  /**
   * Look up in the association list the checkpoint with the given name or
   * number. If "print_errors" is true, print error messages about why the
   * checkpoint can not be found.
   */
  ckpt_descriptor find_ckpt(const MBA_string& name_or_index,
			    bool print_errors = true);

  /// Enter the checkpoint in the association list
  void register_ckpt(ckpt_descriptor);

  /// Delete the checkpoint from the association list of checkpoints
  void delete_ckpt(ckpt_descriptor);

  /// Print the checkpoint's index and name (if named)
  void briefly_print_ckpt(ckpt_descriptor);
#endif

  /** Return a reference to the ouput stream */
  _STD_ ostream& get_ostream() const { return stream; }

 private:
  /**
   * An element of a "dictionary" associating Find Candidates style name
   * strings and enum Tracker_debug::FC_style symbolic constants.
   * \todo Use String_key_dict_item in array_dict.h
   */
  struct FC_dict_item {
    const char* name;
    Tracker::FC_style style;
  };

  /**
   * A "dicionary" associating Find Candidates style name strings and enum
   *  Tracker_debug::FC_style symbolic constants.
   */
  static FC_dict_item fc_dict[];
  /// The tracker being reported
  Tracker* tracker;
  /// Where the output goes
  _STD_ ostream& stream;
  /**
   * Saved result of a T_system::num_conflicts() call.
   * \todo Pass this either as a return value or an out parameter.
   */
  unsigned nconflicts;

#ifdef ENABLE_CHECKPOINT
  /// Maps checkpoint name to checkpoint
  Hash_table<MBA_string, ckpt_descriptor> ckpt_by_name;
  /// Maps checkpoint number to checkpoint
  Hash_table<unsigned,   ckpt_descriptor> ckpt_by_id; // sparse array
  /// Checkpoint count
  unsigned nckpt;
  /// Serial enumerator generator
  unsigned last_ckpt_id;
#endif

  /** Cache a reference to the T_system to ease repeated access */
  T_system& t_system;
};

#endif // TRACKER_DEBUG_H
