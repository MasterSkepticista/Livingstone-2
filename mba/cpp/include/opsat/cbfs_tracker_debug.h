/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: cbfs_tracker_debug.h,v 1.18 2004/01/28 23:31:38 lbrown Exp $ */

#ifndef _CBFS_TRACKER_DEBUG_H_
#define _CBFS_TRACKER_DEBUG_H_

#include <opsat/cbfs_tracker.h>
#include <debuggers/tracker_debug.h>

class L2_string_map;

/**
 * When Livingstone_debug::create_debugger() is called and the search style
 * is L2_parameters::cbfs, this becomes the "debugger".
 */

class CBFS_tracker_debug : public Tracker_debug {
public:

  /// Nothing special to do
  CBFS_tracker_debug(CBFS_tracker &tracker,
		     L2_string_map& map,
		     _STD_ ostream &outstream) :
    Tracker_debug(&tracker, map, outstream) { };

  /**
   * Traps the "nc" (next consistent Candidate) and "extend" (extend
   * Candidates) command lines.
   */
  virtual bool parse_cmd(const char *cmd, const char* args);

  /// Print a usage message to the object's ostream
  virtual void listener_usage() const;

  /// Return a string describing why the most recent search terminated
  virtual MBA_string search_termination() const;

  /// Return a pointer to the Tracker, known to be a CBFS_tracker
  CBFS_tracker *get_cbfs_tracker() {
    return static_cast<CBFS_tracker*>(get_tracker());
  }

  /// Return a const pointer to the Tracker, known to be a CBFS_tracker
  const CBFS_tracker *get_cbfs_tracker() const {
    return static_cast<const CBFS_tracker*>(get_tracker());
  }
};


#endif
