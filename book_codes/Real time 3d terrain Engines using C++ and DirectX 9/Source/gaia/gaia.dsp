# Microsoft Developer Studio Project File - Name="gaia" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gaia - Win32 Profile
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gaia.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gaia.mak" CFG="gaia - Win32 Profile"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gaia - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gaia - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gaia - Win32 Profile" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/3D Terrain Engines/source/gaia", BCAAAAAA"
# PROP Scc_LocalPath "..\..\..\3d terrain engines from the ground up\source\gaia"
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "gaia - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gaia - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /D "_MBCS" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_CODE_TIMING" /D "_DEBUG_OUTPUT" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gaia - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gaia___Win32_Profile"
# PROP BASE Intermediate_Dir "gaia___Win32_Profile"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "gaia___Win32_Profile"
# PROP Intermediate_Dir "gaia___Win32_Profile"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /Ob2 /I "..\\" /D "_MBCS" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_CODE_TIMING" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "gaia - Win32 Release"
# Name "gaia - Win32 Debug"
# Name "gaia - Win32 Profile"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\animation_controller.cpp
# End Source File
# Begin Source File

SOURCE=.\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\chunk_section.cpp
# End Source File
# Begin Source File

SOURCE=.\chunk_system.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dx_frame_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\display_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\effect_file.cpp
# End Source File
# Begin Source File

SOURCE=.\error.cpp
# End Source File
# Begin Source File

SOURCE=.\game_host.cpp
# End Source File
# Begin Source File

SOURCE=.\grass_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\grass_patch.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\index_buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\lens_flare.cpp
# End Source File
# Begin Source File

SOURCE=.\light_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\light_scattering_data.cpp
# End Source File
# Begin Source File

SOURCE=.\model_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\ocean_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\orientation.cpp
# End Source File
# Begin Source File

SOURCE=.\plant_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\quad_tree.cpp
# End Source File
# Begin Source File

SOURCE=.\quad_tree_node.cpp
# End Source File
# Begin Source File

SOURCE=.\render_method.cpp
# End Source File
# Begin Source File

SOURCE=.\render_queue.cpp
# End Source File
# Begin Source File

SOURCE=.\roam_section.cpp
# End Source File
# Begin Source File

SOURCE=.\roam_system.cpp
# End Source File
# Begin Source File

SOURCE=.\scene_model.cpp
# End Source File
# Begin Source File

SOURCE=.\scene_node.cpp
# End Source File
# Begin Source File

SOURCE=.\scene_object.cpp
# End Source File
# Begin Source File

SOURCE=.\sky_model.cpp
# End Source File
# Begin Source File

SOURCE=.\subnode_group.cpp
# End Source File
# Begin Source File

SOURCE=.\surface_material.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain_section.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain_system.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain_tile_section.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain_tile_system.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\vertex_buffer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\animation_controller.h
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\chunk_section.h
# End Source File
# Begin Source File

SOURCE=.\chunk_system.h
# End Source File
# Begin Source File

SOURCE=.\d3dx_frame_manager.h
# End Source File
# Begin Source File

SOURCE=.\display_manager.h
# End Source File
# Begin Source File

SOURCE=.\effect_file.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\gaia.h
# End Source File
# Begin Source File

SOURCE=.\game_host.h
# End Source File
# Begin Source File

SOURCE=.\grass_manager.h
# End Source File
# Begin Source File

SOURCE=.\grass_patch.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\index_buffer.h
# End Source File
# Begin Source File

SOURCE=.\lens_flare.h
# End Source File
# Begin Source File

SOURCE=.\light_manager.h
# End Source File
# Begin Source File

SOURCE=.\light_scattering_data.h
# End Source File
# Begin Source File

SOURCE=.\model_resource.h
# End Source File
# Begin Source File

SOURCE=.\ocean_manager.h
# End Source File
# Begin Source File

SOURCE=.\orientation.h
# End Source File
# Begin Source File

SOURCE=.\plant_manager.h
# End Source File
# Begin Source File

SOURCE=.\quad_tree.h
# End Source File
# Begin Source File

SOURCE=.\quad_tree_node.h
# End Source File
# Begin Source File

SOURCE=.\render_method.h
# End Source File
# Begin Source File

SOURCE=.\render_queue.h
# End Source File
# Begin Source File

SOURCE=.\roam_section.h
# End Source File
# Begin Source File

SOURCE=.\roam_system.h
# End Source File
# Begin Source File

SOURCE=.\scene_model.h
# End Source File
# Begin Source File

SOURCE=.\scene_node.h
# End Source File
# Begin Source File

SOURCE=.\scene_object.h
# End Source File
# Begin Source File

SOURCE=.\sky_model.h
# End Source File
# Begin Source File

SOURCE=.\subnode_group.h
# End Source File
# Begin Source File

SOURCE=.\surface_material.h
# End Source File
# Begin Source File

SOURCE=.\terrain_section.h
# End Source File
# Begin Source File

SOURCE=.\terrain_system.h
# End Source File
# Begin Source File

SOURCE=.\terrain_tile_section.h
# End Source File
# Begin Source File

SOURCE=.\terrain_tile_shapes.h
# End Source File
# Begin Source File

SOURCE=.\terrain_tile_system.h
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# Begin Source File

SOURCE=.\vertex_buffer.h
# End Source File
# Begin Source File

SOURCE=.\xfile_templates.h
# End Source File
# End Group
# Begin Group "D3D Sample Framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\framework\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3denumeration.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3denumeration.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3dfile.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3dfile.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3dfont.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3dres.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3dsettings.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3dsettings.h
# End Source File
# Begin Source File

SOURCE=.\framework\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\framework\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\framework\dxutil.h
# End Source File
# End Group
# End Target
# End Project
