/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef STATE_VARIABLE_H
#define STATE_VARIABLE_H 1

#include <conflict_db/assumption.h>

/**
 * An Assumption that stores what is left after truncation. It doesn't do
 * anything; it is here for type safety.
 * \todo Move this to mba/cpp/transition
 */

class State_variable : public Assumption {
 public:
  State_variable(Conflict_db &conflict_db,
		 unsigned nvalues,
		 const Assignment::ListOfWeight& costs,
		 Theory::time_step_t time_step,
		 unsigned int id) :
    Assumption(conflict_db, nvalues, costs, time_step, id) { }

  virtual bool is_state_variable() const { return true; }
};

#endif
