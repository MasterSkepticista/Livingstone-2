/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_remove_duplicates.h,v 1.4 2001/12/06 20:10:40 lbrown Exp $ */
#ifndef xlate_remove_duplicates_HEADER
#define xlate_remove_duplicates_HEADER

#include <readers/common.h>
#include <readers/L2_file_translator.h>

/**
 * Optimize the source L2_file by removing L2rClauses and L2rPropositions
 * that are logically equivalent.
 *
 * Step 1: Map over only one of a set of duplicate propositions, where two
 *         propositions are duplicates if they have the same variable and
 *         value or otherVar.
 *         and don't map propositions that have no effect (like v1=v1)
 * Step 2: Map over only one of a set of duplicate clauses, where two clauses
 *         are duplicates if all their propositions map to the same thing.
 *         Make sure to remove duplicates within a clause.
 * Step 3: Map over transitions, making sure not to have duplicate clauses.
 *
 * Everything else is unchanged.
 */

class xlate_remove_duplicates : public L2_file_translator {
 public:
  /// Specify the source and destination L2_file objects.
  xlate_remove_duplicates(const L2_file *source = 0, L2_file *destination = 0);

 protected:
  /// Copy, with optimization, L2rPropositions from source to destination.
  virtual void copy_all_props();

  /// Copy, with optimization, L2rClauses from source to destination.
  virtual void copy_all_clauses();

 private:
  // Declared in the *.cpp file.
  class Hashable_prop;

  // Declared in the *.cpp file.
  class Hashable_clause;
};

#endif
