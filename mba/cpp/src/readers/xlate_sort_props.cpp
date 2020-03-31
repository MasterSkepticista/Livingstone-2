/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_sort_props.cpp,v 1.4 2001/11/16 19:32:38 lbrown Exp $ */
#include <readers/xlate_sort_props.h>

#include <readers/L2_file.h>
#include <readers/proposition.h>

// Auxiliary to copy_all_props()

void xlate_sort_props::count_props(const L2_file * file,
				   unsigned& npos , unsigned& nneg,
				   unsigned& nsame, unsigned& ndiff) {
  L2_assert(file != NULL,
	    L2_writer_error,
	    ("NULL L2_file"));
  npos = nneg = nsame = ndiff = 0;
  for (unsigned i = 0; i < file->nprops(); ++i) {
    const L2rProposition *p = file->getProp(i);
    if (p->isEquality()) {
      if (p->isPositive()) { nsame++; }
      else                 { ndiff++; }
    } else {
      if (p->isPositive()) { npos++;  }
      else                 { nneg++;  }
    }
  }
  L2_assert(file->nprops() == (npos + nneg + nsame + ndiff),
	    L2_writer_error,
	    ("Tallies don't add up"));
}


// An API method

void xlate_sort_props::check_sorted(const L2_file * file,
				    unsigned npos , unsigned nneg,
				    unsigned nsame, unsigned ndiff) {
  L2_assert(file != NULL,
	    L2_writer_error,
	    ("NULL L2_file"));
  unsigned i = 0;
  {
    for (unsigned idx = 0; idx < npos ; ++i, ++idx) {
      const L2rProposition *p = file->getProp(i);
      L2_assert(p->isPositive() && !p->isEquality(), L2_writer_error,
                ("propositions are ill-sorted at index " + MBA_string(i)));
    }
  }
  // assert(i == npos);
  {
    for (unsigned idx = 0; idx < nneg ; ++i, ++idx) {
      const L2rProposition *p = file->getProp(i);
      L2_assert(!p->isPositive() && !p->isEquality(), L2_writer_error,
                ("propositions are ill-sorted at index " + MBA_string(i)));
    }
  }
  // assert(i == (npos + nneg));
  {
    for (unsigned idx = 0; idx < nsame ; ++i, ++idx) {
      const L2rProposition *p = file->getProp(i);
      L2_assert(p->isPositive() && p->isEquality(), L2_writer_error,
                ("propositions are ill-sorted at index " + MBA_string(i)));
    }
  }
  // assert(i == (npos + nneg + nsame));
  {
    for (unsigned idx = 0; idx < ndiff ; ++i, ++idx) {
      const L2rProposition *p = file->getProp(i);
      L2_assert(!p->isPositive() && p->isEquality(), L2_writer_error,
                ("propositions are ill-sorted at index " + MBA_string(i)));
    }
  }
  // assert(i == (npos + nneg + nsame + ndiff));
}


// An API method

void xlate_sort_props::copy_all_props() {
  L2_assert(get_source() != NULL,
	    L2_writer_error,
	    ("NULL source L2_file"));
  L2_assert(get_dest() != NULL,
	    L2_writer_error,
	    ("NULL destination L2_file"));

  // The proposition counts
  unsigned npos, nneg, nsame, ndiff;
  // Tally the L2rProposition objects in the L2_file object
  count_props(get_source(), npos, nneg, nsame, ndiff);

  // Establish the order: positive (variable == value), negative
  // (variable != value), same (variable1 == variable2) and different
  // (variable1 != variable2)
  unsigned pos_idx  = 0;
  unsigned neg_idx  = npos;
  unsigned same_idx = npos + nneg;
  unsigned diff_idx = npos + nneg + nsame;

  // Allocate an array to hold the propositions
  dest()->allocProps(get_source()->nprops());

  for (unsigned i = 0; i < get_source()->nprops() ; ++i) {
    const L2rProposition *p = source()->getProp(i);
    if (p->isEquality()) {
      if (p->isPositive()) { copy_prop(p, same_idx++); }
      else                 { copy_prop(p, diff_idx++); }
    } else {
      if (p->isPositive()) { copy_prop(p, pos_idx++);  }
      else                 { copy_prop(p, neg_idx++);  }
    }
  }
}
