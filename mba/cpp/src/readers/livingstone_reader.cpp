/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone_reader.cpp,v 1.26 2004/02/05 22:36:20 lbrown Exp $ */
#include <readers/livingstone_reader.h>

// in-memory representation of a file
#include <readers/L2_file.h>
// required readers
#include <readers/from_any.h>
#include <readers/from_memory.h>

// optimizers
#ifndef DISABLE_OPTIMIZER
#  include <readers/xlate_merge_eq_vars.h>
#  include <readers/xlate_strip.h>
#  include <readers/xlate_remove_duplicates.h>
#  include <readers/xlate_sort_props.h>
#endif

// Verbose output.  We need to check outside whether to actually do it.
// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#  define verbose(expr) do { expr; } while(0)
#else
#  define verbose(expr)
#endif

Livingstone_reader::Livingstone_reader() :
  out(&_STD_ cout), verbose_reader_(false), verbose_optimizer_(false),
  use_optimizer_(false) { }


Livingstone_reader::~Livingstone_reader() {
  for (Slist<L2_file*>::iterator it=l2files.begin();
       it != l2files.end(); ++it ) {
    L2_file *l2File = *it;
    delete l2File;
  }
  l2files.erase(); // unnecessary
}


bool Livingstone_reader::read_model_array(const unsigned char *modelArray,
					  size_t modelArrayLength) {
  L2_TRY {
    // Precondition: There must be a model array
    L2_assert((modelArray != 0 && modelArrayLength > 0),
	      L2_argument_error,
	      ("no model file"));
    // There must be no L2_file
    L2_assert(l2files.empty(),
	      L2_argument_error,
	      ("reader has already read a model"));
    l2files.push_front(new L2_file());
    from_memory reader(l2files.front(), modelArray, modelArrayLength);
    if (verbose_reader_) {
      reader.setVerbose(true);
      verbose(get_output() << "Reading from array..." << _STD_ endl);
    }
    return reader.read();
  }
#ifdef ENABLE_EXCEPTIONS
  catch(L2_error err) {
    _STD_ cerr << "l2test: read error: " << err.error_string << _STD_ endl;
    return false;
  }
  // any other exception sails through
#endif
}


#ifndef DISABLE_MODEL_FILES

bool Livingstone_reader::read_file(MBA_string filename, MBA_string filetype) {
  L2_TRY {
    L2_assert(l2files.empty(),
	      L2_argument_error,
	      ("reader has already read a file"));
    L2_file *l2 = new L2_file();
    l2files.push_front(l2);
    from_any reader(l2, filename, filetype);
    if (verbose_reader_) {
      reader.setVerbose(true);
      verbose(get_output() << "Reading from " << filename << "...\n");
    }
    return reader.read();
  }
#ifdef ENABLE_EXCEPTIONS
  catch(L2_error err) {
    _STD_ cerr << "l2test: read error: " << err.error_string << _STD_ endl;
    return false;
  }
#endif
  // any other exception sails through
}
#endif


/***************************************************************************
        Optimization, piece-meal.
 ***************************************************************************/
#ifndef DISABLE_OPTIMIZER

// Common code to all instances of the template, plus allowing it
// access without befriending it.

bool Livingstone_reader::apply_translator(L2_file_translator* x)
{
  L2_TRY {
    L2_assert(get_file(),
	      L2_initialization_error,
	      ("no model to optimize; read one first"));
    x->set_source(get_file());
    
    L2_file *dest = new L2_file();
    l2files.push_front(dest);
    x->set_dest(dest);
    if (verbose_optimizer_) {
      x->setVerbose(true);
      x->set_output(*out);
      verbose(get_output() << "Optimizing model ...\n");
    }
    return x->translate();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (const L2_error& err) {
    _STD_ cerr<< "l2: optimization error: " << err.error_string << _STD_ endl;
    return false;
  }
#endif
}


// Merge variables that are always equal into a single variable.
bool Livingstone_reader::merge_eq_vars() {
  xlate_merge_eq_vars x;
  return apply_translator(&x);
}


// Merge variables that are always equal into a single variable.
// Return data used to get the names of the lost Variables
bool Livingstone_reader::merge_eq_vars(Hash_table<MBA_string, MBA_string>&
				       variableHashMap,
				       Array<MBA_string, true>&
				       optimizedAwayVariableNames) {
  xlate_merge_eq_vars x;
  bool result = apply_translator(&x);
  x.getOptimizedAwayVariableNames(optimizedAwayVariableNames);
  x.getVariableHashMap(variableHashMap);
  return result;
}


// Remove debugging information.
bool Livingstone_reader::strip() {
  xlate_strip x;
  return apply_translator(&x);
}


// Remove duplicate props and clauses.
bool Livingstone_reader::remove_duplicates() {
  xlate_remove_duplicates x;
  return apply_translator(&x);
}


// Sort propositions in the order to_l2 and to_bin want it.
bool Livingstone_reader::sort_props() {
  xlate_sort_props x;
  return apply_translator(&x);
}

#endif

