/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Incremental Truth Maintenance System, NASA Ames Research Center
// $Id: clause.h,v 1.63 2004/01/26 19:57:19 lbrown Exp $
//

#ifndef CLAUSE_H
#define CLAUSE_H

#include <limits.h>
#include <mba_utils/slist.h>
#include <tms/proposition.h>

/**
 * This class represents a clause, a disjunction of Proposition literals, any
 * number of which may be positive and any number of which may be negative.
 * A Clause may be declared as "volatile" if its truth value changes often.
 */

class Clause : public Pooled {
 public:

  friend class Ptheory;
  friend class LTMS;

  /// The type of an index into the Proposition set
  typedef signed char    length_t;
  /// An iterator over the Proposition set
  typedef Proposition**  iterator;
  /// Used in mark-and-sweep
  typedef unsigned short time_stamp_t;
  /// Ordered collection of Clause
  typedef Slist<Clause *> ListOfClause;

  /// Symbolic constants
  enum public_constants {
    /// A symbolic constant for an uninitialized time stamp
    MAX_TIME_STAMP = USHRT_MAX
  };

 private:
  /**
   * Create a new Clause with the specified positive and negative literals. A
   * literal is a Proposition or the negation of a Proposition.
   *
   * Note that this constructor creates the Clause but does not add it to
   * any theory. For this reason the constructor is private. Clauses
   * should be created via a Ptheory instance.
   *
   * Parameter datum is the datum of this new Clause. It is a pointer to
   * whatever item (object, string, etc) should be associated with this Clause
   * for whatever purpose the caller desires.  One might store a model
   * fragment that generated this Clause or something similar.
   *
   * Parameter isVolatile may be set to true if it's expected that this Clause
   * will often be temporarily removed then added back to the theory.
   * Truth-maintenance algorithms are free to use this advice to support
   * inferences based on non-volatile Clauses when there is a choice.
   * This may reduce the amount of re-support of existing inferences that
   * needs to be done.
   *
   * Note that no check is made to ensure that a Clause containing the same
   * literals has not already been created, so it is possible to create
   * multiple copies of what are essentially the same Clause. This should not
   * change the results of inference, but it is inefficient.
   */
  Clause(void* datum,
	 Proposition::ListOfProposition& posPropositions,
	 Proposition::ListOfProposition& negPropositions,
	 bool isVolatile = false);

 public:

  /// Deallocate the set of literals
  virtual ~Clause();

  /** Initialization after construction. */
  void initialize();

 protected:

  /**
   * Sets the Proposition that is supported by this Clause.
   * \param pTargetProposition pointer to the Proposition this Clause supports
   */
  void set_supports(Proposition* pTargetProposition);

  /**
   * Find and return a true postive literal or a false negative literal, if it
   * exists; otherwise return NULL.
   */
  const Proposition* find_satisfying_prop() const;

 private:

  /// Bit masks for the flags
  enum bit_masks {
    DELETED     = 1 << 0, // 1
    IN_FRINGE   = 1 << 1, // 2
    UNSATISFIED = 1 << 2, // 4
    TRACE       = 1 << 3, // 8
    BREAK       = 1 << 4, // 16
    VOLATILE    = 1 << 5, // 32
    DESTROYING  = 1 << 6, // 64
    TRANSITION  = 1 << 7  // 128
  };

  /// Proposition index denoting no proposition
  enum private_constants {
    NO_PROPOSITION = (length_t)-1
  };

  // Used by class Ptheory and class LTMS as a friend
  void set_unassigned_count(length_t n) { num_unassigned = n; }

  length_t incr_num_unassigned();

  length_t decr_num_unassigned();

  void set_time_stamp(unsigned short stamp) { time_stamp = stamp; }

  unsigned short get_time_stamp() const { return time_stamp; }

  void set_satisfying_prop(const Proposition* p) { satisfying_prop = p; }

  bool get_flag(bit_masks mask) const { return (flags & mask) != 0; }
  
  void set_flag(bool b, bit_masks mask) {
    if (b) { flags |=  mask; } // set true
    else   { flags &= ~mask; } // set false
  }

  void set_break(bool brk)           { set_flag(brk, BREAK); }
  void set_deleted(bool deleted)     { set_flag(deleted, DELETED); }
  void set_traced(bool trace)        { set_flag(trace, TRACE); }
  void set_unsatisfied(bool unsat)   { set_flag(unsat, UNSATISFIED); }
  void set_volatile(bool isVolatile) { set_flag(isVolatile, VOLATILE); }

 public:

  //----------------------------------------------------------------------
  // Public methods
  //----------------------------------------------------------------------

  /// Return the datum that was associated with the Clause when it was created.
  void* get_datum() const { return clause_datum; }

  /**
   * Return a pointer to the Proposition that is supported by this Clause,
   * or NULL if there is none.
   */
  Proposition* get_supports() const;

  /**
   * Return a pointer to the Proposition that is causing this Clause to be
   * satisfied in the current truth assignment, or NULL if this Clause is not
   * satisfied.
   */
  const Proposition* get_satisfying_prop() const { return satisfying_prop; }

  /// Return the total number of positive and negative literals in the Clause
  length_t number_of_literals() const { return num_lits; }

  /**
   * Returns the number of unassigned literals in the Clause (that is, those
   * with UNKNOWN truth value). This is useful for debugging the LTMS.
   */
  int get_unassigned_count() const { return (int) num_unassigned; }

  /**
   * This is like get_unassigned_count() except that, instead of returning
   * a cached value, it returns the result of counting.
   */
  length_t count_unassigned() const;

