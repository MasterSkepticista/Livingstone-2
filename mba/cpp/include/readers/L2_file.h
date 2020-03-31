/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file.h,v 1.15 2001/12/28 20:04:46 lbrown Exp $ */

#ifndef L2_file_HEADER
#define L2_file_HEADER

// Can't forward-declare enumerations
#include <readers/varkind.h>

// forward declarations
class L2rEnumeration;
class L2rVariable;
class L2rProposition;
class L2rClause;
class L2rTransition;
class T_system;

/**
 * This is an in-memory representation of a model that facilitates
 * optimiziation and communication betwen the Livingstone engine and the file
 * system.
 * All subclasses of L2_reader populate a destination L2_file object. All
 * xlate_* translators translate from one L2_file object to another. All
 * writers populate their destinations from a source L2_file object.
 */

class L2_file : public Pooled {
 private:

  /// The number of L2rEnumeration objects.
  unsigned nenums_;

  /**
   * The set of all L2rEnumeration objects, each a domain of one or more
   * Variables, implemented as an array.
   */
  L2rEnumeration **enums_;

  /// The number of L2rVariable objects.
  unsigned nvars_;

  /// The set of all L2rVariable objects, implemented as an array.
  L2rVariable **vars_;

  /// The number of L2rProposition objects.
  unsigned nprops_;

  /**
   * The set of all L2rProposition objects refered to in the L2rClause
   * objects, implemented as an array.
   */
  L2rProposition **props_;

  /// The number of L2rClause objects.
  unsigned nclauses_;

  /**
   * The set of all L2rClause objects, implemented as an array.
   * This includes all Clauses refered to in the background model or in
   * Transitions.
   */
  L2rClause **clauses_;

  /// Will the functions will print debugging information while they run?
  bool isVerbose_:1;

  // These are set on the appropriate alloc_* call.

  /**
   * Do the L2rClause objects have debugging information?
   * Set by allocClauses().
   */
  bool dbg_clauses_:1;

  /**
   * Do the L2rEnumerations have debugging information?
   * Set by allocEnums().
   */ 
  bool dbg_enums_:1;

  /**
   * Do the L2rVariables have debugging information?
   * Set by AllocVars().
   */
  bool dbg_vars_:1;

 public:
  /**
   * Initialize sets of L2rEnumeration, L2rVariable, L2rProposition and
   * L2rClause objects to empty; intialize verbosity to false.
   */
  L2_file();

  /**
   * Deallocate the sets of  L2rEnumeration, L2rVariable, L2rProposition and
   * L2rClause objects.
   * No subclasses ==> ok to be non-virtual.
   */
  ~L2_file();

  /// Is operation in verbose mode?
  bool isVerbose() const { return isVerbose_; }

  /// Set whether operation is in verbose mode.
  void setVerbose(bool verbose) { isVerbose_ = verbose; }

  /// Return the ith L2rEnumeration with bounds checking.
  const L2rEnumeration *getEnum(unsigned i) const {
    return bounds_check_access(i, nenums(), enums_, "enumeration");
  }

  /// Return the ith L2rVariable with bounds checking.
  const L2rVariable *getVar(unsigned i) const {
    return bounds_check_access(i, nvars(), vars_, "variable");
  }

  /// Return the ith L2rProposition with bounds checking.
  const L2rProposition *getProp(unsigned i) const {
    return bounds_check_access(i, nprops(), props_, "propositions");
  }

  /// Return the ith L2rClause with bounds checking.
  const L2rClause *getClause(unsigned i) const {
    return bounds_check_access(i, nclauses(), clauses_, "clause");
  }

  /**
   * Return the ith L2rVariable object if i is within range; otherwise 0.
   * Return non-const, needed by the harness file and the commands/
   * modes/observed sections of the l2 file.
   * Only 'friends' can call it, but since any reader may need it, we make it
   * public rather than requiring all readers to be declared as friends above.
   */
  L2rVariable *getVar_friend(unsigned i) { return getVar(i) ? vars_[i] : 0; }

  /// Return the number of L2rEnumeration objects.
  unsigned nenums()       const { return nenums_;  }

  /// Return the number of L2rVariable objects.
  unsigned nvars()        const { return nvars_;   }

  /// Return the number of L2rProposition objects.
  unsigned nprops()       const { return nprops_;  }

  /// Return the number of L2rClause objects.
  unsigned nclauses()     const { return nclauses_;}
  
  /// Is processing of L2rClause objects in debug mode?
  bool dbg_clauses()      const { return dbg_clauses_; }

  /// Is processing of L2rEnumeration objects in debug mode?
  bool dbg_enums()        const { return dbg_enums_; }

  /// Is processing of L2rVariable objects in debug mode?
  bool dbg_vars()         const { return dbg_vars_; }

  /// These are really just for friends, but I don't care so much

  /// Allocate memory for the set of n L2rEnumeration objects.
  void allocEnums  (unsigned n, bool hasDebug);

  /// Allocate memory for the set of n L2rVariable objects.
  void allocVars   (unsigned n, bool hasDebug);

  /// Allocate memory for the set of n L2rProposition objects.
  void allocProps  (unsigned n);

  /// Allocate memory for the set of n L2rClause objects.
  void allocClauses(unsigned n, bool hasDebug);

  /// The ith Enumeration becomes e
  void setEnum  (unsigned i, L2rEnumeration* e)
    { getEnum(i); enums_[i] = e; }

  /// The ith Variable becomes v
  void setVar   (unsigned i, L2rVariable* v)
    { getVar(i);  vars_[i]  = v; }

  /// The ith Proposition becomes p
  void setProp(unsigned i, L2rProposition *p);

  /// The ith Clause becomes c
  void setClause(unsigned i, L2rClause* c)
    { getClause(i); clauses_[i] = c; }

  // Make other changes

  /// Declare the Clause to be in the background
  void addBackground(const L2rClause* c);

  /// Add this Transition to its Mode
  void addTransition(const L2rTransition*);
};

#endif
