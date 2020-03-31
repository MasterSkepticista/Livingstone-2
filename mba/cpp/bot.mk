############### bot.mk #################
# Global settings for all makefiles.
# This is appended to each makefile before Makefile.in
# Similarly, top.mk is prepended.

# See the file "mba/disclaimers-and-notices-L2.txt" for
# information on usage and redistribution of this file,
# and for a DISCLAIMER OF ALL WARRANTIES.

# This file mostly contains rules; most require variable assignments
# either in top.mk or Makefile.in

# Variable assignments not covered in top.mk:
#	SUBDIRS		all subdirectories with makefiles
#	TAGFILES	other files over which to generate tags
#	DEPFILES	other files over which to generate makedepend
#	CLEANFILES	extra files to delete with make clean
#	DLEANFILES	extra files to delete with make distclean
#	MCLEANFILES     extra files to delete with make maintainer-clean
#
#	LIBTARGET	the name, in ${loc_libdir}, of the library target
#	BINTARGET	the name, in `.', of the executable target
#	TARGET		make all makes this -- must be only one!
#	DEPS		other libraries; the top-level make handles these
#
#	HPREFIX		subdirectory of $(includedir) to install in
#				typically a package name, may be '.'
#	HINSTALL	full path of include files when installed (must be
#				$(includedir)/$(HPREFIX)/foo.h )

# $Id: bot.mk,v 1.18 2001/11/18 23:28:20 taylor Exp $

# Build libraries from the current directory.
$(LIBTARGET): $(OBJECTS) $(DEPS)
	@test -d `dirname $(LIBTARGET)` || { \
		echo $(INSTALL) -d `dirname $(LIBTARGET)` ; \
		$(INSTALL) -d `dirname $(LIBTARGET)` ; \
	}
	$(LINKLIB) $(LDFLAGS) -o $@ $(OBJECTS) \
		$(DEPS:${loc_libdir}/lib%.so=-l%) $(LIBS)

# This says that to build dependencies in other directories, we first see if
# the file exists.  If so, no questions asked.  If not, ask the top-level
# makefile to build it for us.  Note: the dependency will be relative to the
# current directory, whereas after -C we'll be relative to top_srcdir, hence
# the pattern.
$(DEPS):
	$(MAKE) -C ${top_srcdir} $(@:${top_srcdir}/%=%)

%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFS) $(CPPFLAGS) $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(DEFS) $(CPPFLAGS) $<

# mainly for debugging: compile .i files by running cpp on .c files.
%.i: %.c
	$(CPP) $(DEFS) $(CPPFLAGS) $< > $@

%.ii: %.cpp
	$(CXXCPP) $(DEFS) $(CPPFLAGS) $< > $@

# all files depend on config.h
%.i %.ii %.lo: ${top_srcdir}/include/config.h


# write out a file that prints the filenames of all objects
objects: Makefile $(OBJECTS)
	@if test "x$(OBJECTS)" = "x" ; then \
	    $(RM) objects ;\
	else \
	    $(RM) objects ;\
	    echo "$(OBJECTS)" > objects ;\
	fi


#### the 'all' rule just builds all the targets.  If it was already defined,
#### this will do no harm.
.PHONY: all
all: $(TARGET)


#### Keep the makefile up to date
Makefile: ${top_srcdir}/top.mk Makefile.in ${top_srcdir}/bot.mk \
          ${top_srcdir}/config.status
	${top_srcdir}/reconf ${top_srcdir}

####################
#### The following is only slightly modified from the autoconf info
#### file.  It makes sure the makefile and everything else is up to
#### date.  I added directory specifications since we're not using VPATH.

${top_srcdir}/configure: ${top_srcdir}/configure.in ${top_srcdir}/aclocal.m4
	cd ${top_srcdir} && autoconf

# autoheader might not change config.h.in, so touch a stamp file.
${top_srcdir}/config.h.in: ${top_srcdir}/stamp-h.in
${top_srcdir}/stamp-h.in: ${top_srcdir}/configure.in ${top_srcdir}/aclocal.m4 \
			${top_srcdir}/acconfig.h
	cd ${top_srcdir} && autoheader
	echo timestamp > ${top_srcdir}/stamp-h.in

${locincl_dir}/config.h: ${locincl_dir}/stamp-h
${locincl_dir}/stamp-h: ${top_srcdir}/config.h.in \
			${top_srcdir}/config.status
	cd ${top_srcdir} && ./reconf include/config.h


${top_srcdir}/config.status: ${top_srcdir}/configure
	cd ${top_srcdir} && ./config.status --recheck

#### done copying
####################


####################
#### Tags are a great way to navigate across the code.
#### On some systems, making tags creates a huge number of warnings.
.PHONY: tags TAGS ${top_srcdir}/tags ${top_srcdir}/TAGS

# vi & clones
tags: Makefile ${top_srcdir}/tags

