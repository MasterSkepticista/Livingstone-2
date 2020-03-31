/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// $Id: conflict_db.h,v 1.92 2003/01/16 21:26:50 lbrown Exp $

#ifndef CONFLICT_DB_H
#define CONFLICT_DB_H

#include <conflict_db/assumption.h>
#include <conflict_db/candidate.h>
#include <conflict_db/dependent.h>
#include <conflict_db/conflict.h>
#include <conflict_db/command.h>
#include <conflict_db/observable.h>
#include <tms/ltms.h>

#ifndef DISABLE_CDB_LISTEN
// Listener for the Conflict_db
class Conflict_db_listener;
#endif

/**
 * The container for Variables and the Conflicts with respect to the LTMS.
 * A Conflict Data Base is a set of Variables and a set of Conflicts, along
 * with an LTMS. Variables of various subtypes can be created.
 *
 * <p>The Conflict_db contains Assignable Variables and Dependent Variables
 * and a set of Clauses that constrain what values of the Variables are
 * consistent with one another.  Assignable Variables may be assigned by the
 * user.  Dependent Variables are constrained by the Clauses and the Assignable
 * Variables.</p>
 * 
 * <p>If the Assignments to the Assignable Variables and the Clauses allow the
 * Conflict_db to infer two different values for the same Variable, an
 * inconsistency results. Some of the Assignments must be changed in order to
 * eliminate the inconsistency.  A subset of the Assignable Variables are
 * Assumption Variables. The Assumptions are the Variables that a search
 * procedure will change to regain consistency. The other Assignable Variables
 * are used to represent quantities such as external observations that a
 * search procedure should not modify.</p>
 * 
 */

class Conflict_db : public Pooled {
  friend class Variable;

 public:

  /**********************************************************************
        Constructor/Destructor
   **********************************************************************/

  /**
   * When a new Conflict_db is created, establish that a Variable is inserted
   * at the beginning of the list.
   */
  Conflict_db(): insert_before(variables.begin()) { }

 /// Deallocate the Variable set and the Conflict set 
 virtual ~Conflict_db();

#ifndef DISABLE_CDB_LISTEN
 /**
  * Add the listener callback object
  * \param pConflict_db_listener pointer to the listener to add
  */
 void add_listener(Conflict_db_listener *pConflict_db_listener) {
   listeners_.push_front(pConflict_db_listener);
 }

 /**
  * Remove the listener callback object
  * \param pConflict_db_listener pointer to the listener to remove
  */
 void remove_listener(Conflict_db_listener *pConflict_db_listener) {
   listeners_.remove_first(pConflict_db_listener);
 }
#endif

 /**
  * Create and return an Assignable Variable.
  * \param domainCardinality the cardinality of the domain
  * \param time_step the time step of the Assignable Variable
  * \param variableID the ID of the Assignable Variable
  * \return a pointer to the newly-created Assignable Variable
  */
 // A pointer added to a list of Assignable Variables.
 Assignable *create_assignable(unsigned domainCardinality,
			       Theory::time_step_t time_step,
			       unsigned variableID) {
   Assignable* pAssignable = new Assignable(*this, domainCardinality,
					    time_step, variableID);
   registerAssignable(pAssignable);
   return pAssignable;
 }

 /**
  * Create and return a Dependent Variable.
  * \param domainCardinality the cardinality of the domain
  * \param time_step the time step of the Dependent Variable
  * \param variableID the ID of the Dependent Variable
  * \return a pointer to the newly-created Dependent Variable
  */
 Dependent  *create_dependent(unsigned domainCardinality,
			      Theory::time_step_t time_step,
			      unsigned variableID) {
   return new Dependent(*this, domainCardinality, time_step, variableID);
 }

 /**
  * Create and return a Command Variable. A Command is also an Assignable.
  * \param domainCardinality the cardinality of the domain
  * \param time_step the time step of the Command Variable
  * \param variableID the ID of the Command Variable
  * \return a pointer to the newly-created Command Variable
  */
 Command *create_command(unsigned domainCardinality,
			 Theory::time_step_t time_step,
			 unsigned variableID) {
   Command* pCommand = new Command(*this, domainCardinality, time_step,
				   variableID);
   // A Command is an Assignable
   registerAssignable(pCommand);
   return pCommand;
 }

