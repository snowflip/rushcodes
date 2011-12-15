#ifndef MATHTOOLS_H
#define MATHTOOLS_H
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

#ifndef STD_MATH_H
#define STD_MATH_H
#include <math.h>
#endif

#ifndef STD_STDLIB_H
#define STD_STDLIB_H
#include <stdlib.h>
#endif


//	Name Space declaration
namespace gaia
{

//
// BASIC MATH AND UNIT CONVERSION CONSTANTS
//

extern const float    FLOAT_PRECISION;					
extern const float    PI;								
extern const float    TWO_PI;							
extern const float    HALF_PI;							
extern const float    QUARTER_PI;						
extern const float    EIGHTH_PI;						
extern const float    PI_SQUARED;						
extern const float    PI_INVERSE;						
extern const float    PI_OVER_180;						
extern const float    PI_DIV_180;						
extern const float    NATURAL_LOGARITHM_BASE;			
extern const float    EULERS_CONSTANT;					
extern const float    SQUARE_ROOT_2;					
extern const float    INVERSE_ROOT_2;					
extern const float    SQUARE_ROOT_3;					
extern const float    SQUARE_ROOT_5;					
extern const float    SQUARE_ROOT_10;					
extern const float    CUBE_ROOT_2;						
extern const float    CUBE_ROOT_3;						
extern const float    FOURTH_ROOT_2;					
extern const float    NATURAL_LOG_2;					
extern const float    NATURAL_LOG_3;					
extern const float    NATURAL_LOG_10;					
extern const float    NATURAL_LOG_PI;					
extern const float    BASE_TEN_LOG_PI;					
extern const float    NATURAL_LOGARITHM_BASE_INVERSE;	
extern const float    NATURAL_LOGARITHM_BASE_SQUARED;	
extern const float    GOLDEN_RATIO;					
extern const float    DEGREE_RATIO;					
extern const float    RADIAN_RATIO;					
extern const float    GRAVITY_CONSTANT;				

//
// MATH AND UNIT CONVERSION FUNCTION PROTOTYPES
//

template <class T> inline T Sine(T angle);
template <class T> inline T ArcSine(T angle);
template <class T> inline T HypSine(T angle);
template <class T> inline T Cosine(T angle);
template <class T> inline T ArcCosine(T angle);
template <class T> inline T HypCosine(T angle);
template <class T> inline T Tangent(T angle);
template <class T> inline T ArcTangent(T angle);
template <class T> inline T HypTangent(T angle);
template <class T> inline T ArcTangentOfRatio(T Numerator, T Denominator);

template <class T> inline T Log(T value);
template <class T> inline T Log10(T value);
template <class T> inline T RaiseToPower(T value, T power);
template <class T> inline T SquareRoot(T value);
template <class T> inline T Square(T value);

template <class T> inline T Hypotenuse(T SideA, T SideB);
template <class T> inline T CalcSide(T Side, T Hypotenuse);

template <class T> inline T DegToRad(T Degrees);
template <class T> inline T RadToDeg(T Radians);

//:	fast_inverse
//----------------------------------------------------------------------------------------
//
//	Calculates the inverse of a given value. (1/value)
//	approximatly 3 times faster than doing the divide yourself
//
//	This approximation gets innacurate with small values.
//	For the test case of 10000 values iterated between -5.0f and 5.0f,
//	the worst deviation was the source value -0.000000126 creating a result of
//	-7916990.5 instead of -7910778.5 (an error of 6212). But for reasonably small values
//	like 0.0001, the approximate error is less than 10.
//
//-------------------------------------------------------------------------------------://
inline float fast_inverse(float value)
{
    long _i = 2 * fpOneBits - fpBits(value);                                 
    float r = intBits(_i);                                                       
    return fpBits(value) ? (r * (2.0f - (value) * r)) : 0;  
}

//:	fast_sqrt
//----------------------------------------------------------------------------------------
//
//	Uses Jim Blinn's IEEE sqaure root method.
//	approximatly 10 times faster than sqrt()
//
//	For the test case of 10000 values iterated between -5.0f and 5.0f,
//	The worst deviation was 0.08 for the source value 2.0
//
//-------------------------------------------------------------------------------------://
inline float fast_sqrt(float n)
{
	long val = (fpBits(n)>>1) + (fpOneBits>>1);
	return (intBits(val));
}

//
//  Fast Table-lookup routines (tables must be init prior to first use)
//
union FastSqrtUnion
{
  float d;
  uint32 i;
};


#define TRIG_TABLE_SIZE 2048
#define TRIG_INDEX_MASK (TRIG_TABLE_SIZE-1)
#define TRIG_QUAD_SIZE (TRIG_TABLE_SIZE>>2)
#define TRIG_TABLE_FACTOR ((float)(TWO_PI/(float)TRIG_TABLE_SIZE))
#define TRIG_TABLE_MULT ((float)((float)TRIG_TABLE_SIZE/TWO_PI))
 
extern float s_SinCosTable[TRIG_TABLE_SIZE];
extern float s_TangentTable[TRIG_TABLE_SIZE];
extern uint32 fast_sqrt_table[0x10000];  // declare table of square roots 

inline float FastTangent(float radians)
{
	float value = radians * TRIG_TABLE_MULT;

	// find the index and the table value
	int index = ((int)realToInt32_chop(value)) & TRIG_INDEX_MASK;
	value = s_TangentTable[index];

	// toggle the sign of the result 
	// based on the sign of the index
	*((int*)&value) ^= (index&80000000);

	return(value);
}

inline float FastSine(float radians)
{
	float value = radians * TRIG_TABLE_MULT;

	// find the index and the table value
	int index = ((int)realToInt32_chop(value)) & TRIG_INDEX_MASK;
	value = s_SinCosTable[index];

	// toggle the sign of the result 
	// based on the sign of the index
	*((int*)&value) ^= (index&80000000);

	return(value);
}

inline float FastCos(float radians)
{
	float value = radians * TRIG_TABLE_MULT;

	// find the index and the table value
	int index = ((int)realToInt32_chop(value)) & TRIG_INDEX_MASK;
	value = -s_SinCosTable[index];

	// toggle the sign of the result 
	// based on the sign of the index
	*((int*)&value) ^= (index&80000000);

	return(value);
}

//
// Remember, the Square Root of zero is undefined. FastSquareRoot will give a much different result than
// sqrt for zero. (sqrt returns zero, FastSquareRoot returns 7.66647e-020)
//
inline float FastSquareRoot (float n)
{
#ifdef PLATFORM_IEEE_FLOAT
  *(int*)&n = fast_sqrt_table[(*(int *)&n >> 8) & 0xFFFF] ^ ((((*(int*)&n - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
  return n;
#else
  return (SquareRoot(n));
#endif
}

extern bool InitFastMathTables();
extern void DestroyFastMathTables();

extern float FastSquareRoot (float n);

//
// INLINE MATH AND UNIT CONVERSION FUNCTIONS
//

//:	Sine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Sine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Sine(T angle)
{
	return fast_cast<T>(sinf(fast_cast<float>(angle))); 
}

template <> inline int32 Sine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(sinf(fast_cast<float>(angle))); 
}


//:	ArcSine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the ArcSine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T ArcSine(T angle)
{
	return fast_cast<T>(asinf(fast_cast<float>(angle))); 
}

template <> inline int32 ArcSine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(asinf(fast_cast<float>(angle))); 
}

//:	HypSine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Hyperbolic Sine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T HypSine(T angle)
{
	return fast_cast<T>(sinhf(fast_cast<float>(angle))); 
}

template <> inline int32 HypSine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(sinhf(fast_cast<float>(angle))); 
}

//:	Cosine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Cosine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Cosine(T angle)
{
	return fast_cast<T>(cosf(fast_cast<float>(angle))); 
}

template <> inline int32 Cosine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(cosf(fast_cast<float>(angle))); 
}

//:	ArcCosine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the ArcCosine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T ArcCosine(T angle)
{
	return fast_cast<T>(acosf(fast_cast<float>(angle))); 
}

template <> inline int32 ArcCosine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(acosf(fast_cast<float>(angle))); 
}

