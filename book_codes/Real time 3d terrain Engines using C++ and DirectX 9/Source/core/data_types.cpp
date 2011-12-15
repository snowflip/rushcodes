#define DATATYPES_CPP
#define CORE_DLL
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

#include "data_types.h"

using namespace gaia;
 
const int32	    gaia::MIN_INT32   =   (int32)0xffffffff; // (-2147483646);
const int32	    gaia::MAX_INT32   =   (int32)0x7fffffff; // ( 2147483647);
const int16	    gaia::MIN_INT16   =   (int16)0xffff; // (-32768);
const int16	    gaia::MAX_INT16   =   (int16)0x7fff; // ( 32767);
const int8	    gaia::MIN_INT8    =   (int8)0xff; // (-128);
const int8	    gaia::MAX_INT8    =   (int8)0x7f; // ( 127);
const uint32    gaia::MIN_UINT32  =   (uint32)0;
const uint32    gaia::MAX_UINT32  =   (uint32)0xffffffff;
const uint16    gaia::MIN_UINT16  =   (uint16)0;
const uint16    gaia::MAX_UINT16  =   (uint16)0xffff;
const uint8	    gaia::MIN_UINT8   =   (uint8)0;
const uint8	    gaia::MAX_UINT8   =   (uint8)0xff;

const float	gaia::MAX_REAL32  =   static_cast<float>(3.4E+38);
const float	gaia::MIN_REAL32  =   static_cast<float>(-3.4E+38);
const float	gaia::TINY_REAL32  =   static_cast<float>(3.4E-38);

//***************************************************************
// end of file      ( datatypes.cpp )

//----------------------------------------------------------
//$Log: $