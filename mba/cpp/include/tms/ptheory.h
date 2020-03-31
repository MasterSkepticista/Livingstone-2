/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Propositional theory used by the TMS
// $Id: ptheory.h,v 1.51 2002/07/25 23:50:09 lbrown Exp $

#ifndef PTHEORY_H
#define PTHEORY_H

#include <mba_utils/slist.h>
#include <tms/clause.h>
#include <tms/proposition.h>

#ifndef DISABLE_PTHEORY_LISTEN
// The listener for this class
class Ptheory_listener;
#endif

/**
 * A propositional theory, containing Propositions and Clauses.
 * An LTMS is one kind of propositional theory.
 */

class Ptheory : public Pooled {
 public:

  /// A set of Propositions.
  typedef Array<Proposition*, false> prop_list;

  /// A set of Clauses.
  typedef Array<Clause*, false> clause_list;

  //----------------------------------------------------------------------
  //
  // Constructors and destructors
  //
  //----------------------------------------------------------------------

  /// Just set the time stamp to 0
  Ptheory() : time_stamp(0) { }

  /// Deallocate the Propositions and Clauses
  virtual ~Ptheory();

#ifndef DISABLE_PTHEORY_LISTEN
  /// Add a listener callback object.
  void add_listener(Ptheory_listener *listen) {
    if (listen != NULL) { listeners_.push_front(listen); }
  }

  /// Remove a listener callback object.
  void remove_listener(Ptheory_listener *listen) {
    if (listen != NULL) { listeners_.remove_first(listen); }
  }
#endif

  //----------------------------------------------------------------------
  //
  // Creating and accessing a theory's components
  //
  //----------------------------------------------------------------------

  /**
   * Create a new Clause with the specified positive and negative literals and
   * add it to the Clauses of the theory. A literal is a Proposition or the
   * negation of a Proposition.
   *
   * "inTheory" determines if the Clause will be created active in the theory
   * or currently deleted.
   *
   * "datum" is the datum of this new Clause. It is a pointer to whatever item
   * (object, etc) should be associated with this Clause for whatever purpose
   * the caller desires.  One might store a model fragment that generated this
   * Clause or something similar.
   *
   * "isVolatile" may be set to true if it's expected that this Clause will
   * often be temporarily removed then added back to the theory.
   * Truth-maintenance algorithms are free to use this advice to support
   * inferences based on non-volatile Clauses when there is a choice. This may
   * reduce the amount of re-support of existing inferences that needs to be
   * done.
   *
   * Note that no check is made to ensure that a Clause containing the same
   * literals has not already been created, so it is possible to create
   * multiple copies of what are essentially the same Clause. This should not
   * change the results of inference, but it is inefficient.
   */

  virtual
    Clause* create_clause(void* datum,
			  Proposition::ListOfProposition& posPropositions,
			  Proposition::ListOfProposition& negPropositions,
			  bool isVolatile,
			  bool inTheory);

  /// Create a Proposition and insert it into the theory.
  virtual Proposition* create_proposition();

  /**
   * Re-initialize the Ptheory.
   * \todo Apparently this is never called. Either call it or remove it.
   */
  virtual void initialize();

  //----------------------------------------------------------------------
  // Adding and removing clauses.
  // Note removal does not permanently destroy the object.
  //----------------------------------------------------------------------

  /**
   * Add the Clause to the theory.
   * This involves tallying its unassigned literals and setting its satisfying
   * Proposition.
   * \param clause the Clause to add to the theory.
   */
  virtual void add(Clause& clause);

  /**
   * Remove the Clause from the theory without destroying it.
   * Simply mark it as deleted and it will be ignored.
   * \param clause the Clause to remove from the theory.
   */
  virtual void remove(Clause& clause) { clause.set_deleted(true); }

  /**
   * Finish destroying marked Clauses and Propositions.
   * After this call, they are removed from the theory and are deallocated.
   * This is the "sweep" of a mark-and-sweep.
   */
  virtual void finish_destroying();

  //----------------------------------------------------------------------
  // Iterating through theory's propositions and clauses
  //----------------------------------------------------------------------

  /// Begin iterator over the theory's Clauses
  clause_list::iterator begin_clauses() const { return clauses.begin(); }

  /// End iterator over the theory's Clauses
  clause_list::iterator end_clauses()   const { return clauses.end();   }

  /// Begin iterator over the theory's Propositions
  prop_list::iterator begin_propositions() const { return props.begin(); }

  /// End iterator over the theory's Propositions
  prop_list::iterator end_propositions()   const { return props.end();   }

  //----------------------------------------------------------------------
  // Informational only
  //----------------------------------------------------------------------

  /// The number of Clauses in the theory, including deleted ones.
  unsigned int num_clauses()      const { return clauses.size(); }
  /// The number of Propositions in the theory.
  unsigned int num_propositions() const { return props.size();   }

protected:

  //----------------------------------------------------------------------
  // Initialization
  //----------------------------------------------------------------------

  /// Set the time stamp of the Ptheory and of all its Clauses to 0.
  void initialize_time_stamps();

protected:
  /**
   * A cohort flag used for mark and sweep.
   * A time stamp is an unsigned integer that allows groups of Clauses to be
   * associated together. The TMS will provide methods for performing
   * operations on all Clauses with the same time stamp. This defaults to 0 for
   * those not using the temporal aspects of the system.
   */
  Clause::time_stamp_t time_stamp;

private:
  /**
   * The set of all Propositions in the theory.
   * A Proposition does not have to be associated with a Clause
   */
  prop_list props;

  /**
   * The set of all Clauses associated with the theory, comprising Clauses that
   * are currently in the theory and Clauses that are currently deleted.
   */
  clause_list clauses;

#ifndef DISABLE_PTHEORY_LISTEN
  /**
   * The listeners: unused in flight, used in the debuggers.
   * Does nothing if null; on some significant events, we invoke callbacks on
   * them. The order of listeners is unspecified.
   */
  Slist<Ptheory_listener*> listeners_;
#endif

};

#endif // PTHEORY_H
