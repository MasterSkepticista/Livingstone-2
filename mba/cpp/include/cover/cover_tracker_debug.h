/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// A nonflight class that contains debugging functions
// $Id: cover_tracker_debug.h,v 1.19 2004/01/28 23:31:34 lbrown Exp $

#ifndef COVER_TRACKER_DEBUG_H
#define COVER_TRACKER_DEBUG_H

#include <cover/cover_tracker.h>
#include <debuggers/tracker_debug.h>

class L2_string_map;

/**
 * Enable command-line interaction with the Cover_tracker.
 * When Livingstone_debug::create_debugger() is called and the search style is
 * L2_parameters::cover, this becomes the "debugger".
 */

class Cover_tracker_debug : public Tracker_debug {
public:
  /// Nothing special to do
  Cover_tracker_debug(Cover_tracker &tracker,
		      L2_string_map& map,
		      _STD_ ostream& outstream) :
    Tracker_debug(&tracker, map, outstream) { };

  /**
   * Interpret the command line, delegating to Tracker_debug if unable.
   * Responds only to the "find" command.
   */
  virtual bool parse_cmd(char* cmd, char* args);

  /// Write a usage message to the ostream.
  virtual void listener_usage() const;

  /**
   * Print a message that this method is unimplemented.
   * \todo Implement Cover_tracker_debug::search_termination
   */
  virtual MBA_string search_termination() const {
    return "Cover: search termination is unimplemented";
  }
};

#endif // COVER_TRACKER_DEBUG_H
