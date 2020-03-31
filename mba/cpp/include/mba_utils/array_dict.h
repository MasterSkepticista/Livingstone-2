/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: array_dict.h,v 1.3 2002/02/26 21:29:36 lbrown Exp $ */
#ifndef array_dict_HEADER
#define array_dict_HEADER

/**
 * \file
 * Support for associative arrays.
 * The associative array consists of (key, element) pairs, with a designated
 * terminator element. The arrays can be searched on either key or element.
 * \internal Unfortunately, if these templates are used in all their
 * generality, statically-allocated associative arrays will have to be
 * initialized with static constructor calls, which interact poorly with the
 * custom memory allocator. Rather than abolish this potentially-useful class,
 * I have supplied a specialization of it that uses zero-terminated const char
 * arrays -- the old familiar C strings.
 */

/**
 * The element type of an associative array
 */

template <class K, class E>
struct Dict_item {
    K key;
    E elt;
    Dict_item(K k, E e) : key(k), elt(e) { }
};

/**
 * Search for the first item with key k
 * \param dict associative array
 * \param terminator the element that terminates the search
 * \param the key the element of which is sought
 * \return the element with the key, if found; otherwise the terminating
 * element
 */

template <class K, class E>
E search_dict_by_key(const Dict_item<K,E>* dict, E terminator, K k) {
    unsigned i;
    for(i=0; dict[i].elt != terminator; ++i) {
        if(dict[i].key == k) break;
    }
    return dict[i].elt;
}


/**
 * Search for the first item with element e
 * \param dict associative array
 * \param terminator the element that terminates the search
 * \param the element the key of which is sought
 * \return the key for the element, if found; otherwise the terminating key
 */

template <class K, class E>
K search_dict_by_elt(const Dict_item<K,E>* dict, E terminator, E e) {
    unsigned i;
    for(i=0; dict[i].elt != terminator; ++i) {
        if(dict[i].elt == e) break;
    }
    return dict[i].key;
}

/**
 * Specialization of Dict_item with const char* key.
 * No explicit constructor.
 */

template <class E>
struct String_key_dict_item {
    const char* key;
    E element;
};


/**
 * Specialization for const char* key.
 */

template <class E>
E search_dict_by_key(const String_key_dict_item<E>* dict,
		     E terminator,
		     const char* key) {
  unsigned i;
  for (i = 0; dict[i].element != terminator; ++i) {
    if (strcmp(dict[i].key, key) == 0) {
      break; // found it
    }
  }
  return dict[i].element;
}

/**
 * Specialization for const char* key.
 */

template <class E>
const char* search_dict_by_elt(const String_key_dict_item<E>* dict,
			       E terminator,
			       E element) {
  unsigned i;
  for (i = 0; dict[i].element != terminator; ++i) {
    if (dict[i].element == element) {
      break; // found it
    }
  }
  return dict[i].key;
}

#endif
