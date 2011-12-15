#ifndef VECTOR_FUNCTIONS_H
#define VECTOR_FUNCTIONS_H
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
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "vector_operators.h"

//	Name Space declaration
namespace gaia
{

inline void cVector2::set(float _x, float _y)
{
	x = _x;
	y = _y;
}
inline void cVector3::set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
inline void cVector4::set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline void cVector2::clear()
{
	x = 0.0f;
	y = 0.0f;
}
inline void cVector3::clear()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
inline void cVector4::clear()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

inline void cVector2::setToAbsoluteValue()
{
	x = absoluteValue(x);
	y = absoluteValue(y);
}
inline void cVector3::setToAbsoluteValue()
{
	x = absoluteValue(x);
	y = absoluteValue(y);
	z = absoluteValue(z);
}
inline void cVector4::setToAbsoluteValue()
{
	x = absoluteValue(x);
	y = absoluteValue(y);
	z = absoluteValue(z);
	w = absoluteValue(w);
}

inline void cVector2::invert()
{
	x=inverse(x);
	y=inverse(y);
}
inline void cVector3::invert()
{
	x=inverse(x);
	y=inverse(y);
	z=inverse(z);
}
inline void cVector4::invert()
{
	x=inverse(x);
	y=inverse(y);
	z=inverse(z);
	w=inverse(w);
}

inline void cVector2::negate()
{
	x = -x;
	y = -y;
}
inline void cVector3::negate()
{
	x = -x;
	y = -y;
	z = -z;
}
inline void cVector4::negate()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

inline float cVector2::maxValue()const
{
	return maximum(x, y);
}
inline float cVector3::maxValue()const
{
	return maximum(x, maximum(y, z));
}
inline float cVector4::maxValue()const
{
	return maximum(x, maximum(y, maximum (z,w)));
}



inline float cVector2::minValue()const
{
	return minimum(x, y);
}
inline float cVector3::minValue()const
{
	return minimum(x, minimum(y, z));
}
inline float cVector4::minValue()const
{
	return minimum(x, minimum(y, minimum (z,w)));
}


inline bool cVector2::isClear()const
{
	return (x == 0.0f) 
		&& (y == 0.0f);
}
inline bool cVector3::isClear()const
{
	return (x == 0.0f) 
		&& (y == 0.0f) 
		&& (z == 0.0f);
}
inline bool cVector4::isClear()const
{
	return (x == 0.0f) 
		&& (y == 0.0f) 
		&& (z == 0.0f) 
		&& (w == 0.0f);
}


// length and distance calculations

inline void cVector2::normalize()
{
	D3DXVec2Normalize(this, this);
}

inline void cVector3::normalize()
{
	D3DXVec3Normalize(this, this);
}

inline void cVector4::normalize()
{
	D3DXVec4Normalize(this, this);
}


inline float cVector2::length()const
{
	return D3DXVec2Length(this);
}
inline float cVector3::length()const
{
	return D3DXVec3Length(this);
}
inline float cVector4::length()const
{
	return D3DXVec4Length(this);
}


inline float cVector2::lengthSquared()const
{
	return D3DXVec2LengthSq(this);
}
inline float cVector3::lengthSquared()const
{
	return D3DXVec3LengthSq(this);
}
inline float cVector4::lengthSquared()const
{
	return D3DXVec4LengthSq(this);
}


inline float cVector2::distance(const cVector2& vec)const
{
	return SquareRoot((float)distanceSquared(vec));
}
inline float cVector3::distance(const cVector3& vec)const
{
	return SquareRoot((float)distanceSquared(vec));
}
inline float cVector4::distance(const cVector4& vec)const
{
	return SquareRoot((float)distanceSquared(vec));
}



inline float cVector2::distanceSquared(const cVector2& vec)const
{
	cVector2 temp(x-vec.x, y-vec.y); 
	return temp.lengthSquared();
}
inline float cVector3::distanceSquared(const cVector3& vec)const
{
	cVector3 temp(x-vec.x, y-vec.y, z-vec.z); 
	return temp.lengthSquared();
}
inline float cVector4::distanceSquared(const cVector4& vec)const
{
	cVector4 temp(x-vec.x, y-vec.y, z-vec.z, w-vec.w); 
	return temp.lengthSquared();
}


// dot and cross product

inline float cVector2::dotProduct(const cVector2& vec)const
{
	return D3DXVec2Dot(this, &vec);
}
inline float cVector3::dotProduct(const cVector3& vec)const
{
	return D3DXVec3Dot(this, &vec);
}
inline float cVector4::dotProduct(const cVector4& vec)const
{
	return D3DXVec4Dot(this, &vec);
}
inline float cVector4::dotProduct(const cVector3& vec)const
{
	return D3DXVec3Dot((D3DXVECTOR3*)this, &vec);
}


inline float dotProduct(const cVector2& a, const cVector2& b)
{
	return a.dotProduct(b);
}

inline float dotProduct(const cVector3& a, const cVector3& b)
{
	return a.dotProduct(b);
}

inline float dotProduct(const cVector4& a, const cVector4& b)
{
	return a.dotProduct(b);
}

inline float dotProduct(const cVector4& a, const cVector3& b)
{
	return a.dotProduct(b);
}



inline void cVector3::crossProduct(const cVector3& b)
{
	set((y*b.z) - (z*b.y),
		(z*b.x) - (x*b.z),
		(x*b.y) - (y*b.x));
}

inline void cVector4::crossProduct(const cVector4& b)
{
	set((y*b.z) - (z*b.y),
		(z*b.x) - (x*b.z),
		(x*b.y) - (y*b.x),
		0.0f);
}


inline void cVector3::crossProduct(const cVector3& a, const cVector3& b)
{
	x= (a.y*b.z) - (a.z*b.y);
	y= (a.z*b.x) - (a.x*b.z);
	z= (a.x*b.y) - (a.y*b.x);
}

inline void cVector4::crossProduct(const cVector4& a, const cVector4& b)
{
	x= (a.y*b.z) - (a.z*b.y);
	y= (a.z*b.x) - (a.x*b.z);
	z= (a.x*b.y) - (a.y*b.x);
	w= 0.0f;
}


inline void cVector3::computeHeadingAndPitch(float& heading, float& pitch)const
{
	heading = ArcTangentOfRatio(-x, y);  
	pitch   = -ArcCosine(z) + HALF_PI;
}
inline void cVector4::computeHeadingAndPitch(float& heading, float& pitch)const
{
	heading = ArcTangentOfRatio(-x, y);  
	pitch   = -ArcCosine(z) + HALF_PI;
}


inline void cVector3::setFromHeadingAndPitch(float heading, float pitch)
{
	  float ch = FastCos(heading);
	  float cp = FastCos(pitch);
	  float sh = FastSine(heading);
	  float sp = FastSine(pitch);
	  x = cp * sh;
	  y = -sp;
	  z = cp * ch;
}

inline void cVector4::setFromHeadingAndPitch(float heading, float pitch)
{
	  float ch = FastCos(heading);
	  float cp = FastCos(pitch);
	  float sh = FastSine(heading);
	  float sp = FastSine(pitch);
	  x = cp * sh;
	  y = -sp;
	  z = cp * ch;
	  w = 0.0f;
}


inline void cVector3::computeSphericalCoords(float& longitude, float& latitude)const
{
	computeHeadingAndPitch(longitude, latitude);
	longitude = modulus(longitude, TWO_PI);
	latitude = modulus(latitude, TWO_PI);

	if (longitude<0.0f) longitude = TWO_PI + longitude;
	if (latitude<0.0f) latitude = TWO_PI + latitude;

	longitude /= TWO_PI;
	latitude /= TWO_PI;
}

inline void cVector4::computeSphericalCoords(float& longitude, float& latitude)const
{
	computeHeadingAndPitch(longitude, latitude);
	longitude = modulus(longitude, TWO_PI);
	latitude = modulus(latitude, TWO_PI);

	if (longitude<0.0f) longitude = TWO_PI + longitude;
	if (latitude<0.0f) latitude = TWO_PI + latitude;

	longitude /= TWO_PI;
	latitude /= TWO_PI;
}


inline void cVector3::setFromSphericalCoords(float longitude, float latitude)
{
	setFromHeadingAndPitch(longitude * TWO_PI, latitude * TWO_PI);
}

inline void cVector4::setFromSphericalCoords(float longitude, float latitude)
{
	setFromHeadingAndPitch(longitude * TWO_PI, latitude * TWO_PI);
}


// reflect against the normal of a plane

inline void cVector2::reflect(const cVector2& normal)
{
    float scalar = dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - x;
	y = (scalar*normal.y) - y;
}

inline void cVector3::reflect(const cVector3& normal)
{
    float scalar = dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - x;
	y = (scalar*normal.y) - y;
	z = (scalar*normal.z) - z;
}

inline void cVector4::reflect(const cVector3& normal)
{
    float scalar = dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - x;
	y = (scalar*normal.y) - y;
	z = (scalar*normal.z) - z;
	w = 0.0f;
}


inline void cVector2::reflect(const cVector2& source, const cVector2& normal)
{
    float scalar = source.dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - source.x;
	y = (scalar*normal.y) - source.y;
}

inline void cVector3::reflect(const cVector3& source, const cVector3& normal)
{
    float scalar = source.dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - source.x;
	y = (scalar*normal.y) - source.y;
	z = (scalar*normal.z) - source.z;
}

inline void cVector4::reflect(const cVector4& source, const cVector3& normal)
{
    float scalar = source.dotProduct(normal) * 2.0f;
    x = (scalar*normal.x) - source.x;
	y = (scalar*normal.y) - source.y;
	z = (scalar*normal.z) - source.z;
	w = 0.0f;
}


// refract through the normal of a plane

inline void cVector2::refract(const cVector2& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (x);
		y= (doubleDot*(normal.y)) - (y);
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*x) - (scalar*normal.x);    
		y= (n*y) - (scalar*normal.y);    
   }
}

