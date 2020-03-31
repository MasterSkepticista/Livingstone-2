/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict database, NASA Ames Research Center
// See Copyright.txt
// $Id: assignment.h,v 1.69 2004/01/26 20:23:18 lbrown Exp $

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <mba_utils/slist.h>
#include <tms/theory.h>

class Assignable;
class Assumption;
class Clause;
class Conflict;

/**
 * An Assignment represents one possible assignment of a value to an Assignable
 * Variable. Each Assignable Variable contains as many Assignment objects as
 * values in its domain. The lifetime of an Assignment is the lifetime of its
 * Assignable Variable. These are the only Assignment objects in the system.
 * Pointers to these objects are used in many places, such as in observing and
 * commanding and in Candidates.
 * An Assignment is an (Assignable, Value Index, Weight) triple, with some
 * cached data. It refers to the corresponding Clause in the LTMS that asserts
 * the specified value of the Assignable Variable.
 */

class Assignment : public Pooled {
  friend class Assignable;
  friend class Assumption;

public:
  /// An ordered collection of Assignment *
  typedef Slist<Assignment *> ListOfAssignment;

  /// An ordered collection of const Conflict *
  /// Also defined as Conflict::ListOfConflict, but can't #include conflict.h
  typedef Slist<const Conflict *> ListOfConflict;

  /// Initialize the fields we can at this point.
  Assignment(Assignable* pAssignable, unsigned int i) :
    weight(0), variable(pAssignable), index(i), clause(NULL) { }

  /// No subclasses ==> we needn't have a virtual destructor.
  ~Assignment() { }

  /// The assumption is that weight of an Assignment is a non-negative number;
  /// the lowest weight is weight 0 (we used to have a constant for that, but
  /// it was underused).
  typedef unsigned weight_t;

  /// An ordered collection of weight_t
  typedef Slist<weight_t> ListOfWeight;

  /// Notify the Assignment that a new Conflict involving it has been
  /// discovered.
  void add_conflict(const Conflict& conflict);

  /// Notify the Assignment that a Conflict involving it has been deleted
  void remove_conflict(const Conflict& conflict);

  /// Two Assignments are inconsistent if they are Assignments to the same
  /// Variable and are not exactly the same Assignment.
  bool is_inconsistent(const Assignment& assignment) const {
    return
      (&assignment != this) &&
      (assignment.variable == variable);
  }

  /// Return the weight (cost, probability, etc) of using this Assignment
  weight_t get_weight() const { return weight; }

  /// Set the weight of the assignment to be new_weight.  The caller is
  /// responsible for making sure the weights remain sorted; the search will
  /// fail otherwise.
  void set_weight(weight_t new_weight) { weight = new_weight; }

  /// Return the Assignment's Variable as const
  const Assignable* get_variable() const { return variable; }

  /// Return the Assignment's Variable
  Assignable* get_variable() { return variable; }

  /// Return the Assignment's Variable's time step
  Theory::time_step_t get_time_step() const;

  /// Return a const reference to the Conflicts resolved by this Assignment
  const ListOfConflict& get_resolved_conflicts() const {
    return resolved_conflicts;
  }

  /// Return a const reference to the Conficts that involve this Assignment
  const ListOfConflict& get_conflicts() const { return conflicts; }

  /// Append to the input list the siblings of this Assignment
  void append_other_values(ListOfAssignment& assignments);

  /// Does the Assignment contain the given Conflict? Parameter should be const
  bool is_in_conflict(const Conflict *pConflict) const {
    return (conflicts.contains(pConflict));
  }

  /// Does the Assignment resolve the given Conflict? Parameter should be const
  bool resolves_conflict(const Conflict *pConflict) const {
    return (resolved_conflicts.contains(pConflict));
  }

  /// Conflicts can be rediscovered, so it's safe to allow clearing them all.
  void erase_conflicts() {
    conflicts.erase();
    resolved_conflicts.erase();
  }

  /// Returns true if the Assignment is to an Assumption Variable.
  bool is_assumption() const;

  /// Ensures any conflicting Assignments are undone and assigns itself.
  /// Note the implementation of this, assign(tms) and unassign(tms) are
  /// hokey and need to be rethought.  See the comment on the protected
  /// assign(tms) member
  void assign() const;

  /// GT for total ordering on weight
  bool operator>(const Assignment& b) const { return weight > b.get_weight(); }

  /// LT for total ordering on weight
  bool operator<(const Assignment& b) const { return weight < b.get_weight(); }

  /// Return the index of the Variable's value for this Assignment
  unsigned int get_index() const { return index; }

  /// Return the LTMS Clause that corresponds to this Assignment
  Clause *get_clause() const { return clause; }

protected:
  /// Set the Clause of the Assignment after the Assignment has
  // The Clause an Assignment uses to represent itself will have a pointer
  // back to the Assignment.  Therefore we create the Assignment, pass it to
  // the Clause constructor, and set the Assignment's Clause.
  void set_clause(Clause* pClause) { clause = pClause; }

private:
  /// The weight (cost, probability, etc) of using this Assignment
  weight_t weight;

  /// The Assignable Variable that this is an Assignment of.
  Assignable* const variable;

  /// The ordinal number of this Assignment within the lists of Assignments
  /// to the Variable this Assignment belongs to
  const unsigned int index;

  /// We might want to put everything below here into a propositional
  /// assignment subclass.

  /// The Clause representing the fact that this Assignment has been made.
  Clause* clause;

  /// Conflicts in which Variable appears with a different value than this.
  /// Since this Assignment has a different value than appears in the
  /// Conflict, using this Assignment resolves the Conflict.
  ListOfConflict resolved_conflicts;

  /// The Conflicts in which this Assignment appears. The Assignments in
  /// append_other_values() resolve these Conflicts.
  ListOfConflict conflicts;
};

#endif /* ASSIGNMENT_H */
