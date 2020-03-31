/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// L2_queue.h
/// L2 queue interface definition -- basically declares functions called 
/// from other RTAPI objects.

#ifndef L2_QUEUE_H_
#define L2_QUEUE_H_

#include <realtime_api/livingstonemessage.h>

class L2_queue  
{
public:
	virtual void add_message_to_queue(LivingstoneMessage *msg)=0;
	virtual void get_message_from_queue(LivingstoneMessage *msg)=0;

	virtual void is_empty()=0;
	virtual void is_done_empty()=0;

	virtual void increment_timeouts()=0;
	virtual void decrement_timeouts()=0;

};
	 
#endif
