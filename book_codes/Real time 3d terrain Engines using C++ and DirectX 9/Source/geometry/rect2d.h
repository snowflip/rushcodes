#ifndef RECT2D_H
#define RECT2D_H
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




class cRect2d
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	union
	{
		struct
		{
			float x0, x1;
			float y0, y1;
		};
	};

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cRect2d(){};
	cRect2d(float _x0, float _x1, float _y0, float _y1);
	cRect2d(const cRect2d& src);
	cRect2d(float _xSize, float _ySize);
	explicit cRect2d(const cVector2& size);
	~cRect2d(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------
	// negate
	cRect2d operator - ();

	// equality
	bool operator == (const cRect2d& src)const; 
	
	// inequality
	bool operator != (const cRect2d& src)const; 

	// copy
	const cRect2d& operator = (const cRect2d& src); 

	const cRect2d& operator = (const cVector2& vec); 

	const cRect2d& operator += (const cVector2& vec); 
	const cRect2d& operator += (float s);

	const cRect2d& operator -= (const cVector2& vec); 
	const cRect2d& operator -= (float s);

	const cRect2d& operator *= (const cVector2& vec) ;
	const cRect2d& operator *= (float s) ;
	
	const cRect2d& operator /= (const cVector2& vec) ;		
	const cRect2d& operator /= (float s) ;		

	//--------------------------------------------------
	// Calculations\Operations...
	//--------------------------------------------------
	void clear();
	bool isClear()const;
	bool valid()const;
	void assertValid()const;
	void normalize();

	void set(const cRect2d& v);
	void set(float _x0, float _x1, float _y0, float _y1); 
	void set(const cVector2& size); 

	void setFloor(const cRect2d& input);
	void setCeiling(const cRect2d& input);

	void resizeX(float span);
	void resizeY(float span);
	void resize(const cVector2& size);

	void resizeMinX(float span);
	void resizeMinY(float span);
	void resizeMinZ(float span);
	void resizeMin(const cVector2& size);

	void resizeMaxX(float span);
	void resizeMaxY(float span);
	void resizeMaxZ(float span);
	void resizeMax(const cVector2& size);

	float sizeX()const;
	float sizeY()const;
	cVector2 size()const;

	float midX()const;
	float midY()const;
	cVector2 midpoint()const;
	cVector2 minPoint()const;
	cVector2 maxPoint()const;

	float area()const;

	void unionPoint(const cVector2& point);
	void unionRect(const cRect2d& rect);

	void offset(const cVector2 &offset);

	void expand(float n);
	void expand(const cVector2& size);

	void expandX(float n);
	void expandY(float n);

	cVector2 corner(int index)const;

	bool pointInRect(const cVector2& point)const;
	cCircle createBoundingCircle()const;
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cRect2d
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/

inline cRect2d::cRect2d(float _x0, float _x1, float _y0, float _y1)
:x0(_x0)
,x1(_x1)
,y0(_y0)
,y1(_y1){};


inline cRect2d::cRect2d(const cRect2d& src)
:x0(src.x0)
,x1(src.x1)
,y0(src.y0)
,y1(src.y1){};


inline cRect2d::cRect2d(float _xSize, float _ySize)
:x0(0.0f)
,x1(0.0f)
,y0(0.0f)
,y1(0.0f)
{
	resize(cVector2(_xSize, _ySize));
}


inline cRect2d::cRect2d(const cVector2& size)
:x0(0.0f)
,x1(0.0f)
,y0(0.0f)
,y1(0.0f)
{
	resize(size);
}

//--------------------------------------------------
// Operators
//

inline bool cRect2d::operator == (const cRect2d& src)const
{
	return (	x0 == src.x0 
			&&  x1 == src.x1
			&&  y0 == src.y0
			&&  y1 == src.y1);
}
 


inline bool cRect2d::operator != (const cRect2d& src)const
{
	return (	x0 != src.x0 
			||  x1 != src.x1
			||  y0 != src.y0
			||  y1 != src.y1);
}



inline const cRect2d& cRect2d::operator = (const cRect2d& src)
{
	x0 = src.x0; 
	x1 = src.x1;
	y0 = src.y0;
	y1 = src.y1;
	return(*this);
}
 


inline const cRect2d& cRect2d::operator += (const cVector2& vec)
{
	x0 += vec.x; 
	x1 += vec.x; 
	y0 += vec.y; 
	y1 += vec.y; 
	return(*this);
}
 

inline const cRect2d& cRect2d::operator += (float s)
{
	x0 += s; 
	x1 += s; 
	y0 += s; 
	y1 += s; 
	return(*this);
}


inline cRect2d operator+(const cRect2d& lha, const cVector2& rha)
{
	return(cRect2d(	lha.x0 + rha.x,
						lha.x1 + rha.x,
						lha.y0 + rha.y,
						lha.y1 + rha.y));
}


inline cRect2d operator+(const cVector2& lha, const cRect2d& rha)
{
	return(cRect2d(	rha.x0 + lha.x,
						rha.x1 + lha.x,
						rha.y0 + lha.y,
						rha.y1 + lha.y));
}


inline cRect2d operator+(const cRect2d& lha, float rha)
{
	return(cRect2d(	lha.x0 + rha,
						lha.x1 + rha,
						lha.y0 + rha,
						lha.y1 + rha));
}


inline cRect2d operator+(float lha, const cRect2d& rha)
{
	return(cRect2d(	rha.x0 + lha,
						rha.x1 + lha,
						rha.y0 + lha,
						rha.y1 + lha));
}


inline const cRect2d& cRect2d::operator-=(const cVector2& vec)
{
	x0 -= vec.x; 
	x1 -= vec.x; 
	y0 -= vec.y; 
	y1 -= vec.y; 
	return(*this);
}
 

inline const cRect2d& cRect2d::operator-= (float s)
{
	x0 -= s; 
	x1 -= s; 
	y0 -= s; 
	y1 -= s; 
	return(*this);
}



inline cRect2d operator - (const cRect2d& lha, const cVector2& rha)
{
	return(cRect2d(	lha.x0 - rha.x,
						lha.x1 - rha.x,
						lha.y0 - rha.y,
						lha.y1 - rha.y));
}



inline cRect2d operator - (const cRect2d& lha, float rha)
{
	return(cRect2d(	lha.x0 - rha,
						lha.x1 - rha,
						lha.y0 - rha,
						lha.y1 - rha));
}


inline cRect2d cRect2d::operator - ()
{
	return(cRect2d(-x1,-x0,-y1,-y0));
}


inline const cRect2d& cRect2d::operator *= (float s)
{
	x0 *= s; 
	x1 *= s; 
	y0 *= s; 
	y1 *= s; 
	return(*this);
}


inline const cRect2d& cRect2d::operator *= (const cVector2& vec)
{
	x0 *= vec.x; 
	x1 *= vec.x; 
	y0 *= vec.y; 
	y1 *= vec.y; 
	return(*this);
}



inline cRect2d operator * (const cRect2d& lha, const cVector2& rha)
{
	return(cRect2d(	lha.x0 * rha.x,
						lha.x1 * rha.x,
						lha.y0 * rha.y,
						lha.y1 * rha.y));
}

inline cRect2d operator * (const cVector2& lha, const cRect2d& rha)
{
	return(cRect2d(	rha.x0 * lha.x,
						rha.x1 * lha.x,
						rha.y0 * lha.y,
						rha.y1 * lha.y));
}



inline cRect2d operator * (const cRect2d& lha, float rha)
{
	return(cRect2d(	lha.x0 * rha,
						lha.x1 * rha,
						lha.y0 * rha,
						lha.y1 * rha));
}


inline cRect2d operator * (float lha, const cRect2d& rha)
{
	return(cRect2d(	rha.x0 * lha,
						rha.x1 * lha,
						rha.y0 * lha,
						rha.y1 * lha));
}


inline const cRect2d& cRect2d::operator /= (float s)
{
	debug_assert(s != 0.0f, "divide by zero error");
	x0 /= s; 
	x1 /= s; 
	y0 /= s; 
	y1 /= s; 
	return(*this);
}
	

inline const cRect2d& cRect2d::operator /= (const cVector2& vec)
{
	debug_assert(vec.x != 0.0f, "divide by zero error");
	debug_assert(vec.y != 0.0f, "divide by zero error");

	x0 /= vec.x; 
	x1 /= vec.x; 
	y0 /= vec.y; 
	y1 /= vec.y; 
	return(*this);
}


inline cRect2d operator / (const cRect2d& lha, const cVector2& rha)
{
	debug_assert(rha.x != 0.0f, "divide by zero error");
	debug_assert(rha.y != 0.0f, "divide by zero error");

	return(cRect2d(	lha.x0 / rha.x,
						lha.x1 / rha.x,
						lha.y0 / rha.y,
						lha.y1 / rha.y));
}

inline cRect2d operator / (const cRect2d& lha, float rha)
{
	debug_assert(rha != 0.0f, "divide by zero error");

	return(cRect2d(	lha.x0 / rha,
						lha.x1 / rha,
						lha.y0 / rha,
						lha.y1 / rha));
}

//--------------------------------------------------
// Inline Functions
//

inline void cRect2d::clear(void)
{
	x0 = 0.0f; 
	x1 = 0.0f; 
	y0 = 0.0f; 
	y1 = 0.0f; 
}


inline bool cRect2d::isClear(void)const
{
	return ((0.0f == x0)
		&& (0.0f == x1)
		&& (0.0f == y0)
		&& (0.0f == y1));
}


inline void cRect2d::set (const cRect2d& src)
{
	x0 = src.x0; 
	x1 = src.x1;
	y0 = src.y0;
	y1 = src.y1;
}


inline void cRect2d::set (float _x0, float _x1, float _y0, float _y1)
{
	x0 = _x0; 
	x1 = _x1;
	y0 = _y0;
	y1 = _y1;
}
 

inline void cRect2d::set (const cVector2& size)
{
	clear();
	resize(size);
}


//
// setFloor
//

inline void cRect2d::setFloor(const cRect2d& input)
{
	x0 = floorf(input.x0);
	x1 = floorf(input.x1);
	y0 = floorf(input.y0);
	y1 = floorf(input.y1);
}

//
// setCeiling
//
inline void cRect2d::setCeiling(const cRect2d& input)
{
	x0 = ceilf(input.x0);
	x1 = ceilf(input.x1);
	y0 = ceilf(input.y0);
	y1 = ceilf(input.y1);
}


//
// valid()
//

inline bool cRect2d::valid()const
{
	return(		x0 <= x1
			&&	y0 <= y1);
}

//
// assertValid()
//

inline void cRect2d::assertValid()const
{
	debug_assert((x0 <= x1), "rectangle inverted on X axis");
	debug_assert((y0 <= y1), "rectangle inverted on Y axis");
}


inline void cRect2d::resizeX(float size)
{
	x1 = midX() + (size*0.5f);
	x0 = x1 - size;
}


inline void cRect2d::resizeY(float size)
{
	y1 = midY() + (size*0.5f);
	y0 = y1 - size;
}



inline void cRect2d::resize(const cVector2& size)
{
	resizeX(size.x);
	resizeY(size.y);
}



inline void cRect2d::resizeMaxX(float span)
{
	x1 = x0 + span;
}


inline void cRect2d::resizeMaxY(float span)
{
	y1 = y0 + span;
}


inline void cRect2d::resizeMax(const cVector2& size)
{
	resizeMaxX(size.x);
	resizeMaxY(size.y);
}



inline void cRect2d::resizeMinX(float span)
{
	x0 = x1 - span;
}


inline void cRect2d::resizeMinY(float span)
{
	y0 = y1 - span;
}


inline void cRect2d::resizeMin(const cVector2& size)
{
	resizeMinX(size.x);
	resizeMinY(size.y);
}


inline float cRect2d::midX()const
{
	return (x0+x1)*0.5f;
}


inline float cRect2d::midY()const
{
	return (y0+y1)*0.5f;
}


inline cVector2 cRect2d::midpoint()const
{
	return cVector2(midX(), midY());
}


inline float cRect2d::sizeX()const
{
	return (x1-x0);
}


inline float cRect2d::sizeY()const
{
	return (y1-y0);
}


inline cVector2 cRect2d::size()const
{
	return cVector2(sizeX(), sizeY());
}


inline cVector2 cRect2d::minPoint()const
{
	return cVector2(x0, y0);
}


inline cVector2 cRect2d::maxPoint()const
{
	return cVector2(x1, y1);
}


inline float cRect2d::area()const
{
	return(sizeX()*sizeY());
}


inline void cRect2d::unionPoint(const cVector2& point)
{
	x0 = minimum(x0, point.x);
	y0 = minimum(y0, point.y);

	x1 = maximum(x1, point.x);
	y1 = maximum(y1, point.y);
}


inline void cRect2d::unionRect(const cRect2d& rect)
{
	assertValid();
	rect.assertValid();

	x0 = minimum(x0, rect.x0);
	y0 = minimum(y0, rect.y0);

	x1 = maximum(x1, rect.x1);
	y1 = maximum(y1, rect.y1);
}


inline void cRect2d::offset(const cVector2 &offset)
{
	x0 += offset.x;
	x1 += offset.x;
	y0 += offset.y;
	y1 += offset.y;
}



inline void cRect2d::expand(float n)
{
	x0 -= n;
	x1 += n;
	y0 -= n;
	y1 += n;
}


inline void cRect2d::expandX(float n)
{
	x0 -= n;
	x1 += n;
}


inline void cRect2d::expandY(float n)
{
	y0 -= n;
	y1 += n;
}


inline void cRect2d::expand(const cVector2& size)
{
	expandX(size.x);
	expandY(size.y);
}



inline void cRect2d::normalize()
{
	if(x0 > x1)
	{
		swap(x0, x1);
	}
	if(y0 > y1)
	{
		swap(y0, y1);
	}
}



inline cVector2 cRect2d::corner(int index)const
{
	debug_assert(index>=0 && index <4, "invalid index");

	return cVector2( (index & 1) ? x0: x1,
						(index & 2) ? y0: y1);
}

inline bool cRect2d::pointInRect(const cVector2& point)const
{
	return point.x >= x0
		&& point.y >= y0
		&& point.x <= x1
		&& point.y <= y1;
}

inline cCircle cRect2d::createBoundingCircle()const
{
	return cCircle(
		midpoint(), 
		(sizeX()+sizeY())*0.5f);
}

//. Mutators ....................................................

//. Accessors ...................................................

//- End of cRect2d ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRect2d.h )

//----------------------------------------------------------
//$Log: $