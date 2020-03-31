/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// userfcns.h

/// user-supplied definitions for Livingstone
/// defines the ReportTransitions function object for now

/// could divide into ReportTransitions, ReportObservations, ReportState, ReportCandidates
/// functionally equivalent, but probably more clear to do so

/// user derives their function object from these to override
/// rename this file 'rtapi_report.h' and include 'userfcns.h'

#ifndef USERFCNS_H_
#define USERFCNS_H_

#include <livingstone/l2conf.h>

#include <fstream.h>
#include <debuggers/tracker_debug.h>
#include <realtime_api/reporter.h>

struct ReportTransitions : public ReporterInterface
{
public:
	ReportTransitions( Tracker *system);


	ReporterInterface & operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index,
		unsigned int time_delay); 

	// just output to specified stream for now
	ReporterInterface & operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index); 

	ReporterInterface & operator() (livingstone_message_calls fcn); 

	ReporterInterface & operator() (); 

	ofstream  logstream ;
	Tracker_debug * thesystem;

	void open();
	void close(); 
};

#endif
