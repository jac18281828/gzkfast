# Microsoft Developer Studio Project File - Name="gzkparticle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=gzkparticle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gzkparticle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gzkparticle.mak" CFG="gzkparticle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gzkparticle - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "gzkparticle - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gzkparticle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GPROTON_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D DLLEXPORT=__declspec(dllexport) /D _WIN32_WINNT=0x0400 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "gzkparticle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GPROTON_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D DLLEXPORT=__declspec(dllexport) /D _WIN32_WINNT=0x0400 /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcmt" /out:"../lib/gzkparticled.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "gzkparticle - Win32 Release"
# Name "gzkparticle - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\cmb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\cmbdist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\delta.cpp
# End Source File
# Begin Source File

SOURCE=..\src\delta1600.cpp
# End Source File
# Begin Source File

SOURCE=..\src\deltaspectrum.cpp
# End Source File
# Begin Source File

SOURCE=..\src\eventgenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\evolutionthread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\g4vector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gguard.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ghistogram.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gloghistogram.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gmath.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gmutex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\grotation.cpp
# End Source File
# Begin Source File

SOURCE=..\src\grunthread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gthread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gvegas.cpp
# End Source File
# Begin Source File

SOURCE=..\src\interspace.cpp
# End Source File
# Begin Source File

SOURCE=..\src\mu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\neutron.cpp
# End Source File
# Begin Source File

SOURCE=..\src\nu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\nudetector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\particle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\photondetector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pion.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pizero.cpp
# End Source File
# Begin Source File

SOURCE=..\src\protondetector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\protonsource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\protonspectrum.cpp
# End Source File
# Begin Source File

SOURCE=..\src\sphere.cpp
# End Source File
# Begin Source File

SOURCE=..\src\threebodydecay.cpp
# End Source File
# Begin Source File

SOURCE=..\src\twobodydecay.cpp
# End Source File
# Begin Source File

SOURCE=..\src\universe.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\cmb.h
# End Source File
# Begin Source File

SOURCE=..\include\cmbdist.h
# End Source File
# Begin Source File

SOURCE=..\include\delta.h
# End Source File
# Begin Source File

SOURCE=..\include\delta1600.h
# End Source File
# Begin Source File

SOURCE=..\include\deltaspectrum.h
# End Source File
# Begin Source File

SOURCE=..\include\detector.h
# End Source File
# Begin Source File

SOURCE=..\include\electron.h
# End Source File
# Begin Source File

SOURCE=..\include\eventgenerator.h
# End Source File
# Begin Source File

SOURCE=..\include\evolutionthread.h
# End Source File
# Begin Source File

SOURCE=..\include\g4vector.h
# End Source File
# Begin Source File

SOURCE=..\include\gguard.h
# End Source File
# Begin Source File

SOURCE=..\include\ghistogram.h
# End Source File
# Begin Source File

SOURCE=..\include\gloghistogram.h
# End Source File
# Begin Source File

SOURCE=..\include\glorentz.h
# End Source File
# Begin Source File

SOURCE=..\include\gmath.h
# End Source File
# Begin Source File

SOURCE=..\include\gmatrix.h
# End Source File
# Begin Source File

SOURCE=..\include\gmutex.h
# End Source File
# Begin Source File

SOURCE=..\include\grotation.h
# End Source File
# Begin Source File

SOURCE=..\include\grunthread.h
# End Source File
# Begin Source File

SOURCE=..\include\gslist.h
# End Source File
# Begin Source File

SOURCE=..\include\gthread.h
# End Source File
# Begin Source File

SOURCE=..\include\gvector.h
# End Source File
# Begin Source File

SOURCE=..\include\gvegas.h
# End Source File
# Begin Source File

SOURCE=..\include\interspace.h
# End Source File
# Begin Source File

SOURCE=..\include\mu.h
# End Source File
# Begin Source File

SOURCE=..\include\neutron.h
# End Source File
# Begin Source File

SOURCE=..\include\nu.h
# End Source File
# Begin Source File

SOURCE=..\include\nudetector.h
# End Source File
# Begin Source File

SOURCE=..\include\particle.h
# End Source File
# Begin Source File

SOURCE=..\include\photon.h
# End Source File
# Begin Source File

SOURCE=..\include\photondetector.h
# End Source File
# Begin Source File

SOURCE=..\include\physicalconstants.h
# End Source File
# Begin Source File

SOURCE=..\include\pion.h
# End Source File
# Begin Source File

SOURCE=..\include\pizero.h
# End Source File
# Begin Source File

SOURCE=..\include\proton.h
# End Source File
# Begin Source File

SOURCE=..\include\protondetector.h
# End Source File
# Begin Source File

SOURCE=..\include\protonsource.h
# End Source File
# Begin Source File

SOURCE=..\include\protonspectrum.h
# End Source File
# Begin Source File

SOURCE=..\include\source.h
# End Source File
# Begin Source File

SOURCE=..\include\space.h
# End Source File
# Begin Source File

SOURCE=..\include\sphere.h
# End Source File
# Begin Source File

SOURCE=..\include\threebodydecay.h
# End Source File
# Begin Source File

SOURCE=..\include\twobodydecay.h
# End Source File
# Begin Source File

SOURCE=..\include\universe.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
