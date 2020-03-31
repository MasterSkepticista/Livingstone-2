/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt_debug.cpp

#include <realtime_api/vxworks/livapi_rt_debug.h>
#include <time.h>
#include <unistd.h>

extern unsigned int get_command_timeout(unsigned int);
extern unsigned int get_observation_timeout(unsigned int);

L2_rtapi_debug *rti;

void *debug_observation_timeout(void *param)
{
	rti->queue_find_candidates();
	rti->queue_report_full_state();

	return 0;
}

void *debug_command_timeout(void *param)
{
	rti->queue_find_candidates();
	rti->queue_report_full_state();

	return 0;
}


L2_rtapi_debug::L2_rtapi_debug() : L2_rtapi()
{
   dribblefile.open("rtapi.drib");
   time(&start_time);
   dribblefile << "Seconds : Message\n";
   dribblefile << "-----------------\n\n";
   dribblefile << "0 :Starting dribble\n";

   rti = this;
   set_timeout_handlers(debug_command_timeout, debug_observation_timeout);
}

L2_rtapi_debug::~L2_rtapi_debug()
{
   dribblefile.flush();
   dribblefile.close();
}

void L2_rtapi_debug::queue_command(unsigned int cmd_index, unsigned int cmd_value)
{
   L2_rtapi::queue_command(cmd_index, cmd_value);
   time(&current_time);
   dribblefile << current_time - start_time << " :Command " << cmd_index << "enqueued.\n";
}

void L2_rtapi_debug::queue_observation(unsigned int obs_index, unsigned int obs_value)
{
   L2_rtapi::queue_observation(obs_index, obs_value);
   time(&current_time);
   dribblefile << current_time - start_time << " :Observation " << obs_index << " enqueued.\n";
}

void L2_rtapi_debug::queue_find_candidates()
{
   L2_rtapi::queue_find_candidates();
   time(&current_time);
   dribblefile << current_time - start_time << " :Find_candidates enqueued.\n";
}

void L2_rtapi_debug::queue_start_command_and_time(unsigned int cmd_index, unsigned int cmd_value)
{
   L2_rtapi::queue_start_command_and_time(cmd_index, cmd_value);
   time(&current_time);
      dribblefile << current_time - start_time << " :Command " << cmd_index << " issued with timeout of " << get_command_timeout(cmd_index) << " seconds.\n";
}

void L2_rtapi_debug::queue_observations_and_time(unsigned int obs_index, unsigned int obs_value)
{
   L2_rtapi::queue_observations_and_time(obs_index, obs_value);
   time(&current_time);
      dribblefile << current_time - start_time << " :Observation " << obs_index << " issued with timeout of " << get_observation_timeout(obs_index) << " seconds.\n";
}

void L2_rtapi_debug::queue_report_full_state()
{
  L2_rtapi::queue_report_full_state();
  time(&current_time);
  dribblefile << current_time - start_time << " :Report full state request enqueued.\n";
}
