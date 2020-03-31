/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_l2.cpp,v 1.28 2004/01/28 23:31:49 lbrown Exp $ */

#include <errno.h> // errno, strerror
#include <readers/from_l2.h>
#include <readers/transition.h>

#define tostream(expr) expr

// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) do { if (isVerbose()) { tostream(expr); } } while(0)
#else
#  define verbose(expr)
#endif

/***************************************************************************
        Store a dictionary.  For now, it's very small and could probably
        just be an if/else if/...  ; but this way I argue it's easier to
        read the code, update, and make sure the abbreviations don't clash
 ***************************************************************************/

/// The sections of a model file in L2 format.

enum Section {
    NO_SECTION,
    CLAUSES,
    COMMANDS,
    ENUMS,
    MODES,
    OBSERVED,
    PROPOSITIONS,
    TRANSITIONS,
    VARIABLES,
    INITIAL,
    BACKGROUND
};


/// A (section name string, enum) pair used for mapping.

struct SectionName {
    const char *name;
    enum Section sect;
};


/// A map from section name strings to Section enum values.

SectionName section_names[] = {
    // abbreviated
    { "b",              BACKGROUND      },
    { "cl",             CLAUSES         },
    { "cm",             COMMANDS        },
    { "e",              ENUMS           },
    { "m",              MODES           },
    { "o",              OBSERVED        },
    { "p",              PROPOSITIONS    },
    { "t",              TRANSITIONS     },
    { "v",              VARIABLES       },
    { "i",              INITIAL         },

#ifdef ENABLE_L2_DEBUG_SECTIONS
    // full name
    { "background",     BACKGROUND      },
    { "clauses",        CLAUSES         },
    { "commands",       COMMANDS        },
    { "enums",          ENUMS           },
    { "modes",          MODES           },
    { "observed",       OBSERVED        },
    { "propositions",   PROPOSITIONS    },
    { "transitions",    TRANSITIONS     },
    { "variables",      VARIABLES       },
    { "initial",        INITIAL         }
#endif
};


/// The number of sections in a model file in L2 format.

const size_t num_sections = sizeof(section_names)/sizeof(SectionName);


/**
 * Map a section name string onto its Section enum.
 * \internal this is not a member function.
 * \param the section name string
 * \return the corresponding Section enum
 */

Section find_section(const char *name) {
    for(size_t i=0; i<num_sections; i++) {
        if(!strcmp(name, section_names[i].name))
            return section_names[i].sect;
    }
    return NO_SECTION;
}



/***************************************************************************
        Destructor.
 ***************************************************************************/

from_l2::~from_l2() {
    if(the_file) fclose(the_file);
}


/***************************************************************************
        Error handling and reporting.
 ***************************************************************************/

#ifdef ENABLE_L2_VERBOSE
void from_l2::print_parse_error() {
    _STD_ cerr <<"Parse error at line " << line_number
	      << ", index " << index << _STD_ endl
	      << line << _STD_ endl;
    for(size_t i=0; i<index; i++) { _STD_ cerr << ' '; }
    _STD_ cerr << "^\n";
}
#else
#  define print_parse_error()
#endif


/// Check that a section was parsed.
/// Argument 1 is typically a call to was_X_parsed().
/// Argument 2 is what we expect (true for prereqs, false
///     for the section we're now parsing).
/// Argument 3 is the name of the section we're checking.

bool from_l2::check_parsed(bool condition, bool expected, const char *msg) {
    if(condition == expected) {
        return true;
    } else {
        print_parse_error();
        if (condition) {
	  tostream(_STD_ cerr << "Section " << msg
		   << " has already been parsed.\n");
        } else {
	  tostream(_STD_ cerr<< "Prerequisite " << msg
		   << " not yet parsed.\n");
	}
        return false;
    }
}

/***************************************************************************
        Lexing functions
 ***************************************************************************/

