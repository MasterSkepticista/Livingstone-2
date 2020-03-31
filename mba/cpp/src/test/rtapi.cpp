/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: rtapi.cpp,v 1.23 2001/11/16 19:32:43 lbrown Exp $ */

#include <readers/L2_file.h>
#include <readers/options.h>
#include <readers/from_xmpl.h>
#include <readers/to_t_system.h>

#ifndef DISABLE_CBFS
#  include <opsat/cbfs_tracker.h>
#  include <opsat/cbfs_tracker_debug.h>
#endif

#ifndef DISABLE_COVER
#  include <cover/cover_tracker.h>
#  include <cover/cover_tracker_debug.h>
#endif

#include <debuggers/L2_string_map.h>
#include <debuggers/L2_string_map_listener.h>

#include <livingstone/version.h>

#include <mba_utils/MBA_string.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <iostream.h>

#include <realtime_api/livingstonemessage.h>
#include <unistd.h>
#include <fstream.h>
#include <realtime_api/posix/livapi_rt_debug.h>
#include <realtime_api/userfcns_debug.h>
#include <time_delays.h>

const unsigned int MESSAGE_ARRAY_SIZE = 1024;


// message index, command, arg1(command or observable), arg2(value), arg3(time delay)
unsigned int msg_array[MESSAGE_ARRAY_SIZE][3];
unsigned int NUMBER_VALID_MSG_TYPES = 8;

const unsigned int WAIT = REPORT_FULL_STATE + 1;

// this function reads in an enum scenario and and stores it in the msg_array
// if error found in input, returns number of bad input line
int read_scenario(unsigned int &how_many, ifstream &fin)
{
	how_many = 0;
	int i=0;
	fin >> msg_array[i][0];
	while ( !fin.eof() && (i<1024)) 
	{
		if (msg_array[i][0] > NUMBER_VALID_MSG_TYPES)
		{cout << "Input error: Invalid message function on msg " << how_many;
		return how_many;}


		switch (msg_array[i][0])
		{
		case COMMAND:
		case OBSERVATION:
		case START_COMMAND_AND_TIME:
		case START_OBSERVE_AND_TIME:
			fin >> msg_array[i][1];
			fin >> msg_array[i][2];
			break;
		case WAIT:
			fin >> msg_array[i][1];
			break;
		case FIND_CANDIDATES:
		case REPORT_FULL_STATE:
			break;
		}
		how_many++;
		i++;
		if (how_many == 1024)
			return 0;
		fin >> msg_array[i][0];

	}	
	return 0;

}


void rt_input(unsigned int input_number, L2_rtapi *theapi)
{
	for (unsigned i=0; i<input_number; i++)
	{
		switch(msg_array[i][0]) {
		case COMMAND:
			theapi->queue_command(msg_array[i][1], msg_array[i][2]);
			break;
		case OBSERVATION:
			theapi->queue_observation(msg_array[i][1], msg_array[i][2]);
			break;
		case START_COMMAND_AND_TIME:
			theapi->queue_start_command_and_time(msg_array[i][1], msg_array[i][2]);
			break;
		case START_OBSERVE_AND_TIME:
			theapi->queue_observations_and_time(msg_array[i][1], msg_array[i][2]);
		case FIND_CANDIDATES:
			theapi->queue_find_candidates();
			break;
		case ABORT_COMMAND:
			break;
		case REPORT_FULL_STATE:
			theapi->queue_report_full_state();
			break;
		case WAIT:
#ifdef WIN32 
		SleepEx(msg_array[i][1], true);
#else
		sleep(msg_array[i][1]);
#endif
			break;
		}
	}

}

static L2_string_map map;


static L2_file l2;


// generic options
const int OPT_HELP              = 1;
const int OPT_VERBOSE           = 2;
const int OPT_VERSION           = 3;
const int OPT_HISTORY           = 4;
const int OPT_NSTATE_VALUES     = 5;

