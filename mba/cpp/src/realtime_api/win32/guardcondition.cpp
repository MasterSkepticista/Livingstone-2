/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardcondition.cpp

#include <realtime_api/win32/guardcondition.h>
#include <realtime_api/win32/dataguard.h>

GuardCondition::GuardCondition(DataGuard *guard) : dataguard(guard),
	wait_count(0), hsemaphore(0)
{
	hsemaphore = CreateSemaphore(NULL, 0, 1, NULL); 
	assert(hsemaphore);
}

GuardCondition::~GuardCondition() { CloseHandle(hsemaphore); }

void GuardCondition::wait()
{
	wait_count++;
	dataguard->release(); // release data lock
	WaitForSingleObject(hsemaphore, INFINITE); // block until signaled

	dataguard->lock();
	wait_count--; // got data lock, no longer in wait state
}

void GuardCondition::signal()
{
	if (wait_count > 0) ReleaseSemaphore(hsemaphore,1,NULL); 
}
