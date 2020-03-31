/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_string_map.h,v 1.27 2004/02/05 22:36:12 lbrown Exp $ */
#ifndef L2_string_map_HEADER
#define L2_string_map_HEADER

#include <mba_utils/array.h>
#include <mba_utils/hash_table.h>

class Assignment;
class Clause;
class Domain;
class L2rVariable;
class L2_file;
class Variable;
class Proposition;
class Trajectory;

/**
 * This class has mappings between print name strings and the internal data
 * structures (L2rVariable, Variable, Domain and Proposition), which is
 * necessary for the debuggers, and for the debugger-reader interface.
 * The L2_string_map_listener populates these mappings, and truncation in the
 * T_system may depopulage them.
 */

class L2_string_map : public Pooled {
 public:
  /// Empty the Trajectories and create a Domin for the State_variable.
  L2_string_map();

  /// Deallocate the trajectories and Domain for the State_variable.
  virtual ~L2_string_map();

 private:
  /// Map print name to Variable ID.
  Hash_table<MBA_string, unsigned> name2id;

  /// Map Variable ID to print name.
  Array<MBA_string, true> name_by_id;

  /**
   * Map Variable ID to Variable.
   * This will return the Transitioned (mode), not the Transition, which have
   * the same ID.
   */
  Array<Variable*, false> nowvar_by_id;

  /**
   * Map Variable ID to L2rVariable.
   * This will return the Transitioned (mode), not the Transition, which have
   * the same ID.
   */
  Array<const L2rVariable*, false> readvar_by_id;

  /// Cache a pointer to the current State_variable.
  Variable *state_var;

  /// A list of Domains, created in the constructor.
  Slist<const Domain*> domains;

  /// Map Variable ID to Domain.
  const Domain ** varid2domain;

  // Map Transition ID to Domain.
  // Non-transitioned variables have a null transition domain.
  const Domain ** xvarid2domain;

  /// How many variable IDs we have (bounds-checking for [x]varid2domain)
  unsigned nvars;

  /// A single domain for the current State_variable.
  Domain *state_domain;

 public:
  /**
   * Return the print name of the Variable.
   * \param variable the Variable the name of which is sought
   * \return the print name of variable
   */
  MBA_string          get_variable_name(const Variable& variable) const;

  /**
   * Return the print name of the Variable as if it had the given time step.
   * \param variable the Variable the name of which is sought
   * \param timeStep this overrides the Variable's actual time step
   * \return the print name of the Variable as if it had the given time step.
   */
  MBA_string          get_variable_name(const Variable& variable,
					unsigned timeStep) const;

  /**
   * Return the Variable with the print name; throw an exception if not found.
   * \param the print name of a Variable
   * \return a reference to the Variable that has the print name
   */
  Variable&           get_variable_by_name(const MBA_string&);

  /**
   * Return the L2rVariable for the Variable.
   * The State Variable does not have a corresponding L2rVariable.
   * \param the Variable the L2rVariable of which is sought
   * \return the L2rVariable that corresponds to variable if it exists;
   * otherwise NULL (the State Variable)
   */
  const L2rVariable * get_model_variable(const Variable& variable) const;

  /**
   * Return a string of the Domain values for the supplied L2rVariable.
   * \param pL2rVariable a pointer to the L2rVariable the values of which are
   * sought
   * \return a string of the print names of the values pL2rVariable can assume
   */
  const MBA_string  * get_variable_values(const L2rVariable* pL2rVariable) const;

  /**
   * Return a string of the Domain values for the supplied Variable.
   * \param variable the Variable the values of which are sought
   * \return a string of the print names of the values variable can assume
   */
  const MBA_string  * get_variable_values(const Variable& variable) const;

  /**
   * Return the Domain object for the supplied Variable.
   * \param variable the Variable the Domain of which is sought
   * \return a pointer to the Domain object for variable
   */
  const Domain      * get_variable_domain(const Variable& variable) const;

  /**
   * Return the Domain object for the supplied L2rVariable.
   * \param pL2rVariable a pointer to the L2rVariable the Domain of which is
   * sought
   * \return a pointer to the Domain object for pL2rVariable
   */
  const Domain      * get_variable_domain(const L2rVariable* pL2rVariable) const;

