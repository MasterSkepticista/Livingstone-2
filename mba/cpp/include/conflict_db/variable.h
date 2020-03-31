/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict database, NASA Ames Research Center
// $Id: variable.h,v 1.69 2002/10/24 16:40:23 lbrown Exp $

#ifndef VARIABLE_H
#define VARIABLE_H

#include <mba_utils/slist.h>
#include <tms/proposition.h>
#include <tms/theory.h> // only for Theory::time_step_t

class Conflict_db;

/**
 * This class represents a Variable in the Conflict_db.
 */

class Variable : public Pooled {
 public:

  enum public_constants { NOT_FOUND = -1 };

  /// An ordered collection of Variable *
  typedef Slist<Variable *> ListOfVariable;

  /**
   * Destroy all Propositions and adjust previous and next pointers.
   * Use Conflict_db::destroy_variables() to correctly remove the Variable
   * from memory.
   */
  virtual ~Variable();

  /**
   * Return the number of values this Variable can assume (the cardinality of
   * its domain).
   * For instance, a boolean value has two values.
   * \return the cardinality of the domain
   */
  unsigned get_nvalues() const { return propositions.size(); }

  /**
   * Return the numeric ID associated with this Variable.
   * Its value is specified by the model file. All Variables descending from a
   * NOW variable have the same ID, though Variable and the Conflict_db don't
   * care about descendants at different time steps.
   * \return the ID
   */
  unsigned get_id() const { return id; }

  /**
   * Get the timestep of the Variable.
   * Variable and Conflict_db don't care about the time step.
   * \return the time step
   */
  Theory::time_step_t get_time_step() const { return time_step; }

  /**
   * Set the time step of the Variable.
   * Checkpointing uses this when it pushes back the time steps of Variables.
   * If not for this, it could be made const.
   * \param time_step the time step
   * \todo There should be more control on who can call this.
   */
  void set_time_step(Theory::time_step_t time_step) {
    this->time_step = time_step;
  }

  /**
   * Return the Proposition that sets this Variable's value, if any.
   * Return NULL if this Variable has no value.
   * The return value is meaningless if the TMS hasn't been propagated.
   * \return a pointer to the Proposition asserting the current value, or NULL
   * if the current value is unknown
   */
  Proposition *get_value();

  /**
   * Return the index of the Proposition that sets this Variable's value, if
   * any. This is also the index of the value in the Variable's domain. If the
   * Variable has no value (is unknown), return NOT_FOUND, a negative number.
   * The return value is meaningless if the TMS hasn't been propagated.
   * \return the index of the value in the domain, which is also the index of
   * the Proposition
   */
  int get_value_index() const;

  /**
   * Return the Proposition that represents Variable=value_i, whether or not
   * this is the current value of the Variable.
   * For example, for a boolean Variable b, if the domain were { true, false },
   * b.get_prop(1) would be Variable='false'.
   * \param i the value index
   * \return the indexed Proposition if it exists; otherwise throw an exception
   */
  Proposition *get_prop(unsigned i) const;

  /**
   * Is this Variable an Assignable?
   * Returns true if one can make assignments to this Variable. Each subclass
   * must implement this for itself, and should return true if it supports the
   * assign() method.
   * \return whether the Variable is Assignable
   */
  virtual bool is_assignable() const = 0;

  /**
   * Is the Variable an Assumption?
   * Returns true if this is a Variable whose value a search procedure is free
   * to change during its search (for example, a mode assignment).
   * \return whether the Variable is an Assumption
   */
  virtual bool is_assumption() const = 0;

  /**
   * Is the Variable a Command?
   * \return whether the Variable is a Command
   */
  virtual bool is_command() const = 0;

  /**
   * Is the Variable a Dependent?
   * \return whether the Variable is a Dependent
   */

  virtual bool is_dependent() const = 0;

  /**
   * Is the Variable an Observable?
   * \return whether the Variable is Observable
   */
  virtual bool is_observable() const = 0;

  /**
   * Is the Variable a State_variable?
   * \return whether the Variable is a State_variable
   * \todo The Conflict_db should not know about State_variable.
   */
  virtual bool is_state_variable() const = 0;

  /**
   * Is the Variable a Transition? Maybe.
   * \return whether the Variable is a Transition
   * \todo The Conflict_db should not know about Transition.
   */
  virtual bool is_transition() const = 0;

  /**
   * Is the Variable a Transitioned (mode)? Maybe.
   * \return whether the Variable is Transitioned (a mode)
   * \todo The Conflict_db should not know about Transitioned.
   */
  virtual bool is_transitioned() const = 0;

  /**
   * Return the Variable that corresponds to the same thing but at the closest
   * preceding time step where this Variable exists.
   * \return the predecessor if it exists; otherwise NULL
   */
  Variable* get_previous() const { return previous; }

  /**
   * Return the Variable that corresponds to the same thing but at the closest
   * following time step where this Variable exists.
   * \return the successor if it exists; otherwise NULL
   */
  Variable* get_next() const { return next; }

  /**
   * Insert a Variable before this one into the doubly-linked list of
   * Variables over time.  Can't insert nulls; unlinking can only be done by
   * deleting this Variable.
   * \param pVariable pointer to the Variable which is to become the
   * predecessor
   */
  void insert_previous(Variable* pVariable);

  /**
   * This is meaningful only for the Assignable subclass, but is defined here
   * as a no-op for purposes of type safety.
   * \todo Re-write the clients so that this method is not necessary, then
   * remove it.
   */
  virtual void save() { }

  /**
   * This is meaningful only for the Assignable subclass, but is defined here
   * as a no-op for purposes of type safety.
   * \todo Re-write the clients so that this method is not necessary.
   */
  virtual void restore() { }

  /**
   * This is meaningful only for the Assignable subclass, but is defined here
   * as a no-op for purposes of type safety.
   * \todo Re-write the clients so that this method is not necessary.
   */
  virtual void unassign() { }

 protected:

  /**
   * Protected constructor. We have abstract functions here, but this gives
   * an extra assurance we can't actually allocate any instances of this class.
   * \param conflict_db the Conflict_db that is to contain the Variable
   * \param domainCardinality the cardinality of the domain
   * \param time_step the time step
   * \param the ID
   */
  Variable(Conflict_db& conflict_db,
	   unsigned domainCardinality,
	   Theory::time_step_t time_step,
	   unsigned variableID);

 private:
  /**
   * An ID we can set at will on creation, but is thereafter immutable.
   * It's used to keep track of Variables in a reasonable manner based on the
   * model file. The conflict_db doesn't use this but the T_system does. Note
   * that the ID is not unique: indeed, the T_system has all Variables
   * corresponding to the same NOW variable share an ID. You would get a
   * unique identifier by also specifying an (ID, time_step) pair, except for
   * Transition and Transitioned (mode) Variables, which share an ID even
   * within a time step.
   */
  const unsigned id;
  
  /// The time step of the Variable.
  Theory::time_step_t time_step;

  /**
   * The list of all Propositions in which this variable is named.
   * They represent all the Assignments Variable=value that the domain implies.
   * The order is the same as the order in the domain, which is important for
   * the Assignable constructor.
   */
  Proposition::ArrayOfProposition propositions;

  // Useful when we create superclasses of the Conflict_db that deal with
  // Variables at different time steps.
  // These form a doubly-linked list maintained by set_previous.

  /**
   * The Variable of the same type with the same ID at the closest previous
   * time step.
   */
  Variable *previous;
  /**
   * The Variable of the same type with the same ID at the closest following
   * time step.
   */
  Variable *next;
};

#endif /* VARIABLE_H */

