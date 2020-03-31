/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict database, NASA Ames Research Center
// $Id: dependent.h,v 1.20 2002/01/08 22:50:55 lbrown Exp $

#ifndef DEPENDENT_H
#define DEPENDENT_H

#include <conflict_db/variable.h>

/**
 * This class represents a Variable whose value is inferred and cannot be set.
 * It can of course be set at the TMS level for debugging purpsoses, but at the
 * db level its value is inferred from the Clauses and Assignments in the db.
 * \todo This should be an abstract base class, with a new class declared for a
 * Dependent that is not a Transitioned.
 */

class Dependent : public Variable {
  friend class Conflict_db;

 public:
  /// Nothing special to do
  Dependent(Conflict_db& db, unsigned nvalues,
            Theory::time_step_t time_step, unsigned id) :
    Variable(db, nvalues, time_step, id) { }

  /// Nothing special to do.
  /// The destructor should be virtual because Transitioned is a subclass.
  virtual ~Dependent() { }

  /// Is this Variable an Assignable? No, a Dependent is never an Assignable.
  virtual bool is_assignable() const { return false; }
  /// Is this Variable an Assumption? No, a Dependent is never an Assumption.
  virtual bool is_assumption() const { return false; }
  /// Is this Variable a Command? No, a Dependent is never a Command.
  virtual bool is_command() const { return false; }
  /// Is this Variable a Dependent? Yes, it is.
  virtual bool is_dependent() const { return true; }
  /// Is this Variable an Observable? No, a Dependent is never an Observable.
  virtual bool is_observable() const { return false; }
  /**
   * Is this Variable a State_variable? No, a Dependent is never a
   * State_variable.
   */
  virtual bool is_state_variable() const { return false; }
  /**
   * Is this Variable a Transition? No, a Dependent is never a Transition.
   * \todo The Conflict_db should not know about Transitioned.
   */
  virtual bool is_transition() const { return false; }
  /**
   * Is this Variable a Transitioned (mode)? Maybe.
   * \todo The Conflict_db should not know about Transitioned.
   */
  virtual bool is_transitioned() const { return false; } // default

  /**
   * Return a pointer to the previous Variable as a Dependent, which we know
   * it will be.
   */
  Dependent *get_previous_dependent() const {
    return static_cast<Dependent*>(get_previous());
  }
};

#endif /* VARIABLE_H */