/**
 * Is string[index] a word separator/terminator?
 * White space and the string terminator are word boundaries.
 * \internal This is not a member function.
 * \param string the string that may contain words
 * \param index the character within string that is being tested
 * \return whether string[index] is a word separator or terminator.
 */

static bool str_at_word_boundary(const char *string, size_t index) {
    switch(string[index]) {
        case ' ':
        case '\n':
        case '\t':
        case '\0':
            return true;
        default:
            return false;
    }
}


/**
 * Copy the next word from string to into and update the index to point to the
 * next boundary
 * \internal this is not a member function.
 * \param into the next word from string is copied into this out parameter
 * \param the current character within string; this out parameter is advanced
 * \param the string that may contain words
 */

static void str_get_next_word(char *into, size_t& index, const char *string) {
    size_t i=0;
    while(!str_at_word_boundary(string, index+i)) {
        into[i] = string[index+i];
        i++;
    }
    into[i] = 0;
    index += i;
}


/// Read a line from the file; leave the results in 'line'

bool from_l2::readline() {
    ++line_number;
    index = 0;
    char *ret = fgets(line, L2_MODEL_FILE_MAX_LINE_SIZE, the_file);
    // check for EOF
    if(!ret) return false;

    // check for overflow; also, get rid of the newline: it's annoying
    size_t len = strlen(line);

    // empty line?  I don't think this should ever actually happen.
    if(len == 0) return true;

    if(line[len-1] != '\n') {
        print_parse_error();
        tostream(_STD_ cerr
		 << "Max line length ("
		 << (L2_MODEL_FILE_MAX_LINE_SIZE - 1)
		 << ") exceeded."
		 << _STD_ endl
		 << "Re-compile with a larger value for "
		 << "L2_MODEL_FILE_MAX_LINE_SIZE"
		 << _STD_ endl);
        return false;
    }

    // kill the \n, thus reducing line length by one
    line[len-1] = 0;
    return true;
}


/// Intuitively: read the next integer.
/// More formally:
/// Read the substring of the line that starts at index
/// and ends at the next whitespace, newline, or the end
/// of the string.  Interpret that substring as a positive
/// integer.  Return the integer and set the index to point
/// to the next whitespace etc.
/// Currently we use base 10; the code is easy to modify to
/// other bases.
/// Argument: the name of the thing we're parsing, used for
/// debugging.
/// Errors: return a negative value (other than L2rTransition::ANY_MODE).
/// This occurs if:
///     the integer overflows
///     there are non-digits in the substring
/// If `acceptStar' is on, return L2rTransiton::ANY_MODE if we read a '*'

int from_l2::read_integer(const char *msg, bool acceptStar) {
    size_t retval = 0;
    unsigned index0 = index;
    // loop ends at line[index] in { ' ', '\n', '\t', '\0' },
    // or a non-digit line[index]; i's easier to write that as a switch
    // statement.
    // The line is guaranteed to end, so the loop is too.
    if(acceptStar && line[index] == '*') {
        index++;
        return L2rTransition::ANY_MODE;
    }
    for(;;++index) {

        if(line[index] >= '0' && line[index] <= '9') {
            retval *= 10;
            retval += line[index]-'0';
            continue;
        }
        // for base > 10, use line[index] >= 'a' && line[index] <= 'f'
        // or whatever; and change the *= above.

        if(str_at_word_boundary(line, index)) {
            if(index0==index) {
                print_parse_error();
                tostream(_STD_ cerr<< "No integer found while reading "
			<< msg << _STD_ endl);
                return NOT_AN_INTEGER;
            }
            return retval;
        } else {
            // printf("Non-digit character `%c' at index %d in:\n"
            //        "%s", ...);
            print_parse_error();
            tostream(_STD_ cerr<< "While reading " << msg << _STD_ endl);
            return NOT_AN_INTEGER;
        }

        // can't get here; we do a continue or return
        assert(0);
    }
    // can't get here; the loop runs until a return
    assert(0);
}


