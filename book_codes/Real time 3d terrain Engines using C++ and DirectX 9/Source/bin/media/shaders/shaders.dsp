# Microsoft Developer Studio Project File - Name="shaders" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=shaders - Win32 Profile
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shaders.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shaders.mak" CFG="shaders - Win32 Profile"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shaders - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "shaders - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE "shaders - Win32 Profile" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Real-Time 3D Terrain Engines Using C++ and Direct X/source/bin/media/shaders", SIBAAAAA"
# PROP Scc_LocalPath "..\..\..\..\source\bin\media\shaders"
MTL=midl.exe

!IF  "$(CFG)" == "shaders - Win32 Release"

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

!ELSEIF  "$(CFG)" == "shaders - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "shaders - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "shaders___Win32_Profile"
# PROP BASE Intermediate_Dir "shaders___Win32_Profile"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "shaders___Win32_Profile"
# PROP Intermediate_Dir "shaders___Win32_Profile"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "shaders - Win32 Release"
# Name "shaders - Win32 Debug"
# Name "shaders - Win32 Profile"
# Begin Group "effect files"

# PROP Default_Filter "*.h;*.fx"
# Begin Source File

SOURCE=.\animated_clouds.fx
# End Source File
# Begin Source File

SOURCE=.\DefaultMesh.fx
# End Source File
# Begin Source File

SOURCE=.\DefaultSkinnedMesh.fx
# End Source File
# Begin Source File

SOURCE=.\grass_patch.fx
# End Source File
# Begin Source File

SOURCE=.\grass_patch_ambient.fx
# End Source File
# Begin Source File

SOURCE=.\grass_patch_bump.fx
# End Source File
# Begin Source File

SOURCE=.\grass_patch_sunlight.fx
# End Source File
# Begin Source File

SOURCE=.\lens_flare.fx
# End Source File
# Begin Source File

SOURCE=.\light_scattering_constants.h
# End Source File
# Begin Source File

SOURCE=.\multipass_terrain.fx
# End Source File
# Begin Source File

SOURCE=.\ocean_water.fx
# End Source File
# Begin Source File

SOURCE=.\patch_deform.h
# End Source File
# Begin Source File

SOURCE=.\simple_skybox.fx
# End Source File
# Begin Source File

SOURCE=.\simple_skydome.fx
# End Source File
# Begin Source File

SOURCE=.\simple_terrain.fx
# End Source File
# Begin Source File

SOURCE=.\simple_terrain_ambient.fx
# End Source File
# Begin Source File

SOURCE=.\simple_terrain_bump.fx
# End Source File
# Begin Source File

SOURCE=.\simple_terrain_sunlight.fx
# End Source File
# Begin Source File

SOURCE=.\single_texture_terrain.fx
# End Source File
# Begin Source File

SOURCE=.\sunlit_object.fx
# End Source File
# Begin Source File

SOURCE=.\sunlit_skydome.fx
# End Source File
# Begin Source File

SOURCE=.\tree_fan_ambient.fx
# End Source File
# Begin Source File

SOURCE=.\tree_fan_bump.fx
# End Source File
# Begin Source File

SOURCE=.\tree_fan_sunlight.fx
# End Source File
# End Group
# End Target
# End Project
