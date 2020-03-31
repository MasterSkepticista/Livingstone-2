/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Singly linked lists.
// $Id: slist.h,v 1.27 2004/02/24 01:14:45 lbrown Exp $

#ifndef SLIST_H
#define SLIST_H

#include <livingstone/L2_assert.h>
#include <livingstone/L2_iostream.h>
#include <mba_utils/slist_iterator.h>

#define SLIST_SIZE_TYPE unsigned
#ifdef CODETEST
#define TEMPLATE_OPERATOR <>
#else
#define TEMPLATE_OPERATOR
#endif

template <class Type>
class Slist {
/**
 Slists provide a generic singly linked list capability.  The interface
 to Slists are compatible with container interfaces in STL.  Slists use
 Slist_items to store objects in the list.  Slist_items provide
 convenient memory management, so that client programs need not worry
 about such issues.  Slist_items are allocated from a chunk of separately
 allocated raw memory, and recycled whenever they are deleted.
 Additional raw memory can be allocated if all the originally allocated
 memory is used up.  Client programs can control the size of the
 originally allocated raw memory and the incremental memory by modifying
 two static variables in Slist_item:

   Slist_item<Type>::alloc_init_size
     controls the number of Slist_items that are initially allocated.

   Slist_item<Type>::alloc_incr_size
     controls the number of additional Slist_items that are allocated.

 Slists provide no interface to allow sharing of Slist_item structures.
 This enables a particularly simple memory management scheme: an Slist's
 items are destroyed precisely when an Slist is destroyed.  Hence, if a
 client program only allocates Slist objects on the stack, rather than on
 the heap, it need not worry about memory management problems.  This is
 enforced in the interface by making Slist's operator new private and
 undefined (see EC++:27).
*/



public:

  //---------------------------------------------------------------------
  // Typdefs for STL compatibility
  //---------------------------------------------------------------------

  ///      Holds the size of the list.
  typedef SLIST_SIZE_TYPE      size_type;

  ///      Holds a pointer to an object in the list (Type*)
  typedef Type*                pointer;
  
  ///      Holds a pointer to a constant object in the list (const Type*)
  typedef const Type*          const_pointer;

  ///      Holds a reference to an object in the list (Type&)
  typedef Type&                reference;
 
  ///      Holds a reference to a constant object in the list (const Type&)
  typedef const Type&          const_reference;

  ///      Holds an iterator for a modifiable list.
  typedef Slist_iterator<Type> iterator;

  ///      Holds an iterator for a constant list.
  typedef Slist_iterator<const Type> const_iterator;

public:

  //---------------------------------------------------------------------
  // Constructors and destructor
  //---------------------------------------------------------------------

  ///      Creates an empty list.
  Slist() : head(0), length(0) { };

  ///      Constructs a list with just value in it.  This is not a standard
  ///      STL list constructor.
  Slist(const Type& obj) : head(new Slist_item<Type>(obj)), length(1) { };

  ///      Constructs a copy of slist.  Note that the copy of slist shares no
  ///      Slist_items with slist.
  Slist(const Slist& slist);

  ///      Destroys the list, and deletes all the Slist_items in the list.
  ~Slist() { erase(); }

  //---------------------------------------------------------------------
  // Member functions
  //---------------------------------------------------------------------

  ///      Returns true if the list is empty.
  bool empty() const { return !head; }

  ///      Returns the number of elements in the list.
  SLIST_SIZE_TYPE size() const;

  ///      Does an itemwise copy of the list.  As with the copy constructor,
  ///      there is no support for sharing Slist_items.
  Slist& operator=(const Slist& slist);

  /// Swaps the elements in slist with the elements in this list.
  void swap(Slist& slist);

  /// Checks if this list has the same elements in the same order as
  /// slist.  Uses Type's operator==.
  bool operator==(const Slist& list) const;

 ///      Pushes obj onto the front of the list.
  void push_front(const Type& value) {
    head = new Slist_item<Type>(value, head);
    length++;
  }

