/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt.cpp

#include <realtime_api/posix/livapi_rt.h>
#include <realtime_api/livingstonemessage.h>
#include <realtime_api/posix/livingstone_dispatcher.h>
#include <realtime_api/reporter.h>
//#include <test/time_delays.h>
#include <livingstone/L2_assert.h>

#include <unistd.h>


class CBFS_tracker;
class Cover_tracker;

extern unsigned int get_observation_timeout(unsigned int);
extern unsigned int get_command_timeout(unsigned int);

// this is the function called to create a timer, then invoke an appropriate
// handler after the specified timeout has elapsed.
void *rti_timer(void *param)
{
	LivingstoneMessage *msg = ((LivingstoneMessage *)param);
	int sleeptime = 0;
 	switch (msg->fcn_name)
 	{
 	case OBSERVATION:
 	   sleeptime = get_observation_timeout(msg->args[0]);
 	   while (sleeptime > 0)
 		sleeptime = sleep(sleeptime);
 	   glbl_plr->observation_timeout(NULL);
 	   break;
 	case COMMAND:
 	   sleeptime = get_command_timeout(msg->args[0]);
 	   while (sleeptime > 0)
 	      sleeptime = sleep(sleeptime);
 	   glbl_plr->command_timeout(NULL);
 	   break;
 	default:
 	   break;
 	}

	return 0;
}

L2_rtapi::L2_rtapi(Tracker *syst, ReporterInterface &liv_report )
{
   thesystem = syst;
   dispatcher = NULL;
   dispatcher = new LivingstoneDispatcher(&thequeue, thesystem, liv_report);
   L2_assert(dispatcher, L2_resource_error,("Error while attempting to allocate LivingstoneDispatcher object"));
   glbl_plr = this;

   int err = pthread_attr_init(&timer_attr);
   L2_assert(!err, L2_resource_error,("Error in L2_rtapi constructor while attempting to initialize pthread attributes."));

   pthread_attr_setdetachstate(&timer_attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_setscope(&timer_attr, PTHREAD_SCOPE_SYSTEM);

   set_timeout_handlers();
}

L2_rtapi::~L2_rtapi()
{

   // normal behavior is to wait until the queue is emptied
   thequeue.is_done_empty();

   // kill dispatcher process and delete object
  // the dispatcher object may or may not be suspended when arriving here

   dispatcher->wait_for_exit();
   delete dispatcher;

   pthread_attr_destroy(&timer_attr);

}


// adds command and its assignment to the message queue
void L2_rtapi::queue_command(unsigned int cmd_index,
										   unsigned int cmd_value)
{
   LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));
   thequeue.add_message_to_queue(msg);
}

// adds monitor observed values to the message queue for assignment
void L2_rtapi::queue_observation(unsigned int obs_index,
											   unsigned int value_index)
{
   LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));
   thequeue.add_message_to_queue(msg);
}

void L2_rtapi::queue_observations(int number, unsigned int obs[], unsigned int values[])
{
   for (int i=0; i<number; i++)
      queue_observation(obs[i],values[i]);
}

void L2_rtapi::queue_find_candidates()
{
   LivingstoneMessage *msg = new LivingstoneMessage(FIND_CANDIDATES);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));
   thequeue.add_message_to_queue(msg);
}

void L2_rtapi::queue_start_command_and_time(unsigned int cmd_index, unsigned int cmd_value)
{
   LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));

   // start timeout task
   pthread_t tid;
   int err = pthread_create(&tid, &timer_attr, rti_timer, msg);
   L2_assert(!err, L2_resource_error,("Error while attempting to create command timer pthread."));
   thequeue.increment_timeouts();

   thequeue.add_message_to_queue(msg);
}

void L2_rtapi::queue_observations_and_time(unsigned int obs_index,
	 unsigned int value_index)
{
   LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));

// start timeout task
   pthread_t tid;
   int err = pthread_create(&tid, &timer_attr, rti_timer, msg);
   L2_assert(!err, L2_resource_error,("Error while attempting to create observation timer pthread."));
   thequeue.increment_timeouts();

   thequeue.add_message_to_queue(msg);
}

void L2_rtapi::queue_abort_command()
{
}

void L2_rtapi::queue_report_full_state()
{
   LivingstoneMessage *msg = new LivingstoneMessage(REPORT_FULL_STATE);
   L2_assert(msg, L2_resource_error,("Out of memory for LivingstoneMessages."));
   thequeue.add_message_to_queue(msg);
}

void L2_rtapi::set_timeout_handlers( void*(*cmd)(void *), void *(*obs)(void *) )
{
   command_timeout = cmd;
   observation_timeout = obs;
}

