/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include "conflict_db/assumption.h"

Assumption::Assumption(Conflict_db &db, unsigned nvalues,
		       const Assignment::ListOfWeight& weights,
		       Theory::time_step_t time_step, unsigned id) :
  Assignable(db, nvalues, time_step, id),
  // The default assignment is the first in the list.
  default_assignment(*assignments_begin())
{
  L2_assert((unsigned)weights.size() == nvalues,
            L2_argument_error,
            (MBA_string(weights.size()) + " weights for "
             + MBA_string(nvalues) +
             " propositions in Assumption::Assumption"));

  // Do two things in this loop:
  // (1) the real work: set the weights
  // (2) check that the weights are sorted in increasing order

  Assignment::ListOfWeight::const_iterator wit = weights.begin();
  Assignment::weight_t last_weight = 0;
  for (iterator it = assignments_begin();
      it != assignments_end(); ++wit, ++it) {
    Assignment *pAssignment = *it;
    Assignment::weight_t weight = *wit;
    pAssignment->set_weight(weight); // set the weight

    L2_assert(weight >= last_weight,
	      L2_argument_error,
	      ("weights are unsorted"));
    last_weight = weight;
  }
}
