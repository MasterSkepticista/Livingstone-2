/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef COMMAND_H
#define COMMAND_H 1

#include <conflict_db/assignable.h>

/**
 * An Assignable Variable that represents a command issued to the device.
 * This class doesn't do anything; it is here for type safety.
 */

class Command : public Assignable {
 public:
  /// Nothing special to do
  Command(Conflict_db& conflict_db,
	  unsigned nvalues,
	  Theory::time_step_t time_step,
	  unsigned id) :
    Assignable (conflict_db, nvalues, time_step, id) { }

  /// Is this Variable a Command? Yes, it is.
  virtual bool is_command() const { return true; }
  /// Is this Variable an Observable? No, a Command is never an Observable.
  virtual bool is_observable() const { return false; } // could default
};

#endif
