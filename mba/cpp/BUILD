$Id: BUILD,v 1.49 2003/06/23 22:00:18 taylor Exp $

See the file "mba/disclaimers-and-notices-L2.txt" for
information on usage and redistribution of this file,
and for a DISCLAIMER OF ALL WARRANTIES.

Requirements:
=============
(0) These build instructions are for the Linux, Solaris, and Irix
    platforms.  The build instructions for Windows are on the web page.
    No other compiler or platform has been tested.

(1) You must have checked out the appropriate modules in a directory two
    levels above the current one:
        mba/cpp
        support/xerces-unix-src
    If you're following the Stanley build instructions, you'll already have
    the first one.

    If checking out from CVS and you already have an old distribution, run
    l2_update:
        ./l2_update
    If you don't have an old distribution, check out the code:
        cvs co mba/cpp support/xerces-unix-src
        cd mba/cpp
        ./l2_update -t
    If you got a distribution, just unpack the distribution.

(2) You must be using gcc.  Tested: gcc 2.8.1 on Irix, gcc 2.95.2 on both
    solaris and linux, gcc 2.95.3 on solaris.
    No other compilers are supported, but you can always try; tell us
    if it works.  
    gcc-3.0 (at least on solaris) doesn't currently work; we're trying to
    fix that, though it's not the highest priority.

(3) You must use GNU make.  GNU make 3.77 doesn't seem to work; use an
    earlier or later version.


Instructions:
=============

*Note: you may want to build with non-default features; see the `Extra
        features' section on instructions to do this.
*Note: see the Irix Special Magic section to build on SGI platforms.

L2 builds like a regular GNU program:
        ./configure --prefix=`pwd`
will test how to do a variety of things on your system; assuming the system
is reasonably enough like Unix, it will tweak L2 to work.  This will also
configure the XML parser, xerces-c.

The --prefix option means to install in the given directory rather than in
/usr/local.  It must be an abolute path.  Stanley wants the prefix to be
`pwd` (that is, the mba/cpp directory); the standalone programs don't
care.  Libraries are installed in prefix/lib, executables in prefix/bin.

You'll see a message from the xerces-c library telling you to go to a
directory and type gmake.  Ignore it; that's done automatically.  Also
ignore the message about conf.in.

Now that it's configured, compile and install it:
        make install

When you later want to remove it, type:
        make uninstall
You can still uninstall even if you remove the source: first download the
same version as what you'd installed, then configure with the same options
as you had used; then make uninstall.


Using it
========

There are two main ways to use L2 using this configuration system: the
standalone program l2test; and with stanley.

To build the stand-alone executable, install livingstone as described in
the previous section.  The l2test executable will be found in prefix/bin.


To use the new L2 with stanley, install it.  The prefix must be `pwd`.

Diagnosis: If the pink-orange window in which the L2 messages normally go
instead flips in and out of existance, and you get an inifite sequence of
dots in the xterm from which you ran stanley, the the library was
improperly installed or has some other problem.


Extra features:
===============

By doing:
        ./configure --help
You will get a list of features you can toggle.  Put them on the command
line with `./configure --prefix=...'

Note that the following options are silently ignored:
        srcdir
        program-prefix
        program-suffix
        program-transform-name
