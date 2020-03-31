/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt.h

#ifndef _LIVAPI_RT_H
#define _LIVAPI_RT_H

#include <livingstone/l2conf.h>

#include <realtime_api/posix/guardedqueue.h>
class LivingstoneDispatcher;
class ReporterInterface;
class Tracker;


class L2_rtapi;
extern L2_rtapi *glbl_plr;

void *default_observation_timeout(void *param);
void *default_command_timeout(void *param);


class L2_rtapi
{

public:


   L2_rtapi(Tracker * syst, ReporterInterface &liv_report );
   ~L2_rtapi();


// adds command and its assignment to the message queue
   virtual void queue_command(unsigned int cmd_index, unsigned int cmd_value);
// adds monitor observed values to the message queue for assignment
   virtual void queue_observation(unsigned int obs_index,
											   unsigned int value_index);
   void queue_observations(int number, unsigned int obs [], unsigned int values[]);
   void queue_find_candidates();
   virtual void queue_start_command_and_time(unsigned int cmd_index,
														  unsigned int cmd_value);
   virtual void queue_observations_and_time(unsigned int obs_index,
														 unsigned int value_index);
   void queue_abort_command();
   void queue_report_full_state();

   void * (*observation_timeout)(void *param);
   void * (*command_timeout)(void *param);
   void set_timeout_handlers( void *(*cmd)(void *param)=default_command_timeout, void *(*obs)(void *param)=default_observation_timeout);

   pthread_attr_t timer_attr;

//protected:
   GuardedQueue thequeue;
   Tracker *thesystem;
   LivingstoneDispatcher *dispatcher;

};


#endif
