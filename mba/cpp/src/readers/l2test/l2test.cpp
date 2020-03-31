/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: l2test.cpp,v 1.48 2004/01/29 19:17:12 lbrown Exp $ */

#include <stdlib.h> // atoi
#include <debuggers/livingstone_debug.h>
// for fc_style
#include <debuggers/tracker_debug.h>
#include <livingstone/version.h>
#include <opsat/cbfs_tracker.h>
// For the default settings, and setting the progress style.
#include <readers/from_any.h> // to get the file type
#include <readers/options.h>

// generic options
const int OPT_HELP              = 1;
const int OPT_VERBOSE           = 2;
const int OPT_VERSION           = 3;
const int OPT_HISTORY           = 4;
const int OPT_MAX_TRUNC_CAND    = 5;
const int OPT_PROGRESS          = 6;

#ifndef DISABLE_CBFS
// CBFS-only options
const int OPT_CBFS              =10;
const int OPT_CBFS_CANDIDATES   =11;
const int OPT_CBFS_SEARCHSIZE   =12;
const int OPT_CBFS_CANDIDATE_CLASSES =13;
#endif

#ifndef DISABLE_COVER
// cover-only options
const int OPT_COVER             =20;
const int OPT_COVER_MAXRANK     =21;
#endif

// optimizer options only
const int OPT_OPTIMIZE          =30;

const int OPT_RECOVERY          =40;

// Set of options and data about them from readers/options.h

const Option options[] = {
  { 'h', "help",    false, OPT_HELP,    "print this help message and exit" },
  { 'v', "verbose", true , OPT_VERBOSE,
    "print information when [r]eading, [o]ptimizing, [w]riting"},
  {  0 , "version", false, OPT_VERSION, "print version number and exit" },
  { 'y', "history", true,  OPT_HISTORY, "max history to hold; 0 is infinite"},
  {  0 , "max-truncate", true, OPT_MAX_TRUNC_CAND,
                               "max candidates to keep through truncate" },
  { 'p', "progress", true, OPT_PROGRESS, "use min or full progress"},
#ifndef DISABLE_CBFS
  // cbfs-only
  {  0 , "cbfs",    false, OPT_CBFS,   "use CBFS search (default)" },
  {  0 , "cbfs-candidates", true, OPT_CBFS_CANDIDATES,
                                      "max number of candidates" },
  {  0,  "cbfs-candidate-classes", true, OPT_CBFS_CANDIDATE_CLASSES,
     "max number of candidate classes" },
  {  0 , "cbfs-search", true, OPT_CBFS_SEARCHSIZE, "max nodes to search" },
#endif
#ifndef DISABLE_COVER
  // cover-only
  {  0 , "cover",   false, OPT_COVER,   "use conflict coverage search" },
  {  0 , "cover-max-rank", true, OPT_COVER_MAXRANK,
                                 "maximum-rank candidate to consider"},
#endif
  // model optimizer only
  { 'O', "optimize", false, OPT_OPTIMIZE, "optimize model"},
  // recovery
  { 'r', "recovery", false, OPT_RECOVERY, "run in recovery mode"},
};
unsigned noptions = sizeof(options)/sizeof(Option);

// Whether to run in recovery mode
static bool g_recovery = false;
void setRecovery(bool b) { g_recovery = b; }
bool isRecovery() { return g_recovery; }

// If it's not a recognized option, treat it as a file name

void doFile(Livingstone_debug& pack, OptionFinder& of) {
  if (pack.get_filename() != "") {
    _STD_ cerr << "l2test: Being told to load more than one file\n"
      "l2test: Previously was loading `"
	 << pack.get_filename()
	 << "', now requested to load `"
	 << of.optarg()
	 << "'\n";
    exit(1);
  } else {
    pack.set_filename(of.optarg());
  }
}


// Write a usage message and exit

void doHelp(Livingstone_debug& pack, OptionFinder& of) {
  _STD_ cerr << "Usage: l2test [options] <system>\n"
    "  Runs livingstone on the system, which must be in\n"
    "  the l2 format.\n";
  of.printOptions(_STD_ cerr);
  exit(0);
}

// Set verbosity levels (optimizer, reader, writer)

void doVerbose(Livingstone_debug& pack, OptionFinder& of) {
  const char *arg = of.optarg();
  bool value = true;
  for (unsigned i = 0; arg[i]; ++i) {
    switch(arg[i]) {
    case '-':
      // Unset verbose
      value = false;
      break;
    case '+':
      // Set verbose
      value = true;
      break;
    case 'A':
      // All (optimizing, reading, writing) have the same verbosity
      pack.set_verbosity(value, value, value);
      break;
    case 'r':
      // Verbose reading
      pack.getReader().set_verbose_reader(value);
      break;
    case 'o':
      // Verbose optimizing
      pack.getReader().set_verbose_optimizer(value);
      break;
    case 'w':
      // Verbose writing
      pack.set_verbose_writer(value);
      break;
    default:
      // Default verbosity
      break;
    }
  }
}


