/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// An instance of OPSAT
// $Id: opsat.h,v 1.41 2002/09/26 00:11:38 lbrown Exp $
//

#ifndef OPSAT_H
#define OPSAT_H

#include <mba_utils/slist.h>
#include <opsat/cbfs_agenda.h>
#include <transition/T_system.h>
#include <transition/termination.h>

/**
 * A search which returns solutions based upon whether they are consistent
 * with some criterion.
 * Opsat_CBFS uses the CBFSAgenda's implementation of expand_element.
 */

class Opsat_CBFS {
 public:
  
  /**
   * No initial Candidates for constructor.
   * \param pT_system pointer to the T_system. Since this constructor is
   * invoked when the Tracker constructor is invoked, Tracker::t_system might
   * not exist.
   * \param best_weight weight of the empty Candidate
   * \param cutoff do not return any Candidates with weights greater than this
   * \param maximum_candidates do not return more Candidates than this
   */
  Opsat_CBFS(T_system *pT_system,
	     Assignment::weight_t best_weight,
	     Assignment::weight_t cutoff,
	     unsigned maximum_candidates);
  
  // perhaps should add constructors for no cutoff_weight and max_candidates
  
  /**
   * This allows explicit search initialization with multiple candidates.
   * \param agenda_inits list of "seed" Candidates
   */
  void initialize_search(const Candidate::ListOfCandidate& agenda_inits);

  /**
   * Returns "find_number" consistent Candidates in "consistent_candidates"
   * list which have a weight greater than cutoff_weight.
   * \param candidates an out parameter, a list of consistent Candidates found;
   * it is initially empty
   * \param find_number the maximum number of Candidates sought
   */
  void find_consistent_candidates(Candidate::ListOfCandidate& candidates,
				  unsigned find_number);
  
  // Various accessors

  /**
   * Set the weight at which search is terminated.
   * \param weight the weight at which search is terminated.
   */
  void set_cutoff_weight(Assignment::weight_t weight) {
    cutoff_weight = weight;
  }

  /**
   * Get the weight at which search is terminated.
   * \return the weight at which search is terminated.
   */ 
  Assignment::weight_t get_cutoff_weight() const { return cutoff_weight; }

  /**
   * Set the maximum number of Candidates to return
   * \param max the maximum number of Candidates to return
   */
  void set_max_candidates(unsigned max) { max_candidates = max; }

  /**
   * Get the maximum number of Candidates to return
   * \return the maximum number of Candidates to return
   */
  unsigned get_max_candidates() const { return max_candidates; }

  /**
   * Get the number of Candidates already searched.
   * \return the number of Candidates already searched
   */
  unsigned get_candidates_considered() const { return candidates_considered; }

  /**
   * Called within loop by 'find_consistent_candidates' until search is
   * terminated.
   * \return a pointer to a new consistent Candidate
   */
  Candidate* next_consistent_candidate();

  /**
   * Get the status of search -- whether it should be terminated prematurely
   * or not and why, if relevant.
   * \return one of EMPTY_AGENDA, MAX_OVERRUN, WEIGHT_CUTOFF, SEARCH_CONTINUE
   */
  SearchTermination terminate_search() const;

  /**
   * Since the T_system may not be properly established when the constructor
   * is called, this accessor allows it to be set when find-candidates is
   * performed.
   * Original note: Not defined since user also needs to reset LTMS, so this
   * is not a sensible function to use.
   */
  void set_t_system(T_system* pT_system) { this->pT_system = pT_system; }

 private:
  /**
   * Returns true if the testCandidate completely contains any of the
   * Candidates in the candidates list.
   * Q: how does this behave with respect to default values?
   * \param testCandidate reference to a Candidate
   * \param candidates reference to a list of Candidates
   * \return whether testCandidate is a superset of any element of candidates
   */
  bool superset_candidate(const Candidate& testCandidate,
			  const Candidate::ListOfCandidate& candidates) const;

  /**
   * Is the first State Variable Assignment a superset of the second State
   * Variable Assignment?
   * \param pSuperStateVariableAssignment an assignment to a State Variable
   * \param pSubStateVariableAssignment an assignment to a State Variable
   * \return whether pSuperStateVariableAssignment is a superset of 
   * pSubStateVariableAssignment
   */
  bool subsumes(const Assignment* const pSuperStateVariableAssignment,
		const Assignment* const pSubStateVariableAssignment) const;
  

  /**
   * Is the first Candidate a superset of the second Candidate?
   * This is not in class Candidate because class Candidate should not know
   * about class State_variable.
   * \param super a Candidate
   * \param sub a Candidate
   * \return whether super is a superset of sub, taking the State_variable into
   * account
   */
  bool supersetWithState(const Candidate& super, const Candidate& sub) const;

  /**
   * Return the Candidate's State Variable Assignment.
   * This is not in class Candidate because class Candidate should not know
   * about class State_variable.
   * \param candidate the Candidate
   * \return the Candidate's State Variable Assignment
   */
  const Assignment* getStateVariableAssignment(const Candidate& candidate)
    const;

 private:
  /**
   * The T_system which will be used for consistency queries.
   */
  T_system* pT_system;

  /**
   * The number of Candidates (both consistent and inconsistent) considered as
   * solutions so far
   */
  unsigned candidates_considered;
  
  /**
   * Candidate weight at which search will be prematurely terminated.
   */
  Assignment::weight_t cutoff_weight;

  /**
   * Maximum number of Candidates which will be checked for consistency in the
   * search.
   */
  unsigned max_candidates;

  /**
   * The agenda (ordered list of potential Candidates).
   */
  CBFSAgenda agenda;

};
#endif // OPSAT_H