 /**
  * Create and return an Observable Variable. An Observable is also an
  * Assignable.
  * \param domainCardinality the cardinality of the domain
  * \param time_step the time step of the Observable Variable
  * \param variableID the ID of the Observable Variable
  * \return a pointer to the newly-created Observable Variable
  */
 Observable *create_observable(unsigned domainCardinality,
			       Theory::time_step_t time_step,
			       unsigned variableID) {
   Observable* pObservable = new Observable(*this, domainCardinality,
					    time_step, variableID);
   // An Observable is an Assignable
   registerAssignable(pObservable);
   return pObservable;
 }

 /**
  * Create a Clause in the LTMS.
  * \param posPropositions Slist of positive Propositions
  * \param negPropositions Slist of negative Propositions
  * \param isVolatile whether the Clause is volatile
  * \param inTheory whether the Clause is placed in the theory
  * \return a pointer to the newly-created Clause
  * \todo It'd be nice to have a way to create 'constraints' that didn't
  * depend on creating a Clause from a list of Propositions, but used some
  * data types that were more or less agnostic w.r.t the internal
  * representation.
  */
 // Note the datum cannot be specified. This is because the Conflict_db uses
 // it to denote Assignments.

 Clause* create_clause(Proposition::ListOfProposition& posPropositions,
		       Proposition::ListOfProposition& negPropositions,
		       bool isVolatile,
		       bool inTheory) {
   return solver.create_clause(NULL, posPropositions, negPropositions,
			       isVolatile, inTheory);
 }

 /**
  * Returns a Conflict based upon the current state of the db, if any.
  * The discovered Conflict is also added to the list of known Conflicts
  * found in any previously-explored db state.
  * \param tag a datum meaningful to the client
  * \return a const pointer to an arbitrary Conflict
  */
 const Conflict* conflict(unsigned tag = 0);

 /**
  * Propagate the effects of changes throughout the system.
  * Unlike conflict(), if there is an inconsistency it does not return the
  * Conflict that is causing the inconsistency.
  * \return whether the current state of the Conflict_db is consistent
  */
 bool propagate() { return solver.propagate(); }

 /**
  * Return a pointer to the LTMS
  * \return a pointer to the LTMS
  */
 LTMS* get_solver() { return &solver; }

 /**
  * Copies the current Assignment to all Assignable Variables to a safe
  * location so they can be modified to explore hypotheses.  restore() brings
  * back the stored values. Note that subsequent calls to save() will
  * overwrite the previously-saved values (that is, this is not a multi-level
  * undo).
  */
 void save();

 /// Re-assigns the Assignment values stored by save()
 void restore();

 /**
  * Assigns all Assumptions to their best-weight (nonfailure) values.
  * Assumes this is the first value in the domain -- need to ensure when
  * Assumptions are created, their Assignment lists are sorted by weight.
  */
 void restore_assumption_defaults();

 /**
  * Puts the new Assignable in the appropriste lists.
  * \param pAssignable pointer to a new Assignable
  */
 void registerAssignable(Assignable* pAssignable) {
   assignables.push_front(pAssignable);
 }

 /**
  * Puts the new Assumption in the appropriate lists.
  * \param pAssumtion pointer to a new Assumption
  */
 void registerAssumption(Assumption* pAssumption) {
   assumptions.push_front(pAssumption);
   // An Assumption is an Assignable
   registerAssignable(pAssumption);
 }

 // ------------------------------------------------------------------
 // Accessing Variables
 // ------------------------------------------------------------------

 /**
  * Begin iterator for the Variable set
  * \return the begin iterator for the Variable set
  */
 Variable::ListOfVariable::iterator begin_variables() {
   return variables.begin();
 }

 /**
  * End iterator for the Variable set
  * \return the begin iterator for the Variable set
  */
 Variable::ListOfVariable::iterator end_variables() { return variables.end(); }

 /**
  * Begin iterator for the Assignable set
  * \return the begin iterator for the Assignable set
  */
 Assignable::ListOfAssignable::iterator begin_assignables()
   { return assignables.begin(); }

 /**
  * End iterator for the Assignable set
  * \return the end iterator for the Assignable set
  */
 Assignable::ListOfAssignable::iterator end_assignables()
   { return assignables.end(); }

 /**
  * Begin iterator for the Assumption set
  * \return the begin iterator for the Assumption set
  */
 Assumption::ListOfAssumption::iterator begin_assumptions()
   { return assumptions.begin(); }

 /**
  * End iterator for the Assumption set
  * \return the end iterator for the Assumption set
  */
 Assumption::ListOfAssumption::iterator end_assumptions()
   { return assumptions.end(); }

