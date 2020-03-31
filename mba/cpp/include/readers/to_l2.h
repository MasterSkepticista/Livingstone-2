/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_l2.h,v 1.10 2004/01/28 23:31:41 lbrown Exp $ */
#ifndef to_file_HEADER
#define to_file_HEADER

/**
 * Interface from memory to the L2 file format.
 */

#include <readers/L2_file_writer.h>
#include <readers/varkind.h>

class L2_file;
class L2rClause;
class L2rEnumeration;
class L2rPropVarValue;
class L2rPropVarVar;
class L2rTransition;
class L2rVariable;

class to_l2 : public L2_file_writer {
    private:
        // whether to print longer section names, more newlines, etc
        // Does _not_ affect printing of debugging information.  Use
        // xlate_strip to remove debug info.
        bool include_debug;
        MBA_string filename_;

    public:
        to_l2(const L2_file *f, const MBA_string& fname, bool with_dbg=true)
            : L2_file_writer(f) , include_debug(with_dbg), filename_(fname)
            { }
        bool write();

    private:
        void print_enums(_STD_ ostream&);
        void print_one_enum(_STD_ ostream&, const L2rEnumeration*);

        void print_variables(_STD_ ostream&);
        void print_one_var(_STD_ ostream&, const L2rVariable*);

        void print_propositions(_STD_ ostream&);
        void print_one_varval(_STD_ ostream&, const L2rPropVarValue*);
        void print_one_varvar(_STD_ ostream&, const L2rPropVarVar*);

        void print_clauses(_STD_ ostream&);
        void print_one_clause(_STD_ ostream&, const L2rClause*);

        void print_background(_STD_ ostream&);

        void print_transitions(_STD_ ostream&);
        void print_one_transition(_STD_ ostream&, const L2rTransition*);

        void print_kind(_STD_ ostream& os, unsigned count, VarKind kind);

        void print_initial(_STD_ ostream&);
};

#endif
