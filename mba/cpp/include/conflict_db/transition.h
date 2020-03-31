/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef TRANSITION_H
#define TRANSITION_H 1

#include <conflict_db/assumption.h>

/**
 * A Transition is an Assumption about the relationship of modes on successive
 * time steps. It doesn't do anything; it is here for type safety.
 * \todo Move this to mba/cpp/transition/
 */

class Transition : public Assumption {
 public:

  /// An ordered collection of Transition *
  typedef Slist<const Transition *> ListOfTransition;

  /// Nothing special to do
  Transition(Conflict_db& conflict_db,
	     unsigned valueCount,
	     const Assignment::ListOfWeight& costs,
	     Theory::time_step_t timeStep,
	     unsigned ID) :
    Assumption (conflict_db, valueCount, costs, timeStep, ID) { }

  /// Is this Variable a Transition? Yes, it is.
  virtual bool is_transition() const { return true; }
};

#endif
