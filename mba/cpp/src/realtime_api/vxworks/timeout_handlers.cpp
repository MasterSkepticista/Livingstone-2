/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// timeout_handlers.cpp for unix

#include <realtime_api/vxworks/livapi_rt.h>
#include <unistd.h>

L2_rtapi *glbl_plr;

void *default_observation_timeout(void *param)
{
	// timeout finished so put diagnosis messages on queue
	glbl_plr->queue_find_candidates();
        cout << " obs timer expired " << endl; 
	return 0;
}

void *default_command_timeout(void *param)
{
	glbl_plr->queue_find_candidates();
        cout << " cmd timer expired " << endl; 
	return 0;
}
