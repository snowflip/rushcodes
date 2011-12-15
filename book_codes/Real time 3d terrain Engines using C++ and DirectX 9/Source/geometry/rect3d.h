#ifndef RECT3D_H
#define RECT3D_H
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
#include "rect2d.h"
#include "sphere.h"


//	Name Space declaration
namespace gaia
{


class cRect3d
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
			float z0, z1;
		};
		struct
		{
			cRect2d rect2d;
			float z0, z1;
		};
	};

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cRect3d(){};
	cRect3d(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1);
	cRect3d(const cRect3d& src);
	cRect3d(float _xSize, float _ySize, float _zSize);
	explicit cRect3d(const cVector3& size);
	~cRect3d(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------

	// casting down
	operator const cRect2d&()const {return(rect2d);};
	operator cRect2d&() {return(rect2d);};

	// negate
	cRect3d operator - ();

	// equality
	bool operator == (const cRect3d& src)const; 
	
	// inequality
	bool operator != (const cRect3d& src)const; 

	// copy
	const cRect3d& operator = (const cRect3d& src); 

	const cRect3d& operator = (const cVector3& vec); 

	const cRect3d& operator += (const cVector3& vec); 
	const cRect3d& operator += (float s);

	const cRect3d& operator -= (const cVector3& vec); 
	const cRect3d& operator -= (float s);

	const cRect3d& operator *= (const cVector3& vec) ;
	const cRect3d& operator *= (float s) ;
	
	const cRect3d& operator /= (const cVector3& vec) ;		
	const cRect3d& operator /= (float s) ;		

	//--------------------------------------------------
	// Calculations\Operations...
	//--------------------------------------------------
	void clear();
	bool isClear()const;
	bool valid()const;
	void assertValid()const;
	void normalize();

	void set(const cRect3d& v);
	void set(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1); 
	void set(const cVector3& size); 

	void setFloor(const cRect3d& input);
	void setCeiling(const cRect3d& input);

	void resizeX(float span);
	void resizeY(float span);
	void resizeZ(float span);
	void resize(const cVector3& size);

	void resizeMinX(float span);
	void resizeMinY(float span);
	void resizeMinZ(float span);
	void resizeMin(const cVector3& size);

	void resizeMaxX(float span);
	void resizeMaxY(float span);
	void resizeMaxZ(float span);
	void resizeMax(const cVector3& size);

	float sizeX()const;
	float sizeY()const;
	float sizeZ()const;
	cVector3 size()const;

	float midX()const;
	float midY()const;
	float midZ()const;
	cVector3 midpoint()const;
	cVector3 minPoint()const;
	cVector3 maxPoint()const;

	float area()const;

	void unionPoint(const cVector3& point);
	void unionRect(const cRect3d& rect);

	void offset(const cVector3 &offset);

	void expand(float n);
	void expand(const cVector3& size);

	void expandX(float n);
	void expandY(float n);
	void expandZ(float n);

	cVector3 corner(int index)const;

	bool pointInRect(const cVector3& point)const;

	void transform(const cMatrix4x4* matrix);

	cSphere createBoundingSphere()const;
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cRect3d
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/

inline cRect3d::cRect3d(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1)
:x0(_x0)
,x1(_x1)
,y0(_y0)
,y1(_y1)
,z0(_z0)
,z1(_z1){};


inline cRect3d::cRect3d(const cRect3d& src)
:x0(src.x0)
,x1(src.x1)
,y0(src.y0)
,y1(src.y1)
,z0(src.z0)
,z1(src.z1){};


inline cRect3d::cRect3d(float _xSize, float _ySize, float _zSize)
:x0(0.0f)
,x1(0.0f)
,y0(0.0f)
,y1(0.0f)
,z0(0.0f)
,z1(0.0f)
{
	resize(cVector3(_xSize, _ySize, _zSize));
}


inline cRect3d::cRect3d(const cVector3& size)
:x0(0.0f)
,x1(0.0f)
,y0(0.0f)
,y1(0.0f)
,z0(0.0f)
,z1(0.0f)
{
	resize(size);
}

//--------------------------------------------------
// Operators
//

inline bool cRect3d::operator == (const cRect3d& src)const
{
	return (	x0 == src.x0 
			&&  x1 == src.x1
			&&  y0 == src.y0
			&&  y1 == src.y1
			&&  z0 == src.z0
			&&  z1 == src.z1);
}
 


inline bool cRect3d::operator != (const cRect3d& src)const
{
	return (	x0 != src.x0 
			||  x1 != src.x1
			||  y0 != src.y0
			||  y1 != src.y1
			||  z0 != src.z0
			||  z1 != src.z1);
}



inline const cRect3d& cRect3d::operator = (const cRect3d& src)
{
	x0 = src.x0; 
	x1 = src.x1;
	y0 = src.y0;
	y1 = src.y1;
	z0 = src.z0;
	z1 = src.z1;
	return(*this);
}
 


inline const cRect3d& cRect3d::operator += (const cVector3& vec)
{
	x0 += vec.x; 
	x1 += vec.x; 
	y0 += vec.y; 
	y1 += vec.y; 
	z0 += vec.z; 
	z1 += vec.z; 
	return(*this);
}
 

inline const cRect3d& cRect3d::operator += (float s)
{
	x0 += s; 
	x1 += s; 
	y0 += s; 
	y1 += s; 
	z0 += s; 
	z1 += s; 
	return(*this);
}


inline cRect3d operator + (const cRect3d& lha, const cVector3& rha)
{
	return(cRect3d(	lha.x0 + rha.x,
						lha.x1 + rha.x,
						lha.y0 + rha.y,
						lha.y1 + rha.y,
						lha.z0 + rha.z,
						lha.z1 + rha.z));
}


inline cRect3d operator + (const cVector3& lha, const cRect3d& rha)
{
	return(cRect3d(	rha.x0 + lha.x,
						rha.x1 + lha.x,
						rha.y0 + lha.y,
						rha.y1 + lha.y,
						rha.z0 + lha.z,
						rha.z1 + lha.z));
}


inline cRect3d operator + (const cRect3d& lha, float rha)
{
	return(cRect3d(	lha.x0 + rha,
						lha.x1 + rha,
						lha.y0 + rha,
						lha.y1 + rha,
						lha.z0 + rha,
						lha.z1 + rha));
}


inline cRect3d operator + (float lha, const cRect3d& rha)
{
	return(cRect3d(	rha.x0 + lha,
						rha.x1 + lha,
						rha.y0 + lha,
						rha.y1 + lha,
						rha.z0 + lha,
						rha.z1 + lha));
}


inline const cRect3d& cRect3d::operator -= (const cVector3& vec)
{
	x0 -= vec.x; 
	x1 -= vec.x; 
	y0 -= vec.y; 
	y1 -= vec.y; 
	z0 -= vec.z; 
	z1 -= vec.z; 
	return(*this);
}
 

inline const cRect3d& cRect3d::operator -= (float s)
{
	x0 -= s; 
	x1 -= s; 
	y0 -= s; 
	y1 -= s; 
	z0 -= s; 
	z1 -= s; 
	return(*this);
}



inline cRect3d operator - (const cRect3d& lha, const cVector3& rha)
{
	return(cRect3d(	lha.x0 - rha.x,
						lha.x1 - rha.x,
						lha.y0 - rha.y,
						lha.y1 - rha.y,
						lha.z0 - rha.z,
						lha.z1 - rha.z));
}



inline cRect3d operator - (const cRect3d& lha, float rha)
{
	return(cRect3d(	lha.x0 - rha,
						lha.x1 - rha,
						lha.y0 - rha,
						lha.y1 - rha,
						lha.z0 - rha,
						lha.z1 - rha));
}


inline cRect3d cRect3d::operator - ()
{
	return(cRect3d(-x1,-x0,-y1,-y0,-z1,-z0));
}


inline const cRect3d& cRect3d::operator *= (float s)
{
	x0 *= s; 
	x1 *= s; 
	y0 *= s; 
	y1 *= s; 
	z0 *= s; 
	z1 *= s; 
	return(*this);
}


inline const cRect3d& cRect3d::operator *= (const cVector3& vec)
{
	x0 *= vec.x; 
	x1 *= vec.x; 
	y0 *= vec.y; 
	y1 *= vec.y; 
	z0 *= vec.z; 
	z1 *= vec.z; 
	return(*this);
}



inline cRect3d operator * (const cRect3d& lha, const cVector3& rha)
{
	return(cRect3d(	lha.x0 * rha.x,
						lha.x1 * rha.x,
						lha.y0 * rha.y,
						lha.y1 * rha.y,
						lha.z0 * rha.z,
						lha.z1 * rha.z));
}

inline cRect3d operator * (const cVector3& lha, const cRect3d& rha)
{
	return(cRect3d(	rha.x0 * lha.x,
						rha.x1 * lha.x,
						rha.y0 * lha.y,
						rha.y1 * lha.y,
						rha.z0 * lha.z,
						rha.z1 * lha.z));
}



inline cRect3d operator * (const cRect3d& lha, float rha)
{
	return(cRect3d(	lha.x0 * rha,
						lha.x1 * rha,
						lha.y0 * rha,
						lha.y1 * rha,
						lha.z0 * rha,
						lha.z1 * rha));
}


inline cRect3d operator * (float lha, const cRect3d& rha)
{
	return(cRect3d(	rha.x0 * lha,
						rha.x1 * lha,
						rha.y0 * lha,
						rha.y1 * lha,
						rha.z0 * lha,
						rha.z1 * lha));
}


inline const cRect3d& cRect3d::operator /= (float s)
{
	debug_assert(s != 0.0f, "divide by zero error");
	x0 /= s; 
	x1 /= s; 
	y0 /= s; 
	y1 /= s; 
	z0 /= s; 
	z1 /= s; 
	return(*this);
}
	

inline const cRect3d& cRect3d::operator /= (const cVector3& vec)
{
	debug_assert(vec.x != 0.0f, "divide by zero error");
	debug_assert(vec.y != 0.0f, "divide by zero error");
	debug_assert(vec.z != 0.0f, "divide by zero error");

	x0 /= vec.x; 
	x1 /= vec.x; 
	y0 /= vec.y; 
	y1 /= vec.y; 
	z0 /= vec.z; 
	z1 /= vec.z; 
	return(*this);
}


inline cRect3d operator / (const cRect3d& lha, const cVector3& rha)
{
	debug_assert(rha.x != 0.0f, "divide by zero error");
	debug_assert(rha.y != 0.0f, "divide by zero error");
	debug_assert(rha.z != 0.0f, "divide by zero error");

	return(cRect3d(	lha.x0 / rha.x,
						lha.x1 / rha.x,
						lha.y0 / rha.y,
						lha.y1 / rha.y,
						lha.z0 / rha.z,
						lha.z1 / rha.z));
}

inline cRect3d operator / (const cRect3d& lha, float rha)
{
	debug_assert(rha != 0.0f, "divide by zero error");

	return(cRect3d(	lha.x0 / rha,
						lha.x1 / rha,
						lha.y0 / rha,
						lha.y1 / rha,
						lha.z0 / rha,
						lha.z1 / rha));
}

//--------------------------------------------------
// Inline Functions
//

inline void cRect3d::clear(void)
{
	x0 = 0.0f; 
	x1 = 0.0f; 
	y0 = 0.0f; 
	y1 = 0.0f; 
	z0 = 0.0f; 
	z1 = 0.0f; 
}


inline bool cRect3d::isClear(void)const
{
	return ((0.0f == x0)
		&& (0.0f == x1)
		&& (0.0f == y0)
		&& (0.0f == y1)
		&& (0.0f == z0)
		&& (0.0f == z1));
}


inline void cRect3d::set (const cRect3d& src)
{
	x0 = src.x0; 
	x1 = src.x1;
	y0 = src.y0;
	y1 = src.y1;
	z0 = src.z0;
	z1 = src.z1;
}


inline void cRect3d::set (float _x0, float _x1, float _y0, float _y1, float _z0, float _z1)
{
	x0 = _x0; 
	x1 = _x1;
	y0 = _y0;
	y1 = _y1;
	z0 = _z0;
	z1 = _z1;
}
 

inline void cRect3d::set (const cVector3& size)
{
	clear();
	resize(size);
}




//
// setFloor
//

inline void cRect3d::setFloor(const cRect3d& input)
{
	x0 = floorf(input.x0);
	x1 = floorf(input.x1);
	y0 = floorf(input.y0);
	y1 = floorf(input.y1);
	z0 = floorf(input.z0);
	z1 = floorf(input.z1);
}

//
// setCeiling
//

inline void cRect3d::setCeiling(const cRect3d& input)
{
	x0 = ceilf(input.x0);
	x1 = ceilf(input.x1);
	y0 = ceilf(input.y0);
	y1 = ceilf(input.y1);
	z0 = ceilf(input.z0);
	z1 = ceilf(input.z1);
}


//
// valid()
//

inline bool cRect3d::valid()const
{
	return(		x0 <= x1
			&&	y0 <= y1
			&&	z0 <= z1);
}

//
// assertValid()
//

inline void cRect3d::assertValid()const
{
	debug_assert((x0 <= x1), "rectangle inverted on X axis");
	debug_assert((y0 <= y1), "rectangle inverted on Y axis");
	debug_assert((z0 <= z1), "rectangle inverted on Z axis");
}


inline void cRect3d::resizeX(float size)
{
	x1 = midX() + (size*0.5f);
	x0 = x1 - size;
}


inline void cRect3d::resizeY(float size)
{
	y1 = midY() + (size*0.5f);
	y0 = y1 - size;
}


inline void cRect3d::resizeZ(float size)
{
	z1 = midZ() + (size*0.5f);
	z0 = z1 - size;
}


inline void cRect3d::resize(const cVector3& size)
{
	resizeX(size.x);
	resizeY(size.y);
	resizeZ(size.z);
}



inline void cRect3d::resizeMaxX(float span)
{
	x1 = x0 + span;
}


inline void cRect3d::resizeMaxY(float span)
{
	y1 = y0 + span;
}


inline void cRect3d::resizeMaxZ(float span)
{
	z1 = z0 + span;
}


inline void cRect3d::resizeMax(const cVector3& size)
{
	resizeMaxX(size.x);
	resizeMaxY(size.y);
	resizeMaxZ(size.z);
}



inline void cRect3d::resizeMinX(float span)
{
	x0 = x1 - span;
}


inline void cRect3d::resizeMinY(float span)
{
	y0 = y1 - span;
}


inline void cRect3d::resizeMinZ(float span)
{
	z0 = z1 - span;
}


inline void cRect3d::resizeMin(const cVector3& size)
{
	resizeMinX(size.x);
	resizeMinY(size.y);
	resizeMinZ(size.z);
}


inline float cRect3d::midX()const
{
	return (x0+x1)*0.5f;
}


inline float cRect3d::midY()const
{
	return (y0+y1)*0.5f;
}


inline float cRect3d::midZ()const
{
	return (z0+z1)*0.5f;
}


inline cVector3 cRect3d::midpoint()const
{
	return cVector3(midX(), midY(), midZ());
}


inline float cRect3d::sizeX()const
{
	return (x1-x0);
}


inline float cRect3d::sizeY()const
{
	return (y1-y0);
}


inline float cRect3d::sizeZ()const
{
	return (z1-z0);
}


inline cVector3 cRect3d::size()const
{
	return cVector3(sizeX(), sizeY(), sizeZ());
}


inline cVector3 cRect3d::minPoint()const
{
	return cVector3(x0, y0, z0);
}


inline cVector3 cRect3d::maxPoint()const
{
	return cVector3(x1, y1, z1);
}


inline float cRect3d::area()const
{
	return(sizeX()*sizeY()*sizeZ());
}


inline void cRect3d::unionPoint(const cVector3& point)
{
	x0 = minimum(x0, point.x);
	y0 = minimum(y0, point.y);
	z0 = minimum(z0, point.z);

	x1 = maximum(x1, point.x);
	y1 = maximum(y1, point.y);
	z1 = maximum(z1, point.z);
}


inline void cRect3d::unionRect(const cRect3d& rect)
{
	assertValid();
	rect.assertValid();

	x0 = minimum(x0, rect.x0);
	y0 = minimum(y0, rect.y0);
	z0 = minimum(z0, rect.z0);

	x1 = maximum(x1, rect.x1);
	y1 = maximum(y1, rect.y1);
	z1 = maximum(z1, rect.z1);
}


inline void cRect3d::offset(const cVector3 &offset)
{
	x0 += offset.x;
	x1 += offset.x;
	y0 += offset.y;
	y1 += offset.y;
	z0 += offset.z;
	z1 += offset.z;
}



inline void cRect3d::expand(float n)
{
	x0 -= n;
	x1 += n;
	y0 -= n;
	y1 += n;
	z0 -= n;
	z1 += n;
}


inline void cRect3d::expandX(float n)
{
	x0 -= n;
	x1 += n;
}


inline void cRect3d::expandY(float n)
{
	y0 -= n;
	y1 += n;
}


inline void cRect3d::expandZ(float n)
{
	z0 -= n;
	z1 += n;
}


inline void cRect3d::expand(const cVector3& size)
{
	expandX(size.x);
	expandY(size.y);
	expandZ(size.z);
}



inline void cRect3d::normalize()
{
	if(x0 > x1)
	{
		swap(x0, x1);
	}
	if(y0 > y1)
	{
		swap(y0, y1);
	}
	if(z0 > z1)
	{
		swap(z0, z1);
	}
}



inline cVector3 cRect3d::corner(int index)const
{
	debug_assert(index>=0 && index <8, "invalid index");

	return cVector3( (index & 1) ? x0: x1,
		(index & 2) ? y0: y1,
		(index & 4) ? z0: z1);
}


inline void cRect3d::transform(const cMatrix4x4* matrix)
{
	cVector3 basePoint(x0,y0,z0);
	cVector3 xLeg(x1,y0,z0);
	cVector3 yLeg(x0,y1,z0);
	cVector3 zLeg(x0,y0,z1);

	D3DXVec3TransformCoord(&basePoint, &basePoint, matrix);
	D3DXVec3TransformCoord(&xLeg, &xLeg, matrix);
	D3DXVec3TransformCoord(&yLeg, &yLeg, matrix);
	D3DXVec3TransformCoord(&zLeg, &zLeg, matrix);

	xLeg -= basePoint;
	yLeg -= basePoint;
	zLeg -= basePoint;

	cVector3 farPoint(basePoint + xLeg + yLeg + zLeg);

	set(	basePoint.x,farPoint.x,
						basePoint.y,farPoint.y,
						basePoint.z,farPoint.z);

	unionPoint(basePoint + xLeg);
	unionPoint(basePoint + yLeg);
	unionPoint(basePoint + zLeg);
	unionPoint(basePoint + xLeg + yLeg);
	unionPoint(basePoint + yLeg + zLeg);
	unionPoint(basePoint + zLeg + xLeg);
}

inline bool cRect3d::pointInRect(const cVector3& point)const
{
	return point.x >= x0
		&& point.y >= y0
		&& point.z >= z0
		&& point.x <= x1
		&& point.y <= y1
		&& point.z <= z1;
}

inline cSphere cRect3d::createBoundingSphere()const
{
	return cSphere(
		midpoint(), 
		(sizeX()+sizeY()+sizeZ())*0.5f);
}

//- End of cRect3d ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRect3d.h )

//----------------------------------------------------------
//$Log: $