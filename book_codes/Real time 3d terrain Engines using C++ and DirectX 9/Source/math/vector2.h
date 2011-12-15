#ifndef VECTOR2_H
#define VECTOR2_H
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
#include "d3dx9math.h"

//	Name Space declaration
namespace gaia
{


class cVector2 : public D3DXVECTOR2
{
public:

	//--------------------------------------------------
	// Creators...
	//--------------------------------------------------

	cVector2(){};
	cVector2(const cVector2& vec):D3DXVECTOR2(vec.x,vec.y){};
	cVector2(float _x, float _y):D3DXVECTOR2(_x,_y){};
	~cVector2(void){};

	//--------------------------------------------------
	// Operators...
	//--------------------------------------------------

	const cVector2& operator *= (const cVector2& vec);
	const cVector2& operator /= (const cVector2& vec);
	const cVector2& operator *= (float v);
	const cVector2& operator /= (float v);

	//--------------------------------------------------
	// Mutators...
	//--------------------------------------------------
	
	void clear(void);
	void setToAbsoluteValue();
	void set(float _x, float _y);
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
	float distance(const cVector2& vec)const;
	float distanceSquared(const cVector2& vec)const;
	
	// dot product
	float dotProduct(const cVector2& vec)const;

	// reflect against the normal of a plane
	void reflect(const cVector2& normal);
	void reflect(const cVector2& source, const cVector2& normal);

	// refract through the normal of a plane
	void refract(const cVector2& normal, float incident, float transmitted);
	void refract(const cVector2& source, const cVector2& normal, float incident, float transmitted);

	// project along a normal vector
	void project(const cVector2& normal);
	void project(const cVector2& source, const cVector2& normal);

	// linear interpretation (float scale from 0-1)
	void lerp(const cVector2& source_b, float scale);
	void lerp(const cVector2& source_a, const cVector2& source_b, float scale);

	// test whether the vector is in the positive half-space of a normal
	bool testPositiveHalfSpace(const cVector2& normal)const;

	// compare with another vector using an optional epsilon value
	bool comparePosition(const cVector2& source)const;
	bool comparePosition(const cVector2& source, float epsilon)const;
	bool compareLength(const cVector2& source)const;
	bool compareLength(const cVector2& source, float epsilon)const;
	bool compareDirection(const cVector2& source)const;
	bool compareDirection(const cVector2& source, float epsilon)const;
	bool compareToZero()const;
	bool compareToZero(float epsilon)const;

	// set to the lesser components of two inputs
	void setMinByValue(const cVector2& source_b);
	void setMinByValue(const cVector2& source_a, const cVector2& source_b);
	// set to the lesser of two inputs (using length)
	void setMinByLength(const cVector2& source_b);
	void setMinByLength(const cVector2& source_a, const cVector2& source_b);

	// set to the greater components of two inputs
	void setMaxByValue(const cVector2& source_b);
	void setMaxByValue(const cVector2& source_a, const cVector2& source_b);
	// set to the greater of two inputs (using length)
	void setMaxByLength(const cVector2& source_b);
	void setMaxByLength(const cVector2& source_a, const cVector2& source_b);

	//--------------------------------------------------
	// File...
	//--------------------------------------------------
	uint32 load(cFile& InputFile);
	uint32 save(cFile& OutputFile);
};


//- End of cVector2 ---------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cVector2.h )

//----------------------------------------------------------
//$Log: $