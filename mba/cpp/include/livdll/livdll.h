/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIVDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIVDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef LIVDLL_H_
#define LIVDLL_H_

#ifdef WIN32

#ifdef STATIC_LIV
#define LIVDLL_API
#define EXPIMP_TEMPLATE
#endif

#ifdef LIVDLL_EXPORTS
#define LIVDLL_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#endif
#ifdef LIVDLL_IMPORTS
#define LIVDLL_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#else
#define LIVDLL_API

#endif

#endif