${top_srcdir}/tags: $(TAGSFILES) Makefile
	make -C ${top_srcdir} tags

tags-links:
	$(RM) tags
	$(LN_S) ${top_srcdir}/tags tags
	@${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

# emacs & clones
TAGS: Makefile ${top_srcdir}/TAGS

${top_srcdir}/TAGS: $(TAGSFILES) Makefile
	make -C ${top_srcdir} TAGS

TAGS-links:
	$(RM) TAGS
	$(LN_S) ${top_srcdir}/TAGS TAGS
	@${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

####################
#### generate and use dependency information.
#### Note we define __cplusplus since otherwise we might miss some includes.
.PHONY: depend dep
depend: $(DEPFILES)
	makedepend $(CPPFLAGS) $(DEFS) -D__cplusplus *.c *.cc *.cpp

# do it recursively
dep: depend
	@${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

####################
#### clean up.
.PHONY: clean distclean maintainer-clean

DEF_CLEANFILES = *.lo *.o *.i *.ii *.so *.la *.a \
	core a.out mon.out gmon.out tags TAGS Makefile.bak objects

clean:
	$(RM) $(DEF_CLEANFILES) $(CLEANFILES) 
	$(RM) $(TARGET)
	${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

# don't delete Makefile; the $(top_srcdir)/Makefile does that.
distclean: 
	$(RM) $(DEF_CLEANFILES) $(CLEANFILES) $(DCLEANFILES) 
	$(RM) $(TARGET)
	${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

maintainer-clean:
	$(RM) $(DEF_CLEANFILES) $(CLEANFILES) $(DCLEANFILES) $(MCLEANFILES)
	$(RM) $(TARGET)
	${top_srcdir}/makerecur "$(MAKE)" $@ $(SUBDIRS)

####################
#### installation

BININSTALL+=$(BINTARGET:%=$(bindir)/%)
LIBINSTALL+=$(LIBTARGET:${loc_libdir}/%=$(libdir)/%)
DATAINSTALL+=$(DATATARGET:%=$(datadir)/%)

$(bindir)/% : %
	@test -d $(bindir) || \
		echo "$(INSTALL) -d $(bindir)" &&\
		$(INSTALL) -d $(bindir)
	$(INSTALL) $< $@

$(datadir)/% : %
	@test -d $(datadir) || \
		echo "$(INSTALL) -d $(datadir)" &&\
		$(INSTALL) -d $(datadir)
		$(INSTALL) -d $(datadir)/rti_user
	$(INSTALL) -m 444 $< $@

$(includedir)/$(HPREFIX)/%.h : %.h
	@test -d $(includedir)/$(HPREFIX) || \
		echo "$(INSTALL) -d $(includedir)/$(HPREFIX)" &&\
		$(INSTALL) -d $(includedir)/$(HPREFIX)
	$(INSTALL) -m644 $< $@

# First make the directory if it doesn't exist.
# Next install, but only if the directories are different.  
# Must be in an if rather than just an && to avoid failing when the 
# directories are identical.
$(libdir)/%: ${loc_libdir}/%
	@test -d $(libdir) || \
		echo "$(INSTALL) -d $(libdir)" &&\
		$(INSTALL) -d $(libdir)
	@if test `cd $(libdir) && pwd` != `cd $(loc_libdir) && pwd` ; then \
	    echo $(INSTALL) $< $@ ; \
	    $(INSTALL) $< $@ ; \
	fi


.PHONY:   install install-binaries install-libs install-includes install-data
install: 	  install-binaries install-libs install-includes install-data
	$(top_srcdir)/makerecur "$(MAKE)" $@ $(SUBDIRS)

install-binaries: $(BININSTALL)
install-includes: $(HINSTALL)
install-libs:  	  $(LIBINSTALL)
install-data:	  $(DATAINSTALL)

.PHONY:   uninstall uninstall-binaries uninstall-libs uninstall-includes uninstall-data
uninstall: 	    uninstall-binaries uninstall-libs uninstall-includes uninstall-data
	$(top_srcdir)/makerecur "$(MAKE)" $@ $(SUBDIRS)

uninstall-binaries:
	@if test -n "$(BININSTALL)" ; then \
	    echo "$(RM) $(BININSTALL)" ; $(RM) $(BININSTALL) ; \
	fi

uninstall-includes:
	@if test -n "$(HINSTALL)" ; then \
	    echo "$(RM) $(HINSTALL)" ; $(RM) $(HINSTALL) ; \
	fi

uninstall-libs:
	@if test -n "$(LIBINSTALL)" ; then \
	    echo "$(RM) $(LIBINSTALL)" ; $(RM) $(LIBINSTALL) ; \
	fi

uninstall-data:
	@if test -n "$(DATAINSTALL)" ; then \
	    echo "$(RM) $(DATAINSTALL)" ; $(RM) $(DATAINSTALL) ; \
	fi

# end of bot.mk
############################################################
