/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: proposition.h,v 1.20 2004/01/28 23:31:40 lbrown Exp $ */
#ifndef proposition_HEADER
#define proposition_HEADER

#include <readers/variable.h>

/**
 * A Proposition in the L2_file, after reading from the model file and before
 * writing to the T_system.
 *
 * This is an abstract class. An L2rProposition must be one of its subclasses,
 * L2rPropVarValue (Variable=Value) or L2rPropVarVar (Variable1=Variable2).
 */

class L2rProposition : public Pooled {
 private:
  /// Unique ID
  unsigned propID_;

  /// The Variable; Variable1 if Variable1=Variable2
  const L2rVariable *var_;

  /// Whether this is a positive literal
  bool positive_;

 public:
  /**
   * Specify the ID, the L2rVariable to whose value is being asserted, and
   * whether it is a positive literal.
   * Since this class is abstract, this constructor handles functionality
   * common to its subclasses.
   */
  L2rProposition(unsigned id, const L2rVariable *var, bool pos) :
    propID_(id), var_(var), positive_(pos) { }

  /// Even without debugging, there are subclasses here.
  virtual ~L2rProposition() { }

  /**
   * Check whether the two L2rPropositions are identical: same L2rVariable,
   * same sign, either both or neither equality.
   * This is common functionality. Subclasses override to check the additional
   * info they have.
   */
  virtual bool operator == (const L2rProposition& other) const {
    // ID doesn't matter
    return
      (this->var()        == other.var())        &&
      (this->isPositive() == other.isPositive()) &&
      (this->isEquality() == other.isEquality());
  }

  /// Set the unique ID if it hasn't already been set to something else.
  void setID(unsigned id) {
    // Once set, don't let it change
    L2_assert(this->id() == 0 || this->id() == id,
	      L2_fatal_error,
	      ("changing prop id from " + MBA_string(this->id()) +
	       " to " + MBA_string(id)));
    propID_ = id;
  }

  /// Return the unique ID.
  unsigned id()            const { return propID_  ; }

  /// Return the L2rVariable to which the L2rProposition refers.
  const L2rVariable *var() const { return var_     ; }

  /// Is the L2rProposition positive?
  bool  isPositive()       const { return positive_; }

  /**
   * Is this a Variable1=Variable2 L2rProposition?
   * This makes the class abstract (pure virtual).
   */
  virtual bool isEquality() const = 0;

#ifdef ENABLE_L2_VERBOSE
  /// Write the L2rProposition to ostream with the ID.
  virtual void toOStream_long(_STD_ ostream& os) const = 0;

  /// Write the L2rProposition to ostream.
  virtual void toOStream(_STD_ ostream& os) const = 0;
#endif
};


/**
 * This subclass of L2rProposition is for a proposition that Variable=Value
 */

class L2rPropVarValue : public L2rProposition {
 private:
  /// The index of the value into its domain.
  unsigned valueID_;

 public:
  /// For a Variable=value L2rProposition, supply the value.
  L2rPropVarValue(unsigned id, const L2rVariable *var, bool pos,
		  unsigned value) :
    L2rProposition(id, var, pos), valueID_(value) { }

  /// Nothing special to do.
  virtual ~L2rPropVarValue() { }

  /**
   * Two Variable=value propositions are equal if the variables are equal,
   * the values are equal, neither is an equality proposition, and both have
   * the same sign.
   */
  virtual bool operator == (const L2rProposition& other) const {
    return
      // Base class equality
      (L2rProposition::operator == (other)) &&
      // other is now known to be L2rPropVarValue
      (this->value() == static_cast<const L2rPropVarValue &>(other).value());
  }

  /// Return the value of the variable that this proposition asserts.
  unsigned value() const { return valueID_ ; }

  /// Is this a Variable1=Variable2 proposition? No, it isn't.
  virtual bool isEquality() const { return false; }

#ifdef ENABLE_L2_VERBOSE
  /// Write the proposition to ostream with the ID.
  virtual void toOStream_long(_STD_ ostream& os) const {
    os << "Proposition " << id() << ": ";
    toOStream(os);
    os << _STD_ endl;
  }

  /// Write the proposition to ostream.
  virtual void toOStream(_STD_ ostream& os) const {
    os << var() << (isPositive() ? " = " : " != ");
    var()->type()->printMember(os, value());
  }
#endif
};


/**
 * This subclass of L2rProposition is for a proposition that
 * Variable1=Variable2.
 */

class L2rPropVarVar : public L2rProposition {
 private:
  /// Variable2 in the assertion Variable1=Variable2.
  // We guarantee (via the constructor) to haave other_var > var().
  const L2rVariable *other_var_;

 public:
  /**
   * For a Variable1=Variable2 L2rProposition, supply the second L2rVariable.
   * The primary L2rVariable is the one with the lesser ID.
   */
  L2rPropVarVar(unsigned id,
		const L2rVariable *variable1,
		bool pos,
		const L2rVariable *variable2) :
    L2rProposition(id,
		   (variable1->id() < variable2->id()) ? variable1 : variable2,
		   pos),
    other_var_((variable1->id() < variable2->id()) ? variable2 : variable1) { }

  /// Nothing special to do.
  // Unused except for eventual subclasses, and for static array.
  virtual ~L2rPropVarVar() { }

  /**
   * Two Variable1=Variable2 L2rPropositions are equal if the primary 
   * L2rVariables are equal, the secondary L2rVariables are equal, both are
   * equality L2rPropositions and both have the same sign.
   */
  virtual bool operator == (const L2rProposition& other) const {
    return
      // Base class equality
      (L2rProposition::operator == (other)) &&
      // other is now known to be L2rPropVarVar
      (otherVar() == static_cast<const L2rPropVarVar&>(other).otherVar());
  }

  /// Return the secondary L2rVariable.
  const L2rVariable *otherVar() const { return other_var_ ; }

  /// Is this a Variable1=Variable2 L2rProposition? Yes, it is.
  virtual bool isEquality() const { return true; }

#ifdef ENABLE_L2_VERBOSE
  /// Write the L2rProposition to ostream with the ID.
  virtual void toOStream_long(_STD_ ostream& os) const {
    os << "Proposition " << id() << ": ";
    toOStream(os);
    os << _STD_ endl;
  }

  /// Write the propsition to the otream.
  virtual void toOStream(_STD_ ostream& os) const {
    os << var() << (isPositive() ? " = " : " != ") << otherVar();
  }
#endif
};

_STD_ ostream& operator<<(_STD_ ostream& os, const L2rPropVarVar& l2rPropVarVar);

#ifdef ENABLE_L2_VERBOSE
/// Write the L2rProposition to ostream.
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rProposition& p) {
  p.toOStream(os);
  return os;
}

/// Write the L2rProposition pointed to to ostream.
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rProposition *p) {
  if (!p) {
    os << "(nil prop)";
  } else {
    p->toOStream(os);
  }
  return os;
}
#endif

#endif
