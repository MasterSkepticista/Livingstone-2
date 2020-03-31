/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Implementation of the LTMS algorithms
// $Id: ltms.h,v 1.60 2002/08/07 18:14:44 lbrown Exp $

#ifndef LTMS_H
#define LTMS_H

#include <tms/ptheory.h>

/**
 * An LTMS represents a propositional Theory. A Theory is a set of Clauses,
 * where each Clause is a disjunction of literals.  A literal is either a 
 * Proposition or the negation of a Proposition. A Proposition is a primitive
 * token that can be either true or false. A positive literal is true if and
 * only if the corresponding Proposition is true. A negative literal is true
 * if and only if the corresponding Proposition is false.  A Clause is
 * satisfied if and only if at least one of its literals is true. A Theory is
 * satisfiable if and only if there is a truth assignment to all Propositions
 * such that every Clause is satisfied.
 *
 * LTMS implements unit propagation, also called called Boolean Constraint
 * Propagation. The idea is that whenever there is a unit clause, i.e., one
 * with exactly one unassigned literal and all other false literals, the
 * unassigned literal must be assigned true so that the Clause may be
 * satisfied. The LTMS caches these Propagations.
 *
 * When we are forced to assign a Proposition to TRUE or FALSE in order to
 * continue unit propagation, we check if there are any other unit Clauses
 * that contain the Proposition in the opposite sign.  If so, that Clause
 * and the theory are unsatisfiable. Presumably the LTMS user will then use
 * add(formula) and remove(formula) to search for a consistent theory.
 *
 * This LTMS allows one to add or eliminate Variables and Clauses in a way
 * that is not part of the traditional TMS functionality. This functionality
 * makes the LTMS better suited for tracking dynamic problems, such as the
 * evolution of a system over time.
 *
 * The methods are:
 *
 * move_support(prop, clauses, new_prop)
 *
 *   This method causes new_prop to take the place of prop as far as the
 *   passed Clauses are concerned.
 *   The method moves Clauses from prop to new_prop.  This means that
 *   occurrences of prop in the passed Clauses are replaced by new_prop.
 *   In addition, if a Clause C in clauses is supporting prop and giving
 *   it its truth value, that support and truth value is given to new_prop.
 */

/* These do not seem to have been even declared, much less implemented.
 *
 * add_summary_clause(index, new_prop, move_support, new_clauses)
 *
 *   For each Proposition P in a time step after index that is currently
 *   supported by a Clause from index or before, a new Clause is added
 *   of the form new_prop -> P. If move_support() is true, the support
 *   is moved from P's old supporting Clause to the newly-introduced Clause.
 *   Conceptually, the new Clause summarizes the Clauses and Propositions
 *   that occur before the passed time step, index.
 *   The new Clauses are added to the list new_clauses.
 *
 * delete_theory(start_index, end_index)
 *
 *   Removes the Proposition and Clauses whose indices lie between the passed
 *   indices, inclusive. Clauses referencing Propositions inside the range are
 *   deleted as well, to ensure there are no dangling links.
 *   Finally, it assumes that if a removed Clause does not support a
 *   Proposition outside of the removed range.
 */

class LTMS : public Ptheory {
 public:

  //----------------------------------------------------------------------
  // Typedefs
  //----------------------------------------------------------------------

  /**
   * A set of Clause pointers
   * \todo Unify with Ptheory::clause_list
   */
  typedef Array<Clause*, false> UnsatType;

  //----------------------------------------------------------------------
  // Constructors and destructors
  //----------------------------------------------------------------------

  /// Nothing special to do
  LTMS(): Ptheory() { }

  /// Nothing special to do
  virtual ~LTMS() { }

  //----------------------------------------------------------------------
  // Inference
  //----------------------------------------------------------------------

  /**
   * This method is the basic unit propagation and caching function.
   * It propagates the unit Clause fringe of the Theory until the fringe
   * becomes empty, or an inconsistency is detected.  If the fringe becomes
   * empty, then all Propositions have truth values that are "unit propagation
   * entailed" by the Theory, and the function returns true. Note that since
   * unit propagation is an incomplete inference procedure, the truth value of
   * some propositions can still be UNKNOWN.  If an inconsistency is detected
   * before the fringe is propagated away, the method returns false.
   * \return whether an inconsistency is detected before the fringe is
   * propagated away
   */
  virtual bool propagate();


  /**
   * This method is similar to propagate().
   *
   * The difference is that it does its work over multiple calls and sets the
   * consistent parameter rather than returning whether the Theory is
   * consistent.
   *
   * The value of out parameter "consistent" should be IGNORED unless the
   * return value is true.  It would be somewhat nicer to have propagate() and
   * partial_propagate() both return the consistency, but that would have been
   * awkward in this case.
   * \param iterations how many iterations to perform
   * \param consistent if propagation has finished, whether the theory is
   * consistent
   * \return whether all propagation has finished
   */
  virtual bool partial_propagate(size_t iterations, bool &consistent);


