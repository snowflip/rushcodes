#ifndef VECTOR_OPERATORS_H
#define VECTOR_OPERATORS_H
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

//	Name Space declaration
namespace gaia
{


// scale
 
inline const cVector2& cVector2::operator *= (const cVector2& vec)
{
	x *= vec.x;
	y *= vec.y;
	return(*this);
}
 
inline const cVector3& cVector3::operator *= (const cVector3& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return(*this);
}
 
inline const cVector4& cVector4::operator *= (const cVector4& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;	
	return(*this);
}
		
inline const cVector2& cVector2::operator *= (float v)
{
	x *= v;
	y *= v;
	return(*this);
}
 
inline const cVector3& cVector3::operator *= (float v)
{
	x *= v;
	y *= v;
	z *= v;
	return(*this);
}
 
inline const cVector4& cVector4::operator *= (float v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;	
	return(*this);
}


// divide
 
inline const cVector2& cVector2::operator /= (const cVector2& vec)
{
	debug_assert(vec.x != 0.0f, "divide by zero error");
	debug_assert(vec.y != 0.0f, "divide by zero error");

	x /= vec.x;
	y /= vec.y;
	return(*this);
}
 
inline const cVector3& cVector3::operator /= (const cVector3& vec)
{
	debug_assert(vec.x != 0.0f, "divide by zero error");
	debug_assert(vec.y != 0.0f, "divide by zero error");
	debug_assert(vec.z != 0.0f, "divide by zero error");

	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return(*this);
}
 
inline const cVector4& cVector4::operator /= (const cVector4& vec)
{
	debug_assert(vec.x != 0.0f, "divide by zero error");
	debug_assert(vec.y != 0.0f, "divide by zero error");
	debug_assert(vec.z != 0.0f, "divide by zero error");
	debug_assert(vec.w != 0.0f, "divide by zero error");

	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	w /= vec.w;	
	return(*this);
}

inline const cVector2& cVector2::operator /= (float v)
{
	debug_assert(v != 0.0f, "divide by zero error");

	x /= v;
	y /= v;
	return(*this);
}
 
inline const cVector3& cVector3::operator /= (float v)
{
	debug_assert(v != 0.0f, "divide by zero error");

	x /= v;
	y /= v;
	z /= v;
	return(*this);
}
 
inline const cVector4& cVector4::operator /= (float v)
{
	debug_assert(v != 0.0f, "divide by zero error");

	x /= v;
	y /= v;
	z /= v;
	w /= v;	
	return(*this);
}
//
// Stand-Alone Vector Operations
//

/*	operator ==
------------------------------------------------------------------------------------------
	
	Equivalence operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline bool operator == (const cVector2 &a, const cVector2 &b)
{
	return((a.x==b.x) && (a.y==b.y));
}

inline bool operator == (const cVector3 &a, const cVector3 &b)
{
	return((a.x==b.x) && (a.y==b.y) && (a.z==b.z));
}

inline bool operator == (const cVector4 &a, const cVector4 &b)
{
	return((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w));
}

/*	operator !=
------------------------------------------------------------------------------------------
	
	Equivalence operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline bool operator != (const cVector2 &a, const cVector2 &b)
{
	return((a.x!=b.x) || (a.y!=b.y));
}

inline bool operator != (const cVector3 &a, const cVector3 &b)
{
	return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z));
}

inline bool operator != (const cVector4 &a, const cVector4 &b)
{
	return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w));
}

/*	operator -
------------------------------------------------------------------------------------------
	
	Negation operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator - (const cVector2 &a)
{
	return(cVector2(-a.x, -a.y));
}

inline cVector3 operator - (const cVector3 &a)
{
	return(cVector3(-a.x, -a.y, -a.z));
}

inline cVector4 operator - (const cVector4 &a)
{
	return(cVector4(-a.x, -a.y, -a.z, -a.w));
}


/*	operator + (vector,vector)
------------------------------------------------------------------------------------------
	
	Addition operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator + (const cVector2 &a, const cVector2 &b)
{
	return(cVector2((a.x + b.x), (a.y + b.y)));
}

inline cVector3 operator + (const cVector3 &a, const cVector3 &b)
{
	return(cVector3((a.x + b.x), (a.y + b.y), (a.z + b.z)));
}

inline cVector4 operator + (const cVector4 &a, const cVector4 &b)
{
	return(cVector4((a.x + b.x), (a.y + b.y), (a.z + b.z), (a.w + b.w)));
}

/*	operator - (vector,vector)
------------------------------------------------------------------------------------------
	
	Subtraction operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator - (const cVector2 &a, const cVector2 &b)
{
	return(cVector2((a.x - b.x), (a.y - b.y)));
}

inline cVector3 operator - (const cVector3 &a, const cVector3 &b)
{
	return(cVector3((a.x - b.x), (a.y - b.y), (a.z - b.z)));
}

inline cVector4 operator - (const cVector4 &a, const cVector4 &b)
{
	return(cVector4((a.x - b.x), (a.y - b.y), (a.z - b.z), (a.w - b.w)));
}

/*	operator * (float, vector)
------------------------------------------------------------------------------------------
	
	Multiplication operators for all vector types with a single value 
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator * (float scalar, const cVector2 &a)
{
	return(cVector2((a.x * scalar), (a.y * scalar)));
}

inline cVector2 operator * (const cVector2 &a, float scalar)
{
	return(cVector2((a.x * scalar), (a.y * scalar)));
}


inline cVector3 operator * (float scalar, const cVector3 &a)
{
	return(cVector3((a.x * scalar), (a.y * scalar), (a.z * scalar)));
}

inline cVector3 operator * (const cVector3 &a, float scalar)
{
	return(cVector3((a.x * scalar), (a.y * scalar), (a.z * scalar)));
}


inline cVector4 operator * (float scalar, const cVector4 &a)
{
	return(cVector4((a.x * scalar), (a.y * scalar), (a.z * scalar), (a.w * scalar)));
}

inline cVector4 operator * (const cVector4 &a, float scalar)
{
	return(cVector4((a.x * scalar), (a.y * scalar), (a.z * scalar), (a.w * scalar)));
}


/*	operator * (vector, vector)
------------------------------------------------------------------------------------------
	
	Multiplication operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator * (const cVector2 &a, const cVector2 &b)
{
	return(cVector2((a.x * b.x), (a.y * b.y)));
}

inline cVector3 operator * (const cVector3 &a, const cVector3 &b)
{
	return(cVector3((a.x * b.x), (a.y * b.y), (a.z * b.z)));
}

inline cVector4 operator * (const cVector4 &a, const cVector4 &b)
{
	return(cVector4((a.x * b.x), (a.y * b.y), (a.z * b.z), (a.w * b.w)));
}

/*	operator / (vector, float)
------------------------------------------------------------------------------------------
	
	Division operators for all vector types with a single value 
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator / (const cVector2 &a, float scalar)
{
	debug_assert(scalar != 0.0f, "divide by zero error");
	return(cVector2((a.x / scalar), (a.y / scalar)));
}


inline cVector3 operator / (const cVector3 &a, float scalar)
{
	debug_assert(scalar != 0.0f, "divide by zero error");
	return(cVector3((a.x / scalar), (a.y / scalar), (a.z / scalar)));
}


inline cVector4 operator / (const cVector4 &a, float scalar)
{
	debug_assert(scalar != 0.0f, "divide by zero error");
	return(cVector4((a.x / scalar), (a.y / scalar), (a.z / scalar), (a.w / scalar)));
}

/*	operator / (vector, vector)
------------------------------------------------------------------------------------------
	
	Division operators for all vector types
	
------------------------------------------------------------------------------------------
*/

inline cVector2 operator / (const cVector2 &a, const cVector2 &b)
{
	debug_assert(b.x != 0.0f, "divide by zero error");
	debug_assert(b.y != 0.0f, "divide by zero error");
	return(cVector2((a.x / b.x), (a.y / b.y)));
}

inline cVector3 operator / (const cVector3 &a, const cVector3 &b)
{
	debug_assert(b.x != 0.0f, "divide by zero error");
	debug_assert(b.y != 0.0f, "divide by zero error");
	debug_assert(b.z != 0.0f, "divide by zero error");
	return(cVector3((a.x / b.x), (a.y / b.y), (a.z / b.z)));
}

inline cVector4 operator / (const cVector4 &a, const cVector4 &b)
{
	debug_assert(b.x != 0.0f, "divide by zero error");
	debug_assert(b.y != 0.0f, "divide by zero error");
	debug_assert(b.z != 0.0f, "divide by zero error");
	debug_assert(b.w != 0.0f, "divide by zero error");
	return(cVector4((a.x / b.x), (a.y / b.y), (a.z / b.z), (a.w / b.w)));
}


//- End of Vector_Operators --------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( Vector_Operators.h )

