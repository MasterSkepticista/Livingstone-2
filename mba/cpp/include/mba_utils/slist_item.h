/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// The item structure used by singly linked lists to store objects.
// $Id: slist_item.h,v 1.20 2004/02/18 18:24:24 lbrown Exp $
//
// Slist_items are similar to cons cells in Lisp (the main difference is
// that, unlike Lisp cons cells, they only store items of a single type).
//
// Public interface:
//    There is no public interface.  Only the Slist and Slist_iterator
//    classes have access to the item class.
//
// Private interface:
//
//   Slist_item()
//     Default constructor that creates an Slist_item with no next item and
//     an empty item initialized with the default constructor.
//
//   Slist_item(const Type& it)
//     Constructs an Slist_item with it as the current item and no next
//     item.
//
//   void delete_list()
//     Fastest available method of deleting an entire list.

#ifndef SLIST_ITEM_H
#define SLIST_ITEM_H

#include <stdlib.h> // size_t
#include <livingstone/L2_iostream.h>
#include <mba_utils/pooled.h>

template <class Type>
class Slist;

template <class Type>
class Slist_iterator;


template <class Type>
class Slist_item : public Pooled {
    public:
        friend class Slist < Type >;
        friend class Slist_iterator < Type >;

        // this declaration causes a spurious warning with gcc if
        // Type is already const.  Turns out we don't need it, so
        // I removed the declaration  -bhudson
        //	friend class Slist_iterator<const Type>;

        friend _STD_ ostream& operator<<(_STD_ ostream&, Slist_item<Type>&);

        Slist_item() :
	  // The VxWorks 5.3.1 compiler does not understand constructor
	  // notation for built-in types (Stroustrup, "The C++ Programming
	  // Language," Section 6.2.8). Thus, when Type is built-in, it does
	  // not know what to do with int(), but it can handle int(0) as a
	  // cast. If all classes C placed into an Slist had constructor C(int)
	  // this would not be a problem. Most Slists are of pointers. Only
	  // class MBApair had to be given a bogus constructor with an unused
	  // int argument.
	  item(
#ifdef VXWORKS531
	       0
#endif
              ),
	  next(0) {}

        Slist_item(const Type& it) : item(it), next(0) {}

        Slist_item(const Type& it, Slist_item* item_p)
            : item(it), next(item_p) {}


        // delete an entire list.  Might we be able to do this faster?
        void delete_list() {
            Slist_item<Type> *cur = this;
            while(cur) {
                Slist_item<Type> *next = cur->next;
                delete cur;
                cur = next;
            }
        }

    private:
        Type item;
        Slist_item* next;
};



//----------------------------------------------------------------------
//
// Friend function definitions
//   _STD_ ostream<<
//
//---------------------------------------------------------------------------

template <class Type>
_STD_ ostream&
operator<<(_STD_ ostream& os, Slist_item<Type>& slist_item)
{
  os << slist_item->item;
}

#endif // SLIST_ITEM_H