/// Skip the word boundary that index is currently in.
/// This advances index to the next non-(word boundary) character,
/// or returns 'false' if we're at the end of the line.
/// If print_error is true, call print_parse_error if we return
/// false.

bool from_l2::skip_word_boundary(bool print_error) {
    while(str_at_word_boundary(line, index)) {
        if(line[index] == '\0') {
            if(print_error)
                print_parse_error();
            return false;
        }
        index++;
    }
    return true;
}

/***************************************************************************
        The top-level function
 ***************************************************************************/

/// The top-level method.

bool from_l2::read() {
    // make sure read() hasn't already been called
    assert(!the_file);

    // open the file
    the_file = fopen(filename_.c_str(), "r");
    if(!the_file) {
#ifndef _MSC_VER
        tostream(_STD_ cerr<< "from_l2: error opening `" << filename_ << "'\n"
		 << strerror(errno) << _STD_ endl);
#else
        // Under Visual C++ we have not quite figured out the linking
        // to get errno to work.
        tostream(_STD_ cerr<< "from_l2: error opening `" << filename_ << "'\n"
		 << _STD_ endl);
#endif
        return false;
    }

    // first line in the file is a magic cookie to make sure it works.
    readline();
    if(0 != strcmp(line, L2_READER_MAGIC)) {
        tostream(_STD_ cerr<< "from_l2: error opening `" << filename_ << "':\n"
            << "Magic cookie `" << line << "' doesn't match expected " <<
            L2_READER_MAGIC << _STD_ endl);
        return false;
    }


    // the main loop; read a line, parse the section name,
    // pass it to the appropriate function
    while(readline()) {
        bool has_debug_info = false;
        char *linep = line;

        // skip empty lines
        if(line[0]=='\0') continue;

#ifdef ENABLE_L2_DEBUG_SECTIONS
        // it's a section with debug info; note the fact,
        // and search for a section that doesn't start in dbg_
        if(!strncmp(linep, "dbg_", 4)) {
            has_debug_info = true;
            linep+=4;
        }
#endif

        switch(find_section(linep)) {
            case ENUMS:
                if(!parse_enums(has_debug_info)) return false;
                continue;
            case VARIABLES:
                if(!parse_variables(has_debug_info)) return false;
                continue;
            case PROPOSITIONS:
                if(!parse_propositions(has_debug_info)) return false;
                continue;
            case CLAUSES:
                if(!parse_clauses(has_debug_info)) return false;
                continue;
            case TRANSITIONS:
                if(!parse_transitions(has_debug_info)) return false;
                continue;
            case OBSERVED:
                if(!parse_observed(has_debug_info)) return false;
                continue;
            case COMMANDS:
                if(!parse_commands(has_debug_info)) return false;
                continue;
            case MODES:
                if(!parse_modes(has_debug_info)) return false;
                continue;
            case BACKGROUND:
                if(!parse_background(has_debug_info)) return false;
                continue;
            case INITIAL:
                if(!parse_initial(has_debug_info)) return false;
                continue;
            case NO_SECTION:
                print_parse_error();
                tostream(_STD_ cerr << "Unknown section name `" << linep
			 << "'\n");
                return false;
        }
        assert(0); // can't get here!
    }
    return true;
}


/***************************************************************************
        enums section
 ***************************************************************************/