By `ignored' I mean that there is no warning, but they don't do anything.
This is unfortunate, I know.

The defaults get you a library that works with Stanley and as a stand-alone
program, as described above.


disable-shared
--------------

Build livdll as a static archive (.a) rather than a shared object (.so)
In addition, we don't direct the compiler to emit position-independent
code.

This option is incompatible with
        --enable-rti    (as of 2000-12-20; this is a TODO)
        --enable-jni
Java requires livdll to be a dynamic object; it will not work if you have
a static object.  However, the library will compile and link, so we only
warn if both options are given.


This option does not change livdll's functionality, but it does affect
livdll.


enable-rti
----------
Create the livrti library.  By default we skip that step.
To build the test program:
        cd src/test
        make l2test-rt
See the RTI documentation.

This option adds a library:
	liblivrti.so
This option does not change livdll.


disable-jni
-----------
Exclude the java native interface from the livdll library; by default it is
included.  This code is required for stanley/skunkworks to work.

This option removes code from the livdll library.
This option breaks stanley/skunkworks.


disable-exceptions
------------------
Instead of throwing exceptions, print an error message and abort.

This option changes code in the livdll library.
This option breaks stanley/skunkworks.


enable-recovery
---------------
Configure the engine and executables to be used for recovery rather than
diagnosis.

This option changes the functionality of livdll rather drastically.
This option breaks stanley/skunkworks.


disable-cover
disable-cbfs
------------
Disable one or the other (both is disallowed) search method.

These options remove code in livdll.
These options will cause errors in stanley/skunkworks if they try to use a
disabled search method.


disable-full-progress
disable-min-progress
--------------------
Disable one or the other (both is disallowed) progress method.

These options remove code in livdll.
These options will cause warnings in stanley/skunkworks if they try to use a
disabled progress method.


disable-checkpoint
------------------
Disable the ability to store checkpoints.

This option removes code from livdll.



enable-count-allocated
----------------------
Enable printing a report of the maximum number of allocated items at any
time, broken down between statically and dynamically allocated items.  The
number reported for statically allocated objects is the number you
statically allocated; so if you create an array of 500 and only actually
use 50, we still report 500.

The report is generated when the program exits and calls the destructors
for the static memory pools.

This option adds code in livdll, and generates output to stdout.


disable-dynamic-memory
----------------------
Does not allocate new objects when the static pool are filled.

This option removes code from livdll.
This option will break scenarios if L2 was compiled with static pools that
are not large enough.


disable-cheap-memory-tests
enable-prohibitive-memory-tests
-------------------------------
Change how much the memory allocation routines test memory usage.
'Cheap' means approximately constant in the amount of memory allocated
(it's actually linear if dynamic memory is enabled, but with a slope
something like 2^-20), 'prohibitive' means linear.  The cheap tests are on
by default, the prohibitive ones off.

The former option removes, the latter adds, code to livdll.
The former option speeds up slightly, the latter slows down greatly,
run-time performance.


enable-l2test
enable-l2flight
enable-names2ids
enable-xmpl2l2
enable-apigen
---------------
Enable various programs to do something useful with the models.
- l2test  : reads a model in any format and provides a command-line
	    interface to test the L2 engine
- l2flight: similar to l2test but no names, just integer IDs; this allows
            testing the flight article (without debuggers)
- names2ids : translates a scenario file with names into one with IDs
- scr2names : translates a Stanley scenario file in a names2ids one
- xmpl2l2 : reads a model in one format (like xmpl), possibly optimizes it,
            then writes it in another format (like ascii L2)
- api_gen : reads a model and output C-style enums into 'enumapi.h'
- api_scr : translates a readable RTI scenario file

l2test is on by default; the others are off.  enable-apigen enables both
api_gen and api_scr ; enable-names2ids enables both names2ids and
scr2names.

These options create new executables.
These options may add code to the livdll library, as needed (i.e. l2test
needs the debuggers).


disable-xmpl-format
disable-l2-format
disable-l2bin-format
-----------------
Disallow l2test from reading xmpl, l2, or the binary format.  l2test will
still recognize files in a disabled format, but will refuse to load them.

These options remove code from the livdll library if l2test is enabled;
otherwise, they are either illegal (if you ask to disable a required
format) or have no effect (if the format wasn't being used anyway).



disable-verbose
---------------
Disable the ability of the reader to print verbose information.
Basically only useful for flight; it's not much code.

This option removes code from the livdll library.


disable-optimizer
-----------------
Disable the file optimizer.

Optimization is required for min-progress to correctly run ; you will need
to compile your models with optimization (using xmpl2l2 or l2strip)
beforehand, and only recompile livingstone with the optimizer off for the
flight code.

Note this option does _not_ affect the level of optimization used in
compiled the Livingstone engine, only whether Livingstone can optimize a
model.

This option removes code from the livdll library.


disable-debug-sections
----------------------
Disable handling debugging sections in L2 files.

You will need to have stripped your model (using l2strip) beforehand, and
only recompile livingstone with debug sections disabled for the flight
code.  XMPL and l2-format files with debugging information cannot be parsed
when debugging information is not available.

Without the debugging sections, variables and values will only be known by
their IDs -- clearly, this makes debugging more difficult.

This option removes code from the livdll library.


with-xerces
-----------
Specify the path to the xerces library.
By default, we use ../../support/xerces-unix-src
This directory must be a source distribution of the library.

This option changes the run-path encoded in the livdll library.
This option is ignored if XMPL support is not needed or is disabled.


with-xerces-lib
---------------
Specify the name of the xerces library; do not include the 'lib' and '.so'.
This allows using a different version of the library, as in:
        --with-xerces-lib=xerces-c1_1
to use version 1.1 instead of 1.2

This option changes the name of the xerces library encoded in the livdll
library.
This option is ignored if XMPL support is not needed or is disabled.


without-Wall
with-Werror
without-warnings
----------------
These three options change what warning options to supply to the compiler.
The first two pass on flags -Wall (print lots of warnings) or -Werror
(treat warnings as errors -- this won't work at the moment) to gcc and are
ignored for other compilers.  The --without-warnings option passes -w to
gcc or Sun CC, which silences all warnings.

By default, -Wall is on while -Werror and -w are off.

These options do not change livdll.


with-profile
------------
Compile with profiler information.  The default switch used, "-pg", can be
changed by using '--with-profile=switch'.

This option is incompatible with --enable-shared.
This option greatly slows down and slightly enlarges livdll.


with-purify
------------
Link with purify, which instruments the code to check for memory access,
open files, and others.

This option is incompatible with --enable-shared.

This option may be incompatible with XMPL support.  Our installation of
purify doesn't support threads; and Xerces 1.2 requires threads.
Use the option --with-purify-despite-threads to skip the error.

This option greatly slows down and slightly enlarges livdll.


Irix Special Magic
==================

Building under Irix needs some special hacking.

Requirements as above; you must use gcc/g++.  We've tested only gcc-2.8.1,
but newer versions will almost certainly work as well.

Steps:
------
(1) configure as above

(2) look at mba/cpp/Makefile, and specifically the XERCESENV variable.
    It has extra settings, possibly for the PATH, and definitely for
    XERCESCROOT.  In a shell, set PATH and XERCESCROOT.
    To do this, if the XERCESENV variable is
        PATH=xxx:${PATH} XERCESCROOT=yyy
    then, if you're using sh, ksh, bash, or some other sh derivative, do:
        PATH=xxx:${PATH}
        XERCESCROOT=yyy
        export PATH XERCESCROOT
    if you're using csh or tcsh, do:
        setenv PATH xxx:${PATH}
        setenv XERCESCROOT yyy

(3) In that shell, do:
        cd ${XERCESCROOT}/src
        make            # this builds most of xerces, except...
        cd ${XERCESCROOT}/src/util/Platforms/IRIX
        make            # they forget to build here
        cd ${XERCESCROOT}/src
        make            # unfortunately, this rebuilds _everything_

(4) make as per the normal instructions; ignore the copious ld warnings
    about multiply defined weak symbols; they're just warning that weak
    symbols are working as expected!

(5) Define the shell variable _RLD_ARGS to include -ignore_unresolved ;
    otherwise, you'll get an error about being unable to find the ftime
    function.  Under sh:
        _RLD_ARGS="-ignore_unresolved" ; export _RLD_ARGS
    under csh:
        setenv _RLD_ARGS "-ignore_unresolved"
    This must be done both to build _and_ to run L2.
