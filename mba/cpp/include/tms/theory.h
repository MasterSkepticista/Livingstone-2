/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// A class that provides the generic interface of a theory.
// $Id: theory.h,v 1.19 2002/01/02 21:31:21 lbrown Exp $
//

#ifndef THEORY_H
#define THEORY_H

#include <mba_utils/pooled.h>

/**
 * This has no subclasses or members, and is now used only for its typedef
 * time_step_t
 */

class Theory : public Pooled {
 public:
  /**
   * The type of a time step.
   * \todo Find the right place for this. Conflict_db, T_system and Tracker use
   * it, but not the ltms. When the typedef is moved, this class can go away.
   */
  typedef unsigned int time_step_t;

 protected:
  /**
   * No one actually subclasses this, but making it private would cause
   * warnings.
   */
  Theory() { }
};

#endif // THEORY_H
