/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardedqueue.cpp

#include <livingstone/L2_assert.h>

#include <realtime_api/posix/guardedqueue.h>

int GuardedQueue::MAX_MSGS = 100;


GuardedQueue::GuardedQueue() : messages_in_queue(0), timeout_handlers(0)
{
	int err = pthread_mutex_init(&queue_items_mutex, NULL);
	L2_assert(!err,L2_resource_error,("Mutex initialization error for queue items.")); 
	err = pthread_cond_init(&notEmpty, NULL);
	L2_assert(!err, L2_resource_error,("Failed pthread condition variable initialization."));

	err =  pthread_cond_init(&notFull, NULL);
	L2_assert(!err, L2_resource_error,("Failed pthread condition variable intialization."));

	err = pthread_cond_init(&isEmpty, NULL);
	L2_assert(!err, L2_resource_error,("Failed pthread condition variable initialization."));

	err = pthread_mutex_init(&timeout_handlers_mutex,NULL);
	L2_assert(!err, L2_resource_error,("Failed pthread mutex initialization."));

	err = pthread_cond_init(&hzero_event, NULL);
	L2_assert(!err, L2_resource_error,("Failed pthread condition variable initialization."));

}

GuardedQueue::~GuardedQueue()
{
	pthread_mutex_destroy(&queue_items_mutex);

	pthread_cond_destroy(&notEmpty);
	pthread_cond_destroy(&notFull);
	pthread_cond_destroy(&isEmpty);

	pthread_cond_destroy(&hzero_event);
	pthread_mutex_destroy(&timeout_handlers_mutex);
}

void GuardedQueue::is_empty()
{
	lock();
	while ( 0 != messages_in_queue )
		pthread_cond_wait(&isEmpty, &queue_items_mutex );

	release();
}

void GuardedQueue::is_done_empty()
{
	is_empty();
	while ( timeout_handlers!=0)
		pthread_cond_wait(&hzero_event, &timeout_handlers_mutex);
	is_empty();
}

void GuardedQueue::add_message_to_queue(LivingstoneMessage *msg)
{
	lock();
// use unix condition variables here (rather than guard condition,
// have to pass dataguard mutex as cv's mutex)
	while (MAX_MSGS == messages_in_queue)
		pthread_cond_wait(&notFull, &queue_items_mutex );

	// returned from wait, no longer full
	messages.push_back(msg);
	messages_in_queue++;

	pthread_cond_broadcast(&notEmpty);
	release();
}

void GuardedQueue::get_message_from_queue(LivingstoneMessage *msg)
{
	lock();
	if ( 0 == messages_in_queue )
	{
		pthread_cond_broadcast(&isEmpty);
		while ( !(0 < messages_in_queue) )
			pthread_cond_wait(&notEmpty, &queue_items_mutex );
	}
			

	// returned from wait so can do get
	// need to make assignment for messages -- this shouldn't be here
	*msg = *messages.front();
	messages.pop_front();
	messages_in_queue--;

	pthread_cond_broadcast(&notFull);
	release();
}

void GuardedQueue::increment_timeouts()
{
	pthread_mutex_lock(&timeout_handlers_mutex);
	++timeout_handlers;
	pthread_mutex_unlock(&timeout_handlers_mutex);
	
}

void GuardedQueue::decrement_timeouts()
{
	pthread_mutex_lock(&timeout_handlers_mutex);
	--timeout_handlers;
	pthread_cond_broadcast(&hzero_event);
	pthread_mutex_unlock(&timeout_handlers_mutex);
}