inline void cVector3::refract(const cVector3& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (x);
		y= (doubleDot*(normal.y)) - (y);
		z= (doubleDot*(normal.z)) - (z);
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*x) - (scalar*normal.x);    
		y= (n*y) - (scalar*normal.y);    
		z= (n*z) - (scalar*normal.z);    
   }
}

inline void cVector4::refract(const cVector3& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (x);
		y= (doubleDot*(normal.y)) - (y);
		z= (doubleDot*(normal.z)) - (z);
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*x) - (scalar*normal.x);    
		y= (n*y) - (scalar*normal.y);    
		z= (n*z) - (scalar*normal.z);    
   }
}




inline void cVector2::refract(const cVector2& source, const cVector2& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = source.dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (source.x);
		y= (doubleDot*(normal.y)) - (source.y);
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*source.x) - (scalar*normal.x);    
		y= (n*source.y) - (scalar*normal.y);    
   }
}

inline void cVector3::refract(const cVector3& source, const cVector3& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = source.dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (source.x);
		y= (doubleDot*(normal.y)) - (source.y);
		z= (doubleDot*(normal.z)) - (source.z);
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*source.x) - (scalar*normal.x);    
		y= (n*source.y) - (scalar*normal.y);    
		z= (n*source.z) - (scalar*normal.z);    
   }
}

