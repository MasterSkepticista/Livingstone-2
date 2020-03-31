/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: varkind.h,v 1.10 2004/01/28 22:02:23 lbrown Exp $ */
#ifndef varkind_HEADER
#define varkind_HEADER

#include <livingstone/L2_iostream.h>
#include <readers/common.h>

/**
 * Instead os subclassing L2rVariable, this type flag is used.
 */

enum VarKind {
  /// None of the others.
  vk_unknown,
  /// Corresponds to Observable.
  vk_observed,
  /// Corresponds to Command.
  vk_commanded,
  /// Corresponds to Transitioned.
  vk_mode
};

/**
 * Map the VarKind type flag to a print name
 * \param vk the VarKind the print name of which is sought
 * \return the print name string for vk
 */
inline MBA_string VarKind_to_string(VarKind vk) {
  switch(vk) {
  case vk_unknown:
    return "unspecified";
  case vk_observed:
    return "observed";
  case vk_commanded:
    return "commanded";
  case vk_mode:
    return "mode";
  default:
    return MBA_string(vk);
  }
}

#endif
