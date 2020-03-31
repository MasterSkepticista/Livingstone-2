/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardedqueue.h

#ifndef _GUARDEDQUEUE_H_
#define _GUARDEDQUEUE_H_

#ifdef WIN32
#include <realtime_api/win32/dataguard.h>
#include <realtime_api/win32/guardcondition.h>
#else
#include <realtime_api/posix/dataguard.h>
#endif

#include <realtime_api/livingstonemessage.h>
#include <mba_utils/slist.h>

#ifdef WIN32
class GuardCondition;
#endif

class GuardedQueue : public DataGuard
{
public:
	GuardedQueue();
	virtual ~GuardedQueue();

	void add_message_to_queue(LivingstoneMessage *msg);
	void get_message_from_queue(LivingstoneMessage *msg);

	void is_empty();
	void is_done_empty(); // whether it's empty and not waiting for timeouts which 
	// may place another message upon it

#ifndef WIN32
	void increment_timeouts();
	void decrement_timeouts();
#endif

protected:
	Slist<LivingstoneMessage *> messages;
#ifdef WIN32
	GuardCondition *notEmpty, *notFull;
	GuardCondition *isEmpty;
#else
	pthread_cond_t notEmpty, notFull;
	pthread_cond_t isEmpty;
#endif
	static int MAX_MSGS;

	int messages_in_queue;

public:
	long timeout_handlers; // number of timeouts expected to post a message
	// on Win32 this needs to be a 32 bit -- Unix?
#ifdef WIN32
	HANDLE hzero_event;
#else
	pthread_cond_t hzero_event;
	pthread_mutex_t timeout_handlers_mutex;
#endif

};


#endif
