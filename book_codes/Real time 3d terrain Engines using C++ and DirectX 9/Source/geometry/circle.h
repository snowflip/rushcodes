#ifndef CIRCLE_H
#define CIRCLE_H
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



class cCircle
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	float	radius;
	cVector2 center;

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cCircle(){};
	cCircle(const cVector2& _center, float _radius);
	cCircle(float _x, float _y, float _radius);
	cCircle(const cCircle& src);
	~cCircle(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------

	// equality
	bool operator == (const cCircle& src)const; 
	
	// inequality
	bool operator != (const cCircle& src)const; 

	// copy
	const cCircle& operator = (const cCircle& src); 


	//--------------------------------------------------
	// Calculations\Operations...
	//--------------------------------------------------
	void clear();
	bool isClear()const;
	bool valid()const;
	void assertValid()const;
	void normalize();

	void set(const cVector2& _center, float _radius);
	void set(float _x, float _y, float _radius);
	void set(const cCircle& src);

	float area()const;
	void offset(const cVector2 &offset);
	void expand(float n);
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cCircle
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/

inline cCircle::cCircle(const cVector2& _center, float _radius)
:center(_center)
,radius(_radius)
{}


inline cCircle::cCircle(float _x, float _y, float _radius)
:center(_x,_y)
,radius(_radius)
{}


inline cCircle::cCircle(const cCircle& src)
:center(src.center)
,radius(src.radius)
{}




//--------------------------------------------------
// Operators
//

inline bool cCircle::operator == (const cCircle& src)const
{
	return (center == src.center 
			&& radius == src.radius);
}
 


inline bool cCircle::operator != (const cCircle& src)const
{
	return (center != src.center 
			|| radius != src.radius);
}



inline const cCircle& cCircle::operator = (const cCircle& src)
{
	center = src.center; 
	radius = src.radius;
	return(*this);
}
 

//--------------------------------------------------
// Inline Functions
//

inline void cCircle::clear(void)
{
	center.clear(); 
	radius = 0.0f; 
}


inline bool cCircle::isClear(void)const
{
	return ((center.isClear())
		&& (0.0f == radius));
}


inline void cCircle::set (const cVector2& _center, float _radius)
{
	center = _center; 
	radius = _radius; 
}


inline void cCircle::set (float _x, float _y, float _radius)
{
	center.set(_x,_y);
	radius = _radius; 
}
 

inline void cCircle::set (const cCircle& src)
{
	center = src.center; 
	radius = src.radius; 
}



//
// valid()
//

inline bool cCircle::valid()const
{
	return(radius >= 0.0f);
}

//
// assertValid()
//

inline void cCircle::assertValid()const
{
	debug_assert(radius >= 0.0f, "sphere inverted");
}


inline void cCircle::offset(const cVector2 &offset)
{
	center += offset;
}



inline void cCircle::expand(float n)
{
	radius += n;
}



inline void cCircle::normalize()
{
	radius = absoluteValue(radius);
}

//- End of cCircle ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cCircle.h )

//----------------------------------------------------------
//$Log: $