bool from_l2::parse_enums(bool has_debug_info) {
    // check prereqs
    // no prereqs for enums

    // check we haven't already parsed
    if(!check_parsed( was_enums_parsed(), false, "enums"))
        return false;
    didEnums_ = true;

    // first line: count of number of enumerations
    readline();
    int count = read_integer("number of enumerations");
    if(count<0) return false;
    verbose(_STD_ cout << "\n* Reading " << count << " enumerations\n");


    // create the array
    dest()->allocEnums(count, has_debug_info);

    // successive lines:
    // (1) read the line
    // (2) read the number of members
    // (3) if we don't have debug info:
    //        we're done, create the enum and store it in the array
    //     if we do:
    //        a) read the enum name
    //        b) read the members
    //        c) create & store the dbg_enum
    // if the line doesn't parse, stop and return false
    for(size_t enum_index = 0; enum_index < nenums(); enum_index++) {
        if(!readline()) return false;

        // number of members
        int nmembers = read_integer("number of enumeration members");
        if(nmembers<0) return false;

#ifdef ENABLE_L2_DEBUG_SECTIONS
	if(has_debug_info) {
            char buffer[MBA_MAX_LINE_SIZE];
            MBA_string enum_name;
            MBA_string *enum_member_names;
            enum_member_names = new MBA_string [nmembers];

            // read enumeration name
            if(!skip_word_boundary()) {
                tostream(_STD_ cerr << "Missing debug info for enumeration "
			<< enum_index << _STD_ endl);
                return false;
            }
            str_get_next_word(buffer, index, line);
            enum_name = buffer;

            // read the members
            for(int i=0; i<nmembers; i++) {
                if(!skip_word_boundary()) {
                    tostream(_STD_ cerr << "Missing " << (nmembers-i)
			     << " enumeration names\n");
                    return false;
                }
                str_get_next_word(buffer, index, line);
                enum_member_names[i] = buffer;
            }

            dest()->setEnum(enum_index, new dbg_L2rEnumeration(
                    enum_index, nmembers,
                    enum_name, enum_member_names));
            delete [] enum_member_names;
        } else
#endif
	{
	    // no debug info
            dest()->setEnum(enum_index, new L2rEnumeration(enum_index, nmembers));
        }

#ifdef WITH_OSTREAM
        // debug print
        verbose(getEnum(enum_index)->toOStream_long(_STD_ cout));
#endif
    }
    return true;
}



/***************************************************************************
        variables section
 ***************************************************************************/

bool from_l2::parse_variables(bool has_debug_info) {
    // check prereqs
    if(!check_parsed( was_enums_parsed(), true, "enums"))
        return false;

    // check we haven't already parsed
    if(!check_parsed( was_variables_parsed(), false, "variables"))
        return false;
    didVariables_ = true;

    // first line: count of number of variables
    readline();
    int count = read_integer("number of variables");
    if(count<0) return false;
    verbose(_STD_ cout<< "\n* Reading " << count << " variables\n");
    dest()->allocVars(count, has_debug_info);

    // read n variables
    for(size_t var_index = 0; var_index < nvars(); var_index++) {
        if(!readline()) return false;
        int enumID = read_integer("enum ID");
        if(enumID<0) return false;

        const L2rEnumeration *the_enum = getEnum(enumID);
        if(!the_enum) return false;

        L2rVariable *retval;

#ifdef ENABLE_L2_DEBUG_SECTIONS
        if(has_debug_info) {
            // read the fully-qualified variable name
            char varname[MBA_MAX_LINE_SIZE];
            skip_word_boundary();
            str_get_next_word(varname, index, line);

            retval = new dbg_L2rVariable(var_index, the_enum, varname);
	} else
#endif
	{
	    // we're done; store in the array
	    retval = new L2rVariable(var_index, the_enum);
	}
        dest()->setVar(var_index, retval);

#ifdef WITH_OSTREAM
        // debug output
        verbose(retval->toOStream_long(_STD_ cout));
#endif
    }
    return true;
}

/***************************************************************************
        propositions section
 ***************************************************************************/

