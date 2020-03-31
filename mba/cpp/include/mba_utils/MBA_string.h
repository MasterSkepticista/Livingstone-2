/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef MBA_STRING_H
#define MBA_STRING_H
//
// Simple implementation of strings.
// $Id: MBA_string.h,v 1.20 2004/01/28 23:31:36 lbrown Exp $
//
// This implementation of strings implements reference counting, but
// essentially only implements immutable strings.
//
// Public interface:
//
//   MBA_string(const char* init_value = "")
//     Construct a MBA_string with the specified initial value.
//
//   MBA_string(const char* init_value, size_t n)
//     Construct a MBA_string containing the first n characters in init_value.
//     Pad with '\0' if init_value has fewer than n elements.
//
//   MBA_string(const MBA_string& s)
//     Construct a copy of the specified MBA_string.
//
//   ~MBA_string()
//     Destructor.
//
//   MBA_string& operator=(const MBA_string& s)
//     Assign one MBA_string to another.
//
//   MBA_string& operator=(const char* cs)
//     Assign a "C" string to a MBA_string.
//
//   char operator[](const size_t pos) const
//     Return the character at the specified position.  Accesses outside
//     the string result in an error.
//
//   const char* c_str() const
//     Return a "C" string corresponding to this MBA_string.  Note that the
//     returned "C" string is constant.
//
//   int compare(const char* cs) const
//     Compares this MBA_string against a "C" string.  Returns negative, zero,
//     or positive depending on whether this MBA_string is lexicographically
//     less than, equal to, or greater than the "C" string.
//
//   int compare(const MBA_string& s) const
//     Compares this MBA_string against another MBA_string.  Returns negative, zero,
//     or positive depending on whether this MBA_string is lexicographically
//     less than, equal to, or greater than the other MBA_string.
//
//   bool contains(const char* cs) const
//     Returns true if & only if MBA_string contains the "C" string as a substring
//
//  bool contains(const MBA_string& s) const
//     Returns true if & only if MBA_string contains the s as a substring
//
//   bool empty() const
//     Returns true if and only if it is an empty MBA_string.
//
//   size_t size() const
//     Returns the length of this MBA_string.
//
// Operators:
//
//   The full range of comparison operators are provided, for comparing a
//   MBA_string against another MBA_string or against a "C" string.
//
//   bool operator==(const MBA_string&, const char*)
//   bool operator==(const char*, const MBA_string&)
//   bool operator==(const MBA_string&, const MBA_string&)
//   bool operator<(const MBA_string&, const char*);
//   bool operator<(const char*, const MBA_string&);
//   bool operator<(const MBA_string&, const MBA_string&);
//
//   operator!=, operator<=, operator>=, and operator> are provided using
//   standard templates.
//
//   _STD_ ostream& operator<<(_STD_ ostream&, const MBA_string&)
//     Prints a MBA_string.


#include <assert.h>
#include <stdio.h> // sprintf
#include <string.h>
#include <livingstone/L2_iostream.h>
#include <mba_utils/pooled.h>

class MBA_string : public Pooled {

private:

  struct String_value : public Pooled {
      int    ref_count;
      size_t length;
      char * data;

      String_value(const char * init_value)
          : ref_count(1), length(strlen(init_value))
          {
              data = L2_alloc_array_no_ctor(char, length+1);
              memcpy(data, init_value, length+1); // copy '\0'
          }

      String_value(const char * init_value, size_t n)
          : ref_count(1), length(n)
          {
              data = L2_alloc_array_no_ctor(char, length+1);
              strncpy(data, init_value, n);
              data[n] = '\0';
          }

      ~String_value() {
          L2_free_array_no_dtor(data, length+1);
      }
  };

public:

  typedef int size_type;

  enum public_constants {
      // Corresponds to "no position" for function find.
      npos = -1
  };

  MBA_string(const char * init_value = "")
    : value(new String_value(init_value)) {}

  MBA_string(const char * init_value, size_t n)
    :value(new String_value(init_value, n)) {}

  MBA_string(const MBA_string& s) : Pooled(), value(s.value) {
    ++value->ref_count;
  }

  MBA_string(unsigned i) {
      char buffer[MBA_MAX_LINE_SIZE];
      sprintf(buffer, "%u", i);
      value = new String_value(buffer);
  }

  MBA_string(int i) {
      char buffer[MBA_MAX_LINE_SIZE];
      sprintf(buffer, "%d", i);
      value = new String_value(buffer);
  }

