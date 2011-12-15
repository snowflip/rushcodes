#ifndef CRANDOM_H
#define CRANDOM_H
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
#include <math.h>

//	Name Space declaration
namespace gaia
{


/*	Random Number Generation
-----------------------------------------------------------------
    
    This file contains a psudo random number generator. 
	All random generation is based on a random integer
	method described by Guy W. Lecky-Thompson in 
	Game Programming Gems (Charles River Media). 
	A set of template functions are also provided to create
	random numbers of various data types.

	A set of Gauss methods are also provided to generate
	Gaussian random numbers from the regular generator.
    
-----------------------------------------------------------------
*/
enum eRandomConstants
{
	random_constant_a= 1664525,
	random_constant_b= 1013904223,
};

extern int g_random_seed;

inline void set_random_seed(int seed)
{
	g_random_seed = seed;
}

inline int random_int()
{
	g_random_seed = 
		(random_constant_a * g_random_seed)
		+ random_constant_b; 

	return g_random_seed;
}

inline float random_unit_float()
{
	// return (random_int / 2^32)
	// to create a value in the [0,1) range
	return random_int()
		* 2.3283064365386963e-10f;
}

inline float random_float()
{
	return (random_unit_float()*MAX_REAL32);
}

//:	RandomNumberInRange
//----------------------------------------------------------------------------------------
//
//	Generatres a random number in the range [Min, Max)
//
//-------------------------------------------------------------------------------------://

// generic integer version
template <class T> 
inline T RandomNumberInRange(T Min, T Max)
{
	T Range = Max - Min;
	T Random = absoluteValue(random_int());

	Random = modulus(Random,Range);
	Random += Min; 

	return (Random);
}

// floating-point version
template <> 
inline float RandomNumberInRange(float Min, float Max)
{
	float scalar = random_unit_float();
	float range = Max - Min;
	return (Min + (scalar*range));
}


//:	GaussRandom
//----------------------------------------------------------------------------------------
//
//	Generatres a gaussian random number using 
//	the polar form of a Box-Muller transformation
//
//-------------------------------------------------------------------------------------://

// The polar form of the Box-Muller transformation 
// is capable of generating two random gauss values
// in one pass. We provide a method to capitolize on that
extern void GaussRandomPair( 
	float& result_a, float& result_b,
	float dMean=0.0f, float dStdDeviation=1.0f);

// when generating single results, 
// we store the second generated result
// for the next call to the function
extern float GaussRandom( float dMean=0.0f, float dStdDeviation=1.0f );


//- End of cRandom ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRandom.h )

//----------------------------------------------------------
//$Log: $