  ///  Removes the first element in the list.  Note that it doesn't return
  /// the first element.
  void pop_front() {
    if (head) {
      Slist_item<Type>* item_p = head;
      head = head->next;
      delete item_p;
      length--;
    }
  }


  ///  Pushes obj onto the back of the list.
  void push_back(const Type& value);

  ///      Removes the last element in the list.  Note that it doesn't return
  ///      the last element.
  void pop_back();

  /// Inserts a new element after the iterator pos.
  /// If pos is end(), same as push_back.
  /// To get an iterator to the new element, do ++pos (or
  /// last() if pos==end()).
  /// Not stricly STL: STL returns the new iterator.
  void insert(Slist_iterator<Type>& pos, const Type& value);

  void insert(Slist_iterator<Type>& pos);


  // Insert an element before pos in constant-time, and
  // return an iterator to the new node.
  // pos remains valid; ++ on the return value is == pos.
  // Since we run in constant-time, pos must not be == end() unless
  // the list is empty.
  Slist_iterator<Type> insert_before(Slist_iterator<Type>& pos,
				     const Type& value) {
      if(pos==begin()) {
	  push_front(value);
          return begin();
      } else {
          assert(pos != end());

          // neither is null
          Slist_item<Type>* current = pos.current;
          Slist_item<Type>* previous= pos.previous;
          assert(current && previous);

          // insert after previous and before current
          Slist_item<Type>* new_item = new Slist_item<Type>(value,current);
          previous->next = new_item;

          // keep pos iterator valid
          pos.previous = new_item;

          // create new iterator
          Slist_iterator<Type> new_iterator(new_item);
          new_iterator.previous = previous;
	  length++;
          return new_iterator;
      }
  }

  ///      Deletes the list element to which pos points.  pos is incremented
  ///      to the next element.  This method invalidates any iterators
  ///      pointing either at the deleted element or at the next element.  But
  ///      all other iterators continue to remain valid.  This is not standard STL,
  ///	   in that in STL all iterators become invalid after invoking this method.
  void erase(Slist_iterator<Type>& pos);

  ///      Deletes all elements in the list.  This is a non-standard, and not
  ///      part of the STL.
  void erase() {
      if(head) {
          head->delete_list(); head = 0;
	  length = 0;
      }
  }

  ///      Returns a reference to the first element in the list.  It is an
  ///      error to call this method on an empty list.
  Type& front() {
    assert(head);
    return head->item;
  }

  ///      Returns a reference to the first element of a constant list.  It is
  ///      an error to call this method on an empty list.
  const Type& front() const {
    assert(head);
    return head->item;
  }

  ///      Returns a reference to the last element in the list.  It is an
  ///      error to call this method on an empty list.
  Type& back();

  ///      Returns a reference to the last element of a constant list.  It is
  ///      an error to call this method on an empty list.
  const Type& back() const;


  ///      Removes all elements in the list that are == to value.
  void remove(const Type& value);

  ///      Removes the first element in the list that is == to value.  This is
  ///      not a standard STL method.
  void remove_first(const Type& value);

  ///      Returns true if value is contained by the list. This is
  ///      not a standard STL method.
  bool contains(const Type& value) const;

  ///      Removes duplicates from the list.  Returns the number of elements
  ///      that are removed.  Duplicates are checked using operator== on the
  ///      list elements.  This is a non-standard function, i.e., it is not
  ///      available in STL.  STL provides the unique() method, which isn't
  ///      quite the same.
  SLIST_SIZE_TYPE remove_duplicates();

  ///      Reverse the elements in the list.
  void reverse();

  ///      Adds on the elements in slist to the end of this list.
  Slist& operator+=(const Slist<Type>& slist);

  /// Removes from the list any items that have the same value as
  /// an item in slist.
  Slist& operator-=(const Slist<Type>& slist);

  //  Iterator functions:
  // In the following four methods, we use the return value optimization
  // trick to return an iterator (MEC++:20).  We also provide two versions
  // each of the begin() and end() methods, one for non-const objects and
  // one for const objects (EC++:21)

