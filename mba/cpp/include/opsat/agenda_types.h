/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: agenda_types.h,v 1.33 2004/01/26 20:13:45 lbrown Exp $ */

#ifndef AGENDA_TYPES_H
#define AGENDA_TYPES_H

#include <conflict_db/assignment.h>
#include <conflict_db/candidate.h>

/**
 * An agenda element which computes the weight of its corresponding Candidate
 * by summing the weight of the individual Assignments. It calls a Candidate
 * function to do this for the "candidate_prefix" so that it doesn't have to
 * know anything about the 'Candidate' implementation.
 * \todo The weight is public; make it protected and supply accessors.
 */

class CBFSElement : public Pooled {
public:
  /**
   * Constructor for empty element, the weight of which must be specified by
   * calling set_empty_element_weight() (default 0).
   */
  CBFSElement() : pCurrentAssignment(NULL) { }

  /**
   * Initialize from the supplied Candidate.
   * \param candidate reference to the Candidate which is the seed
   */
  CBFSElement(const Candidate& candidate) :
    candidate_prefix(candidate), pCurrentAssignment(NULL) { }

  /**
   * Initialize from the supplied Candidate, Assignment and set of remaining
   * Assignments.
   * \param candidate reference to the Candidate which becomes the seed
   * \param pAssignment pointer to the "other" Assignment
   * \param remainder list of Assignment pointers
   */
  CBFSElement(const Candidate& candidate,
	      const Assignment* pAssignment,
	      const Assignment::ListOfAssignment& remainder) :
    candidate_prefix(candidate), pCurrentAssignment(pAssignment),
    remainingAssignments(remainder)
  { }

  /**
   * Nothing special to do.
   */
  virtual ~CBFSElement() { }

  /**
   * Pop the front element from the list of remainingAssignments and save it.
   */
  void shift_assignment() {
    pCurrentAssignment = remainingAssignments.front();
    remainingAssignments.pop_front();
  }

  /**
   * Is the set of remaining Assignments empty?
   */
  bool remaining_assignments_empty() const {
    return remainingAssignments.empty();
  }

  /**
   * Computes the element weight using sum operator, with the side effect of
   * setting the weight.
   * It is the sum of the empty element weight, the Candidate prefix weight,
   * and the weight of the most-recently-popped Assignment (if any).
   * \return the computed weight
   */
  Assignment::weight_t compute_element_weight() const {
    return
      CBFSElement::empty_element_weight +
      candidate_prefix.get_weight() +
      ((pCurrentAssignment == NULL) ? 0 : pCurrentAssignment->get_weight());
  }

  /**
   * Generates complete Candidate from candidate_prefix and current Assignment.
   * \param candidate reference to a partially-completed Candidate
   */
  void generate_candidate(Candidate& candidate) const {
    candidate = candidate_prefix;
    if (pCurrentAssignment != NULL) { candidate.add(pCurrentAssignment); }
  }

  /**
   * Get the weight of the empty element.
   */
  Assignment::weight_t get_empty_element_weight() const {
    return CBFSElement::empty_element_weight;
  }

  /**
   * Set the weight of the empty element.
   * \param empty_weight the weight of the empty element
   */
  static void set_empty_element_weight(Assignment::weight_t empty_weight) {
    CBFSElement::empty_element_weight = empty_weight;
  }

  /// Is the argument greater in weight than this?
  bool operator> (const CBFSElement& obj2) const {
    return (this->weight > obj2.weight);
  }

  /// Is the argument less in weight than this?
  bool operator< (const CBFSElement& obj2) const {
    return (this->weight < obj2.weight);
  }

  /**
   * Get the accumulated weight of the element.
   */
  Assignment::weight_t get_weight() const { return weight; }

  /**
   * Set the accumulated weight of the element.
   * \param weight the accumulated weight of the element.
   */
  void set_weight(Assignment::weight_t weight) { this->weight = weight; }

  /**
   * An Assignment is "useful" to a Candidate if (1) it is not its Variable's
   * default Assignment, (2) it is consistent with the Candidate's other
   * Assignments and (3) the Candidate does not already contain the Assignment.
   * \param pAssignment reference to an Assignment
   * \param candiate reference to a Candidate
   * \return whether the Assignment is usefult to the Candidate
   */
  static bool is_assignment_useful(const Assignment& assignment,
				   const Candidate& candidate) {
    return 
      // The Assignment is not its Variable's default Assignment
      !(assignment.get_variable()->is_default(assignment)) &&
      // The Assignment is consistent with the Candidate's other Assignments
      candidate.consistent_assignment(assignment) &&
      // The Candidate does not already contain the Assignment
      !candidate.contains(&assignment);
  }

  /**
   * Return the "prefix" Candidate
   * \return a reference to the "prefix" Candidate
   */
  const Candidate& get_prefix() const { return candidate_prefix; }

  /**
   * Return the "current" Assignment
   * \return a const pointer to the "current" Assignment
   */
  const Assignment* get_current_assignment() const {
    return pCurrentAssignment;
  }

  /**
   * Return the "remaining" Assignments
   * \return a reference to the "remaining" Assignment
   */
  Assignment::ListOfAssignment& get_remaining_assignments() {
    return remainingAssignments;
  }
  const Assignment::ListOfAssignment& get_remaining_assignments() const {
    return remainingAssignments;
  }

private:

  /// The weight of the all-default Candidate.
  static Assignment::weight_t empty_element_weight;

  /**
   * The Candidate (nondefault set of Assignments) from which this element is
   * initialized. Note that it is a copy, not a pointer or reference.
   */
  const Candidate candidate_prefix;

  /// The other Assignment from which this element is initialized.
  const Assignment* pCurrentAssignment;

  /**
   * The remaining Assignments that can be used to construct elements of
   * lesser (greater) weight.
   */
  Assignment::ListOfAssignment remainingAssignments;

  /// The accumulated weight of the element
  Assignment::weight_t weight;
};

#endif


