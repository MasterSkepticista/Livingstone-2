
######
# $Id: xerces.mk,v 1.4 2001/11/17 00:10:41 lbrown Exp $
# This file is included in order to get a with-xerces build.

# See the file "mba/disclaimers-and-notices-L2.txt" for
# information on usage and redistribution of this file,
# and for a DISCLAIMER OF ALL WARRANTIES.

###########################################################################
# we have to handle libxerces ourself.  Otherwise:
# (a) it rebuilds _every_ time
# (b) make install isn't potty-trained: it dumps all over the place!

XERCESCROOT = @XERCESCROOT@
XERCESENV   = @XERCESENV@
ROOT_XERCES_SO   = ${loc_libXERCES:$(loc_libdir)/%=$(XERCESCROOT)/lib/%}

# making the library; don't if it exists
$(ROOT_XERCES_SO):
	${XERCESENV} make -C $(XERCESCROOT)/src

# copying it to ./lib
# double-colon quashes a warning if loc_libdir==libdir
$(loc_libXERCES):: $(ROOT_XERCES_SO)
	@test -d $(loc_libdir) || { \
	    echo "$(INSTALL) -d $(loc_libdir)" ; \
	    $(INSTALL) -d $(loc_libdir) ; \
	}
	$(INSTALL) $? $@

# installing it
# double-colon quashes a warning if loc_libdir==libdir
$(libXERCES):: $(loc_libXERCES)
	@test -d $(libdir) || { \
	    echo "$(INSTALL) -d $(libdir)" ; \
	    $(INSTALL) -d $(libdir) ; \
	}
	@if test `cd $(libdir) && pwd` != `cd $(loc_libdir) && pwd` ; then \
	    echo $(INSTALL) $? $@ ; \
	    $(INSTALL) $? $@ ; \
	fi

# to allow 'make xerces' symmetrically with 'make xerces-clean'
.PHONY: xerces xerces-clean
xerces: $(loc_libXERCES)

xerces-clean:
	${XERCESENV} $(MAKE) -C $(XERCESCROOT)/src clean



