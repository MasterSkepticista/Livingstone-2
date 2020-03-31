/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// debug wrapper for realtime api

#ifndef LIVAPI_RT_DEBUG_H_
#define LIVAPI_RT_DEBUG_H_

#include <livingstone/l2conf.h>   

#include <realtime_api/posix/livapi_rt.h>
#include <realtime_api/userfcns.h>
#include <fstream.h>
#include <sys/types.h>

/** The L2_rtapi_debug class provides a debug wrapper for the L2_rtapi. 
* The significant difference is that a dribble file created.
**/
class L2_rtapi_debug : public L2_rtapi
{
public:
   L2_rtapi_debug(Tracker *syst, ReporterInterface &livrep_in);
   virtual ~L2_rtapi_debug();

   // the public api mirrors the base class api
   void queue_command(unsigned int cmd_index, unsigned int cmd_value);
   void queue_observation(unsigned int obs_index, unsigned int obs_value);
   void queue_find_candidates();
   void queue_start_command_and_time(unsigned int cmd_index, unsigned int value_index);
   void queue_observations_and_time(unsigned int obs_index, unsigned int value_index);
   void queue_report_full_state();

   time_t start_time;
   time_t current_time;
protected:
   ofstream dribblefile;
};

#endif

