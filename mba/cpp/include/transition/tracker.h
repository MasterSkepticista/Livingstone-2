/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict coverage algorithm, NASA Ames Research Center
// See Copyright.txt
// $Id: tracker.h,v 1.87 2004/06/04 22:23:02 lbrown Exp $

#ifndef TRACKER_H
#define TRACKER_H

#include <transition/T_system.h>
#include <conflict_db/candidate.h>
#include <transition/CandidateEqClass.h>
#include <transition/termination.h>

#ifndef DISABLE_TRACKER_LISTEN
class Tracker_listener;
#endif

/**
 * The abstract Tracker class currently controls growth of the T_system and
 * the search for a consistent set of Assignments to the Assumptions.
 *
 * Subclasses (of which there are currently two, CBFS_tracker and
 * Cover_tracker) must implement pure virtual functions extend_candidates(),
 * find_candidates(), find_fresh_candidates() and search_termination().
 *
 * <b>Using prune_and_search vs. using find_fresh_candidates or extend</b>
 *
 * The prune_and_search method is the preferred way for finding consistent
 * trajectories.  Each trajectory is represented by a set of Assignments to the
 * Assumption Variables, referred to as a Candidate.
 *
 * The prune_and_search() method checks each of the current Candidate
 * Assignments for consistency with the constraints and the Assignments to the
 * Command and Observable Variables.  If at least one of the Candidates remains
 * consistent, it does no further work.
 *
 * If no consistent Candidates remain, it calls find_fresh_candidates(). The
 * find_fresh_candidates() method finds the lowest-cost Candidates that are
 * consistent.  These are then stored for the next call to prune_and_search().
 *
 * In order to provide flexibility, the user may call find_fresh_candidates()
 * directly. This will discard any existing Candidates and find the lowest-cost
 * Candidates.
 *
 * For some searches, the user may also call extend().  This search procedure
 * finds only those consistent Candidates that are a superset (or extension) of
 * one of the current Candidates.
 *
 * When used for diagnosis, extend() conceptually finds only those Candidates
 * that are made consistent by adding failures to the existing Candidates.
 *
 * <b>Important notes:</b>
 * <ul>
 * <li><p><b>Not every subclass of Tracker supports extend().</b></p>
 * Specifically, Cover_tracker is defined to return all Candidates of a
 * given likelihood. It does not support extend(), as it is not guaranteed
 * to do this.
 *
 * <li><p><b>extend() does not return the lowest-cost Candidates</b></p>
 * <p>extend() may return trajectories that are more expensive than the
 * least-expensive consistent trajectory.  Suppose a low cost Candidate
 * Assignment C1 is consistent, as is a higher-cost candidate C2.
 * Suppose, given the settings in our search, we return only C1.</p>
 *
 * <p>We then add a new observation that is inconsistent with C1. Suppose
 * we must add Assignments A1 and A2 to C1 to make it consistent.
 * extend() will find C1+A1+A2 because it is an extension of C1.</p>
 *
 * <p>Now suppose C2 is consistent with the observation.  Thus both C2
 * and C1+A1+A2 are consistent Candidates, but only C1+A1+A2, being an
 * extension of C1, is returned.  If C1+A1+A2 is more expensive than C2,
 * we do not have the lowest cost Candidates.</p>
 * </ul>
 */

class Tracker : public Pooled {
 public:
  /// The iterator over Candidates
  typedef Candidate::ListOfCandidate::iterator iterator;
  /// The const itertator over Candidates
  typedef Candidate::ListOfCandidate::const_iterator const_iterator;

  /// How the Tracker does Find Candidates
  enum FC_style {
    invalid_fc = 0,
    tracker_default_fc = 1,
    extend_fc,
    find_fresh_fc,
    prune_and_search_fc
  };

  /// The Tracker search styles as symbolic constants
  enum Search_style {
    cbfs,
    cover,
    invalid_search
  };

  // The defaults for search_style_ and progress_style_ are conditionally
  // compiled.

  static const Search_style   DEFAULT_SEARCH_STYLE;

  /**
   * Create a new Tracker.
   * \param t_system reference to the T_system
   */

  Tracker(T_system& t_system);

  /// Deallocate all Candidates
  virtual ~Tracker(void) { eraseCandidatePartition(); }


#ifndef DISABLE_TRACKER_LISTEN
  /**
   * Add the listener to the set of listeners
   * \param pTracker_listener pointer to the listener to add
   */
  void add_listener(Tracker_listener *pTracker_listener) {
    listeners_.push_front(pTracker_listener);
  }

  /**
   * Remove the listener from the set of listeners
   * \param pTracker_listener pointer to the listener to remove
   */
  void remove_listener(Tracker_listener *pTracker_listener) {
    listeners_.remove_first(pTracker_listener);
  }
#endif

  /**
   * Is the Candidate set empty?
   * \return whether the Candidate set is empty
   * \todo Deprecated; this is a pass-through.
   */
  bool empty() const { return t_system.getCandidates().empty(); }