  /**
   * Return the NOW Variable that corresponds to the L2rVariable.
   * \param pL2rVaraible a pointer to the L2rVariable the now Variable of which
   * is sought
   * \return a reference to the now Variable for pL2rVariable
   */
  Variable&           get_now_variable(const L2rVariable* pL2rVariable);

  /**
   * Is there a Variable with the supplied name?
   * \param variableName a string that may be the print name of a Variable
   * \return true if there exists a Variable named variableName; otherwise
   * false
   */
  bool has_variable_named(const MBA_string& variableName);

  /**
   * Return the Variable with the print name; may return NULL.
   * \param variableName a string that may be the print name of a Variable
   * \return a pointer to the named Variable if it exists; otherwise NULL
   */
  Variable *get_var_or_null(const MBA_string& variableName);

  /// Begin iterator for the Variable Domain set.
  Slist<const Domain*>::iterator begin_domains() const {
    return const_cast<L2_string_map*>(this)->domains.begin();
  }

  /// End iterator for the Variable Domain set.
  Slist<const Domain*>::iterator end_domains() const {
    return const_cast<L2_string_map*>(this)->domains.end();
  }

  // used mainly by the listener

  /**
   * Create a mapping from Variable ID to Variable and from Variable ID to
   * Transition
   */
  void create_domains(const L2_file& l2_file);

  /// Set the mapping for the L2rVariable and the Variable.
  void add_variable(const L2rVariable* pL2rVariable, Variable& variable);


  /// Set the mapping for the Variable.
  void add_variable(Variable& variable);

  /// Remove the mapping for the Variable.
  void remove_variable(const Variable& variable);

  /**
   * Form the name in a consistent manner across the program.
   * For time steps other than now, prepend the time step with '#' separator.
   */
  static MBA_string form_name_at_time(const MBA_string& name,
				      unsigned timestep);

  // The Clause-component mapping is expensive to maintain and is currently
  // unimplemented.

#if 0
 private:
  /**
   * Map clauses to components.
   * Filled in
   *  - when to_t_system creates a variable (the mutex clauses)
   *  - when to_t_system creates a background clause
   *  - when t_system progresses
   *    -- creates vars, new bg clauses
   */
  Hash_table<Clause*, MBA_string> clause2whence;
  
 public:
  /// Not implemented.
  MBA_string get_component_of_clause(Clause* pClause);
  /// Not implemented.
  void add_clause(Clause* pClause, MBA_string component);
  /// Not Implemented.
  void remove_clause(Clause* pClause);
#endif

 private:
  /**
   * Map Propositions to a (Variable *, valueIndex) Pair object.
   * We rarely actually access the string representations, so we generate them
   * only as needed.
   * Filled in on any add_variable().
   * Removed on remove_variable().
   */
  Hash_table<Proposition*, MBApair<Variable*, unsigned> > prop2varval;

 public:
  /**
   * Return a (Variable*, valueIndex) Pair object for the Proposition.
   * \param pProposition a pointer to a Proposition
   * \return a (Variable*, valueIndex) Pair for the Proposition (copy, not
   * reference)
   */
  MBApair<Variable*, unsigned>
    get_assignment_of_prop(Proposition* pProposition);

  /**
   * Return "<variableName>=<valueName>" as print name for the Proposition.
   * \param pProposition a pointer to the Proposition
   * \return the print name of the Proposition
   */
  MBA_string get_name_of_prop(Proposition* pProposition);
  
  /**
   * Get Proposition that asserts "<variableName>=<valueName>".
   * \param propositionName the print name of the Proposition
   * \return the Proposition with the given print name if it exists; otherwise
   * NULL
   */
  Proposition *get_prop_by_name(const MBA_string& propositionName);

  /**
   * Remove the Proposition from the name mapping.
   * \param pProposition a pointer to the Proposition to remove
   */
  void remove_prop(Proposition* pProposition);

  /**
   * When the State Variable's domain cardinality has changed, replace the
   * State Variable's domain object.
   * \param size the domain cardinality
   * \todo This could change every time step. This might be an opportunity for
   * lazy evaluation of Domain::values.
   */

  void revise_state_domain(unsigned size);
};

#endif