inline void cVector4::refract(const cVector4& source, const cVector3& normal, float incident, float transmitted)
{
	debug_assert(transmitted != 0.0f, "invalid transmitted value");

    float V_dot_N = source.dotProduct(normal);
    float n = incident / transmitted;
    float Discrim = 1.0f - Square(n)*(1.0f - Square(V_dot_N));

    if (Discrim < 0.0f)
    {	
		float doubleDot = 2.0f*V_dot_N;

		x= (doubleDot*(normal.x)) - (source.x);
		y= (doubleDot*(normal.y)) - (source.y);
		z= (doubleDot*(normal.z)) - (source.z);
		w= source.w;
    }
    else
    {
 		float sqrtDiscrim = SquareRoot((float)Discrim);
		float scalar = sqrtDiscrim - (n*V_dot_N);

		x= (n*source.x) - (scalar*normal.x);    
		y= (n*source.y) - (scalar*normal.y);    
		z= (n*source.z) - (scalar*normal.z);    
		w= source.w;
   }
}


// project along a normal vector

inline void cVector2::project(const cVector2& normal)
{
	float scalar = dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
}

inline void cVector3::project(const cVector3& normal)
{
	float scalar = dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
	z = normal.z * scalar;
}

inline void cVector4::project(const cVector3& normal)
{
	float scalar = dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
	z = normal.z * scalar;
}


