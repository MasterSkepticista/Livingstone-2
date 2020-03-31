/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// userfcns_debug.h -- debug versions of ouput objects

#ifndef USERFCNS_DEBUG_H_
#define USERFCNS_DEBUG_H_

#include <livingstone/l2conf.h>

#include <realtime_api/userfcns.h>
#include <fstream.h>

// this debug class wraps the regular reporting functions and outputs
// an script to be fed into L2
struct ReportTransitionsDebug : public ReportTransitions
{
public:
   ReportTransitionsDebug(Tracker_debug *system) : ReportTransitions(system)
   {
   }

   ReporterInterface & operator()();
   ReporterInterface & operator()(livingstone_message_calls fcn);
   ReporterInterface & operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index);

   void open();
   void close(); 

   ofstream scrstream;

};

#endif