//:	HypCosine
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Hyperbolic Cosine of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T HypCosine(T angle)
{
	return fast_cast<T>(coshf(fast_cast<float>(angle))); 
}

template <> inline int32 HypCosine(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(coshf(fast_cast<float>(angle))); 
}

//:	Tangent
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Tangent of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Tangent(T angle)
{
	return fast_cast<T>(tanf(fast_cast<float>(angle))); 
}

template <> inline int32 Tangent(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(tanf(fast_cast<float>(angle))); 
}

//:	ArcTangent
//----------------------------------------------------------------------------------------
//
//	Calcualtes the ArcTangent of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T ArcTangent(T angle)
{
	return fast_cast<T>(atanf(fast_cast<float>(angle))); 
}

template <> inline int32 ArcTangent(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(atanf(fast_cast<float>(angle))); 
}

//:	HypTangent
//----------------------------------------------------------------------------------------
//
//	Calcualtes the Hyperbolic Tangent of a radian angle 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T HypTangent(T angle)
{
	return fast_cast<T>(tanhf(fast_cast<float>(angle))); 
}

template <> inline int32 HypTangent(int32 angle)
{
	// use fast double to integer conversion
	return realToInt32_chop(tanhf(fast_cast<float>(angle))); 
}

//:	ArcTangentOfRatio
//----------------------------------------------------------------------------------------
//
//	Calcualtes the ArcTangent of the ratio Numerator/Denominator 
//
//-------------------------------------------------------------------------------------://
template <class T> inline T ArcTangentOfRatio(T Numerator, T Denominator)
{
	return fast_cast<T>(atan2f(fast_cast<float>(Numerator), fast_cast<float>(Denominator))); 
}

