/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Generic facility for heterogenous pairs of values that is part of STL
// $Id: pair.h,v 1.14 2004/02/18 18:24:23 lbrown Exp $

#ifndef PAIR_H
#define PAIR_H

#include <livingstone/L2_iostream.h>
#include <livingstone/l2conf.h>

/**
 * This is the STL facility for easily constructing heterogenous pairs of
 * values. The pair class is parameterized by two types, T1 and T2,
 * corresponding to the types of the first and second values.
 * Since this has the same name as the STL template, you can not use the STL
 * in any file that includes this header file.
 */

template <class T1, class T2>
struct MBApair {
  /**
   * Publicly accessible data member representing the first element of the
   * pair.
   */
  T1 first;
  /**
   * Publicly accessible data member representing the second element of the
   * pair.
   */
  T2 second;
  /**
   * Constructs a pair in which both elements are initialized by their
   * respective default constructors.
   */
  MBApair() {}

#ifdef VXWORKS531
  // See Slist_item::Slist_item() for explanation
  MBApair(int) { }
#endif

  /**
   * Explicit constructor for constructing a pair.  Note that make_MBApair() is
   * the easier way to construct this pair since it infers the types of T1 and
   * T2, while this constructor requires that the types be specified.
   */
  MBApair(const T1& x, const T2& y) : first(x), second(y) {}

};

/**
 * Checks the equality of two pairs, which is that corresponding pair elements
 * are equal.
 */
template <class T1, class T2>
inline bool
operator==(const MBApair<T1, T2>& x, const MBApair<T1, T2>& y) {
  return x.first == y.first && x.second == y.second;
}

/**
 * Checks if the first pair is less than the second pair.  Comparison is done
 * lexicographically, with the first element taking precedence over the second
 * element.
 */
template <class T1, class T2>
inline bool operator<(const MBApair<T1, T2>& x, const MBApair<T1, T2>& y) {
  return x.first <y.first || (!(y.first < x.first) && x.second < y.second);
}

/**
 * An easy way to construct a pair: a factory method, returning a copy of the
 * pair.
 */
template <class T1, class T2>
inline MBApair<T1, T2>
make_MBApair(const T1&x, const T2& y) { return MBApair<T1, T2>(x, y); }

/// Write a pair in Lisp dotted-pair notation.
template <class T1, class T2>
_STD_ ostream&
operator<<(_STD_ ostream& os, const MBApair<T1,T2>& p) {
  os << "(" << p.first << " . " << p.second << ")";
}

#endif // PAIR_H
