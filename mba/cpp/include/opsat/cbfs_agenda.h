/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: cbfs_agenda.h,v 1.43 2002/09/26 00:11:38 lbrown Exp $ */

#ifndef _CBFS_AGENDA_H
#define _CBFS_AGENDA_H

#include <conflict_db/candidate.h>
#include <opsat/agenda_types.h>
#include <opsat/binaryheap.h>

/// The order in which elements are stored in the agenda.
enum Ordering {GREATESTFIRST = 0, LEASTFIRST = 1};

/**
 * An agenda specialized to the conflict-directed best-first search.
 * The template parameter specifies the weight computation function via
 * instancing of the appropriate element class.
 *
 * An agenda is a list of weighted elements in increasing or decreasing order.
 * This particular agenda holds composite elements which contain weighted
 * items. It is implemented as a BinaryHeap.
 *
 * Note the agenda will be responsible for its own memory management; or,
 * rather, the agenda elements will come from a pool which will be destroyed
 * with the agenda
 */

class CBFSAgenda : private BinaryHeap<CBFSElement> {
public:

  /**
   * Constructor for an initially empty agenda.
   * \param empty_element_weight the weight of the empty element
   * \param order the order in which the agenda is kept; decreasing by default
   */
  CBFSAgenda(Assignment::weight_t empty_element_weight,
	     Ordering order = GREATESTFIRST);

  /// Pop and deallocate all agenda elements
  virtual ~CBFSAgenda() { clean(); }

  /**
   * Empties the agenda and inserts elements corresponding to supplied
   * Candidates. If no Candidates are supplied, insert an empty element.
   * \param candidates list of "seed" Candidates from which the agenda's
   * elements are initialized
   */
  void initialize_agenda(const Candidate::ListOfCandidate& candidates);

  /**
   * Computes one or two immediate successors of last_popped_element and
   * inserts into the heap.
   * \param pConflict pointer to a Conflict
   */
  void expand_element(const Conflict *pConflict);

  /**
   * Pops Assignments from list which are default, redundant, or inconsistent
   * with Assignments in the Candidate.
   * \param assignments list of Assignments; may be shortened by this function
   * \param candidate Assignments are popped if they are useless to this
   * Candidate
   */
  void pop_useless_assignments(Assignment::ListOfAssignment& assignments,
			       const Candidate& candidate) const;
  
  /**
   * Returns list of Assignments which may resolve the candidate Conflict.
   * \param newAssignments reference to a list of Assignments. Initially
   * empty, it may be populated by this function
   * \param candidate reference to Candidate which may be populated with
   * Assignments
   * \param pConflict pointer to Conflict that is the source of Assignments
   */
  void get_new_assignments(Assignment::ListOfAssignment& assignments,
			   const Candidate& candidate,
			   const Conflict *pConflict) const;

  /**
   * Returns the Candidate associated with the top element on the agenda
   * and pops the top element to be stored in last_popped_element.
   * \param candidate reference to an empty Candidate; its Assignments will
   * be populated by this function.
   */
  void pop_candidate(Candidate& candidate);

  /**
   * Compute the element's weight and insert it into the agenda.
   * Overridden to force weight computation before element is added --
   * slightly wasteful but due to comparison operators taking 'const' arguments
   * \param pElement pointer to a CBFSElement to insert into the agenda
   */
  void insert(CBFSElement* pElement) {
    // Compute the cost/rank/weight at the last possible moment
    pElement->set_weight(pElement->compute_element_weight());
    // Insert in increasing order of cost/rank/weight
    BinaryHeap<CBFSElement>::insert(pElement);
  }

  /// Is the agenda empty?
  bool empty_agenda() const { return empty(); }

  /// Pop and deallocate all agenda elements
  void clean() { while (!empty_agenda()) { delete pop(); } }

  /// Returns the weight of the top element
  Assignment::weight_t top_weight() const { return top()->get_weight(); }

private:
  /// Saves the element most recently popped from the agenda
  CBFSElement* last_popped_element;
};
#endif
