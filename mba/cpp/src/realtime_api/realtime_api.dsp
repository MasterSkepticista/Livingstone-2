# Microsoft Developer Studio Project File - Name="realtime_api" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=realtime_api - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "realtime_api.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "realtime_api.mak" CFG="realtime_api - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "realtime_api - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "realtime_api - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "realtime_api - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/realtime_api.lib"

!ELSEIF  "$(CFG)" == "realtime_api - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "STATIC_LIV" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/realtime_api.lib"

!ENDIF 

# Begin Target

# Name "realtime_api - Win32 Release"
# Name "realtime_api - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "win32"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\win32\guardcondition.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\guardedqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\livapi_rt.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\livingstone_dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\threadobject.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\timeout_handlers.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "win32 No. 1"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\include\realtime_api\win32\dataguard.h
# End Source File
# Begin Source File

SOURCE=..\..\include\realtime_api\win32\guardcondition.h
# End Source File
# Begin Source File

SOURCE=..\..\include\realtime_api\win32\livingstone_dispatcher.h
# End Source File
# Begin Source File

SOURCE=..\..\include\realtime_api\win32\threadobject.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\realtime_api\livingstonemessage.h
# End Source File
# Begin Source File

SOURCE=..\..\include\realtime_api\userfcns.h
# End Source File
# End Group
# End Target
# End Project
