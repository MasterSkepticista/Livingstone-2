/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// livingstonemessage.h

#ifndef _LIVINGSTONEMESSAGE_H
#define _LIVINGSTONEMESSAGE_H
#include <livingstone/l2conf.h>

enum livingstone_message_calls {COMMAND, OBSERVATION, START_COMMAND_AND_TIME, 
	START_OBSERVE_AND_TIME, FIND_CANDIDATES, ABORT_COMMAND, 
	REPORT_FULL_STATE };

struct LivingstoneMessage 
{
	LivingstoneMessage(livingstone_message_calls fcn, unsigned int arg1=0, unsigned int 
		arg2=0, unsigned int arg3=0) : fcn_name(fcn) 
	{
		args[0] = arg1;
		args[1] = arg2;
		args[2] = arg3;
	}

	LivingstoneMessage() {}

	 livingstone_message_calls fcn_name;
	 unsigned int args[3]; 

	 // need to add memory pooling

	 LivingstoneMessage & operator=(LivingstoneMessage &mother)
	 {
		 fcn_name = mother.fcn_name;
		 for (int i=0; i<3; i++)
			 args[i] = mother.args[i];
		return *this;
	 }

};

#endif
