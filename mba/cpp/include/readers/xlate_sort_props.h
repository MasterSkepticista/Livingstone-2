/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_sort_props.h,v 1.5 2001/12/06 20:10:41 lbrown Exp $ */
#ifndef xlate_sort_props_HEADER
#define xlate_sort_props_HEADER

#include <readers/common.h>
#include <readers/L2_file_translator.h>

/**
 * Sort L2rPropositions in the source L2_file such that the positive ones are
 * first, negative second, equalities (same) next, and inequalities (diff)
 * last.
 * Something of the sort is required for the to_l2 and to_bin writers.
 */

class xlate_sort_props : public L2_file_translator {
 public:
  /// Specify the source and destination L2_file objects.
  xlate_sort_props(const L2_file *pSource = 0, L2_file *pDestination = 0)
    : L2_file_translator(pSource, pDestination) { }

  /**
   * Tally L2rProposition objects in the L2_file object according to whether
   * they are
   * - same      (variable1 == variable2),
   * - different (variable1 != variable2),
   * - positive  (variable  == value) or
   * - negative  (variable  != value).
   */
  static void count_props(const L2_file *, 
			  unsigned& npos , unsigned& nneg, 
			  unsigned& nsame, unsigned& ndiff);

  /**
   * Check that all L2rProposition objects in the L2_file object are in the
   * order produced by copy_all_props(). If any is out of order, throw an
   * exception.
   */
  static void check_sorted(const L2_file *,
			   unsigned npos , unsigned nneg,
			   unsigned nsame, unsigned ndiff);

 protected:

  /**
   * Copy the L2rPropositions from the source l2_file object to the destination
   * l2_file object in order of L2rProposition type:
   * - positive  (variable  == value),
   * - negative  (variable  != value),
   * - same      (variable1 == variable2) and
   * - different (variable1 != variable2).
   */
  virtual void copy_all_props();
};

#endif
