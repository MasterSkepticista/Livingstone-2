/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// guardedqueue.cpp


#ifdef WIN32
#include <realtime_api/win32/guardedqueue.h>
#include <realtime_api/win32/guardcondition.h>
#else
#include <realtime_api/posix/guardedqueue.h>
#include <realtime_api/posix/guardcondition.h>
#endif

int GuardedQueue::MAX_MSGS = 100;


GuardedQueue::GuardedQueue() : messages_in_queue(0), timeout_handlers(0)
{
#ifdef WIN32
	notEmpty = new GuardCondition(this);
	notFull = new GuardCondition(this);
	isEmpty = new GuardCondition(this);

	hzero_event = CreateEvent(NULL, TRUE, TRUE, "zero_timeouts");
#else
	pthread_cond_init(&notEmpty, NULL);
	pthread_cond_init(&notFull, NULL);
	pthread_cond_init(&isEmpty, NULL);

	pthread_mutex_init(&timeout_handlers_mutex,NULL);
	pthread_cond_init(&hzero_event, NULL);
#endif

}

GuardedQueue::~GuardedQueue()
{
#ifdef WIN32
	delete notEmpty;
	delete notFull;
	delete isEmpty;
	CloseHandle(hzero_event);
#else
	pthread_cond_destroy(&notEmpty);
	pthread_cond_destroy(&notFull);
	pthread_cond_destroy(&isEmpty);

	pthread_cond_destroy(&hzero_event);
	pthread_mutex_destroy(&timeout_handlers_mutex);
#endif
}

void GuardedQueue::is_empty()
{
	lock();
#ifdef WIN32
	if ( 0 != messages_in_queue)
		isEmpty->wait();
#else
	while ( 0 != messages_in_queue )
		pthread_cond_wait(&isEmpty, &pmutex );
#endif

	release();
}

void GuardedQueue::is_done_empty()
{
	is_empty();
#ifdef WIN32
	WaitForSingleObject(hzero_event, INFINITE);
#else
	while ( timeout_handlers!=0)
		pthread_cond_wait(&hzero_event, &timeout_handlers_mutex);
#endif
	is_empty();
}

void GuardedQueue::add_message_to_queue(LivingstoneMessage *msg)
{
	lock();
// use unix condition variables here (rather than guard condition,
// have to pass dataguard mutex as cv's mutex)
#ifdef WIN32
	if (MAX_MSGS == messages_in_queue) notFull->wait();
#else
	while (MAX_MSGS == messages_in_queue)
		pthread_cond_wait(&notFull, &pmutex );
#endif

	// returned from wait, no longer full
	messages.push_back(msg);
	messages_in_queue++;

#ifdef WIN32
	notEmpty->signal();
#else
	pthread_cond_broadcast(&notEmpty);
#endif
	release();
}

void GuardedQueue::get_message_from_queue(LivingstoneMessage *msg)
{
	lock();
#ifdef WIN32
	if ( 0 == messages_in_queue) 
	{
		isEmpty->signal();
		notEmpty->wait();
	}

#else
	if ( 0 == messages_in_queue )
	{
		pthread_cond_broadcast(&isEmpty);
		while ( !(0 < messages_in_queue) )
			pthread_cond_wait(&notEmpty, &pmutex );
	}
#endif
			

	// returned from wait so can do get
	// need to make assignment for messages -- this shouldn't be here
	*msg = *messages.front();
	messages.pop_front();
	messages_in_queue--;

#ifdef WIN32
	notFull->signal();
#else
	pthread_cond_broadcast(&notFull);
#endif
	release();
}

#ifndef WIN32
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

#endif
