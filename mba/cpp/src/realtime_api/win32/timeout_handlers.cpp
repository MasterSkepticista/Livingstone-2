/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// timeout_handlers.cpp

#include <realtime_api/win32/livapi_rt.h>

LivingstoneRealTimeAPI *glbl_plr;

// note that the queue handler count must  be decremented
// need to changed these to default and provide clear override mechanism
DWORD WINAPI observation_timeout(LPVOID param)
{
	DWORD sleeptime = *((DWORD *)param);
	SleepEx(sleeptime, TRUE);

	delete param;

	// time out finished so put diagnosis messages on queue
	GuardedQueue *queue = &glbl_plr->thequeue;
	LivingstoneMessage *mptr2 = new LivingstoneMessage(FIND_CANDIDATES);
	queue->add_message_to_queue(mptr2);


	if(!InterlockedDecrement((&queue->timeout_handlers)))
		SetEvent(queue->hzero_event);
	return 0;
}

// note that the queue handler count must be decremented!
// need to change name to default and provide clear override mechanism
DWORD WINAPI command_timeout(LPVOID param)
{
	DWORD sleeptime = *((DWORD *)param);

	// time out finished so put diagnosis messages on queue -- probably want to do 
	// something different here
	GuardedQueue *queue = &glbl_plr->thequeue;

	SleepEx(sleeptime, TRUE);

	delete param;

	LivingstoneMessage *mptr2 = new LivingstoneMessage(FIND_CANDIDATES);
	queue->add_message_to_queue(mptr2);

	if(!InterlockedDecrement((&queue->timeout_handlers)))
		SetEvent(queue->hzero_event);

	return 0;
}