  /**
   * This function traces back the dependencies from a Clause, adding to out
   * parameter "result" the Clauses that support parameter "clause".
   *
   * It applies parameter "predicate" to each Clause in the trace. If predicate
   * returns true, then the Clause is recorded as part of the support and its
   * support is not recorded. If the predicate returns false, the Clause is
   * not recorded as part of the support, but we look for support in its
   * support.
   * \param clause the Clause to be explained
   * \param result the collected Clauses supporting the Clause
   * \param predicate a unary filter predicate on a Clause
   */
  virtual void get_clause_support(Clause& clause,
				  Clause::ListOfClause& result,
				  bool (*predicate)(const Clause&));

  /**
   * Adds to out parameter "result" the Clause that caused the Proposition to
   * be assigned its current value, and the Clauses that support that Clause.
   *
   * If we encounter a Clause that satisfies the predicate, it is returned as
   * a support. If the predicate does not satisfy the predicate, we look in
   * its support for that satisfy predicate are returned. This allows the
   * caller to retreive support in terms of some distinguished set of Clauses,
   * such as a set of assumptions or mode assignments.
   *
   * We could simply give Clauses an ASSERTION? data member and test it if
   * passing a function pointer is not allowed in flight applications.
   * \param proposition the Proposition to be explained
   * \param result the colected Clauses supporting the Proposition
   * \param predicate a unary filter predicate on a Clause
   */
  void get_proposition_support(Proposition& proposition,
			       Clause::ListOfClause& result,
			       bool (*predicate)(const Clause&)) {
    Clause* pSupportingClause = proposition.get_supported_by();
    if (pSupportingClause != NULL) {
      get_clause_support(*pSupportingClause, result, predicate);
    }
  }

  
  /**
   * This function adds to out parameter "conflict" the Clauses representing a
   * Conflict based on the current Theory. The function assumes that
   * propagation is complete. It uses one of the unsatisfied Clauses of Theory,
   * and traces back from it to find the Conflict. If no such unsatisfied
   * Clause exists, the function adds nothing to conflict and returns. On
   * each Clause in the dependency trace of the unsatisfiable Clause, the
   * function applies parameter predicate. If predicate returns true, the
   * Clause is added to out parameter "conflict". This allows the caller to add
   * only Clauses representing Assumptions to the Conflict.
   * \param conflict Slist of Clause pointers; this list is assumed to be
   * empty upon entry and is augmented to specify a Conflict
   * \param predicate a unary filter predicate on a Clause
   */

  virtual void last_conflict(Clause::ListOfClause& conflict,
			     bool (*predicate)(const Clause&));

  // ******************************************************************
  //
  // The following functionality is not traditional LTMS functionality
  //
  // ******************************************************************

  /**
   * Inserts a new intermediate Proposition, pNewProposition, in between
   * pProposition and a subset the Clauses that previously constrained it.
   * If pProposition was supported by one of these Clauses, that Clause now
   * supports pNewProposition.
   * This is not traditional LTMS functionality.
   * \param pProposition pointer to the already-supported Proposition
   * \param clauses Slist of pointers to Clauses that support the Proposition
   * \param pNewProposition pointer to proposition that is to receive support
   */
  void move_support(Proposition* pProposition,
		    Clause::ListOfClause& clauses,
		    Proposition* pNewProposition);


  /**
   * Finish destroying marked Clauses and Propositions.
   * After this call, they are removed from the TMS and are deallocated.
   * This is the sweep of a mark-and-sweep.
   */
  virtual void finish_destroying();

  //----------------------------------------------------------------------
  // Modifying the clauses in the LTMS
  //----------------------------------------------------------------------

  /**
   * Adds back to the Theory a Clause that was previously removed or created
   * without being entered.
   * It also correctly updates the fringe and unsatisfied Clause lists.
   * \param clause the Clause to add to the theory
   */
  virtual void add(Clause& clause);

  /**
   * Temporarily removes the Clause from consideration in the Theory and undoes
   * any propagation that was supported by the Clause. It also correctly
   * updates the fringe and unsatisfied Clause lists.
   *
   * Adding and removing Clauses allows systems that use the LTMS to perform
   * searches for Clauses that make the theory consistent. A Clause might
   * represent a component mode for a diagnosis problem, for example.
   * \param clause the Clause to remove from the theory
   */
  virtual void remove(Clause& clause);

  /**
   * Begin iterator for the Clauses directly involved in a Conflict.
   * \return begin iterator for the unsatisfied Clause set
   */
  UnsatType::iterator begin_unsatisfied() const { return unsatisfied.begin(); }

