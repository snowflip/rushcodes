#ifndef PERLIN_H
#define PERLIN_H
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
#include <math.h>


//	Name Space declaration
namespace gaia
{


/*	cPerlinNoise
-----------------------------------------------------------------
    
    A class to generate perlin noise in 2D
    
-----------------------------------------------------------------
*/

class cPerlinNoise
{
public:

	struct vec2
	{
		float x,y;

		vec2(float _x, float _y):x(_x),y(_y){}; 
		vec2(){};
	};

	enum
	{
		k_tableSize = 256,
		k_tableMask = k_tableSize-1,
	};

    cPerlinNoise();
    ~cPerlinNoise();

	float noise(int x, int y, float scale);
	float noise(float x, float y, float scale);

private:

	vec2 m_vecTable[k_tableSize];
	unsigned char m_lut[k_tableSize];

	// Private Functions...
	void setup();
	const vec2& getVec(int x, int y)const;
};

inline void cPerlinNoise::setup()
{
	float step = 6.24f / k_tableSize;
	float val=0.0f;

	for (int i=0; i<k_tableSize; ++i)
	{
		m_vecTable[i].x = (float)sin(val);
		m_vecTable[i].y = (float)cos(val);
		val += step;

		m_lut[i] = rand() & k_tableMask;
	}
}

inline const cPerlinNoise::vec2& cPerlinNoise::getVec(
	int x, 
	int y)const
{
	unsigned char a = m_lut[x&k_tableMask]; 
	unsigned char b = m_lut[y&k_tableMask]; 
	unsigned char val = m_lut[(a+b)&k_tableMask];
	return m_vecTable[val];
}


inline float cPerlinNoise::noise(
	float x, 
	float y,
	float scale)
{
	vec2 pos(x*scale,y*scale);

	float X0 = (float)floor(pos.x);
	float X1 = X0 + 1.0f;
	float Y0 = (float)floor(pos.y);
	float Y1 = Y0 + 1.0f;

	const vec2& v0 = 
		getVec((int)X0, (int)Y0);
	const vec2& v1 = 
		getVec((int)X0, (int)Y1);
	const vec2& v2 = 
		getVec((int)X1, (int)Y0);
	const vec2& v3 = 
		getVec((int)X1, (int)Y1);

	vec2 d0(pos.x-X0, pos.y-Y0);
	vec2 d1(pos.x-X0, pos.y-Y1);
	vec2 d2(pos.x-X1, pos.y-Y0);
	vec2 d3(pos.x-X1, pos.y-Y1);

	float h0 = (d0.x * v0.x)+(d0.y * v0.y);
	float h1 = (d1.x * v1.x)+(d1.y * v1.y);
	float h2 = (d2.x * v2.x)+(d2.y * v2.y);
	float h3 = (d3.x * v3.x)+(d3.y * v3.y);

	float Sx,Sy;

/*
	Perlin's original equation was faster,
	but produced artifacts in some situations
	Sx = (3*powf(d0.x,2.0f))
		-(2*powf(d0.x,3.0f));

	Sy = (3*powf(d0.y,2.0f))
		-(2*powf(d0.y,3.0f));
*/

	// the revised blend equation is 
	// considered more ideal, but is
	// slower to compute
	Sx = (6*powf(d0.x,5.0f))
		-(15*powf(d0.x,4.0f))
		+(10*powf(d0.x,3.0f));

	Sy = (6*powf(d0.y,5.0f))
		-(15*powf(d0.y,4.0f))
		+(10*powf(d0.y,3.0f));


	float avgX0 = h0 + (Sx*(h2 - h0));
	float avgX1 = h1 + (Sx*(h3 - h1));
	float result = avgX0 + (Sy*(avgX1 - avgX0)); 

	return result;
}

inline float cPerlinNoise::noise(
	int x, 
	int y, 
	float scale)
{
	return noise((float)x, (float)y, scale);
}

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cPerlinNoise
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cPerlinNoise::cPerlinNoise()
{
	setup();
}


/*	~cPerlinNoise
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cPerlinNoise::~cPerlinNoise()
{
}

//. Operators ...................................................

//. Mutators ....................................................

//. Accessors ...................................................

//- End of cPerlinNoise ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cPerlinNoise.h )

//----------------------------------------------------------
//$Log: $