bool from_l2::parse_one_prop(unsigned prop_index,
        bool equality, bool positive) {
    int varID, valOrVarID;
    if(!readline()) return false;

    if(!skip_word_boundary()) return false;
    varID = read_integer("variable ID");
    if(varID < 0) return false;

    if(!skip_word_boundary()) return false;
    valOrVarID = read_integer("value");
    if(valOrVarID < 0) return false;

    // check the variable ID is valid
    const L2rVariable *the_var = getVar(varID);
    if(!the_var) return false;

    L2rProposition *the_prop;

    if(!equality) {
        // check that the memberID is a valid ID for the variable type.
        const L2rEnumeration *the_enum = the_var->type();
        if(!the_enum->isValidMember(valOrVarID)) return false;

        // we're safe; create & store
        the_prop =new L2rPropVarValue(prop_index,
                the_var, positive, valOrVarID);
    } else {
        // check the varID is valid
        const L2rVariable *other_var = getVar(valOrVarID);
        if(!other_var) return false;
        if(the_var->type() != other_var->type()) {
            tostream(_STD_ cerr <<"Type mismatch (`"
                    << the_var->type() << "' and `" << other_var->type()
                    << "') in `" << the_var << (positive?"=":"!=") << other_var
                    << "'\n");
            return false;
        }
        the_prop = new L2rPropVarVar(prop_index,
                the_var, positive, other_var);
    }
    dest()->setProp(prop_index, the_prop);
    verbose(the_prop->toOStream_long(_STD_ cout));
    return true;
}


bool from_l2::parse_propositions(bool /*has_debug_info*/) {
    // check prereqs
    if(!check_parsed( was_enums_parsed(), true, "enums"))
        return false;
    if(!check_parsed( was_variables_parsed(), true, "variables"))
        return false;

    // check we haven't already parsed
    if(!check_parsed( was_propositions_parsed(), false, "propositions"))
        return false;
    didPropositions_ = true;


    // first line: count of number of positive props
    readline();
    // counts:
    int pos_count, neg_count, same_count, diff_count;

    pos_count = read_integer("number of positive propositions");
    if(pos_count<0) return false;
    if(!skip_word_boundary()) return false;

    neg_count = read_integer("number of negative propositions");
    if(neg_count<0) return false;
    if(!skip_word_boundary()) return false;

    same_count = read_integer("number of equalities");
    if(same_count<0) return false;
    if(!skip_word_boundary()) return false;

    diff_count = read_integer("number of inequalities");
    if(diff_count<0) return false;
    // nothing more to read; don't s_w_b

    // allocate all the propositions
    dest()->allocProps(pos_count + neg_count + same_count + diff_count);
    verbose(_STD_ cout
            << "\n* Reading " << nprops() << " propositions ("
            << pos_count  << " positive, "
            << neg_count  << " negative,\n          "
            << same_count << " equalities, "
            << diff_count << " inequalities)\n");

    unsigned prop_id=0;
    int i; // signed b/c the counts are

    for(i=0 ; i < pos_count; ++i, ++prop_id)
        // not an equality; positive
        if(!parse_one_prop(prop_id, false, true)) return false;

    for(i=0 ; i < neg_count; ++i, ++prop_id)
        // not an equality; negative
        if(!parse_one_prop(prop_id, false, false)) return false;

    for(i=0 ; i < same_count; ++i, ++prop_id)
        // equality; positive
        if(!parse_one_prop(prop_id, true, true)) return false;

    for(i=0 ; i < diff_count; ++i, ++prop_id)
        // equality; negative
        if(!parse_one_prop(prop_id, true, false)) return false;

    return true;
}

/***************************************************************************
        clauses section
 ***************************************************************************/

