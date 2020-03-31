/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef OBSERVABLE_H
#define OBSERVABLE_H 1

#include <conflict_db/assignable.h>

/**
 * An Observable, a value that comes from a sensor reading.
 * This class doesn't do anything; it is here for type safety.
 */

class Observable : public Assignable {
 public:
  /// Nothing special to do
  Observable(Conflict_db& conflict_db,
	     unsigned nvalues,
	     Theory::time_step_t time_step,
	     unsigned id) :
    Assignable (conflict_db, nvalues, time_step, id) { }

  /**
   * Make an observation: assign the indexed value to the Observable
   * \param valueIndex the index of the value; if negative, unassign.
   * Assignable::assign() will complain if valueIndex is too big
   */
  void observe(int valueIndex) {
    if (valueIndex < 0) { unassign(); } else { assign(valueIndex); }
    set_exogenously_assigned(valueIndex >= 0);
  }

  /// Is this Variable a Command? No, an Observable is never a Command.
  virtual bool is_command() const { return false; } // could default

  /// Is this Variable an Observable? Yes, it is.
  virtual bool is_observable() const { return true; }
};

#endif
