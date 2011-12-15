#ifndef CMIDPOINTGEN_H
#define CMIDPOINTGEN_H
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
#include "data_types.h"


//	Name Space declaration
namespace gaia
{


/*	cMidpointNoise
-----------------------------------------------------------------
    
    A Simple class performing midpoint displacement to generate
	a noise image
    
-----------------------------------------------------------------
*/

class cMidpointNoise
{
public:

	// Data Types & Constants...
	enum
	{
		k_tableSize = 256
	};

	// Public Data...

	// Creators...

    cMidpointNoise();
    ~cMidpointNoise();

	// Mutators...
	float noise(uint16 x, uint16 y, uint16 sizeX, uint16 sizeY, float falloff);

	// Accessors...

private:

	// Private Data...
	float m_floatTable[k_tableSize];
	unsigned char m_indexTable[k_tableSize];

	// Private Functions...
    explicit cMidpointNoise(const cMidpointNoise& Src);
    cMidpointNoise& operator=(const cMidpointNoise& Src);

	void setup();

	float randomValue(uint16 x, uint16 y, float range);

	float recursiveDisplacement(
		uint16 target_x, uint16 target_y,
		uint16 min_x, uint16 min_y,
		uint16 max_x, uint16 max_y,
		float A, float B, float C, float D,
		float range,
		float falloff);

};


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cMidpointNoise.h )

//----------------------------------------------------------
//$Log: $