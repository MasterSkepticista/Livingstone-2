/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_t_system_listener.h,v 1.7 2001/12/04 01:36:56 lbrown Exp $ */
#ifndef to_t_system_listener_HEADER
#define to_t_system_listener_HEADER

#ifndef DISABLE_TO_T_SYS_LISTEN

#include <livingstone/l2conf.h>

class Clause;
class L2rClause;
class L2rVariable;
class Variable;
class to_t_system;

/**
 * This abstract class declares pure virtual callback methods for interesting
 * events from the to_t_system writer.
 */

class to_t_system_listener {
 public:
  /// Called when a Variable is created from an L2rVariable.
  virtual void created_variable(const L2rVariable* pL2rVariable,
				Variable& variable) = 0;

  /**
   * Called when a transition or background Clause is created from an
   * L2rClause.
   */
  virtual void created_clause(const L2rClause* pL2rClause,
			      Clause& clause) = 0;

  /// Called for mutex Clause is created for an L2rVariable.
  virtual void created_clause(const L2rVariable* pL2rVariable,
			      Clause& clause)=0;

  /// Called when the to_t_system writer is destroyed.
  virtual void destroying_container(to_t_system& writer) = 0;
};
#endif

#endif
