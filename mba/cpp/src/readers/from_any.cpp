/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_any.cpp,v 1.18 2004/01/28 23:31:48 lbrown Exp $ */

#include <livingstone/L2_iostream.h>
#include <readers/from_any.h>

#ifdef ENABLE_L2_FORMAT
#  include <readers/from_l2.h>
#endif

#ifdef ENABLE_XMPL_FORMAT
#  include <readers/from_xmpl.h>
#endif

#ifdef ENABLE_L2BIN_FORMAT
#  include <readers/from_bin.h>
#endif

#include <livingstone/L2_assert.h>

// This macro executes the C statement bound to "stream" once if both
// ENABLE_L2_VERBOSE is defined and isVerbose() returns true
// The do-while(0) is the only portable way to block.

#ifdef ENABLE_L2_VERBOSE
#  define verbose(stream) do { if (isVerbose()) { stream; } } while(0)
#else
#  define verbose(stream)
#endif


/***************************************************************************
  dictionary functions
 ***************************************************************************/

from_any::Type_item from_any::extension_dict[] = {
  { ".l2",    l2 },
  { ".l2s",   l2 },  // stripped l2
  { ".l2bin", l2bin },
  { ".bin",   l2bin },
  { ".xmpl",  xmpl },
  { ".hrn",   xmpl },
  { ".ini",   xmpl },
  { "",       unknown } // must be last
};

from_any::Type_item from_any::format_dict[] = {
  { "l2",    l2 },
  { "l2s",   l2 }, // stripped l2
  { "xmpl",  xmpl },
  { "l2bin", l2bin },
  { "bin",   l2bin },
  { "",      unknown } // must be last
};


from_any::FileType from_any::discover_file_type(const MBA_string& filename,
						const MBA_string& format) {
  if (format != "") {
    // discover from the format given
    return search_dict_by_key(format_dict, unknown, format.c_str());
  } else {
    // discover from file extension
    const char *lastdot = strrchr(filename.c_str(), '.');
    MBA_string extension;

    // if the basename of the file has a dot, the last
    // one starts the extension ; else stick with ""
    if (lastdot && lastdot > strrchr(filename.c_str(), '/')) {
      extension = lastdot;
    }

    // search for the extension
    FileType type = search_dict_by_key(extension_dict, unknown,
				       extension.c_str());

    // unknown (or no) extension => xmpl 
    if (type == unknown) {
      type = xmpl;
    }
    return type;
  }
}


/***************************************************************************
  constructor
 ***************************************************************************/

// Constructor with format name; must look up format object

from_any::from_any(L2_file *dest,
		   MBA_string filename,
		   MBA_string format_name) :
  L2_file_reader(dest), file_(filename),
  format_(discover_file_type(filename, format_name)) {
}


// Constructor with explicit format object

from_any::from_any(L2_file *dest,
		   MBA_string filename,
		   FileType format) :
  L2_file_reader(dest), file_(filename), format_(format) {
}


// Call the (XMPL, L2, L2bin) reader's read() method and return what it returns

bool from_any::doRead(L2_file_reader& reader) {
  L2_assert(&reader,
	    L2_fatal_error,
	    ("didn't create the reader!"));
  bool retval = false; // false if exception is thrown
  verbose(_STD_ cout << "Reading " << file_ << _STD_ endl);
  verbose(reader.setVerbose(isVerbose()));
  L2_TRY {
    retval = reader.read();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error err) {
    _STD_ cerr << "Reader error:\n" << err.error_string << _STD_ endl;
  }
#endif
  return retval;
}


// Read from XMPL  file from_any::file_ to dest()

bool from_any::readXMPL() {
  verbose(_STD_ cout << "Loading " << file_ << " as XMPL\n");
#ifdef ENABLE_XMPL_FORMAT
  from_xmpl reader(dest(), file_);
  return doRead(reader);
#else
  _STD_ cerr << "Decided on XMPL format but it's disabled!\n";
  return false;
#endif
}


// Read from l2 file from_any::file_ to dest()

bool from_any::readL2() {
  verbose(_STD_ cout << "Loading " << file_ << " as l2\n");
#ifdef ENABLE_L2_FORMAT
  from_l2 reader(dest(), file_);
  return doRead(reader);
#else
  _STD_ cerr << "Decided on l2 format but it's disabled!\n";
  return false;
#endif
}


// Read from l2bin file from_any_::file_ to dest()

bool from_any::readL2bin() {
  verbose(_STD_ cout << "Loading " << file_ << " as binary l2\n");
#ifdef ENABLE_L2BIN_FORMAT
  from_bin reader(dest(), file_);
  return doRead(reader);
#else
  _STD_ cerr << "Decided on l2bin format but it's disabled!\n";
  return false;
#endif
}


// Dispatch on from_any::format_

bool from_any::read() {
  switch(format_) {
    case xmpl:  return readXMPL();
    case l2:    return readL2();
    case l2bin: return readL2bin();
    case unknown:
      L2_throw(L2_fatal_error,
	       ("shouldn't get here at " + MBA_string(__FILE__) + ":" +
		MBA_string(__LINE__)));
  }
  return false;
}
