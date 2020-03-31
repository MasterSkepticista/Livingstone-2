/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// A hash table class.
// $Id: hash_table.h,v 1.31 2004/02/24 01:14:44 lbrown Exp $
//
// This is a simple implementation of hash tables that uses buckets to
// store keys that hash to the same value.  The basic implementation comes
// with a default hash function for keys of type String.  To use this
// implementation with keys of other types, the hash table needs to be
// initialized with the hash function of the following type:
//      unsigned int (*hash_function)(const Key&)
//
// The default number of buckets in the hash table is specified in the
// static variable
//      static unsigned int default_table_size
// This variable can be initialized as desired for specific hash table
// types.  It currently defaults to 211.
//
// Public types:
//
//   Hash_table<Key,Type>::size_type
//     Holds the size of the hash table.
//
//   Hash_table<Key,Type>::iterator
//     Holds an iterator for the hash table.  This is really a
//     const_iterator, since it cannot be used to modify the elements in
//     the hash table.
//
// Public interface:
//
//   Hash_table(unsigned int (*hash_fn)(const Key&) = hashpjw)
//     Construct an empty hash table using hash_fn as the hash function.
//     The hash function defaults to hashpjw, taken from the Compilers book
//     by Aho, Sethi, and Ullman (page 436), which works on String keys.
//     The default number of buckets in the hash table is specified in the
//     static variable default_table_size.
//
//   Hash_table(size_type size, unsigned int (*hash_fn)(const Key&) = hashpjw)
//     Construct an empty hash table with size buckets and hash_fn as the
//     hash function.  The hash function defaults to hashpjw as above.
//     Actually, the size of the hash table is adjusted to be the smallest
//     prime larger than size.
//
//   ~Hash_table()
//     Destructor.
//
//   bool empty() const
//     Returns true iff there are no elements in the hash table.
//
//   size_type size() const
//     Returns the number of elements currently in the hash table.
//
//   size_type erase(const Key& key)
//     If there is an element in the hash table with the specified key,
//     then it is erased.  Returns the number of erased elements (can only
//     be 0 or 1).
//
//   void erase(iterator)
//     Removes the element being pointed to from the hash table.  Returns
//     the number of erased elements (0 or 1; 0 only if iterator is
//     pointing at end()).
//
//     Invalidates iterators that point to that element, and possibly to
//     the next element.  However, the current iterator is not invalidated:
//     it points to the next element, if any.
//
//   iterator find(const Key& key)
//     Returns an iterator pointing at the element with the specified key.
//     If no such element exists, then it returns end().
//
//   MBApair<iterator, bool> insert(const Key& key, const Type& value)
//     Inserts <key,value> into the hash table.  Returns a pair whose first
//     element is an iterator pointing at the hash  table element whose key
//     is key.  The second element is true if this element is in fact
//     value, i.e., value was inserted successfully.  The second element is
//     false if value was not inserted because there already was an element
//     in the hash table with the specified key.
//
//   iterator insert_duplicate(const Key& key, const Type& value)
//     Inserts <key,value> into the hash table.  Faster than the above
//     because we don't check for duplicates.
//
//   iterator begin()
//     Returns an iterator pointing at the first element in the hash
//     table.  Returns end() if the hash table is empty.
//
//   iterator end();
//     Returns an iterator pointing past the last element in the hash
//     table.
//
// Iterator interface:
//
//   Hash_table::iterator operator++()
//     Increments the iterator to the next element.
//
//   const Type& operator*()
//     Returns the Type being pointed to.
//
//   const Key& key()
//     Returns the Key being pointed to.
//
//   bool operator==(const Hash_table::iterator& iter) const
//     Returns true iff this iterator is pointing at the same element as
//     iter.
//
//   Hash_table::iterator& operator=(const Hash_table::iterator& iter)
//     Assigns an iterator.

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <mba_utils/pair.h>
#include <mba_utils/prime.h>
#include <mba_utils/slist.h>

// The static variables are changed to defines under VxWorks
#ifdef VXWORKS
#define default_table_size 211
#endif // VXWORKS

#define HASH_TABLE_SIZE_TYPE long

/**
 * A hash function that works with anything that can be cast to
 * an unsigned integer (includes pointers).
 */
template <class Key>
unsigned int hash_integral_key(const Key& key) {
    return unsigned(key);
}

/**
 * A hash function for MBA_strings.
 */
unsigned int hashpjw(const MBA_string&);

template <class Key, class Type> class Hash_table_iterator;

template <class Key, class Type>
class Hash_table {

public:

  //---------------------------------------------------------------------
  // Typdefs for STL compatibility
  //---------------------------------------------------------------------

  typedef HASH_TABLE_SIZE_TYPE           size_type;
  typedef MBApair<const Key, const Type> value_type;
  typedef Hash_table_iterator<Key, Type> iterator;

public:

  friend class Hash_table_iterator<Key,Type>;

#ifndef VXWORKS531
  // The VxWorks 5.3.1 compiler doesn't like using hashpjw as the default hash
  // function when the key is not an MBA_string.
  Hash_table() : table_size(default_table_size), hash(hashpjw) {
    table = L2_alloc_array( Slist< value_type >, table_size );
  }
#endif

