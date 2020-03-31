/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: names2ids.cpp,v 1.8 2004/01/28 23:31:53 lbrown Exp $ */

#include <ctype.h> // isspace
#include <debuggers/livingstone_debug.h>
#include <debuggers/tracker_debug.h>
#include <livingstone/L2_fstream.h> // _STD_ ofstream

static unsigned linenum = 0;

static void error(const char *caller) {
    _STD_ cerr << "Error in `" << caller << "' while parsing line " << linenum
	      << _STD_ endl;
    exit(1);
}

static void split(const char *line, MBA_string *values, unsigned nvalues) {
    // skip to next ws
    while(*line && !isspace(*line)) ++line;
    if(!*line) error("splitting");

    // foreach value, skip ws, read a string
    for(unsigned i=0; i<nvalues; ++i) {
        while(*line && isspace(*line)) ++line;
        if(!*line) error("splitting");

        const char *val_start = line;
        unsigned nchars=0;
        while(*line && !isspace(*line)) {
            ++nchars;
            ++line;
        }
        values[i] = MBA_string(val_start, nchars);
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
        _STD_ cerr <<
            "USAGE: names2ids system\n"
            "Translates a script in the syntax of l2flight scripts, but\n"
            "with names, into an ID-based one palatable to l2flight.\n"
            "Reads from stdin, writes to stdout.\n";
        return 1;
    }

    Livingstone_debug l2;
    l2.set_filename(argv[1]);
    if(!l2.read_file()) {
        _STD_ cerr << "Error reading `" << argv[1] << "'\n";
        return 1;
    }

    l2.create_tracker();
    l2.initialize_tracker();
    l2.create_debugger();
    Tracker_debug *dbg = l2.get_debugger();


    char buffer[256];
    MBA_string values[2];
    while(1) {
        _STD_ cin.getline(buffer, 256);
        ++linenum;
        if(_STD_ cin.eof()) break;
        if(_STD_ cin.bad()) error("reading from input");
        switch(buffer[0]) {
            case 'o':
                // observe
                split(buffer, values, 2);
                {
                    Variable *var;
                    int value;
                    var = dbg->find_current_observable(values[0]);
                    value = (values[1]=="unknown" ? -1 :
                            dbg->find_prop_index(*var, values[1], true));
                    _STD_ cout << "o " << var->get_id() << ' ' << value
			      << _STD_ endl;
                }
                break;

            case 'c':
                // command
                split(buffer, values, 2);
                {
                    Variable *var;
                    int value;
                    var = dbg->find_current_command(values[0]);
                    value = dbg->find_prop_index(*var, values[1], true);
                    _STD_ cout << "c " << var->get_id() << ' ' << value
			      << _STD_ endl;
                }
                break;

            case 'I':
            case 'd':
            case 'i':
            case 'r':
            case 'q':
                _STD_ cout << buffer << '\n';
                break;

            case 'v':
                split(buffer, values, 1);
                {
                    Variable *var = dbg->find_variable(values[0], true);
                    _STD_ cout << "v " << var->get_id() << _STD_ endl;
                }
                break;

        } // end switch(buffer[0])
    }  // end read loop

    return 0;
}