  /**
   * Return the number of Candidates.
   * \return the Candidate count
   * \todo Deprecated; this is a pass-through
   */
  size_t size() const { return t_system.getCandidates().size(); }

  /**
   * Return a reference to the T_system.
   * \return a reference to the T_system
   */
  T_system& get_t_system() { return t_system; }

  /**
   * Return a const reference to the T_system.
   * \return a const reference to the T_system
   */

  const T_system& get_t_system() const { return t_system; }

  /**
   * Returns the Conflicts discovered so far.
   * \return an Slist of pointers to the current Conflict set
   */
  Conflict::ListOfConflict* get_conflicts() {
    return t_system.get_conflicts();
  }

  /**
   * Progress with the Command specified as an Assignment.
   * \param pCommandAssignment pointer to the value Assignment for a Command
   * Variable; idle if null.
   * \todo What is the call to prune_and_search() doing here?
   */
  void progress(Assignment *pCommandAssignment = 0) {
    // Unless this is done LTMS::get_clause_support() will abort.
    prune_and_search();
    t_system.progress(pCommandAssignment);
  }

  /**
   * Progress with the Command specified as Variable and value indices.
   * \param commandIndex the ID of the Command Variable
   * \param valueIndex the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void progress(unsigned int commandIndex, unsigned int valueIndex) {
    Assignment* const pAssignment =
      t_system.get_command(commandIndex)->get_assignment(valueIndex);
    t_system.progress(pAssignment);
  }

  /**
   * Progress with the Command specified as Command Variable and value index.
   * \param command the Command Variable
   * \param valueIndex the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void progress(Command& command, unsigned int valueIndex) {
    t_system.progress(command.get_assignment(valueIndex));
  }

#ifndef DISABLE_FULL_PROGRESS
  /**
   * Moves the transition system forward by one step by introducing the
   * Variables needed to represent the Command.
   * \param pCommandAssignment pointer to Assignment to the Command Variable;
   * if null, idle
   * \todo Deprecated; this is a pass-through.
   */
  void full_progress(const Assignment *pCommandAssignment = 0) {
    t_system.full_progress(pCommandAssignment); // idle if 0
  }

  /**
   * Moves the transition system forward by one step by introducing the
   * Variables needed to represent the Command.
   * \param commandIndex the ID of the Command Variable
   * \param the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void full_progress(unsigned commandIndex, unsigned valueIndex) {
    const Assignment* const pAssignment =
      t_system.get_command(commandIndex)->get_assignment(valueIndex);
    t_system.full_progress(pAssignment);
  }

  /**
   * Moves the transition system forward by one step by introducing the
   * Variables needed to represent the Command.
   * \param command the Command Variable
   * \param valueIndex the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void full_progress(Command& command, unsigned valueIndex) {
    t_system.full_progress(command.get_assignment(valueIndex));
  }

#endif

#ifndef DISABLE_MIN_PROGRESS
  /**
   * Analogous to full_progress(); but copies much less of the model.
   * This saves space and time, at the cost of allowing some otherwise
   * inconsistent diagnoses.
   * \param pCommandAssignment pointer to Assignment to the Command Variable;
   * if null, idle
   * \todo Deprecated; this is a pass-through.
   */
  void minimal_progress(Assignment *pCommandAssignment = 0) {
    t_system.minimal_progress(pCommandAssignment); // idle if cmd==0
  }

  /**
   * Analogous to full_progress(); but copies much less of the model.
   * This saves space and time, at the cost of allowing some otherwise
   * inconsistent diagnoses.
   * \param commandIndex the ID of the Command Variable
   * \param valueIndex the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void minimal_progress(unsigned commandIndex, unsigned valueIndex) {
    Assignment* const pAssignment =
      t_system.get_command(commandIndex)->get_assignment(valueIndex);
    t_system.minimal_progress(pAssignment);
  }

  /**
   * Analogous to full_progress(); but copies much less of the model.
   * This saves space and time, at the cost of allowing some otherwise
   * inconsistent diagnoses.
   * \param command a Command Variable
   * \param valueIndex the index of the value
   * \todo Deprecated; this is a pass-through.
   */
  void minimal_progress(Command& command, unsigned valueIndex) {
    t_system.minimal_progress(command.get_assignment(valueIndex));
  }
#endif

  /**
   * Assign the indexed Observation Variable to the indexed value.
   * \param observableIndex the ID of the Observable Variable
   * \param valueIndex the index of the value; if negative, unassign
   * \todo Deprecated; this is a pass-through.
   */
  void observe(unsigned observableIndex, signed valueIndex) {
    t_system.observe(observableIndex, valueIndex); }

  /**
   * Assign the supplied Observation Variable to the indexed value.
   * \param observable the Observable Variable
   * \param valueIndex the index of the value; if negative, unassign
   * \todo Deprecated; this is a pass-through.
   */
  void observe(Observable& observable, signed valueIndex) {
    observable.observe(valueIndex);
  }

