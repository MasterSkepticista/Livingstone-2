/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: agenda_types.cpp,v 1.15 2004/01/28 22:02:31 lbrown Exp $ */

#include <opsat/agenda_types.h>

// static all default Candidate weight initialization
// This is dependent upon weight convention, so should be parameter for agenda
// constructor

Assignment::weight_t CBFSElement::empty_element_weight = 0;
