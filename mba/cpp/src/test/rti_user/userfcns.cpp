/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// userfcns.cpp

#include <realtime_api/userfcns.h>

ReportTransitions::ReportTransitions( Tracker_debug *system) : thesystem(system){}

ReporterInterface & ReportTransitions::operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index,
		unsigned int time_delay) 
{
	Assignable *var;
	T_system &t_system = thesystem->get_tracker()->get_t_system();

	switch (fcn) {
	case COMMAND:
	var = t_system.get_command(var_index);
	logstream << "Timeout of " << time_delay << " s begun for command: "
	 << thesystem->get_var_name(*var) << "\n";
		break;
	case OBSERVATION:
	var = t_system.get_observable(var_index);
	logstream << "Timeout of " << time_delay << " s begun for observation: "
	<< thesystem->get_var_name(*var) << "with value of: " 
	<< thesystem->get_variable_value(*var, val_index) << "\n";
		break;
	default:
		break;
	}
	return *this;
}

/// just output to specified stream for now
ReporterInterface & ReportTransitions::operator()(livingstone_message_calls fcn, unsigned int var_index, unsigned int val_index) {

    T_system &t_system = thesystem->get_tracker()->get_t_system();
    Assignable *var;

    switch (fcn) {
	case COMMAND:
	    logstream << "Command issued: " << 
                thesystem->get_var_name(*t_system.get_command(var_index))<< "\n";
	    break;
	case OBSERVATION:
            logstream << "Observation recorded: " << 
            thesystem->get_var_name(*t_system.get_observable(var_index)) 
            << "=" << thesystem->get_variable_value(*t_system.get_observable(var_index), val_index) << "\n"; 
	    break;
	default:
	    break;
	}
	return *this;
}

ReporterInterface & ReportTransitions::operator() (livingstone_message_calls fcn) {
	switch (fcn)
	{
	case FIND_CANDIDATES: // output candidates
		logstream << "Candidates are: ";
	        thesystem->print_candidates(logstream);	
		break;
	default:
		break;
	}
	return *this;
}

ReporterInterface & ReportTransitions::operator() () {
		// output full state -- considered to be all variables and their values

	logstream << "\n\nCurrent state is: \n";
	thesystem->print_all_variable_values(logstream);
	
	return *this;

}


void ReportTransitions::open() {logstream.open("rtapi.log");}
void ReportTransitions::close() {logstream.flush(); logstream.close();}


