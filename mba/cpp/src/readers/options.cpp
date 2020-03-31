/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: options.cpp,v 1.9 2004/01/28 23:31:50 lbrown Exp $ */

#include <string.h>
#include <readers/options.h>

OptionFinder::OptionFinder(const char *const* a, const Option* o, unsigned n) {
    argv = a;
    options = o;
    noptions = n;
}


void OptionFinder::printOptions(_STD_ ostream& os) {
    os << "Options:\n";
    for(unsigned i=0; i<noptions; i++) {
        os << "    ";
        if(options[i].shortname)
            os << '-' << options[i].shortname << ' ';
        else
            os << "   ";
        if(options[i].shortname && options[i].longname)
            os << '|';
        else
            os << ' ';
        if(options[i].longname) {
            os << " --" << options[i].longname;
            if(options[i].has_argument)
                os << "=n";
            os << '\t';
        }
        else {
            os << "\t\t";
        }
        os << ' ' << options[i].documentation << _STD_ endl;
    }
}

void OptionFinder::printOptions() {
    printOptions(_STD_ cout);
}


// used in next_option; search for a -- option
const Option *OptionFinder::find_long_option(const char *name) {
    unsigned len;
    const char *eq = strchr(name, '=');
    if(!eq) len=strlen(name);
    else    len = (unsigned)eq - (unsigned)name;

    for(unsigned i=0; i<noptions; i++) {
        if(!strncmp(options[i].longname, name, len)) {
            return options+i;
        }
    }
    return 0;
}

// used in next_option; search for a - option
const Option *OptionFinder::find_short_option(char name) {
    for(unsigned i=0; i<noptions; i++) {
        if(options[i].shortname == name)
            return options+i;
    }
    return 0;
}


// Return the option for argv[0] and set option_argument
int OptionFinder::next_option() {
    const char *arg = argv[0];
    if(!arg) return OPT_DONE;
    argv++;
    if(arg[0] != '-') {
        option_argument = arg;
        return OPT_NOT_AN_OPTION;
    }

    bool is_long = (arg[1]=='-');
    const Option *the_opt;
    if(arg[1] == '-')
        // skip the --
        the_opt = find_long_option(arg+2);
    else
        the_opt = find_short_option(arg[1]);

    if(!the_opt) {
        _STD_ cerr<< "Unrecognized option " << arg << _STD_ endl;
        return OPT_ERROR;
    }

    if(the_opt->has_argument) {
        if(is_long) {
            option_argument = strchr(arg, '=');
            if(option_argument) option_argument++; // skip '='
        } else {
            // accept either "-ofoo" or "-o" "foo"
            if(!arg[2]) { // -o was everything
                // -o foo
                option_argument = argv[0]; // may be 0, triggers error
                argv++;
            } else {
                // -ofoo
                option_argument = arg+2;
            }
        }
        if(!option_argument) {
            _STD_ cerr << "Option " << arg << " requires an argument\n";
            return OPT_ERROR;
        }
    }
    return the_opt->id;
}
