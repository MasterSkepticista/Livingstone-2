/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: stringops.cpp,v 1.4 2001/11/16 19:32:32 lbrown Exp $ */
#include <mba_utils/stringops.h>

#include <mba_utils/MBA_string.h>
#include <ctype.h>


/***************************************************************************
  Split a filename.
 ***************************************************************************/
void split_filename(const MBA_string& file,
        MBA_string& path,
        MBA_string& base,
        MBA_string& ext) {
    unsigned len = file.size();
    int i;
    int found_dot = -1;
    int found_slash = -1;
    for(i=len-1; i>=0; --i) {
        if(found_dot<0 && file[i] == '.') {
            found_dot = i;
        }
        if(file[i] == '/' || file[i]=='\\') {
            found_slash = i;
            break; // don't look for dots before the slash
        }
    }

    unsigned end_of_base;
    if(found_dot < 0) {
        end_of_base = file.size();
        ext = "";
    } else {
        end_of_base = found_dot;
        ext = MBA_string(file.c_str()+found_dot+1);
    }

    unsigned start_of_base;
    if(found_slash < 0) {
        start_of_base = 0;
        path = ".";
    } else {
        start_of_base = found_slash+1;

        // fix the slashes into whatever the OS wants
        char buffer[256];
        assert(found_slash < 256);
        memcpy(buffer, file.c_str(), found_slash);
        buffer[found_slash] = '\0';
        for(int i=0; i<found_slash; ++i) {
            if(buffer[i]=='/' || buffer[i]=='\\') {
#ifdef WIN32
                buffer[i] = '\\';
#else
                buffer[i] = '/';
#endif
            }
        }
        path = MBA_string(buffer);
    }

    base = MBA_string(file.c_str()+start_of_base, end_of_base - start_of_base);
}




/***************************************************************************
  Split an assignment.
 ***************************************************************************/

bool split_equality(const MBA_string& eq_str,
        MBA_string& varname,
        MBA_string& valuename)
{
    const char *str = eq_str.c_str();
    const char *namep;

    /////// Find the variable name
    // skip leading whitespace
    while(*str && isspace(*str)) ++str;

    // that's the start of the variable name
    namep = str;

    // skip until an equal sign or whitespace
    while(*str && *str!='=' && !isspace(*str)) ++str;
    if(!*str) {
        varname = namep;
        valuename = "";
        return false;
    }

    // end of variable name; create the string
    // abc=d  ; namep=0, str=3, str-namep=3, strlen("abc") = 3
    varname = MBA_string(namep, (str-namep));

    /////// Find the value name
    // skip until equal sign
    while(*str && *str!='=') ++str;
    if(!*str) {
        valuename = "";
        return false;
    }

    // skip equal sign, then whitespace
    ++str;
    while(*str && isspace(*str)) ++str;

    // that's the start of the value name
    namep = str;

    // skip until whitespace or end of string
    while(*str && !isspace(*str)) ++str;
    // don't assert; either is ok ; note this allows garbage at the end.

    // abc=d  ; namep=4, str=5, str-namep=1, strlen("d")=1
    valuename = MBA_string(namep, (str-namep));

    return true;
}
