/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: trajectory.cpp,v 1.8 2002/07/16 21:53:20 lbrown Exp $ */
#include <debuggers/trajectory.h>

#include <conflict_db/assignable.h> // a->get_variable is Assignable

Trajectory::Parts::Parts(const Assignment* a) :
  time(a->get_variable()->get_time_step()),
  rank(a->get_weight()), id(a->get_variable()->get_id()),
  index(a->get_variable()->get_value_index()) { }
