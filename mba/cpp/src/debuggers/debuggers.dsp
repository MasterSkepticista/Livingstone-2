# Microsoft Developer Studio Project File - Name="debuggers" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=debuggers - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "debuggers.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "debuggers.mak" CFG="debuggers - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "debuggers - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "debuggers - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "debuggers - Win32 Release"

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
# ADD LIB32 /nologo /out:"../../lib/debuggers.lib"

!ELSEIF  "$(CFG)" == "debuggers - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/debuggersd.lib"

!ENDIF 

# Begin Target

# Name "debuggers - Win32 Release"
# Name "debuggers - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\conflict_db_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\domain.cpp
# End Source File
# Begin Source File

SOURCE=.\L2_string_map.cpp
# End Source File
# Begin Source File

SOURCE=.\L2_string_map_listener.cpp
# End Source File
# Begin Source File

SOURCE=.\livingstone_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\ltms_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\T_system_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\tracker_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\trajectory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\debuggers\conflict_db_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\domain.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\L2_string_map.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\L2_string_map_listener.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\livingstone_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\ltms_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\T_system_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\tracker_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debuggers\trajectory.h
# End Source File
# End Group
# End Target
# End Project
