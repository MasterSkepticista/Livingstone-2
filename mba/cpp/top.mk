SHELL = /bin/sh

############### top.mk #################
# Global settings for all makefiles.
# This is prepended to each makefile before Makefile.in
# Similarly, bot.mk is appended.

# See the file "mba/disclaimers-and-notices-L2.txt" for
# information on usage and redistribution of this file,
# and for a DISCLAIMER OF ALL WARRANTIES.

# This file mostly contains variable assignments.

# $Id: top.mk,v 1.16 2001/11/18 23:28:20 taylor Exp $


#### make sure this is the first rule
default: all

.PHONY: default

########################################
#### Grab the settings from autoconf

@SET_MAKE@

# where to install stuff
prefix      = @prefix@
exec_prefix = @exec_prefix@
bindir      = @bindir@
libdir      = @libdir@
includedir  = @includedir@
datadir     = @datadir@
infodir     = @infodir@

# where the source code and local libraries are:
top_srcdir	= @top_srcdir@
loc_incldir	= ${top_srcdir}/include
loc_libdir	= ${top_srcdir}/lib

## external libraries
LIBS = @LIBS@

# programs
CC	= @CC@
CPP	= @CPP@
CXX	= @CXX@
CXXCPP	= @CXXCPP@
INSTALL	= @INSTALL@
LN_S	= @LN_S@
PURIFY  = @PURIFY@

# flags used in linking
CXXSHARED=@CXXSHARED@
CXXRFLAG= @CXXRFLAG@

# programs used to link.  We first prepend loc_libdir to LD_LIBRARY_PATH so
# that recursive library dependencies (of one library on another) work even
# before we install anything.
LINKEXEC=LD_LIBRARY_PATH=$(loc_libdir):$${LD_LIBRARY_PATH} \
	$(PURIFY) $(CXX) $(CXXFLAGS) ${LDFLAGS} -L${loc_libdir} -L$(libdir) \
		$(CXXRFLAG)$(libdir)
LINKLIB =LD_LIBRARY_PATH=$(loc_libdir):$${LD_LIBRARY_PATH} \
	$(CXX) $(CXXFLAGS) ${LDFLAGS} -L${loc_libdir} -L$(libdir) \
	    $(CXXRFLAG)$(libdir) $(CXXSHARED)

# flags for programs.
CFLAGS = @CFLAGS@
CXXFLAGS = @CXXFLAGS@
CPPFLAGS = @CPPFLAGS@ -I${loc_incldir}
LDFLAGS = @LDFLAGS@
DEFS = @DEFS@

# program version
VERSION=@VERSION@

# a dependency on xerces, either installed or just compiled
lXERCES=@lXERCES@			# how to link with xerces
loc_libXERCES=@loc_libXERCES@		# local to the build directory
libXERCES=@libXERCES@			# installed

# use -l$(L2LIB) to link, depend on the file ${loc_libdir}/$(L2LIB_FILE)
L2LIB=@L2LIB@
L2LIB_FILE=@L2LIB_FILE@

# end of top.mk
############################################################
