/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// The domain of a variable.
// $Id: domain.h,v 1.7 2002/06/20 21:24:09 lbrown Exp $

#ifndef DOMAIN_H
#define DOMAIN_H

#include <mba_utils/MBA_string.h>

class L2rEnumeration;
class L2rMode;

/**
 * Associate print names to the set of values a Variable may assume.
 */

class Domain : public Pooled {
 public:

  enum public_constants { NOT_FOUND = -1 };

  /**
   * Supply Domain name and generate element names s&lt;i&gt;,
   * 0 &lt;= i &lt; cardinality - 1.
   */
  Domain(const MBA_string& domainName,
	 unsigned cardinality);

  /// Domain name and element names come from the L2rEnumeration
  Domain(const L2rEnumeration*);

  /**
   * The Domain name of a mode comes from the Transitioned name; the names of
   * its elements are the Transition names.
   * The 0th element is named "nominal".
   */
  Domain(const L2rMode* pL2rMode);

  /// Supply Domain name and array of element names of length cardinality.
  Domain(const MBA_string& domainName,
	 const MBA_string *elementNames,
	 unsigned cardinality);

  /// Free the array of element names
  virtual ~Domain();

  /// Iterator over the element names
  typedef const MBA_string* iterator;

  /// Begin iterator for the element names
  iterator begin() const { return values; }

  /// End iterator for the element names
  iterator end()   const { return values + nvalues; }

  /// Return the name of the ith value
  const MBA_string& value(unsigned i) const { return values[i]; }

  /// Return the index of the named element or NOT_FOUND if not found
  int value(const MBA_string& elementName) const;
  
  /// Return an array of element names
  const MBA_string *get_values() const { return values; }

  /// Return the cardinality of the Domain
  unsigned size() const { return nvalues; }

  /// Return the print name of the Domain
  const MBA_string& get_name() const { return domain_name; }

 private:
  /// The name strings of the elements
  MBA_string *values;
  /// The cardinality of the domain
  unsigned nvalues;
  /// The print name of the Domain
  MBA_string domain_name;
};

#endif // DOMAIN_H
