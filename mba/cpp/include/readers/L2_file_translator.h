/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_file_translator.h,v 1.18 2004/01/28 23:31:38 lbrown Exp $ */
#ifndef L2_file_translator_HEADER
#define L2_file_translator_HEADER

#include <livingstone/L2_iostream.h>
#include <mba_utils/hash_table.h>

class L2_file;
class L2rClause;
class L2rEnumeration;
class L2rProposition;
class L2rTransition;
class L2rVariable;

/**
 * A class that takes an L2_file, processes it, and outputs another L2_file.
 * For instance, stripping the file of debugging info, sorting the 
 * L2rPropositions, or optimizing out certain redundant elements
 * (L2rClause, L2rEnumeration, L2rPropostion, L2rVariable).
 *
 * We keep the mapping from L2rVariables in the source to L2rVariables in the
 * destination; this mapping may be many-to-one.
 *
 * The work should be done in the translate function, which should be called
 * only once.  Any arguments it will need should be passed in the constructor.
 * The exception is the source and destination, which may be specified
 * later. This is needed to easily hook up a list of translations.
 */

class L2_file_translator : public Pooled {
 public:

  /// 
  enum public_constants { GET_FROM_SOURCE = -1 };

  /// Perform the translation. Override to transform.
  virtual bool translate();

  /// Return the source L2_file.
  const L2_file *get_source() const { return source_; }

  /// Return the destination L2_file.
  const L2_file *get_dest()   const { return dest_;   }

  /// Set the source L2_file, if it hasn't already been set to something else.
  void set_source(const L2_file *pSource) {
    L2_assert(!get_source() || get_source() == pSource,
	      L2_fatal_error,
	      ("source of translation is already set"));
    source_ = pSource;
  }

  /**
   * Set the destination L2_file, if it hasn't already been set to something
   * else.
   */
  void set_dest(L2_file *pDestination) {
    L2_assert(!get_dest() || get_dest() == pDestination, L2_fatal_error,
	      ("destination of translation is already set"));
    dest_ = pDestination;
  }

  /// Set the verbose mode
  void setVerbose(bool v) { isVerbose_ = v; }

  /// Is it in verbose mode?
  bool isVerbose() const { return isVerbose_; }

  /// Set the ostream to which verbose output is sent.
  void set_output(_STD_ ostream& o) { out = &o; }

  /// Return the ostream to which verbose output is sent.
  _STD_ ostream& get_output() { return *out; }

  // We need to store mappings from the source to the destination.
  // Each item in the source maps to one or zero elements in the destination.
  // We return null if it maps to nothing.
  // Each item in the destination maps to any number of elements in the
  // source (including zero).  We fill in the list appropriately.

  /**
   * Return the destination L2Enumeration that corresponds to the source
   * L2rEnumeration, if there is one.
   */
  const L2rEnumeration *get_dest_enum(const L2rEnumeration *src);
  /**
   * Return the set of source L2rEnumerations that correspond to the
   * destination L2rEnumeration.
   */
  void get_source_enums(const L2rEnumeration *dest,
			Slist<const L2rEnumeration*>& srcs);

  /**
   * Return the destination L2rVariable that corresponds to the source
   * L2rVariable, if there is one.
   */
  const L2rVariable *get_dest_var(const L2rVariable *src);
  /**
   * Return the set of source L2rVariables that correspond to the
   * destination L2rVariable.
   */
  void get_source_vars(const L2rVariable *dest,
		       Slist<const L2rVariable*>& srcs);

  /**
   * Return the destination L2rProposition that corresponds to the source
   * L2rProposition, if there is one.
   */
  const L2rProposition *get_dest_prop(const L2rProposition *src);

  /**
   * Return the set of source L2rPropositions that correspond to the
   * destination L2rProposition.
   */
  void get_source_props(const L2rProposition *dest,
			Slist<const L2rProposition*>& srcs);

  /**
   * Return the destination L2rClause that corresponds to the source
   * L2rClause, if there is one.
   */
  const L2rClause *get_dest_clause(const L2rClause *src);
  /**
   * Return the set of source L2rClauses that correspond to the
   * destination L2rClause.
   */
  void get_source_clauses(const L2rClause *dest,
			  Slist<const L2rClause*>& srcs);

  /// Nothing special to do.
  // Must be public so we can delete.
  virtual ~L2_file_translator() { }

 protected:

  /**
   * Specify the source and destination L2_file objects.
   * By default it is not verbose and verbose output goes to _STD_ cout.
   * Protect the constructor to force using subclasses.
   */
  L2_file_translator(const L2_file *source = 0, L2_file *destination = 0) :
  source_(source), dest_(destination), isVerbose_(false),
  fore_enum_map_(hash_integral_key),
  back_enum_map_(hash_integral_key),
  fore_var_map_(hash_integral_key),
  back_var_map_(hash_integral_key),
  fore_prop_map_(hash_integral_key),
  back_prop_map_(hash_integral_key),
  fore_clause_map_(hash_integral_key),
  back_clause_map_(hash_integral_key),
  out(&_STD_ cout) { }

  /// Return the source L2_file.
  const L2_file *source() const { return source_; }

  /// Return the destination L2_file.
  L2_file *dest() { return dest_;   }

  /// Manage the mapping of L2rEnumeration from source to destination.
  void add_mapping(const L2rEnumeration *from, const L2rEnumeration *to);

  /// Manage the mapping of L2rProposition from source to destination.
  void add_mapping(const L2rProposition *from, const L2rProposition *to);

