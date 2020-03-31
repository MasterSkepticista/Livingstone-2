/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef FROM_BIN_H
#define FROM_BIN_H

#include "L2_file_reader.h"        // for class L2_file_reader
#include "mba_utils/MBA_string.h"  // for class MBA_string

/**
 * A reader for model files in the L2bin format
 */

class from_bin : public L2_file_reader {
 public:
  /// Save destination L2_file, model file pathname.
  from_bin(L2_file* f, MBA_string fname);

  /// Free the arry into which the model file was read.
  virtual ~from_bin();

  /// Read the model file into the destination L2_file object.
  virtual bool read();

 private:
  /// Read the model file into member array theModel
  size_t readIntoMemory(const char * const pathname);

 private:
  /// The pathname of the model file
  const char * modelFilePathname;

  /// The number of bytes in the model file/array
  unsigned int modelSize;

  /// The contents of the model file
  unsigned char * theModel;
};
#endif
