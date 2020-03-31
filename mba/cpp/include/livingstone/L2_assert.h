/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/**
 * This file contains an exception hierarchy, which is independent of the
 * stdexcept hierarchy. The maximal nodes are L2_fatal_error and L2_error.
 */

#ifndef L2_ASSERT_H_
#define L2_ASSERT_H_

#include <mba_utils/MBA_string.h>

/**
 * Thrown for things that should _never_ happen.
 * Deliberately not a subclass of L2_error.
 */

class L2_fatal_error {
 public:
  MBA_string error_string;
  L2_fatal_error(MBA_string err) : error_string(err) {};
};


/**
  * A general base class for errors.  We should throw subclasses instead.
  */

class L2_error {
 public:
  MBA_string error_string;
 protected:
  L2_error(MBA_string err) : error_string(err) {};
};


/**
 * Exception base class for model reading errors.
 */

class L2_reader_error : public L2_error {
 public:
  L2_reader_error(MBA_string err) : L2_error(err) {};
};


/**
 * Parse error while reading a model
 */

class L2_parse_model_error : public L2_reader_error {
    public:
        L2_parse_model_error(MBA_string err) : L2_reader_error(err) {};
};


/**
 * Parse error while reading a scenario.
 */

class L2_parse_scenario_error : public L2_reader_error {
 public:
  L2_parse_scenario_error(MBA_string err) : L2_reader_error(err) {};
};


/**
 * Parse error while reading a search params file.
 */

class L2_parse_params_error : public L2_reader_error {
    public:
        L2_parse_params_error(MBA_string err) : L2_reader_error(err) {};
};


/**
 * The model is empty.
 */

class L2_empty_model_error : public L2_reader_error {
 public:
  L2_empty_model_error(MBA_string err) :
    L2_reader_error("empty model: "+err) {};
};


/**
 * Exception class for model reading errors (in optimization).
 */

class L2_optimizer_error : public L2_reader_error {
 public:
  L2_optimizer_error(MBA_string err) : L2_reader_error(err) {};
};


/**
 * Errors while writing the L2_file to disk.
 */

class L2_writer_error : public L2_error {
 public:
  L2_writer_error(MBA_string err) : L2_error(err) {};
};


/**
 * Exception class specific to bad runtime function arguments.
 */

class L2_argument_error : public L2_error {
 public:
  L2_argument_error(MBA_string err) : L2_error(err) {};
};


/**
 * Initialization preconditions not satisfied.
 */

class L2_initialization_error : public L2_error {
 public:
  L2_initialization_error(MBA_string err) : L2_error(err) {};
};


/**
 * Exception class specialized to resource allocation errors.
 * Most likely, this won't be very useful: the MBA_string tries to allocate
 * memory, but this error is thrown when we're out of memory!
 */

class L2_resource_error : public L2_error {
 public:
  L2_resource_error(MBA_string err) : L2_error(err) {};
};


/**
 * Exception class for reporting duplicate frees (duf in Sun's bcheck).
 */

class L2_duf_error : public L2_error {
 public:
  L2_duf_error(MBA_string classname, void *ptr) :
    L2_error(classname + ": duplicate free on " + MBA_string(ptr)) { };
};


/**
 * An error with going out of bounds of an array or list.
 */

class L2_bounds_error : public L2_error {
public:
  L2_bounds_error(MBA_string description) : L2_error(description) { }

  L2_bounds_error(MBA_string arrayname, int index, int size) :
    L2_error("index " + MBA_string(index) +
	     " out of bounds in " + arrayname +
	     " of size " + MBA_string(size)) { }
};


/**
 * An error with not finding a value in something other than an array or list.
 */

class L2_not_found_error : public L2_error {
public:
  L2_not_found_error(MBA_string description) : L2_error(description) { }
};


/**
 * An error with trying to use some disabled functionality.
 */

class L2_disabled_error : public L2_error {
 public:
  L2_disabled_error(MBA_string function) :
    L2_error(function + " was disabled at compile-time") { }
};


/****************************************************************************
 Define the NORETURN macro
*****************************************************************************/

/**
 * From gcc 2.5 on, it is possible to tell the compiler that a function does
 * not return. This helps suppress spurious variable-not-initialized warnings.
 * VxWorks with the cygnus gcc 2.7.2 does not seem to know that abort() and
 * exit() are noreturn, so if your function doesn't return because of calls to
 * them, you get a spurious warning that a function returns although declared
 * noreturn.
 * Error handler functions for fatal errors don't return, in case you wondered.
 */