  Hash_table(unsigned int (*hash_fn)(const Key&)) :
    table_size(default_table_size), hash(hash_fn) {
#ifdef VXWORKS531
    // The VxWorks 5.3.1 compiler doesn't properly handle
    // typdef MBApair<const Key, const Type> value_type;
    // and the preprocessor loses count of the number of arguments when
    // Slist< value_type > is fully expanded to
    // Slist < MBApair<const Key, const Type> >
    // Instead of invoking the macro L2_alloc_array, expand that macro inline.
    const size_t eltSize = sizeof(Slist< MBApair<const Key, const Type> >);
    const size_t byteCount = eltSize * table_size;
    void* rawArray = L2_alloc(byteCount);
    Slist< MBApair<const Key, const Type> >* typedArray =
      (Slist< MBApair<const Key, const Type> >*)rawArray;
    table = L2_initialize_array(typedArray, table_size);
#else
    table = L2_alloc_array( Slist< value_type > , table_size );
#endif
  }

  ~Hash_table() {
      L2_free_array(table, table_size);
  }

  // Member functions

  bool empty() const {
    for (size_t i = 0; i < table_size; ++i)
      if (!(table[i].empty())) // then we have a non-empty bucket
	return false;
    return true;               // all buckets are empty
  }

  HASH_TABLE_SIZE_TYPE size() const {
    HASH_TABLE_SIZE_TYPE total = 0;
    for (size_t i = 0; i < table_size; ++i)
      total += table[i].size();
    return total;
  }

  HASH_TABLE_SIZE_TYPE erase(const Key&);

  HASH_TABLE_SIZE_TYPE erase(Hash_table_iterator<Key,Type>&);

  void erase() {
      for(size_t i=0; i<table_size; ++i)
          table[i].erase();
  }

  Hash_table_iterator<Key,Type> find(const Key&);

#ifdef VXWORKS531
  // VxWorks 5.3.1 isn't allowed to use the return value
  bool
#else
  MBApair<Hash_table_iterator<Key,Type>, bool>
#endif
  insert(const Key&, const Type&);
  Hash_table_iterator<Key,Type> insert_duplicate(const Key&, const Type&);

  // Iterator functions

  Hash_table_iterator<Key,Type> begin();

  Hash_table_iterator<Key,Type> end();

private:

  // operator= is declared to be private and has no definition.  Hence, we
  // cannot currently assign to our hash tables.
  Hash_table& operator=(const Hash_table& ht);

  // operator== is declared to be private and has no definition.  Hence, we
  // cannot currently check for equality of two hash tables.
  bool operator==(const Hash_table& ht) const;

  // The array that stores the hash table buckets.  Each bucket is a list
  // of pairs, where the pair holds the key string and the value.
  Slist< MBApair<const Key, const Type> >* table;

  // The size of the array that stores the hash table buckets.
  unsigned int table_size;

// VxWorks' compilers do not like static variables inside of class templates.
#ifndef VXWORKS
  // The default size of the hash table.
  static unsigned int default_table_size;
#endif // VXWORKS

  // The function to be invoked for hashing a key.
  unsigned int (*hash)(const Key&);
};

template <class Key, class Type>
class Hash_table_iterator {

public:

  friend class Hash_table<Key,Type>;

  typedef Hash_table<Key, Type> ht_type;

  typedef Slist_iterator< MBApair<const Key, const Type> > slist_iterator;

  Hash_table_iterator& operator++();

  const Type& operator*() { return (*bucket_iter).second; }

  const Key& key() { return (*bucket_iter).first; }

  bool operator==(const Hash_table_iterator& iter) const {
    return
      ht_p == iter.ht_p &&
      index == iter.index &&
      bucket_iter == iter.bucket_iter;
  }
  bool operator!=(const Hash_table_iterator& iter) const {
      return ! operator== (iter);
  }

  Hash_table_iterator& operator=(const Hash_table_iterator& iter) {
    ht_p = iter.ht_p;
    index = iter.index;
    bucket_iter = iter.bucket_iter;
    return *this;
  }

private:

  // A private constructor, to be used only by Hash_table<Key,Type>

  Hash_table_iterator(ht_type & ht, unsigned int current=0)
      : ht_p(&ht), index(current)
      {
          bucket_iter = ht.table[current].begin();
      }

  Hash_table_iterator(ht_type & ht, unsigned int current,
          slist_iterator iter)
      : ht_p(&ht), index(current), bucket_iter(iter)
      {
      }


  // erase(): deletes the element being pointed to.
  void erase() {
    ht_p->table[index].erase(bucket_iter);
    if (bucket_iter == ht_p->table[index].end())
      operator++();
  }

  ht_type*       ht_p;       // the hash table over which it iterates
  unsigned int   index;            // the current bucket
  slist_iterator bucket_iter;      // for iterating through a bucket

};

//----------------------------------------------------------------------
//
// Hash table method definitions
//
//----------------------------------------------------------------------

