/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Iterators for Slists.
// $Id: slist_iterator.h,v 1.6 2002/01/02 20:30:57 lbrown Exp $
//

#ifndef SLIST_ITERATOR_H
#define SLIST_ITERATOR_H

#include <assert.h>
#include <mba_utils/slist_item.h>

template <class Type>
    class Slist;

template <class Type>
class Slist_iterator {
public:

  Slist_iterator(Slist_item<Type>* item_p = 0)
    : current(item_p), previous(0) {}

  Slist_iterator& operator++() {
    if (current) {
      previous = current;
      current = current->next;
    }
    return *this;
  }

  Type& operator*() {
    assert(current);
    return current->item;
  }

  bool operator==(const Slist_iterator& iter) const {
    return current == iter.current;
  }

  bool operator!=(const Slist_iterator& iter) const {
    return current != iter.current;
  }

  Slist_iterator& operator=(const Slist_iterator& iter) {
    current = iter.current;
    previous = iter.previous;
    return *this;
  }

 friend class Slist<Type>;

private:

  Slist_item<Type>* current;
  Slist_item<Type>* previous;  // to help with erasure of elements.
};

#endif // SLIST_ITERATOR_H
