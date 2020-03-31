/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstone_dispatcher.h

#ifndef _LIVINGSTONE_DISPATCHER_H
#define _LIVINGSTONE_DISPATCHER_H

#include <realtime_api/livingstonemessage.h>
#include <opsat/cbfs_tracker.h>
#include <cover/cover_tracker.h>
#include <realtime_api/win32/guardedqueue.h>
#include <realtime_api/userfcns.h>

#include <fstream.h>

#ifdef WIN32
#include <realtime_api/win32/threadobject.h>
#include <windows.h>
#else
#include <realtime_api/posix/threadobject.h>
#include <pthread.h>
#endif


/** The LivingstoneDispatcher object runs in a separate thread.
 * As implied, it takes messages from the queue (when the queue is 
 * not empty or locked -- this is monitored by the guarded queue type )
 * and invokes a corresponding Livingstone engine function. 
 **/
class LivingstoneDispatcher : public ThreadObject
{

public:
	LivingstoneDispatcher( GuardedQueue *queue, Tracker *syst, ofstream *fp);
	virtual ~LivingstoneDispatcher() {
	};


//protected:
	// hard-coded real-time command execution

	void parse_msg(LivingstoneMessage *msg);
#ifdef WIN32
    virtual DWORD thread_member_func();
#else
   virtual void *thread_member_func();
#endif

	// user-defined reporting functions
	ReportTransitions & report_func;
	//ReportCandidates


//private:
	GuardedQueue *thequeue;
	Tracker *thesystem;

	bool verbose; // are commands echoed? default is yes

};



#endif
