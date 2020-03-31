/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: tracker_listener.h,v 1.5 2002/05/13 18:08:15 lbrown Exp $ */
#ifndef tracker_listener_HEADER
#define tracker_listener_HEADER

class Candidate;
class Tracker;

/**
 * This abstract class declares pure virtual callback methods for interesting
 * events from the Tracker.
 */

class Tracker_listener {
 public:
  
  /// Called when the Tracker is destroyed.
  virtual void destroying_container(Tracker& tracker) = 0;
};

#endif
