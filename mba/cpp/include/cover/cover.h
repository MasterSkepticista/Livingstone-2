/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// Conflict coverage algorithm, NASA Ames Research Center
// See Copyright.txt
// $Id: cover.h,v 1.12 2002/01/11 16:06:30 lbrown Exp $

#ifndef COVER_H
#define COVER_H

#include <conflict_db/conflict_db.h>
#include <conflict_db/candidate.h>
	
/// This function implements the rho_coverage algorithm, which we will rename
/// the Gama algorithm, in honor of Vasco de Gama (or Gamma?)

void  rho_coverage(Conflict_db *db,
		   Candidate::ListOfCandidate& candidates,
		   Assignment::weight_t rho);


#endif  /* COVER_H */
