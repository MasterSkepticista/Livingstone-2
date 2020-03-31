/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_strip.h,v 1.7 2001/12/06 20:10:41 lbrown Exp $ */
#ifndef xlate_strip_HEADER
#define xlate_strip_HEADER

#include <readers/common.h>
#include <readers/L2_file_translator.h>

class L2rTransition;

/**
  * Strip debugging symbols from the incoming L2_file.
  * Do this by replacing dbg_L2rEnumeration objects with L2rEnumeration
  * objects.
  */

class xlate_strip : public L2_file_translator {
 public:
  /// Specify the source and destination L2_file objects.
  xlate_strip(const L2_file *source = 0, L2_file *destination = 0) :
    L2_file_translator(source, destination) { }

 protected:
  /// Convert dbg_L2rEnumeration to L2rEnumeration, merging opportunistically.
  virtual void copy_all_enums();

  /// Convert dbg_L2rVariable to L2rVariable.
  virtual void copy_all_vars();

  /// Convert dbg_L2rClause to L2rClause.
  virtual void copy_all_clauses();

  /// Convert dbg_L2rTransition to L2rTransition.
  virtual L2rTransition *copy_transition(const L2rTransition*,
					 bool add_to_dest = true);
};

#endif
