/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livapi_rt.cpp


#include <realtime_api/vxworks/livapi_rt.h>

#include "stdlib.h" 
#include <realtime_api/livingstonemessage.h>
#include <realtime_api/vxworks/queues.h>
#include <livingstone/L2_assert.h> 
#include <unistd.h>



extern void rti_timer(void *param, L2_rtapi *glbl_plr);
extern LivingstoneMessage xlate(NDAS_DATA monitorMsg);

void rt_input(LivingstoneMessage msg)
{

   switch(msg.fcn_name) {
      case COMMAND:
              glbl_plr->queue_command(msg.args[0],msg.args[1]);
              break; 
      case OBSERVATION:
              glbl_plr->queue_observation(msg.args[0],msg.args[1]);
              break;
      case START_COMMAND_AND_TIME:
              glbl_plr->queue_start_command_and_time(msg.args[0],msg.args[1]);
              break;
      case START_OBSERVE_AND_TIME:
              glbl_plr->queue_observations_and_time(msg.args[0],msg.args[1]);
      case FIND_CANDIDATES:
              glbl_plr->queue_find_candidates();
              break;
      case ABORT_COMMAND:
              break;
      case REPORT_FULL_STATE:
              glbl_plr->queue_report_full_state();
              break;
   }
}


void L2_rtapi::processMonitorData()
{

   while (1) {
     // cout << "real-time interface waiting for input from monitors." << endl;

       // first, wait to retrieve a message from the queue
       NDAS_DATA *monitorMsg = (NDAS_DATA *)malloc(sizeof(NDAS_DATA));
       readMsgQ(MQIDTI_LIV, monitorMsg, -1);  

       // NDAS_DATA monMsg = *((NDAS_DATA *) get_message_from_queue(MQIDTI_LIV));
       cout << "  RealTime got msg " << monitorMsg->msgType << ", " <<
	 monitorMsg->arg1 << ", " << monitorMsg->arg2 << endl;

       LivingstoneMessage L2msg = xlate(*monitorMsg);

       // now parse the message and invoke
       rt_input(L2msg);

   }

}


L2_rtapi::L2_rtapi() 
{
   glbl_plr = this;

   set_timeout_handlers();  
}


L2_rtapi::~L2_rtapi()
{

}


// adds command and its assignment to the message queue
void L2_rtapi::queue_command(unsigned int cmd_index, 
			     unsigned int cmd_value)
{
   LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg), MSG_PRI_NORMAL);
}

// adds monitor observed values to the message queue for assignment
void L2_rtapi::queue_observation(unsigned int obs_index, 
				 unsigned int value_index)
{
   LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg),
			 MSG_PRI_NORMAL);
}

void L2_rtapi::queue_find_candidates()
{
   LivingstoneMessage *msg = new LivingstoneMessage(FIND_CANDIDATES);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg),
			 MSG_PRI_NORMAL);
}

void L2_rtapi::queue_start_command_and_time(unsigned int cmd_index, unsigned int cmd_value)
{
   LivingstoneMessage *msg = new LivingstoneMessage(COMMAND, cmd_index, cmd_value);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

   // start timeout task
   rti_timer (msg, glbl_plr);

   //L2_assert(!err, L2_error("Error while attempting to create command timer pthread."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg),
			 MSG_PRI_NORMAL);
}

void L2_rtapi::queue_observations_and_time(unsigned int obs_index,
	                                   unsigned int value_index)
{
   LivingstoneMessage *msg = new LivingstoneMessage(OBSERVATION, obs_index, value_index);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

// start timeout task
   rti_timer (msg, glbl_plr);

   //   L2_assert(!err, L2_error("Error while attempting to create observation timer."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg),
			 MSG_PRI_NORMAL);
}

void L2_rtapi::queue_abort_command()
{
}

void L2_rtapi::queue_report_full_state()
{
   LivingstoneMessage *msg = new LivingstoneMessage(REPORT_FULL_STATE);
   L2_assert(msg, L2_error("Out of memory for LivingstoneMessages."));

   int status = sendMsgQ(MQIDTI_DIS, (void *) msg, sizeof(*msg), MSG_PRI_NORMAL);
}

void L2_rtapi::set_timeout_handlers( void*(*cmd)(void *), void *(*obs)(void *) )
{
   command_timeout = cmd;
   observation_timeout = obs; 
}

