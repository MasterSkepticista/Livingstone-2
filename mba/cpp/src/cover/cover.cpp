/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <time.h>
#include <stdlib.h>
#include <cover/cover.h>
#include <mba_utils/hash_table.h>

#undef COVER_DEBUG
#undef COVER_TIME

/**
 * This class maps Assignable* to bool. I think it's needlessly complicated.
 */

class Mark_table : private Hash_table<const Assignable*, bool> {
private:
  typedef Hash_table<const Assignable*, bool> super;

public:

  Mark_table() : super(hash_integral_key) { }

  ~Mark_table() { }

  /// Add pAssignable to map and make the mapping false
  void insert(const Assignable *pAssignable) {
    super::insert_duplicate(pAssignable, false);
  }

  /// If pAssignable is mapped, remove it. Then re-insert with mapping true
  void set(const Assignable *pAssignable) {
    super::erase(pAssignable);
    super::insert_duplicate(pAssignable, true);
  }

  /// If pAssignable is mapped, remove it. Then re-insert with mapping false
  void unset(const Assignable *pAssignable) {
    super::erase(pAssignable);
    super::insert_duplicate(pAssignable, false);
  }

  /// Return the mapping of pAssignable
  bool is_set(const Assignable *pAssignable) const {
    super *ths = const_cast<Mark_table*>(this);
    super::iterator it = ths->find(pAssignable);
    L2_assert(it != ths->end(),
	      L2_not_found_error,
	      ("variable v" + MBA_string(pAssignable->get_id()) +
	       " not found"));
    bool result = *it;
    return result;
  }
};


/**
 * This class is a template for producing Candidates. It contains a sequence of
 * Assignment objects.
 */

class Proto_candidate : public Pooled {
public:
  // Almost every Proto_candidate has 2 Assignments, many have more, few are
  // empty; this saves on the number of allocations by quite a bit.
  Proto_candidate() : a(4) { }

  // Add an Assignment*
  void add(Assignment* const pNewAssignment) {
    // insertion-sort by ID first, value second, time third
    unsigned i;
    for (i = 0; i < size(); ++i) {
      int c = compare(a[i], pNewAssignment);
      if (c == 0) return; // no duplicates
      if (c >  0) break;  // a[i] > pNewAssignment, so insert at a[i]
    }
    if (i == size()) {
      // pNewAssignment is greatest; insert at end
      a.push(pNewAssignment);
    } else {
      // Insert pAssignment at index i
      // pNewAssignment is a
      // duplicate the last
      // for j = last downto i+1
      //      a[j] <- a[j-1]
      // a[i] = pNewAssignment
      // Note : we need to copy this.  a[size()-1] returns
      // a.array_ + (size()-1)
      // a.array_ may be changed by Array::ensure.  Yuck!!!!
      Assignment *pLastAssignment = a[size() - 1];
      a.push(pLastAssignment);
      // Minimum i is 0, so min j is 1, so j-1 is safe
      // Use size - 2 since we added element size - 1 = size - 2
      // (it was called size-1 in the previous line; size grew)
      for (unsigned j = size() - 2; j > i; --j) {
	a[j] = a[j-1];
      }
      a[i] = pNewAssignment;
    }
  }

  /// A factory for creating a Candidate: instantiate, copy Assignments; return

  Candidate *make_candidate() const {
    Candidate *pCandidate = new Candidate;
    for (assign_iterator it = begin(); it != end(); ++it) {
      Assignment *pAssignment = *it;
      pCandidate->add(pAssignment);
    }
    return pCandidate;
  }


  /// Proto_candidates must have the same elements in the same order to be ==

  bool operator == (const Proto_candidate& other) const {
    if (this == &other) {                      // Reflexivity (pointer eq)
      return true;
    } else if (this->size() != other.size()) { // Equal cardinality
      return false;
    } else {                                   // Elementwise equality
      // Remember that our items are sorted, so we can implement this
      // in linear time
      for (unsigned i = 0; i < size(); ++i) {
	if (this->get_assignment(i) != other.get_assignment(i)) {
	  return false;
	}
      }
      // If we get here, we found every element ==> we're identical
      return true;
    }
  }