  /**
   * End iterator for the Clauses directly involved in a Conflict
   * \return end iterator for the unsatisfied Clause set
   */
  UnsatType::iterator end_unsatisfied()   const { return unsatisfied.end(); }

 protected:
  //----------------------------------------------------------------------
  // To support inference
  //----------------------------------------------------------------------

  /**
   * Assigns the truth value of the Proposition and updates Clauses that are
   * satisfied or revealed to be unsatisfiable given the assignment and the
   * current theory.
   * Also updates the fringe and support bookkeeping.
   * \param proposition the Proposition whose truth value is being assigned
   * \param is_true is the truth assignment to be given to the Proposition
   * \param clause the fringe Clause that contained the Proposition and forced
   * its assignment during unit propagation.
   */
  virtual void assign(Proposition& proposition,
		      bool becomingTrue,
		      Clause& clause);

  /**
   * Unassigns the Proposition, and propagates the unassignment one level by
   * putting affected propositions onto the undo_list.
   * \param proposition the Proposition whose truth value is being unassigned
   */
  virtual void unassign(Proposition& proposition);

  /**
   * Adds the Clause to the fringe of its Theory. The function makes sure that
   * if the Clause is already in the fringe then it isn't added in again.
   * \param clause the Clause being inserted into the fringe
   */
  void insert_into_fringe(Clause& clause);

  /**
   * Removes the Clause from the fringe of the Theory.  Currently this merely
   * involves removing the Clause from either the volatile fringe or the
   * nonvolatile fringe. Note that if the Clause isn't actually in either
   * fringe, no error is signalled.
   * \param clause the Clause being removed from the fringe
   */
  void remove_from_fringe(Clause& clause);

  /**
   * Adds the Clause as a new unsatisfied Clause to its Theory.  If the Clause
   * is already in the set of unsatisfied Clauses, it is not added again.
   * \param clause the Clause being added into the unsatisified Clause set
   */
  void insert_into_unsat(Clause& clause);

  /**
   * Removes the Clause from the set of unsatisfied Clauses of its Theory.
   * If the Clause is not in the set of unsatisfied Clauses, no action is taken
   * and no warning or error is issued.
   * \param clause the Clause being removed from the unsatisified Clause set
   */
  void remove_from_unsat(Clause& clause);

  /**
   * Returns a Clause in the unit Clause fringe, if it exists, and NULL
   * otherwise.  Currently, the function prefers to return a Clause from the
   * non-volatile fringe if possible.  Otherwise it returns the first Clause
   * in the volatile fringe. The heuristic is that if a Clause is volatile,
   * i.e., often deleted and undeleted, we really don't want it to have many
   * consequences if we can support its consequences with a nonvolatile
   * Clause. Hence, whenever possible, we prefer to propagate from
   * non-volatile Clauses first.
   * \return an arbitrary Clause in the fringe
   */
  Clause* get_fringe_clause();

  /**
   * Add the Proposition to the list of Propositions to unassign next time
   * we call undo_propagations.
   * \param pProposition pointer to the Proposition to enqueue for unassignment
   */
  void enqueue_for_undo(Proposition* pProposition);

  /**
   * Unassign all the Propositions on the undo list, and recursively all the
   * Propositions whose value depends on those on the undo list.
   * This is the sweep of a mark-and-sweep.
   */
  void undo_propagations();

 protected:

  //----------------------------------------------------------------------
  // Initialization
  //----------------------------------------------------------------------

  /**
   * Re-initialize the LTMS.
   * Overrides Ptheory::initialize()
   */
  virtual void initialize();

 private:

  /**
   * Update Clauses that are satisfied by assigning this Proposition.
   * Update involves doing the following on Clauses that are not deleted:
   *  o decrement the number of unassigned literals
   *  o Set the Proposition as the satisfying Proposition of the Clause,
   *    unless it is already satisfied
   *  o If the Clause was in the fringe, remove it, since we just assigned its
   *    last Proposition.
   * Note that, if the Clause already has a satisfying Proposition, there is no
   * need to remove it from the fringe, since it can't be in the fringe.
   * Auxiliary to LTMS::assign()
   * \param proposition reference to Proposition being unassigned
   * \param whether the Proposition is being assigned to be true
   */

  void assignUpdateSameSense(const Proposition& proposition,
			     bool becomingTrue);

  /**
   * Update Clauses in which the Proposition being assigned occurs with the
   * opposite sense.  We are assigning to the Proposition the truth value that
   * is the opposite of what each Clause contains.
   * This involves doing the following on Clauses that are not deleted:
   *  o decrement the number unassigned
   *  o if it does not have a satisfying Proposition, then
   *    o if it was in the fringe, it will now be unsatisfied
   *      Being in the fringe meant that it had one unassigned Proposition.
   *	  This was just assigned to the opposite truth value as was needed,
   *      so the Clause can now not be satisfied.
   *    o otherwise, see if it needs to be inserted into the fringe
   * Auxiliary to LTMS::assign()
   * \param proposition reference to Proposition being unassigned
   * \param whether the Proposition is being assigned to be true
   */

