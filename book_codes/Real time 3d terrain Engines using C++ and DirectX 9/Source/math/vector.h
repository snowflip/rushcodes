#ifndef VECTOR_H
#define VECTOR_H
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
#include "core/core.h"


// vector declarations, operators and functions
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "vector_functions.h"

namespace gaia
{
	// global, static vector values.
	static const cVector2 ZeroV2(0.0f,0.0f);
	static const cVector3 ZeroV3(0.0f,0.0f,0.0f);
	static const cVector4 ZeroV4(0.0f,0.0f,0.0f,0.0f);
	static const cVector4 ZeroV4W1(0.0f,0.0f,0.0f,1.0f);

	static const cVector2 OneV2(1.0f,1.0f);
	static const cVector3 OneV3(1.0f,1.0f,1.0f);
	static const cVector4 OneV4(1.0f,1.0f,1.0f,1.0f);

	static const cVector2 XAxisV2(1.0f,0.0f);
	static const cVector3 XAxisV3(1.0f,0.0f,0.0f);
	static const cVector4 XAxisV4(1.0f,0.0f,0.0f,0.0f);
	static const cVector4 XAxisV4W1(1.0f,0.0f,0.0f,1.0f);

	static const cVector2 YAxisV2(0.0f,1.0f);
	static const cVector3 YAxisV3(0.0f,1.0f,0.0f);
	static const cVector4 YAxisV4(0.0f,1.0f,0.0f,0.0f);
	static const cVector4 YAxisV4W1(0.0f,1.0f,0.0f,1.0f);

	static const cVector3 ZAxisV3(0.0f,0.0f,1.0f);
	static const cVector4 ZAxisV4(0.0f,0.0f,1.0f,0.0f);
	static const cVector4 ZAxisV4W1(0.0f,0.0f,1.0f,1.0f);

} 


//- End of Vector ------------------------------------------------------------------------

//****************************************************************************************


#endif  // end of file      ( Vector.h )

