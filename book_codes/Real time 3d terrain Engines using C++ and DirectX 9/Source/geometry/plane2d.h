#ifndef PLANE2D_H
#define PLANE2D_H
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


class cPlane2d
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	union
	{
		float n[3];
		struct
		{
			float		distance;
			cVector2	normal;
		};
	};

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------
    cPlane2d();
    cPlane2d(const cPlane2d& cPlane2d);
    cPlane2d(const cVector2& Point0, const cVector2& Point1);
    cPlane2d(const cVector2& Normal, float Distance);
    ~cPlane2d();

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------
	cPlane2d& operator=( const cPlane2d& Src );
	bool operator==( const cPlane2d& PlaneA);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
    void set(const cVector2& Point0, const cVector2& Point1);
    void set(const cVector2& Normal, float Distance);

	//--------------------------------------------------
	// Point Operations...
	//--------------------------------------------------
    float signedDistance(const cVector2& Point) const;
	float solveForX(float Y, float Z)const;
	float solveForY(float X, float Z)const;
	float solveForZ(float X, float Y)const;

	cVector2 projectPointToPlane(const cVector2& Point)const;
};

//- Inline Functions ---------------------------------------------------------------------

//= CREATORS =============================================================================

/*	cPlane2d
------------------------------------------------------------------------------------------
	
	Default Constructor
	
------------------------------------------------------------------------------------------
*/
inline cPlane2d::cPlane2d()
{
}

/*	cPlane2d
------------------------------------------------------------------------------------------
	
	Default Copy Constructor
	
------------------------------------------------------------------------------------------
*/
inline cPlane2d::cPlane2d(const cPlane2d& Rhs)
:normal(Rhs.normal)
,distance(Rhs.distance)
{
}


inline cPlane2d::cPlane2d(const cVector2& Point0, const cVector2& Point1)
{
    set(Point0, Point1);
}

inline cPlane2d::cPlane2d(const cVector2& Normal, float Distance)
:normal(Normal)
,distance(Distance)
{
}

inline cPlane2d::~cPlane2d()
{
}

//= OPERATORS ============================================================================


inline cPlane2d& cPlane2d::operator=( const cPlane2d& Src )
{
	normal = Src.normal;
    distance = Src.distance;

	return (*this);
}

inline bool cPlane2d::operator==(const cPlane2d& PlaneB )
{
	return (normal == PlaneB.normal && distance==PlaneB.distance);
}

//= MUTATORS =============================================================================

//:	solveForX
//----------------------------------------------------------------------------------------
//
//	Given Z and Y, solve for X on the plane 
//
//-------------------------------------------------------------------------------------://
inline float cPlane2d::solveForX(float Y, float Z)const
{
	//Ax + By + Cz + D = 0
	// Ax = -(By + Cz + D)
	// x = -(By + Cz + D)/A

	if (normal.x)
	{
		return ( -(normal.y*Y + distance) / normal.x );
	}

	return (0.0f);
}

//:	solveForY
//----------------------------------------------------------------------------------------
//
//	Given X and Z, solve for Y on the plane 
//
//-------------------------------------------------------------------------------------://
inline float cPlane2d::solveForY(float X, float Z)const
{
	//Ax + By + Cz + D = 0
	// By = -(Ax + Cz + D)
	// y = -(Ax + Cz + D)/B

	if (normal.y)
	{
		return ( -(normal.x*X + distance) / normal.y );
	}

	return (0.0f);
}



//:	projectPointToPlane
//----------------------------------------------------------------------------------------
//
//	Given a 3D point in space, project the point onto this plane along the plane normal
//
//-------------------------------------------------------------------------------------://
inline cVector2 cPlane2d::projectPointToPlane(const cVector2& Point)const
{
	float distance = signedDistance(Point);
	cVector2 result((normal.x * -distance) + Point.x,
					(normal.y * -distance) + Point.y);

	return(result);
}

//:	set
//----------------------------------------------------------------------------------------
//
//	Setup cPlane2d object given a clockwise ordering of 3D points 
//
//-------------------------------------------------------------------------------------://
inline void cPlane2d::set(const cVector2& Point0, const cVector2& Point1)
{
	cVector2 line(Point1 - Point0);
    line.normalize();
    cVector2 point(Point0.x, Point0.y);
    normal.set(-line.y, line.x);


    distance = -normal.dotProduct(point);
}

inline void cPlane2d::set(const cVector2& Normal, float Distance)
{
	normal=Normal;
	distance=Distance;
}


/*	signedDistance
------------------------------------------------------------------------------------------
	
	Returns the signed distance between the plane and the provided 3D point. Negative
	distances are "behind" the plane, i.e. in the opposite direction of the plane normal.
	
------------------------------------------------------------------------------------------
*/
inline float cPlane2d::signedDistance(const cVector2& Point) const
{
    return(normal.dotProduct(Point) + distance);
}


//- End of cPlane2d -------------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia


#endif  // end of file      ( cPlane2d.h )

