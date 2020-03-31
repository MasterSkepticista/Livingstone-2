/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_memory.h,v 1.4 2001/12/28 19:20:50 lbrown Exp $ */
#ifndef from_memory_HEADER
#define from_memory_HEADER

#include <sys/types.h> // For typedef off_t
#include <readers/common.h>
#include <readers/L2_file_reader.h>

class L2rVariable;

/**
 * Interface from the L2 binary model file format from memory.
 * The result of reading the model file is an L2_file object.
 */

class from_memory : public L2_file_reader {
 private:
  // field widths
  unsigned
    We,  // Enumeration
    Wv,  // Variable
    Wp,  // Proposition
    Wc,  // Clause
    Wch, // Clause header
    Wx,  // Transition
    Wxc; // transition count

  // section sizes
  unsigned nenums_;      // number of Enumerations
  unsigned nvars_;       // number of Variables
  unsigned nposprops_;   // number of Variable==Value Propositions
  unsigned nnegprops_;   // number of Variable!=Value Propositions
  unsigned nsameprops_;  // number of Variable1==Variable2 Propositions
  unsigned ndiffprops_;  // number of Variable1!=Variable2 Propositions
  unsigned nclauses_;    // number of Clauses

  /// The number of bytes in the model
  const off_t modelSize;
  /// The model read in from the file
  const unsigned char * const theModel;
  /// The next character to get
  size_t nextChar;

 public:
  /**
   * This object read from the array with the given size into the destination
   * L2_file object.
   * Call from_bin::read() to read the model array.
   */
  from_memory(L2_file *f,
	      const unsigned char * const theModel,
	      size_t modelSize);

  /// Nothing to do.
  virtual ~from_memory();

  /// The API action to read the model array into the destination object.
  virtual bool read();

 protected:
  /// Read the cookie, version and checksum
  bool read_header();

  /// Read the "magic cookie," a code that identifies this as an L2bin file
  bool read_cookie();

  /// Read the format version number.
  bool read_version();

  /**
   * Read the checksum; currently a no-op.
   * \todo Either implement this or get rid of it.
   */
  bool read_checksum();

  /// Read the "weight," the number of bytes used to represent the count.
  void read_weights();

  /// Read the numbers of model elements of each type.
  void read_counts();

  /// Read the enumerations (Variable data types) into L2rEnumeration objects.
  void read_enums       ();

  /// Read the Variables into L2rVariable objects.
  void read_variables	();

  /// Read the Propositions into L2rProposition objects.
  void read_propositions();

  /// Read the Clauses into L2rClause objects.
  void read_clauses	();

  /// Read the Transitions into L2rTransition objects.
  void read_transitions	();

 private:
  // These are used int the body of for-each loops
  void read_one_enum(unsigned);
  void read_one_variable(unsigned);
  void read_one_varval(unsigned, bool); // variable=value
  void read_one_varvar(unsigned, bool); // variable1=variable2
  void read_one_clause(unsigned);
  void read_one_transition(bool, const L2rVariable*);

  /// Get the specified number of bytes, packed into a word
  unsigned get(unsigned width);

  /// Get one byte
  unsigned char get1();
};

#endif