inline void cVector2::project(const cVector2& source, const cVector2& normal)
{
	float scalar = source.dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
}

inline void cVector3::project(const cVector3& source, const cVector3& normal)
{
	float scalar = source.dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
	z = normal.z * scalar;
}

inline void cVector4::project(const cVector4& source, const cVector3& normal)
{
	float scalar = source.dotProduct(normal);
	x = normal.x * scalar;
	y = normal.y * scalar;
	z = normal.z * scalar;
	w = source.w;
}


// linear interpotation (integer scale from 0-100, real scale from 0-1)

inline void cVector2::lerp(const cVector2& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((x*scale) + (b.x*invScale));
	y = ((y*scale) + (b.y*invScale));
}
inline void cVector3::lerp(const cVector3& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((x*scale) + (b.x*invScale));
	y = ((y*scale) + (b.y*invScale));
	z = ((z*scale) + (b.z*invScale));
}
inline void cVector4::lerp(const cVector4& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((x*scale) + (b.x*invScale));
	y = ((y*scale) + (b.y*invScale));
	z = ((z*scale) + (b.z*invScale));
	w = ((w*scale) + (b.w*invScale));
}


inline void cVector2::lerp(const cVector2& a, const cVector2& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((a.x*scale) + (b.x*invScale));
	y = ((a.y*scale) + (b.y*invScale));
}
inline void cVector3::lerp(const cVector3& a, const cVector3& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((a.x*scale) + (b.x*invScale));
	y = ((a.y*scale) + (b.y*invScale));
	z = ((a.z*scale) + (b.z*invScale));
}
inline void cVector4::lerp(const cVector4& a, const cVector4& b, float scale)
{
	debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

	float invScale = 1.0f-scale;
	x = ((a.x*scale) + (b.x*invScale));
	y = ((a.y*scale) + (b.y*invScale));
	z = ((a.z*scale) + (b.z*invScale));
	w = ((a.w*scale) + (b.w*invScale));
}

// create a normal to a plane given three points in clockwise order

inline void cVector3::createNormal(const cVector3& a, const cVector3& b, const cVector3& c)
{
	cVector3 SideA(b);
	cVector3 SideB(c);
	SideA -= a;
	SideB -= b;
	crossProduct(SideA, SideB);
	normalize();
}

inline void cVector4::createNormal(const cVector4& a, const cVector4& b, const cVector4& c)
{
	cVector4 SideA(b);
	cVector4 SideB(c);
	SideA -= a;
	SideB -= b;
	crossProduct(SideA, SideB);
	normalize();
}


// create a normal given two vectors in clockwise order

inline void cVector3::createNormal(const cVector3& a, const cVector3& b)
{
	crossProduct(a, b);
	normalize();
}

