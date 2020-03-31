/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: clause.h,v 1.26 2004/02/11 00:54:50 lbrown Exp $ */
#ifndef _L2RCLAUSE_H
#define _L2RCLAUSE_H 1

#include <readers/proposition.h>       // for class L2rProposition

/**
 * A Clause in the L2_file, after reading from the model file and before
 * writing to the T_system.
 */

class L2rClause : public Pooled {
 public:
  /// Associate the set of L2rPropositions to the L2rClause.
  L2rClause(unsigned clauseID,
	    size_t propositionCount,
	    const L2rProposition* const * propositions,
	    bool copyPropositions = true);

  /**
   * Deallocate the proposition set.
   * The only subclass is dbg_L2rClause ==> non-virtual if no debugging info.
   */
  l2_virtual ~L2rClause();

  /// Return the integer ID of the L2rClause.
  unsigned id() const { return clauseID_; }

  /// Return the L2rClause's ith Proposition.
  const L2rProposition *prop(unsigned i) const {
    // Until I fix this
    // return bounds_check_access(i, nprops(), props_, "proposition");
    assert(i < nprops_);
    return props_[i];
  }

  /// Return the number of L2rPropositions in the L2rClause.
  const size_t nprops() const { return nprops_; }

  /**
   * Is the L2rClause is in the background?
   * "Background" means that it pertains to within-timestep constraints.
   */
  bool isInBackground() const { return isInBackground_; }

  /// Put the L2rClause in the background.
  void putInBackground() { isInBackground_ = true; }

#ifdef ENABLE_L2_VERBOSE
  // Print the Clause's ID and L2rPropositions.
  void toOStream_long(_STD_ ostream& os) const {
    os << "Clause " << id() << ": ";
    toOStream(os);
    os << _STD_ endl;
  }

  /// Print the L2rClause's L2rPropositions.
  void toOStream(_STD_ ostream& os) const {
    if (nprops_ == 0) {
      // If the Clause has no Propositions, it is trivially satisfied
      os << "*true*";
    } else {
      // Print each proposition
      assert(nprops_ > 0);
      os << props_[0];
      for (unsigned i = 1; i < nprops(); i++) {
	assert(0 <= i && i < nprops_);
	os << " || " << props_[i];
      }
    }
  }
#endif

#ifdef ENABLE_L2_DEBUG_SECTIONS
  // Is this the debugging version? No, it isn't.
  l2_virtual bool hasDebug() const { return false; }
#endif

 private:
  /// The integer ID of the L2rClause.
  unsigned clauseID_;

  /// The number of L2rPropositions in the L2rlause.
  size_t nprops_;

  /// An array of the L2rClause's L2rPropositions.
  const L2rProposition * const *props_;

  /// Whether the L2rClause is in the background.
  bool isInBackground_;
};


#ifdef ENABLE_L2_DEBUG_SECTIONS

/**
 * This subclass of L2rClause is used for debugging.
 * It is decorated with the "from" component name string.
 * \todo Move from clause.cpp to clause.h for inlining.
 */

class dbg_L2rClause : public L2rClause {
 public:
  /// Save the "from" string.
  dbg_L2rClause(MBA_string from,
		unsigned clauseID,
                size_t propositionCount,
		const L2rProposition* const * propositions,
		bool copyPropositions = true);

  /// Nothing special to do.  
  l2_virtual ~dbg_L2rClause() { }
  
  /// Is this the debugging version? Yes, it is.
  l2_virtual bool hasDebug() const { return true; }
  
  /// Return the name of the model component to which this L2rClause refers.
  MBA_string component() const { return fromComponent_; }

 private:
  /// The name of the model component to which this L2rClause refers.
  MBA_string fromComponent_;
};
#endif


#ifdef ENABLE_L2_VERBOSE
/// operator<< for an L2rClause.
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rClause& clause) {
  clause.toOStream(os);
  return os;
}

/// operator<< for a pointer to an L2rClause
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rClause *pClause) {
  if (!pClause) {
    os << "(nil clause)";
  } else {
    pClause->toOStream(os);
  }
  return os;
}
#endif

#endif
