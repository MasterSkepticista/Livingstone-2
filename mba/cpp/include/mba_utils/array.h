/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: array.h,v 1.11 2004/02/05 22:36:14 lbrown Exp $ */

#ifndef array_HEADER
#define array_HEADER

#include <mba_utils/array_allocator.h>
#include <assert.h>

/**
 * A self-resizing array that uses the L2_alloc mechanism.
 *
 * The "usector" argument specifies whether T's constructor is to be used.
 *
 * It is safe to use it as Array<T>.
 *
 * In performance-critical applications, if the Array's members are for
 * instance pointers or integers, or another data type that needs no
 * initialization, consider setting 'usector' to false.
 */

template <class T, bool usector
#ifndef VXWORKS531
= true
#endif
>
class Array {
  public:
  /// Creates an empty Array.
  Array() : array_(NULL), used_(0), allocated_(0) { }
  
  /**
   * Creates an empty Array with storage for <i>initial_size</i> elements
   * already allocated.
   */
  Array(unsigned initial_size) : used_(0), allocated_(initial_size) {
    array_ = do_allocate(allocated_);
  }

  /// Destroy the Array, deallocating its elements.
  ~Array() {
    erase();
#if !defined(NDEBUG) && !defined(VXWORKS531)
    array_ = (T*)0x30303;
    used_  = allocated_ = 0x30303;
#endif
  }

  /// Empty the Array, deallocating its elements.
  void erase() {
    do_free(array_, allocated_);
    array_ = NULL;
    used_ = allocated_ = 0;
  }

  /// Return the number of elements.
  unsigned size() const { return used_; }
  
  /// See if there are any elements.
  bool empty() const { return size() == 0; }

  /// Return the ith element.
  const T& operator[] (unsigned i) const {
    assert(i < size());
    return array_[i];
  }

  /**
   * Return the ith element, or set it.
   * This form is given to allow statements of the form:
   * <code>array[i] = x</code>
   */
  T& operator[] (unsigned i) {
    assert(i < size());
    return array_[i];
  }

  /**
   * operator[] disallows setting things out of bounds; this form will grow
   * the Array if needed to make it legal.
   */
  void grow_and_set(unsigned i, T t) {
    if (i >= size()) {
      ensure(i + 1);
      used_ = i + 1;
    }
    array_[i] = t;
  }

  /// An iterator over the Array elements
  typedef T* iterator;
  /// Begin iterating over the Array elements
  iterator begin() const { return array_; }
  /// End iterating over the Array elements
  iterator end() const { return array_ + size(); }

  /// The same as push_back(const T&)
  void push(const T& elt) {
    ensure(size() + 1);
    array_[size()] = elt;
    used_++;
  }

  /**
   * Push an element onto the stack this Array represents.
   * This appends to the end of the Array.
   */
  void push_back(const T& elt) { push(elt); }

  /// The same as pop_back()
  T pop() {
    assert(size() != 0);
    --used_;
    return array_[size()];
  }

  /**
   * Pop an element from the stack this Array represents.
   * This removes the last element and returns it.
   */
  T pop_back() { return pop(); }

  /**
   * Remove the ith element in O(1) time.
   * This mangles the order of the Array, as the ith element is replaced by
   * the last element.
   */
  void remove(unsigned i) {
    assert(i < size()); // refuses used_==0 for any i, so --used_ is safe
    --used_;
    if (i != size()) {
      // Some classes have trouble with being assigned to themselves
      // (consider if operator=() first deallocates everything, then
      // copies ... what it just deallocated!)
      array_[i] = array_[size()];
    }
  }

  /**
   * Remove the element to which the iterator points.
   * This mangles the order of the Array, as the element is replaced by the
   * last element.
   */
  void erase(iterator it) {
    // assert the iterator belongs to us
    assert((it >= array_) &&
	   (it <  array_ + size()));
    // turn 'it', a pointer, into an offset
    remove(it - array_);
  }

  private:

  // The default bit-copy won't work for these (we'd deallocate twice;
  // and for operator= leak once).  If needed, we'll get back to these.

  /// Copy constructor
  Array(const Array<T, usector>&);

  /// Assignment operator
  void operator= (const Array<T, usector>&);

  /**
   * Ensure the Array has enough capacity to hold newsize elements.
   * We at least double the Array size (perhaps more if newsize is more than
   * double the old size), which ensures amortized constant-time appending to
   * the Array.
   */
  void ensure(unsigned newsize) {
    // First, check if we already have enough space
    if (newsize > allocated_) {
      // We don't; double the Array size or, if that's not enough, make it
      // just big enough.
      unsigned old_alloc = allocated_;
      allocated_ *= 2;
      if (newsize > allocated_) {
	allocated_ = newsize;
      }
      T *old = array_;
      array_ = do_allocate(allocated_);
      // not memcpy -- we need to call operator =
      for (unsigned i = 0; i < size(); ++i) {
	array_[i] = old[i];
      }
      do_free(old, old_alloc);
    }
  }

  /// Allocate and return n elements of type T 
  T *do_allocate(unsigned n) {
    return (usector) ?
    L2_alloc_array(T, n) :
    L2_alloc_array_no_ctor(T, n);
  }

  /// Deallocate n elements from the Array
  void do_free(T* array, unsigned n) {
    if (usector) {
      L2_free_array(array, n);
    } else {
      L2_free_array_no_dtor(array, n);
    }
  }

  private:

  /// The storage for the elements
  T *array_;
  /// The number of elements used
  unsigned used_;
  /// The number of elements which can be used without re-allocating
  unsigned allocated_;
};

#endif