inline void cVector4::createNormal(const cVector4& a, const cVector4& b)
{
	crossProduct(a, b);
	normalize();
}


// test whether the vector is in the positive half-space of a normal

inline bool cVector2::testPositiveHalfSpace(const cVector2& normal)const
{
	return (dotProduct(normal) > 0.0f); 
}

inline bool cVector3::testPositiveHalfSpace(const cVector3& normal)const
{
	return (dotProduct(normal) > 0.0f); 
}

inline bool cVector4::testPositiveHalfSpace(const cVector3& normal)const
{
	return (dotProduct(normal) > 0.0f); 
}


// compare with another vector using an optional epsilon value

inline bool cVector2::comparePosition(const cVector2& source)const
{
	return deltaRangeTest(x, source.x)
		&& deltaRangeTest(y, source.y);
}

inline bool cVector3::comparePosition(const cVector3& source)const
{
	return deltaRangeTest(x, source.x)
		&& deltaRangeTest(y, source.y)
		&& deltaRangeTest(z, source.z);
}

inline bool cVector4::comparePosition(const cVector4& source)const
{
	return deltaRangeTest(x, source.x)
		&& deltaRangeTest(y, source.y)
		&& deltaRangeTest(z, source.z)
		&& deltaRangeTest(w, source.w);
}


inline bool cVector2::comparePosition(const cVector2& source, float epsilon)const
{
	return deltaRangeTest(x, source.x, epsilon)
		&& deltaRangeTest(y, source.y, epsilon);
}

inline bool cVector3::comparePosition(const cVector3& source, float epsilon)const
{
	return deltaRangeTest(x, source.x, epsilon)
		&& deltaRangeTest(y, source.y, epsilon)
		&& deltaRangeTest(z, source.z, epsilon);
}

inline bool cVector4::comparePosition(const cVector4& source, float epsilon)const
{
	return deltaRangeTest(x, source.x, epsilon)
		&& deltaRangeTest(y, source.y, epsilon)
		&& deltaRangeTest(z, source.z, epsilon)
		&& deltaRangeTest(w, source.w, epsilon);
}




inline bool cVector2::compareLength(const cVector2& source)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared());
}

inline bool cVector3::compareLength(const cVector3& source)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared());
}

inline bool cVector4::compareLength(const cVector4& source)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared());
}


inline bool cVector2::compareLength(const cVector2& source, float epsilon)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared(), epsilon);
}

inline bool cVector3::compareLength(const cVector3& source, float epsilon)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared(), epsilon);
}

inline bool cVector4::compareLength(const cVector4& source, float epsilon)const
{
	return deltaRangeTest(lengthSquared(), source.lengthSquared(), epsilon);
}


inline bool cVector2::compareDirection(const cVector2& source)const
{
	cVector2 rotate90(source.y, -source.x);
	return deltaRangeTest(dotProduct(rotate90), 0.0f);
}

inline bool cVector3::compareDirection(const cVector3& source)const
{
	cVector3 rotate90(source.y, -source.x, source.z);
	return deltaRangeTest(dotProduct(rotate90), 0.0f);
}

inline bool cVector4::compareDirection(const cVector4& source)const
{
	cVector3 rotate90(source.y, -source.x, source.z);
	return deltaRangeTest(vec3().dotProduct(rotate90), 0.0f);
}


inline bool cVector2::compareDirection(const cVector2& source, float epsilon)const
{
	cVector2 rotate90(source.y, -source.x);
	return deltaRangeTest(dotProduct(rotate90), 0.0f, epsilon);
}

inline bool cVector3::compareDirection(const cVector3& source, float epsilon)const
{
	cVector3 rotate90(source.y, -source.x, source.z);
	return deltaRangeTest(dotProduct(rotate90), 0.0f, epsilon);
}

