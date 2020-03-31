/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_graphviz.cpp,v 1.5 2004/01/28 23:31:50 lbrown Exp $ */

#include <errno.h>
#include <livingstone/L2_fstream.h>
#include <readers/to_graphviz.h>
#include <readers/common.h>
#include <readers/L2_file.h>
#include <readers/transition.h>

void to_graphviz::write_header(_STD_ ostream& os) {
	os << "/***************************************************/" << _STD_ endl;
    os << "/*                                                 */" << _STD_ endl;
    os << "/*  L2 graphviz file                               */" << _STD_ endl;
    os << "/*                                                 */" << _STD_ endl;
	os << "/* This file will generate a graph when used with  */" << _STD_ endl;
    os << "/* AT&T Research's graphviz package, available from*/" << _STD_ endl;
    os << "/* http://www.research.att.com/sw/tools/graphviz   */" << _STD_ endl;
    os << "/*                                                 */" << _STD_ endl;
	os << "/* Run this file through the graphviz neato program*/" << _STD_ endl;
    os << "/* to create a nice layout.  Then use lneato or    */" << _STD_ endl;
    os << "/* dotty to draw the layout.                       */" << _STD_ endl;
    os << "/*                                                 */" << _STD_ endl;
    os << "/*     lneato < neato \"thisfile\"                   */" << _STD_ endl;
    os << "/*                                                 */" << _STD_ endl;
	os << "/***************************************************/" << _STD_ endl;
}



bool to_graphviz::write_variable_connection_graph(const char *fname) {
    _STD_ ofstream l2file(fname);
    if(!l2file) {
        _STD_ cerr<< "Error opening `" << fname << "' for write: "
            << strerror(errno) << _STD_ endl;
        return false;
    }

	write_header(l2file);
	l2file << "graph G {" << _STD_ endl;
    l2file << "  edge [size=\"20,20\"];" << _STD_ endl;
    l2file << "  node [fontsize=8,height=0.3,width=1,shape=box,fontname=Courier];" << _STD_ endl;

	unsigned i=0;
	/// Write out "regular" clauses
    for(i=0; i < l2.nclauses(); i++) 
        print_clause_connections(*l2.getClause(i),l2file);

	/// Write out background clauses
    for(i=0; i < l2.nbackground(); i++) 
        print_clause_connections(*l2.getBackground(i),l2file);

    //    print_transitions(l2file);

	l2file << "}" << _STD_ endl;
    return true;
}


void to_graphviz::print_clause_connections(const L2rClause& cl, _STD_ ostream& os) {
	if(cl.nprops()==0) return;
	bool printed=false;
	Slist<const L2rVariable*> vars;
	for(unsigned i=0; i<cl.nprops(); i++)  {
		const L2rProposition* prop = cl.prop(i);
		if (!prop->isEquality())
					vars.push_front(prop->var());					
				else {
					const L2rPropVarVar *eqProp = static_cast<const L2rPropVarVar*>(prop);
					vars.push_front(eqProp->var());					
					vars.push_front(eqProp->otherVar());					
				}
			}
			
			Slist<const L2rVariable*>::iterator begin = vars.begin();
			Slist<const L2rVariable*>::iterator end = vars.end();
			Slist<const L2rVariable*>::iterator  it1, it2;
			for (it1=begin;it1!=end;++it1) {
				for (it2=begin;it2!=end;++it2) {
					if (*it1 != *it2)
						os << "\"" << *it1 << "\" -- \"" << *it2 << "\"  ";
				}
			}
			os << _STD_ endl;
}
	