// Read the command-line options and carry them out

void processOptions(Livingstone_debug& pack, char **argv) {
  // read the command line; skip argv[0] -- the executable name
  OptionFinder of(argv+1, options, noptions);
  int option;
  do {
    option = of.next_option();
    switch (option) {
    case OPT_NOT_AN_OPTION:
      doFile(pack, of);
      continue;
    case OPT_ERROR:
      exit(1);
    case OPT_HELP:
      doHelp(pack, of);
    case OPT_VERBOSE:
      doVerbose(pack, of);
      continue;
    case OPT_VERSION:
      _STD_ cout<< "Livingstone version " << l2_version_string << _STD_ endl;
      exit(0);
    case OPT_HISTORY:
      pack.set_history( atoi(of.optarg()));
      continue;
    case OPT_MAX_TRUNC_CAND:
      pack.set_max_truncated_candidates( atoi(of.optarg()));
      continue;
    case OPT_PROGRESS:
      pack.set_progress_style_str(of.optarg());
      continue;
#ifndef DISABLE_CBFS
    case OPT_CBFS:
      pack.set_search_style(Tracker::cbfs);
      continue;
    case OPT_CBFS_CANDIDATES:
      pack.set_cbfs_maxcand( atoi(of.optarg()));
      continue;
    case OPT_CBFS_CANDIDATE_CLASSES:
      pack.set_cbfs_max_candidate_classes(atoi(of.optarg()));
      continue;
    case OPT_CBFS_SEARCHSIZE:
      pack.set_cbfs_search( atoi(of.optarg()));
      continue;
#endif
#ifndef DISABLE_COVER
    case OPT_COVER:
      pack.set_search_style(Tracker::cover);
      continue;
    case OPT_COVER_MAXRANK:
      pack.set_cover_maxrank(atoi(of.optarg()));
      continue;
#endif
#ifndef DISABLE_OPTIMIZER
    case OPT_OPTIMIZE:
      pack.set_use_optimizer(true);
      continue;
#endif
#ifdef ENABLE_RECOVERY
    case OPT_RECOVERY:
      // The tracker hasn't been created yet, so save a flag until it exists
      setRecovery(true);
      continue;
#endif
    } // end switch
  } while (option != OPT_DONE);
}


// The entry point: make a Livingstone_debug object; process options; read the
// model file; optimize if requested; create tracker; put model in tracker; run

int main(int /*argc*/, char **argv) {
  // By default, in diagnosis mode
  setRecovery(false);
  while (1) {
    // Reset the static variables before using in case the last run crashed
    reset_memory_flag();
    // Allocate the engine
    Livingstone_debug pack;
    processOptions(pack, argv);
    // The command line must supply the file
    if (pack.get_filename() == "") {
      _STD_ cerr << "l2test: No file specified\n";
      exit(1);
    }
    try {
      // read the model file, read search parameters file and, if needed,
      // optimize (also happens automagically if using min-progress)

      from_any::FileType type =
	from_any::discover_file_type(pack.get_filename());
      _STD_ cerr << "Reading model file(s): " << pack.get_filename();
      if (type == from_any::xmpl) { _STD_ cerr << ".{xmpl,hrn,ini}"; }
      _STD_ cerr << _STD_ endl;

      if (!pack.read_file()) { return 1; } // Failed to read the file

#ifndef DISABLE_OPTIMIZER
      // Optimize if requested
      if (pack.get_use_optimizer()) {
	if (!pack.getReader().verbose_optimizer()) {
	  _STD_ cerr << "Optimizing model ..." << _STD_ endl;
	}
	if (!pack.getReader().optimize()) { return 1; } // Failed to optimize model
      }
#endif
      pack.create_tracker();
      // Begin reconfiguration
      if (isRecovery()) {
	pack.get_tracker()->get_t_system().setPlanning();
      }
      // End reconfiguration
      // write the file into the tracker
      if (!pack.initialize_tracker()) { return 1; } // Failed to init tracker
      pack.create_debugger();
      pack.print_search_parameters();
      if (pack.run()) {	continue; } // reset
      else            {	return 0; } // exit successfully
    }
    catch(const L2_error& err) {
      _STD_ cerr << "l2test: " << err.error_string << _STD_ endl;
      return 1;
    }
  }
  return 0;
}
