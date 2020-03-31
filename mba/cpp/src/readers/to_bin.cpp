/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_bin.cpp,v 1.10 2002/09/20 22:31:33 lbrown Exp $ */

#include <readers/L2_file.h>
#include <readers/to_bin.h>
#include <readers/transition.h>
#include <readers/xlate_sort_props.h>

to_bin::to_bin(const L2_file *f, MBA_string fname)
    : L2_file_writer(f)
{
    thefilename = fname;
    thefile = NULL;

    // make sure the input file is valid for us
    xlate_sort_props::count_props(f,  npos, nneg, nsame, ndiff);
    xlate_sort_props::check_sorted(f, npos, nneg, nsame, ndiff);
}

to_bin::~to_bin()
{
    if(thefile) fclose(thefile);
}

bool to_bin::write()
{
    thefile = fopen(thefilename.c_str(), "wb");
    if(!thefile) return false;

    write_header();
    write_enums();
    write_variables();
    write_props();
    write_clauses();
    write_transitions();

    fclose(thefile);
    thefile = NULL;

    return true;
}


unsigned to_bin::width_of_int(unsigned val) {
    unsigned width = 0;
    while(val!=0) {
        val >>= 8;
        ++width;
        assert(val!=(unsigned)-1); // beware of sign extension
    }
    return width;
}

unsigned to_bin::width_to_two_bits(unsigned w) {
    switch(w) {
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 4:
            return 3;
        default:
            L2_throw(L2_fatal_error,
                    ("width of "+MBA_string(w)+" does not fit in 2 bits"));
    }
}

void to_bin::put1(unsigned c) {
    putc(c, thefile);
}

void to_bin::put(unsigned width, unsigned value) {
    // first, flip the bytes so we write MSB first
    unsigned newval = 0;
    unsigned i;
    for(i=0; i<width; ++i) {
        newval <<=8;
        newval |= (value & 0xff);
        value  >>= 8;
    }

    L2_assert(value==0, L2_writer_error,
            ("value was too big for width "+MBA_string(width)));

    // write the integer
    for(i=0; i<width; ++i) {
        put1(newval & 0xff);
        newval >>= 8;
    }
}


void to_bin::write_header() {
    put1('L');
    put1('2');
    put1('b');
    put1('i');
    put(2, 3);    // 2 bytes, format version number
    put(4, 0);    // TODO: checksum

    // find the widths
    unsigned i;
    unsigned largest;

    // enumeration value widths
    for(largest=0, i=0; i<source()->nenums(); ++i) {
        const L2rEnumeration *e = source()->getEnum(i);
        if(largest < e->nmembers())
            largest = e->nmembers();
    }
    We = width_of_int(largest);

    // variable ID widths
    Wv = width_of_int(source()->nvars());

    // proposition ID widths
    Wp = width_of_int(source()->nprops());

    // clause ID widths
    Wc = width_of_int(source()->nclauses());

    // clause count of propositions (clause header)
    for(largest=0, i=0; i<source()->nclauses(); ++i) {
        const L2rClause *c = source()->getClause(i);
        if(largest < c->nprops())
            largest = c->nprops();
    }
    Wch = width_of_int(largest<<1); // one extra bit for background flag

    // max # of transitions per mode
    for(largest=0, i=0; i<source()->nvars(); ++i) {
        const L2rVariable *v = source()->getVar(i);
        if(v->kind() != vk_mode) continue;
        if(largest < v->mode()->nnominal())
            largest = v->mode()->nnominal();
        if(largest < v->mode()->nfailure())
            largest = v->mode()->nfailure();
    }
    Wx = width_of_int(largest);

    // max # of clauses per transition
    for(largest=0, i=0; i<source()->nvars(); ++i) {
        const L2rVariable *v = source()->getVar(i);
        if(v->kind() != vk_mode) continue;
        const L2rMode *m = v->mode();
        L2rMode::iterator it;
        for(it = m->begin_nominal(); it!=m->end_nominal(); ++it)
            if(largest < (*it)->nclauses())
                largest = (*it)->nclauses();
        for(it = m->begin_failure(); it!=m->end_failure(); ++it)
            if(largest < (*it)->nclauses())
                largest = (*it)->nclauses();
    }
    Wxc = width_of_int(largest);


    // write the sizes out
    // We, Wv, Wp, Wc share the first byte; make sure each actually only
    // takes 2 bits
    unsigned char pack = 0;
    pack += width_to_two_bits(We);
    pack <<= 2;
    pack += width_to_two_bits(Wv);
    pack <<= 2;
    pack += width_to_two_bits(Wp);
    pack <<= 2;
    pack += width_to_two_bits(Wc);

    put1(pack);

    // Wch, Wx, Wxc go together
    pack = 0;
    pack += width_to_two_bits(Wch);
    pack <<= 2;
    pack += width_to_two_bits(Wx);
    pack <<= 2;
    pack += width_to_two_bits(Wxc);
    put1(pack);



    // counts
    put(We, source()->nenums());
    put(Wv, source()->nvars());
    put(Wp, npos);
    put(Wp, nneg);
    put(Wp, nsame);
    put(Wp, ndiff);
    put(Wc, source()->nclauses());
}


