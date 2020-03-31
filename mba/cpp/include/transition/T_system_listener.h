/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: T_system_listener.h,v 1.8 2001/12/04 01:36:56 lbrown Exp $ */
#ifndef T_system_listener_HEADER
#define T_system_listener_HEADER

#include <livingstone/l2conf.h>


class Assignable;
class Assumption;
class Dependent;
class Variable;
class T_system;

/**
 * This abstract class declares pure virtual callback methods for interesting
 * events from the T_system.
 */

class T_system_listener {
 public:
  /**
   * Called in progress to tell us about a new Variable and where it comes
   * from.
   */
  virtual void progressed_variable(Variable& now_var, 
				   Variable& newly_created_var) = 0;

  /**
   * Called in initialization and in progress to tell us about the
   * State_variable that holds the truncated (or initial) state.
   */
  virtual void created_state_variable(Assignable& state_var) = 0;

  /**
   * Called in progress to tell us about the assumption variable that sets the
   * dependent variable, which is a mode.
   */
  virtual void created_mode_transition(Dependent& mode,
				       Assumption& transition) = 0;

  /// Called when the T_system is destroyed.
  virtual void destroying_container(T_system&) = 0;
};
#endif