  void assignUpdateOppositeSense(const Proposition& proposition,
				 bool becomingTrue);

  /**
   * Update Clauses which were satisfied by the unassigned Proposition.
   * Update involves (do this only for Clauses that are not deleted):
   *   o Increment the number of unassigned literals.
   *   o Find an alternate support if necessary.  If no alternate support
   *     is found, see if it is now a fringe clause.
   * Auxiliary to LTMS::unassign()
   * \param proposition reference to Proposition being unassigned
   * \param wasTrue whether the Proposition was true before being unassigned
   */
  void unassignUpdateSameSense(const Proposition& proposition,
			       bool wasTrue);

  /**
   * Update Clauses which were not satisfied by the unassigned Proposition.
   * Update involves (do this only for Clauses that are not deleted):
   *   o Increment the number of unassigned literals.
   *   o If the Clause was an unsatisfied Clause, it is now in the fringe.
   *   o If the Clause was a fringe Clause, it is now not in the fringe.
   *   o If the Clause supported a Proposition, then need to queue that
   *     Proposition for loss of support.
   * Auxiliary to LTMS::unassign()
   * \param proposition reference to Proposition being unassigned
   * \param wasTrue whether the Proposition was true before being unassigned
   */

  void unassignUpdateOppositeSense(const Proposition& proposition,
				   bool wasTrue);

  /**
   * For each Clause in the list, the Clause used to support pOldProposition;
   * make is support pNewProposition
   * instead.
   * Auxiliary to move_support()
   * \param pOldProposition the Proposition already supported
   * \param pNewProposition the Proposition receiving support
   * \param transferredClauses reference to the set of Clauses to transfer
   * \param losing_assignment whether the supporing Clause is one of those
   * being transferred
   */
  void transfer_clauses(Proposition* pOldProposition,
			Proposition* pNewProposition,
			Clause::ListOfClause& transferredClauses,
			bool losing_assignment);

  /**
   * This inner class implements a fringe, a set of Clauses such that it has
   * exactly one false Proposition.
   * The LTMS has two fringes, for volatile and non-volatile Clauses.
   * We need fast access to the fringe, so we create our own stack-like data
   * structure (only stack-like because it has constant-time remove of an
   * element in the middle of the stack, which changes the order).
   */
  class Fringe : private Array<Clause*, false> {
  public:
    /// Initialize the size of the Fringe
    Fringe();

    /// Nothing special to do
    ~Fringe() { }

    /**
     * Put the Clause at the end of the Fringe.
     * \param pClause pointer to the Clause to push
     */
    void push(Clause* pClause) {
      pClause->set_in_fringe(size());
      super::push(pClause);
    }

    /**
     * Remove the first Clause from this Fringe and return it.
     * \return a pointer to the popped Clause
     */
    Clause *pop() {
      Clause *pClause = super::pop();
      pClause->take_from_fringe();
      return pClause;
    }

    /**
     * Remove the specified Clause from this Fringe.
     * \param pointer to the Clause to remove from the fringe
     */
    void remove(Clause* pClause);

    /**
     * Remove from this Fringe all Clauses that are marked as destroying.
     * This is the sweep of a mark-and-sweep.
     */
    void remove_destroying();

    /// Remove all elements from this Fringe.
    void erase() { super::erase(); }

    /// Does this Fringe have no elements?
    bool empty() const { return size() == 0; }

  private:
    typedef Array<Clause*, false> super;
  };

  /**
   * Set of non-volatile, undeleted Clauses that have exactly one literal
   * being false.
   */
  Fringe fringe;

  /**
   * Set of volatile, undeleted Clauses that have exactly one literal being
   * false.
   */
  Fringe volatile_fringe;
  
  /**
   * Set of Clauses that conflict -- that is, all the Propositions in the
   * Clause are false.
   */
  UnsatType unsatisfied;

  /**
   * Stack of propositions being unassigned; this allows us to avoid recursion.
   */
  Array<Proposition*, false> undo_list;

#if 0
  // The following are not implemented and thus far not neeeded.
    
  // Permanently deletes the Proposition from the Theory. The Clauses that
  // contain the Proposition are not updated, the assumption being that they
  // are to be deleted as well.
  void delete_propostions_without_update(Proposition& proposition);

  // Permanently deletes the Clause from the Theory without properly updating
  // the Propositions it contains. The fringe is properly updated. This method
  // should only be used if the Propositions in the passed Clause are deleted
  // from the Theory before any Propagation is done. This does not delete the
  // Clause object from memory.
  void delete_clause_without_update(Clause& clause);
#endif
};

#endif // LTMS_H

