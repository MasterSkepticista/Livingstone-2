/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Implementation of proposition methods
// $Id: proposition.cpp,v 1.30 2004/01/28 22:02:38 lbrown Exp $

#include <tms/proposition.h>
#include <tms/clause.h>

// Set the mark, then mark all our Clauses as well.

void Proposition::set_destroying() {
  set_flag(true, DESTROYING);
  {
    for (iterator it = begin_positive(); it != end_positive(); ++it) {
      Clause *pClause = *it;
      pClause->set_destroying();
    }
  }
  {
    for (iterator it = begin_negative(); it != end_negative(); ++it) {
      Clause *pClause = *it;
      pClause->set_destroying();
    }
  }
}
