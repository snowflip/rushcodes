#define CRANDOM_CPP
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

#include "random_numbers.h"

using namespace gaia;

// the global random seed (value chosen at random)
int gaia::g_random_seed = 22471;

//:	GaussRandom
//----------------------------------------------------------------------------------------
//
//	Generatres a gaussian random number using 
//	the polar form of the Box-Muller transformation
//
//-------------------------------------------------------------------------------------://

// The polar form of the Box-Muller transformation 
// is capable of generating two random gauss values
// in one pass. We provide a method to captolize on that
// since we need our Gaussian values in pairs
// to build complex numbers
void gaia::GaussRandomPair( 
	float& result_a, float& result_b,
	float dMean, float dStdDeviation)
{
	float x1, x2, w, y1, y2;

	do {
		 x1 = 2.0f * random_unit_float() - 1.0f;
		 x2 = 2.0f * random_unit_float() - 1.0f;
		 w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0f );

	w = sqrtf( (-2.0f * logf( w ) ) / w );
	y1 = x1 * w;
	y2 = x2 * w;

    result_a = ( dMean + y1 * dStdDeviation );
    result_b = ( dMean + y2 * dStdDeviation );
}

// when generating single results, 
// we store the second generated result
// for the next call to the function
float gaia::GaussRandom( float dMean, float dStdDeviation )
{
	static bool use_precalced_result = false;
	static float y2=0.0f;
	float x1, x2, w, y1;
	
	if (use_precalced_result)
	{
		use_precalced_result = false;
		return( dMean + y2 * dStdDeviation );
	}

	do {
		 x1 = 2.0f * random_unit_float() - 1.0f;
		 x2 = 2.0f * random_unit_float() - 1.0f;
		 w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0f );

	w = sqrtf( (-2.0f * logf( w ) ) / w );
	y1 = x1 * w;
	y2 = x2 * w;

	use_precalced_result = true;

    return( dMean + y1 * dStdDeviation );
}


//***************************************************************
// end of file      ( random_numbers.cpp )

//----------------------------------------------------------
//$Log: $