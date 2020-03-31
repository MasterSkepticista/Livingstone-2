/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Implementation of propositional theory used by the TMS
// $Id: ptheory.cpp,v 1.46 2004/01/22 01:21:43 lbrown Exp $

#include <tms/ptheory.h>

// Allow easily disabling the listeners for flight
// Note that since it's a macro, the args should be as simple as possible.
// The do-while(0) is the only portable way to block
#ifndef DISABLE_PTHEORY_LISTEN
#  include <tms/ptheory_listener.h>
#  define ptheory_record(call_with_args) \
   do { \
    Slist<Ptheory_listener*>::iterator listen_it##__LINE__ \
        = listeners_.begin(); \
    while(listen_it##__LINE__ != listeners_.end()) { \
        (*listen_it##__LINE__)->call_with_args;  \
        ++listen_it##__LINE__; \
    } \
   } while(0)
#else
#  define ptheory_record(call_with_args)
#endif


Ptheory::~Ptheory() {
  ptheory_record(destroying_container(*this));
  // First delete all allocated propositions
  {
    for (prop_list::iterator it = begin_propositions();
	 it != end_propositions(); ++it) {
      Proposition *pProposition = *it;
      delete pProposition;
    }
  }
  // and then delete all allocated clauses
  {
    for (clause_list::iterator it = begin_clauses();
	 it != end_clauses(); ++it) {
      Clause *pClause = *it;
      delete pClause;
    }
  }
}


Clause* Ptheory::create_clause(void* datum,
			  Proposition::ListOfProposition& posPropositions,
			  Proposition::ListOfProposition& negPropositions,
			  bool isVolatile,
			  bool inTheory) {
  Clause *pClause = new Clause(datum, posPropositions, negPropositions,
			       isVolatile);
  // Add this Clause to the Ptheory's set of Clauses
  clauses.push(pClause);
  // If specified, insert add it to the theory now
  if (inTheory) { add(*pClause); }
  // Notify all Ptheory_listeners, if enabled
  ptheory_record(created_clause(*pClause));
  return pClause;
}


Proposition* Ptheory::create_proposition() {
  Proposition * pProposition = new Proposition();
  props.push(pProposition);
  ptheory_record(created_proposition(*pProposition));
  return pProposition;
}


void Ptheory::add(Clause& clause) {
  if (clause.is_deleted()) {
    clause.set_deleted(false);
    clause.set_satisfying_prop(clause.find_satisfying_prop());
    clause.set_unassigned_count(clause.count_unassigned());
  }
}


void Ptheory::initialize() {
  // Initialize the Ptheory proper
  time_stamp = 0;
  // Initialize the Propositions
  {
    for (prop_list::iterator it = begin_propositions();
	 it != end_propositions(); ++it) {
      Proposition *pProposition = *it;
      pProposition->initialize();
    }
  }
  // Initialize the Clauses
  {
    for (clause_list::iterator it = begin_clauses();
	 it != end_clauses(); ++it) {
      Clause *pClause = *it;
      pClause->initialize();
    }
  }
}


void Ptheory::initialize_time_stamps() {
  // Initialize the ptheory time stamp
  time_stamp = 0;
  // Initialize the Clauses' time stamps
  for (clause_list::iterator it = begin_clauses(); it != end_clauses(); ++it) {
    Clause *pClause = *it;
    pClause->time_stamp = 0;
  }
}


/***************************************************************************
  Deallocating a subset of the theory:
  First, the user must mark the propositions and clauses to be destroyed.
  Use Proposition::mark_destroying() for that.  Second, the user calls
  finish_destroying() to remove marked propositions and clauses from the
  theory and deallocate them.
 ***************************************************************************/
void Ptheory::finish_destroying() {
  // foreach clause of theory
  //     if marked
  //         remove from list
  //         foreach prop of clause
  //             if not marked
  //                 remove clause from prop's list
  //         delete clause
        
  for (clause_list::iterator cit = begin_clauses(); cit != end_clauses(); ) {
    Clause *pClause = *cit;
    if (!pClause->is_destroying()) { ++cit; }
    else {
      ptheory_record(destroying_clause(*pClause));
      clauses.erase(cit); // also iterates
      // foreach positive prop
      {
	for (Clause::iterator it = pClause->begin_positive();
	     it != pClause->end_positive(); ++it) {
	  Proposition *pProposition = *it;
	  if (!pProposition->is_destroying()) {
	    pProposition->remove_positive(pClause);
	  }
	}
      }

      // foreach negative prop
      {
	for (Clause::iterator it = pClause->begin_negative();
	     it != pClause->end_negative(); ++it) {
	  Proposition *pProposition = *it;
	  if (!pProposition->is_destroying()) {
	    pProposition->remove_negative(pClause);
	  }
	}
      }
      delete pClause;
    }
  }

  // Must be after the above loop: that loop looks at props which
  // may be marked, and this one deallocates those.
  // foreach prop of theory
  //     if marked
  //        remove from list
  //        delete

  for (prop_list::iterator it = begin_propositions();
       it != end_propositions(); ) {
    Proposition *pProposition = *it;
    if (!pProposition->is_destroying()) { ++it; }
    else {
      ptheory_record(destroying_proposition(*pProposition));
      props.erase(it); // also iterates
      delete pProposition;
    }
  }
}
