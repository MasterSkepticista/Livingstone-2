/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_string_map_listener.h,v 1.12 2002/05/13 18:08:14 lbrown Exp $ */
#ifndef L2_string_map_listener_header
#define L2_string_map_listener_header

#include <readers/to_t_system_listener.h>
#include <transition/tracker_listener.h>
#include <transition/T_system_listener.h>
#include <conflict_db/conflict_db_listener.h>
#include <tms/ptheory_listener.h>
#include <mba_utils/pooled.h>

class L2_string_map;
class L2_file;
class to_t_system;
class Tracker;

/**
 * This one class has all the listener calls we need to keep the L2_string_map
 * up to date.
 * All of the other *_listener classes are abstract; this one implements all
 * of the abstract member functions, which are callbacks. Each callback 
 * potentially updates the same L2_string_map for the Tracker.
 * The constructor adds itself to each of the objects that support listeners:
 * the to_t_system writer, the Tracker, the T_system, the Conflict_db and the
 * LTMS.
 */

class L2_string_map_listener :
public Pooled,
public virtual to_t_system_listener,
public virtual Tracker_listener,
public virtual T_system_listener,
public virtual Conflict_db_listener,
public virtual Ptheory_listener {
 private:
  /// The name mapping object that is updated when various events occur.
  L2_string_map& map;

  /// The L2_file object into which an L2_file_reader is reading a model.
  L2_file& file;

  /// The Livingstone engine.
  Tracker& tracker;

 public:
  /**
   * Add a to_t_system_listener to the T_system; a Tracker_listener to the
   * Tracker, a Conflict_db_listener to the Conflict_db and a Ptheory_listener
   * to the LTMS (which is a Ptheory). Also create the Domain mapping.
   */
  L2_string_map_listener(L2_string_map&,
			 L2_file&,
			 to_t_system&,
			 Tracker&);

  /**
   * Remove the T_system_listener from the T_system, the Conflict_db_listener
   * from the Conflict_db, and the Ptheory_listener from the LTMS (which is a
   * Ptheory).
   * What about the Tracker's Tracker_listener and the to_t_system's
   * to_t_system_listener?
   */
  virtual ~L2_string_map_listener();

 private:
  ////////////////////////////////////////////////////
  // to_t_system_listener

  /**
   * When the to_t_system creates a Variable from a L2rVariabl, create
   * mappings for both the L2rVariable and the Variable.
   */
  virtual void created_variable(const L2rVariable* pL2rVariable,
				Variable& variable);

  /// When the to_t_system creates a Clause from an L2rClause, do nothing.
  virtual void created_clause(const L2rClause* pL2rClause, Clause& clause);

  /// When the to_t_system creates a Clause from an L2Variable, do nothing.
  virtual void created_clause(const L2rVariable* pL2rVariable, Clause& clause);

  /// When the to_t_system is being destroyed, do nothing.
  virtual void destroying_container(to_t_system&);

  ////////////////////////////////////////////////////
  // Tracker_listener

  /// Will throw an exception if called.
  virtual void destroying_container(Tracker& tracker);
        

  ////////////////////////////////////////////////////
  // T_system_listener

  /**
   * When the T_system progresses a Variable, add the newly-created Variable
   * to the name mapping.
   */
  virtual void progressed_variable(Variable& nowVariable,
				   Variable& pastVariable);
  /**
   * When the T_system creates a new State_variable, add the newly-created
   * Variable to the name mapping and re-map the Trajectory set.
   */
  virtual void created_state_variable(Assignable&);

  /**
   * When the T_system creates a new mode Transition, add the newly-created
   * Transition to the name mapping.
   */
  virtual void created_mode_transition(Dependent&, Assumption&);

  /// Will throw an exception if called.
  virtual void destroying_container(T_system&);

  ////////////////////////////////////////////////////
  // Conflict_db_listener

  /// When the Conflict_db creates a Variable, do nothing.
  virtual void created_variable(Variable&);

  /**
   * When the Conflict_db destroys a Variable, remove it from the name mapping.
   */
  virtual void destroying_variable(Variable& variable);

  /// Will throw an exception if called.
  virtual void destroying_container(Conflict_db& conflict_db);

  ////////////////////////////////////////////////////
  // Ptheory_listener

  /**
   * When the Ptheory creates a Proposition, do nothing. The mapping will
   * be populated when a Variable is created.
   */
  virtual void created_proposition(Proposition& proposition);

  /// When the Ptheory creates a Clause, do nothing.
  virtual void created_clause(Clause& clause);

  /**
   * When the Ptheory destroys a Proposition, remove the Proposition from the
   * mapping.
   */
  virtual void destroying_proposition(Proposition& proposition);

  /// When the Ptheory destroys a Clause, do nothing
  virtual void destroying_clause(Clause& clause);

  /// Will throw an exception if called.
  virtual void destroying_container(Ptheory& ptheory);
};
#endif
