#include <conflict_db/conflict_db.h>
#include <mba_utils/array.h>
#include <transition/T_system.h>

/**
 * Export a supplied LTMS to DIMACS cnf format.
 */
class DIMACSExporter {
 public:

  /**
   * Supply the Conflict_db, the ostream to which the DIMACS representation is
   * written, and the modelFilePathname.
   * \param conflict_db The Conflict_db that contains Variables
   * \param os The output stream to which the theory is exported
   * \param modelFilePathname The pathname, written as a comment.
   */

  DIMACSExporter(Conflict_db& conflict_db, ostream& os,
		 const char* modelFilePathname) :
    conflict_db(conflict_db), ltms(*(conflict_db.get_solver())), os(os),
    modelFilePathname(modelFilePathname) { }

  /**
   * Return the number of clauses to be exported.
   */

  unsigned getClauseCount() const {
    return clauses.size() + // non-singletons
      initialModes.size() + futureObservations.size(); // singletons
  }

  /**
   * Return the number of propositional variables.
   * This is the same as the number of Variables in the Conflict_db and the
   * number of Propositions in the LTMS.
   */

  unsigned getVariableCount() const {
    return conflict_db.get_solver()->num_propositions();
  }

  /**
   * Return the number of clauses in the theory.
   */

  /**
   * A Clause is a singleton if it has one literal (Proposition)
   * \param clause The Clause
   * \return whether the Clause is a singleton
   */
  bool singleton(const Clause& clause) const {
    // All non-singletons are exportable
    return ((clause.number_of_positive_literals() +
	     clause.number_of_negative_literals()) == 1);
  }

  /**
   * Export the entire contents of the LTMS to os.
   * \internal This can't be called 'export' because that's a C++ keyword.
   */
  void exportTheory() {
    // Build the intermediate data structure
    buildTheory();
    // Retrieve the theory
    int* wff = getWff();
    unsigned variableCount = getVariableCount();
    unsigned clauseCount = getClauseCount();
    // Write the theory to the ostream
    writeHeader(variableCount, clauseCount);
    int i = 0;
    for (unsigned c = 0; c < clauseCount; c++) {
      do {
	os << wff[i++] << " ";
      } while (wff[i] != 0);
      os << 0 << endl;
      i++;
    }
    // De-allocate
    delete [] wff;
  }

  /**
   * The size of the array is the number of propositions in all the clauses,
   * plus the zero terminators (one per clause)
   */

  unsigned wffArraySize() {
    unsigned arraySize = 0;
    for (Array<Clause *>::iterator it = clauses.begin();
	 it != clauses.end(); ++it) {
      Clause *pClause = *it;
      arraySize += pClause->number_of_literals();
    }
    return arraySize +
      1 * clauses.size() +
      2 * initialModes.size() +
      2 * futureObservations.size();
  }
 
  /**
   * Return a wff data structure. This is the sequence of integers that follows
   * the header in a DIMACS cnf format file.
   */
  int* getWff() {
    unsigned arraySize = wffArraySize();
    int* wff = new int[arraySize];
    unsigned insert = 0;
    {
      for (Array<Clause *>::iterator it = clauses.begin(); it != clauses.end();
	   ++it) {
	assert(insert < arraySize);
	Clause* pClause = *it;
	// Insert each positive Proposition
	{
	  for (Clause::iterator pit = pClause->begin_positive();
	       pit != pClause->end_positive(); ++pit) {
	    Proposition* pProposition = *pit;
	    wff[insert++] = propositionIndex(*pProposition);
	  }
	}
	// Insert each negative Proposition
	{
	  for (Clause::iterator pit = pClause->begin_negative();
	       pit != pClause->end_negative(); ++pit) {
	    Proposition* pProposition = *pit;
	    wff[insert++] = -propositionIndex(*pProposition);
	  }
	}
	// Insert the Clause-terminating '0' and newline
	wff[insert++] = 0;
      }
    }
    // Insert each initial mode
    {
      for (Array<Variable *>::iterator it = initialModes.begin();
	   it != initialModes.end(); ++it) {
	Variable* pVariable = *it;
	Proposition* pProposition = pVariable->get_value();
	wff[insert++] = propositionIndex(*pProposition);
	// Write the Clause-terminating '0' and newline
	wff[insert++] = 0;
      }
    }
    // Insert each future observation
    {
      for (Array<Variable *>::iterator it = futureObservations.begin();
	   it != futureObservations.end(); ++it) {
	Variable* pVariable = *it;
	// The value has not been propagated yet
	assert(pVariable->is_assignable());
	const Assignment* pAssignment =
	  static_cast<Assignable *>(pVariable)->get_current();
	Proposition* pProposition = NULL;
	if (pAssignment == NULL) {
	  // There is no current Assignment; use its current value
	  pProposition = pVariable->get_value();
	} else {
	  // There is a current Assignment; look up its Proposition
	  unsigned index = pAssignment->get_index();
	  pProposition = pVariable->get_prop(index);
	}
	assert(pProposition != NULL);
	wff[insert++] = propositionIndex(*pProposition);
	// Write the Clause-terminating '0' and newline
	wff[insert++] = 0;
      }
    }
    assert(insert == arraySize);
    return wff;
  }

