/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: options.h,v 1.10 2004/01/28 23:31:40 lbrown Exp $ */
#ifndef options_HEADER
#define options_HEADER

#include <livingstone/L2_iostream.h>
#include <livingstone/l2conf.h>

/**
 * Returned by next_option() if the next Option doesn't start with a hyphen.
 * \todo Make this and the others an enum.
 */
#define OPT_NOT_AN_OPTION 0

/**
 * Returned by next_option() if the next Option either
 *  (1) isn't recognized; or
 *  (2) needs an argument but doesn't have one
 */
#define OPT_ERROR -2

/// Returned by next_option if we're out of Options.
#define OPT_DONE -1


/**
 * Representation of a command-line option that the program understands. The
 * client creates an array of these and passes it to the OptionFinder
 * constructor.
 */

struct Option {
  /// Match -s
  char shortname;
  /// Match --long
  const char *longname;
  /// require an argument (-opt=arg)
  bool has_argument;
  /// Unique ID; for a switch statement.
  int id;
  /// Printed by the usage statement
  const char *documentation;
};

/**
 * Class to parse command-line options in argv without using getopt().

   Example of setting up your options:
<code><pre>
const Option options[] = {
   { 'h', "help",    false, OPT_HELP,    "print this help message and exit" },
   { 'v', "verbose", false, OPT_VERBOSE, "print gobs of info when parsing"},
   {  0 , "history", true,  OPT_HISTORY, "max history to hold; 0 is infinite"},
   {  0 , "max-rank",true,  OPT_MAX_RANK,"maximum-rank candidate to consider"},
   {  0 , "cbfs",    false, OPT_CBFS,    "unimplemented" },
   {  0 , "cover",   false, OPT_COVER,   "unimplemented" }
};
unsigned noptions = sizeof(options)/sizeof(Option);
</pre></code>

Then, from main, do:
<code><pre>
    OptionFinder of(argv+1, options, noptions);
    int option;
    do {
        switch(option = of.next_option()) {
        case OPT_VERBOSE: verbose=true; break;
        case OPT_ERROR:   exit(1);
        }
    } while(option != OPT_DONE);
</pre></code>
*/

class OptionFinder {
 private:
  /// Array of Option objects.
  const Option *options;

  /// The number of Options.
  unsigned noptions;

  /// The command line argv from main().
  const char * const *argv;
  
  /// This is where next_option() will put the argument to an Option.
  const char *option_argument;

 public:
  /**
   * The client supplies argv, the command line to be parsed for options, an
   * array of Option objects describing the accepted options, and the length
   * of the array.
   */
  OptionFinder(const char* const* commandLine,
	       const Option* optionArray,
	       unsigned optionCount);
  
  /**
   * Read argv[0] and return the ID it matches; set option_argument if the
   * Option requires an argument.
   * If argv[0] isn't an Option (doesn't start with '-' or '--'), return
   * OPT_NOT_AN_OPTION and set option_argument; if it's an Option but
   * something's wrong with it, print an error and return OPT_ERROR.
   */
  int next_option();
  
  /// Return the latest option's argument string, if any.
  const char *optarg() const { return option_argument; }

  /// Print all the Options to the supplied ostream.
  void printOptions(_STD_ ostream& os);

  /// Print all the Options to _STD_ cout.
  void printOptions(); // to _STD_ cout

 private:
  /// Search for the Option with the specified long name string.
  const Option *find_long_option(const char*);

  /// Search for the Option with the specified short name char.
  const Option *find_short_option(char);
};

#endif
