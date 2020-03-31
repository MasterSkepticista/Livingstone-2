/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_l2.cpp,v 1.19 2004/01/28 23:31:50 lbrown Exp $ */

#include <errno.h>
#include <livingstone/L2_fstream.h>
#include <readers/L2_file.h>
#include <readers/to_l2.h>
#include <readers/transition.h>
#include <readers/xlate_sort_props.h>

bool to_l2::write() {
    _STD_ ofstream l2file(filename_.c_str());
    if(!l2file) {
        _STD_ cerr << "Error opening `" << filename_ << "' for write: "
		  << strerror(errno) << _STD_ endl;
        return false;
    }

    unsigned i;

    // magic cookie at bof
    l2file << L2_READER_MAGIC << _STD_ endl;

    print_enums(l2file);
    if(include_debug) l2file << _STD_ endl;

    print_variables(l2file);
    if(include_debug) l2file << _STD_ endl;

    print_propositions(l2file);
    if(include_debug) l2file << _STD_ endl;

    print_clauses(l2file);
    if(include_debug) l2file << _STD_ endl;

    print_background(l2file);
    if(include_debug) l2file << _STD_ endl;

    print_transitions(l2file);
    if(include_debug) l2file << _STD_ endl;

    // the var modifiers sections; first count how much of each
    unsigned nmodes=0, ncmds=0, nobs=0;
    for(i=0; i < get_source()->nvars(); i++) {
        switch(get_source()->getVar(i)->kind()) {
            case vk_commanded:
                ncmds++; break;
            case vk_mode:
                nmodes++; break;
            case vk_observed:
                nobs++; break;
            default:
                break;
        }
    }
    // now print out the sections
    if(nmodes>0) {
        print_kind(l2file, nmodes, vk_mode);
        if(include_debug) l2file << _STD_ endl;
    }
    if(ncmds>0) {
        print_kind(l2file, ncmds,  vk_commanded);
        if(include_debug) l2file << _STD_ endl;
    }
    if(nobs>0) {
        print_kind(l2file, nobs,   vk_observed);
        if(include_debug) l2file << _STD_ endl;
    }

    // grand finale: print out the intial mode assignments
    print_initial(l2file);

    return true;
}


/***************************************************************************
  Print all domains.
 ***************************************************************************/
void to_l2::print_enums(_STD_ ostream& os) {
    if(include_debug) {
        if(get_source()->dbg_enums())
            os << "dbg_enums\n";
        else
            os << "enums\n";
    } else
        os << "e\n";
    os << get_source()->nenums() << _STD_ endl;
    for(unsigned i=0; i < get_source()->nenums(); i++)
        print_one_enum(os, get_source()->getEnum(i));
}

void to_l2::print_one_enum(_STD_ ostream& os, const L2rEnumeration *en) {
    os << en->nmembers();
    if(include_debug && en->hasDebug()) {
        const dbg_L2rEnumeration *dbg = static_cast<const dbg_L2rEnumeration *>
            (en);
        os << ' ' << dbg->name();
        for(unsigned i=0; i < dbg->nmembers(); ++i)
            os << ' ' << dbg->name(i);
    }
    os << _STD_ endl;
}


/***************************************************************************
  Print all variables.
 ***************************************************************************/
void to_l2::print_variables(_STD_ ostream& os) {
    if(include_debug) {
        if(get_source()->dbg_vars())
            os << "dbg_variables\n";
        else
            os << "variables\n";
    } else
        os << "v\n";
    os << get_source()->nvars() << _STD_ endl;
    for(unsigned i=0; i < get_source()->nvars(); i++)
        print_one_var(os, get_source()->getVar(i));
}

void to_l2::print_one_var(_STD_ ostream& os, const L2rVariable* var) {
    os << var->type()->id();
    if(include_debug && var->hasDebug()) {
        const dbg_L2rVariable* dbg = static_cast<const dbg_L2rVariable*>(var);
        os << ' ' << dbg->name();
    }
    os << _STD_ endl;
}


/***************************************************************************
  Print all clauses.
 ***************************************************************************/