 // ------------------------------------------------------------------
 // Accessing Candidates
 // ------------------------------------------------------------------

 /// Return a reference to the Candidate set.
 Candidate::ListOfCandidate& getCandidates() { return candidates; }

 /// Return a const reference to the Candidate set.
 const Candidate::ListOfCandidate& getCandidates() const { return candidates; }

 /**
  * Return the ith Candidate, if it exists; othwise return NULL.
  * \param i index of the Candidate to return
  */
 Candidate *getCandidate(unsigned i);

 /**
  * Return the ith Candidate as a const, if it exists; otherwise return NULL.
  * \param i index of the Candidate to return
  */
 const Candidate* getCandidate(unsigned i) const;


 /**
  * Return the index of the Candidate in the list of Candidates.
  * It is an error for the Candidate not to be in the list
  * \param candidate the Candidate for which the index is sought
  */
 unsigned getIndex(const Candidate& candidate) const;

  /**
   * Installs the passed Candidate into the Conflict_db.
   * If propagate is true (which it is by default) the system propagates to
   * infer the values entailed by the Candidate. The only reason one wouldn't
   * propagate was if one wanted to immediately make additional changes to the
   * t_system without querying it, and then propagate.
   * \param candidate the Candidate to install
   * \param whether to propagate after installing
   */
  void install(const Candidate& candidate, bool propagate = true);

  /**
   * Operates as does install(Candidate&, bool) except that the Candidate is
   * specified by an index that specifies which of the N Candidates being
   * tracked should be installed. If the index is out of range, an exception
   * is thrown.
   * \param candidateIndex index of the Candidate to install
   * \param propagate whether to propagate after installing
   */
  void install(unsigned int candidateIndex, bool propagate = true) {
    install(*getCandidate(candidateIndex), propagate);
  }

  /**
   * Add the Candidate in decreasing order of weight
   * \param pCandidate a pointer to the Candidate to add
   */
 void addCandidate(Candidate* pCandidate);

  /**
   * Add Assumption to all current Candidates. Added to allow fault injection
   * when using Livingstone for simulation.
   * \param pAssignment the Assumption Assignment to add
   */
 void addAssumption(const Assignment *pAssignment);

 /// Deallocate all Candidates and make the Candidate set empty
 void eraseCandidates();

 // ------------------------------------------------------------------
 // Accessing Conflicts
 // ------------------------------------------------------------------

 /**
  * Begin iterator for the Conflict set
  * \return the begin iterator for the Conflict set
  */
 Conflict::ListOfConflict::iterator begin_conflicts() {
   return conflicts.begin();
 }

 /**
  * End iterator for the Conflict set
  * \return the end iterator for the Conflict set
  */
 Conflict::ListOfConflict::iterator end_conflicts() { return conflicts.end(); }

 /**
  * Returns the set of Conflicts discovered so far.
  * \return a reference to the Conflict set
  */
 Conflict::ListOfConflict *get_conflicts() { return &conflicts; }

 // ------------------------------------------------------------------
 // Informational only
 // ------------------------------------------------------------------

 /**
  * Return the number of Variables in the system
  * \return the size of the Variable set.
  */
 unsigned int size() const { return variables.size(); }

 /**
  * Returns the number of Conflicts discovered so far
  * \return the size of the Conflict set
  */
 unsigned int num_conflicts() const { return  conflicts.size(); }

 private:
 /**
  * Inserts the Variable into the Conflict_db, so we can look it up later. name
  * should be stack allocated to prevent memory leaks. Its value will be
  * copied into a reference-counted string. Should only be called by
  * Variable::Variable()
  * \param pVariable pointer to the Variable to insert
  */
 void insert_variable(Variable* pVariable);

 protected:
 
 /**
  * Determines if a Clause from the solver represents an Assignment to an
  * Assumption. This is useful for mapping between a conflict (unsatisfied
  * Clause) at the solver level and a Conflict at the Conflict_db Variable
  * level. In particular, it is passed as a filter predicate to
  * LTMS::last_conflict() to return only unit Clauses for Assumptions.
  * \param clause the Clause
  * \return whether the Clause represents an Assignment to an Assumption
  */
 static bool Conflict_db::is_assumption(const Clause& clause);

  /**
   * Return a reference to the list of Conflicts.
   */
  Conflict::ListOfConflict& getConflicts() { return conflicts; }

