/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <fcntl.h>        // for function open()
#include <readers/from_bin.h>
#include <readers/from_memory.h>  // for class from_memory

/* Do everything in the initializers */

from_bin::from_bin(L2_file* f, MBA_string fname) : 
  L2_file_reader(f),
  modelFilePathname(fname.c_str()),
  modelSize(0),
  theModel(0) {
}


/* Free the array containing the model that was read in from the file */

from_bin::~from_bin() {
  L2_free_array(theModel, modelSize);
}


/* Read the model from file to memory, then delegate reading to a
   from_memory object. This is the main method */

bool from_bin::read() {
  if (0 == readIntoMemory(modelFilePathname)) {
    return false;
  } else {
    // L2_file_reader::dest() returns the L2_file
    from_memory fm(dest(), theModel, modelSize);
    return fm.read();
  }
}


/** Return the number of bytes in the opened file. If the size could not be
    determined, return 0. Leave the file pointer where it was.
    This is a non-member function so that from_bin.h need not know FILE*
*/

long fileSize(FILE* f) {
  int result = 0;
  fpos_t startPosition;
  if ((0 != f)                          &&        // Non-null FILE*
      (0 == fgetpos(f, &startPosition)) &&        // Found start position
      (0 == fseek(f, 0, SEEK_END))) {             // Moved to the end
    result = ftell(f);
    fsetpos(f, &startPosition);                   // Returned to start position
  }
  return result;
}

/** Read modelSize, allocate theModel, and read the contents of the file
    into theModel. Return the number of bytes read successfully.
*/

size_t from_bin::readIntoMemory(const char * const pathname) {
  size_t result = 0;
  FILE* f = fopen(pathname, "rb");
  if (f != 0) {                    // File opened
    modelSize = fileSize(f);
    if (modelSize > 0) {           // Model size found
      theModel =
	L2_alloc_array(unsigned char, static_cast<unsigned>(modelSize));
      if (theModel != 0) {         // Model allocated
        if (static_cast<size_t>(fread(theModel,
				      sizeof(unsigned char),
				      static_cast<size_t>(modelSize),
				      f)) ==
	    static_cast<size_t>(modelSize)) {// All bytes read in from file
	  result = static_cast<size_t>(modelSize);
	}
      }
    }
    fclose(f);
  }
  return result;
}
