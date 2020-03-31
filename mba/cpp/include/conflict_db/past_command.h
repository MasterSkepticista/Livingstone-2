/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef PAST_COMMAND_H
#define PAST_COMMAND_H 1

#include <conflict_db/assumption.h>

/**
 * A Past_command is a Command that is assumed to have been issued in a
 * previous time step. It doesn't do anything; it is here for type safety.
 * \todo Move this to mba/cpp/transition
 */

class Past_command : public Assumption {
 public:
  Past_command(Conflict_db& conflict_db,
	       unsigned valueCount,
	       const Assignment::ListOfWeight& costs,
	       Theory::time_step_t timeStep,
	       unsigned ID) :
    Assumption (conflict_db, valueCount, costs, timeStep, ID) { }
};

#endif
