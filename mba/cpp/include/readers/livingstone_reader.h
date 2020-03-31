/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone_reader.h,v 1.26 2004/02/10 22:16:37 lbrown Exp $ */
#ifndef livingstone_reader_HEADER
#define livingstone_reader_HEADER

#include <livingstone/L2_iostream.h>
#include <mba_utils/slist.h>

#ifdef ENABLE_L2_DEBUG_SECTIONS
#include <mba_utils/array.h>
#include <mba_utils/hash_table.h>
#endif

class L2_file;
class to_t_system;
class Tracker;

#ifdef ENABLE_L2_DEBUG_SECTIONS
class Variable;
#endif

#ifndef DISABLE_OPTIMIZER
class L2_file_translator;
#endif

/**
 * This base class for class Livingstone supplies reading and optimizing.
 * It joins together the capability to:
 *  - read a model using class from_any
 *  - possibly optimize it
 *  - get the result
 */

class Livingstone_reader : public Pooled {
 public:
  /// Initialize some member variables
  Livingstone_reader();

  /// Delete all the L2_file objects
  virtual ~Livingstone_reader();

  /// Is the reader verbose?
  bool verbose_reader() const { return verbose_reader_; }

  /// Set the reader's verbosity.
  void set_verbose_reader(bool val) { verbose_reader_ = val; }

  /// Is the optimizer verbose?
  bool verbose_optimizer() const { return verbose_optimizer_; }

  /// Set the optimizer's verbosity.
  void set_verbose_optimizer(bool val) { verbose_optimizer_ = val; }

  /// Set both reader and optimizer verbosity in one swell foop.
  void set_verbosity(bool reader, bool optimizer) {
    set_verbose_reader(reader);
    set_verbose_optimizer(optimizer);
  }

  /// Return the ostream to which verbose output is sent.
  _STD_ ostream& get_output() { return *out; }

  /// Set the ostream to which verbose output is sent.
  void set_output(_STD_ ostream& os) { out = &os; }

  /// Return a const pointer to the object into which the model is read.
  const L2_file *get_file() const {
    return l2files.empty() ? 0 : l2files.front();
  }

  /// Return a pointer to the object into which the model is read.
  L2_file *get_file() {
    return l2files.empty() ? 0 : l2files.front();
  }

  /**
   * Bypass the model file by using the supplied L2_file.
   * \param l2_file the L2_file object already read in by a Livingstone_reader
   */

  void set_file(L2_file* l2_file) { l2files.push_front(l2_file); }

  /**
   * Clean up by popping the L2_file
   */

  void pop_file() { l2files.pop_front(); }

  /************************************************************
   * Reading in a model and putting it into the tracker.
   ************************************************************/

#ifndef DISABLE_MODEL_FILES
  /**
   * Read a model file from the file system.
   * The file type is specified by the second argument; if unspecified or
   * empty, we discover the file type from the file name's extension.
   * \param filename the model file pathname
   * \param filetype the model file format
   */

  bool read_file(MBA_string filename, MBA_string filetype = "");

  /**
   * Read a model file from the file system.
   * It is assumed that the file name and file type have already been set.
   */
  bool read_file() { return read_file(filename_, filetype_); }

  /**
   * Return the pathname of the model file.
   * \return the pathname of the model file (without extension)
   */
  const MBA_string& getFilename() const { return filename_; }

  /**
   * Set the pathname of the model file.
   * \param modelFileName the pathname of the model file (without extension)
   */
  void setFilename(const MBA_string& modelFileName) {
    filename_ = modelFileName;
  }

  /**
   * Set the type (format) of the model file.
   * If empty (the default), it will be determined from the file name.
   * \param modelFileFormat the name of the format, "bin", "l2", "l2bin",
   * "l2s", "xmpl" or ""
   */
  void setFiletype(const MBA_string& modelFileFormat) {
    filetype_ = modelFileFormat;
  }

  /**
   * Return the type (format) of the model file.
   * \return  the name of the format, "bin", "l2", "l2bin", "l2s", "xmpl" or ""
   */
  const MBA_string& getFiletype() const { return filetype_; }

  /**
   * Return whether the reader is flagged to be optimized upon reading.
   * This flag is informational only. It is the client's responsibility to call
   * Livingstone::optimize().
   * \return whether the model is flagged to be optimized upon reading
   */
  bool getUseOptimizer() const { return use_optimizer_; }

  /**
   * Set whether the reader is flagged to be optimized upon reading.
   * If progress style is L2_parameters::min, leave this flag set to true or
   * results will be incorrect. This flag is informational only. It is the
   * client's responsibility to call Livingstone::optimize().
   * \param b whether the model should be optimized upon reading
   */

  void setUseOptimizer(bool b) { use_optimizer_ = b; }

#endif

  /// Read a model from an array in L2bin format.

  bool read_model_array(const unsigned char* modelArray,
			      size_t modelArrayLength);

#ifndef DISABLE_OPTIMIZER
  /**
   * Reduce the number of Variables, Propositions and Clauses if possible.
   * For flight code.
   * \return whether the operation succeded
   */
  bool optimize() { return (merge_eq_vars() && remove_duplicates()); }

  /**
   * Reduce the number of Variables, Propositions and Clauses if possible.
   * For debugger code.
   * \param variableHashMap
   * \param optimizedAwayVariableNames
   * \return whether the operation succeded
   */
  bool optimize(Hash_table<MBA_string, MBA_string>& variableHashMap,
		Array<MBA_string, true>& optimizedAwayVariableNames) {
    return
      (merge_eq_vars(variableHashMap, optimizedAwayVariableNames) &&
       remove_duplicates());
  }

  // Select each optimization separately.

  /**
   * Merge Variables that must always be equal to one another.
   * \return whether the operation succeeded
   */
  bool merge_eq_vars();

  /*
   * Merge Variables that must always be equal to one another.
   * \param variableHashMap equivalence classes of Variables
   * \param optimizedAwayVariableNames Array of names of vanished Variables
   * \return whether the operation succeeded
   */
  bool merge_eq_vars(Hash_table<MBA_string, MBA_string>& variableHashMap,
		     Array<MBA_string, true>& optimizedAwayVariableNames);
  
  /**
   * Map dbg_L2rClause onto L2rClause, dbg_L2rEnumeration onto L2rEnumeration,
   * dbg_L2rTranition onto L2rTransition, dbg_L2rVariable onto L2rVariable.
   */
  bool strip();

  /// Remove redundant L2rClauses and L2rPropositions
  bool remove_duplicates();

  /**
   * Sort Propositions into the order: (1) Variable1 == Variable2;
   * (2) Variable1 != variable2; (3) Variable = value; (4) Variable != value
   */
  bool sort_props(); // required for writing l2 and binary files

 protected:

  /**
   * Apply the optimizing translator to the model.
   * The translator must be uninitialized (source == dest == NULL)
   */
  bool apply_translator(L2_file_translator*);

#endif

#ifdef ENABLE_L2_DEBUG_SECTIONS
 public:
#endif

 private:
  /// The destination object is l2files[0]. Translated source files may follow.
  Slist<L2_file*> l2files;
  /// The output stream for verbose operations.
  _STD_ ostream* out;
  /// Whether the reader is verbose
  bool verbose_reader_:1;
  /// Whether the optimizer is verbose
  bool verbose_optimizer_:1;
  /// Pathname to the model file.
  MBA_string filename_;
  /// What kind of model file; that is, the format (L2, L2bin, XMPL).
  MBA_string filetype_;
  /// Is the optimizer to be used?
  bool use_optimizer_;
};

#endif