 public:

 /**
  * Is the Clause a unit Clause for a Command Assignment?
  * \param clause a Clause
  * \param whether the Clause represents an Assignment to a Command
  */
 static bool Conflict_db::is_command(const Clause& clause);

 /**
  * Is the Clause a unit Clause for an Observable Assignment?
  * \param clause a Clause
  * \param whether the Clause represents an Assignment to an Observable
  */
 static bool Conflict_db::is_observable(const Clause& clause);

 /**
  * The abstract base class for a callback object whose sole method is a
  * predicate on a Variable.
  */
 struct Variable_acceptor {
   /// Predicate on a Variable
   virtual bool accept(const Variable&) const = 0;
 };

 /**
  * Loop through all Variables and remove those for which pVariable_acceptor
  * returns true.  The iterators define the range over which we should destroy
  * Variables in the main Variables list; we'll iterate over all Variables
  * in other lists. The iterators must be in the range begin_variables to
  * end_variables.
  * \param pVariable_acceptor a callback predicate on a Variable
  * \param beginIterator iterator pointing to the first Variable to destroy
  * \param endIterator iterator pointer beyond the last Variable to destroy
  * \param cleanup whether the LTMS should reap Clauses
  */
 virtual void
   destroy_variables(Variable_acceptor* pVariable_acceptor,
		     Variable::ListOfVariable::iterator beginIterator,
		     Variable::ListOfVariable::iterator endIterator,
		     bool cleanup = true);
 
 /**
  * Destroy the Conflict pointed to by the iterator. The iterator must come
  * from the Conflicts list (anything derived from begin_conflicts() works).
  * The interface is like Slist::erase() in that the iterator moves forward
  * after the call.
  * \param it iterator pointing to the Conflict to destroy
  */
 void destroy_conflict(Conflict::ListOfConflict::iterator& it);

 /**
  * Allow adding a Conflict to the list
  * \param assignments an Slist of Assignments specifying a Conflict
  * \param tag a datum meaningful to the client
  * \return a pointer either to an existing Conflict that subsumes the
  * Assignment set or to a newly-created Conflict
  */
 const Conflict *add_conflict(const Assignment::ListOfAssignment& assignments,
			      unsigned tag = 0);

 /**
  * Returns a previously-discovered Conflict that subsumes the passed list of
  * Assignments, if any has been discovered.
  * \param assignments an Slist of Assignments specifying a Conflict
  * \return const a pointer to an existing Conflict that subsumes the
  * Assignment set, if it exists; otherwise NULL
  * 
  */
 // See internal description.
 const Conflict* find_subsuming_conflict(const Assignment::ListOfAssignment&
					 assignments)
   const;

 /**
  * Allow the client to say where to put a new Variable.
  * We add before the iterator, as per the semantics of
  * Slist::insert_before().
  * <pre>
  * it==begin()  ->  prepend to the list, next action will prepend again
  * else         ->  add before it; next action will add before the new
  *                  element
  * </pre>
  * it==end is illegal.
  * \param it iterator pointing to the insertion position
  */
 void set_new_variable_iterator(Variable::ListOfVariable::iterator it) {
   assert(it != variables.end()); // illegal
   insert_before = it;
 }

 private:

 /**
  * A callback class whose accept() method is a predicate on a Variable.
  * This predicate is true of all Variable objects.
  */
 struct accept_all : public Variable_acceptor {
   /// Return true unconditionally
   virtual bool accept(const Variable&) const { return true; }
 };

 private:
 /// The list of all Variables in the database. Order is important.
 Variable::ListOfVariable variables;

 /// We add new Variables to be inserted before `insert_before', which allows
 /// the client to ensure that the list is sorted by time.
 Variable::ListOfVariable::iterator insert_before;

 /// The Conflict_db's LTMS
 LTMS solver;

 /// The current set of consistent Candidates
 Candidate::ListOfCandidate candidates;

 /// The Conflict set
 Conflict::ListOfConflict conflicts;

 /// The subset of the Variable set that are Assumptions
 Assumption::ListOfAssumption assumptions;

 /// The subset of the Variable set that are Assignable
 Assignable::ListOfAssignable assignables;

#ifndef DISABLE_CDB_LISTEN
  /// The listener.  Unused in flight, used in the debuggers.
  /// Essentially a list of call-backs.
  Slist<Conflict_db_listener*> listeners_;
#endif
};

#endif // CONFLICT_DB_H
