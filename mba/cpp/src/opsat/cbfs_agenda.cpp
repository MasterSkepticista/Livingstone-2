/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: cbfs_agenda.cpp,v 1.17 2002/10/01 19:06:55 lbrown Exp $ */

#include <opsat/cbfs_agenda.h>

/**
 * Initialize with empty Candidate element. "empty_element_weight" is the
 * weight of the empty or all-default candidate. The optional leastfirst
 * ordering is currently not implemented.
 */

CBFSAgenda::CBFSAgenda(Assignment::weight_t empty_element_wt,
		       Ordering order) :
  BinaryHeap<CBFSElement>(order)
{
  CBFSElement::set_empty_element_weight(empty_element_wt);
  CBFSElement* element;
  if ( (element = new CBFSElement ) != NULL) {
    insert(element);
  }
}


/**
 * Separate agenda initialization function to initialize afresh without
 * reconstructing. It empties agenda and inserts newly-constructed elements
 * corresponding to those in the argument. The Candidates are inserted in
 * inverse order as a convenience so that if they're all consistent and the
 * same weight they will be returned in the original order.
 */

void
CBFSAgenda::initialize_agenda(const Candidate::ListOfCandidate& candidates)
{
  clean();                         // Pops all agenda elements and frees memory
  if (!candidates.empty()) {
    Candidate::ListOfCandidate c2(candidates);// Copy the list
    c2.reverse();                    // Reverse the order
    while(!c2.empty()) {
      Candidate* pCandidate = c2.front();
      c2.pop_front();
      insert(new CBFSElement(*pCandidate));
    }
  } else {
    // Initialize with good ol' empty element
    insert(new CBFSElement());
  }
}


/**
 * This function expands the most recently popped element, and inserts the
 * resulting elements (at most two immediate successors) into the agenda.
 *
 * The function assumes that 'last_popped_element' is *not* currently in the
 * agenda. Expanding an element involves two things: (1) moving to the next
 * Assignment in the remainingAssignments data member; and (2) creating an
 * additional element whose Candidate is the union of the current Candidate
 * and the current Assignment, and whose currentAssignment and
 * remainingAssignments are constructed from the conflict database.
 */

void CBFSAgenda::expand_element(const Conflict* pConflict) {
  // Get a new list of Assignments which resolve sub-candidate Conflict, if
  // one exists
  if (pConflict != NULL) {
    // Generate a new Candidate by adding the current Assignment
    Candidate candidate;
    // Copy the prefix Assignments and current Assignment to the Candidate
    last_popped_element->generate_candidate(candidate);
    
    // Get a list of new Assignments which may resolve the candidate Conflict
    Assignment::ListOfAssignment assignments;
    get_new_assignments(assignments, candidate, pConflict);

    if (!assignments.empty()) {
      // a new element should be constructed including the new Assignments
      Assignment* pAssignment = assignments.front();
      assignments.pop_front();
	
      // weight is computed upon construction
      CBFSElement *pElement =
	new CBFSElement(candidate, pAssignment, assignments);
      insert(pElement);// NOTE SHOULD INSERT WITHOUT DUPLICATES !!
    }
  }

  // Next check if an element can be constructed out of the remaining
  // Assignments of the element being expanded

  // Ensure that the first Assignment of the remaining Assignments is
  // consistent with the prior Assignments
  pop_useless_assignments(last_popped_element->get_remaining_assignments(),
			  last_popped_element->get_prefix());

  // now that the passed element has been processed, it may be reused
  if ( !(last_popped_element->remaining_assignments_empty()) ) {
    last_popped_element->shift_assignment();
    insert(last_popped_element); // NOTE SHOULD INSERT WITHOUT DUPLICATES !!
  } else {
    delete last_popped_element;
  }
}


/**
 * Pops the best-weight element off the agenda and copies the corresponding
 * Candidate to the argument Candidate.
 * CBFSAgenda stores the pointer to the most recently popped element.
 */

void CBFSAgenda::pop_candidate(Candidate &candidate) {
  last_popped_element = pop();
  last_popped_element->generate_candidate(candidate);
}


/**
 * Generates a list of Assignments which may resolve the candidate Conflict.
 * This function adds to the Assignments list resulting from the smallest
 * unentailed Conflict of this element's Candidate. The returned list may be
 * empty. It ensures that the first element of the returned list of Assignments
 * is consistent with the sub-candidate.
 */

void CBFSAgenda::get_new_assignments(Assignment::ListOfAssignment& assignments,
				     const Candidate& candidate,
				     const Conflict *pConflict) const {
  assert(assignments.empty());
  // Get all candidate Assignments which entail the Conflict
  if (pConflict != NULL) {
    pConflict->resolving_assignments(assignments);
    if (!assignments.empty()) {
      // Sort Assignments in increasing order of weight
      // Add them to a heap and pop them off; not most efficient, but it works
      BinaryHeap<Assignment> workheap(get_order());
      while (!assignments.empty()) {
	workheap.insert(assignments.front());
	assignments.pop_front();
      }
      while (!workheap.empty()) { assignments.push_back(workheap.pop()); }

      // Make sure first Assignment is consistent with passed Candidate
      pop_useless_assignments(assignments, candidate);
    }
  }
}


/**
 * This function pops off the initial Assignments in the assignments list
 * argument that are inconsistent with Assignments in the Candidate, or that
 * are default values.
 * An Assignment is considered to be inconsistent with a prior Candidate
 * Assignment if both Assignments are values of the same Variable and are not
 * the same Assignment. The reason for popping off default values is that the
 * search is in the space of faults. It also ensures the Assignment is not
 * already in the Candidate.
 * \param assignments a sequence of Assignments from which useless ones in theN
 * front will be removed
 * \param candidate a Candidate against which the Assignments are tested
 */

void
CBFSAgenda::pop_useless_assignments(Assignment::ListOfAssignment& assignments,
				    const Candidate& candidate) const {
  while (!assignments.empty()) {
    const Assignment* const pAssignment = assignments.front();
    if (CBFSElement::is_assignment_useful(*pAssignment, candidate)) { return; }
    else { assignments.pop_front(); }
  }
}
