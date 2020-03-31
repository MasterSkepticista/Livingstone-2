/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: ptheory_listener.h,v 1.5 2001/12/04 01:36:56 lbrown Exp $ */
#ifndef ltms_listener_HEADER
#define ltms_listener_HEADER

#include <livingstone/l2conf.h>

class Clause;
class Proposition;
class Ptheory;

/**
 * This abstract class declares pure virtual callback methods for interesting
 * events from the TMS.
 * The LTMS_debug class uses it, for instance, to make sure it doesn't keep
 * around destroyed Propositions.
 */

class Ptheory_listener {
 public:
  /// Called when the Ptheory creates a Proposition.
  virtual void created_proposition(Proposition& proposition) = 0;

  /// Called when the Ptheory creates a Clause.
  virtual void created_clause(Clause& clause) = 0;

  /// Called when the Ptheory destroys a Proposition.
  virtual void destroying_proposition(Proposition& proposition) = 0;

  /// Called when the Ptheory destroys a Clause.
  virtual void destroying_clause(Clause& clause) = 0;

  /// Called when the Ptheory is destroyed.
  virtual void destroying_container(Ptheory& ptheory) = 0;
};

#endif
