README FOR L2 REALTIME API
------------------------------


The realtime api provides an interface for posting messages to the L2
engine that will run in a separate thread of execution. This maintains 
reactivity of the system as a whole while long computations complete. 
As implied, the api knows about time and thus commands may be scheduled 
for execution after some amount of clock time has passed. The api also 
provides for the specification of runtime autonomous diagnostic policies.
Defaults are provided. Details are in the api documentation.

Build notes:
Currently, the win32 threading support is native. Therefore, most of 
the api source files exist under the separate 'win32' directories. The 
'posix' directories contain an implementation utilizing the posix threads
specification interface or "pthreads". This is the implementation for unix 
builds. Win32 implementations of the pthreads specification exist, so 
in the future the code bases will most likely be merged.

Caveat: The pthreads libraries are not generally built with exception 
handling enabled. If an exception is encountered, the application crashes.



