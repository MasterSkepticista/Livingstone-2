/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_graphviz.h,v 1.5 2004/01/28 23:31:40 lbrown Exp $ */
#ifndef to_file_HEADER
#define to_file_HEADER

/**
 * Interface from memory to a graph format supported by
   AT&T's dot and neato graphing programs
 */

#include <readers/clause.h>

class _STD_ ostream;
class L2_file;

/// This class accepts an L2_file representing some model, and 
/// writes out various graphs that visualize its structure.
/// It writes the graph in a text format that can be rendered 
/// with the graphviz toolkit, available for free from AT&T Research.
/// http://www.research.att.com/sw/tools/graphviz  
class to_graphviz {
    private:
        const L2_file& l2;

    public:
        to_graphviz(const L2_file& f) : l2(f) { }

		/// Creates a graph where if two variables appear in 
		/// a constraint together, they are connected by an edge.
		bool write_variable_connection_graph(const char *fname);

    private:
		void write_header(_STD_ ostream& os);
		void print_clause_connections(const L2rClause& cl,
					      _STD_ ostream& os);
};

#endif
