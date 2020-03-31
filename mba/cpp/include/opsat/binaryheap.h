/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: binaryheap.h,v 1.27 2002/06/28 16:48:36 lbrown Exp $ */

#ifndef _BINARYHEAP_H_
#define _BINARYHEAP_H_

#include <mba_utils/array.h>

/**
 * A simple binary heap.
 * There must be bool Type::operator>(const Type&) and
 * bool Type::operator<(const Type&)
 */

template <class Type>
class BinaryHeap {

public:
  /**
   * If inorder = 0, elements are in decreasing order; if inorder = 1, elements
   * are in increasing order.
   */
  BinaryHeap(unsigned int inorder = 0) : order(inorder) {
    compare[0] = &Type::operator>;
    compare[1] = &Type::operator<;
  }

  /**
   * Insert the element into the heap
   * \param pin pointer to the element to be inserted
   */
  void insert(Type* pin) {
    unsigned i = arr.size();
    arr.push_back(pin);
    while ( (i > 0) && (arr[parent(i)]->*compare[1 - order])(*pin) ) {
      arr[i] = arr[parent(i)];
      i = parent(i);
    }
    arr[i] = pin;
  }

  /// Returns first element and removes it from the heap
  Type* pop() {
    L2_assert(size() > 0, L2_bounds_error, ("popping with no elements left"));
    Type *max = top();
    arr.remove(0);
    if (size() > 0) { heapify(0); }
    return max;
  }

  /// Return the first element without removing it from the heap
  Type* top() const { return arr[0]; }
  
  /// Is the heap empty?
  bool empty() const { return arr.empty(); }

  /// Return the number of elements in the heap
  unsigned size() const { return arr.size(); }

  /// Return the index of the final element in the heap
  unsigned last_index() const { return size() - 1; }
  
  /// Is the order increasing?
  bool get_order() const { return order != 0; }

protected:
 /// Return the index of the parent of i
 int parent(int i) { return (i >> 1); }

 /// Return the index of the left child of i
 int left(int i) { return (i << 1); }

 /// Return the index of the right child of i
 int right(int i) { return ( (i << 1) + 1); }
 
 /**
  * Restore the heap property, assuming that it is intact below node i.
  * In practice, this is called only by BinaryHeap::pop() with argument 0
  * after removing the first element of the heap's array.
  * \param index index of the topmost node that might not be in heap order
  */

 void heapify(unsigned index)  {

   unsigned i = index;

   while (1) {
     // invariant: everything with index < i is heap ordered
     unsigned l = left(i);  // left child of i
     unsigned r = right(i); // right child of i
     unsigned largest = i;

     if ( (l <= last_index()) && // left child of i is in the heap
	  ((arr[l]->*compare[order])(*(arr[i]))) ) { // out of order
       largest = l;
     }

     if ( (r <= last_index()) && // right child of i is in the heap
	  ((arr[r]->*compare[order])(*(arr[largest]))) ) { // out of order
       largest = r;
     }
   
     if (largest != i) {                 // something out of order
       exchange(i, largest);             // swap
       i = largest; // heapify(largest); // repeat for swapped child
     } else {
       break;
     }
   }
 }

 /// Exchange (swap) the heap positions of the two items
 void exchange(int i1, int i2) {
   Type* tmp1 = arr[i1], *tmp2 = arr[i2];
   arr[i1] = tmp2;
   arr[i2] = tmp1;
 }

private:
 /// The heap is stored in this Array
 Array<Type*, false> arr;

 /**
  * This Array of pointers to binary operators allows the user to determine the
  * heap ordering (decreasing or increasing) by setting the order parameter.
  */
 bool (Type::*compare[2])(const Type &) const;

 /// 0 if decreasing; 1 if increasing
 unsigned int order;
};

#endif
