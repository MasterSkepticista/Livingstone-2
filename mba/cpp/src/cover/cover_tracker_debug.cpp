/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Non-flight debugging/inspection code for Conflict_db

#include <stdlib.h>
#include <cover/cover_tracker_debug.h>
#include <livingstone/L2_assert.h>

bool Cover_tracker_debug::parse_cmd(char* cmd, char* args) {
  T_system& t_system = get_tracker()->get_t_system();
  if (!strcmp("find", cmd)) {
    if (t_system.get_conflicts()->empty())  {
      get_ostream() << "No conflicts discovered yet" << _STD_ endl;
    } else if (!t_system.getCandidates().empty())  {
      get_ostream() << "Tracker still has viable candidates" << _STD_ endl;
    } else {
      L2_TRY {
	get_tracker()->find_fresh_candidates();
      }
#ifdef ENABLE_EXCEPTIONS
      catch (L2_error lobj) {
	_STD_ cout << lobj.error_string << "\n";
      }
#endif
      get_ostream() << "At rho="
		    << ((Cover_tracker*)get_tracker())->get_gamma()
		    << " new candidates are:"
		    << _STD_ endl
		    << get_tracker()
		    << _STD_ endl;
    }
    return true;
  } else {
    // See if the parser common to all trackers can handle the command for us.
    return this->Tracker_debug::parse_cmd(cmd,args);
  }
}


void Cover_tracker_debug::listener_usage() const {
  get_ostream()
    << "Cover Tracker commands:\n"
    << "----------------------\n"
    << "  find                        --> start search from scratch\n"
    << _STD_ endl;
  
  Tracker_debug::listener_usage();
}
