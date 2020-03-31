/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone_debug.cpp,v 1.27 2004/02/06 18:53:28 lbrown Exp $ */
#include <debuggers/livingstone_debug.h>

#ifndef DISABLE_CBFS
#  include <opsat/cbfs_tracker_debug.h>
#endif

#ifndef DISABLE_COVER
#  include <cover/cover_tracker_debug.h>
#endif

// string mappings
#include <debuggers/L2_string_map.h>
#include <debuggers/L2_string_map_listener.h>

// _STD_ cout, fstream
#include <livingstone/L2_fstream.h>

// string & memory functions
#include <stdlib.h> // realloc
#include <ctype.h> // isspace


Livingstone_debug::Livingstone_debug() :
  map_(NULL), listener_(NULL), debugger_(NULL), line_alloc_(256)
#ifdef ENABLE_L2_DEBUG_SECTIONS
  , variableHashMap(hashpjw)
#endif
{
  line_ = (char*)malloc(line_alloc_);
}


Livingstone_debug::~Livingstone_debug() {
  free(line_);
  delete debugger_;
  delete listener_;
  delete map_;
}


void Livingstone_debug::create_debugger() {
  L2_assert(get_tracker() != NULL,
            L2_fatal_error,
	    ("creating debugger before tracker"));
  L2_assert(get_debugger() == NULL,
            L2_fatal_error,
	    ("creating two debuggers"));

  // Create the String map
  if (!map_) { map_ = new L2_string_map(); }

  // Create the tracker debugger
#ifndef DISABLE_CBFS
    if (get_use_cbfs_tracker()) {
      debugger_ =
	new CBFS_tracker_debug(static_cast<CBFS_tracker&>(*get_tracker()),
			       *map_,
			       _STD_ cout);
    }
#endif

#ifndef DISABLE_COVER
    if (!get_use_cbfs_tracker()) {
      debugger_ =
	new Cover_tracker_debug(static_cast<Cover_tracker&>(*get_tracker()),
				*map_,
				_STD_ cout);
    }
#endif

    L2_assert(debugger_ != NULL,
	      L2_fatal_error,
	      ("no debugger was created"));
}


bool Livingstone_debug::run(_STD_ istream& input, bool is_interactive) {
  L2_assert(get_debugger(),
	    L2_argument_error,
            ("running before having created a debugger"));

  // If true, echo commands before executing them. No need to make this a member variable
  bool isEchoingCommands = false;

  while (input) {
    // Prompt for input
    _STD_ cout << "L2> ";
    _STD_ cout.flush();

    // Read the line of input
    input.getline(line_, line_alloc_);
    if (input.eof() || input.bad()) { return false; }
    unsigned len = 0;
    while (input.fail()) {
      input.clear();
      line_alloc_ *= 2;
      line_ = (char*)realloc(line_, line_alloc_);
      len += strlen(line_+len);
      input.getline(line_+len, line_alloc_-len);
      if (input.eof() || input.bad()) return false;
    }

    // wipe out comments
    char *comment_start = strstr(line_, "//");
    if (comment_start) *comment_start = 0;

    // skip initial whitespace
    char *cmd = line_;
    while (isspace((int)*cmd)) ++cmd;

    // check if there's anything left.
    if (*cmd=='\n' || *cmd=='\0') continue;

    // find the end of cmd, skip whitespace
    char *args = cmd;
    while (!isspace((int)*args) && *args!='\0') ++ args;
    if (*args) {
      // null out the space and skip it
      *args = 0;
      ++args; // if it was the newline, this will give args=""
    }
    // skip any further spaces, allowing "progress   a=5"
    while (isspace((int)*args)) ++args;

    // Echo commands to _STD_ cout if requested
    if (isEchoingCommands) {
      _STD_ cout << cmd << " " << args << _STD_ endl;
    }

    // The command is parsed. Interpret it.

    if (!strcmp("exit", cmd) ||
	!strcmp("quit", cmd) ||
	!strcmp("bye" , cmd)) {
      break;
    } else if (!strcmp("echo", cmd)) {
      _STD_ cout << args << _STD_ endl;
    } else if (!strcmp("echo-commands", cmd)) {
      if (!strcmp("on", args)) {
	isEchoingCommands = true;
      } else if (!strcmp("off", args)) {
	isEchoingCommands = false;
      } else {
	_STD_ cout << "echo-commands is "
		  << (isEchoingCommands ? "on\n" : "off\n");
      }
    } else if (!strcmp("load-scenario", cmd)) {
      _STD_ ifstream file(args);
      if (!file) {
	_STD_ cerr << "Error opening `" << args << "'\n";
	continue;
      } else {
	run(file);
      }
    } else if (is_interactive && !strcmp("restart", cmd)) {
      return true;
    } else if (!strcmp("help", cmd)) {
      _STD_ cout << 
	"Overall commands:\n"
	"-----------------\n"
	"  exit | quit | bye | ^d      : exit from Livingstone\n"
	"  echo                        : echo arguments\n"
	"  echo-commands { on | off }  : echo commands and arguments\n"
	"  help                        : print this message\n"
	"  restart                     : throw it all away and start anew\n"
	"  load-scenario <file>        : run the commands in the scenario file\n"
	"\n";
      debugger_->listener_usage();
    } else {
      L2_TRY {
	if (!debugger_->parse_cmd(cmd, args))
	  _STD_ cerr<< "Didn't understand command `"
		   << cmd << "'; try help\n";
      }
#ifdef ENABLE_EXCEPTIONS
      catch(L2_error err) {
	_STD_ cerr << "l2test: " << err.error_string << _STD_ endl;
      }
#endif
    }
  }
  return false;
}


