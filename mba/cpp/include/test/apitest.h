/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// apitest.h
// file defining test function which describes sequence of commands to run through
// realtime livingstone api
#include <test/enumapi.h>
#include <realtime_api/livapi_rt.h>

void apitest(LivingstoneRealTimeAPI  & api)
{

	api.queue_command(cbAndLed_cb15_cmdIn, cbAndLed_cb15_cmdIn_on);
	api.queue_command(cbAndLed_cb14_cmdIn, cbAndLed_cb14_cmdIn_on);
	api.queue_command(cbAndLed_cb12_cmdIn, cbAndLed_cb12_cmdIn_on);
	api.queue_command(cbAndLed_cb11_cmdIn, cbAndLed_cb11_cmdIn_on);
	api.queue_start_command_and_time(cbAndLed_cb5_cmdIn, cbAndLed_cb5_cmdIn_on);

	// maybe should change to queue_observations_and_report and queue_start_observations
	api.queue_observation(cbAndLed_led5_ledState, cbAndLed_led5_ledState_on);
	api.queue_observation(cbAndLed_led6_ledState, cbAndLed_led6_ledState_off);
	api.queue_observation(cbAndLed_led7_ledState, cbAndLed_led7_ledState_off);
	api.queue_observation(cbAndLed_led8_ledState, cbAndLed_led8_ledState_off);

	SleepEx(2000, TRUE);

	api.queue_report_full_state();


}