  /**
   * Manage the mapping of L2rClause from source to destination.
   * Check the flags in 'from' and copy them into 'to'.
   * This avoids a common bug, but requires non-const second arg.
   */
  void add_mapping(const L2rClause *from, L2rClause *to);

  /**
   * Manage the mapping of L2rVariable from source to destination.
   * Check the flags in 'from' and copy them into 'to'.
   * This avoids a common bug, but requires non-const second arg.
   */
  void add_mapping(const L2rVariable *from, L2rVariable *to);

  /**
   * Manage the mapping of L2rClause from source to destination.
   * Doesn't do the flag check, and works with const second arg.
   */
  void add_mapping_unchecked(const L2rClause *from,
			     const L2rClause *to);

  /**
   * Manage the mapping of L2rVariable from source to destination.
   * Doesn't do the flag check, and works with const second arg.
   */
  void add_mapping_unchecked(const L2rVariable *from,
			     const L2rVariable *to);

  /**
   * Copy an L2rEnumeration from source to destination, mapping any fields that
   * need to be.
   * The ID stays the same unless specified otherwise by passing in a positive
   * ID. We maintain debug info.
   */
  virtual L2rEnumeration *copy_enum(const L2rEnumeration*,
				    int newID = GET_FROM_SOURCE,
				    bool add_to_dest = true);

  /**
   * Copy an L2rVariable from source to destination, mapping any fields that
   * need to be.
   * The ID stays the same unless specified otherwise by passing in a positive
   * ID. We maintain debug info.
   */
  virtual L2rVariable *copy_var(const L2rVariable*,
				int newID = GET_FROM_SOURCE,
				bool add_to_dest = true);

  /**
   * Copy an L2rProposition from source to destination, mapping any fields that
   * need to be.
   * The ID stays the same unless specified otherwise by passing in a positive
   * ID. We maintain debug info.
   */
  virtual L2rProposition *copy_prop(const L2rProposition*,
				    int newID = GET_FROM_SOURCE,
				    bool add_to_dest = true);

  /**
   * Copy an L2rClause from source to destination, mapping any fields that
   * need to be.
   * The ID stays the same unless specified otherwise by passing in a positive
   * ID. We maintain debug info.
   */
  virtual L2rClause *copy_clause(const L2rClause*,
				 int newID = GET_FROM_SOURCE,
				 bool add_to_dest = true);

  /**
   * Copy an L2rTransition from source to destination, mapping any fields that
   * need to be.
   * The ID stays the same unless specified otherwise by passing in a positive
   * ID. We maintain debug info.
   */
  virtual L2rTransition *copy_transition(const L2rTransition*,
					 bool add_to_dest = true);

  // The default implementations just map everything over.
  // IDs stay the same; anything that needs to be is mapped correctly.
  // Debug information is copied if available.

  /**
   * Copy the L2rEnumerations from source to destination. May be overridden.
   * IDs stay the same; anything that needs to be is mapped correctly.
   * Debug information is copied if available.
   */
  virtual void copy_all_enums();

  /**
   * Copy the L2rVariables from source to destination. Override to transform.
   * IDs stay the same; anything that needs to be is mapped correctly.
   * Debug information is copied if available.
   */
  virtual void copy_all_vars();

  /**
   * Copy the L2rPropositions from source to destination. Override to
   * transform.
   * IDs stay the same; anything that needs to be is mapped correctly.
   * Debug information is copied if available.
   */
  virtual void copy_all_props();

  /**
   * Copy the L2rClauses from source to destination. Override to transform.
   * IDs stay the same; anything that needs to be is mapped correctly.
   * Debug information is copied if available.
   */
  virtual void copy_all_clauses();

  /**
   * Copy the L2rTransitions from source to destination. Override to transform.
   * IDs stay the same; anything that needs to be is mapped correctly.
   * Debug information is copied if available.
   */
  virtual void copy_all_transitions();

 private:
  /// The input L2_file.
  const L2_file *source_;
  /// The output L2_file.
  L2_file *dest_;
  /// Whether the translator is in verbose mode.
  bool isVerbose_;

  /// Map input L2rEnumeration to output L2rEnumeration.
  Hash_table<const L2rEnumeration*, const L2rEnumeration*> fore_enum_map_;
  /// Map output L2rEnumeration to input L2rEnumeration.
  Hash_table<const L2rEnumeration*, Slist<const L2rEnumeration*> >
    back_enum_map_;
  
  /// Map input L2rVariable to output L2rVariable.
  Hash_table<const L2rVariable*, const L2rVariable*> fore_var_map_;
  /// Map output L2rVariable to input L2rVariable.
  Hash_table<const L2rVariable*, Slist<const L2rVariable*> > back_var_map_;
  
  /// Map input L2rProposition to output L2rProposition.
  Hash_table<const L2rProposition*, const L2rProposition*> fore_prop_map_;
  /// Map output L2rProposition to input L2rProposition.
  Hash_table<const L2rProposition*, Slist<const L2rProposition*> >
    back_prop_map_;

  /// Map input L2rClause to output L2rClause.
  Hash_table<const L2rClause*, const L2rClause*> fore_clause_map_;
  /// Map output L2rClause to input L2rClause.
  Hash_table<const L2rClause*, Slist<const L2rClause*> > back_clause_map_;

  /// The ostream to which verbose output is written.
  _STD_ ostream *out;
};

#endif
