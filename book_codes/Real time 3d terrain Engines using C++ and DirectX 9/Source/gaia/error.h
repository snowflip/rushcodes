#ifndef ERROR_H
#define ERROR_H
#pragma once
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    sample code from the book...
    Real Time 3D Terrain Engines Using C++ and DirectX

    by Greg Snook
    greg@mightystudios.com

\***************************************************************/
#ifndef CORE_H
#include "core\core.h"
#endif

#include <windows.h>


//	Name Space declaration
namespace gaia
{


/*	Error
-----------------------------------------------------------------
    
    Simple Error handling code
    
-----------------------------------------------------------------
*/

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum GAIA_MESSAGE_TYPE
{ 
	MSG_NONE =0, 
	MSGERR_APPMUSTEXIT, 
	MSGWARN_SWITCHEDTOREF 
};

#define GAIA_OK						0
#define GAIAERR_NODIRECT3D          0x82000001
#define GAIAERR_NOWINDOW            0x82000002
#define GAIAERR_NOCOMPATIBLEDEVICES 0x82000003
#define GAIAERR_NOWINDOWABLEDEVICES 0x82000004
#define GAIAERR_NOHARDWAREDEVICE    0x82000005
#define GAIAERR_HALNOTCOMPATIBLE    0x82000006
#define GAIAERR_NOWINDOWEDHAL       0x82000007
#define GAIAERR_NODESKTOPHAL        0x82000008
#define GAIAERR_NOHALTHISMODE       0x82000009
#define GAIAERR_NONZEROREFCOUNT     0x8200000a
#define GAIAERR_MEDIANOTFOUND       0x8200000b
#define GAIAERR_RESIZEFAILED        0x8200000c
#define GAIAERR_CANTINVALIDATE      0x8200000d
#define GAIAERR_RELEASEFAILED       0x8200000e
#define GAIAERR_RESETFAILED			0x8200000f
#define GAIAERR_RESTOREFAILED		0x82000010
#define GAIAERR_DISPLAYFAILED		0x82000020
#define GAIAERR_INVALIDPARAM		0x82000030
#define GAIAERR_NODEVICE			0x82000040

//-----------------------------------------------------------------------------
// Error functions
//-----------------------------------------------------------------------------

void errorMessageBox(tchar* strMsg, DWORD flags);
uint32 DisplayErrorMsg( uint32 hr, uint32 dwType );


} //End NameSpace: gaia

#endif  // end of file      ( Error.h )

//----------------------------------------------------------
//$Log: $