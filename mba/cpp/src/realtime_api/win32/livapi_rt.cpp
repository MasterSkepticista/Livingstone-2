/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt.cpp

#include <realtime_api/win32/livapi_rt.h>
#include <realtime_api/livingstonemessage.h>
#include <../src/test/time_delays.h>


#ifdef WIN32
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC)
	(LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;

#endif


class CBFS_tracker;
class Cover_tracker;

LivingstoneRealTimeAPI::LivingstoneRealTimeAPI(Tracker *syst, 
											   ofstream *fp)
	{
		thesystem = syst;
		dispatcher = new LivingstoneDispatcher(&thequeue, thesystem, fp);
		glbl_plr = this;
#ifndef WIN32
		pthread_attr_init(&timer_attr);
		pthread_attr_setdetachstate(&timer_attr, PTHREAD_CREATE_DETACHED);
		pthread_attr_setscope(&timer_attr, PTHREAD_SCOPE_SYSTEM);
#endif

	}

LivingstoneRealTimeAPI::~LivingstoneRealTimeAPI()
{
		// normal behavior is to wait until the queue is emptied
	thequeue.is_done_empty();

		// kill dispatcher process and delete object
	// the dispatcher object may or may not be suspended when arriving here

// at this point, the dispatcher is processing the last message.
// BUT, the call below does not currently guarantee it gets to complete 
// that message -- that's what wait for exit should do in the dispatcher 
// ensure its task is completed, then notify -- could set exit bit for 
// next loop before hitting get_msg.

	dispatcher->wait_for_exit();
	delete dispatcher;

#ifndef WIN32
	pthread_attr_destroy(&timer_attr);
#endif
}



// adds command and its assignment to the message queue
void LivingstoneRealTimeAPI::queue_command(unsigned int cmd_index, 
										   unsigned int cmd_value)
{
	LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
	thequeue.add_message_to_queue(msg);
}

// adds monitor observed values to the message queue for assignment
void LivingstoneRealTimeAPI::queue_observation(unsigned int obs_index, 
											   unsigned int value_index)
{
	LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
	thequeue.add_message_to_queue(msg);
}

void LivingstoneRealTimeAPI::queue_find_candidates()
{
	LivingstoneMessage *msg = new LivingstoneMessage(FIND_CANDIDATES);
	thequeue.add_message_to_queue(msg);
}

void LivingstoneRealTimeAPI::queue_start_command_and_time(unsigned int cmd_index, 
														  unsigned int cmd_value)
{
#ifdef WIN32
//		HANDLE hthread;
		DWORD threadID; // probably will need to use this for command abortion eventually

		DWORD *delay = new DWORD;
		*delay = get_command_timeout(cmd_index);

		_beginthreadex(NULL, 0, 
			(PBEGINTHREADEX_THREADFUNC)command_timeout,
			(LPVOID) delay,
			0,
			(PBEGINTHREADEX_THREADID) &threadID);

		//assert (hthread);
		InterlockedIncrement((LPLONG) (&thequeue.timeout_handlers));
		ResetEvent(thequeue.hzero_event);
#else
		int *delay = new int;
		*delay = get_command_timeout(cmd_index);
		pthread_t tid;

		pthread_create(&tid, &timer_attr, command_timeout, delay);
		thequeue.increment_timeouts();
#endif
	LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
	thequeue.add_message_to_queue(msg);
}

void LivingstoneRealTimeAPI::queue_observations_and_time(unsigned int obs_index, 
														 unsigned int value_index)
{
#ifdef WIN32
//		HANDLE hthread;
		DWORD threadID; // probably will need to use this for command abortion eventually

		DWORD *delay = new DWORD;
		*delay = get_observation_timeout(obs_index);
		_beginthreadex(NULL, 0, 
		(PBEGINTHREADEX_THREADFUNC) observation_timeout,
		(LPVOID) delay,
		0,
		(PBEGINTHREADEX_THREADID) &threadID);

		InterlockedIncrement((LPLONG) (&thequeue.timeout_handlers));
		ResetEvent(thequeue.hzero_event);
#else
		int *delay = new int;
		*delay = get_command_timeout(obs_index);
		pthread_t tid;

		pthread_create(&tid, &timer_attr, observation_timeout, delay);
		thequeue.increment_timeouts();
#endif

	LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
	thequeue.add_message_to_queue(msg);
}

void LivingstoneRealTimeAPI::queue_abort_command()
{
}

void LivingstoneRealTimeAPI::queue_report_full_state()
{
	LivingstoneMessage *msg = new LivingstoneMessage(REPORT_FULL_STATE);
	thequeue.add_message_to_queue(msg);
}