  typedef Array<Assignment*,false>::iterator assign_iterator;

  /// Return a begin iterator over Assignment*
  assign_iterator begin() const { return a.begin(); }

  /// Return and end iterator over Assignment*
  assign_iterator end()   const { return a.end(); }

  /// Return the ith Assignment*
  Assignment *get_assignment(unsigned i) const { return a[i]; }

  /// Return the number of Assignment*
  unsigned size() const { return a.size(); }

  /// Build up the hash code by summing the Assignment IDs .

  unsigned hash() const {
    unsigned result = 0;
    for (assign_iterator it = begin(); it != end(); ++it) {
      Assignment *pAssignment = *it;
      result += pAssignment->get_variable()->get_id();
    }
    return result;
  }

  /**
   * Hold the set of Proto_candidate objects, so that we can add to the set
   * in linear time.
   * Note: once an element has been added, it becomes the property of the set,
   * which is now responsible for deleting it.  The client may no longer safely
   * use the element, as it may have been replaced by another element and
   * deallocated.
   *
   * Note the key and element are identical.  I'd use a hash_map if we had one.
   */
  class set : private Hash_table<Proto_candidate*, Proto_candidate*> {
  private:
    static unsigned hash(Proto_candidate * const & pc) { return pc->hash(); }
    typedef Hash_table<Proto_candidate*, Proto_candidate*> super;
  public:
    set() : super(hash) { }
    
    ~set() {
      for (iterator it = begin(); it != end(); ++it) {
	Proto_candidate *pProto_candidate = *it;
	delete pProto_candidate;
      }
    }

    /// Adds the new Proto_candidate* to the set.  If pProto_candidate is a
    /// duplicate, deletes pProto_candidate and returns the original element.

    const Proto_candidate *add(Proto_candidate* pProto_candidate) {
      super::iterator it = super::find(pProto_candidate);
      if (it == super::end()) {
	// It was not in the set; insert it and return it
	super::insert(pProto_candidate, pProto_candidate);
	return pProto_candidate;
      } else {
	// It was in the set; delete it and return the one in the set
	delete pProto_candidate;
	const Proto_candidate* pFoundProto_candidate = *it;
	return pFoundProto_candidate;
      }
    }

    /// Add the Assignment to all Proto_candidates, rehashing them all.

    void map_add(Assignment *pAssignment) {
      // Build up an Array that holds all the Proto_candidate objects
      Array<Proto_candidate*, false> arr;
      {
	for (super::iterator it = super::begin(); it != super::end(); ) {
	  Proto_candidate* pProto_candidate = *it;
	  arr.push(pProto_candidate);
	  super::erase(it); /* also iterates it */
	}
      }
      // Add each Proto_candidate to the map
      {
	for (Array<Proto_candidate*,false>::iterator it = arr.begin();
	     it != arr.end(); ++it) {
	  Proto_candidate *pProto_candidate = *it;
	  pProto_candidate->add(pAssignment);
	  super::insert(pProto_candidate, pProto_candidate);
	}
      }
    }


    /// Acquires all Proto_candidates in the other set.
    ///This empties out the other set, and may destroy some of its elements.

    void acquire(set& other) {
      for (iterator it = other.begin(); it != other.end(); ++it) {
	Proto_candidate *pProto_candidate = *it;
	add(pProto_candidate);
      }
      static_cast<super&>(other).erase();
    }


    /// Creates a Candidate for each Proto_candidate in the set.

    void make_candidates(Candidate::ListOfCandidate& list) const {
      for (iterator it = begin(); it != end(); ++it) {
	Proto_candidate *pProto_candidate = *it;
	list.push_front(pProto_candidate->make_candidate());
      }
    }

    typedef super::iterator iterator;

    // Hash_table::begin isn't const for some reason
    iterator begin() const { return const_cast<set*>(this)->super::begin(); }

    // Hash_table::end isn't const for some reason
    iterator end() const { return const_cast<set*>(this)->super::end(); }
  };

private:

  /// A total ordering on Assignment*
  // if > then a>b ; if < then a<b ; if = then look to the next criterion

  static int compare(const Assignment *a, const Assignment *b) {
    
    // criterion: sort by ID of Variable
    unsigned id1 = a->get_variable()->get_id();
    unsigned id2 = b->get_variable()->get_id();
    if      (id1 > id2) { return  1; }
    else if (id1 < id2) { return -1; }
    
    // criterion: sort by value
    unsigned index1 = a->get_index();
    unsigned index2 = b->get_index();
    if      (index1 > index2) { return  1; }
    else if (index1 < index2) { return -1; }
    
    // criterion: sort by time step
    unsigned timeStep1 = a->get_time_step();
    unsigned timeStep2 = b->get_time_step();
    if      (timeStep1 > timeStep2) { return  1;}
    else if (timeStep1 < timeStep2) { return -1; }

    // Same variable, same value, same time step: they're equal.
    return 0;
  }

private:
  // The sequence of Assignment*
  Array<Assignment*, false> a;
};


#if 0
static void print_conflicts(Slist<Conflict*>& conflicts){
  for (Slist<Conflict*>::iterator it = conflicts.begin();
       it != conflicts.end(); ++it) {
    Conflict *pConflict = *it;
    _STD_ cout <<  pConflict << "| " ;
  }
  _STD_ cout << _STD_ endl;
}
#endif


// Important note about conflicts:
//
// This algorithm finds all covers for the conflict list that cost less than
// or equal to rho.   It does not yet check each covering candidate
// against a model to ensure they are consistent.  If one is found to be
// inconsistent, this would add in additional conflicts we could use to rule
// out subsequent assignments.

static void rho_coverage_aux (Conflict_db* db,
			      Conflict::ListOfConflict& conflicts,
			      Proto_candidate::set &candidates,
			      Assignment::weight_t rho,
			      Mark_table& mark_table) {
  const Conflict* pConflict = conflicts.front();
  conflicts.pop_front();

  // ************************************************************
  // choose variable to cover the shortest remaining conflict
  // ************************************************************

  for (Slist<Assignment* const>::const_iterator it = pConflict->begin();
       it != pConflict->end(); ++it) {
    Assignment * const pConflictingAssignment = *it;
    Assignable* const pAssignable = pConflictingAssignment->get_variable();

    // It could be the case that we have already chosen an Assignment for this
    // Variable in an enclosing call to rho-coverage. In that case we have to
    // look to another Variable to cover this Conflict

    if (mark_table.is_set(pAssignable)) {
      continue;
    }

    // ************************************************************
    // Consider each assignment to the variable under consideration
    // ************************************************************
    //
    // All-assignments is the nominal assigment followed by the failures.
    // Failures is a list of failure assignments that must be stored presorted
    // in cost order.   We consider both the expected value of the assignment
    // and the failures because we might have a conflict involving a failure
    // assignment. In that case, the nominal value is a reasonable way to cover
    // that conflict.

    for (Assignable::iterator assignment_it = pAssignable->assignments_begin();
	 assignment_it != pAssignable->assignments_end(); ++assignment_it) {
      Assignment* pAssignment = *assignment_it;
      Assignment::weight_t next_rho = rho - pAssignment->get_weight();
      // A conflict should be a set of assumptions, not just a set of
      // assignables.
      Assumption* pAssumption = static_cast<Assumption *>(pAssignable);

      // **********************************************************************
      // Make sure we are considering a different assignment from the
      // conflicting one
      // **********************************************************************

      // If the conflicts that this assignment appears in contains the conflict
      // we are trying to resolve, we have to skip to the next assignment.

      if (pAssignment->is_in_conflict(pConflict)) {
	continue;
      }

      // Since failures are sorted by cost, if we are out of cost at this
      // point, we should skip to the next variable

      if (next_rho < 0) {
	// This breaks out of the assignment iteration.  It does not return
	// from the entire function.
	break;
      }
      
      // **********************************************************************
      // With the current Assignment we have covered this Conflict and any
      // others that contain this Variable with a different Assignment.
      // **********************************************************************

      // This Variable is now taken. Mark it so we don't repeatedly reassign it
      // var->assign(*pAssignment);
      mark_table.set(pAssignable);

      Conflict::ListOfConflict active_conflicts = conflicts;
      active_conflicts -= pAssignment->get_resolved_conflicts();
      bool is_default = pAssumption->is_default(*pAssignment);

      if (active_conflicts.empty())  {
	// ************************************************************
	// If we have covered all conflicts, start building the candidate.
	// ************************************************************
	Proto_candidate* pProto_candidate = new Proto_candidate;
	if (!is_default)
	  pProto_candidate->add(pAssignment);
	candidates.add(pProto_candidate);
      } else {
	// ************************************************************
	// Otherwise, see if we can cover the remaining conflicts.
	// ************************************************************
	if (next_rho < 1) {
	  // Note that if we are guaranteed the next assignment for this
	  // variable exceeds the cost of this one by at least one, we
	  // could skip the rest of the assignments on this variable.
	  // However, we might have two different failures with equal
	  // likelihood and the next one might cover all the conflicts.
	} else {
	  // If we have one unit or more, try to cover the remaining conflicts
	  Proto_candidate::set subcandidates;
	  rho_coverage_aux(db, active_conflicts, subcandidates, next_rho,
			   mark_table);
	  if (!is_default) { subcandidates.map_add(pAssignment); }
	  // Copy its elements onto candidates (if they aren't already there)
	  candidates.acquire(subcandidates);
	}
      }
      // Mark this variable as free for re-assignment by the search
      mark_table.unset(pAssignable);
    } // End of iteration over the assignments to the variable
  } // End of iteration over the vars in the conflict
}