  ///      Returns an iterator to iterate through the list.  The iterator can
  ///      be incremented by a prefix ++, and the element at a position can be
  ///      accessed using operator*.  Equality with iterator returned by end()
  ///      signals the end of the list.
  Slist_iterator<Type> begin() {
    return Slist_iterator<Type>(head);
  }

  ///      Returns an iterator to iterate through a constant list.
  Slist_iterator<const Type> begin() const {
      // head has type Slist_item<Type>* ;
      // force it into Slist_item<const Type>*
      // Note: static_cast doesn't work since they aren't related.
      return Slist_iterator<const Type>(
              (Slist_item<const Type>*)(head));
  }

  ///      Returns an iterator that points immediately after the end of the
  ///      list.  Hence, when an iterator starting at begin() is incremented
  ///      and becomes equal to end(), then it has reached the end of the
  ///      list.
  Slist_iterator<Type> end() {
    return Slist_iterator<Type>(0);
  }


  ///      Returns an iterator that points immediately after the end of a
  ///      constant list.
  Slist_iterator<const Type> end() const {
    return Slist_iterator<const Type>(0);
  }

  ///      Used to print Slists.  Assumes that Type objects have operator<<
  ///      defined.
  friend _STD_ ostream& operator<< TEMPLATE_OPERATOR (_STD_ ostream&,
						      const Slist<Type>&);

  /// Returns a new list that concatenates the elements in slist1 and slist2.
  friend inline Slist<Type>
    operator+ TEMPLATE_OPERATOR (const Slist<Type>& slist1,
				 const Slist<Type>& slist2);

private:

  // A private constructor that constructs a list by copying and appending
  // two other lists.  The only reason to have this constructor is to
  // facilitate the return value optimization in the defintion of operator+
  // (MEC++:20).
  Slist(const Slist& slist1, const Slist& slist2);

  // last(): returns a pointer to the last Slist_item in the list, and 0 if
  // the list is empty.
  Slist_item<Type>* last() const;

  // operator new is declared private, and is undefined.  This ensures that
  // Slists cannot be allocated dynamically (see EC++:27), thus plugging a
  // potential memory leak.
  void* operator new(size_t size);

private:

  // The head of the list is stored here.
  Slist_item<Type>* head;

  // Leave this public during development
 public:
  // The number of items in the list
  SLIST_SIZE_TYPE length;
};

//----------------------------------------------------------------------
//
// Definitions for Slist methods
//
//----------------------------------------------------------------------

template <class Type>
Slist<Type>::Slist(const Slist<Type>& slist)
{
  Slist_item<Type>* item_p = 0;
  Slist_item<Type>* copy_p = 0;

  head = 0;
  for (item_p = slist.head; item_p != 0; item_p = item_p->next) {
    if (copy_p) {
      copy_p->next = new Slist_item<Type>(item_p->item);
      copy_p = copy_p->next;
    } else {
      head = new Slist_item<Type>(item_p->item);
      copy_p = head;
    }
  }
  length = slist.length;
}

template <class Type>
Slist<Type>::Slist(const Slist<Type>& slist1, const Slist<Type>& slist2) : length(0)
{
  Slist_item<Type>* item_p = 0;
  Slist_item<Type>* copy_p = 0;

  head = 0;
  for (item_p = slist1.head; item_p != 0; item_p = item_p->next) {
    if (copy_p) {
      copy_p->next = new Slist_item<Type>(item_p->item);
      copy_p = copy_p->next;
    } else {
      head = new Slist_item<Type>(item_p->item);
      copy_p = head;
    }
  }
  length += slist1.length;
  for (item_p = slist2.head; item_p != 0; item_p = item_p->next) {
    if (copy_p) {
      copy_p->next = new Slist_item<Type>(item_p->item);
      copy_p = copy_p->next;
    } else {
      head = new Slist_item<Type>(item_p->item);
      copy_p = head;
    }
  }
  length += slist2.length;
}

