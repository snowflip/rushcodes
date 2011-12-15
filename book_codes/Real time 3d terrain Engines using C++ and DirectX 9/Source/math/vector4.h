#ifndef VECTOR4_H
#define VECTOR4_H
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
#include "core\core.h"
#include "vector2.h"
#include "vector3.h"

//	Name Space declaration
namespace gaia
{


class cVector4 : public D3DXVECTOR4
{
public:

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cVector4(){};
	cVector4(const cVector4& vec):D3DXVECTOR4(vec.x, vec.y, vec.z, vec.w){};
	cVector4(float _x, float _y, float _z, float _w):D3DXVECTOR4(_x,_y,_z,_w){};
	cVector4(cVector3 _xyz, float _w):D3DXVECTOR4(_xyz.x, _xyz.y, _xyz.z, _w){};
	~cVector4(void){};

	//--------------------------------------------------
	// Aliases...
	//--------------------------------------------------

	// aliases
	cVector2& vec2() {return *((cVector2*)this);}
	cVector3& vec3() {return *((cVector3*)this);}
	operator cVector2& () {return vec2();}
	operator cVector3& () {return vec3();}

	const cVector2& vec2()const {return *((cVector2*)this);}
	const cVector3& vec3()const {return *((cVector3*)this);}
	operator const cVector2&()const {return vec2();}
	operator const cVector3&()const {return vec3();}

	const cVector4& operator *= (const cVector4& vec);
	const cVector4& operator /= (const cVector4& vec);
	const cVector4& operator *= (float v);
	const cVector4& operator /= (float v);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
	
	void set(float _x, float _y, float _z, float _w);
	void clear(void);
	void setToAbsoluteValue();
	void invert();
	void negate();

	//--------------------------------------------------
	// Calculations\Operations...
	//--------------------------------------------------
	
	// testing operations
	float maxValue()const;
	float minValue()const;
	bool isClear()const;

	// length and distance calculations
	void normalize();
	float length()const;
	float lengthSquared()const;
	float distance(const cVector4& vec)const;
	float distanceSquared(const cVector4& vec)const;
	
	// dot and cross product
	float dotProduct(const cVector4& vec)const;
	float dotProduct(const cVector3& vec)const;
	void crossProduct(const cVector4& vec_b);
	void crossProduct(const cVector4& vec_a, const cVector4& vec_b);

	// angle operations (only valid for float, normalized versions)
	void computeHeadingAndPitch(float& heading, float& pitch)const;
	void setFromHeadingAndPitch(float heading, float pitch);
	void computeSphericalCoords(float& longitude, float& latitude)const;
	void setFromSphericalCoords(float longitude, float latitude);

	// reflect against the normal of a plane
	void reflect(const cVector3& normal);
	void reflect(const cVector4& source, const cVector3& normal);

	// refract through the normal of a plane
	void refract(const cVector3& normal, float incident, float transmitted);
	void refract(const cVector4& source, const cVector3& normal, float incident, float transmitted);

	// project along a normal vector
	void project(const cVector3& normal);
	void project(const cVector4& source, const cVector3& normal);

	// linear interpretation (integer scale from 0-100, real scale from 0-1)
	void lerp(const cVector4& source_b, float scale);
	void lerp(const cVector4& source_a, const cVector4& source_b, float scale);

	// create a normal to a plane given three points in clockwise order
	void createNormal(const cVector4& a, const cVector4& b, const cVector4& c);

	// create a normal given two vectors in clockwise order
	void createNormal(const cVector4& a, const cVector4& b);

	// test whether the vector is in the positive half-space of a normal
	bool testPositiveHalfSpace(const cVector3& normal)const;

	// compare with another vector using an optional epsilon value
	bool comparePosition(const cVector4& source)const;
	bool comparePosition(const cVector4& source, float epsilon)const;
	bool compareLength(const cVector4& source)const;
	bool compareLength(const cVector4& source, float epsilon)const;
	bool compareDirection(const cVector4& source)const;
	bool compareDirection(const cVector4& source, float epsilon)const;
	bool compareToZero()const;
	bool compareToZero(float epsilon)const;

	// set to the lesser components of two inputs
	void setMinByValue(const cVector4& source_b);
	void setMinByValue(const cVector4& source_a, const cVector4& source_b);
	// set to the lesser of two inputs (using length)
	void setMinByLength(const cVector4& source_b);
	void setMinByLength(const cVector4& source_a, const cVector4& source_b);

	// set to the greater components of two inputs
	void setMaxByValue(const cVector4& source_b);
	void setMaxByValue(const cVector4& source_a, const cVector4& source_b);
	// set to the greater of two inputs (using length)
	void setMaxByLength(const cVector4& source_b);
	void setMaxByLength(const cVector4& source_a, const cVector4& source_b);

	//--------------------------------------------------
	// File...
	//--------------------------------------------------
	uint32 load(cFile& InputFile);
	uint32 save(cFile& OutputFile);
};

// define a type for aligned vectors
// this uses the _ALIGN_16 definition from
// d3dxmath.h, which is only set when compiled
// with VC.NET (_MSC_VER >= 1300)
#define cSIMDVector _ALIGN_16 cVector4

//- End of cVector4 ---------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cVector4.h )

//----------------------------------------------------------
//$Log: $