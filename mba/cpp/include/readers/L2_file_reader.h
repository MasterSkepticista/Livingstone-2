/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file_reader.h,v 1.7 2001/12/28 22:07:17 lbrown Exp $ */
#ifndef L2_file_reader_HEADER
#define L2_file_reader_HEADER

#include <mba_utils/pooled.h>

class L2_file;

/**
 * An abstract base class that fills in an L2_file object from some source
 * (typically a file).
 * The work should be done in the read() function, which should be called only
 * once. Any arguments it will need should be given in the constructor.
 * Concrete subclasses must implement member function read().
 */

class L2_file_reader : public Pooled {
 public:

  /**
   * Read from the source file into the destination L2_file object.
   * Concrete subclasses must implement this method.
   */
  virtual bool read() = 0;

  /// Return a const pointer to the destination L2_file object
  const L2_file *get_dest() const { return dest_; }

  /// Set the verbosity level of the reader
  void setVerbose(bool v) { isVerbose_ = v; }

  /// Is the reader set to be verbose?
  bool isVerbose() const { return isVerbose_; }

  /// Nothing special to do
  virtual ~L2_file_reader() { }

 protected:

  /**
   * Disallow construction of the superclass.
   * This is also ensured by making method read() pure virtual.
   */
  L2_file_reader(L2_file *dest) : dest_(dest), isVerbose_(false) { }

 protected:

  /// Return a pointer to the destination L2_file object.
  L2_file *dest() { return dest_; }

 private:

  /// The destination L2_file object into which the model is read.
  L2_file *dest_;

  /// Whether the reader is set to be verbose.
  bool isVerbose_;
};

#endif