void to_l2::print_clauses(_STD_ ostream& os) {
    if(include_debug) {
        if(get_source()->dbg_clauses())
            os << "dbg_clauses\n";
        else
            os << "clauses\n";
    }
    else
        os << "cl\n";
    os << get_source()->nclauses() << _STD_ endl;
    for(unsigned i=0; i < get_source()->nclauses(); i++)
        print_one_clause(os, get_source()->getClause(i));
}

void to_l2::print_one_clause(_STD_ ostream& os, const L2rClause* cls) {
    if(include_debug && cls->hasDebug()) {
        const dbg_L2rClause *dbg = static_cast<const dbg_L2rClause *>(cls);
        os << dbg->component() << ' ';
    }
    if(cls->nprops()==0) return;
    os << cls->prop(0)->id();
    for(unsigned i=1; i<cls->nprops(); i++)
        os << ' ' << cls->prop(i)->id();
    os << _STD_ endl;
}


/***************************************************************************
  Print the IDs of background clauses.
 ***************************************************************************/
void to_l2::print_background(_STD_ ostream& os) {
    if(include_debug)
        os << "background\n";
    else
        os << "b\n";
    // count the number of background clauses
    unsigned i;
    unsigned nback=0;
    for(i=0; i<get_source()->nclauses(); ++i) {
        if(get_source()->getClause(i)->isInBackground())
            nback++;
    }
    os << nback << _STD_ endl;
    for(i=0; i < get_source()->nclauses(); i++) {
        // print the clauseID, not the clause
        if(get_source()->getClause(i)->isInBackground())
            os << i << _STD_ endl;
    }
}






/***************************************************************************
        propositions section:
        we need to partition the propositions into positive and negative ones
 ***************************************************************************/
void to_l2::print_propositions(_STD_ ostream& os) {
    if(include_debug)
        os << "propositions\n";
    else
        os << "p\n";

    // collect the props of each type
    unsigned npos, nneg, nsame, ndiff;
    xlate_sort_props::count_props(get_source(), npos, nneg, nsame, ndiff);

    // make sure they're correctly sorted
    xlate_sort_props::check_sorted(get_source(),npos, nneg, nsame, ndiff);

    // print the four counts, all on the same line
    os  << npos << ' ' << nneg << ' ' << nsame << ' ' << ndiff << '\n';

    unsigned i;

    // print the var=value propositions
    for(i=0; i<npos+nneg; ++i) {
        const L2rProposition *prop = source()->getProp(i);
        L2_assert(!prop->isEquality(), L2_fatal_error,
                ("counting or sorting props failed: equality in pos/neg"));
        print_one_varval(os, static_cast<const L2rPropVarValue*>(prop));
    }
    for( ; i<npos+nneg+nsame+ndiff; ++i) {
        const L2rProposition *prop = source()->getProp(i);
        L2_assert(prop->isEquality(), L2_fatal_error,
                ("counting or sorting props failed: var-value in same/diff"));
        print_one_varvar(os, static_cast<const L2rPropVarVar*>(prop));
    }

    L2_assert(i==get_source()->nprops(), L2_fatal_error,
            ("counting or sorting props failed: counts don't add up"));
}


void to_l2::print_one_varval(_STD_ ostream& os, const L2rPropVarValue* p) {
    os << p->var()->id() << ' ' << p->value() << _STD_ endl;
}
void to_l2::print_one_varvar(_STD_ ostream& os, const L2rPropVarVar* p) {
    os << p->var()->id() << ' ' << p->otherVar()->id() << _STD_ endl;
}

/***************************************************************************
        transitions section:
        similarly, we need to partition nominal/failure
 ***************************************************************************/

static void gather_one_transition(Slist<const L2rTransition*>& list,
        const L2rTransition *x, bool& has_debug, unsigned& listsize) {
#ifdef ENABLE_L2_DEBUG_SECTIONS
    if(x->hasDebug()) {
        has_debug = true;
    } else {
        L2_assert(!has_debug, L2_parse_model_error,
                ("not all transitions have debug info"));
    }
#endif
    list.push_front(x);
    listsize++;
}

