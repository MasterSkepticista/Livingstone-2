/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: variable.h,v 1.22 2004/01/28 23:31:41 lbrown Exp $ */

#ifndef variable_HEADER
#define variable_HEADER

#include <readers/enumeration.h>
#include <readers/modes.h>
#include <readers/varkind.h>

#ifdef ENABLE_L2_VERBOSE
class L2rVariable;
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rVariable *v);
#endif

class L2rMode;

/**
 * A Variable in the L2_file, after reading from the model file and before
 * writing to the T_system.
 */

class L2rVariable : public Pooled {
 public:
  /// Symbolic constant for Variable without an initial value.
  enum public_constants { NO_INITIAL_VALUE = -1 };

 private:

  /// Unique ID number.
  unsigned varID_;
  /// The data type (domain) is an enumeration type.
  const L2rEnumeration *enum_;
  /// Index of initial value; NO_INITIAL_VALUE if unknown.
  int initial_;
  /// What kind of variable it is (who sets the value).
  VarKind kind_;
  /// only if kind == vk_mode.
  L2rMode *mode_;
  /// A backup of initial_ for restoration
  int savedInitial_;

 public:
  /// Specify the ID and domain (enumeration data type).
  L2rVariable(unsigned v, const L2rEnumeration *e) :
  varID_(v), enum_(e), initial_(NO_INITIAL_VALUE), kind_(vk_unknown),
    mode_(NULL) { }

  /**
   * If this is a mode, delete the L2rMode object.
   * The only subclass is dbg_L2rVariable ==> non-virtual dtor if it's
   * disabled.
   */
  l2_virtual ~L2rVariable() { if (kind() == vk_mode) { delete mode_; } }

  /**
   * Set whether Command, Observable, Transitioned or something else.
   * If it is a mode, allocate an L2rMode to hold an L2rTransition set.
   */
  void setKind(VarKind vk);

  /**
   * Set the initial value of the Variable, if it hasn't already been set to
   * something else.
   */
  void setInitial(int i);

  /**
   * This is like setInitial, except it saves and then forcibly over-rides the
   * current value
   */
  void shadowInitial(int newInitialValueIndex);

  /**
   * Used in conjuction with shadowInitial, this restores the saved value.
   */
  void restoreInitial();

  /// Return the unique ID.
  unsigned id() const { return varID_; }

  /// Return the domain (enumeration data type).
  const L2rEnumeration *type() const { return enum_; }

  /// Return the L2rVariable subtype.
  VarKind kind() const { return kind_; }

  /// Return the index of the initial value.
  int initial() const { return initial_; }
  
  /// Return the L2rMode object as a read-only pointer.
  const L2rMode *mode() const { 
    // assumes mode_friend is const except for returning non-const
    return const_cast<L2rVariable*>(this)->mode_friend();
  }

  /// Return the L2rMode object.
  L2rMode *mode_friend() { 
    // const except we return non-const
    L2_assert(kind_ == vk_mode,
	      L2_fatal_error,
	      ("Reading L2rVariable::mode_ for non-mode variable"));
    return mode_; 
  }

#ifdef ENABLE_L2_DEBUG_SECTIONS
  /// Is this the debugging version? No, it isn't.
  l2_virtual bool hasDebug() const { return false; }
#endif

#ifdef ENABLE_L2_VERBOSE
  /// Write the L2rVariable to the _STD_ ostream.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os  << "Variable "  << id() << ": "	<< " type " << type();
    if (initial() != NO_INITIAL_VALUE) {
      os << ", initial value "; 
      type()->printMember(os, initial());
    }
    os << _STD_ endl;
  }

  l2_virtual void toOStream(_STD_ ostream& os) const {
    os << varID_;
  }
#endif
};

#ifdef ENABLE_L2_DEBUG_SECTIONS

/**
 * This subclass of L2rVariable is decorated with a name string.
 */

class dbg_L2rVariable : public L2rVariable {
 private:
  /// The print name string.
  MBA_string name_;

 public:
  /// The debugging subclass also a name string.
  dbg_L2rVariable(unsigned v, const L2rEnumeration *e, MBA_string n) 
    : L2rVariable(v, e), name_(n) { }

  /// Nothing special to do.
  // Unused except for eventual subclasses, and for static array.
  l2_virtual ~dbg_L2rVariable() { }

  /// Is this the debugging version? Yes, it is.
  l2_virtual bool hasDebug() const { return true; }

  /// Return the name string of the L2rVariable.
  MBA_string name() const { return name_; }

#ifdef ENABLE_L2_VERBOSE
  /// Write the ID, name and type of the L2rVariable to the _STD_ ostream.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os  << "Variable "  << id() << ": " << name()
	<< " has type " << type();
    if(initial() != NO_INITIAL_VALUE) {
      os << ", initial value " ;
      type()->printMember(os, initial());
    }
    os << _STD_ endl;
  }

  /// Write the name of the L2rVariable to the _STD_ ostream.
  l2_virtual void toOStream(_STD_ ostream& os) const { os << name(); }
#endif
};
#endif

#ifdef ENABLE_L2_VERBOSE
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rVariable& v) {
  v.toOStream(os);
  return os;
}

inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rVariable *v) {
  if (!v)
    os << "(nil var)";
  else
    v->toOStream(os);
  return os;
}
#endif

#endif
