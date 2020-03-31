/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict database, NASA Ames Research Center
// $Id: conflict.h,v 1.36 2004/01/26 20:16:20 lbrown Exp $

#ifndef CONFLICT_H
#define CONFLICT_H

#include <conflict_db/assignable.h>

/**
 * This class represents a Conflict, or a set of Assignments that cannot all
 * be true at once. The Assignments will always be to Dependent Variables.
 */

class Conflict : public Pooled {
public:
  /// An ordered collection of const Conflict *
  typedef Slist<const Conflict *> ListOfConflict;

  /// An iterator over the Conflict's Assignments
  typedef Assignment::ListOfAssignment::iterator iterator;

  /// A const iterator over the Conflict's Assignments
  typedef Assignment::ListOfAssignment::const_iterator const_iterator;

  /**
   * Represent the fact that the given Assignments conflict.
   * \param assignments the conflicting Assumption assignments
   * \param a datum meaningful to the client
   */
  Conflict(const Assignment::ListOfAssignment& assignments, unsigned tag = 0);

  /// No subclasses ==> no need for virtual.
  ~Conflict() { }

  /// Compares the size of two Conflicts by Assignment count
  bool Conflict::operator>(const Conflict &other) const {
    return this->assignments.size() > other.assignments.size();
  }

  /// Compares the size of two Conflicts by Assignment count
  bool Conflict::operator<(const Conflict &other) const {
    return this->assignments.size() < other.assignments.size();
  }

  /// Fill in a list of all possible Assignments which may resolve the Conflict
  void resolving_assignments(Assignment::ListOfAssignment &new_items) const;

  /**
   * Are the Conflict's Assignments a subset [sic] of the supplied set?
   * \param assignments a list of Assignment pointers
   * \return whether all the Conflict's Assignments are in the supplied list
   */
  bool subsumes(const Assignment::ListOfAssignment& assignments) const;

  /**
   * Are the Conflict's Assignments a superset [sic] of the supplied set?
   * \param assignments a list of Assignment pointers
   * \return whether all the supplied Assignments are in the Conflict
   */
  bool subsumedBy(const Assignment::ListOfAssignment& assignments) const;

  /**
   * Return the tag data member
   */
  unsigned getTag() const { return tag; }

  //----------------------------------------------------------------------
  // Iterator methods
  //----------------------------------------------------------------------

  /// The number of Assignments in the Conflict
  unsigned size() const { return assignments.size(); }

  /// Returns an iterator at the start of the Assignments in the Conflict
  const_iterator  const begin() const { return assignments.begin(); }

  /// Returns an iterator at the end of the Assignments in the Conflict
  const_iterator const end() const { return assignments.end(); }

private:
  /// The Assignments that are in conflict.
  Assignment::ListOfAssignment assignments;

  /// A datum meaningful to the client
  unsigned tag;
};

#endif /* CONFLICT_H */