/***************************************************************************
  Enumerations
 ***************************************************************************/
void to_bin::write_one_enum(const L2rEnumeration *en) {
    put(We, en->nmembers());
}

void to_bin::write_enums() {
    for(unsigned i=0; i<source()->nenums(); ++i)
        write_one_enum(source()->getEnum(i));
}


/***************************************************************************
  Variables
 ***************************************************************************/
void to_bin::write_one_variable(const L2rVariable *var) {
    put(We, var->type()->id());
    unsigned char kind;
    switch(var->kind()) {
        case vk_unknown:
            kind = 100;
            break;
        case vk_observed:
            kind = 101;
            break;
        case vk_commanded:
            kind = 102;
            break;
        case vk_mode:
            kind = 103;
            break;
        default:
            L2_throw(L2_reader_error,
                    ("unknown kind "+MBA_string(var->kind())));
    }
    put1(kind);
    if(var->kind()==vk_mode) {
        put(We, var->initial());
    }
}

void to_bin::write_variables() {
    for(unsigned i=0; i<source()->nvars(); ++i) {
        write_one_variable(source()->getVar(i));
    }
}


/***************************************************************************
  Propositions
 ***************************************************************************/
void to_bin::write_one_varvalue(const L2rProposition *prop) {
    L2_assert(!prop->isEquality(), L2_writer_error,
            ("proposition "+MBA_string(prop->id())+" is an equality"));
    const L2rPropVarValue *p = static_cast<const L2rPropVarValue*>(prop);
    put(Wv, p->var()->id());
    put(We, p->value());
}

void to_bin::write_one_varvar(const L2rProposition *prop) {
    L2_assert(prop->isEquality(), L2_writer_error,
            ("proposition "+MBA_string(prop->id())+" is not an equality"));
    const L2rPropVarVar *p = static_cast<const L2rPropVarVar*>(prop);
    put(Wv, p->var()->id());
    put(Wv, p->otherVar()->id());
}
void to_bin::write_props() {
    unsigned count, id=0;
    for(count=0 ; count < npos ; ++count, ++id )
        write_one_varvalue(source()->getProp(id));
    for(count=0 ; count < nneg ; ++count, ++id )
        write_one_varvalue(source()->getProp(id));

    for(count=0 ; count < nsame ;  ++count, ++id )
        write_one_varvar(source()->getProp(id));
    for(count=0 ; count < ndiff ; ++count, ++id )
        write_one_varvar(source()->getProp(id));
}




/***************************************************************************
  Clauses
 ***************************************************************************/

void to_bin::write_one_clause(const L2rClause *cls) {
    unsigned pack;
    pack = cls->nprops();
    pack <<= 1;
    if(cls->isInBackground()) ++pack;
    put(Wch, pack);

    for(unsigned j=0; j<cls->nprops(); ++j) {
        const L2rProposition *p = cls->prop(j);
        put(Wp, p->id());
    }
}
void to_bin::write_clauses() {
    for(unsigned i=0; i<source()->nclauses(); ++i) {
        write_one_clause(source()->getClause(i));
    }
}

/***************************************************************************
  Transitions
 ***************************************************************************/
void to_bin::write_one_transition(const L2rTransition *x) {
    if(x->isNominal()) {
        put(We, x->from());
        // rank is 0
    } else {
        // from is L2rTransition::ANY_MODE
        put1(x->rank());
    }
    put(We, x->to());
    put(Wxc, x->nclauses());
    for(unsigned j=0; j<x->nclauses(); ++j) {
        put(Wc, x->clause(j)->id());
    }
}

void to_bin::write_transitions() {
    for(unsigned i=0; i<source()->nvars(); ++i) {
        const L2rVariable *var = source()->getVar(i);
        if(var->kind() != vk_mode) continue;
        // not needed: we can do the same loop on the receiving end
        // put(Wv, var->id());
        const L2rMode *m = var->mode();
        put(Wx, m->nnominal());
        put(Wx, m->nfailure());

        L2rMode::iterator it;
        for(it = m->begin_nominal(); it!=m->end_nominal(); ++it)
            write_one_transition(*it);
        for(it = m->begin_failure(); it!=m->end_failure(); ++it)
            write_one_transition(*it);
    }
}
