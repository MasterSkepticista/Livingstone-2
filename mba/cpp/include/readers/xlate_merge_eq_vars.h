/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xlate_merge_eq_vars.h,v 1.12 2004/02/06 18:53:27 lbrown Exp $ */
#ifndef xlate_merge_eq_vars_HEADER
#define xlate_merge_eq_vars_HEADER

#include "common.h"
#include "L2_file_translator.h"
#ifdef ENABLE_L2_DEBUG_SECTIONS
#include <mba_utils/array.h>
#include <mba_utils/hash_table.h>
#endif

class L2_file;
class L2rClause;
class L2rVariable;
class L2rPropVarVar;
class L2rPropVarValue;

/**
 * Optimize the source L2_file by merging Variables whose values are
 * constrained to be equal.
 * It does this by constructing equivalence classes of L2rVariables and
 * retaining only one "representative" from each equivalence class.
 */

class xlate_merge_eq_vars : public L2_file_translator {
 public:
  /// Specify the source and destination L2_file objects.
  xlate_merge_eq_vars(const L2_file *source=0, L2_file *destination=0) :
    L2_file_translator(source, destination), variableHashMap(hashpjw) { }

  /// Delete the set of equivalence classes generated during optimization.
  virtual ~xlate_merge_eq_vars();

 protected:
  /// Invoke the optimizer.
  virtual void copy_all_vars();

 private:
  // Declared in the *.cpp file
  class EqClass;

  /// The set of all equivalence classes.
  Slist<EqClass*> eq_classes;

  /// Find all the equivalence classes in the model, filling in eq_classes.
  void find_equivalences();

  /**
   * If the L2rClause has exactly on L2rProposition and it asserts that
   * Variable1=Variable2, return it.
   */
  const L2rPropVarVar* get_equivalence(const L2rClause* cl);

  /**
   * Return an equivalence class that contains the L2rVariable. If one
   * does not already exist, create it.
   */
  EqClass* get_equivalence_class(const L2rVariable *var);
  
  void compute_equivalence_classes(Slist<const L2rPropVarVar*>&);

#ifdef ENABLE_L2_DEBUG_SECTIONS
 public:
  /// Copy the contents of optimizedAwayVariableNames into the parameter.
  void getOptimizedAwayVariableNames(Array<MBA_string, true>& array);

  /// Copy the contents of variableHashMap into the parameter.
  void getVariableHashMap(Hash_table<MBA_string, MBA_string>& hash_table);

 private:
  /**
   * Add the name of the L2rVariable.
   * This is called for all L2rVariables that have been optimized away.
   */
  void addOptimizedAwayVariableName(MBA_string& variableName) {
    optimizedAwayVariableNames.grow_and_set(optimizedAwayVariableNames.size(),
					    variableName);
  }

  /// Map the from L2rVariable (optimized away) to the to L2rVariable.
  void setVariableMapping(MBA_string& from, MBA_string& to) {
    variableHashMap.insert(from, to);
  }

  /// Associates optimized-away variables with their representatives.
  Hash_table<MBA_string, MBA_string> variableHashMap;

  /// The set of all names of all Variables optimized away.
  Array<MBA_string, true> optimizedAwayVariableNames;
#endif
};

#endif
