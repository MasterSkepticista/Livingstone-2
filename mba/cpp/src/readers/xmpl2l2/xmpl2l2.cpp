/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: xmpl2l2.cpp,v 1.17 2004/01/28 23:31:53 lbrown Exp $ */

#include <livingstone/version.h>
#include <mba_utils/MBA_string.h>
#include <mba_utils/stringops.h>
#include <readers/livingstone_reader.h>
#include <readers/options.h>
#include <readers/to_bin.h>
#include <readers/to_l2.h>
#include <stdlib.h> // exit()

#define OPT_HELP        1
#define OPT_VERSION     2
#define OPT_VERBOSE     3
#define OPT_OUTPUT      4
#define OPT_OPTIMIZE    5
#define OPT_STRIP       6
#define OPT_BINARY      7
#define OPT_ASCII       8

static const Option options[] = {
    { 'h', "help",       false, OPT_HELP,  "print this help message and exit" },
    {  0 , "version",    false, OPT_VERSION,"print version number and exit" },
    { 'v', "verbose", true , OPT_VERBOSE,
        "print information when [r]eading, [o]ptimizing, [w]riting"},
    { 'o', "output-file",true,  OPT_OUTPUT,"specify output file [system.l2]" },
    { 'O', "optimize",   false, OPT_OPTIMIZE, "optimize model" },
    { 's', "strip",      false, OPT_STRIP,  "strip debugging symbols" },

    { 'b', "binary",     false, OPT_BINARY, "output to a binary file"},
    { 'a', "ascii",      false, OPT_ASCII,  "output to an ascii file"}
};

static void printUsage(OptionFinder& of) {
    _STD_ cerr << "Usage: xmpl2l2 [options] system\n"
      << "Converts system.xmpl, system.hrn, and system.ini into system.l2\n";
    of.printOptions(_STD_ cerr);
    exit(1);
}

int main(int /*argc*/, char **argv) {
    OptionFinder of(argv+1, options, sizeof(options)/sizeof(Option));
    MBA_string inname;
    MBA_string outname;

    bool use_optimizer= false;
    bool do_strip     = false;

    bool verbose_read = false;
    bool verbose_opt  = false;
    bool verbose_write= false;

    bool output_ascii = false;
    bool output_binary= false;

    int option;
    do {
        option = of.next_option();
        switch(option) {
            case OPT_ERROR:
                exit(1);
                break;

            case OPT_DONE:
                break; // NOT continue!

            case OPT_NOT_AN_OPTION:
                if(inname != "") {
                    _STD_ cerr << "xmpl2l2: multiple input files\n";
                    exit(1);
                }
                inname = of.optarg();
                break;

            case OPT_HELP:
                printUsage(of);
                break;

            case OPT_VERSION:
                _STD_ cout << "Livingstone version " << l2_version_string
			  << _STD_ endl;
                exit(0);
                break;

            case OPT_VERBOSE:
                {
                    const char *arg = of.optarg();
                    bool value = true ;
                    for(unsigned i=0; arg[i]; ++i) {
                        switch(arg[i]) {
                            case '-':
                                value = false;
                                break;
                            case '+':
                                value = true;
                                break;
                            case 'A':
                                verbose_read = value;
                                verbose_opt  = value;
                                verbose_write= value;
                                break;

                            case 'r':
                                verbose_read = value;
                                break;

                            case 'o':
                                verbose_opt  = value;
                                break;

                            case 'w':
                                verbose_write= value;
                                break;

                            // silently ignore default
                        }
                    }
                }
                break;

            case OPT_OUTPUT:
                outname = of.optarg();
                break;

            case OPT_ASCII:
                output_ascii = true;
                break;

            case OPT_BINARY:
                output_binary = true;
                break;

            case OPT_OPTIMIZE:
                use_optimizer = true;
                break;

            case OPT_STRIP:
                do_strip = true;
                break;

            default:
                assert(0);
        }
    } while(option != OPT_DONE);

    if(inname=="") {
        _STD_ cerr << "xmpl2l2: no input files\n";
        exit(1);
    }

    // if neither is set, output to ascii
    if(!output_binary) output_ascii = true;

    Livingstone_reader reader;
    reader.set_verbosity(verbose_read, verbose_opt);

    try {
        if(!reader.read_file(inname, ""))
            return 1;

        if(do_strip)
            if(!reader.strip())
                return 1;

        // optimize if requested
        if(use_optimizer)
            if(!reader.optimize())
                return 1;

        // sort the propositions
        reader.sort_props();

        MBA_string dir, file, ext;
        split_filename(inname, dir, file, ext);
        MBA_string inname_noext(dir + 
#ifdef WIN32
            "\\"
#else
            "/"
#endif
            + file);


        // write out in L2 format
        if(output_ascii) {
            MBA_string l2name;
            if(outname == "") {
                if(do_strip)
                    l2name = inname_noext+".l2s";
                else
                    l2name = inname_noext+".l2";
            }
            else
                l2name = outname;
            if(verbose_write)
                _STD_ cout << "Writing `" << l2name << "' ..." << _STD_ flush;
            // third arg is whether to use long or short section names;
            // we say short (false) if we stripped, long (true) if we didn't
            to_l2 writer(reader.get_file(), l2name, !do_strip);
            if(verbose_write) writer.setVerbose(true);
            bool ret = writer.write();
            if(verbose_write) _STD_ cout << "\n";
            if(!ret) return 1;
        }

        // write out in L2 binary format
        if(output_binary) {
            MBA_string l2name;
            if(outname == "")
                l2name = inname_noext+".l2bin";
            else
                l2name = outname;
            if(verbose_write)
                _STD_ cout << "Writing `" << l2name << "' ..." << _STD_ flush;
            to_bin writer(reader.get_file(), l2name);
            if(verbose_write) writer.setVerbose(true);
            bool ret = writer.write();
            if(verbose_write) _STD_ cout << "\n";
            if(!ret) return 1;
        }


    } catch(L2_error err) {
        _STD_ cerr << "\nxmpl2l2: Error: " << err.error_string << _STD_ endl;
        return 1;
    }
    return 0;
}
