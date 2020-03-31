// time_delays.h -- contains timeouts for commands and observations
// ******************all times are in milliseconds!********************

#ifndef _TIME_DELAYS_H
#define _TIME_DELAYS_H

#include "./enumapi.h"

unsigned int get_command_timeout(unsigned int cmd)
{
    unsigned int timeout;
    switch(cmd)
    {
    case cbAndLed_cb15_cmdIn:
    case cbAndLed_cb14_cmdIn:
    case cbAndLed_cb13_cmdIn:
	timeout=5;
        break;
    case cbAndLed_cb12_cmdIn:
    case cbAndLed_cb11_cmdIn:
    case cbAndLed_cb9_cmdIn:
 	timeout=1;
        break;
    default:
        timeout = 2;
    }
    return timeout;
     
}

unsigned int get_observation_timeout(unsigned int obs)
{
    return 5;
}



#endif