#  if !defined(VXWORKS) && (__GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 5))
#    define NORETURN __attribute__((noreturn))
#  else
#    define NORETURN
#  endif


/***************************************************************************
  Define the L2_TRY macro
**************************************************************************/

/**
 * This leaves try blocks as is when exceptions are enabled, but turns them
 * into ordinary blocks when disabled. This won't work for catch blocks, since
 * we don't want them executed at all when exceptions are disabled.
 */

#ifdef ENABLE_EXCEPTIONS
#define L2_TRY try
#else
#define L2_TRY /*no-exceptions*/
#endif

/***************************************************************************
  Define the L2_throw and L2_error macros.
 ***************************************************************************/

/**
 * L2_throw(xcp, args)
 *
 * xcp is the exception class.
 *
 * args is the list of arguments to the constructor of the exception.  It can
 * be empty (though currently no ctor allows that); but must include parens if
 * non-empty.
 *
 * Examples:
 *
 *      L2_throw( L2_resource_error , ("in Memory_pool") );
 *      L2_throw( L2_bounds_error , ("commands", i, size()) );
 *
 * The behaviour is to
 *      - if DISABLE_EXCEPTIONS, just print the error
 *      - if DISABLE_PRINT_ERROR, don't even do that -- just print file & line
 */

#if defined(ENABLE_EXCEPTIONS)

/**
 * Throw the exception.
 * The do-while(0) is the only portable way to make a block
 */

#define L2_throw(xcp, args)  \
        do { \
            xcp err##__LINE__ args; \
            throw (err##__LINE__ ); \
        } while(0)

#else /* that is, we didn't enable exceptions */

#if defined(ENABLE_PRINT_ERROR)

#include <stdlib.h>           // abort
#include <livingstone/L2_iostream.h>         // _STD_ cerr

/**
 * Don't throw, but do print the exception and abort.
 * The abort() and exit(1) calls were previously followed by while(1); to
 * assure that the body was noreturn; some compilers (e.g., gcc 3.0) don't
 * recognize that abort() and exit(1) are noreturn.
 * The do-while(0) is the only portable way to block.
 */

#ifdef ENABLE_ABORT
// Note: --enable-abort is not yet a configure option
#define L2_throw(xcp, args) \
        do { \
            xcp err##__LINE__ args; \
            _STD_ cerr<< #xcp ":\n" \
                << (err##__LINE__).error_string << _STD_ endl \
                << "Thrown at " \
                << __FILE__ << ':' << __LINE__ << _STD_ endl; \
            abort(); \
        } while(0)
#else
#define L2_throw(xcp, args) \
        do { \
            xcp err##__LINE__ args; \
            _STD_ cerr<< #xcp ":\n" \
                << (err##__LINE__).error_string << _STD_ endl \
                << "Thrown at " \
                << __FILE__ << ':' << __LINE__ << _STD_ endl; \
            exit(1); \
        } while(0)
#endif

#else  /* exceptions are disabled, and we don't print the error message */

/**
 * Define an error-handling function.
 * The default prints the exception name, as well as the source file and line
 * whence it was thrown. This saves a huge amount of code as compared to
 * creating the exception and printing the message -- something like 2kb per
 * L2_throw, or 30% of the overall size.
 */

/**
 * Macro L2_throw() is defined to be a call to this function when
 * ENABLE_EXCEPTIONS is undefined and ENABLE_PRINT_ERROR is undefined. It has
 * C linkage but is a C++ function.
 * \param xcp_name the name of the exception as a C string
 * \param filename the name of the source code file in which the throw occurs
 * \param linenum the line number of the throw within the source code file
 * \todo the NORETURN isn't working under VxWorks
 */
extern "C" void L2_error_function(const char *xcp_name, 
        const char *filename,
        unsigned linenum) NORETURN;

#define L2_throw(xcp, args) L2_error_function(#xcp, __FILE__, __LINE__)

#endif /* ENABLE_PRINT_ERROR */
#endif /* ENABLE_EXCEPTIONS */


/**
 * L2_assert(cond, xcp, args)
 *
 * Calls L2_throw if cond is false; in this way, it's analogous to stdlib
 * assert and should be used as such.
 *
 * See L2_throw for the xcp and args arguments ; cond is any expression
 * on which we can apply operator !
 * The do-while(0) is the only portable way to block
 */

#define L2_assert(cond, xcp, args) \
 do { if (! (cond)) { L2_throw(xcp, args); } } while(0)

#endif
