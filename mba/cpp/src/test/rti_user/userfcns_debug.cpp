/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// userfcns_debug.cpp

#include <realtime_api/userfcns_debug.h>

void ReportTransitionsDebug::open() {
   scrstream.open("rtL2.scr");
   ReportTransitions::open();
}

void ReportTransitionsDebug::close() {
   scrstream.flush();
   scrstream.close();
   ReportTransitions::close();
}

ReporterInterface & ReportTransitionsDebug::operator()()
{
// translates to report full state at moment 
   ReportTransitions::operator()();

   return *this;
}

ReporterInterface & ReportTransitionsDebug::operator()(livingstone_message_calls fcn)
{
   switch(fcn) {
      case FIND_CANDIDATES:
	 scrstream << "fc\n";
	 break;
     // case REPORT_FULL_STATE:
//	 break;
      default:
	 break;
    }

    ReportTransitions::operator()(fcn);       

    return *this;
}

ReporterInterface & ReportTransitionsDebug::operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index)
{
   Assignable *var;
   T_system &t_system = thesystem->get_tracker()->get_t_system();

   switch(fcn) {
      case COMMAND:
	// first get string command and value
	var = t_system.get_command(var_index);
	 scrstream << "progress " << thesystem->get_var_name(*var) << "\n"; 
	 break;
      case OBSERVATION:
	 var = t_system.get_observable(var_index);
	 scrstream << "assign " << thesystem->get_var_name(*var) << "=" 
	 << thesystem->get_variable_value(*var, val_index) << "\n"; 
	 break;
      default:
	 scrstream << "unknown L2 reference\n";
	 break;
   } 

   ReportTransitions::operator()(fcn, var_index, val_index);

   return *this;
}

