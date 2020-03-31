/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// threadobject.cpp

#include <realtime_api/win32/threadobject.h>

ThreadObject::ThreadObject() : hthread(NULL), threadID(0), WAIT_FOR_EXIT_TIMEOUT(10000) {}

ThreadObject::~ThreadObject()
{
	//if (hthread) 
	//	wait_for_exit();
}

void ThreadObject::start_thread()
{
	hthread = (HANDLE)_beginthreadex(NULL, 0, 
		(PBEGINTHREADEX_THREADFUNC) thread_func,
		(LPVOID) this,
		0,
		(PBEGINTHREADEX_THREADID) &threadID);

	assert(hthread);

}

// terminates thread
void ThreadObject::wait_for_exit()
{
	// call Win32 threading functions
	WaitForSingleObject(hthread, WAIT_FOR_EXIT_TIMEOUT);
	CloseHandle(hthread);
	hthread = NULL;
}

// static function to invoke thread_member_func with object pointer
DWORD WINAPI ThreadObject::thread_func(LPVOID param)
{
	ThreadObject *pobj = (ThreadObject *)param;
	return pobj->thread_member_func();
}

// presumably this will be overridden to do interesting things
DWORD ThreadObject::thread_member_func() { return 0; }
