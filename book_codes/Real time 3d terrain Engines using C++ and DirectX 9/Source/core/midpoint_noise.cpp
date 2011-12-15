#define CMIDPOINTGENERATOR_CPP
#define CORE_DLL
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

#include "midpoint_noise.h"
#include "memory.h"

using namespace gaia;



void cMidpointNoise::setup()
{
	float fstep = 2.0f/k_tableSize;
	float value = -1.0f;

	for (int i=0; i<k_tableSize; ++i)
	{
		m_floatTable[i] = value;
		value += fstep;
		m_indexTable[i] = rand();
	}

}

float cMidpointNoise::randomValue(uint16 x, uint16 y, float range)
{
	int lut0= m_indexTable[(x*3) % k_tableSize];
	int lut1= m_indexTable[(y*7) % k_tableSize];
	int lut2= m_indexTable[(lut0*lut1) % k_tableSize];

	float rval = m_floatTable[lut2];

	rval *= range;

	return rval;
}

cMidpointNoise::cMidpointNoise()
{
	setup();
}

cMidpointNoise::~cMidpointNoise()
{
}

float cMidpointNoise::noise(uint16 x, uint16 y, uint16 sizeX, uint16 sizeY, float falloff)
{
	float range = 0.5f;

	uint16 maxX = sizeX-1;
	uint16 maxY = sizeY-1;

	float c0 = 0.5f+(randomValue(0,0, range));
	float c1 = 0.5f+(randomValue(maxX,0, range));
	float c2 = 0.5f+(randomValue(maxX,maxY, range));
	float c3 = 0.5f+(randomValue(0,maxY, range));

	range *= falloff;

	if (x==0 && y==0) return c0;
	if (x==maxX && y==0) return c1;
	if (x==maxX && y==maxY) return c2;
	if (x==0 && y==maxY) return c3;

	return recursiveDisplacement(
		x, y,
		0, 0, 
		sizeX, sizeY,
		c0, c1, c2, c3,
		range, falloff);
}

float cMidpointNoise::recursiveDisplacement(
	uint16 target_x, uint16 target_y,
	uint16 min_x, uint16 min_y,
	uint16 max_x, uint16 max_y,
	float A, float B, float C, float D,
	float range,
	float falloff)
{
	if (max_x - min_x > 1
		|| max_y - min_y > 1)
	{
		// compute edge points
		uint16 mid_x = (min_x + max_x) >> 1;
		uint16 mid_y = (min_y + max_y) >> 1;
		uint16 x,y;
		float P1,P2,P3,P4,P5;

		// point 1
		x = mid_x;
		y = max_y;

		P1 = (A+B)*0.5f;
		P1 += randomValue(x,y, range);
		P1 = clamp(P1, 0.0f, 1.0f);

		if (x == target_x && y== target_y)
		{
			return P1;
		}

		// point 2
		x = max_x;
		y = mid_y;

		P2 = (B+D)*0.5f;
		P2 += randomValue(x,y, range);
		P2 = clamp(P2, 0.0f, 1.0f);

		if (x == target_x && y== target_y)
		{
			return P2;
		}

		// point 3
		x = mid_x;
		y = min_y;

		P3 = (C+D)*0.5f;
		P3 += randomValue(x,y, range);
		P3 = clamp(P3, 0.0f, 1.0f);

		if (x == target_x && y== target_y)
		{
			return P3;
		}

		// point 4
		x = min_x;
		y = mid_y;

		P4 = (A+C)*0.5f;
		P4 += randomValue(x,y, range);
		P4 = clamp(P4, 0.0f, 1.0f);

		if (x == target_x && y== target_y)
		{
			return P4;
		}

		// point 5
		x = mid_x;
		y = mid_y;

		P5 = (A+B+C+D)*0.25f;
		P5 += randomValue(x,y, range);
		P5 = clamp(P5, 0.0f, 1.0f);

		if (x == target_x && y== target_y)
		{
			return P5;
		}

		range *= falloff;

		// which quadrant do we step into?
		if (target_x >= min_x 
			&& target_x <= mid_x
			&& target_y >= mid_y
			&& target_y <= max_y)
		{
			return recursiveDisplacement(
				target_x, target_y,
				min_x, mid_y, 
				mid_x, max_y,
				A, P1, P4, P5,
				range, falloff);
		}
		else if (target_x >= mid_x 
			&& target_x <= max_x
			&& target_y >= mid_y
			&& target_y <= max_y)
		{
			return recursiveDisplacement(
					target_x, target_y,
					mid_x, mid_y, 
					max_x, max_y,
					P1, B, P5, P2,
					range, falloff);
		}
		else if (target_x >= min_x 
			&& target_x <= mid_x
			&& target_y >= min_y
			&& target_y <= mid_y)
		{
			return recursiveDisplacement(
				target_x, target_y,
				min_x, min_y, 
				mid_x, mid_y,
				P4, P5, C, P3,
				range, falloff);
		}
		else if (target_x >= mid_x 
			&& target_x <= max_x
			&& target_y >= min_y
			&& target_y <= mid_y)
		{
			return recursiveDisplacement(
				target_x, target_y,
				mid_x, min_y, 
				max_x, mid_y,
				P5, P2, P3, D,
				range, falloff);
		}
	}

	return 0;
}


//***************************************************************
// end of file      ( cMidpointGenerator.cpp )

//----------------------------------------------------------
//$Log: $