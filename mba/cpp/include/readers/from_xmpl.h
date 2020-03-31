/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_xmpl.h,v 1.20 2004/02/06 18:53:26 lbrown Exp $ */
#ifndef from_xmpl_HEADER
#define from_xmpl_HEADER

/**
 * Interface from the xml-based file format into memory.
 */

#include <mba_utils/hash_table.h>
#include <readers/L2_file.h>
#include <readers/L2_file_reader.h>

// forward-declarations; these are mostly the data types that we parse
// We include enumeration.h et al in parser.cpp
class L2rEnumeration;
class dbg_L2rEnumeration;
class L2rVariable;
class dbg_L2rVariable;
class L2rProposition;
class L2rClause;
class L2rTransition;
class dbg_L2rTransition;

class from_xmpl : public L2_file_reader {
    public:
        // The value mut be different from that of L2rTransition::ANY_MODE
        enum public_constants { NOT_FOUND = -2 };

        from_xmpl(L2_file *f, const MBA_string& fname) :
	  L2_file_reader(f),
	  filename_(fname),
	  parsing_transition(false), // not parsing anything yet
	  modevar_(0),
	  transition_from(0),
	  transition_to(0),
	  transition_rank(0),
	  transition_isNominal(false),
	  transition_name(""),
	  parsing_clause(false), // not parsing anything yet
	  nclause_props(0), // not parsing anything yet
	  parsing_prop(false),
	  component_name(""),
	  enums_(hashpjw),
	  vars_(hashpjw),
	  nenums_(0),
	  nvars_(0),
	  nprops_(0),
	  nclauses_(0) { }

        virtual ~from_xmpl() {}

        bool read();


    private:
        MBA_string filename_;


        // used to store a transition while we are still getting its clauses
        bool parsing_transition;
        L2rVariable *modevar_; // actually set by <ci:attribute>
        int transition_from, transition_to;
        unsigned transition_rank;
        bool transition_isNominal;
        MBA_string transition_name;
        Slist<L2rClause*> transition_clauses;

        // used to store a clause while we're getting its propositions
        bool parsing_clause;
        Slist<L2rProposition*> clause_props;
        unsigned nclause_props;

        // used to denote that we're in a <ci:term> tag (parsing a proposition)
        // and thus should listen to the characters callback.
        bool parsing_prop;

        // name of the current component
        MBA_string component_name;


        // stores the lists that directly map to the sections; these
        // are then copied into the L2_file
        Hash_table<MBA_string, dbg_L2rEnumeration*> enums_;
        Hash_table<MBA_string, dbg_L2rVariable*> vars_;
        Slist<L2rProposition*>  props_;
        Slist<L2rClause*>       clauses_;
        Slist<L2rTransition*>   X_;

        // the size, and also the next ID, of enumerations, variables, etc.
        unsigned nenums_;
        unsigned nvars_;
        unsigned nprops_;
        unsigned nclauses_;

        typedef Hash_table<MBA_string,dbg_L2rEnumeration*>::iterator enumsIT;
        typedef Hash_table<MBA_string,dbg_L2rVariable*>   ::iterator varsIT;
        typedef Slist<L2rProposition*>::iterator propsIT;
        typedef Slist<L2rClause*>     ::iterator clausesIT;
        typedef Slist<L2rTransition*> ::iterator transitionsIT;

        /// Find an enumeration given its name
        dbg_L2rEnumeration *find_enum(const MBA_string&);

        /// Create a new enumeration
        dbg_L2rEnumeration *create_enum(const MBA_string&,
                /*const*/ Slist<MBA_string>&);

        /**
	 * Find a member of an enumeration given its name.
	 * \param pL2rEnumeration pointer to a dbg_L2rEnumeration object
	 * \param name the name of the element
	 * \return if name is "*", return L2rTransition::ANY_MODE; if the
	 * element is found, return it index; otherwise return NOT_FOUND
	 */
        int find_enum_member(const L2rEnumeration* pL2rEnumeration,
			     const MBA_string& name);

        /// Find a variable given the fully-qualified name
        L2rVariable *find_variable(const MBA_string&, bool fatal=true);
        L2rVariable *create_variable(const dbg_L2rEnumeration*,
                const MBA_string&);


        /// Find a proposition given its components; or, if it doesn't
        /// exist, create it.
        L2rProposition *find_or_create_prop(
                const L2rVariable *var,
                bool isPositive,
                const MBA_string& valOrVar);

        /// Add the proposition to the list that'll become a clause.
        void add_clause_prop(L2rProposition*);
        bool clause_eq_clause_props(const L2rClause*);

        /// Create the clause that we've been accumulating (or, if
        /// we already have an identical one, return it).
        L2rClause *find_or_create_clause();


        /****************************************
         * The parser.
         * We have a class that internally uses Xerces; we separate
         * it out to make it easier not to require including the Xerces
         * headers.  We're a friend to it and have it call our functions.
         ****************************************/
        class HandlerBase_subclass;
        friend class HandlerBase_subclass;

        // the callbacks
        void start_attribute(const MBA_string&, const MBA_string&);
        void start_cmd(const MBA_string&);
        void start_obs(const MBA_string&);
        void start_assign(const MBA_string&);
        void start_attributetype(const MBA_string&, const MBA_string&);
        void start_component(const MBA_string&);
        void start_clause();
        void start_prop();
        void start_statevector(const MBA_string&);
        void start_transition(const MBA_string&, const MBA_string&, 
                const MBA_string&, const MBA_string&);
        void characters(const MBA_string&);
        void end_component();
        void end_clause();
        void end_prop();
        void end_transition();
};

#endif