bool from_l2::parse_clauses(bool has_debug_info) {
    // check prereqs
    if(!check_parsed( was_propositions_parsed(), true, "propositions"))
        return false;

    // check we haven't already parsed
    if(!check_parsed( was_clauses_parsed(), false, "clauses"))
        return false;
    didClauses_ = true;

    // first line: count of number of clauses
    readline();
    int count = read_integer("number of clauses");
    if(count<0) return false;
    verbose(_STD_ cout<< "\n* Reading " << count << " clauses\n");

    dest()->allocClauses(count, has_debug_info);

    // read n clauses
    for(size_t clause_index = 0; clause_index < nclauses(); clause_index++) {
        if(!readline()) return false;

#ifdef ENABLE_L2_DEBUG_SECTIONS
        // if we have debug info, read the name first
        char component[MBA_MAX_LINE_SIZE];
        if(has_debug_info) {
            str_get_next_word(component, index, line);
            if(!skip_word_boundary()) return false;
        }
        unsigned first_prop_index = index;
#else
        unsigned first_prop_index = 0;
#endif

        // two passes: first:  count and allocate an array big enough;
        //             second: fill the array

        // first, count the number of props; note: not an error
        // to hit end-of-line -- in fact, we expect to!
        unsigned nclause_props = 0;
        while(skip_word_boundary(false)) {
            if(0>read_integer("prop ID")) return false;
            nclause_props++;
        }

        // go to the beginning of the line (except for the component name)
        index = first_prop_index;

        const L2rProposition **clause_props =
            L2_alloc_array(const L2rProposition* , nclause_props);

        for(unsigned i=0; i<nclause_props; i++) {
            // now it _is_ an error, since we have counted already
            if(!skip_word_boundary(true)) return false;
            int propID = read_integer("prop ID");
            if(propID<0) return false;
            const L2rProposition *the_prop = getProp(propID);
            if(!the_prop) return false;
            clause_props[i] = the_prop;
        }

        // create the clause object; no need to copy the props
        L2rClause *cls;
#ifdef ENABLE_L2_DEBUG_SECTIONS
        if(has_debug_info) {
            cls = new dbg_L2rClause(component, clause_index,
                nclause_props, clause_props, false);
        } else
#endif
        {
            cls = new L2rClause(clause_index,
                nclause_props, clause_props, false);
        }
        dest()->setClause(clause_index,cls);
        verbose(cls->toOStream_long(_STD_ cout));
    }
    return true;
}

/***************************************************************************
        transition sections (nominal, failure)
 ***************************************************************************/

bool from_l2::parse_one_transition(bool has_debug_info, bool isNominal) {
    if(!readline()) return false;

#ifdef ENABLE_L2_DEBUG_SECTIONS
    char dbg_name[MBA_MAX_LINE_SIZE];
    if(has_debug_info) {
        str_get_next_word(dbg_name, index, line);
        if(!skip_word_boundary()) return false;
    }
#endif

    int varID = read_integer("mode variable");
    if(varID<0) return false;

    if(!skip_word_boundary()) return false;
    int from = read_integer("from mode", true);
    // L2rTransition::ANY_MODE is negative but ok
    if (from < 0 && from != L2rTransition::ANY_MODE) return false;

    if(!skip_word_boundary()) return false;
    int to = read_integer("to mode");
    if(to<0) return false;

    int rank;
    if(isNominal) rank=0; // kill a potential 'uninitialized' warning
    else {
        if(!skip_word_boundary()) return false;
        rank = read_integer("probability");
        if(rank<0) return false;
    }

    // now, error-check all those
    const L2rVariable *mode = getVar(varID);
    if(!mode) return false;
    switch(mode->kind()) {
        case vk_unknown:
            dest()->getVar_friend(varID)->setKind(vk_mode);
            break;
        case vk_mode:
            break;
        default:
            tostream(_STD_ cerr<<"Transition on " << mode->kind()
		     << " variable " << mode << _STD_ endl);
            return false;
    }

    if(from != L2rTransition::ANY_MODE && !mode->type()->isValidMember(from)) {
      return false;
    }
    if(!mode->type()->isValidMember(to))   return false;

    // ok; read the clauses: two scans -- first count,
    // next store
    unsigned first_clause = index;
    unsigned x_nclauses = 0;
    while(skip_word_boundary(false)) {
        int clsID = read_integer("prerequisite clause");
        if(clsID<0) return false;
        x_nclauses++;
    }

    // second scan; store
    index = first_clause;
    const L2rClause ** x_clauses;
    if(x_nclauses == 0) {
        x_clauses = 0;
    } else {
        x_clauses = L2_alloc_array(const L2rClause* , x_nclauses);
        for(unsigned i=0; i<x_nclauses; i++) {
            if(!skip_word_boundary(true)) return false;
            int clsID = read_integer("prerequisite clause");
            if(clsID<0) return false;
            const L2rClause *the_clause = getClause(clsID);
            if(!the_clause) return false;
            x_clauses[i] = getClause(clsID);
        }
    }

    L2rTransition *the_x;

#ifdef ENABLE_L2_DEBUG_SECTIONS
    if(has_debug_info) {
        the_x = new dbg_L2rTransition(dbg_name,
            mode, from, to,
            x_nclauses, x_clauses, false, // don't copy
            isNominal, rank);
    } else
#endif
    {
        the_x = new L2rTransition(
            mode, from, to,
            x_nclauses, x_clauses, false, // don't copy
            isNominal, rank);
    }
    dest()->addTransition(the_x);

#ifdef WITH_OSTREAM
    // debug out
    verbose(the_x->toOStream_long(_STD_ cout));
#endif

    return true;
}


