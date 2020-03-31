/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstone_dispatcher.h

#ifndef _LIVINGSTONE_DISPATCHER_H
#define _LIVINGSTONE_DISPATCHER_H

#include <livingstone/livingstone.h>
#include <livingstone/livingstone_debug.h>
#include <opsat/cbfs_tracker.h>
#include <cover/cover_tracker.h>
#include <realtime_api/reporter.h>
#include <realtime_api/userfcns_debug.h>



/** The LivingstoneDispatcher object runs in a separate thread.
 * As implied, it takes messages from the queue (when the queue is 
 * not empty or locked -- this is monitored by the guarded queue type )
 * and invokes a corresponding Livingstone engine function. 
 **/
class LivingstoneDispatcher
{

public:
	LivingstoneDispatcher(Tracker *syst, ReporterInterface &inrep);
	virtual ~LivingstoneDispatcher() {
	};


//protected:
	// hard-coded real-time command execution

	void parse_msg(LivingstoneMessage *msg);
	virtual void *thread_member_func();

	// user-defined reporting functions
	ReporterInterface & report_func;

	// thread cleanup handler for reporting
	static void cleanup_reporting(void *arg);


//private:
	Tracker *thesystem;

};



#endif