  /// Return the number of positive literals
  length_t number_of_positive_literals() const { return num_pos_lits; }

  /// Return the number of negative literals
  length_t number_of_negative_literals() const {
    return num_lits - num_pos_lits;
  }

  /// Has the Clause been temporarily removed from the theory?
  bool is_deleted() const { return get_flag(DELETED); }

  /**
   * To efficiently delete a list of Propositions, the user marks Clauses (or
   * the user marks Propositions, which in turn mark their Clauses) as being
   * in the process of 'DESTROYING' them, for mark-and-sweep.
   */
  void set_destroying() { set_flag(true, Clause::DESTROYING); }

  /// Return true if the Clause is marked to be permanently removed.
  bool is_destroying() const { return get_flag(DESTROYING); }

  /// Returns true if the Clause is in the fringe of unit Clauses that can be
  /// propagated.
  bool is_in_fringe() const { return get_flag(IN_FRINGE); }

  /// Return an index into the applicable (volatile or non-volatile) fringe.
  unsigned get_fringe_index() const { return fringe_index; }

  /**
   * Flag the Clause as being in the fringe and save the fringe index
   * (assuming it is in range).
   * \todo Only LTMS::Fringe should set this, but I can't figure out how to
   * befriend an inner class without #including the header.
   */
  void set_in_fringe(unsigned index) {
    set_flag(true, IN_FRINGE);
    fringe_index = index;
  }

  /// Is the Clause a transition Clause?
  bool is_transition() const { return get_flag(Clause::TRANSITION); }

  /// Specify at creation time whether the Clause is a transition Clause.
  void set_transition(bool b) { set_flag(b, Clause::TRANSITION); }

  /// Flag the Clause as not being in the fringe.
  void take_from_fringe() { set_flag(false, IN_FRINGE); }

  /// Is the Clause is unsatisfied?
  bool is_unsatisfied() const { return get_flag(UNSATISFIED); }

  /// Should the Clause be traced? Apparently unused.
  bool is_traced() const { return get_flag(TRACE); }

  /// Is the break" flag set? Apparently unused.
  bool is_break() const { return get_flag(BREAK); }

  /**
   * Is the Clause is volatile?
   * When a Clause is created, it can be marked as being volatile.
   */
  bool is_volatile() const { return get_flag(VOLATILE); }

  /// Does the Clause contain the Proposition?
  bool contains(const Proposition& proposition) const;

  //----------------------------------------------------------------------
  // Iterator methods
  //----------------------------------------------------------------------

  /// Returns an iterator at the start of all literals in the Clause
  iterator begin() const { return literals; }

  /// Returns an iterator at the end of all literals in the Clause
  iterator end() const { return literals + num_lits; }

  /// Returns an iterator at the start of the positive literals in the Clause
  iterator begin_positive() const { return literals; }

  /// Returns an iterator at the end of the positive literals in the Clause
  iterator end_positive() const { return literals + num_pos_lits; }

  /// Returns an iterator at the start of the negative literals in the Clause
  iterator begin_negative() const { return literals + num_pos_lits; }

  /// Returns an iterator at the end of the negative literals in the Clause
  iterator end_negative() const { return literals + num_lits; }

  //----------------------------------------------------------------------
  // Utility methods
  //----------------------------------------------------------------------

  /**
   * Return a Proposition whose value is UNKNOWN, if any.
   * Set is_positive as a side effect: true if the Proposition is positive,
   * false if negative
   */
  Proposition* get_unassigned_literal(bool& is_positive) const;

 private:

  /** Number of literals in Clause */
  length_t num_lits;

  /** Number of positive literals in Clause */
  length_t num_pos_lits;

  /** Number of unassigned literals in Clause */
  length_t num_unassigned;      

  /**
   * Index of Proposition supported by this Clause. An index of NO_PROPOSITION
   * denotes no supported Proposition.
   */
  length_t supports;            

  /**
   * The current flag assignment is as follows:
   *   bit 0 : set if the Clause is currently deleted
   *   bit 1 : set if the Clause is in the unit propagation fringe
   *   bit 2 : set if the Clause is in the set of unsatisfied Clauses
   *   bit 3 : set if the Clause is to be traced when debugging
   *   bit 4 : set if the Clause propagation should lead to a break
   *   bit 5 : set if the Clause is a volatile Clause
   *   bit 6 : set if the Clause is being deallocated
   *   bit 7 : set if the Clause is a transition Clause
   */
  unsigned char flags;

  /**
   * For quick removal from the fringe (which otherwise takes linear time).
   * This is a pointer into the fringe if non-volatile, volatile-fringe
   * otherwise.
   */
  unsigned fringe_index;

  /**
   * time_stamp is used to keep track of when we have visited a Clause when
   * traversing the theory (e.g., when doing explaination).  This ensures we
   * do not follow cycles indefinitely.  We could set a bit, but then we'd
   * have to go around and reset it.  With a time stamp, we just check to see
   * if we've visited the Clause on the current search. We should probably
   * change the name of time_stamp to visit_count or something similar to
   * avoid confusion with time_step.
   */
  time_stamp_t time_stamp;

  /** A proposition satisfying this Clause; the first one encountered. */
  const Proposition* satisfying_prop; 

  /**
   * Stores domain-specific information; what this Clause represents in the
   * client.
   */
  void* clause_datum;                 

  /**
   * literals is stored as a C array of Propositions. The first num_pos_lits
   * Propositions are positive literals, and the remaining are negative
   * literals. The size of the array is num_lits. This supports iteration over
   * all literals, or each of the subsets.
   */
  Proposition** literals;
};

#endif // CLAUSE_H
