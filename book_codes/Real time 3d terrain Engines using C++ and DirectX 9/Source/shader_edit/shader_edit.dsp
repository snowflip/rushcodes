# Microsoft Developer Studio Project File - Name="shader_edit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=shader_edit - Win32 Profile
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shader_edit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shader_edit.mak" CFG="shader_edit - Win32 Profile"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shader_edit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "shader_edit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "shader_edit - Win32 Profile" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Real-Time 3D Terrain Engines Using C++ and Direct X/source/shader_edit", AWAAAAAA"
# PROP Scc_LocalPath "..\..\source\shader_edit"
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shader_edit - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3dx9.lib SHLWAPI.LIB winmm.lib d3d9.lib dxerr9.lib largeint.lib version.lib imagehlp.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "shader_edit - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_CODE_TIMING" /D "_DEBUG_OUTPUT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3dx9dt.lib SHLWAPI.LIB winmm.lib d3d9.lib dxerr9.lib largeint.lib version.lib imagehlp.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd" /out:"..\bin\shader_edit_debug.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "shader_edit - Win32 Profile"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "shader_edit___Win32_Profile"
# PROP BASE Intermediate_Dir "shader_edit___Win32_Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "shader_edit___Win32_Profile"
# PROP Intermediate_Dir "shader_edit___Win32_Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /Ob2 /I "..\\" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_CODE_TIMING" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 winmm.lib d3d9.lib d3dx9dt.lib dxerr9.lib largeint.lib SHLWAPI.LIB /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd" /out:"..\bin\shader_edit_debug.exe" /pdbtype:sept
# ADD LINK32 d3dx9dt.lib SHLWAPI.LIB winmm.lib d3d9.lib dxerr9.lib largeint.lib version.lib imagehlp.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd" /out:"..\bin\shader_edit_debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "shader_edit - Win32 Release"
# Name "shader_edit - Win32 Debug"
# Name "shader_edit - Win32 Profile"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnimationBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelBrowserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\shader_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\shader_edit.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnimationBrowser.h
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ModelBrowserDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\shader_edit.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shader_edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\shader_edit.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