/// This is the API. Parameter candidates is assumed to be empty.

void rho_coverage(Conflict_db *db,
		  Candidate::ListOfCandidate& candidates,
		  Assignment::weight_t rho) {
  //	db->restore_assumption_defaults();
  // Copy the conflict list, because the rho coverage algorithm will pop
  // members off it
  Conflict::ListOfConflict conflict_list = *(db->get_conflicts());

#ifdef COVER_TIME
  _STD_ cout << "Number of conflicts to cover" << conflict_list.size()
	    << _STD_ endl;
  clock_t start_time = clock();
#endif

  // Store marks for all the Variables. The Variables start unmarked;
  // rho_coverage_aux uses them to avoid recurring into already-fixed Variables

  Mark_table mark_table;
  {
    for (Slist<Assignable*>::iterator it = db->begin_assignables();
	 it != db->end_assignables(); ++it) {
      Assignable *pAssignable = *it;
      // We know it isn't there: mark_table is new & Assignables are unique
      mark_table.insert(pAssignable);
    }
  }

  Proto_candidate::set pc_list;
  // Fill up pc_list
  rho_coverage_aux(db, conflict_list, pc_list, rho, mark_table);
  // Convert members of pc_list into Candidates
  pc_list.make_candidates(candidates);

#ifdef COVER_TIME
  float seconds = float(clock() - start_time) / CLOCKS_PER_SEC;
  _STD_ cout << "Rho coverage time : " << seconds << "Candidates: "
	    << candidates.size() << _STD_ endl;
#endif

  // Filter the Candidates for consistency
#ifdef COVER_TIME
    start_time = clock();
#endif
    for (Candidate::ListOfCandidate::iterator it = candidates.begin();
	 it != candidates.end(); )  {
      Candidate *pCandidate = *it;
      // Put the current Candidate into the Conflict_db
      db->restore_assumption_defaults();
      pCandidate->assign();
      if (db->conflict() != NULL) {
	// The Candidate is inconsistent; delete it and erase it from the list
	delete pCandidate;
	candidates.erase(it);
      } else {
	++it;
      }
    }
#ifdef COVER_TIME
    seconds = float(clock() - start_time) / CLOCKS_PER_SEC;
    _STD_ cout << "Rho Consistency time: " << seconds
	      << "Candidates left: " << candidates.size()
	      << "Conflicts after:" << db->get_conflicts()->size()
	      << _STD_ endl;
#endif
}
