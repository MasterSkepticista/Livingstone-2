/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: api_gen.cpp,v 1.23 2004/01/28 23:31:45 lbrown Exp $ */

#include <stdlib.h>
#include <debuggers/livingstone_debug.h>
#include <debuggers/tracker_debug.h>
#include <livingstone/L2_fstream.h>
#include <livingstone/version.h>
#include <mba_utils/slist_iterator.h>

/** The file output stream for ./enumapi.h */
_STD_ ofstream fout("enumapi.h");


/**
 * Substitutes each occurrence of '.' with '_', since C++ identifiers can
 * not contain the '.' character.
 */

MBA_string convert_to_enum_name(const MBA_string& name) {
    const char *src = name.c_str();
    char * mystring = new char [name.size()+1];
    for(unsigned i=0; src[i] ; ++i) {
        if (src[i] == '.')
            mystring[i] = '_';
        else
            mystring[i] = src[i];
    }
    mystring[name.size()] = '\0';
    MBA_string ret(mystring);
    delete [] mystring;
    return ret;
}


/**
 * For each Assignable or Transitioned in the list, write to fout an enum
 * definition, each of the form
 * enum var_values {
 *      var_value0 = 0,
 *      var_value1 = 1
 * };
 */

void write_domain(Variable *var, T_system_debug &system_debug) {
    MBA_string obsname =
      convert_to_enum_name(system_debug.get_var_name(*var));
    // The first one:
    // enum <name>_values {
    //   <name>_<value> = 0
    fout << "enum "
	 << obsname
	 << "_values"
	 << " {\n    "
	 << obsname
	 << "_"
	 << system_debug.get_variable_value(*var, 0)
	 << "="
	 << 0;
    // Later ones have a preceding comma:
    // ,
    // <name>_<value> = <index>
    for (unsigned value = 1; value < var->get_nvalues(); ++value) {
      fout << ",\n    "
	   << obsname
	   << "_" 
	   << system_debug.get_variable_value(*var, value)
	   << "="
	   << value;
    }
    fout << "\n};\n";
}

void write_observable_domains(T_system& system, T_system_debug& system_debug) {
  // Descend for compatibility
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable =
      system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_observable()) {
      write_domain(pVariable, system_debug);
    }
  }
}


void write_command_domains(T_system &system, T_system_debug &system_debug) {
  // Descend for compatibility
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable =
      system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_command()) {
      write_domain(pVariable, system_debug);
    }
  }
}


void write_mode_domains(T_system &system, T_system_debug &system_debug) {
  // Descend for compatibility
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable =
      system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_transitioned()) {
      write_domain(pVariable, system_debug);
    }
  }
}


/**
 * Write out variable names. First get the variable pointer from the
 * conflict_db. Then get its name from the debugger and write it out.
 */

void write_name_header(const MBA_string& name) {
  // enum <name>_types {
  fout << "enum " << name << "_types {" << _STD_ endl;
}

void write_name(Variable *var, T_system_debug& dbg) {
  // <name> = <index>,
  fout << "    "
       << convert_to_enum_name(dbg.get_var_name(*var))
       << " = "
       << var->get_id()
       << ","
       << _STD_ endl;
}

void write_name_footer(const MBA_string& name, unsigned count) {
  // NUM_<name> = <count>
  fout << "    NUM_" << name << "_types=" << count << _STD_ endl << "};"
       << _STD_ endl;
}

void write_observable_names(T_system &system, T_system_debug& dbg) {
  write_name_header("observable");
  unsigned count = 0;
  // Descend for compatibility
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable =
      system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_observable()) {
      write_name(pVariable, dbg);
      count++;
    }
  }
  write_name_footer("observable", count);
}


void write_command_names(T_system &system, T_system_debug& dbg) {
  write_name_header("command");
  unsigned count = 0;
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable =
      system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_command()) {
      write_name(pVariable, dbg);
      count++;
    }
  }
  write_name_footer("command", count);
}


void write_mode_names(T_system &system, T_system_debug& dbg) {
  write_name_header("mode");
  unsigned count = 0;
  for (unsigned i = 0; i < system.npresent_variables(); i++) {
    Variable *pVariable
      = system.get_present_variable(system.npresent_variables() - i - 1);
    if (pVariable->is_transitioned()) {
      write_name(pVariable, dbg);
      count++;
    }
  }
  write_name_footer("mode", count);
}


/**
 * Now that the model is in memory, map over its assumptions and
 * assignables and spit out the commands, modes and observables into an
 * API file ./enumapi.h
 */

void writeToFile(const MBA_string& modelFilePathname,
		 T_system& sys, T_system_debug& dbg) {
  time_t t = time(0);
  fout<< "/*\n"
      << " * automatically generated on " << ctime(&t) /* ctime has \n */
      << " * from " << modelFilePathname << _STD_ endl
      << " * by api_gen version " << l2_version_string << _STD_ endl
      << " */\n\n";

  // Begin guard
  fout<< "#ifndef _ENUMAPI_H_\n"
      << "#define _ENUMAPI_H_\n";
  
  fout << "\n\n/*  CONSTANTS */\n"
       << "enum enumapi_constants {" << _STD_ endl
       << "  VARIABLE_COUNT = " << sys.npresent_variables() << _STD_ endl
       << "};" << _STD_ endl;

  fout << "\n\n/*  OBSERVATIONS */\n";
  write_observable_names(sys, dbg);

  fout << "\n\n/*  OBSERVATION TYPES */\n";
  write_observable_domains(sys, dbg);

  fout << "\n\n/*  COMMANDS */\n";
  write_command_names(sys, dbg);

  fout << "\n\n/*  COMMAND TYPES */\n";
  write_command_domains(sys, dbg);

  fout << "\n\n/*  MODES */\n";
  write_mode_names(sys, dbg);

  fout << "\n\n/*  MODE TYPES */\n";
  write_mode_domains(sys, dbg);

  // End guard
  fout << "\n#endif\n";
}


/**
 * argv[1] is the pathname of an XMPL model file (without the file extension).
 * Write to ./enumapi.h enums for each command, mode and observation, and
 * for their domains.
 */

int main(int argc,char** argv) {
  // Validate command-line arguments
  if (argc != 2)  {
    _STD_ cout << "Usage: " << _STD_ endl;
    _STD_ cout << "       " << argv[0] << " <model-name>" << _STD_ endl;
    return(-1);
  } else {
    // Read in the model
    _STD_ cout << "Reading " << argv[1] << _STD_ endl;
    Livingstone_debug pack;
    pack.set_filename(argv[1]);
    pack.read_file();
    pack.create_tracker();
    pack.create_debugger();
    pack.initialize_tracker();
    writeToFile(argv[1],
		pack.get_tracker()->get_t_system(),
		*pack.get_debugger());
    return 0;
  }
}
