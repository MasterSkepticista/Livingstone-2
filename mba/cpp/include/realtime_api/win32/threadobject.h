/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// threadobject.h -- declaration of a base class for an object to run in its 
// own thread under Win32

#ifndef _THREAD_OBJECT_H
#define _THREAD_OBJECT_H

#include <windows.h>
#include <assert.h>
#include <process.h>

// typedefs required for incorrect begin_threadex prototypes
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC)
	(LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;

class ThreadObject
{
public:
	ThreadObject();
	virtual ~ThreadObject();

	void start_thread();
	void wait_for_exit();

	// static function needed to invoke thread_member_func
	// since system won't automatically push this pointer onto stack
	static DWORD WINAPI thread_func(LPVOID param);

protected:
	virtual DWORD thread_member_func();
	HANDLE hthread;
	DWORD threadID;

	DWORD WAIT_FOR_EXIT_TIMEOUT;
};

#endif