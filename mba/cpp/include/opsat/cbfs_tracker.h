/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: cbfs_tracker.h,v 1.42 2002/11/08 00:34:51 lbrown Exp $ */

#ifndef _CBFS_TRACKER_H_
#define _CBFS_TRACKER_H_

#include <transition/tracker.h>
#include <opsat/opsat.h>

class Candidate;
class CandidateEqClass;

/** This class tracks multiple candidates for a CBFS system. */

class CBFS_tracker : public Tracker {

  friend class CBFS_tracker_debug;

 public:
  /// The default value of the maximum cutoff weight
  enum public_constants {
    DEFAULT_CBFS_MAX_CUTOFF_WEIGHT = 100,
    DEFAULT_MAXIMUM_CANDIDATE_COUNT = 5,
    DEFAULT_MAXIMUM_CANDIDATE_CLASSES = 0,
    DEFAULT_MAXIMUM_SEARCH = 1000
  };

  /**
   * Constructor with all parameters optional.
   * Assumes start with the empty initial Candidate.
   */

  CBFS_tracker(T_system& t_system,
	       unsigned N = DEFAULT_MAXIMUM_CANDIDATE_COUNT,
	       unsigned max_class = DEFAULT_MAXIMUM_CANDIDATE_CLASSES,
	       unsigned max_cands = DEFAULT_MAXIMUM_SEARCH,
	       unsigned int max_cutoff_weight = DEFAULT_CBFS_MAX_CUTOFF_WEIGHT);

  /// Nothing special to do
  virtual ~CBFS_tracker(void) { }

  /**
   * Return a pointer to the next consistent Candidate, or NULL if search
   * terminates before finding one.
   */
  Candidate* next_consistent_candidate() {
    return opsat.next_consistent_candidate();
  }

  /// Set the weight threshold
  void set_cutoff_weight(Assignment::weight_t weight) {
    opsat.set_cutoff_weight(weight);
  }

  /// Return the weight threshold
  Assignment::weight_t get_cutoff_weight() const {
    return opsat.get_cutoff_weight();
  }

  /**
   * Set the maximim number of Candidate diagnoses which will be checked for
   * consistency in the search
   */
  void set_max_candidates(unsigned max) { opsat.set_max_candidates(max); }
  /**
   * Return the maximim number of Candidate diagnoses which will be checked for
   * consistency in the search
   */
  unsigned get_max_candidates() const { return opsat.get_max_candidates(); }

  /**
   * Return the maximum number of Candidate equivalence classes
   */
  unsigned get_max_candidate_classes() const { return maxClassCount; }

  /**
   * Return the maximum number of Candidates we track.
   * This corresponds to the command-line option "cbfs-candidates" and is
   * known elsewhere as 'max candidates' (not to be confused with member
   * function get_max_candidates()! ).
   */
  unsigned get_number_tracked() const { return number_tracked; }

  // could move this down to Satisfaction Search

  /// Return the number of Candidates searched
  unsigned get_candidates_considered() const {
    return opsat.get_candidates_considered();
  }

  /// Reinitializes search state to start with the empty Candidate.
  void find_fresh_candidates();

  /// Searches for Candidates using the existing Candidates as a starting point
  void extend_candidates();

  /// This calls extend_candidates() and is deprecated.
  void find_candidates()  { extend_candidates(); }

  /// Return the reason the preceding search terminated
  virtual SearchTermination search_termination() const {
    return opsat.terminate_search();
  }

private:
  /// Number of Candidates being tracked
  const unsigned number_tracked;
  /// Maximum number of Candidate equilvalence classes
  const unsigned maxClassCount;
  /// Delegate a lot of the work to this object
  Opsat_CBFS  opsat;
};


#endif

