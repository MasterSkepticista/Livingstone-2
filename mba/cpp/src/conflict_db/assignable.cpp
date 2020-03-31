/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: assignable.cpp,v 1.70 2002/10/24 15:51:00 lbrown Exp $ */
#include <conflict_db/assignable.h>
#include <conflict_db/conflict_db.h>    // get_solver

Assignable::Assignable(Conflict_db &db, unsigned nvalues,
		       Theory::time_step_t time_step, unsigned id) :
  Variable(db, nvalues, time_step, id), tms(db.get_solver()), current(NULL),
  exogenouslyAssigned(false)
{
  // For each value we will create an Assignment and a Clause in DB that will
  // represent this Variable being assigned to the value. These Clauses are
  // created in the DB but not added since we do not wish to assert the
  // Assignment.  They are created as volatile Clauses since we will be
  // asserting and retracting it when we change the Assignment to this Variable.
  {
    Proposition::ListOfProposition posPropositions;
    // This will always be empty, so allocate it statically
    static Proposition::ListOfProposition emptyPropositionList;
    for (unsigned i = 0; i < get_nvalues(); i++) {
      // Create the that Clause will represent the assertion and retraction of
      // each Assignment. The Clause will consist of the one Proposition that
      // represents this Assignment; that is, it will be a unit Clause.
    
      // Create the Assignment.
      Assignment* pAssignment = new Assignment(this, i);
      assignments.push_back(pAssignment);

      // Next, find the Proposition that means "name=value"
      // Simple since we have the Propositions in order.
      Proposition * const pProposition = get_prop(i);

      // Next, create the Clause that has only this one Proposition.
      // In order to be satisfied, this Clause will force the "name=value"
      // Proposition to be true. The datum of the Clause will be the Assignment.
      posPropositions.push_front(pProposition);
      Clause *pClause = tms->create_clause(pAssignment, 
			 		   posPropositions,
					   emptyPropositionList,
					   true,   // volatile
					   false); // inTheory
      // Clear the list for re-use on subsequent cycles
      posPropositions.pop_front();
      
      // Finally, store a pointer to the Clause that implements the Assignment
      // in the Assignment, so the Assignment can make itself true.
      pAssignment->set_clause(pClause);
    }
  }
}

/***************************************************************************
        Destructor: destroy all the assignments.
 ***************************************************************************/

Assignable::~Assignable() {
  for (iterator it = assignments_begin(); it != assignments_end(); ) {
    Assignment *pAssignment = *it;
    delete pAssignment;
    assignments.erase(it); // also iterates it
  }
}

 
/***************************************************************************
        Various ways of fiddling with the TMS.
 ***************************************************************************/

// Called by assign(unsigned)

void Assignable::assign(const Assignment *pAssignment) {
  if (current != pAssignment) {
    // Remove from the LTMS the unit Clause asserting its old value
    if (current) { tms->remove(*current->get_clause()); }
    // Set the Assignment to its new value
    current = pAssignment;
    // Add to the LTMS the unit Clause asserting its new value
    if (current) { tms->add(*current->get_clause()); }
  }
}


void Assignable::unassign() {
  if (current) {
    // Remove from the LTMS the unit Clause that asserts the old value
    tms->remove(*current->get_clause());
    // Reset the current Assignment
    current = NULL;
  }
}


// This would be shorter if Assignable::assignments were an Array, but the
// entire system would, surprisingly, be slower.

Assignment* Assignable::get_assignment(unsigned int index) {
  unsigned int current_index = 0;
  for (Assignable::iterator it = assignments_begin();
       it != assignments_end(); ++current_index, ++it)  {
    if (current_index == index) {
      Assignment *pAssigment = *it;
      return pAssigment;
    }
  }
  return NULL;
}