#ifndef DISABLE_CBFS
// CBFS-only options
const int OPT_CBFS              =10;
const int OPT_CBFS_CANDIDATES   =11;
const int OPT_CBFS_SEARCHSIZE   =12;
#endif

#ifndef DISABLE_COVER
// cover-only options
const int OPT_COVER             =20;
const int OPT_COVER_MAXRANK     =21;
#endif

#ifndef DISABLE_OPTIMIZER
// optimizer options only
const int OPT_OPTIMIZE          =30;
#endif


const Option options[] = {
    { 'h', "help",    false, OPT_HELP,    "print this help message and exit" },
    { 'v', "verbose", false, OPT_VERBOSE, "print gobs of info when parsing"},
    {  0 , "version", false, OPT_VERSION, "print version number and exit" },
    { 'y', "history", true,  OPT_HISTORY, "max history to hold; 0 is infinite"},
    {  0 , "max-truncate", true, OPT_NSTATE_VALUES,
        "max candidates to keep through truncate" },

#ifndef DISABLE_CBFS
    // cbfs-only
    {  0 , "cbfs",    false, OPT_CBFS,    "use CBFS search (default)" },
    {  0 , "cbfs-candidates", true, OPT_CBFS_CANDIDATES,
        "max number of candidates" },
    {  0 , "cbfs-search", true, OPT_CBFS_SEARCHSIZE,
        "max nodes to search" },
#endif

#ifndef DISABLE_COVER
    // cover-only
    {  0 , "cover",   false, OPT_COVER,   "use conflict coverage search" },
    {  0 , "cover-max-rank",true, OPT_COVER_MAXRANK,
       "maximum-rank candidate to consider"},
#endif

#ifndef DISABLE_OPTIMIZER
    // model optimizer only
    { 'O', "optimize", false, OPT_OPTIMIZE, "optimize model"},
#endif

};
unsigned noptions = sizeof(options)/sizeof(Option);



