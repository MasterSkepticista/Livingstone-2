/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: stringops.h,v 1.2 2001/11/16 19:32:11 lbrown Exp $ */
#ifndef stringops_HEADER
#define stringops_HEADER

class MBA_string;

/**
 * Split a filename into its components:
 * path / name . ext
 * '/' and '\' are seen as identical ; on Win32, they are all 
 * translated to '\' ; on others, to '/'
 */
extern void split_filename(const MBA_string& file,
        MBA_string& path,
        MBA_string& base,
        MBA_string& ext);

/**
 * Split an equality into its components:
 * var = value
 */
extern bool split_equality(const MBA_string& eq,
        MBA_string& var,
        MBA_string& value);


#endif
