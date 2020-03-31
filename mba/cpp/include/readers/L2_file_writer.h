/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file_writer.h,v 1.6 2001/12/29 00:39:58 lbrown Exp $ */
#ifndef L2_file_writer_HEADER
#define L2_file_writer_HEADER

#include <mba_utils/pooled.h>

class L2_file;

/**
 * A file responsible for writing an L2_file into some other data structure,
 * such as a file on disk or the T_system.
 *
 * The work is all done in the 'write' function.  Any arguments it needs should
 * be given in the constructor.
 */

class L2_file_writer : public Pooled {
 public:

  /**
   * Write the source L2_file to the destination.
   * \return whether the write operation succeeded
   */
  virtual bool write() = 0;
  
  /**
   * Return the source L2_file (that which is written).
   * \return the source L2_file
   */
  const L2_file *get_source() const { return source_; }

  /**
   * Return whether the writer is in verbos mode.
   * \return whether the writer is in verbose mode
   */
  bool isVerbose() const { return isVerbose_; }

  /**
   * Set whether the writer is in verbose mode.
   * \param v whether the writer is in verbose mode
   */
  void setVerbose(bool v) { isVerbose_ = v; }

 protected:

  L2_file_writer(const L2_file *s) { source_ = s; isVerbose_ = false; }

  /**
   * Nothing special to do.
   */
  virtual ~L2_file_writer() { }
  
  /**
   * Return the source L2_file
   * \return the source L2_file
   */
  const L2_file *source() const { return source_; }

 private:
  /// The source L2_file -- that which is being written.
  const L2_file *source_;

  /// Whether the writer is in verbose mode
  bool isVerbose_;
};

#endif
