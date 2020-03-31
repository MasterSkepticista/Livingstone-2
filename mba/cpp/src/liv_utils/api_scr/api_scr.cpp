/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//str_convert.cpp -- main for program which takes string sequences as input and converts
// to appropriate enums for realtime api testing.
#include <stdio.h>
#include <stdlib.h>
#include <debuggers/livingstone_debug.h>
#include <debuggers/tracker_debug.h>
#include <livingstone/L2_assert.h>
#include <livingstone/L2_fstream.h>
#include <livingstone/version.h>
#include <realtime_api/livingstonemessage.h>
#include <transition/T_system.h>

// file where output is sent
FILE *fp = fopen("scenario.rt", "w");

// input stream
_STD_ ifstream fin;

const unsigned int NUM_MESSAGE_TYPES = 8; // read from livingstonemessage.h
const char *fcns[NUM_MESSAGE_TYPES]= {
    "cmd",
    "obs",
    "cmdt",
    "obst",
    "fc",
    "abort",
    "report",
    "wait"
};

//enum message_types { COMMAND, COMMAND_AND_TIME, OBSERVE, OBSERVE_AND_TIME, FIND_CANDIDATES,
//		WAIT, REPORT };
const unsigned int WAIT = REPORT_FULL_STATE+1;

void out_to_file(unsigned int FCN, unsigned int arg1=0, unsigned int arg2=0)
{
	// want formatted column output
//	fout << FCN << index << val_index << time_delay << "\n";
	switch (FCN) {
	case START_COMMAND_AND_TIME:
	case START_OBSERVE_AND_TIME:
		fprintf(fp, "%-10d %-10d %-10d \n", FCN, arg1, arg2);
		break;
	case COMMAND:
	case OBSERVATION:
		fprintf(fp, "%-10d %-10d %-10d \n", FCN, arg1, arg2);
		break;
	case FIND_CANDIDATES:
	case REPORT_FULL_STATE:
		fprintf(fp, "%-10d \n", FCN);
		break;
	case WAIT:
		fprintf(fp, "%-10d %-10d \n", FCN, arg1);
		break;
	}
}

void map_indices(Tracker_debug &tracker_debugger, _STD_ ifstream &fin,
		 unsigned int & var_index, unsigned int & val_index)
{
    Assignable *var;
    char str_input[80];
    fin >> str_input;

    // convert first string argument to command enum, second to value.
    // do this by getting the variable pointer and id
    var = tracker_debugger.find_assignable(str_input);
    var_index = var->get_id();

    // now index is equal to command enum, what about value? get next string
    fin >> str_input;
    val_index = tracker_debugger.find_assignment(*var,str_input)->get_index();
}

int main(int argc,char** argv) {
    if (argc < 3)  {
        _STD_ cerr<< "Usage:\n"
            << "       " << argv[0] << " <model-name>" << " <script-file>\n";
        return(-1);
    }


    Livingstone_debug pack;
    pack.set_filename(argv[1]);
    pack.read_file();
    pack.create_tracker();
    pack.create_debugger();
    pack.initialize_tracker();

    Tracker_debug& tracker_debugger = *pack.get_debugger();

    /////////////////////////////////////////////////////////

    // read in strings

    char str_input[80];
    fin.open(argv[2]);
    fin >> str_input;

    unsigned int message_type=0;
    unsigned int linenum = 0, var_index=0, val_index=0, time_delay = 0;

    while ( strcmp(str_input,"end") != 0 ) {
        if (fin.eof()) return 0;

        for (unsigned i=0; i<NUM_MESSAGE_TYPES; i++) {
            if ( strcmp( str_input, fcns[i]) == 0 ) {
                message_type = i;
                continue;
            }
        }

        switch (message_type) {
            case COMMAND:
                map_indices(tracker_debugger, fin, var_index, val_index);
                out_to_file(COMMAND, var_index, val_index);
                break;

            case START_COMMAND_AND_TIME:
                map_indices(tracker_debugger, fin, var_index, val_index);
                out_to_file(START_COMMAND_AND_TIME, var_index, val_index);
                break;

            case OBSERVATION:
                map_indices(tracker_debugger, fin, var_index, val_index);
                out_to_file(OBSERVATION, var_index, val_index);
                break;

            case START_OBSERVE_AND_TIME:
                map_indices(tracker_debugger, fin, var_index, val_index);
                out_to_file(START_OBSERVE_AND_TIME, var_index, val_index);
                break;

            case FIND_CANDIDATES:
                out_to_file(FIND_CANDIDATES);
                break;

            case WAIT:
                fin >> time_delay;
                out_to_file(WAIT, time_delay);
                break;

            case REPORT_FULL_STATE:
                out_to_file(REPORT_FULL_STATE);
                break;

            default:
                _STD_ cout << "Invalid message at line " << linenum << "\n";
                return 1;
        }
        linenum++;
        // here execute out to file for all with phony args, if necessary.

        fin >> str_input;
    }

    return 0;
}
