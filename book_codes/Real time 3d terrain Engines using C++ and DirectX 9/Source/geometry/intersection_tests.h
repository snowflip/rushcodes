#ifndef INTERSECTIONTESTS_H
#define INTERSECTIONTESTS_H
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
#include "rect2d.h"
#include "rect3d.h"
#include "plane2d.h"
#include "plane3d.h"
#include "circle.h"
#include "sphere.h"
#include "lines.h"


//	Name Space declaration
namespace gaia
{


enum eVolumeClassifications
{
	NO_RELATION=0,
	EQUAL,
	A_CONTAINS_B,
	B_CONTAINS_A,
	INTERSECTING,
};


enum ePlaneClassifications
{
	k_plane_front = 0,
	k_plane_back,
	k_plane_intersect
};

/*	Intersection Tests
-----------------------------------------------------------------
    
    The functions provide all possible intersection tests 
	between lines, planes and volumetric shapes in 2D and 3D
    
-----------------------------------------------------------------
*/

// testing for intersection between a
// 3D plane and a 3D ray 
inline bool intersect(const cPlane3d& plane, 
				const cRay3d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	float NdotV = dotProduct(plane.normal, ray.normal);

	if (NdotV != 0.0f)
	{
		*pResult = dotProduct(plane.normal, ray.point) 
			+ plane.distance;
		*pResult /= NdotV;
		return (*pResult >= 0.0f);
	}
	
	return false;
}

// testing for intersection between a
// 2D plane and a 2D ray 
inline bool intersect(const cPlane2d& plane, 
				const cRay2d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	float NdotV = dotProduct(plane.normal, ray.normal);

	if (NdotV != 0.0f)
	{
		*pResult = dotProduct(plane.normal, ray.point) 
			+ plane.distance;
		*pResult /= NdotV;
		return (*pResult >= 0.0f);
	}
	
	return false;
}

// testing for intersection between a
// 3D sphere and a 3D ray 
inline bool intersect(const cSphere& sphere, 
				const cRay3d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	cVector3 Q(ray.point.x - (float)sphere.center.x,
				ray.point.y - (float)sphere.center.y,
				ray.point.z - (float)sphere.center.z);

	float c = Q.lengthSquared() 
				- (float)Square(sphere.radius);
	
	if (c < 0.0f)
	{
		*pResult = 0.0f;
		return (true);
	}

	float b = Q.dotProduct(ray.normal);

	if (b > 0.0f)
	{
		return (false);
	}

	// rays are assumed to be normalized
	// so we do not need to calc a
	float a = 1.0f; 

	float d = Square(b) - (4.0f*a*c);

	if (d < 0.0f)
	{
		return (false);
	}

	*pResult = (-b - SquareRoot(d))/a;

	return (*pResult>=0.0f);
}

// testing for intersection between a
// 2D circle and a 2D ray 
inline bool intersect(const cCircle& sphere, 
				const cRay2d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	cVector2 Q(ray.point.x - (float)sphere.center.x,
				ray.point.y - (float)sphere.center.y);

	float c = Q.lengthSquared() 
				- (float)Square(sphere.radius);
	
	if (c < 0.0f)
	{
		*pResult = 0.0f;
		return (true);
	}

	float b = Q.dotProduct(ray.normal);

	if (b > 0.0f)
	{
		return (false);
	}

	// rays are assumed to be normalized
	// so we do not need to calc a
	float a = 1.0f; 
	
	float d = Square(b) - (4.0f*a*c);

	if (d < 0.0f)
	{
		return (false);
	}

	*pResult = (-b - SquareRoot(d))/a;

	return (*pResult>=0.0f);
}

// testing for intersection between a
// 3D rectangle and a 3D ray 

inline bool intersect(const cRect3d& rect, 
				const cRay3d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	// determine if the ray begins in the box
	if (ray.point.x >= (float)rect.x0
		&& ray.point.x <= (float)rect.x1
		&& ray.point.y >= (float)rect.y0
		&& ray.point.y <= (float)rect.y1
		&& ray.point.z >= (float)rect.z0
		&& ray.point.z <= (float)rect.z1)
	{
		*pResult = 0.0f;
		return (true);
	}


	// test the proper sides of the rectangle
	float maxT = MIN_REAL32;
	float t;

	if (ray.normal.x > 0.0f)
	{
		t = (rect.x0 - ray.point.x)/ray.normal.x;
		maxT = maximum(maxT,t);
	}
	else if (ray.normal.x < 0.0f)
	{
		t = (rect.x1 - ray.point.x)/ray.normal.x;
		maxT = maximum(maxT,t);
	}

	if (ray.normal.y > 0.0f)
	{
		t = (rect.y0 - ray.point.y)/ray.normal.y;
		maxT = maximum(maxT,t);
	}
	else if (ray.normal.y < 0.0f)
	{
		t = (rect.y1 - ray.point.y)/ray.normal.y;
		maxT = maximum(maxT,t);
	}

	if (ray.normal.z > 0.0f)
	{
		t = (rect.z0 - ray.point.z)/ray.normal.z;
		maxT = maximum(maxT,t);
	}
	else if (ray.normal.z < 0.0f)
	{
		t = (rect.z1 - ray.point.z)/ray.normal.z;
		maxT = maximum(maxT,t);
	}

	*pResult = maxT;

	return (*pResult>=0.0f);
}

// testing for intersection between a
// 2D rectangle and a 2D ray 
inline bool intersect(const cRect2d& rect, 
				const cRay2d& ray, 
				float* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	// determine if the ray begins in the box
	if (ray.point.x >= (float)rect.x0
		&& ray.point.x <= (float)rect.x1
		&& ray.point.y >= (float)rect.y0
		&& ray.point.y <= (float)rect.y1)
	{
		*pResult = 0.0f;
		return (true);
	}


	// test the proper sides of the rectangle
	float maxT = MIN_REAL32;
	float t;

	if (ray.normal.x > 0.0f)
	{
		t = (rect.x0 - ray.point.x)/ray.normal.x;
		maxT = maximum(maxT,t);
	}
	else if (ray.normal.x < 0.0f)
	{
		t = (rect.x1 - ray.point.x)/ray.normal.x;
		maxT = maximum(maxT,t);
	}

	if (ray.normal.y > 0.0f)
	{
		t = (rect.y0 - ray.point.y)/ray.normal.y;
		maxT = maximum(maxT,t);
	}
	else if (ray.normal.y < 0.0f)
	{
		t = (rect.y1 - ray.point.y)/ray.normal.y;
		maxT = maximum(maxT,t);
	}

	*pResult = maxT;

	return (*pResult>=0.0f);
}

// testing for intersection between a
// 2D circle and a 2D plane 
inline int planeClassify(const cCircle& circle, 
				const cPlane2d& plane)
{
	float d = plane.signedDistance(circle.center);

	if (absoluteValue(d) < circle.radius)
	{
		return k_plane_intersect;
	}
	else if (d)
	{
		return k_plane_front;
	}

	return k_plane_back;
}

// testing for intersection between a
// 3D sphere and a 3D plane 
inline int planeClassify(const cSphere& sphere, 
				const cPlane3d& plane)
{
	float d = plane.signedDistance(sphere.center);

	if (absoluteValue(d) < sphere.radius)
	{
		return k_plane_intersect;
	}
	else if (d)
	{
		return k_plane_front;
	}

	return k_plane_back;
}



inline int planeClassify(const cRect2d& rect, 
				const cPlane2d& plane)
{
	cVector2 minPoint, maxPoint;

	// build two points based on the direction
	// of the plane vector. minPoint 
	// and maxPoint are the two points
	// on the rectangle furthest away from
	// each other along the plane normal

	if (plane.normal.x > 0.0f)
	{
		minPoint.x = (float)rect.x0;
		maxPoint.x = (float)rect.x1;
	}
	else
	{
		minPoint.x = (float)rect.x1;
		maxPoint.x = (float)rect.x0;
	}

	if (plane.normal.y > 0.0f)
	{
		minPoint.y = (float)rect.y0;
		maxPoint.y = (float)rect.y1;
	}
	else
	{
		minPoint.y = (float)rect.y1;
		maxPoint.y = (float)rect.y0;
	}

	// compute the signed distance from 
	// the plane to both points
	float dmin = plane.signedDistance(minPoint);
	float dmax = plane.signedDistance(minPoint);

	// the rectangle intersects the plane if
	// one value is positive and the other is negative
	if (dmin * dmax < 0.0f)
	{
		return k_plane_intersect;
	}
	else if (dmin)
	{
		return k_plane_front;
	}

	return k_plane_back;
}


inline int planeClassify(
	const cRect3d& rect, 
	const cPlane3d& plane)
{
	cVector3 minPoint, maxPoint;

	// build two points based on the direction
	// of the plane vector. minPoint 
	// and maxPoint are the two points
	// on the rectangle furthest away from
	// each other along the plane normal

	if (plane.normal.x > 0.0f)
	{
		minPoint.x = (float)rect.x0;
		maxPoint.x = (float)rect.x1;
	}
	else
	{
		minPoint.x = (float)rect.x1;
		maxPoint.x = (float)rect.x0;
	}

	if (plane.normal.y > 0.0f)
	{
		minPoint.y = (float)rect.y0;
		maxPoint.y = (float)rect.y1;
	}
	else
	{
		minPoint.y = (float)rect.y1;
		maxPoint.y = (float)rect.y0;
	}

	if (plane.normal.z > 0.0f)
	{
		minPoint.z = (float)rect.z0;
		maxPoint.z = (float)rect.z1;
	}
	else
	{
		minPoint.z = (float)rect.z1;
		maxPoint.z = (float)rect.z0;
	}

	// compute the signed distance from 
	// the plane to both points
	float dmin = plane.signedDistance(minPoint);
	float dmax = plane.signedDistance(minPoint);

	// the rectangle intersects the plane if
	// one value is positive and the other is negative
	if (dmin * dmax < 0.0f)
	{
		return k_plane_intersect;
	}
	else if (dmin)
	{
		return k_plane_front;
	}

	return k_plane_back;
}


// testing for intersection between two
// 2D spheres of any data type
inline bool intersect(const cCircle& sphereA, 
				const cCircle& sphereB)
{
	cVector2 center_delta(
		(sphereA.center.x - sphereB.center.x),
		(sphereA.center.y - sphereB.center.y));

	float distance_squared = 
			dotProduct(center_delta,center_delta);

	return distance_squared < 
			Square(sphereA.radius+sphereB.radius);
}


// testing for intersection between two
// 3D spheres of any data type
inline bool intersect(const cSphere& sphereA, 
				const cSphere& sphereB)
{
	cVector3 center_delta(
		(sphereA.center.x - sphereB.center.x),
		(sphereA.center.y - sphereB.center.y),
		(sphereA.center.z - sphereB.center.z));

	float distance_squared = 
		dotProduct(center_delta,center_delta);

	return distance_squared < 
		Square(sphereA.radius+sphereB.radius);
}

// testing for intersection between a
// 2D rectangle and a 2D sphere 
// of any data type

inline bool intersect(const cRect2d& rect, 
				const cCircle& sphere)
{
	cVector2 offset(0.0f,0.0f);
	int interior_count = 0;

    if (sphere.center.x < rect.x0)
    {
        offset.x = rect.x0 - sphere.center.x;
    }
    else if (sphere.center.x >= rect.x1)
    {
        offset.x = sphere.center.x - rect.x1;
    }
	else
	{
		++interior_count;
	}

    if (sphere.center.y < rect.y0)
    {
        offset.y = rect.y0 - sphere.center.y;
    }
    else if (sphere.center.y >= rect.y1)
    {
        offset.y = sphere.center.y - rect.y1;
    }
	else
	{
		++interior_count;
	}

	// test if the center of the 
	// sphere is inside the rectangle
	if (interior_count==2)
	{
		return true;
	}

	float distance_squared = dotProduct(offset,offset);
	float radius_squared = sphere.radius*sphere.radius;

	return (distance_squared < radius_squared);
}

// testing for intersection between a
// 3D rectangle and a 3D sphere 
// of any data type

inline bool intersect(const cRect3d& rect, 
				const cSphere& sphere)
{
	cVector3 offset(0.0f,0.0f,0.0f);
	int interior_count = 0;

    if (sphere.center.x < rect.x0)
    {
        offset.x = rect.x0 - sphere.center.x;
    }
    else if (sphere.center.x >= rect.x1)
    {
        offset.x = sphere.center.x - rect.x1;
    }
	else
	{
		++interior_count;
	}

    if (sphere.center.y < rect.y0)
    {
        offset.y = rect.y0 - sphere.center.y;
    }
    else if (sphere.center.y >= rect.y1)
    {
        offset.y = sphere.center.y - rect.y1;
    }
	else
	{
		++interior_count;
	}

    if (sphere.center.z < rect.z0)
    {
        offset.z = rect.z0 - sphere.center.z;
    }
    else if (sphere.center.z >= rect.z1)
    {
        offset.z = sphere.center.z - rect.z1;
    }
	else
	{
		++interior_count;
	}

	// test if the center of the 
	// sphere is inside the rectangle
	if (interior_count==3)
	{
		return true;
	}

	float distance_squared = dotProduct(offset,offset);
	float radius_squared = sphere.radius*sphere.radius;

	return (distance_squared < radius_squared);
}

// testing for intersection between two
// 2D rectangles of any data type
// returning the resulting rectangle
// of intersection

inline bool intersect(const cRect2d& rectA, 
				const cRect2d& rectB,
				cRect2d* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	pResult->x0 = maximum(rectA.x0, rectB.x0);
	pResult->x1 = minimum(rectA.x1, rectB.x1);

	if (pResult->x0 < pResult->x1)
	{
		pResult->y0 = maximum(rectA.y0, rectB.y0);
		pResult->y1 = minimum(rectA.y1, rectB.y1);

		if (pResult->y0 < pResult->y1)
		{
			return true;
		}
	}

	return false;
}

// testing for intersection between two
// 3D rectangles of any data type
// returning the resulting rectangle
// of intersection

inline bool intersect(const cRect3d& rectA, 
				const cRect3d& rectB,
				cRect3d* pResult)
{
	debug_assert(pResult, "a result address must be provided");

	pResult->x0 = maximum(rectA.x0, rectB.x0);
	pResult->x1 = minimum(rectA.x1, rectB.x1);

	if (pResult->x0 < pResult->x1)
	{
		pResult->y0 = maximum(rectA.y0, rectB.y0);
		pResult->y1 = minimum(rectA.y1, rectB.y1);

		if (pResult->y0 < pResult->y1)
		{
			pResult->z0 = maximum(rectA.z0, rectB.z0);
			pResult->z1 = minimum(rectA.z1, rectB.z1);

			if (pResult->z0 < pResult->z1)
			{
				return true;
			}
		}
	}

	return false;
}


inline bool intersectRect3d(const cRect3d& a, const cRect3d& b, cRect3d& result)
{
	result.x0 = maximum(a.x0, b.x0);
	result.y0 = maximum(a.y0, b.y0);
	result.z0 = maximum(a.z0, b.z0);

	result.x1 = minimum(a.x1, b.x1);
	result.y1 = minimum(a.y1, b.y1);
	result.z1 = minimum(a.z1, b.z1);

	return(   (result.x0 <= result.x1)
			&&(result.y0 <= result.y1)
			&&(result.z0 <= result.z1));
}


inline int	classifyRect3d(const cRect3d& a, const cRect3d& b, cRect3d& result)
{
	if (intersectRect3d(a,b, result))
	{
		if (a == b)
		{
			// the two rectangles are equal
			return(EQUAL);
		}
		if (b == result)
		{
			// b is equal to the result, so it is surrounded by a
			return(A_CONTAINS_B);
		}
		if (a == result)
		{
			// a is equal to the result, so it is surrounded by b
			return(B_CONTAINS_A);
		}

		// if none of the above is true, the result is a discreet intersection
		return(INTERSECTING);
	}

	// no itersection takes place
	return(NO_RELATION);
}



inline bool intersectRect2d(const cRect2d& a, const cRect2d& b, cRect2d& result)
{
	result.x0 = maximum(a.x0, b.x0);
	result.y0 = maximum(a.y0, b.y0);

	result.x1 = minimum(a.x1, b.x1);
	result.y1 = minimum(a.y1, b.y1);

	return(   (result.x0 <= result.x1)
			&&(result.y0 <= result.y1));
}

inline int	classifyRect2d(const cRect2d& a, const cRect2d& b, cRect2d& result)
{
	if (intersectRect2d(a,b, result))
	{
		if (a == b)
		{
			// the two rectangles are equal
			return(EQUAL);
		}
		if (b == result)
		{
			// b is equal to the result, so it is surrounded by a
			return(A_CONTAINS_B);
		}
		if (a == result)
		{
			// a is equal to the result, so it is surrounded by b
			return(B_CONTAINS_A);
		}

		// if none of the above is true, the result is a discreet intersection
		return(INTERSECTING);
	}

	// no itersection takes place
	return(NO_RELATION);
}



//- End of IntersectionTests ------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( IntersectionTests.h )

//----------------------------------------------------------
//$Log: $