bool from_l2::parse_transitions(bool has_debug_info) {
    // check prereqs
    if(!check_parsed( was_enums_parsed(), true, "enums"))
        return false;
    if(!check_parsed( was_variables_parsed(), true, "variables"))
        return false;
    if(!check_parsed( was_clauses_parsed(), true, "clauses"))
        return false;

    // check we haven't already parsed
    if(!check_parsed( was_transitions_parsed(), false, "transitions"))
        return false;
    didTransitions_ = true;


    // first line: count of number of nominal transitions
    readline();
    int nominal = read_integer("number of nominal transitions");
    if(nominal<0) return false;

    // second line: failures
    readline();
    int failure = read_integer("number of failure transitions");
    if(failure<0) return false;

    unsigned ntransitions = nominal+failure;

    verbose(_STD_ cout<< "\n* Reading " << ntransitions << " transitions ("
	    << nominal << " nominal, " << failure << " failure)\n");

    // read n transitions
    unsigned x_index = 0;
    for( ; x_index < (unsigned)nominal; x_index++)
        if(!parse_one_transition(has_debug_info, true))
            return false;
    for( ; x_index < ntransitions; x_index++)
        if(!parse_one_transition(has_debug_info, false))
            return false;
    return true;
}


/***************************************************************************
        observed, commands, modes sections
        The three are very similar.
 ***************************************************************************/

bool from_l2::parse_var_modifier(VarKind kind) {
    // check prereqs
    if(!check_parsed( was_variables_parsed(), true, "variables"))
        return false;

    // initialize to squash warnings
    const char *kindname=0;
    const char *sectionname=0;
    bool wasParsed=false;
    switch(kind) {
        case vk_observed:
            kindname = "observed";
            sectionname = "observed";
            wasParsed = was_observed_parsed();
            didObserved_ = true;
            break;

        case vk_commanded:
            kindname = "commanded";
            sectionname = "commands";
            wasParsed = was_commands_parsed();
            didCommands_ = true;
            break;

        case vk_mode:
            kindname = "mode";
            sectionname = "modes";
            wasParsed = was_modes_parsed();
            didModes_ = true;
            break;

        default:
            print_parse_error();
            tostream(_STD_ cerr<<"Unhandled kind " << kind << _STD_ endl);
            return false;
    }

    if(!check_parsed(wasParsed, false, sectionname)) return false;

    verbose(_STD_ cout<<"\n* Setting `" << kindname << "' flag\n");

    if(!readline()) {
        print_parse_error();
        tostream(_STD_ cerr<<"Reached EOF in "<<sectionname<<" section\n");
        return false;
    }

    // read to the end of the line, but don't complain about it
    while(skip_word_boundary(false)) {
        int varID = read_integer("var ID");
        if(varID<0) return false;

        if(!getVar(varID)) return false;
        VarKind oldkind = getVar(varID)->kind();
        if(oldkind != vk_unknown && oldkind != kind) {
            tostream(_STD_ cerr << "Changing kind of "<<getVar(varID)<< " from "
                    << oldkind << " to " << kind << _STD_ endl);
            return false;
        }
        // can't use getVar since it returns const
        dest()->getVar_friend(varID)->setKind(kind);
        verbose(_STD_ cout << "    " << getVar(varID) << _STD_ endl);
    }
    return true;
}