inline bool cVector4::compareDirection(const cVector4& source, float epsilon)const
{
	cVector3 rotate90(source.y, -source.x, source.z);
	return deltaRangeTest(vec3().dotProduct(rotate90), 0.0f, epsilon);
}


inline bool cVector2::compareToZero()const
{
	return deltaRangeTest(x, 0.0f)
		&& deltaRangeTest(y, 0.0f);
}

inline bool cVector3::compareToZero()const
{
	return deltaRangeTest(x, 0.0f)
		&& deltaRangeTest(y, 0.0f)
		&& deltaRangeTest(z, 0.0f);
}

inline bool cVector4::compareToZero()const
{
	return deltaRangeTest(x, 0.0f)
		&& deltaRangeTest(y, 0.0f)
		&& deltaRangeTest(z, 0.0f)
		&& deltaRangeTest(w, 0.0f);
}


inline bool cVector2::compareToZero(float epsilon)const
{
	return deltaRangeTest(x, 0.0f, epsilon)
		&& deltaRangeTest(y, 0.0f, epsilon);
}

inline bool cVector3::compareToZero(float epsilon)const
{
	return deltaRangeTest(x, 0.0f, epsilon)
		&& deltaRangeTest(y, 0.0f, epsilon)
		&& deltaRangeTest(z, 0.0f, epsilon);
}

inline bool cVector4::compareToZero(float epsilon)const
{
	return deltaRangeTest(x, 0.0f, epsilon)
		&& deltaRangeTest(y, 0.0f, epsilon)
		&& deltaRangeTest(z, 0.0f, epsilon)
		&& deltaRangeTest(w, 0.0f, epsilon);
}


// set to the lesser of two inputs (using length)

inline void cVector2::setMinByLength(const cVector2& b)
{
	float magSquared = lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredB < magSquared)
	{
		x = b.x;
		y = b.y;
	}
}

inline void cVector3::setMinByLength(const cVector3& b)
{
	float magSquared = lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredB < magSquared)
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
}

inline void cVector4::setMinByLength(const cVector4& b)
{
	float magSquared = vec3().lengthSquared();
	float magSquaredB = b.vec3().lengthSquared();

	if (magSquaredB < magSquared)
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}
}


inline void cVector2::setMinByLength(const cVector2& a, const cVector2& b)
{
	float magSquaredA = a.lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredA < magSquaredB)
	{
		x = a.x;
		y = a.y;
	}
	else 
	{
		x = b.x;
		y = b.y;
	}
}

inline void cVector3::setMinByLength(const cVector3& a, const cVector3& b)
{
	float magSquaredA = a.lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredA < magSquaredB)
	{
		x = a.x;
		y = a.y;
		z = a.z;
	}
	else 
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
}

inline void cVector4::setMinByLength(const cVector4& a, const cVector4& b)
{
	float magSquaredA = a.vec3().lengthSquared();
	float magSquaredB = b.vec3().lengthSquared();

	if (magSquaredA < magSquaredB)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
	}
	else 
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}
}


// set to the greater of two inputs (using length)

inline void cVector2::setMaxByLength(const cVector2& b)
{
	float magSquared = lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredB > magSquared)
	{
		x = b.x;
		y = b.y;
	}
}

inline void cVector3::setMaxByLength(const cVector3& b)
{
	float magSquared = lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredB > magSquared)
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
}

inline void cVector4::setMaxByLength(const cVector4& b)
{
	float magSquared = vec3().lengthSquared();
	float magSquaredB = b.vec3().lengthSquared();

	if (magSquaredB > magSquared)
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}
}


inline void cVector2::setMaxByLength(const cVector2& a, const cVector2& b)
{
	float magSquaredA = a.lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredA > magSquaredB)
	{
		x = a.x;
		y = a.y;
	}
	else 
	{
		x = b.x;
		y = b.y;
	}
}

