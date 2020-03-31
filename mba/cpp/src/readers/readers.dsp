# Microsoft Developer Studio Project File - Name="readers" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=readers - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "readers.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "readers.mak" CFG="readers - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "readers - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "readers - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "readers - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include" /I "../../../../support/xerces/src/" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/readers.lib"

!ELSEIF  "$(CFG)" == "readers - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../../support/xerces/src/" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/readersd.lib"

!ENDIF 

# Begin Target

# Name "readers - Win32 Release"
# Name "readers - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\clause.cpp
# End Source File
# Begin Source File

SOURCE=.\enumeration.cpp
# End Source File
# Begin Source File

SOURCE=.\from_any.cpp
# End Source File
# Begin Source File

SOURCE=.\from_bin.cpp
# End Source File
# Begin Source File

SOURCE=.\from_l2.cpp
# End Source File
# Begin Source File

SOURCE=.\from_memory.cpp
# End Source File
# Begin Source File

SOURCE=.\from_xmpl.cpp
# End Source File
# Begin Source File

SOURCE=.\L2_file.cpp
# End Source File
# Begin Source File

SOURCE=.\L2_file_translator.cpp
# End Source File
# Begin Source File

SOURCE=.\livingstone_reader.cpp
# End Source File
# Begin Source File

SOURCE=.\modes.cpp
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\proposition.cpp
# End Source File
# Begin Source File

SOURCE=.\to_bin.cpp
# End Source File
# Begin Source File

SOURCE=.\to_l2.cpp
# End Source File
# Begin Source File

SOURCE=.\to_t_system.cpp
# End Source File
# Begin Source File

SOURCE=.\transition.cpp
# End Source File
# Begin Source File

SOURCE=.\variable.cpp
# End Source File
# Begin Source File

SOURCE=.\xlate_merge_eq_vars.cpp
# End Source File
# Begin Source File

SOURCE=.\xlate_remove_duplicates.cpp
# End Source File
# Begin Source File

SOURCE=.\xlate_sort_props.cpp
# End Source File
# Begin Source File

SOURCE=.\xlate_strip.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\readers\clause.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\enumeration.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\from_any.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\from_bin.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\from_l2.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\from_memory.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\from_xmpl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\L2_file.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\livingstone_reader.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\modes.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\options.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\proposition.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\to_bin.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\to_l2.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\to_t_system.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\transition.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\variable.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\xlate_merge_eq_vars.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\xlate_remove_duplicates.h
# End Source File
# Begin Source File

SOURCE=..\..\include\readers\xlate_sort_props.h
# End Source File
# End Group
# End Target
# End Project
