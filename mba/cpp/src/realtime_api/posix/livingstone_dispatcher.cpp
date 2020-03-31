/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstone_dispatcher.cpp


#include <realtime_api/posix/livingstone_dispatcher.h>

#ifndef TRUE
#define TRUE 1
#endif

LivingstoneDispatcher::LivingstoneDispatcher(L2_queue *queue, Tracker *sys, ReporterInterface &inrep) : 
ThreadObject(), report_func(inrep), thequeue(queue), thesystem(sys)
{
	start_thread();
}

void LivingstoneDispatcher::parse_msg(LivingstoneMessage *msg)
{

	// switch on the function specified, parse arguments accordingly
	switch(msg->fcn_name) {
	case COMMAND:
	   thesystem->progress(msg->args[0],msg->args[1]);
		 
	   // output string vales of progress and args
	   report_func(COMMAND, msg->args[0], msg->args[1]);
		break;
	case OBSERVATION:
	   thesystem->observe(msg->args[0],msg->args[1]);
	   report_func(OBSERVATION, msg->args[0], msg->args[1]);
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

#define THREAD_RETURN void*

THREAD_RETURN LivingstoneDispatcher::thread_member_func()
{
	report_func.open(); // opens stream for output
	pthread_cleanup_push(cleanup_reporting,  &report_func);
			     
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		// requires pthread_test_cancel() calls as appropriate

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);


	// this is where it all takes place
	LivingstoneMessage msg;

	while (1) {

// note this is okay, since if the dispatcher is already suspended, it's 
// in a condition variable wait, that is a cancellation point
// Note also that WIN32 in fact needs something like this, TODO
		pthread_testcancel();

		// first, wait to retrieve a message from the queue
		thequeue->get_message_from_queue(&msg);

		// now parse the message and invoke
		parse_msg(&msg);

	}

	pthread_cleanup_pop(1); // should never get here, actually
	return 0;
}

void LivingstoneDispatcher::cleanup_reporting(void *arg)
{
	ReporterInterface *rptr = (ReporterInterface *) arg;
	rptr->close();
}

