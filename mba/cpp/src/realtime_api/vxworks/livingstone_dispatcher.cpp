/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstone_dispatcher.cpp


#include <realtime_api/vxworks/livingstone_dispatcher.h>
#include <realtime_api/vxworks/queues.h>


#ifndef TRUE
#define TRUE 1
#endif



LivingstoneDispatcher::LivingstoneDispatcher(Tracker *sys, ReporterInterface &inrep) : 
report_func(inrep), thesystem(sys)
{
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

	while (1) {

		// first, wait to retrieve a message from the queue
                LivingstoneMessage *msg = new LivingstoneMessage();

                readMsgQ(MQIDTI_DIS, msg, -1);  


                cout << "  Dispatcher got msg " << msg->fcn_name << ", " <<
		  msg->args[0] << ", " << msg->args[1] << endl;

		// now parse the message and invoke
	        parse_msg(msg);

	}

	return 0;
}

void LivingstoneDispatcher::cleanup_reporting(void *arg)
{
	ReporterInterface *rptr = (ReporterInterface *) arg;
	rptr->close();
}