/*
template <class Key, class Type>
Hash_table<Key,Type>::Hash_table(HASH_TABLE_SIZE_TYPE size,
				 unsigned int (*hash_fn)(const Key&))
  : table_size(size), hash(hash_fn)
{
  if (table_size % 2 == 0)
    ++table_size;
  while (!pseudoprime(table_size))
    table_size += 2;
  table = new Slist< MBApair<const Key, const Type> >[table_size];
}
*/

template <class Key, class Type>
Hash_table_iterator<Key,Type>
Hash_table<Key,Type>::begin()
{
  for (unsigned int i = 0; i < table_size; ++i)
    if (!table[i].empty())
      return Hash_table_iterator<Key,Type>(*this, i, table[i].begin());
  // else the table is empty
  return Hash_table_iterator<Key,Type>
    (*this, table_size - 1, table[table_size - 1].end());
}

template <class Key, class Type>
inline Hash_table_iterator<Key,Type>
Hash_table<Key,Type>::end()
{
  return Hash_table_iterator<Key,Type>(*this,
				       table_size - 1,
				       table[table_size - 1].end());
}

#ifndef VXWORKS
template <class Key, class Type>
unsigned int
Hash_table<Key, Type>::default_table_size = 211;
#endif // VXWORKS

template <class Key, class Type>
HASH_TABLE_SIZE_TYPE
Hash_table<Key,Type>::erase(const Key& key)
{
  Hash_table_iterator<Key,Type> it = find(key);

  if (it != end()) {
    it.erase();
    return 1;
  } else
    return 0;
}

template <class Key, class Type>
HASH_TABLE_SIZE_TYPE
Hash_table<Key,Type>::erase(Hash_table_iterator<Key,Type>& iter)
{
  if (iter != end()) {
    iter.erase();
    return 1;
  } else
    return 0;
}

template <class Key, class Type>
Hash_table_iterator<Key,Type>
Hash_table<Key,Type>::find(const Key& key)
{
  unsigned int               index = hash(key) % table_size;
  Slist_iterator< MBApair<const Key, const Type> > slist_iter =
    table[index].begin();
  Slist_iterator< MBApair<const Key, const Type> > slist_end  =
    table[index].end();

  for (; slist_iter != slist_end; ++slist_iter) {
    if ((*slist_iter).first == key) { // found it
      return Hash_table_iterator<Key,Type>(*this, index, slist_iter);
    }
  }
  // else return an iterator pointing at the very end of the hash table
  return Hash_table_iterator<Key,Type>(*this,
				       table_size - 1,
				       table[table_size - 1].end());
}

template <class Key, class Type>
#ifdef VXWORKS531
bool
#else
MBApair< Hash_table_iterator<Key,Type>, bool >
#endif
Hash_table<Key,Type>::insert(const Key& key, const Type& value)
{
  unsigned int index = hash(key) % table_size;
  Slist_iterator< MBApair<const Key, const Type> > slist_iter =
    table[index].begin();

  for (; slist_iter != table[index].end(); ++slist_iter) {
    if ((*slist_iter).first == key) {  // it already exists
      return
#ifdef VXWORKS531
	0;
#else
	MBApair<Hash_table_iterator<Key,Type>,bool>
	(Hash_table_iterator<Key,Type>(*this, index, slist_iter),
	 0); // should be "false", but Sun C++ fails to link properly.
#endif
    }
  }
  // means it does not exist
  table[index].push_front(MBApair<const Key, const Type>(key, value));
  return
#ifdef VXWORKS531
    1;
#else
    MBApair<Hash_table_iterator<Key,Type>,bool>
    (Hash_table_iterator<Key,Type>(*this, index, table[index].begin()),
     1); // should be "true", but Sun C++ fails to link it properly.
#endif
}

template <class Key, class Type>
Hash_table_iterator<Key,Type>
Hash_table<Key,Type>::insert_duplicate(const Key& key, const Type& value)
{
  unsigned int index = hash(key) % table_size;

  table[index].push_front(MBApair<const Key, const Type>(key, value));
  return Hash_table_iterator<Key,Type>(*this, index, table[index].begin());
}


///



//----------------------------------------------------------------------
//
// Hash table iterator method definitions
//
//----------------------------------------------------------------------

template <class Key, class Type>
Hash_table_iterator<Key, Type>&
Hash_table_iterator<Key, Type>::operator++()
{
  if (++bucket_iter == ht_p->table[index].end()) {
    // then increment the index until the end
    for (++index; index < ht_p->table_size; ++index) {
      if (!(ht_p->table[index].empty())) {
	// found the next non-empty bucket, so point to its first element
	bucket_iter = ht_p->table[index].begin();
	return *this;
      }
    }
    // Didn't find a non-empty bucket, so point to the end of the last list
    bucket_iter = ht_p->table[--index].end();
    return *this;
  } else            // it's still in the same bucket
    return *this;
}

#ifdef VXWORKS
#undef default_table_size
#endif // VXWORKS

#endif // HASH_TABLE_H
