#ifndef SPHERE_H
#define SPHERE_H
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
#include "circle.h"


//	Name Space declaration
namespace gaia
{


class cSphere
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	union
	{
		struct
		{
			float	radius;
			cVector3 center;
		};
		struct
		{
			cCircle circle;
			float z;
		};
	};

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cSphere(){};
	cSphere(const cVector3& _center, float _radius);
	cSphere(float _x, float _y, float _z, float _radius);
	cSphere(const cSphere& src);
	~cSphere(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------


	// casting down
	operator const cCircle&()const {return(circle);};
	operator cCircle&() {return(circle);};

	// equality
	bool operator == (const cSphere& src)const; 
	
	// inequality
	bool operator != (const cSphere& src)const; 

	// copy
	const cSphere& operator = (const cSphere& src); 


	//--------------------------------------------------
	// Calculations\Operations...
	//--------------------------------------------------
	void clear();
	bool isClear()const;
	bool valid()const;
	void assertValid()const;
	void normalize();

	void set(const cVector3& _center, float _radius);
	void set(float _x, float _y, float _z, float _radius);
	void set(const cSphere& src);

	float area()const;
	void offset(const cVector3 &offset);
	void expand(float n);
};


//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cSphere
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/

inline cSphere::cSphere(const cVector3& _center, float _radius)
:center(_center)
,radius(_radius)
{}


inline cSphere::cSphere(float _x, float _y, float _z, float _radius)
:center(_x,_y,_z)
,radius(_radius)
{}


inline cSphere::cSphere(const cSphere& src)
:center(src.center)
,radius(src.radius)
{}




//--------------------------------------------------
// Operators
//

inline bool cSphere::operator == (const cSphere& src)const
{
	return (center == src.center 
			&& radius == src.radius);
}
 


inline bool cSphere::operator != (const cSphere& src)const
{
	return (center != src.center 
			|| radius != src.radius);
}



inline const cSphere& cSphere::operator = (const cSphere& src)
{
	center = src.center; 
	radius = src.radius;
	return(*this);
}
 

//--------------------------------------------------
// Inline Functions
//

inline void cSphere::clear(void)
{
	center.clear(); 
	radius = 0.0f; 
}


inline bool cSphere::isClear(void)const
{
	return ((center.isClear())
		&& (0.0f == radius));
}


inline void cSphere::set (const cVector3& _center, float _radius)
{
	center = _center; 
	radius = _radius; 
}


inline void cSphere::set (float _x, float _y, float _z, float _radius)
{
	center.set(_x,_y,_z);
	radius = _radius; 
}
 

inline void cSphere::set (const cSphere& src)
{
	center = src.center; 
	radius = src.radius; 
}


//
// valid()
//

inline bool cSphere::valid()const
{
	return(radius >= 0.0f);
}

//
// assertValid()
//

inline void cSphere::assertValid()const
{
	debug_assert(radius >= 0.0f, "sphere inverted");
}


inline void cSphere::offset(const cVector3 &offset)
{
	center += offset;
}



inline void cSphere::expand(float n)
{
	radius += n;
}



inline void cSphere::normalize()
{
	radius = absoluteValue(radius);
}

//- End of cSphere ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSphere.h )

//----------------------------------------------------------
//$Log: $