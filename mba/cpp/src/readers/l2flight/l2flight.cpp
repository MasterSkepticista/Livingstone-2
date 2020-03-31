/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: l2flight.cpp,v 1.15 2004/01/29 19:17:10 lbrown Exp $ */

#include <api/livingstone.h>
#include <ctype.h> // isspace

// shouldn't need this
#include <transition/tracker.h>

static const int MAX_VALUES = 2;
bool split(const char *line, int *values, unsigned nvalues) {
    // skip to next ws
    while(*line && !isspace(*line)) ++line;

    // foreach value, skip ws, read an int (don't check it's an int)
    for(unsigned i=0; i<nvalues; ++i) {
        while(*line && isspace(*line)) ++line;
        if(!*line) {
            _STD_ cerr << (nvalues-i) << " argument(s) missing\n";
            return false;
        }
        int val=0;
        int sign;
        switch(line[0]) {
            case '-':
                sign=-1;
                ++line;
                break;
            case '+':
                sign=1;
                ++line;
                break;
            default:
                sign=1;
        }

        while(*line && !isspace(*line)) {
            val *= 10;
            val += line[0]-'0';
            ++line;
        }
        values[i] = sign * val;
    }

    return true;
}

/** Help; print command list */

void doHelp() {
  _STD_ cout << "c var val Command (& progress) var <- val\n"
	    << "d         Diagnose (find candidates)\n"
	    << "F         Failure info\n"
	    << "f         Failure count\n"
	    << "h         Write this help message\n"
	    << "I         Idle (progress w/o command)\n"
	    << "i cand    Install cand\n"
	    << "o var val Observe (assign) var <- val\n"
	    << "q         Quit\n"
	    << "r         Reset\n"
	    << "t         Termination-of-search reason\n"
	    << "v var ts  Value of var at time step ts\n";
}

/** Reason for termination of most recent search */

void doTermination(Livingstone& l2) {
  _STD_ cout << l2.get_search_termination_reason() << _STD_ endl;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        _STD_ cerr <<
            "USAGE: l2flight system.l2bin\n"
            "Runs a very slim interface over the binary-format system.\n"
            "The parameters (search method etc) are to be specified by\n"
            "system.l2bin.params\n";
        exit(1);
    }

    _STD_ cin.clear();
    bool exit_exec_loop = true;
    do {
        // Reset the static variables before using in case the last run crashed
        reset_memory_flag();
        Livingstone l2;
        l2.set_filename(argv[1]);
        if(!l2.read_file()) return 1;
        l2.create_tracker();
        l2.initialize_tracker();

        char buffer[256];
        int values[2];
        bool exit_read_loop = false;
        while(!exit_read_loop) {
            _STD_ cout << "l2flight> "<< _STD_ flush;
            _STD_ cin.getline(buffer, 256);
            if (_STD_ cin.eof() || _STD_ cin.bad()) {
                exit_exec_loop = true;
                break;
            }
            switch(buffer[0]) {
	        case 'h':
		    doHelp();
		    break;

                case 'o':
                    // observe
                    if(split(buffer, values, 2)) {
                        _STD_ cout << "Observing v" << values[0] << " = "
				  << values[1] << _STD_ endl;
                        if(!l2.observe(values[0], values[1]))
                            _STD_ cerr << "ignored: Illegal arguments.\n";
                    }
                    break;

                case 'c':
                    // command
                    if(split(buffer, values, 2)) {
                        _STD_ cout << "Commanded v" << values[0] << " = "
				  << values[1]<<_STD_ endl;
                        if(!l2.command(values[0], values[1]))
                            _STD_ cerr << "ignored: Illegal arguments.\n";
                    }
                    break;

                case 'I':
                    // idle
                    l2.idle();
                    break;

                case 'd':
                    // diagnose
                    {
                        unsigned ndiag = l2.diagnose();
                        _STD_ cout << "Found " << ndiag << " diagnoses\n";
                    }
                    break;

                case 'i':
                    // install
                    if(split(buffer, values, 1)) {
                        _STD_ cout << "Installing candidate " << values[0]
				  << _STD_ endl;
                        if(!l2.install(values[0]))
                            _STD_ cerr << "ignored: Illegal arguments.\n";
                    }
                    break;

                case 'v':
                    // get_value
                    if(!l2.have_installed()) {
                        _STD_ cerr << "Install a candidate first\n";
                        break;
                    }
                    if(split(buffer, values, 2)) {
                        unsigned id = unsigned(values[0]);
                        unsigned t  = unsigned(values[1]);
			MBA_string tstr = (t==(unsigned)-1)
			    ? "now" : MBA_string(t);
                        bool ok;
                        int val = l2.get_value(id, t, ok);
                        if(ok) {
                            _STD_ cout << 'v' << id << '[' << tstr << "] = ";
                            if(val<0) _STD_ cout << "unknown\n";
                            else      _STD_ cout << val << _STD_ endl;
                        }
                        else {
                            _STD_ cout << "Getting v" << id << '[' << tstr
				      << "]\n";
                            _STD_ cerr << "ignored: Illegal arguments.\n";
                        }
                    }
                    break;


                case 'f':
                    // nfailures
                    if(!l2.have_installed()) {
                        _STD_ cerr << "Install a candidate first\n";
                        break;
                    }
                    _STD_ cout << l2.nfailures() << " failures\n";
                    break;

                case 'F':
                    // get_failure
                    if(!l2.have_installed()) {
                        _STD_ cerr << "Install a candidate first\n";
                        break;
                    }
                    if(split(buffer, values, 1)) {
                        Livingstone::failure f = l2.get_failure(values[0]);
                        if(!f.is_valid()) {
                            _STD_ cerr << "ignored: Illegal arguments.\n";
                            break;
                        }
                        _STD_ cout<< 'v' << f.get_id()
                            << " had failure " << f.get_value();
                        if (f.get_time_step() < 0) {
                            _STD_ cout << " before the horizon.\n";
                        } else {
                            _STD_ cout << " at time " << f.get_time_step()
				      << '\n';
			}
                    }
                    break;

	        case 't':
		    doTermination(l2);
		    break;

                case 'r':
                    // reset/restart
                    exit_read_loop = true;
                    exit_exec_loop = false;
                    break;

                case 'q':
                    // quit
                    exit_read_loop = true;
                    exit_exec_loop = true;
                    break;

            } // end switch(buffer[0])
        }  // end read loop
    } while(!exit_exec_loop);

    return 0;
}
