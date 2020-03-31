/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardcondition.h

#ifndef _GUARDCONDITION_H_
#define _GUARDCONDITION_H_

#include <windows.h>
#include <process.h>
#include <assert.h>

class DataGuard;

// a condition variable class intended for use by threads accessing a 
// DataGuard - implemented using a binary semaphore
class GuardCondition
{
public:
	GuardCondition( DataGuard *guard);
	virtual ~GuardCondition(); // releases semaphore

	// called to release DataGuard and block until signaled
	void wait();

	// release a thread blocked on the semaphore, if any
	void signal();

protected:

	HANDLE hsemaphore;
	int wait_count; // number of waiting threads
	DataGuard *dataguard;
};


#endif
