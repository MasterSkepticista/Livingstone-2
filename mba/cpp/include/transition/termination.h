/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: termination.h,v 1.5 2001/11/16 19:32:24 lbrown Exp $ */
#ifndef termination_HEADER
#define termination_HEADER

#include <livingstone/l2conf.h>



/// Possible answers to a query on whether it is necessary to terminate the
/// search before finding the desired number of solutions.  Aside from
/// SEARCH_CONTINUE, all are reasons for immediate termination.

///  Used by the Tracker and the classes that implement the search;
///  the latter are why I put it in a separate file (to avoid
///  circular includes between them and the tracker).

enum SearchTermination {
    SEARCH_CONTINUE,    // no reason to terminate
    EMPTY_AGENDA,       //
    WEIGHT_CUTOFF,      //
    MAX_OVERRUN         //
};

/**
EMPTY_AGENDA: The search terminated because it couldn't construct any more
candidates. This means no (more) solution(s) exist(s) in the L2 framework
(ignoring issues of incomplete propagation that can impact the results in
any case).

WEIGHT_CUTOFF: No more candidates can be constructed that have weights less
than the specified cutoff paramter.

MAX_OVERRUN: This means that the maximum number of nodes that can be
checked for consistency has been exceeded. The CBFS search algorithm max memory
use is bounded by 2*max_overrun_count where max_overrun_count is the input
parameter that triggers this termination.
**/

#endif
