/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt.h

#ifndef _LIVAPI_RT_H
#define _LIVAPI_RT_H

#include <api/livingstone.h>
#include <realtime_api/win32/guardedqueue.h>
#include <fstream.h>
#include <realtime_api/win32/livingstone_dispatcher.h>

// icky fix
class LivingstoneRealTimeAPI;
extern LivingstoneRealTimeAPI *glbl_plr;

#ifdef WIN32
DWORD WINAPI observation_timeout(LPVOID param);
DWORD WINAPI command_timeout(LPVOID param);
#else
void *observation_timeout(void *param);
void *command_timeout(void *param);
#endif


class LivingstoneRealTimeAPI
{

public:


	LivingstoneRealTimeAPI(Tracker * syst, ofstream *fp);

	~LivingstoneRealTimeAPI();


// adds command and its assignment to the message queue
   void queue_command(unsigned int cmd_index, unsigned int cmd_value);
// adds monitor observed values to the message queue for assignment
   void queue_observation(unsigned int obs_index, 
											   unsigned int value_index);
   void queue_find_candidates();
   void queue_start_command_and_time(unsigned int cmd_index, 
														  unsigned int cmd_value);
   void queue_observations_and_time(unsigned int obs_index, 
														 unsigned int value_index);
   void queue_abort_command();
   void queue_report_full_state();

#ifdef WIN32
	friend DWORD WINAPI observation_timeout(LPVOID param);
	friend DWORD WINAPI command_timeout(LPVOID param);
#else
	friend void *observation_timeout(void *param);
	friend void *command_timeout(void *param);

	pthread_attr_t timer_attr;
#endif


//protected:
   GuardedQueue thequeue;
   Tracker *thesystem;
   LivingstoneDispatcher *dispatcher;

};


#endif
