/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: transition.h,v 1.27 2004/02/11 00:54:51 lbrown Exp $ */
#ifndef _L2RTRANSITION_H
#define _L2RTRANSITION_H

#include <readers/clause.h>

/**
 * A Transition in the L2_file, after reading from the model file and before
 * writing to the T_system.
 */

class L2rTransition : public Pooled {
 public:

  enum public_constants {
    ANY_MODE = -1
  };

  /**
   * Specify the mode L2rVariable, the "from" and "to" states as value indices,
   * the set of L2rClauses constraining the transition, whether it is nominal,
   * and its cost/rank/weight.
   */
  L2rTransition(const L2rVariable *mode,
                int fromStateIndex,
		int toStateIndex,
                size_t clauseCount,
		const L2rClause * const *clauses,
		bool copyClauses,
                bool isNominal,
		unsigned rank);
  
  /**
   * Deallocate the L2rClause set.
   * The only subclass is dbg_L2rTransition.
   */
  l2_virtual ~L2rTransition();
  
  /// Return the mode L2rVariable.
  const L2rVariable *mode() const { return mode_; }
  
  /// Return the number of L2rClauses constraining the L2rTransition.
  size_t nclauses() const { return nclauses_; }

  /// Return the ith L2rClause.
  const L2rClause *clause(unsigned i) const {
    // Until I fix this
    // return bounds_check_access(i, nclauses(), clauses_, "clause");
    assert(i < nclauses_);
    return clauses_[i];
  }

  /// Is the L2rTransition is nominal?
  bool isNominal() const { return isNominal_; }

  /// Is the L2rTransition is failure (that is, not nominal)?
  bool isFailure() const { return !isNominal_; }

  /// Return the rank (negative log prior probability) of the L2rTransition.
  unsigned rank() const { return rank_; }
  
  /// Return the index of the "from" state.
  int from() const { return from_; }

  /// Return the index of the "to" state.
  int to() const { return to_; }

#ifdef ENABLE_L2_DEBUG_SECTIONS
  /// Is this the debugging version? No, it isn't.
  l2_virtual bool hasDebug() const { return false; }
#endif

#ifdef ENABLE_L2_VERBOSE
  /// Print the "from" state.
  void printFrom(_STD_ ostream& os) const {
    if (from_ == ANY_MODE) {
      os << '*';
    } else {
      mode()->type()->printMember(os, from());
    }
  }
#endif

  /// Print the "to" state (a failure state).
  void printTo(_STD_ ostream& os) const {
    mode()->type()->printMember(os, to());
  }

#ifdef ENABLE_L2_VERBOSE
  /// Print the L2rTransition.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os << "Transition: ";
    toOStream_internal(os);
  }

  /// Print the "to" state unless nominal.
  void toOStream(_STD_ ostream& os) const {
    if (isNominal()) {
      os << "nominal";
    } else {
      printTo(os);
    }
  }

 protected:

  /// Auxiliary to toOStream_long()
  void toOStream_internal(_STD_ ostream& os) const {
    os  << (isNominal() ? "nominal" : "failure")
	<< " moves " << mode() << " from ";
    printFrom(os);
    os << " to ";
    printTo(os);
    if (isFailure()) os << " (probability rank " << rank() << ')';
    os << _STD_ endl;

    if (nclauses() == 0) {
      os << "    may occur at any time\n";
    } else if (nclauses() == 1) {
      os << "    occurs unless " << clauses_[0] << _STD_ endl;
    } else {
      os << "    occurs unless:\n";
      assert(nclauses_ > 0);
      os << "    " << clauses_[0];
      for (size_t i = 1; i < nclauses(); i++) {
	assert(0 <= i && i < nclauses_);
	os << " and\n    " << clauses_[i];
      }
      os << _STD_ endl;
    }
  }
#endif

 private:
  /// The number of L2rClauses constraining the L2rTransition.
  size_t nclauses_;

  /// An array of the L2rTransition's L2rClauses.
  const L2rClause * const *clauses_;
  
  /// The L2rTransition's mode L2rVariable.
  const L2rVariable *mode_;

  // modes: members of the enum of mode_

  /// The "from" mode; ANY_MODE means "any mode"
  int from_;

  /// The "to" mode
  int to_;
  
  /// cost/rank/weight is negative log probability.
  unsigned rank_;

  /// Whether the transition is nominal (or failure).
  bool isNominal_;
};


/**
 * This subclass of L2rTransition is used for debugging. It carries the
 * additional "name" string.
 */

#ifdef ENABLE_L2_DEBUG_SECTIONS
class dbg_L2rTransition : public L2rTransition {
 public:
  /// In addition to everthing else, name the L2rTransition.
  dbg_L2rTransition(const MBA_string& name,
		    const L2rVariable *mode,
		    int fromStateIndex,
		    int toStateIndex,
		    size_t clauseCount,
		    const L2rClause * const *clauses,
		    bool copyClauses,
		    bool isNominal,
		    unsigned rank)
    : L2rTransition(mode, fromStateIndex, toStateIndex, clauseCount, clauses,
		    copyClauses, isNominal, rank),
    name_(name) { }

  /// Nothing special to do.
  l2_virtual ~dbg_L2rTransition() { }

  /// Return the name of the model component.
  MBA_string name() const { return name_; }

  /// Is this the debugging version? Yes, it is.
  l2_virtual bool hasDebug() const { return true; }

#ifdef ENABLE_L2_VERBOSE
  /// Print the L2rTransition.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os << "Transition " << name_ << ": ";
    toOStream_internal(os);
  }
#endif

 private:

  /// The name of the model component to which this L2rTransition refers.
  MBA_string name_;
};
#endif


#ifdef ENABLE_L2_VERBOSE
/// operator<< for an L2rTransition
inline _STD_ ostream&
operator << (_STD_ ostream& os, const L2rTransition& transition) {
  transition.toOStream(os);
  return os;
}


/// operator<< for a pointer to an L2rTransition
inline _STD_ ostream&
operator << (_STD_ ostream& os, const L2rTransition *pTransition) {
  if (!pTransition) {
    os << "(nil transition)";
  } else {
    pTransition->toOStream(os);
  }
  return os;
}
#endif

#endif
