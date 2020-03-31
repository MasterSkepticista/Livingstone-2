/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: assumption.h,v 1.24 2002/08/08 00:05:16 lbrown Exp $

#ifndef ASSUMPTION_H
#define ASSUMPTION_H

#include <conflict_db/assignable.h>

/**
 * An Assumption is a Variable whose value we are free to re-assign during a
 * search.  This is as opposed to a Dependent Variable, whose value will be
 * inferred, or a premise whose value is set and cannot change during search
 * (e.g., an Observble or a Command).
 *
 * \todo Much of the functionality of the Assignable class should be moved into
 * this class, as it does not apply to Variables we do not search over.
 */

class Assumption : public Assignable {
  friend class Conflict_db;

public:

  /// An ordered collection of Assumption *
  typedef Slist<Assumption *> ListOfAssumption;

  /**
   * An Assumption is an Assignable with a mapping from Assignments to costs.
   * The costs must be positive and sorted in increasing order. Thus the first
   * member of the domain must be the default (i.e., lowest cost) Assignment.
   * The costs are copied to the Assignments.
   */
  Assumption(Conflict_db &db,
	     unsigned nvalues,
	     const Assignment::ListOfWeight& costs,
	     Theory::time_step_t time_step,
	     unsigned id);

  /// The destructor has nothing special to do
  virtual ~Assumption() { }

  /**
   * Is this Variable an Assumption?
   * Yes, it is.
   */
  virtual bool is_assumption() const { return true; }

  /**
   * Is this Variable a Command?
   * No, an Assumption is never a Command.
   */
  virtual bool is_command() const { return false; } // could default

  /**
   * Is this Variable Observable?
   * No, an Assumption is never an Observable.
   */
  virtual bool is_observable() const { return false; } // could default

  /// Is the given Assignment this Assumption's default Assignment?
  virtual bool is_default(const Assignment& assignment) const {
    return (&assignment == default_assignment);
  }

  /// Restore the initial (default) Assignment.
  void restore_default() { assign(default_assignment); }

private:

  /**
   * The default, nominal Assignment to this Assumption Variable.
   * This is the 0th Assignment, cached for convenience.
   * \todo Determine whether caching the default Assignment is worthwhile.
   */
  const Assignment * const default_assignment;
};

#endif /* ASSUMPTION_H */
