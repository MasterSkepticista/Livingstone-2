/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardedqueue.h

#ifndef _GUARDEDQUEUE_H_
#define _GUARDEDQUEUE_H_

#include <livingstone/l2conf.h>

#include <realtime_api/L2_queue.h>
#include <mba_utils/slist.h>
#include <pthread.h>

class GuardedQueue : public L2_queue 
{
public:
	GuardedQueue();
	virtual ~GuardedQueue();

	void add_message_to_queue(LivingstoneMessage *msg);
	void get_message_from_queue(LivingstoneMessage *msg);

	void is_empty();
	void is_done_empty(); // whether it's empty and not waiting for timeouts which 
	// may place another message upon it

	void increment_timeouts();
	void decrement_timeouts();

	void lock() { pthread_mutex_lock(&queue_items_mutex); }
	void release() { pthread_mutex_unlock(&queue_items_mutex); }

protected:
	Slist<LivingstoneMessage *> messages;

	pthread_cond_t notEmpty, notFull;
	pthread_cond_t isEmpty;
	static int MAX_MSGS;

	int messages_in_queue;

public:
	long timeout_handlers; // number of timeouts expected to post a message
	// on Win32 this needs to be a 32 bit -- Unix?

	/// the mutex protecting the queue items from multiple access
	pthread_mutex_t queue_items_mutex;

	pthread_cond_t hzero_event;
	pthread_mutex_t timeout_handlers_mutex;

};


#endif
