/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef MBA_UTILS_ALGOS_H
#define MBA_UTILS_ALGOS_H
//
// Generic algorithms
// $Id: algos.h,v 1.8 2004/02/05 22:36:13 lbrown Exp $
//

#include <livingstone/l2conf.h>



/**
 * Looks for value in a collection starting at start and ending before
 * end.  If value is found, it returns the iterator at that point.
 * operator== is used to compare equality.  It is assumed that the
 * iterator supports operator++.
 */
template <class Iterator, class Type>
Iterator find(Iterator start, Iterator end, const Type& value)
{
  for (; start != end; ++start)
    if (*start == value)
      return start;
  return end;
}

/**
 * Insert 'item' into the 'list' sorted from smallest to largest.
 * Runs in O(n) time, O(1) space.
 */
template <class T, class Targ>
void insert_sorted(Slist<T>& list, const T& item, 
        bool (*is_smaller)(Targ, Targ))
{
    for(TYPENAME Slist<T>::iterator it = list.begin(); it!=list.end(); ++it) {
        if(is_smaller(item, *it)) {
            list.insert_before(it, item);
            return;
        }
    }
    // If here, the item is larger than anything.  Unfortunately, at present,
    // this requires looping to the end.  That could be avoided fairly cheaply.
    list.push_back(item);
}

/**
 * Sort an array.  
 * is_smaller must be a full ordering over the inputs; it is an error
 * to have a<b, b<c, and c<a ; the results of that are undefined.
 * Currently, a very slow O(n^2) owing to the ridiculous implementation.
 * If we need to, we can make this much faster.
 */
template <class T, class Targ>
void sort(Array<T, true>& array, bool (*is_smaller)(Targ, Targ)) 
{
    Slist<T> list;
    unsigned i;
    for(i=0; i<array.size(); ++i)
        insert_sorted(list, array[i], is_smaller);
    TYPENAME Slist<T>::iterator it = list.begin();
    for(i=0 ; i<array.size(); ++i, ++it)
        array[i] = *it;
}

#endif