bool from_l2::parse_observed(bool /*has_debug_info*/) {
    return parse_var_modifier(vk_observed);
}

bool from_l2::parse_commands(bool /*has_debug_info*/) {
    return parse_var_modifier(vk_commanded);
}

bool from_l2::parse_modes(bool /*has_debug_info*/) {
    return parse_var_modifier(vk_mode);
}


/***************************************************************************
        background section
 ***************************************************************************/

bool from_l2::parse_background(bool) {
    // prereqs
    if(!check_parsed( was_clauses_parsed(), true, "clauses"))
        return false;

    // don't parse twice
    if(!check_parsed( was_background_parsed(), false, "background"))
        return false;

    // first line: a count
    if(!readline()) {
        tostream(_STD_ cerr << "Expected a count\n");
        return false;
    }

    int count = read_integer("number of variables");
    if(count<0) return false;
    unsigned nbackground = count;
    verbose(_STD_ cout <<"\n* Reading "<< nbackground
	    << " background clauses\n");

    for(unsigned i=0 ; i < nbackground; i++) {
        if(!readline()) {
            print_parse_error();
            tostream(_STD_ cerr<<"Missing " << (nbackground-i) << " clauses\n");
            return false;
        }
        int clauseID = read_integer("clause ID");
        if(clauseID < 0) return false;
        const L2rClause *cls = getClause(clauseID);
        if(!cls) return false;
        dest()->addBackground(cls);
        verbose(_STD_ cout<< "    " << cls << _STD_ endl);
    }

    return true;
}

/***************************************************************************
        initial section
 ***************************************************************************/

bool from_l2::parse_initial(bool) {
    // prereqs
    if(!check_parsed( was_enums_parsed(), true, "enums"))
        return false;
    if(!check_parsed( was_variables_parsed(), true, "variables"))
        return false;

    // don't parse twice
    if(!check_parsed( was_initial_parsed(), false, "initial"))
        return false;

    // get count
    if(!readline()) {
        tostream(_STD_ cerr << "Expected a count\n");
        return false;
    }

    int count = read_integer("number of initial mode assignments");
    verbose(_STD_ cout<<"\n* Reading "<< count << " initial modes\n");

    for(int i=0; i<count; i++) {
        if(!readline()) {
            tostream(_STD_ cerr << "Expected " << (count-i) << " more lines\n");
            return false;
        }
        int varID = read_integer("var ID");
        if(varID<0) return false;
        if(!skip_word_boundary()) {
            tostream(_STD_ cerr << "Expected memberID\n");
            return false;
        }
        int memberID = read_integer("member ID");
        if(memberID<0) return false;

        L2rVariable *var = dest()->getVar_friend(varID);
        if(!var) return false;
        if(!var->type()->isValidMember(memberID)) return false;
        var->setInitial(memberID);
    }
    return true;
}


/***************************************************************************
        debugging mainline
 ***************************************************************************/

/**
 * An entry point for stand-alone testing.
 * \param argc must equal 2 (one command-line argument)
 * \param argv argv[1] is the L2 model file pathname
 * \return 0 if successfully read; otherwise 1
 */

#ifdef PARSER_MAIN
int main(int argc, char **argv) {
    from_l2 reader;

    if(argc!=2) {
        tostream(_STD_ cerr<< "USAGE: parser <file>\n"
            << "The 'file' argument includes the extension (.l2)\n");
        return 2;
    }
    if(reader.read(argv[1]))
        return 0;
    else
        return 1;
}
#endif