inline void cVector3::setMaxByLength(const cVector3& a, const cVector3& b)
{
	float magSquaredA = a.lengthSquared();
	float magSquaredB = b.lengthSquared();

	if (magSquaredA > magSquaredB)
	{
		x = a.x;
		y = a.y;
		z = a.z;
	}
	else 
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
}

inline void cVector4::setMaxByLength(const cVector4& a, const cVector4& b)
{
	float magSquaredA = a.vec3().lengthSquared();
	float magSquaredB = b.vec3().lengthSquared();

	if (magSquaredA > magSquaredB)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
	}
	else 
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}
}

inline void cVector2::setMinByValue(const cVector2& source_b)
{
	D3DXVec2Minimize(this, this, &source_b);
}
inline void cVector3::setMinByValue(const cVector3& source_b)
{
	D3DXVec3Minimize(this, this, &source_b);
}
inline void cVector4::setMinByValue(const cVector4& source_b)
{
	D3DXVec4Minimize(this, this, &source_b);
}

inline void cVector2::setMinByValue(const cVector2& source_a, const cVector2& source_b)
{
	D3DXVec2Minimize(this, &source_a, &source_b);
}
inline void cVector3::setMinByValue(const cVector3& source_a, const cVector3& source_b)
{
	D3DXVec3Minimize(this, &source_a, &source_b);
}
inline void cVector4::setMinByValue(const cVector4& source_a, const cVector4& source_b)
{
	D3DXVec4Minimize(this, &source_a, &source_b);
}


inline void cVector2::setMaxByValue(const cVector2& source_b)
{
	D3DXVec2Maximize(this, this, &source_b);
}
inline void cVector3::setMaxByValue(const cVector3& source_b)
{
	D3DXVec3Maximize(this, this, &source_b);
}
inline void cVector4::setMaxByValue(const cVector4& source_b)
{
	D3DXVec4Maximize(this, this, &source_b);
}

inline void cVector2::setMaxByValue(const cVector2& source_a, const cVector2& source_b)
{
	D3DXVec2Maximize(this, &source_a, &source_b);
}
inline void cVector3::setMaxByValue(const cVector3& source_a, const cVector3& source_b)
{
	D3DXVec3Maximize(this, &source_a, &source_b);
}
inline void cVector4::setMaxByValue(const cVector4& source_a, const cVector4& source_b)
{
	D3DXVec4Maximize(this, &source_a, &source_b);
}


//--------------------------------------------------
// File...
//--------------------------------------------------

inline uint32 cVector2::load(cFile& InputFile)
{
	InputFile >> x;
	InputFile >> y;
	return (sizeof(float)*2);
}

inline uint32 cVector3::load(cFile& InputFile)
{
	InputFile >> x;
	InputFile >> y;
	InputFile >> z;
	return (sizeof(float)*3);
}

inline uint32 cVector4::load(cFile& InputFile)
{
	InputFile >> x;
	InputFile >> y;
	InputFile >> z;
	InputFile >> w;
	return (sizeof(float)*4);
}




inline uint32 cVector2::save(cFile& OutputFile)
{
	OutputFile << x;
	OutputFile << y;
	return (sizeof(float)*2);
}

inline uint32 cVector3::save(cFile& OutputFile)
{
	OutputFile << x;
	OutputFile << y;
	OutputFile << z;
	return (sizeof(float)*3);
}

inline uint32 cVector4::save(cFile& OutputFile)
{
	OutputFile << x;
	OutputFile << y;
	OutputFile << z;
	OutputFile << w;
	return (sizeof(float)*4);
}


inline float dotProduct3(const cVector4* a, const cVector3* b)
{
	return a->vec3().dotProduct(*b);
}

inline float dotProduct3(const cVector3* a, const cVector4* b)
{
	return a->dotProduct(b->vec3());
}

inline float dotProduct3(const cVector4* a, const cVector4* b)
{
	return a->vec3().dotProduct(b->vec3());
}


} //End NameSpace: gaia

#endif  // end of file      ( Vector_Functions.h )

