/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict coverage algorithm, NASA Ames Research Center
// $Id: cover_tracker.h,v 1.29 2002/11/04 19:37:50 lbrown Exp $

#ifndef COVER_TRACKER_H
#define COVER_TRACKER_H

#include <transition/tracker.h>

/**
 * A Tracker that uses the conflict coverage algorithm to track a transition
 * system.
 */

class Cover_tracker : public Tracker {
  friend class Cover_tracker_debug;

public:

  enum public_constants {
    DEFAULT_COVER_MAX_RANK = 8
  };

  /**
   * The only parameter specific to the cover Tracker is the Gamma limit.
   * \todo Migrate creation of empty Candidate to Tracker
   */
  Cover_tracker(T_system& t_system,
		Assignment::weight_t limit /* = 10.0 */,
		int /* dummy = 0 */) :
    Tracker(t_system), gamma(0), gamma_limit(limit) {
    // We should remove this and ensure the superclass inserts the empty
    // Candidate
    if (get_t_system().getCandidates().empty()) {
      get_t_system().getCandidates().push_front(new Candidate);
    }
  }

  /**
   * This is deprecated in favor of calling prune_and_search on the parent
   * class
   */
  void find_candidates();

  /**
   * Return why find_candidates ended. The only reason cover terminates is
   * that we exceeded the max rank.
   */
  virtual SearchTermination search_termination() const {
    return WEIGHT_CUTOFF;
  }

protected:
  /**
   * Generates a set of candidates at the next likelihood level, or at a
   * deeper level if none exist on the next level. Assumes that all candidates
   * returned at the last checked likelihood level have been made inconsistent
   * by observations.
   */
  void find_fresh_candidates();

  /**
   * Cover_tracker does not support extend candidates, so calling this method
   * will cause an exception.  Either it should be removed from the tracker
   * specification or we should figure out what it means in the context of a
   * coverage search.
   */
  void extend_candidates();

  /// Return the gamma weight limit
  Assignment::weight_t get_gamma() { return gamma;}

private:
  /// The current likelihood limit of the coverage search
  Assignment::weight_t gamma;
  /// A gamma limit beyond which we do not with the algorithm to go.
  Assignment::weight_t gamma_limit;
};

#endif  /* COVER_TRACKER_H */
