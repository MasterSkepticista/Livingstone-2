# Microsoft Developer Studio Project File - Name="mba_utils" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mba_utils - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mba_utils.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mba_utils.mak" CFG="mba_utils - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mba_utils - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mba_utils - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mba_utils - Win32 Release"

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
# ADD LIB32 /nologo /out:"../../lib/mba_utils.lib"

!ELSEIF  "$(CFG)" == "mba_utils - Win32 Debug"

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
# ADD LIB32 /nologo /out:"../../lib/mba_utilsd.lib"

!ENDIF 

# Begin Target

# Name "mba_utils - Win32 Release"
# Name "mba_utils - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\array_allocator.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash_table.cpp
# End Source File
# Begin Source File

SOURCE=.\stringops.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\mba_utils\algos.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\array_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\array_dict.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\Hash_table.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\MBA_string.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\pair.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\pooled.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\prime.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\Slist.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\Slist_item.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\Slist_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\stringops.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mba_utils\utils.h
# End Source File
# End Group
# End Target
# End Project
