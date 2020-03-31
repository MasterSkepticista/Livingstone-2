/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: CandidateEqClass.h,v 1.4 2004/02/10 22:16:38 lbrown Exp $ */

#ifndef CANDIDATE_EQ_CLASS_H
#define CANDIDATE_EQ_CLASS_H

#include <conflict_db/assignable.h>
#include <conflict_db/assignable.h>
#include <conflict_db/candidate.h>
#include <mba_utils/array.h>
#include <mba_utils/pooled.h>

class T_system;

/**
 * A CandidateEqClass is an equivalence class of Candidates.
 * The Tracker partitions its Candidates into a set of CandidateEqClass.
 */

class CandidateEqClass : public Pooled {
  /*
   * A private inner class
   */ 
 private:
  /**
   * An immutable (variableID, timeStep, valueIndex) triple to unify the
   * handling of Assignments and Trajectory::Parts.
   */
  class Triple : public Pooled {
    public:
    Triple(unsigned variableID, unsigned timeStep, unsigned valueIndex) :
      variableID(variableID), timeStep(timeStep), valueIndex(valueIndex) { }
    const unsigned variableID;
    const unsigned timeStep;
    const unsigned valueIndex;
  };

  void createTriples(Slist<const Triple*>& triples,
		     const Candidate& candidate,
		     const T_system& t_system) const;
  void deleteTriples(Slist<const Triple*>& triples) const;

 public:

  /**
   * Make an equivalence class with one element.
   * \param candidate the initial element.
   */
  CandidateEqClass(const Candidate& candidate) { add(candidate); }

  /// Clear the list, but don't deallocate the elements; Conflict_db owns them.
  ~CandidateEqClass() { candidates.erase(); }

  /**
   * The equivalence relation that determines the partition.
   * Two Candidates are equivalent for the purpose of this equivalence class 
   * if (1) One of the Assignments is for the same Variable ID and value but
   * for different time steps and (2) all other Assignments are identical
   * (that is, same Variable ID, same time step, same value).
   * This relation is reflexive and symmetric but not transitive.
   * \param candidate1 One of the two Candidates being compared
   * \param candidate2 The other of the two Candidates being compared
   * \return whether the two Candidates are "equivalent" -- differing only
   * in the time step of one assignment
   */
  bool isEquivalent (const Candidate& candidate1,
		     const Candidate& candidate2) const;
  bool isEquivalent (const Slist<const Triple*>& ct1,
		     const Slist<const Triple*>& ct2) const;

  /**
   * Does the Candidate belong in the equivalence class?
   * \param candidate The Candidate being considered for membership
   * \param t_system a reference to the T_system that contains the Candidate
   * \return whether the Candidate should be added to the equivalence class
   */

  bool CandidateEqClass::belongs(const Candidate& candidate,
				 const T_system& t_system) const;

  /**
   * Assure that the equivalence class contains the Candidate.
   * \param candidate The candidate being added to the equivalence class
   * \return whether the equivalence class was changed
   */
  bool add(const Candidate& candidate) {
    if (!contains(candidate)) { candidates.push(&candidate); return true; }
    else { return false; }
  }

  /// Return the number of Candidate elements in the equivalence class.
  unsigned size() const { return candidates.size(); }

  /**
   * Return the ith element if it exists; otherwise NULL.
   * \param i The index of the Candidate
   * \return a pointer to Candidate i if it exists; otherwise NULL
   */
  const Candidate* get(unsigned i) const {
    return (i < size()) ? candidates[i] : 0;
  }

 protected:

  /**
   * Does the equivalence class contain the Candidate?
   * \param candidate The Candidate being tested
   * \return whether the equivalence class contains candidate
   */

  bool contains(const Candidate& candidate) const;

 private:

  /// The set of Candidates in the equivalence class.
  Array<const Candidate*, false> candidates;

};

#endif
