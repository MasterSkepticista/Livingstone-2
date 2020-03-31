/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: enumeration.h,v 1.19 2004/02/10 22:36:49 lbrown Exp $ */
#ifndef enumeration_HEADER
#define enumeration_HEADER

#include <readers/common.h>

#ifdef ENABLE_L2_VERBOSE
class L2rEnumeration;
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rEnumeration *e);
#endif

/**
 * A Variable's domain in the L2_file, after reading from the model file and
 * before writing to the T_system.
 *
 * An enumeration is the domain of a variable. It has only two attributes: its
 * serial enumerator and its cardinality.
 */

class L2rEnumeration : public Pooled {
 public:
  /// Save the ID and cardinality.
  L2rEnumeration(unsigned eid, unsigned n) : enumID_(eid), nmembers_(n) { }

  /**
   * Nothing special to do.
   * Note: the only subclass is dbg_*, so make this conditionally virtual.
   */
  l2_virtual ~L2rEnumeration() { }

 public:
  /// Return the unique ID of the domain.
  unsigned id() const { return enumID_; }

  /// Return the cardinality of the domain.
  unsigned nmembers() const { return nmembers_; }
  
  /// Is the index is within the cardinality?
  bool isValidMember(unsigned i) const {
    if (i < nmembers_) return true;
    else {
      _STD_ cerr << "Invalid member ID " << i
		 << " for enumeration " << const_cast<L2rEnumeration*>(this)
		 << _STD_ endl;
      return false;
    }
  }

  /// Write out index i if it is within the cardinality.
  l2_virtual void printMember(_STD_ ostream& os, unsigned i) const {
    if (isValidMember(i)) { os << i; }
  }

#ifdef ENABLE_L2_VERBOSE
  /// Write out the unique ID and cardinality.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os  << "Enumeration "
	<< enumID_
	<< " has "
	<< nmembers_
	<< " members\n";
  }

  /// Write out the unique ID.
  l2_virtual void toOStream(_STD_ ostream& os) const {
    os << enumID_;
  }
#endif

#ifdef ENABLE_L2_DEBUG_SECTIONS
  /// Is this the debugging version? No, it isn't.
  l2_virtual bool hasDebug() const { return false; }
#endif

 private:
  /// Unique ID of the domain.
  unsigned enumID_;
  /// Cardinality of the domain.
  unsigned nmembers_;
};


/**
 * This subclass of L2rEnumeration adds the name of the enumeration and the
 * names of the domain elements.
 */

#ifdef ENABLE_L2_DEBUG_SECTIONS
class dbg_L2rEnumeration : public L2rEnumeration {
 public:
  /// Also supply the names of the domain's element.
  dbg_L2rEnumeration(unsigned eid,
		     unsigned n,
		     const MBA_string& ename,
		     const MBA_string *memnames);

  /// Deallocate the array of element names.
  l2_virtual ~dbg_L2rEnumeration();

  /// Return the name of the domain.
  MBA_string name() const { return enum_name_; }

  /// Return the name of the ith element of the domain, if it exists.
  MBA_string name(unsigned i) const { 
    assert(0 <= i && i < nmembers());
    return member_name_[i];
  }

  /// Return the member names as a read-only array.
  const MBA_string *names() const { return member_name_; }

#ifdef ENABLE_L2_VERBOSE
  /// Write the name of the ith element of the domain, if it exists.
  l2_virtual void printMember(_STD_ ostream& os, unsigned i) const {
    if (isValidMember(i)) { os << name(i); }
  }

  /// Write the name, cardinality and elements of the domain.
  l2_virtual void toOStream_long(_STD_ ostream& os) const {
    os  << "Enumeration "
	<< name()
	<< " has "
	<< nmembers()
	<< " members:\n";
    for (size_t i = 0; i < nmembers(); i++)
      os << "    "
	 << i
	 << ": "
	 << name(i)
	 << _STD_ endl;
  }

  /// Write the name of the domain.
  l2_virtual void toOStream(_STD_ ostream& os) const {
    os << name();
  }
#endif

  /// Is this the debugging version? Yes, it is.
  l2_virtual bool hasDebug() const { return true; }

 private:
  /// The name of the domain.
  MBA_string enum_name_;
  /// The names of the elements.
  MBA_string *member_name_;
};
#endif


#ifdef ENABLE_L2_VERBOSE
/// Write the name of the domain.
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rEnumeration& e) {
  e.toOStream(os);
  return os;
}

/// Write the domain pointed to, if the pointer isn't null.
inline _STD_ ostream& operator << (_STD_ ostream& os, const L2rEnumeration *e) {
  if (!e) { os << "(nil enum)"; }
  else    { e->toOStream(os); }
  return os;
}
#endif

#endif
