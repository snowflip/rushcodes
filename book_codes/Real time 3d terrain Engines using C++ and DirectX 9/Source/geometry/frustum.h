#ifndef FRUSTUM_H
#define FRUSTUM_H
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
#include <geometry/plane3d.h>
#include <geometry/rect3d.h>
#include <geometry/sphere.h>
#include <geometry/intersection_tests.h>

//	Name Space declaration
namespace gaia
{


/*	cFrustum
-----------------------------------------------------------------
	
	A cFrustum is a set of six cPlane3d objects representing 
	camera space. These planes are extracted from a camera
	matrix directly. 

	NOTE: The planes of a cFrustum object are not normalized!
	This means they are only sutable for half-space testing.
	No distance values calcualted using these planes will be
	accurate other than to show whether positions lie in the 
	positive or negative half-space of the plane.

-----------------------------------------------------------------
*/

class cFrustum
{
public:

	//--------------------------------------------------
	// Public Data...
	//--------------------------------------------------
	cPlane3d leftPlane;
	cPlane3d rightPlane;
	cPlane3d topPlane;
	cPlane3d bottomPlane;
	cPlane3d nearPlane;
	cPlane3d farPlane;

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------
    cFrustum();
    cFrustum( const cFrustum& Src );
    ~cFrustum();

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------
	cFrustum& operator=( const cFrustum& Src );
	bool operator==( const cFrustum& Src);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
	void extractFromMatrix(
		const cMatrix4x4& matrix,
		bool normalizePlanes = false);

 	//--------------------------------------------------
	// Test Operations...
	//--------------------------------------------------
	bool testPoint(const cVector3& point) const;
	bool testRect(const cRect3d& rect) const;
	bool testSphere(const cSphere& sphere) const;
};

//- Inline Functions ---------------------------------------------------------------------

//= CREATORS =============================================================================

/*	cFrustum
------------------------------------------------------------------------------------------
	
	Default Constructor
	
------------------------------------------------------------------------------------------
*/
inline cFrustum::cFrustum()
{
}

/*	cFrustum
------------------------------------------------------------------------------------------
	
	Default Copy Constructor
	
------------------------------------------------------------------------------------------
*/
inline cFrustum::cFrustum(const cFrustum& src)
:leftPlane(src.leftPlane)
,rightPlane(src.rightPlane)
,topPlane(src.topPlane)
,bottomPlane(src.bottomPlane)
,nearPlane(src.nearPlane)
,farPlane(src.farPlane)
{
}


inline cFrustum::~cFrustum()
{
}

//= OPERATORS ============================================================================


inline cFrustum& cFrustum::operator=( const cFrustum& src )
{
	leftPlane = src.leftPlane;
	rightPlane = src.rightPlane;
	topPlane = src.topPlane;
	bottomPlane = src.bottomPlane;
	nearPlane = src.nearPlane;
	farPlane = src.farPlane;

	return (*this);
}

inline bool cFrustum::operator==(const cFrustum& src )
{
	return (leftPlane == src.leftPlane)
		&&(rightPlane == src.rightPlane)
		&&(topPlane == src.topPlane)
		&&(bottomPlane == src.bottomPlane)
		&&(nearPlane == src.nearPlane)
		&&(farPlane == src.farPlane);
}

//= MUTATORS =============================================================================

inline void cFrustum::extractFromMatrix(
	const cMatrix4x4& matrix,
	bool normalizePlanes)
{
	// Left clipping plane 
	leftPlane.normal.x = matrix._14 + matrix._11; 
	leftPlane.normal.y = matrix._24 + matrix._21; 
	leftPlane.normal.z = matrix._34 + matrix._31; 
	leftPlane.distance = matrix._44 + matrix._41;

	// Right clipping plane 
	rightPlane.normal.x = matrix._14 - matrix._11; 
	rightPlane.normal.y = matrix._24 - matrix._21; 
	rightPlane.normal.z = matrix._34 - matrix._31; 
	rightPlane.distance = matrix._44 - matrix._41;

	// Top clipping plane 
	topPlane.normal.x = matrix._14 - matrix._12; 
	topPlane.normal.y = matrix._24 - matrix._22; 
	topPlane.normal.z = matrix._34 - matrix._32; 
	topPlane.distance = matrix._44 - matrix._42;

	// Bottom clipping plane 
	bottomPlane.normal.x = matrix._14 + matrix._12; 
	bottomPlane.normal.y = matrix._24 + matrix._22; 
	bottomPlane.normal.z = matrix._34 + matrix._32; 
	bottomPlane.distance = matrix._44 + matrix._42;

	// Near clipping plane 
	nearPlane.normal.x = matrix._13; 
	nearPlane.normal.y = matrix._23; 
	nearPlane.normal.z = matrix._33; 
	nearPlane.distance = matrix._43;

	// Far clipping plane 
	farPlane.normal.x = matrix._14 - matrix._13; 
	farPlane.normal.y = matrix._24 - matrix._23; 
	farPlane.normal.z = matrix._34 - matrix._33; 
	farPlane.distance = matrix._44 - matrix._43; 

	// it is not always nessesary to normalize
	// the planes of the frustum. Non-normalized
	// planes can still be used for basic
	// intersection tests.
	if (normalizePlanes)
	{
		leftPlane.normalize();
		rightPlane.normalize();
		topPlane.normalize();
		bottomPlane.normalize();
		nearPlane.normalize();
		farPlane.normalize();
	}
}

//= TESTS =============================================================================
inline bool cFrustum::testPoint(const cVector3& point) const
{
	if ((leftPlane.signedDistance(point)		< 0.0f)
		|| (rightPlane.signedDistance(point)	< 0.0f)
		|| (topPlane.signedDistance(point)		< 0.0f)
		|| (bottomPlane.signedDistance(point)	< 0.0f)
		|| (nearPlane.signedDistance(point)		< 0.0f)
		|| (farPlane.signedDistance(point)		< 0.0f))
	{
		return false;
	}

	return true;
}

inline bool cFrustum::testRect(
	const cRect3d& rect) const
{

	if ((planeClassify(rect, leftPlane)		
			== k_plane_back)
		|| (planeClassify(rect, rightPlane) 
			== k_plane_back)
		|| (planeClassify(rect, topPlane)	
			== k_plane_back)
		|| (planeClassify(rect, bottomPlane)
			== k_plane_back)
		|| (planeClassify(rect, nearPlane)	
			== k_plane_back)
		|| (planeClassify(rect, farPlane)	
			== k_plane_back))
	{
		return false;
	}
	
	return true;
}

inline bool cFrustum::testSphere(const cSphere& sphere) const
{

	if ((planeClassify(sphere, leftPlane)		== k_plane_back)
		|| (planeClassify(sphere, rightPlane)	== k_plane_back)
		|| (planeClassify(sphere, topPlane)		== k_plane_back)
		|| (planeClassify(sphere, bottomPlane)	== k_plane_back)
		|| (planeClassify(sphere, nearPlane)	== k_plane_back)
		|| (planeClassify(sphere, farPlane)		== k_plane_back))
	{
		return false;
	}
	
	return true;
}

//- End of cFrustum -------------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia


#endif  // end of file      ( cFrustum.h )

