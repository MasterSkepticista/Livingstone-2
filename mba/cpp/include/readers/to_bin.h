/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_bin.h,v 1.5 2002/09/20 18:37:03 lbrown Exp $ */
#ifndef to_bin_HEADER
#define to_bin_HEADER

#include <readers/L2_file_writer.h>
#include <readers/common.h>

class L2rClause;
class L2rEnumeration;
class L2rProposition;
class L2rTransition;
class L2rVariable;

/**
 * Writes the model in an L2_file object in l2bin format to a model file
 * specified by pathname.
 */

class to_bin : public L2_file_writer {
    public:
        
	/** Store the arguments; count and sort the Propositions.
	 * \param pL2_file the model as read in
	 * \param fname pathname of the l2bin model output file
	 */ 
        to_bin(const L2_file *pL2_file, MBA_string fname);

	/** Close the output file if it is still open. */
        virtual ~to_bin();

	/**
	 * Write the model to the output file.
	 * \return whether the output model file could be opened
	 */
        bool write();

    private:
	/**
	 * Write the cookie string "L2bi", the version number, the checksum,
	 * the 7 number field widths, and the 7 number fields. The 7 number
	 * fields are: (1) maximum domain cardinality, (2) Variable count,
	 * (3) Proposition count, (4) Clause count, (5) maximum Propositions
	 * per Clause, (6) maximum Transitions per Mode and (7) maximum
	 * Clauses per Transition.
	 */
        void write_header();

	/**
	 * Something like the minimum number of bytes needed to represent an
	 * unsigned.
	 * \param val the unsigned int whose width is sought
	 * \return the minimum number of bits needed to represent val
	 * \todo If val=0, this returns 0. It should be 1. A value of 0
	 * indicates an incomplete model (e.g., lacking transitions), but if
	 * so it should fail in ways that are not obscure.
	 */
        unsigned width_of_int(unsigned val);

	/**
	 * Since the minimum field width is 1, map [1..4] as [0..3] to encode
	 * field width.
	 * \param w field width
	 * \return w-1 if w is in [1..4]; otherwise throw an exception
	 */
        unsigned width_to_two_bits(unsigned w);

	/** Write all the enums to the L2bin model file */
        void write_enums();

	/** Write all the Variables to the L2bin model file */
        void write_variables();

	/** Write all the Propositions to the L2bin model file */
        void write_props();

	/** Write all the Clauses to the L2bin model file */
        void write_clauses();

	/** Write all the Transitions to the L2bin model file */
        void write_transitions();

	/** Write the enum to the L2bin model file
	 * \param pL2rEnumeration pointer to an enum
	 */
        void write_one_enum(const L2rEnumeration* pL2rEnumeration);

	/** Write the Variable to the L2bin model file
	 * \param pL2rEnumeration pointer to a Variable
	 */
        void write_one_variable(const L2rVariable* pL2rVariable);

	/** Write the Variable=value Proposition to the L2bin model file
	 * \param pL2rProposition pointer to a Proposition
	 */
        void write_one_varvalue(const L2rProposition* pL2rProposition);

	/** Write the Variable1=Variable2 Proposition to the L2bin model file
	 * \param pL2rProposition pointer to a Proposition
	 */
        void write_one_varvar(const L2rProposition* pL2rProposition);

	/** Write the Clause to the L2bin model file
	 * \param pL2rClause pointer to a Clause
	 */
        void write_one_clause(const L2rClause* pL2rClause);

	/** Write the Transition to the L2bin model file
	 * \param pL2rTransition pointer to a Transition
	 */
        void write_one_transition(const L2rTransition *pL2rTransition);

	/**
	 * Write one char to the model file.
	 * \param c the char
	 */
        void put1(unsigned c);

	/**
	 * Write the value using the specified number of byes.
	 * \param width the number of bytes to use
	 * \value the value to write
	 */
        void put(unsigned width, unsigned value);

 private:
        // field widths

	/** Width of enum count */
        unsigned We;
	/** Width of Variable count */
	unsigned Wv;
	/** Width of Proposition count */
	unsigned Wp;
	/** Width of Clause count */
	unsigned Wc;
	/** Width of max Clause size plus one */
	unsigned Wch;
	/** Width of max number of Transitions per mode */
	unsigned Wx;
	/** Width of max number of Clauses per Transition */
	unsigned Wxc;

        // how many propositions of each type
	/** Number of Variable = value Propositions */
        unsigned npos;
	/** Number of Variable != value Propositions */
	unsigned nneg;
	/** Number of Variable1 = Variable2 Propositions */
	unsigned nsame;
	/** Number of Variable1 != Variable2 Propositions */
	unsigned ndiff;

        // the file
	/** Output l2bin model open FILE */
        FILE *thefile;
	/** Pathname of the output l2bin model file */
        MBA_string thefilename;
};

#endif
