/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstone_dispatcher.cpp


#include <realtime_api/win32/livingstone_dispatcher.h>

#ifndef TRUE
#define TRUE 1
#endif

LivingstoneDispatcher::LivingstoneDispatcher(GuardedQueue *queue,
											 Tracker *sys, ofstream *fp) : thequeue(queue),
											 thesystem(sys), verbose(TRUE),
											 report_func(*(new ReportTransitions(sys))), ThreadObject()
{
	start_thread();
}

void LivingstoneDispatcher::parse_msg(LivingstoneMessage *msg)
{

	LivingstoneMessage *msgnew = NULL;

	// switch on the function specified, parse arguments accordingly
	switch(msg->fcn_name) {
	case COMMAND:
		thesystem->progress(msg->args[0],msg->args[1]);
		if (verbose)
			// output string vales of progress and args
			report_func(COMMAND, msg->args[0], msg->args[1]);
		break;
	case OBSERVATION:
		thesystem->observe(msg->args[0],msg->args[1]);
		if (verbose)
		{
			report_func(OBSERVATION, msg->args[0], msg->args[1]);
		}
		break;
	case FIND_CANDIDATES:
		thesystem->find_candidates();
		report_func(FIND_CANDIDATES); 
		break;
	case ABORT_COMMAND:
		break;
	case REPORT_FULL_STATE:
		// need to call the user's function here
		// assume it was a function object passed upon api creation
		report_func();
		break;
	default:
		break;
	}

}

#ifdef WIN32 
#define THREAD_RETURN DWORD
#else
#define THREAD_RETURN void*
#endif

THREAD_RETURN LivingstoneDispatcher::thread_member_func()
{
#ifndef WIN32
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
		// requires pthread_test_cancel() calls as appropriate

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
	// this is where it all takes place
	LivingstoneMessage msg;

	while (1) {

#ifndef WIN32
// note this is okay, since if the dispatcher is already suspended, it's 
// in a condition variable wait, that is a cancellation point
// Note also that WIN32 in fact needs something like this, TODO
		pthread_testcancel();
#endif

		// first, wait to retrieve a message from the queue
		thequeue->get_message_from_queue(&msg);

		// now parse the message and invoke
		parse_msg(&msg);

	}

	return 0;
}


