/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Incremental Truth Maintenance System, NASA Ames Research Center
// $Id: proposition.h,v 1.54 2004/02/05 22:36:15 lbrown Exp $

#ifndef PROPOSITION_H
#define PROPOSITION_H

#include <mba_utils/array.h>
#include <mba_utils/pooled.h>

#ifdef VXWORKS531
// Forward declaration of templates doesn't work
#include <mba_utils/slist.h>
#else
template <class Type> class Slist;
#endif

/**
 * This class represents a proposition in a propositional theory. A Clause is
 * a disjunction of Proposition literals.
 */

class Proposition : public Pooled {
public:

  friend class Clause;
  friend class Ptheory;
  friend class LTMS;

  /// The type of the truth value for the Proposition
  typedef signed char truth_value;

  /// An iterator over back pointers to the Clauses that have this Proposition
  /// as a literal
  typedef Array<Clause*, false>::iterator iterator;

  /// An ordered collection of Proposition *
  typedef Slist<Proposition *> ListOfProposition;
  typedef Array<Proposition *, false> ArrayOfProposition;

  /// Three-valued logic truth value constants
  enum truth_value_t {
    PFALSE = 0,
    PTRUE = 1,
    UNKNOWN = 2
  };

public:

  /**
   * Propositions should be marked by set_destroying() and then removed by
   * Ptheory::finish_destroying(); this destructor is for Ptheory's use.
   */

  ~Proposition() { }

  /**
   * Re-initialization after construction.
   * \TODO What about flags?
   */
  void initialize() {
    // For the Ptheory
    set_value(Proposition::UNKNOWN);
    // For the LTMS
    set_supported_by(NULL);
  }

  /**
   * Get ready for deallocation.
   * The idiom is that we mark everything we want to get rid of, then ask the
   * TMS to clean itself up.
   */
  void set_destroying();

  /**
   * Is the Proposition marked for destruction?
   */
  bool is_destroying() const { return get_flag(DESTROYING); }

  /**
   * Return the truth value of the Proposition.
   */
  truth_value value() const { return tv; }

  /**
   * Return whether the Proposition's truth value is PTRUE.
   */
  bool isTrue() const { return (tv == PTRUE); }

  /**
   * Return whether the Proposition's truth value is PFALSE.
   */
  bool isFalse() const { return (tv == PFALSE); }

  /**
   * Return whether the Proposition's truth value is UNKNOWN.
   */
  bool isUnknown() const { return (tv == UNKNOWN); }

  /**
   * Return the Clause whose consequence is Proposition, if such a Clause
   * exists, and NULL otherwise.
   */
  Clause* get_supported_by() { return supported_by; }

  /// Is the "trace" flag set? Apparently unused.
  bool is_traced() const { return get_flag(TRACE); }

  /// Is the "break" flag set? Apparently unused.
  void set_traced(bool trace) { set_flag(trace, TRACE); }

  /// Is the "break" flag set? Apparently unused.
  bool is_break() const { return get_flag(BREAK); }

  /// Set the "break" flag. Apparently unused.
  void set_break(bool brk) { set_flag(brk, BREAK); }

  //----------------------------------------------------------------------
  // Iterator methods
  //----------------------------------------------------------------------

  /// Begin iterator over the positive conjuncts.
  iterator begin_positive() const { return positive.begin(); }

  /// End iterator over the positive conjuncts.
  iterator end_positive()   const { return positive.end();   }

  /// Begin iterator over the negative conjuncts.
  iterator begin_negative() const { return negative.begin(); }

  /// End iterator over the negative conjuncts.
  iterator end_negative()   const { return negative.end();   }

private:
  /**
   * Propositions should be created by a call to Ptheory::create_prop() rather
   * than with this constructor; the constructor here is provided for the use
   * of class Ptheory.
   */
  Proposition() : flags(0), tv(UNKNOWN), supported_by(NULL) { }

  // Current flag assignments are:
  //   bit 0 : (TRACE) set if the proposition is being traced
  //   bit 1 : (BREAK) set if changes in the proposition's truth value can lead
  //            to a break
  //   bit 2 : (DESTROYING): set if we will be destroying this proposition
  //            upon Ptheory::finish_destroying
  enum bit_masks {
      TRACE     = 1<<0,
      BREAK     = 1<<1,
      DESTROYING= 1<<2
  };

  // is_break(), is_destroying(), is_traced()
  bool get_flag(bit_masks mask) const { return (flags & mask) != 0; }

  // set_break(), set_traced()
  void set_flag(bool b, bit_masks mask) {
    if (b) { flags |=  mask; } // set true
    else   { flags &= ~mask; } // set false
  }

  /**
   * Set the truth value.
   * \param value the truth value
   * Used by class Ptheory and class LTMS as a friend
   */
  void set_value(Proposition::truth_value value) { tv = value; }

  /**
   * When the Proposition is assigned a truth value, this is the fringe Clause
   * that was satisfied by the assignment.
   * \param pClause pointer to the Clause that constrains the Proposition
   */
  void set_supported_by(Clause* pClause) { supported_by = pClause; }

 protected:

  /**
   * Add a Clause where this Proposition appears with a positive sign.
   * Assumes the Clause does not already appear in our list.
   * \param pClause pointer to a Clause that is to be added
   */
  void add_positive(Clause* pClause) { positive.push(pClause); }

  /**
   * Add a Clause where this Proposition appears with a negative sign.
   * Assumes the Clause does not already appear in our list.
   * \param pClause pointer to a Clause that is to be added
   */
  void add_negative(Clause* pClause) { negative.push(pClause); }

  /**
   * Removes the knowledge about a Clause in which we appear as a positive
   * literal. Assumes the Clause appears at most once in our list.
   * \param pClauseToRemove const pointer to a Clause that is to be removed
   */
  void remove_positive(const Clause* pClauseToRemove) {
    for (iterator it = begin_positive(); it != end_positive(); ++it) {
      Clause *pClause = *it;
      if (pClause == pClauseToRemove) {
	positive.erase(it);
	return;
      }
    }
  }

  /**
   * Removes the knowledge about a Clause in which we appear as a negated
   * literal. Assumes the Clause appears at most once in our list.
   * \param pClauseToRemove const pointer to a Clause that is to be removed
   */
  void remove_negative(const Clause* pClauseToRemove) {
    for (iterator it = begin_negative(); it != end_negative(); ++it) {
      Clause *pClause = *it;
      if (pClause == pClauseToRemove) {
	negative.erase(it);
	return;
      }
    }
  }

 private:
  /// Various boolean properties: trace, break, destroying
  unsigned char     flags;

  /// The truth value of
  truth_value       tv;

  /// The set of Clauses in which this appears as a positive literal
  Array<Clause*, false> positive;

  /// The set of Clauses in which this appears as a negative literal
  Array<Clause*, false> negative;

  /// A pointer to the Clause that constrains this Proposition.
  Clause* supported_by;

};

#endif // PROPOSITION_H