int main(int argc, char **argv) {
    MBA_string system;

    unsigned history = Tracker::DEFAULT_HISTORY_LENGTH;
    unsigned nstates = Tracker::DEFAULT_NUM_HISTORIES;

#ifndef DISABLE_CBFS
    // by default, if cbfs is enabled, use it
    // If cover is disabled, there'll be no way to make this false
    bool use_cbfs_tracker = true;
#else
    // cbfs is disabled; so don't use it.  There's no way to make
    // this true.
    bool use_cbfs_tracker = false;
#endif


#ifndef DISABLE_CBFS
    // cbfs-only settings
    unsigned cbfs_maxcand = 5;
    unsigned cbfs_search  = 1000;
#endif

#ifndef DISABLE_COVER
    // cover-only settings
    unsigned cover_maxrank = 10;
#endif

#ifndef DISABLE_OPTIMIZER
    // optimizer only settings
    bool use_optimizer=false;
#endif


    // skip argv[0] -- the executable name
    OptionFinder of(argv+1, options, noptions);

    int option;
    do {
        option = of.next_option();
        switch(option) {
            case OPT_NOT_AN_OPTION:
                if(system != "") {
                    cerr << "Being told to load more than one file\n"
                            "Previously was loading `" << system <<
                            "', now requested to load `" << of.optarg() << "'\n";
                    exit(1);
                }
                else {
                    system = of.optarg();
                    continue;
                }

            case OPT_ERROR:
                exit(1);

            case OPT_HELP:
                cerr << "Usage: l2test [options] <system>\n"
                        "  Runs livingstone on the system, which must be in\n"
                        "  the l2 format.\n";
                of.printOptions(cerr);
                exit(0);

            case OPT_VERBOSE:
                l2.setVerbose(true);
                continue;

            case OPT_VERSION:
                cout<< "Livingstone version " << l2_version_string << endl;
                exit(0);

            case OPT_HISTORY:
                history = atoi(of.optarg());
                continue;

            case OPT_NSTATE_VALUES:
                nstates = atoi(of.optarg());
                continue;

#ifndef DISABLE_CBFS
            case OPT_CBFS:
                use_cbfs_tracker = true;
                continue;

            case OPT_CBFS_CANDIDATES:
                cbfs_maxcand = atoi(of.optarg());
                continue;

            case OPT_CBFS_SEARCHSIZE:
                cbfs_search = atoi(of.optarg());
                continue;
#endif

#ifndef DISABLE_COVER
            case OPT_COVER:
                use_cbfs_tracker = false;
                continue;

            case OPT_COVER_MAXRANK:
                cover_maxrank = atoi(of.optarg());
                continue;
#endif

#ifndef DISABLE_OPTIMIZER
	case OPT_OPTIMIZE:
	  use_optimizer = true;
	  continue;
#endif



        }
    } while(option != OPT_DONE);
    if(system == "") {
        cerr << "No file specified\n";
        exit(1);
    }

    Tracker *tracker;
    Tracker_debug *debugger;

#ifndef DISABLE_CBFS
    CBFS_tracker cbfst(cbfs_maxcand, cbfs_search, history);
    CBFS_tracker_debug cbfstd(cbfst, map, cout);
#endif

#ifndef DISABLE_COVER
    Cover_tracker ct(static_cast<Assignment::weight_t>(cover_maxrank), 0,
		     history);
    Cover_tracker_debug ctd(ct, map, cout);
#endif

    // TODO: make debuggers later
    if(use_cbfs_tracker)
    {
#ifndef DISABLE_CBFS
        tracker = &cbfst;
        debugger = &cbfstd;
#else
        // cbfs is disabled; how did we get here?
        assert(0);
#endif
    }
    else
    {
#ifndef DISABLE_COVER
        tracker = &ct;
        debugger = &ctd;
#else
        // cover is disabled; how did we get here?
        assert(0);
#endif
    }

    // allocate the readers; must be before listener
    from_xmpl reader(l2);
    to_t_system writer(l2);

    // now we have everything we need to build the listener
    L2_string_map_listener map_listener(map, l2, writer, *tracker);


    if(l2.isVerbose())
        cout << "Reading from " << system << "...\n";

    if(!reader.read(system.c_str()))
        return false;

#ifndef   DISABLE_OPTIMIZER
    if (use_optimizer) {
      if(l2.isVerbose())
	cout << "Optimizing model ...\n";
      l2.optimize(cout);
    }
#endif



    if(!writer.write(&tracker->get_t_system()))
        return false;
    writer.createInitialState(&tracker->get_t_system());

    cout<< "Read from " << system << endl
        << "Search:      " << (use_cbfs_tracker?"CBFS":"cover") << endl
        << "History:     ";
    if(history)
        cout << history << endl;
    else
        cout << "unlimited\n";
    if(use_cbfs_tracker) {
#ifndef DISABLE_CBFS
        cout<< "Candidates:  " << cbfs_maxcand << endl
            << "Search size: " << cbfs_search << endl;
#else
        assert(0);
#endif
    } else {
#ifndef DISABLE_COVER
        cout<< "Max rank:    " << cover_maxrank << endl;
#else
        assert(0);
#endif
    }
    cout << "---------------\n";

    ifstream fin;
    fin.open("scenario.rt");

    // TODO get rid of debugger usage in ReportTransitions
    ReportTransitionsDebug liv_rep(debugger);

    L2_rtapi_debug l2api(tracker,liv_rep);

    unsigned int nmsgs = 0;
    int error = 0;
    do {
        // read in up to 1024 messages
        error = read_scenario(nmsgs, fin);
        if (error != 0)
            return 0;
        // send them to the realtime api
        rt_input(nmsgs, &l2api);
       }
       while (nmsgs == 1024);

    return 0;
}
