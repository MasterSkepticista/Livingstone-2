/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// dataguard.h

#ifndef _DATAGUARD_H
#define _DATAGUARD_H

#include <windows.h>
#include <process.h>
#include <assert.h>

class GuardCondition;

class DataGuard
{
public:
	DataGuard() : hmutex(NULL) { 
		hmutex = CreateMutex(NULL, FALSE, NULL); 
		assert(hmutex); 
	}

	virtual ~DataGuard() { CloseHandle(hmutex); }

protected:
	HANDLE hmutex;
	
	virtual void lock() { WaitForSingleObject(hmutex, INFINITE); }
	virtual void release() { ReleaseMutex(hmutex); }

	friend class GuardCondition;
};

#endif