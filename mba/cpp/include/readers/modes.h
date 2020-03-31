/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: modes.h,v 1.6 2001/12/06 17:48:21 lbrown Exp $ */
#ifndef modes_HEADER
#define modes_HEADER

#include <readers/common.h>
#include <mba_utils/slist.h>

class L2rTransition;

/**
  * A mode Variable in the L2_file, after reading from the model file and
  * before writing to the T_system.
  * Represents all the Transitions on that mode.
  */

class L2rMode : public Pooled {
 public:
  /// Construct with nothing; build up the L2rTransitions incrementally.
  L2rMode() { }

  /**
   * Deallocate the nominal and failure L2rTransition sets.
   * No subclass ==> non-virtual dtor.
   */
  ~L2rMode();

  /**
   * Add a transition to this mode in increasing order of rank.
   * pL2rTransition must be allocated by L2rTransition::make() ; it is
   * deallocated when this L2rMode is deallocated.
   */
  void add_transition(const L2rTransition *pL2rTransition);

  // Iterator over an L2rTransition set.
  typedef Slist<const L2rTransition*>::iterator iterator;

  /// Begin iterator over the nominal L2rTransition set.
  iterator begin_nominal() const {
    return const_cast<Slist<const L2rTransition*>&>(nominal_).begin();
  }

  /// End iterator over the nominal L2rTransition set.
  iterator end_nominal() const {
    return const_cast<Slist<const L2rTransition*>&>(nominal_).end();
  }

  /// Begin iterator over the failure L2rTransition set.
  iterator begin_failure() const {
    return const_cast<Slist<const L2rTransition*>&>(failure_).begin();
  }

  /// End iterator over the failure L2rTransition set.
  iterator end_failure() const {
    return const_cast<Slist<const L2rTransition*>&>(failure_).end();
  }

  /**
   * Return the cardinality of the domain, the number of L2rTransitions.
   * All nominal transitions share a value, so they're counted as 1; thus,
   * this is one plus the number of failure L2rTransitions.
   */
  unsigned domain_size() const { return 1 + nfailure(); }

  /// Return the number of nominal L2rTransitions.
  unsigned nnominal() const { return nominal_.size(); }

  /// Return the number of failure L2rTransitions.
  unsigned nfailure() const { return failure_.size(); }

 private:
  /// The nominal L2rTransition set.
  Slist<const L2rTransition*> nominal_;

  /// The failure L2rTransition set.
  // Sorted by lowest rank first
  Slist<const L2rTransition*> failure_;

};

#endif