  /**
   * Copy the theory to be exported into an intermediate representation
   */

  void buildTheory() {
    // Copy the non-singleton Clauses
    {
      for (Ptheory::clause_list::iterator it = ltms.begin_clauses();
	   it != ltms.end_clauses(); ++it) {
	Clause *pClause = *it;
	if (!singleton(*pClause)) { addClause(*pClause); }
      }
    }
    // Copy the Variables for the singleton Clauses
    {
      for (Variable::ListOfVariable::iterator it =
	     conflict_db.begin_variables();
	   it != conflict_db.end_variables(); ++it) {
	Variable* pVariable = *it;
	Proposition *pProposition = pVariable->get_value();
	if (pProposition != NULL) {
	  if (isInitialMode(*pVariable)) {
	    addInitialMode(*pVariable);
	  } else if (isCurrentObservable(*pVariable)) {
	    addFutureObservation(*pVariable);
	  }
	}
      }
    }
  }

  /**
   * Add a Clause to the theory.
   * It is assumed that the Clause is not a singleton.
   * \param clause the Clause to add
   */
  void addClause(Clause& clause) { clauses.push_back(&clause); }

  /**
   * Add an initial mode to the theory.
   * It is assumed that the time step is 1.
   * \param initialMode the initial mode to add 
   */
  void addInitialMode(Variable& initialMode) {
    initialModes.push_back(&initialMode);
  }

  /**
   * Add a future observation to the theory.
   * It is assumed that the time step is T_system::NOW.
   * \param futureObservation the future observation to add.
   */
  void addFutureObservation(Variable& futureObservation) {
    futureObservations.push_back(&futureObservation);
  }

  /**
   * Is the Variable a mode (Transitioned) at time step 1?
   * \param variable the Variable
   * \return whether the Variable is a mode at time step 1
   * Only at the NOW time step are modes of type Transitioned; otherwise they
   * are of type Dependent
   */
  bool isInitialMode(Variable& variable) {
    // Only Dependents can be modes
    if (variable.get_time_step() == (unsigned)1 && variable.is_dependent()) {
      for (Variable* pVariable = &variable; pVariable != NULL;
	   pVariable = pVariable->get_next()) {
	if (pVariable->is_transitioned()) {
	  // It or a successor is a Transitioned
	  return true;
	}
      }
    }
    return false;
  }

  /**
   * Is the Variable an Observable from the NOW time step?
   * \param variable the Variable
   * \return whether the Variable is an Observable from the NOW time step
   */
  bool isCurrentObservable(Variable& variable) {
    return variable.is_observable() &&
      variable.get_time_step() == T_system::NOW;
  }

  /**
   * The number of the Proposition is its 1-based index in LTMS::propositions.
   * \param proposition The Proposition the index of which is sought
   * \return The index of the Proposition, or 0 if it is not found in the
   * LTMS's list of Propositions
   * \internal Building a hash table would reduce this from O(n), but it's a
   * fair amount of work.
   */
  int propositionIndex(const Proposition& proposition) const {
    unsigned index = 1;
    for (Ptheory::prop_list::iterator it = ltms.begin_propositions();
	 it != ltms.end_propositions(); ++it, ++index) {
      Proposition *pProposition = *it;
      if (pProposition == &proposition) {
	return index;
      }
    }
    return 0; // not found
  }

 private:

  /**
   * Write a single Proposition.
   * \param proposition The Proposition to write
   * \param positive True if the Proposition is positive (i.e., not negated)
   */
  void writeProposition(const Proposition& proposition, bool positive) {
    os << propositionIndex(proposition) * (positive ? 1 : -1) << " ";
  }

  /**
   * Write a single Clause.
   * First write the positive Propositions, then the negative Propositions, the
   * terminating '0' and finally the newline.
   * \param clause The Clause to write.
   */
  void writeClause(const Clause& clause) {
    // Write each positive Proposition
    {
      for (Clause::iterator pit = clause.begin_positive();
	   pit != clause.end_positive(); ++pit) {
	writeProposition(**pit, true);
      }
    }
    // Write each negative Proposition
    {
      for (Clause::iterator pit = clause.begin_negative();
	   pit != clause.end_negative(); ++pit) {
	writeProposition(**pit, false);
      }
    }
    // Write the Clause-terminating '0' and newline
    os << '0' << endl;
  }

  /// Write a comment line and the problem line
  void writeHeader(unsigned variableCount, unsigned clauseCount) {
    // Write the model file pathname to a comment
    os << "c Livingstone model '" << modelFilePathname << "'" << endl;
    // Write the problem line
    os << "p cnf " << variableCount << " " << clauseCount << endl;
  }

 private:
  /** The Conflict_db */
  Conflict_db& conflict_db;

  /** The LTMS to be exported. */
  LTMS& ltms;

  /** The ostream to which to export the LMTS. */
  ostream& os;

  /** The pathname of the model file, written as a comment. */
  const char* modelFilePathname;

  /** Incrementally built Array of Clause pointers */
  Array<Clause *> clauses;

  /** Incrementally build Array of initial modes */
  Array<Variable *> initialModes;

  /** Incremenatally build Array of future observations */
  Array<Variable *> futureObservations;
};
