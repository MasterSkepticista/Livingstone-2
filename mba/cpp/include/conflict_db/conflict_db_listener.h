/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: conflict_db_listener.h,v 1.6 2001/12/04 01:36:56 lbrown Exp $ */

#ifndef conflict_db_listener_HEADER
#define conflict_db_listener_HEADER

#include <livingstone/l2conf.h>

class Variable;
class Conflict_db;

/**
 * This abstract class defines pure virtual callback methods for interesting
 * events from the Conflict_db.
 */

class Conflict_db_listener {
 public:
  /// Called when the Conflict_db creates a Variable.
  virtual void created_variable(Variable& variable) = 0;

  /// Called when the Conflict_db destroys a Variable.
  virtual void destroying_variable(Variable& variable) = 0;

  /// Called when the Conflict_db is destroyed.
  virtual void destroying_container(Conflict_db& conflict_db) = 0;
};

#endif
