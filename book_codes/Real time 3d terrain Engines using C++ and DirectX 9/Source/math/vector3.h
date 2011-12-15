#ifndef VECTOR3_H
#define VECTOR3_H
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


class cVector3 : public D3DXVECTOR3
{
public:

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cVector3(){};
	cVector3(const cVector3& vec):D3DXVECTOR3(vec.x, vec.y, vec.z){};
	cVector3(float _x, float _y, float _z):D3DXVECTOR3(_x,_y,_z){};
	cVector3(cVector2 _xy, float _z):D3DXVECTOR3(_xy.x, _xy.y, _z){};
	~cVector3(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------

	// aliases
	cVector2& vec2() {return *((cVector2*)this);}
	operator cVector2& () {return vec2();}

	const cVector2& vec2()const {return *((cVector2*)this);}
	operator const cVector2&()const {return vec2();}

	const cVector3& operator *= (const cVector3& vec);
	const cVector3& operator /= (const cVector3& vec);
	const cVector3& operator *= (float v);
	const cVector3& operator /= (float v);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
	
	void set(float _x, float _y, float _z);
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
	float distance(const cVector3& vec)const;
	float distanceSquared(const cVector3& vec)const;
	
	// dot and cross product
	float dotProduct(const cVector3& vec)const;
	void crossProduct(const cVector3& vec_b);
	void crossProduct(const cVector3& vec_a, const cVector3& vec_b);

	// angle operations (only valid for float, normalized versions)
	void computeHeadingAndPitch(float& heading, float& pitch)const;
	void setFromHeadingAndPitch(float heading, float pitch);
	void computeSphericalCoords(float& longitude, float& latitude)const;
	void setFromSphericalCoords(float longitude, float latitude);

	// reflect against the normal of a plane
	void reflect(const cVector3& normal);
	void reflect(const cVector3& source, const cVector3& normal);

	// refract through the normal of a plane
	void refract(const cVector3& normal, float incident, float transmitted);
	void refract(const cVector3& source, const cVector3& normal, float incident, float transmitted);

	// project along a normal vector
	void project(const cVector3& normal);
	void project(const cVector3& source, const cVector3& normal);

	// linear interpretation (integer scale from 0-100, float scale from 0-1)
	void lerp(const cVector3& source_b, float scale);
	void lerp(const cVector3& source_a, const cVector3& source_b, float scale);

	// create a normal to a plane given three points in clockwise order
	void createNormal(const cVector3& a, const cVector3& b, const cVector3& c);

	// create a normal given two vectors in clockwise order
	void createNormal(const cVector3& a, const cVector3& b);

	// test whether the vector is in the positive half-space of a normal
	bool testPositiveHalfSpace(const cVector3& normal)const;

	// compare with another vector using an optional epsilon value
	bool comparePosition(const cVector3& source)const;
	bool comparePosition(const cVector3& source, float epsilon)const;
	bool compareLength(const cVector3& source)const;
	bool compareLength(const cVector3& source, float epsilon)const;
	bool compareDirection(const cVector3& source)const;
	bool compareDirection(const cVector3& source, float epsilon)const;
	bool compareToZero()const;
	bool compareToZero(float epsilon)const;

	// set to the lesser components of two inputs
	void setMinByValue(const cVector3& source_b);
	void setMinByValue(const cVector3& source_a, const cVector3& source_b);
	// set to the lesser of two inputs (using length)
	void setMinByLength(const cVector3& source_b);
	void setMinByLength(const cVector3& source_a, const cVector3& source_b);

	// set to the greater components of two inputs
	void setMaxByValue(const cVector3& source_b);
	void setMaxByValue(const cVector3& source_a, const cVector3& source_b);
	// set to the greater of two inputs (using length)
	void setMaxByLength(const cVector3& source_b);
	void setMaxByLength(const cVector3& source_a, const cVector3& source_b);

	//--------------------------------------------------
	// File...
	//--------------------------------------------------
	uint32 load(cFile& InputFile);
	uint32 save(cFile& OutputFile);
};

//- End of cVector3 ---------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cVector3.h )

//----------------------------------------------------------
//$Log: $