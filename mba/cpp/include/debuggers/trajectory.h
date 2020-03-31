/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: trajectory.h,v 1.15 2004/02/10 21:13:21 lbrown Exp $ */

#ifndef trajectory_HEADER
#define trajectory_HEADER

#include <conflict_db/assignment.h>

class Assignment;
class Candidate;

/**
 * A class to represent a trajectory over time; that is, its sequence of modes.
 * Each element of the Trajectory is an Assignment, which may have been
 * truncated.
 * \internal Shouldn't a Trajectory be defined as a "sequence of mode
 * Transitions"?
 */

class Trajectory : public Pooled {
 public:
  /// Nothing special to do
  Trajectory() { }

  /**
   * An element of the Trajectory, an Assignment with print names.
   * Values are copied from the Assigment. A reference to the L2_string_map
   * allows print names to be retrieved.
   */

  class Parts : public Pooled {
  public:
#ifdef VXWORKS531
    // To work around limitations of template processing
    Parts() : time(0), rank(0), id(0), index(0) { }
#endif
    /**
     * Copy the data from the Assignment object.
     * \param pAssignment the Assignment of an Assumption Variable
     * \param string_map used for mapping onto print names
     */
    Parts(const Assignment* pAssignment);
    /// Return the time step.
    unsigned   get_time() const { return time;  }
    /// Return the Assignment's weight/cost/rank
    Assignment::weight_t get_rank() const { return rank;  }
    /// Return the Assignment's Variable's ID
    unsigned get_id() const { return id; }
    /// Return the Assignment's Variable's value index
    unsigned get_index() const { return index; }
    /// Two Parts objects are == if they have the same id, time and index
    bool operator==(const Parts& p) const {
      return ((this->get_id()    == p.get_id()) &&
	      (this->get_time()  == p.get_time()) &&
	      (this->get_index() == p.get_index()));
    }


  private:
    /// Time of the variable
    unsigned time;
    /// The Assignment's weight/cost/rank
    Assignment::weight_t rank;
    /// The Variable's ID
    unsigned id;
    /// The value index
    unsigned index;
  };

  /// Return the number of parts.
  unsigned get_parts_count() const { return list.size(); }

  /// An iterator over the Part (mode Assignment) list.
  typedef Slist<Parts>::const_iterator iterator;

  /// Begin iterator for the Trajectory's Parts elements
  iterator begin() const { return list.begin(); }
  /// End iterator for the Trajectory's Parts elements
  iterator end() const { return list.end(); }

  /**
   * Add the Parts (an Assignment) at the beginning of this Trajectory's list.
   * \param the Assignment to insert at the beginning of the Parts list
   */
  void insert(const Parts& parts) { list.push_front(parts); }

  /**
   * Add all the Trajectory argument's Parts to this Trajectory's list.
   * It is assumed that the time steps of the supplied trajectory's
   * Assignments precede those of this Trajectory.
   * \param trajectory the Trajectory whose parts are being prepended
   */
  void prepend(const Trajectory& trajectory) {
    for (iterator it = trajectory.begin(); it != trajectory.end(); ++it) {
      const Parts& parts = *it;
      insert(parts);
    }
  }

  /**
   * Does the Trajectory contain the Parts?
   */
  bool contains(const Parts& targetParts) const {
    for (iterator it = begin(); it != end(); ++it) {
      const Parts& p = *it;
      // This Trajectory contains the target Parts
      if (p == targetParts) { return true; }
    }
    // This Trajectory does not contain the target Parts
    return false;
  }

  /**
   * Does this Trajectory contain all Parts of the targetTrajectory?
   */
  bool subsumes(const Trajectory& targetTrajectory) const {
  // Does each part of the target Trajectory occur in this Trajectory?
  for (Trajectory::iterator it = targetTrajectory.begin();
       it != targetTrajectory.end(); ++it) {
    const Parts& p = *it;
    if (!this->contains(p)) { return false; }
  }
  // This Trajectory contains all Parts of targetTrajectory
  return true;
}

 private:
  /// The sequence of Parts elements, ordered by time step.
  Slist<Parts> list;
};

#endif
