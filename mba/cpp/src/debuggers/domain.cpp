/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: domain.cpp,v 1.18 2004/02/21 01:19:13 lbrown Exp $ */

#include <debuggers/domain.h>
#include <readers/transition.h>
/*
 * g++ 3.2 and later deprecates <*.h> header files to be ANSI compliant, but
 * the C++ compiler for VxWorks 5.3.1, which uses g++ 2.7.2, does not support
 * namespaces, notably the STD__ namespace
 * <iOSTREAM>
 * For maximum compatibility with old compilers, don't use namespaces if they
 * can be avoided
 */
#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ > 1) || defined(_MSC_VER)
// The current ANSI standard is to define ostringstream in <sstream>
#include <sstream>
#define USING_STRINGSTREAM
#undef  USING_STRSTREAM
#elif (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 95)
// There is some instability with g++ 2.95.2, 2.95.3 and 2.96, so this may need
// more work
#include <strstream.h>
#define USING_STRSTREAM
#undef  USING_STRINGSTREAM
#else
// Earlier versions define ostrstream in strstream.h, but this is deprecated
// in later versions
#include <strstream.h>
#define USING_STRSTREAM
#undef  USING_STRINGSTREAM
#endif

// Domain name given; generate element names

Domain::Domain(const MBA_string& domainName, unsigned cardinality) :
  nvalues(cardinality), domain_name(domainName) {
  // Allocate array of element names
  values = new MBA_string[nvalues];
  // 6 usable characters (except 's' and '\0') can hold a cardinality of 10**6
  char elementName[8];
  elementName[0] = 's';
  for (unsigned i = 0; i < nvalues; ++i) {
    sprintf(elementName + 1, "%d", i);
    values[i] = elementName;
  }
}


Domain::Domain(const L2rEnumeration* pL2rEnumeration) :
  nvalues(pL2rEnumeration->nmembers()) {
  // Allocate array of element names
  values = new MBA_string[nvalues];
  {
    // Default element names
    for (unsigned i = 0; i < nvalues; ++i) {
      values[i] = MBA_string(i);
    }
    // Default domain name
    if (nvalues == 1) {
      domain_name = "1-value";
    } else {
      domain_name = MBA_string(nvalues) + "-values";
    }
  }
#ifdef ENABLE_L2_DEBUG_SECTIONS
  if (pL2rEnumeration->hasDebug()) {
    const dbg_L2rEnumeration *pdbg_L2rEnumeration =
      static_cast<const dbg_L2rEnumeration *>(pL2rEnumeration);
    // Supplied element names
    for (unsigned i = 0; i < nvalues; ++i) {
      values[i] = pdbg_L2rEnumeration->name(i);
    }
    // Supplied domain name
    domain_name = pdbg_L2rEnumeration->name();
  }
#endif
}


Domain::Domain(const L2rMode* pL2rMode) :
  nvalues(pL2rMode->domain_size()) {
  // Allocate array of element names
  values = new MBA_string[nvalues];
  {
    // Iterator over nominal Transitions
    L2rMode::iterator it = pL2rMode->begin_nominal();
    // The first (and only) nominal Transition
    const L2rTransition *pL2rTransition = *it;
    // The nominal Transition's mode
    const L2rVariable *pModeL2rVariable = pL2rTransition->mode();
#ifdef ENABLE_L2_DEBUG_SECTIONS
    if (pModeL2rVariable->hasDebug()) {
      domain_name =
	static_cast<const dbg_L2rVariable*>(pModeL2rVariable)->name();
    } else
#endif
      {
	domain_name = "v" + MBA_string(pModeL2rVariable->id());
      }
    domain_name = domain_name + "-transitions";
  }

  // The nominal Transition is always the first, and it is named "nominal"
  values[0] = "nominal";
  // The non-nominal Transitions follow the nominal Transition
  unsigned i = 1;
  for (L2rMode::iterator it = pL2rMode->begin_failure();
       it != pL2rMode->end_failure(); ++it, ++i) {
    const L2rTransition *pL2rTransition = *it;
#ifdef USING_STRINGSTREAM
  _STD_ ostringstream s;
#else
  _STD_ ostrstream s;
#endif
    pL2rTransition->printTo(s); s << '\0';
#ifdef USING_STRINGSTREAM
    values[i] = s.str().c_str();
    // no counterpart to freeze
#else
    values[i] = s.str();
    s.rdbuf()->freeze(0);
#endif
  }
}


Domain::Domain(const MBA_string& name, const MBA_string *elementNames,
	       unsigned cardinality) :
  nvalues(cardinality), domain_name(name) {
  // Allocate array of element names
  values = new MBA_string[cardinality];
  // Copy supplied element names
  for (unsigned i = 0; i < nvalues; ++i) {
    values[i] = elementNames[i];
  }
}


Domain::~Domain() {
  delete [] values;
}

int Domain::value(const MBA_string& elementName) const {
  for (unsigned i = 0; i < nvalues; ++i) {
    if (values[i] == elementName) { return i; }
  }
  return NOT_FOUND;
}
