/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// reporter.h -- interface class for reporting

#ifndef REPORTER_INTERFACE_H_
#define REPORTER_INTERFACE_H_

#include <realtime_api/livingstonemessage.h>
#include <livingstone/l2conf.h>

struct ReporterInterface
{
   virtual void open()=0;
   virtual void close()=0;

   virtual ReporterInterface & operator()()=0;
   virtual ReporterInterface & operator()(livingstone_message_calls fcn)=0;
   virtual ReporterInterface & operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index)=0;
   virtual ReporterInterface & operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index, unsigned int time_delay)=0;

};

#endif
