#ifndef LINES_H
#define LINES_H
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
#include <core/core.h>
#include <math/math.h>


//	Name Space declaration
namespace gaia
{


class cRay2d
{
public:

	cVector2	point;
	cVector2	normal;

    cRay2d(){};
    ~cRay2d(){};

};


class cRay3d
{
public:

	cVector3	point;
	cVector3	normal;

    cRay3d(){};
    ~cRay3d(){};

};

class cSegment2d
{
public:

	union
	{
		struct
		{
			cRay2d		ray;
		};
		struct
		{
			cVector2	point;
			cVector2	normal;
		};
	};
	float		distance;

    cSegment2d(){};
    ~cSegment2d(){};

};

class cSegment3d
{
public:

	union
	{
		struct
		{
			cRay3d		ray;
		};
		struct
		{
			cVector3	point;
			cVector3	normal;
		};
	};
	float		distance;

    cSegment3d(){};
    ~cSegment3d(){};

};

} //End NameSpace: gaia

#endif  // end of file      ( lines.h )

//----------------------------------------------------------
//$Log: $