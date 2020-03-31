/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: candidate.h,v 1.55 2004/02/10 22:16:36 lbrown Exp $

#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <conflict_db/conflict.h>
#include <mba_utils/array.h>

/**
 * A Candidate is a set of Assignments that, when added to the theory, make the
 * conflict data base consistent. The Tracker is responsible for keeping the
 * set of Candidates current.
 */

class Candidate : public Pooled {
 public:

  /**
   * An ordered collection of Candidate*
   */
  typedef Slist<Candidate *> ListOfCandidate;

  /**
   * Construct a Candidate with no Assignments, giving it a unique ID.
   */
  Candidate() : id(++global_id) { }

  /**
   * Copy constructor: construct a Candidate whose Assignments are those of
   * the argument.
   * \param candidate reference to a Candidate whose Assignments become those
   * of the constructed object
   */
  Candidate(const Candidate &candidate);

  /**
   * No subclasses ==> non-virtual dtor is ok.
   */
  ~Candidate() { }

  /**
   * Assignment operator; set the Assignments to those of the rhs.
   * \param rhs the right-hand side of the assignment statement
   * \return a reference to this (the left-hand side Candidate)
   */
  Candidate& operator=(const Candidate& rhs);

  /**
   * Add the Assignment to the Candidate.
   * There is no check whether the Candidate already contains the Assignment.
   * \param pAssignment pointer to the Assignment to add
   * \todo This method could change the weight of the Candidate by adding a
   * non-zero cost Assignment.  It should therefore ensure that the
   * weight-sorted list of Candidates in Conflict_db is still correct.
   */
  void add(const Assignment* pAssignment)  { assignments.push(pAssignment); }

  /**
   * Removes the first occurence of the Assignment from the Candidate (it's
   * useless for a Candidate to contain the same entry twice, but uniqueness
   * is not enforced). Returns true if the Assignment was found and removed.
   * \param pAssignment pointer to the Assignment to remove
   * \return whether the Assignment was found and removed
   * \todo This does not adjust the Conflict_db's weight-sorted list of
   * Candidates.
   */
  bool remove(const Assignment* pAssignment);

  /**
   * Remove Assignments with time < horizon, replacing them with the
   * state=s_i Assignment. Set the weight of that Assignment to the sum of
   * the old Assignments' weights.
   * \param horizon the horizon time step
   * \param state_eq_si reference to the Assignment for the State Variable
   */
  void truncate(Theory::time_step_t horizon, Assignment& state_eq_si);

  /**
   * Returns T if the two Candidates contain the same Assignments.
   * \param candidate reference to the Candidate being compared
   * \return whether this has the same Assignments as the argument
   */
  bool equivalent(const Candidate& candidate) const;

  /**
   * Assign each of the Assignments in the Candidate
   */
  void assign() const;

  /*
   * Return all Assignments in Candidate which involve the passed Conflict
   * \param pConflict pointer to a Conflict
   * \param conflictList an out parameter which will be populated with pointers
   * to the Assignments that involve the Conflict argument
   */
  void get_conflict_assignments(Conflict *const & pConflict,
				Slist<const Assignment *>& conflictList) const;

  /**
   * Returns the smallest Conflict which is not resolved
   * \return the smallest Conflict which is not resolved
   */
  const Conflict* smallest_unresolved_conflict() const;

  /**
   * Returns true if the argument Conflict is not resolved by at least one
   * Assignment.
   * \param pConflict a pointer to a Conflict
   * \return whether the Conflict remains unresolved by at least one Assignment
   */
  bool conflict_not_resolved(const Conflict *pConflict) const;

  /**
   * Exchanges the (assumed) Assignment old for fresh
   * \param fresh a pointer to an Assignment
   */
  void add_consistently(const Assignment *const & fresh);

  /**
   * Are the sub-Candidate's Assignments a subset of my Assignments?
   * \param pSubCandidate a pointer to a Candidate
   * \return whether pSubCandidate's Assignments are all Assignments of this
   */
  bool superset_of(Candidate *const pSubCandidate) const;

  /**
   * Return the number of Assignments
   * \return the number of Assignments in the Candidate
   */
  unsigned int size() const { return assignments.size(); }
  
  /**
   * Is the Assignment argument consistent with the Candidate's Assignments?
   * \param assignment a reference to an Assignment
   * \return whether the Assignment is consistent with the Candidate's other
   * assignments
   */
  bool consistent_assignment(const Assignment& assignment) const;

  /**
   * Does my set of Assignments contain the test Assignment?
   * \param pAssignment pointer to the Assignment being sought
   * \return whether the Candidate contains the Assignment
   */
  bool contains(const Assignment *pAssignment) const;

  /**
   * Return the Candidate's weight, some function of the Assignments' weights
   * \return the Candidate's weight
   */
  Assignment::weight_t get_weight() const { return get_weight(assignments); }

  /**
   * Return the combined weight for the set of Assignments
   * \param assignments an Array of pointers to Assignment
   * \return the weight for the collection of Assignments
   */
  Assignment::weight_t get_weight(const Array<const Assignment*, false>&
				  assignments)
    const;

  /**
   * Return the sum of the weights over all Assignments
   * \param assignments an Array of pointers to Assignment
   * \return the sum of the weights for the collection of Assignments
   */
  Assignment::weight_t sum_assignment_weights(const Array<const Assignment*,
					      false>&  assignments) const;

  /**
   * Return the Assignment at the specified index. If absent, return NULL.
   * \param index the index of the Assignment
   * \return the Assignment with the given index, or NULL if nonexistent
   */
  const Assignment* get_assignment(unsigned int index) const {
    return (index >= assignments.size()) ? 0 : assignments[index];
  }

  /**
   * Return the serial enumerator
   * \return the Candidate's unique ID
   */
  unsigned get_id() const { return id; }

private:
  /// The set of Assignments that define the Candidate
  Array<const Assignment*, false> assignments;

  /// Serial enumerator
  unsigned id;

  /// Serial enumerator generator
  static unsigned global_id;
};

#endif /* CANDIDATE_H */