void to_l2::print_transitions(_STD_ ostream& os) {
    // first, gather them all up
    unsigned i;
    unsigned nom=0, fail=0;
    Slist<const L2rTransition*> nominal, failure;
    bool has_debug = false;

    for(i=0; i<get_source()->nvars(); i++) {
        const L2rVariable *var = get_source()->getVar(i);
        if(var->kind() != vk_mode) continue;
        const L2rMode *mode = var->mode();

        L2rMode::iterator it;
        for(it = mode->begin_nominal() ; it!=mode->end_nominal() ; ++it) {
            gather_one_transition(nominal, *it, has_debug, nom);
        }
        for(it = mode->begin_failure() ; it!=mode->end_failure() ; ++it) {
            gather_one_transition(failure, *it, has_debug, fail);
        }
    }

    // print the section header
    if(include_debug) {
       if(has_debug)
           os << "dbg_transitions\n";
       else
           os << "transitions\n";
    } else
        os << "t\n";
    os << nom << _STD_ endl << fail << _STD_ endl;

    // And now the section data.
    {
        // nominal transitions first
        Slist<const L2rTransition*>::iterator it;
        for(it=nominal.begin(); it!=nominal.end(); ++it)
            print_one_transition(os, *it);
        // failure ones second
        for(it=failure.begin(); it!=failure.end(); ++it)
            print_one_transition(os, *it);
    }
}

void to_l2::print_one_transition(_STD_ ostream& os, const L2rTransition* x) {
    if(include_debug && x->hasDebug())
        os << static_cast<const dbg_L2rTransition*>(x)->name() << ' ';
    os  << x->mode()->id() << ' ';
    if(x->from() == L2rTransition::ANY_MODE) os << '*';
    else os << x->from();
    os << ' ' << x->to();
    if(x->isFailure()) os << ' ' << x->rank();
    for(unsigned i=0; i<x->nclauses(); i++)
        os << ' ' << x->clause(i)->id();
    os << _STD_ endl;
}




/***************************************************************************
        commands, modes, observed sections
 ***************************************************************************/

void to_l2::print_kind(_STD_ ostream& os, unsigned count, VarKind kind) {
    const char *section_name;
    if(include_debug) {
        switch(kind) {
            case vk_commanded:
                section_name = "commands"; break;
            case vk_observed:
                section_name = "observed"; break;
            case vk_mode:
                section_name = "modes"; break;
            default:
                assert(0);
        }
    } else {
        switch(kind) {
            case vk_commanded:
                section_name = "cm"; break;
            case vk_observed:
                section_name = "o"; break;
            case vk_mode:
                section_name = "m"; break;
            default:
                assert(0);
        }
    }

    os << section_name << _STD_ endl;
    for(unsigned i=0 ; i < get_source()->nvars(); i++) {
        const L2rVariable *v = get_source()->getVar(i);
        if(v->kind() == kind) {
            os << v->id();
            count--;
            if(count!=0)
                os << ' ';
            else
                break;
        }
    }
    os << _STD_ endl;
}

/***************************************************************************
        initial (mode assignment) section
 ***************************************************************************/
void to_l2::print_initial(_STD_ ostream& os) {
    unsigned ninitial=0;
    unsigned i;
    // count
    for(i=0; i<get_source()->nvars(); i++) {
        if(get_source()->getVar(i)->initial() != L2rVariable::NO_INITIAL_VALUE)
            ninitial++;
    }
    // unlikely ever to happen
    if(ninitial==0) return;

    // print header
    if(include_debug)
        os << "initial\n";
    else
        os << "i\n";
    os << ninitial << _STD_ endl;

    // print data
    for(i=0; i<get_source()->nvars(); i++) {
        const L2rVariable *var = get_source()->getVar(i);
        if(var->initial() != L2rVariable::NO_INITIAL_VALUE) {
            os << var->id() << ' ' << var->initial() << _STD_ endl;
        }
    }
}
