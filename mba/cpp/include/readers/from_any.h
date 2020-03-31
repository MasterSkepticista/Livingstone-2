/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_any.h,v 1.7 2002/02/26 21:29:36 lbrown Exp $ */
#ifndef from_any_HEADER
#define from_any_HEADER

#include <mba_utils/MBA_string.h>
#include <mba_utils/array_dict.h>
#include <readers/L2_file_reader.h>

/**
 * A class that handles the messiness of figuring out which formats we have
 * compiled support for.
 *
 * Given the filename, an optional format name, and the configured options,
 * it does one of:
 *  - (usually) happily reads the file
 *  - rejects the file because its format is known but not configured
 *  - rejects the file because the reader can't grok it
 *
 * The implementation just feeds the work to another reader.
 */

class from_any : public L2_file_reader {
 public:
  /// Symbolic constants for file types
  enum FileType {
    unknown,
    xmpl,
    l2,
    l2bin
  };

  /**
   * Initialize with a destination object, a model source file name, and a
   * format name.
   */
  from_any(L2_file *dest, MBA_string filename, MBA_string format_name = "");

  /**
   * Initialize with a destination object, a model source file name, and a
   * format symbolic constant.
   */
  from_any(L2_file *dest, MBA_string filename, FileType format);

  /// Read the model from the source file to the destination object.
  virtual bool read();

  /**
   * If the supplied format name is not the empty string, look up the symbolic
   * constant in the "dictionary"; otherwise do the lookup with the file name
   * extension.
   */
  static FileType discover_file_type(const MBA_string& filename,
				     const MBA_string& format = "");

 private:

  /// What is in common among readXMPL(), readL2() and readL2bin().
  bool doRead(L2_file_reader& reader);

  /// Read a file in the XMPL format.
  bool readXMPL();

  /// Read a file in the L2 format.
  bool readL2();

  /// Read a file in the L2bin format.
  bool readL2bin();

 private:

  /// The model input file pathname.
  MBA_string file_;

  /// Symbolic constant designating the file type (XMPL, L2, L2bin).
  FileType format_;
  
  /// Type of an element of the type-string-to-symbolic-constant "dictionary".
  typedef String_key_dict_item<FileType> Type_item;

  /// "Dictionary" mapping file pathname extensions onto symbolic constants.
  static Type_item extension_dict[];

  /// "Dictionary" mapping file type strings onto symbolic constants.
  static Type_item format_dict[];
};

#endif
