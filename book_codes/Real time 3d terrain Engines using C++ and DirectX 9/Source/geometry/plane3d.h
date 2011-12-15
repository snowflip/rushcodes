#ifndef PLANE3D_H
#define PLANE3D_H
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
#include <geometry/plane2d.h>
#include <geometry/rect3d.h>

//	Name Space declaration
namespace gaia
{


/*	cPlane3d
------------------------------------------------------------------------------------------
	
	A cPlane3d in 3D Space represented in point-normal form (Ax + By + Cz + D = 0).

	The convention for the distance constant D is:

	D = -(A, B, C) dot (X, Y, Z)

------------------------------------------------------------------------------------------
*/

class cPlane3d
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	union
	{
		float n[4];
		struct
		{
			float		distance;
			cVector3	normal;
		};
		struct
		{
			cPlane2d	plane2d;
			float		z;
		};
	};

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------
    cPlane3d();
    cPlane3d(const cPlane3d& cPlane3d);
    cPlane3d(const cVector3& Point0, const cVector3& Point1, const cVector3& Point2);
    cPlane3d(const cVector3& Normal, float Distance);
    cPlane3d(const cVector3& Point, const cVector3& Normal);
    ~cPlane3d();

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------
	cPlane3d& operator=( const cPlane3d& Src );
	bool operator==( const cPlane3d& PlaneA);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
    void set(const cVector3& Point0, const cVector3& Point1, const cVector3& Point2);
    void set(const cVector3& Normal, float Distance);
    void set(const cVector3& Point, const cVector3& Normal);

	void normalize();
	void xForm(const cMatrix4x4& matrix);

	//--------------------------------------------------
	// Point Operations...
	//--------------------------------------------------
    float signedDistance(const cVector3& Point) const;
	float solveForX(float Y, float Z)const;
	float solveForY(float X, float Z)const;
	float solveForZ(float X, float Y)const;

	cVector3 projectPointToPlane(const cVector3& Point)const;
};

//- Inline Functions ---------------------------------------------------------------------

//= CREATORS =============================================================================

/*	cPlane3d
------------------------------------------------------------------------------------------
	
	Default Constructor
	
------------------------------------------------------------------------------------------
*/
inline cPlane3d::cPlane3d()
{
}

/*	cPlane3d
------------------------------------------------------------------------------------------
	
	Default Copy Constructor
	
------------------------------------------------------------------------------------------
*/
inline cPlane3d::cPlane3d(const cPlane3d& Rhs)
:normal(Rhs.normal)
,distance(Rhs.distance)
{
}


inline cPlane3d::cPlane3d(const cVector3& Point0, const cVector3& Point1, const cVector3& Point2)
{
    set(Point0, Point1, Point2);
}

inline cPlane3d::cPlane3d(const cVector3& Normal, float Distance)
:normal(Normal)
,distance(Distance)
{
}

inline cPlane3d::cPlane3d(const cVector3& Point, const cVector3& Normal)
{
    set(Point, Normal);
}

inline cPlane3d::~cPlane3d()
{
}

//= OPERATORS ============================================================================


inline cPlane3d& cPlane3d::operator=( const cPlane3d& Src )
{
	normal = Src.normal;
    distance = Src.distance;

	return (*this);
}

inline bool cPlane3d::operator==(const cPlane3d& PlaneB )
{
	return (normal == PlaneB.normal && distance==PlaneB.distance);
}

//= MUTATORS =============================================================================

inline void cPlane3d::normalize()
{
	float mag = normal.length();
	float inv_mag = 1.0f/mag;

	normal.x *= inv_mag;
	normal.y *= inv_mag;
	normal.z *= inv_mag;
	distance *= inv_mag;
}

//:	solveForX
//----------------------------------------------------------------------------------------
//
//	Given Z and Y, solve for X on the plane 
//
//-------------------------------------------------------------------------------------://
inline float cPlane3d::solveForX(float Y, float Z)const
{
	//Ax + By + Cz + D = 0
	// Ax = -(By + Cz + D)
	// x = -(By + Cz + D)/A

	if (normal.x)
	{
		return ( -(normal.y*Y + normal.z*Z + distance) / normal.x );
	}

	return (0.0f);
}

//:	solveForY
//----------------------------------------------------------------------------------------
//
//	Given X and Z, solve for Y on the plane 
//
//-------------------------------------------------------------------------------------://
inline float cPlane3d::solveForY(float X, float Z)const
{
	//Ax + By + Cz + D = 0
	// By = -(Ax + Cz + D)
	// y = -(Ax + Cz + D)/B

	if (normal.y)
	{
		return ( -(normal.x*X + normal.z*Z + distance) / normal.y );
	}

	return (0.0f);
}

//:	solveForZ
//----------------------------------------------------------------------------------------
//
//	Given X and Y, solve for Z on the plane 
//
//-------------------------------------------------------------------------------------://
inline float cPlane3d::solveForZ(float X, float Y)const
{
	//Ax + By + Cz + D = 0
	// Cz = -(Ax + By + D)
	// z = -(Ax + By + D)/C

	if (normal.z)
	{
		return ( -(normal.x*X + normal.y*Y + distance) / normal.z);
	}

	return (0.0f);
}

//:	projectPointToPlane
//----------------------------------------------------------------------------------------
//
//	Given a 3D point in space, project the point onto this plane along the plane normal
//
//-------------------------------------------------------------------------------------://
inline cVector3 cPlane3d::projectPointToPlane(const cVector3& Point)const
{
	float distance = signedDistance(Point);
	cVector3 result((normal.x * -distance) + Point.x,
					(normal.y * -distance) + Point.y,
					(normal.z * -distance) + Point.z);

	return(result);
}

//:	set
//----------------------------------------------------------------------------------------
//
//	Setup cPlane3d object given a clockwise ordering of 3D points 
//
//-------------------------------------------------------------------------------------://
inline void cPlane3d::set(const cVector3& Point0, const cVector3& Point1, const cVector3& Point2)
{
	cVector3 SideA(Point1 - Point0);
	cVector3 SideB(Point2 - Point0);
	cVector3 norm;
    norm.crossProduct(SideB, SideA);
    norm.normalize();
    cVector3 point(Point0.x, Point0.y, Point0.z);
    normal.set(norm.x, norm.y, norm.z);


    distance = -normal.dotProduct(point);
}

inline void cPlane3d::set(const cVector3& Normal, float Distance)
{
	normal=Normal;
	distance=Distance;
}

inline void cPlane3d::set(const cVector3& Point, const cVector3& Normal)
{
    cVector3 point(Point.x, Point.y, Point.z);
    normal.set(Normal.x, Normal.y, Normal.z);

    distance = -normal.dotProduct(point);
}

inline void cPlane3d::xForm(const cMatrix4x4& matrix)
{
	D3DXVec3TransformNormal(&normal, &normal, &matrix);
	normal.normalize();

    cVector3 point = normal * distance;
	D3DXVec3TransformCoord(&point, &point, &matrix);
    distance = -normal.dotProduct(point);
}


/*	signedDistance
--------------------------------------------
	
	Returns the signed distance between 
	the plane and the provided 3D point. 
	Negative distances are "behind" the 
	plane, i.e. in the opposite direction 
	of the plane normal.
	
--------------------------------------------
*/
inline float cPlane3d::signedDistance(
	const cVector3& Point) const
{
    return(normal.dotProduct(Point) + distance);
}

//- End of cPlane3d -------------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia


#endif  // end of file      ( cPlane3d.h )