template <class Type>
#ifdef VXWORKS531
TYPENAME SLIST_SIZE_TYPE
#else
TYPENAME Slist<Type>::size_type
#endif
Slist<Type>::size() const
{
  return length;
}

template <class Type>
Slist<Type>&
Slist<Type>::operator=(const Slist<Type>& slist)
{
  if (this == &slist)
    return *this;

  Slist_item<Type>* item_p = 0;
  Slist_item<Type>* aux_p = 0;

  // First delete the old items stored in this list...
  for (item_p = head; item_p != 0; item_p = aux_p) {
    aux_p = item_p->next;
    delete item_p;
  }

  // ...and then copy over the new items
  aux_p = 0;
  head = 0;
  for (item_p = slist.head; item_p != 0; item_p = item_p->next) {
    if (aux_p) {
      aux_p->next = new Slist_item<Type>(item_p->item);
      aux_p = aux_p->next;
    } else {
      head = new Slist_item<Type>(item_p->item);
      aux_p = head;
    }
  }
  length = slist.length;
  return *this;
}

template <class Type>
void
Slist<Type>::swap(Slist<Type>& list)
{
  Slist_item<Type>* temp = head;

  head = list.head;
  list.head = temp;

  unsigned tempLength = length;
  length = list.length;
  list.length = tempLength;
}

template <class Type>
bool
Slist<Type>::operator==(const Slist<Type>& list) const
{
  if (this == &list)
    return true;

  Slist_item<Type>* item_p = list.head;
  Slist_item<Type>* curr_p = head;

  for (;
       item_p != 0 && curr_p != 0;
       item_p = item_p->next, curr_p = curr_p->next) {
    if (item_p->item != curr_p->item)
      return false;
  }
  if (item_p == 0 && curr_p == 0)
    return true;
  else
    return false;
}

template <class Type>
void
Slist<Type>::push_back(const Type& value)
{
  Slist_item<Type>* end = last();

  if (end)
    end->next = new Slist_item<Type>(value);
  else
    head = new Slist_item<Type>(value);
  length++;
}

template <class Type>
void
Slist<Type>::pop_back()
{
  if (head) {
    Slist_item<Type>* current = head;

    if (current->next) {
      for (; (current->next)->next != 0; current = current->next)
	;
      delete current->next;
      current->next = 0;
    } else {
      delete head;
      head = 0;
    }
    length--;
  }
}

template <class Type>
void
Slist<Type>::erase(Slist_iterator<Type>& pos)
{
  Slist_item<Type>* current = pos.current;
  Slist_item<Type>* previous = pos.previous;

  if (current) {
    Slist_item<Type>* next = current->next;
    delete current;
    // The following makes this a non-standard implementation.  The
    // standard STL implementation would make pos illegal.  This
    // implementation increments pos so that it points to the next item.
    pos.current = next;
    if (previous)
      previous->next = next;
    else
      head = next;
    length--;
  }
}

template <class Type>
void
Slist<Type>::insert(Slist_iterator<Type> &pos)
{
  Type &value = Type();
  insert(pos, value);
}

template <class Type>
void
Slist<Type>::insert(Slist_iterator<Type> &pos,const Type &value )
{
    if(pos==end()) {
      push_back(value);
    } else {
        Slist_item<Type>* current = pos.current;
        L2_assert(current,L2_argument_error, ("invalid iterator in insert"));

        // insert after current
        Slist_item<Type>* new_item = new Slist_item<Type>(value,current->next);
        current->next=new_item;
	length++;
    }
}

template <class Type>
Type&
Slist<Type>::back()
{
  Slist_item<Type>* end = last();

  assert(end);
  return end->item;
}

template <class Type>
const Type&
Slist<Type>::back() const
{
  Slist_item<Type>* end = last();

  assert(end);
  return end->item;
}