  MBA_string(const void *p) {
      char buffer[MBA_MAX_LINE_SIZE];
      sprintf(buffer, "%p", p);
      value = new String_value(buffer);
  }

  // constructor used in the + operators
  MBA_string(
          const char *left, size_t leftlen,
          const char *right, size_t rightlen)
      : value(new String_value("", leftlen+rightlen)) {
          // no need to strcpy; we know exactly how much to copy;
          // also, no need to null-term: String_value does that
          memcpy(value->data, left, leftlen);
          memcpy(value->data+leftlen, right, rightlen);
  }

  ~MBA_string() {
    if (--value->ref_count == 0)
      delete value;
  }


  inline MBA_string& operator=(const MBA_string& s);

  inline MBA_string& operator=(const char * cs);


  char operator[](const size_t pos) const {
      // no need to check pos>=0, it's unsigned
      assert(/*pos>=0 && */ pos < value->length);
      return value->data[pos];
  }

  const char * c_str() const { return value->data; }




  MBA_string::size_type find(const char * target_string) const {
      // Per built-in String class, find should return the index
      // where target_string was found.
      const char *occurrence_ptr = strstr(value->data, target_string);
      if (occurrence_ptr == NULL)
          return MBA_string::npos;
      else
          return (occurrence_ptr - value->data);
  }

  MBA_string::size_type find(const MBA_string& s) const {
      const char *occurrence_ptr = strstr(value->data, s.value->data);
      if (occurrence_ptr == NULL)
          return MBA_string::npos;
      else
          return (occurrence_ptr - value->data);
  }

  bool empty() const {
    if (value->data)
      return false;
    else
      return true;
  }

  size_t size() const { return value->length; }



  /**
   * Compare with strings or char*.  Return <0 if this is lexicographically
   * lower than the other string.
   */
  int compare(const MBA_string& s) const {
      return strcmp(value->data, s.value->data);
  }
  int compare(const char * cs) const {
      return strcmp(value->data, cs);
  }

  /**
   * Only check for equality.  This can be slightly faster.
   */
  bool operator==(const MBA_string& other) const {
      if(other.value == value) return true;
      if(other.size() != size()) return false;
      return compare(other) == 0;
  }
  bool operator==(const char *other) const {
      return compare(other) == 0;
  }
  bool operator!=(const MBA_string& other) const {
      return ! operator==(other);
  }
  bool operator!=(const char *other) const {
      return ! operator==(other);
  }

  bool operator<(const MBA_string& other) const {
      return compare(other) < 0;
  }
  bool operator<(const char *other) const {
      return compare(other) < 0;
  }

  bool operator<=(const MBA_string& other) const {
      return compare(other) <= 0;
  }
  bool operator<=(const char *other) const {
      return compare(other) <= 0;
  }
  bool operator>=(const MBA_string& other) const {
      return compare(other) >= 0;
  }
  bool operator>=(const char *other) const {
      return compare(other) >= 0;
  }

  bool operator>(const MBA_string& other) const {
      return compare(other) > 0;
  }
  bool operator>(const char *other) const {
      return compare(other) > 0;
  }



  MBA_string operator+ (const MBA_string& other) const {
      return MBA_string(c_str(), size(),
              other.c_str(), other.size());
  }
  MBA_string operator+ (const char *other) const {
      return MBA_string(c_str(), size(),
              other, strlen(other));
  }

private:

  String_value* value;
};

inline MBA_string&
MBA_string::operator=(const MBA_string& s)
{
  if (value == s.value)          // subsumes the usual test (this == &s)
    return *this;

  if (--value->ref_count == 0)
    delete value;

  value = s.value;
  ++value->ref_count;

  return *this;
}

inline MBA_string&
MBA_string::operator=(const char * cs)
{
  if (--value->ref_count == 0)
    delete value;

  value = new String_value(cs);
  return *this;
}



inline bool
operator==(const char * cs, const MBA_string& s)
{
    return s == cs;
}


inline bool
operator<(const char * cs, const MBA_string& s)
{
    return s.compare(cs) >= 0;
}

inline _STD_ ostream&
operator<<(_STD_ ostream& os, const MBA_string& s)
{
  os << s.c_str();
  return os;
}

inline MBA_string
operator + (const char *s1, const MBA_string& s2)
{
    return MBA_string(s1, strlen(s1), s2.c_str(), s2.size());
}


#endif // STRING_H