void Livingstone_debug::init_tracker_created_writer(to_t_system* writer) {
  if (!map_) { map_ = new L2_string_map(); }
  listener_ =
    new L2_string_map_listener(*map_,
			       *getReader().get_file(),
			       *writer,
			       *get_tracker());
}


#ifdef ENABLE_L2_DEBUG_SECTIONS

const MBA_string
Livingstone_debug::getRepresentativeVariable(const MBA_string& from) {
  MBA_string latest = from;
  while (true) {
    MBA_string to = getVariableMapping(latest);
    if (to == "") { return latest; } // end of the line
    else          { latest = to; }   // keep looking
  }
}


const MBA_string
Livingstone_debug::getVariableMapping(const MBA_string& from) {
  Hash_table<MBA_string, MBA_string>::iterator it =
    variableHashMap.find(from);
  return (it == variableHashMap.end()) ? "" : *it;
}


// This needs to be in Livingstone_debug because it needs Tracker_debug

void Livingstone_debug::collectAllVariableNames() {
  allVariableNames.erase(); // static
  // The variables in the Conflict_db
  Tracker_debug *pTracker_debug = get_debugger();
  {
    for (Variable::ListOfVariable::iterator it =
	   get_t_system().begin_variables();
	 it != get_t_system().end_variables(); ++it) {
      Variable *pVariable = *it;
      MBA_string variableName = pTracker_debug->get_var_name(*pVariable);
      addVariableName(variableName); // static
    }
  }
  {
    // The variables that have been optimized away
    for (Array<MBA_string, true>::iterator it =
	   beginOptimizedAwayVariableNames();
	 it != endOptimizedAwayVariableNames(); ++it) {
      MBA_string variableName = *it;
      addVariableName(variableName); // static
    }
  }
}


// This has to be in Livingstone_debug because it needs Tracker_debug
// The method can't be const because get_debugger() isn't const
// pTrackerDebug can't be const because find_variable() isn't const

const Variable* Livingstone_debug::getRepresentative(const MBA_string&
						     variableName) {
  Tracker_debug *pTrackerDebug = get_debugger();
  // The name of the Variable it is mapped to, if anything
  const MBA_string toName = getRepresentativeVariable(variableName); // static
  if (toName == "") {
    // It was not optmized away; use VariableName
    return pTrackerDebug->find_variable(variableName, true);
  } else {
    // It was optimized into toName
    return pTrackerDebug->find_variable(toName, true);
  }
}

#endif