template <class Type>
void
Slist<Type>::remove(const Type& value)
{
  Slist_item<Type>* current = head;
  Slist_item<Type>* previous = 0;

  while (current) {
    if (current->item == value) { // remove this value
      if (previous) {
		  previous->next = current->next;
		  delete current;
		  current = previous->next;
      } else { // it's at the head, so move the head to the next element
		  head = current->next;
		  delete current;
		  current = head;
      }
      length--;
    } else { // merely move to the next item
		previous = current;
		current = current->next;
    }
  }
}

template <class Type>
void
Slist<Type>::remove_first(const Type& value)
{
  Slist_item<Type>* current = head;
  Slist_item<Type>* previous = 0;

  while (current) {
    if (current->item == value) { // remove this value
      if (previous) {
	previous->next = current->next;
	delete current;
	current = previous->next;
      } else { // it's at the head, so move the head to the next element
	head = current->next;
	delete current;
	current = head;
      }
      length--;
      return;   // removed the first, so simply return immediately
    } else { // merely move to the next item
      previous = current;
      current = current->next;
    }
  }
}


template <class Type>
bool
Slist<Type>::contains(const Type& value) const
{
   Slist_item<Type>* current = head;

  while (current) {
    if (current->item == value)
		return true;
    else	 // merely move to the next item
		current = current->next;
  }
  return false;
}




template <class Type>
#ifdef VXWORKS531
TYPENAME SLIST_SIZE_TYPE
#else
TYPENAME Slist<Type>::size_type
#endif
Slist<Type>::remove_duplicates()
{
  Slist_iterator<Type> it1 = begin();
  Slist_iterator<Type> it_end= end();
  SLIST_SIZE_TYPE num_deleted = 0;

  for (; it1 != it_end; ++it1) {
    Slist_iterator<Type> it2 = it1;
    for (++it2; it2 != it_end; ) {
      if (*it1 == *it2) {
	erase(it2);     // remove this duplicate and move it2 forward
	++num_deleted;
      } else
	++it2;
    }
  }
  return num_deleted;
}

template <class Type>
void
Slist<Type>::reverse()
{
  if (head) {
    Slist_item<Type>* current = 0;
    Slist_item<Type>* next = head;

    while (next) {
      head = next;
      next = head->next;
      head->next = current;
      current = head;
    }
  }
  return;
}

template <class Type>
Slist<Type>&
Slist<Type>::operator+=(const Slist<Type>& slist)
{
  // In case this == &slist (reflexive)
  Slist slistCopy(slist);
  Slist_item<Type>* item_p = slistCopy.head;
  Slist_item<Type>* copy_p = last();

  for (; item_p != 0; item_p = item_p->next) {
    if (copy_p) {
      copy_p->next = new Slist_item<Type>(item_p->item);
      copy_p = copy_p->next;
    } else {
      head = new Slist_item<Type>(item_p->item);
      copy_p = head;
    }
    length++;
  }
  return *this;
}

template <class Type>
Slist<Type>&
Slist<Type>::operator-=(const Slist<Type>& slist)
{
  if (this == &slist) { // reflexive
    erase();
  } else {
    Slist_item<Type>* current = slist.head;
    while (current) {
      this->remove(current->item);
      current = current->next;
    }
  }
  return *this;
}

template <class Type>
_STD_ ostream&
operator<<(_STD_ ostream& os, const Slist<Type>& slist)
{
  Slist_iterator<const Type> iter = slist.begin();
  bool not_begin = false;

  os << "(";
  for (; iter != slist.end(); ++iter) {
    if (not_begin)
      os << " ";
    else
      not_begin = true;
    os << *iter;
  }
  os << ")";
  return os;
}

template <class Type>
inline Slist<Type>
operator+(const Slist<Type>& slist1, const Slist<Type>& slist2)
{
  return Slist<Type>(slist1, slist2);
}

template <class Type>
inline Slist_item<Type>*
Slist<Type>::last() const
{
  Slist_item<Type>* last = head;

  if (head)
    for (; last->next != 0; last = last->next)
      ;
  return last;
}



#endif // SLIST_H