  /**
   * Perform a find-candidates operation with the prune-and-search style.
   * Checks each of the current Candidates for consistency. If at least one
   * remains consistent, the method returns. If none are consistent, a new set
   * of Candidates is found with find_fresh_candidates().
   */
  void prune_and_search() {
    // If the set of Candidates shrinks, there will be dangling pointers
    eraseCandidatePartition();
    t_system.prune_inconsistent_candidates();
    partition();
    if (t_system.getCandidates().empty()) { find_fresh_candidates(); }
  }

  /**
   * Return why we ended the search when we did.
   * \return the reason the last search terminated (if it did terminate)
   */
  virtual SearchTermination search_termination() const = 0;


  /**
   * Returns the current time step of the transition system.
   * \return the current time step
   * \todo Deprecated; this is a pass-through.
   */
  unsigned int get_time_step() const { return  t_system.get_time_step(); }

  /// ***************************************************************
  /// The rest of these functions are used in the implementation of
  /// progress and prune_and_search.  They are provided here in case
  /// the user wants greater control
  /// ***************************************************************

  /**
   * Perform a find-candidates operation with the system-determined search
   * style.
   * This is deprecated in favor of calling prune_and_search() or
   * extend_candidates().
   */
  virtual void find_candidates() = 0;

  /**
   * Perform a find-candidates operation with the extend-candidate search
   * style.
   * Perform a search that extends the current Candidates so that they are
   * consistent with the current observations. This is not implemented for
   * the Cover_tracker subclass.
   */
  virtual void extend_candidates() = 0;

  /**
   * Perform a find-candidates operation with the find-fresh search style.
   * Perform a new search for Candidates that searches for Candidates that are
   * not required to be extensions of the existing Candidates. This will be
   * called by prune_and_search().
   */
  virtual void find_fresh_candidates() = 0;

  //----------------------------------------------------------------------
  // Iterator methods
  //----------------------------------------------------------------------

  /**
   * The begin iterator for the Candidate set
   * \return the begin iterator for the Candidate set
   * \todo Deprecated; this is a pass-through.
   */
  iterator begin() { return t_system.getCandidates().begin(); }

  /**
   * The end iterator for the Candidate set
   * \return the end iterator for the Candidate set
   * \todo Deprecated; this is a pass-through.
   */
  iterator end()   { return t_system.getCandidates().end();   }

  /**
   * Return the ith Candidate, if it exists; othwise return NULL.
   * \param i index of the Candidate to return
   * \todo Deprecated; this is a pass-through.
   */
  Candidate *getCandidate(unsigned i) { return t_system.getCandidate(i); }

  /**
   * Return the ith Candidate as a const, if it exists; othersie return NULL.
   * \param i index of the Candidate to return
   * \return the ith Candidate if it exists; otherwise NULL
   * \todo Deprecated; this is a pass-through.
   */
  const Candidate* get_candidate(unsigned i) const {
    return t_system.getCandidate(i);
  }

  /**
   * Deallocate all Candidates and make the Candidate set empty.
   * \todo Deprecated; this is a pass-through.
   */
  void erase() { t_system.eraseCandidates(); }

  /**
   * Add the Candidate in decreasing order of weight
   * \param pCandidate a pointer to the Candidate to add
   * \todo Deprecated; this is a pass-through.
   */
  void add_candidate(Candidate* pCandidate) {
    t_system.addCandidate(pCandidate);
  }

  /**
   * Add Assumption to all current Candidates. Added to allow fault injection
   * when using Livingstone for simulation.
   * \param pAssignment the Assumption Assignment to add
   * \todo Deprecated; this is a pass-through.
   */
  void add_assumption(const Assignment *pAssignment) {
    t_system.addAssumption(pAssignment);
  }

  FC_style get_fc_style() const { return fc_style; }
  void set_fc_style(FC_style fc_style) { this->fc_style = fc_style; }

  /// Return the set of Candidate equivalence classes
  Array<CandidateEqClass*, false>& getCandidatePartition() {
    return candidatePartition;
  }

  /// Return the set of Candidate equivalence classes as a const
  const Array<CandidateEqClass*, false>& getCandidatePartition() const {
    return candidatePartition;
  }

  /**
   * Determine to which partition, if any, the Candidate belongs
   * \param candidate the Candidate whose equivalence class is sought
   * \return the index of an equivalence class if less than the partition size;
   * if equal to the partition size, a new equivalence class is required.
   */
  unsigned equivalenceClassIndex(const Candidate& candidate) const;

  /// Partition the Candidates into equivalence classes.
  void partition();

  /// Get rid of all the CandidateEqClass objects in the candidatePartition
  void eraseCandidatePartition();

private:

  /// The transition system that implements the rest of the Livingstone engine.
  T_system& t_system;

#ifndef DISABLE_TRACKER_LISTEN
  /// The set of listeners
  Slist<Tracker_listener*> listeners_;
#endif

  /// How the tracker does Find Candidates.
  FC_style fc_style;
  /// The set of Candidate equivalence Classes.
  Array<CandidateEqClass*, false> candidatePartition;
};

#endif  /* TRACKER_H */
