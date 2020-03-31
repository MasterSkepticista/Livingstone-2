/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: assignable.h,v 1.61 2004/01/28 22:02:18 lbrown Exp $

#ifndef ASSIGNABLE_H
#define ASSIGNABLE_H

#include <conflict_db/assignment.h>
#include <conflict_db/variable.h>
#include <livingstone/L2_iostream.h>

class LTMS;

/**
 * Represents a Variable to which we can make assignments, as opposed to a
 * Dependent, whose value is inferred.
 * We need not make this distinction, but do so for safety and efficiency
 * reasons. We could give every Variable the methods and storage of an
 * Assignable and then only use it in Assignable Variables.
 */

class Assignable : public Variable {
  friend  void Assignment::add_conflict(const Conflict&);
  friend class Conflict_db;
  friend class T_system; // to allow sorting in install_new_state

public:
  /// An ordered collection of Assignable *
  typedef Slist<Assignable *> ListOfAssignable;

  /// An iterator over the set of Assignments
  typedef Assignment::ListOfAssignment::iterator iterator;

  /// The Conflict_db creates an Assignable with this constructor
  Assignable(Conflict_db &db, unsigned nvalues, Theory::time_step_t time_step,
	     unsigned id);

  /// Delete the Assignment set when truncated
  virtual ~Assignable();

  /// Return the previous Variable as an Assignable, which we know it will be.
  Assignable *get_previous_assignable() const {
    return static_cast<Assignable*> (get_previous());
  }

  /// Is the Assignment argument the same as the current Assignment?
  bool is_current(const Assignment& assignment) const {
    return (&assignment == current);
  }

  /// Is the Assignment argument the default Assignment?
  // Design decision? - Only assumptions should have default values.
  virtual bool is_default(const Assignment&) const { return false; }

  /// Return the current Assignment for this Assignable
  const Assignment* get_current() const { return current; }

  /// Does this Assignable have a current Assignment?
  bool is_assigned() const { return (current != NULL); }

  /// For each Assignment, erase its Conflict set.
  // Used in checkpointing. Conflicts can be rediscovered, so it's safe to
  // make this public.
  void erase_conflicts() {
    for (iterator it = assignments.begin(); it != assignments.end(); ++it) {
      Assignment *pAssignment = *it;
      pAssignment->erase_conflicts();
    }
  }

  /// Is this Variable Assignable? Yes, it is.
  virtual bool is_assignable() const { return true; }

  /// Is this Variable an Assumption? No, an Assignable is never an Assumption.
  virtual bool is_assumption() const { return false; }

  /// Is this Variable a Command? Maybe.
  virtual bool is_command() const { return false; } // default

  /// Is this Variable a Dependent? No, an Assignable is never a Dependent.
  virtual bool is_dependent() const { return false; }

  /// Is this Variable Observable? Maybe.
  virtual bool is_observable() const { return false; } // default

  /// Is this Variable a State_variable? Maybe.
  virtual bool is_state_variable() const { return false; } // default

  /**
   * Is this Variable a Transition? Maybe.
   * \todo The Conflict_db should not know about Transition.
   */
  virtual bool is_transition() const { return false; } // default

  /**
   * Is this Variable Transitioned (mode)? No, an Assignable is never
   * a Transitioned.
   * \todo The Conflict_db should not know about Transitioned.
   */
  virtual bool is_transitioned() const { return false; }

  /**
   * Assign this Variable with the Assignment whose ordinal number in the
   * domain is index.
   * \param valueIndex index of value
   */
  void assign(unsigned int valueIndex) {
    const Assignment* const pAssignment = get_assignment(valueIndex);
    // The only way pAssignment can be NULL is for index to be out of bounds
    L2_assert(pAssignment != NULL, L2_bounds_error,
	      ("assignments", valueIndex, assignments.size()));
    assign(pAssignment);
  }

  /**
   * Assign this Variable the same way as the passed Variable.
   * The Assignment with the same value name as the current Assignment of the
   * passed Variable is found and assigned to this Variable.
   * \param assignable the NOW version of this Assignable, the value of which
   * is being copied to this newly-created past Assignable
   */
  void assign(const Assignable& assignable) {
    const Assignment* const pAssignment = assignable.get_current();
    if (pAssignment == NULL) { unassign(); }
    else                     { assign(pAssignment->get_index()); }
  }

  /**
   * Assign the Variable to the passed Assignment.
   * A null Assignment means to unassign.
   * \pAssignment either NULL or a pointer to one of this Assignable's
   * Assignments
   */
  void assign(const Assignment *pAssignment);

  /** 
   * Make the variable be unassigned.
   * This overrides the no-op defined for class Variable.
   */
  void unassign();

  /**
   * Copies the current Assignment into a safe place, allowing searches to
   * reassign the Variable as desired.
   * Note that sequences of calls to save() will overwrite each other (i.e.,
   * this is not like a multi-level undo). This overrides the no-op defined
   * for class Variable.
   */
  void save() { saved_assignment = current; }

  /**
   * Reassigns the saved default value as the current value.
   * This should be called after save(). This overrides the no-op defined for
   * class Variable.
   */
  void restore() { assign(saved_assignment); }

  /** 
   * Restores the best-weight initial assignment.
   * This method may eliminate the need for save and restore.
   */
  void restore_initial_default() {
    // Get the first Assignment
    iterator it = assignments.begin();
    Assignment *pAssignment = *it;
    // Assign it if needed
    if (pAssignment != current) { assign(pAssignment); }
  }

  /**
   * Return the nth Assignment for this Assignable.
   * \todo If assignments were an Array instead of an Slist, this would be
   * trivial.
   */
  Assignment* get_assignment(unsigned int index);

  /**
   * Change the weight of one of the Assignments and re-sort the list.
   * \param assignment reference to the Assignment the weight of which is set
   * \param new_weight the weight to which the Assignment is to be set
   * \todo Sort the Assignments after setting the weight?
   */
  void set_weight(Assignment& assignment, Assignment::weight_t new_weight) {
    assignment.set_weight(new_weight);
  }

  //----------------------------------------------------------------------
  // Iterator methods
  //----------------------------------------------------------------------

  /// Begin iterating over the Assignment set
  Assignable::iterator assignments_begin() { return assignments.begin(); }

  /// End iterating over the Assignment set
  Assignable::iterator assignments_end()   { return assignments.end();   }

  /// Was the Assignment made exogenously (that is, not inferred)?
  bool is_exogenously_assigned() const { return exogenouslyAssigned; }

  /**
   * Assert whether the Assignment was made exogenously (that is, not
   * inferred).
   */
  void set_exogenously_assigned(bool b) { exogenouslyAssigned = b; }

private:

  /// The LTMS in which this Variable is tracked.
  LTMS * const tms;

  /**
   * The current Assignment to this Variable.
   * This Assignment may be made within the context of a search.
   */
  const Assignment* current;

  /**
   * The saved Assignment to this Variable, as determined by the save method.
   */
  const Assignment* saved_assignment;

  /**
   * Legal Assignments to this Variable.
   * These must appear in the order that they appear in the domain. Created
   * in the constructor, so the destructor must delete them.
   */
  Assignment::ListOfAssignment assignments;

  /// Did the value come from an assign (exogenous) or progress operation?
  bool exogenouslyAssigned;
};

#endif /* ASSIGNABLE_H */