template <> inline int32 ArcTangentOfRatio(int32 Numerator, int32 Denominator)
{
	// use fast double to integer conversion
	return realToInt32_chop(atan2f(fast_cast<float>(Numerator), fast_cast<float>(Denominator))); 
}

//:	Log
//----------------------------------------------------------------------------------------
//
//	Calculates the natural logarithm of a given value
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Log(T value)
{
	return fast_cast<T>(logf(fast_cast<float>(value))); 
}

//:	Log10
//----------------------------------------------------------------------------------------
//
//	Calculates the base-10 logarithm of a given value
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Log10(T value)
{
	return fast_cast<T>(log10f(fast_cast<float>(value))); 
}

//:	RaiseToPower
//----------------------------------------------------------------------------------------
//
//	Calculates Value raised to the power of Power
//
//-------------------------------------------------------------------------------------://
template <class T> inline T RaiseToPower(T value, T power)
{
	return fast_cast<T>(powf(fast_cast<float>(value), fast_cast<float>(power))); 
}


//:	SquareRoot
//----------------------------------------------------------------------------------------
//
//	Calculates the SquareRoot of a given value
//
//-------------------------------------------------------------------------------------://
template <class T> inline T SquareRoot(T value)
{
	return fast_cast<T>(sqrtf(fast_cast<float>(value))); 
}

//:	Square
//----------------------------------------------------------------------------------------
//
//	Calculates the Square of a given value
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Square(T value)
{
	return (value * value);
}

//:	Hypotenuse
//----------------------------------------------------------------------------------------
//
//	Returns the length of the hypotenuse of a right triangle with sides of length A & B
//
//-------------------------------------------------------------------------------------://
template <class T> inline T Hypotenuse(T SideA, T SideB)
{
	return fast_cast<T>(SquareRoot( fast_cast<float>(Square(SideA) + Square(SideB))));
}

//:	CalcSide
//----------------------------------------------------------------------------------------
//
//	Returns the length of the side of a right triangle given a side and Hypotenuse
//
//-------------------------------------------------------------------------------------://
template <class T> inline T CalcSide(T Side, T Hypotenuse)
{
	return fast_cast<T>(SquareRoot( fast_cast<float>(Square(Hypotenuse) - Square(Side))));
}

//:	DegToRad
//----------------------------------------------------------------------------------------
//
//	Converts a value represented in Degrees to Radians
//
//-------------------------------------------------------------------------------------://
template <class T> inline T DegToRad(T Degrees)
{
	return fast_cast<T>(fast_cast<float>(Degrees) * PI_OVER_180); 
}

//:	RadToDeg
//----------------------------------------------------------------------------------------
//
//	Converts a value represented in Radians to Degrees
//
//-------------------------------------------------------------------------------------://
template <class T> inline T RadToDeg(T Radians)
{
	return fast_cast<T>(fast_cast<float>(Radians) * PI_DIV_180); 
}


} //End NameSpace: gaia

#endif  // end of file      ( Math.h )

