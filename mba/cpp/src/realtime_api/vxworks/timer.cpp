/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include "taskLib.h" 
#include "sysLib.h" 

#include <realtime_api/livingstonemessage.h>
#include <realtime_api/vxworks/livapi_rt.h>


extern unsigned int get_observation_timeout(unsigned int);
extern unsigned int get_command_timeout(unsigned int);


// this is the function called to create a timer, then invoke an appropriate 
// handler after the specified timeout has elapsed.

void timer_monitor(void *param, L2_rtapi *glbl_plr)
{
	LivingstoneMessage *msg = ((LivingstoneMessage *)param);
	int sleeptime = 0;
 	switch (msg->fcn_name)
 	{
 	case OBSERVATION:
 	   sleeptime = get_observation_timeout(msg->args[0]);
           taskDelay(sleeptime * sysClkRateGet());
           cout << " obs timer expired " << endl; 
 	   glbl_plr->observation_timeout(NULL);
 	   break;
 	case COMMAND:
 	   sleeptime = get_command_timeout(msg->args[0]);
           taskDelay(sleeptime * sysClkRateGet());
           cout << " cmd timer expired " << endl; 
 	   glbl_plr->command_timeout(NULL);
 	   break;
 	default:
 	   break;
 	}

}


void rti_timer(void *param, L2_rtapi *glbl_plr)
{
 int timerTask = taskSpawn("timer_monitor", 60, 0, 1000, 
                           (FUNCPTR) timer_monitor,
                           param,glbl_plr,0,0,0,0,0,0,0,0);
}
