# Microsoft Developer Studio Project File - Name="conflict_db" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=conflict_db - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "conflict_db.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "conflict_db.mak" CFG="conflict_db - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "conflict_db - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "conflict_db - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "conflict_db - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include" /I "../../include/mba_utils" /I "../../../../support/xerces/src/" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/conflict_db.lib"

!ELSEIF  "$(CFG)" == "conflict_db - Win32 Debug"

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
# ADD LIB32 /nologo /out:"../../lib/conflict_dbd.lib"

!ENDIF 

# Begin Target

# Name "conflict_db - Win32 Release"
# Name "conflict_db - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Assignable.cpp
# End Source File
# Begin Source File

SOURCE=.\Assignment.cpp
# End Source File
# Begin Source File

SOURCE=.\assumption.cpp
# End Source File
# Begin Source File

SOURCE=.\Candidate.cpp
# End Source File
# Begin Source File

SOURCE=.\Conflict.cpp
# End Source File
# Begin Source File

SOURCE=.\Conflict_db.cpp
# End Source File
# Begin Source File

SOURCE=.\Dependent.cpp
# End Source File
# Begin Source File

SOURCE=.\Variable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\conflict_db\Assignable.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Assignment.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Assumption.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Candidate.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Conflict.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Conflict_db.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Dependent.h
# End Source File
# Begin Source File

SOURCE=..\..\include\conflict_db\Variable.h
# End Source File
# End Group
# End Target
# End Project
