#ifndef cMatrix4x4_H
#define cMatrix4x4_H
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
#include <core/core.h>

#ifndef VECTOR_H
#include "vector.h"
#endif


//	Name Space declaration
namespace gaia
{


/*	cMatrix4x4
------------------------------------------------------------------------------------------
	
	A Matrix of four x four cells. 
	
------------------------------------------------------------------------------------------
*/

class cMatrix4x4 : public D3DXMATRIX
{
public:


	// Creators...

	cMatrix4x4();
    explicit cMatrix4x4(const cMatrix4x4& Src);
    explicit cMatrix4x4(const float* _16_floats);
    explicit cMatrix4x4(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44);
	~cMatrix4x4();

    // new operators
    void* operator new   ( size_t );
    void* operator new[] ( size_t );

    // delete operators
    void operator delete   ( void* );   // These are NOT virtual; Do not 
    void operator delete[] ( void* );   // cast to D3DXMATRIX and delete.

	// Mutators...
	void SetIdentity();

	void Transpose();
	void Transpose(const cMatrix4x4& matrix);
	void Transpose(const cMatrix4x4* matrix);
	bool invert();
	bool invert( const cMatrix4x4& matrix);
	bool invert( const cMatrix4x4* matrix);

    void set(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44);

	uint32 load(cFile& InputFile);
	uint32 save(cFile& OutputFile);

	void getRow(uint8 i, cVector4* pVector);

	// Accessors...

private:

	// Private Data...

	// Private Functions...

};

//- Inline Functions ---------------------------------------------------------------------

//. Creators .............................................................................

/*	cMatrix4x4
------------------------------------------------------------------------------------------
	
	Default Constructor
	
------------------------------------------------------------------------------------------
*/
inline cMatrix4x4::cMatrix4x4()
{
}

/*	cMatrix4x4(const cMatrix4x4& Src)
------------------------------------------------------------------------------------------
	
	Copy Constructor
	
------------------------------------------------------------------------------------------
*/
inline cMatrix4x4::cMatrix4x4(const cMatrix4x4& Src)
:D3DXMATRIX((const D3DXMATRIX&) Src)
{
}

inline cMatrix4x4::cMatrix4x4(const float* _16_floats)
:D3DXMATRIX((FLOAT*) _16_floats)
{
}

inline cMatrix4x4::cMatrix4x4(float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44)
:D3DXMATRIX(_11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44)
{
}

/*	~cMatrix4x4
------------------------------------------------------------------------------------------
	
	Default Destructor
	
------------------------------------------------------------------------------------------
*/
inline cMatrix4x4::~cMatrix4x4()
{
}

//. Operators ............................................................................

// new operators
inline void* cMatrix4x4::operator new   ( size_t size)
{
	return alloc_aligned_memory(size,memory::ALIGN_16);
}

inline void* cMatrix4x4::operator new[] ( size_t size)
{
	return alloc_aligned_memory(size,memory::ALIGN_16);
}

// delete operators
inline void cMatrix4x4::operator delete   ( void* pMem)   // These are NOT virtual; Do not 
{
	if(pMem)
	{
		free_memory(pMem);
	}
}

inline void cMatrix4x4::operator delete[] ( void* pMem)   // cast to D3DXMATRIX and delete.
{
	if(pMem)
	{
		free_memory(pMem);
	}
}

/*	Casting Operators
------------------------------------------------------------------------------------------
	
	Cast this type to any of the types provided below
	
------------------------------------------------------------------------------------------
*/

//. Mutators .............................................................................


/*	SetIdentity()
------------------------------------------------------------------------------------------
	
	set this matrix to the identity matrix.
	
------------------------------------------------------------------------------------------
*/
inline void cMatrix4x4::SetIdentity()
{
	D3DXMatrixIdentity(this);
}

inline void cMatrix4x4::set(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	_11 = m11;
	_12 = m12;
	_13 = m13;
	_14 = m14;
	_21 = m21;
	_22 = m22;
	_23 = m23;
	_24 = m24;
	_31 = m31;
	_32 = m32;
	_33 = m33;
	_34 = m34;
	_41 = m41;
	_42 = m42;
	_43 = m43;
	_44 = m44;
}

/*	SetTranslation(float x, float y, float z)
------------------------------------------------------------------------------------------
	
	set this matrix to the translation parameters provided.
	
------------------------------------------------------------------------------------------
*/
/*
inline void cMatrix4x4::SetTranslation(float x, float y, float z)
{
	D3DXMatrixTranslation(this, x, y, z);
}

inline void cMatrix4x4::SetTranslation(const cVector3& vec)
{
	D3DXMatrixTranslation(this, vec.x, vec.y, vec.z);
}

inline void cMatrix4x4::SetTranslation(const cVector4& vec)
{
	D3DXMatrixTranslation(this, vec.x, vec.y, vec.z);
}

inline void cMatrix4x4::SetTranslation(const cVector3* vec)
{
	D3DXMatrixTranslation(this, vec->x, vec->y, vec->z);
}

inline void cMatrix4x4::SetTranslation(const cVector4* vec)
{
	D3DXMatrixTranslation(this, vec->x, vec->y, vec->z);
}
*/

/*	SetScale(float x, float y, float z)
------------------------------------------------------------------------------------------
	
	set this matrix to the scale parameters provided.
	
------------------------------------------------------------------------------------------
*/
/*
inline void cMatrix4x4::SetScale(float x, float y, float z)
{
	D3DXMatrixScale(this, x, y, z);
}

inline void cMatrix4x4::SetScale(const cVector3& vec)
{
	D3DXMatrixScale(this, vec.x, vec.y, vec.z);
}

inline void cMatrix4x4::SetScale(const cVector4& vec)
{
	D3DXMatrixScale(this, vec.x, vec.y, vec.z);
}

inline void cMatrix4x4::SetScale(const cVector3* vec)
{
	D3DXMatrixScale(this, vec->x, vec->y, vec->z);
}

inline void cMatrix4x4::SetScale(const cVector4* vec)
{
	D3DXMatrixScale(this, vec->x, vec->y, vec->z);
}
*/

/*	Transpose()
------------------------------------------------------------------------------------------
	
	swap the matrix members along the major diagonal.
	
------------------------------------------------------------------------------------------
*/
inline void cMatrix4x4::Transpose()
{
	swap(m[0][1], m[1][0]);
	swap(m[0][2], m[2][0]);
	swap(m[0][3], m[3][0]);
	swap(m[1][2], m[2][1]);
	swap(m[1][3], m[3][1]);
	swap(m[2][3], m[3][2]);
}

inline void cMatrix4x4::Transpose(const cMatrix4x4& matrix)
{
	D3DXMatrixTranspose(this, &matrix);
}

inline void cMatrix4x4::Transpose(const cMatrix4x4* matrix)
{
	D3DXMatrixTranspose(this, matrix);
}

/*	invert( const cMatrix4x4& matrix)
------------------------------------------------------------------------------------------
	
	set the local matrix to the inverted version of the provided matrix. Returns false if
	no inverse can be calculated.
	
------------------------------------------------------------------------------------------
*/
inline bool cMatrix4x4::invert( const cMatrix4x4& matrix)
{
	float determinant;
	return 0 != D3DXMatrixInverse(this,&determinant, &matrix);
};

inline bool cMatrix4x4::invert( const cMatrix4x4* matrix)
{
	float determinant;
	return 0 != D3DXMatrixInverse(this,&determinant, matrix);
}

inline bool cMatrix4x4::invert()
{
	float determinant;
	return 0 != D3DXMatrixInverse(this,&determinant, this);
}

/*	load(cFile& InputFile)
------------------------------------------------------------------------------------------
	
	Loads the class data from the file specified at the current file position.
	
------------------------------------------------------------------------------------------
*/
inline uint32 cMatrix4x4::load(cFile& InputFile)
{
	InputFile.read(this, sizeof(cMatrix4x4));

	return(sizeof(cMatrix4x4));
}

/*	save(cFile& OutputFile)
------------------------------------------------------------------------------------------
	
	Saves the class data to the file specified at the current file position.
	
------------------------------------------------------------------------------------------
*/
inline uint32 cMatrix4x4::save(cFile& OutputFile)
{
	OutputFile.write(this, sizeof(cMatrix4x4));

	return(sizeof(cMatrix4x4));
}

inline void cMatrix4x4::getRow(uint8 i, cVector4* pVector)
{
	debug_assert(i<4, "invalid row index");

	float* pData = &_11;
	pData += i*4;

	pVector->x = pData[0];
	pVector->y = pData[1];
	pVector->z = pData[2];
	pVector->w = pData[3];

}

#define _cSIMDMatrix D3DX_ALIGN16 cMatrix4x4
//typedef _cSIMDMatrix cSIMDMatrix;

D3DX_ALIGN16 class cSIMDMatrix : public cMatrix4x4
{
public:


	// Creators...

	cSIMDMatrix(){};
	~cSIMDMatrix(){};
   explicit cSIMDMatrix(const cSIMDMatrix& Src);
    explicit cSIMDMatrix(const cMatrix4x4& Src);
    explicit cSIMDMatrix(const D3DXMATRIX& Src);
    explicit cSIMDMatrix(const float* _16_floats);
    explicit cSIMDMatrix(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44);

};

inline cSIMDMatrix::cSIMDMatrix(const cSIMDMatrix& Src)
:cMatrix4x4((const cMatrix4x4&) Src)
{
}
inline cSIMDMatrix::cSIMDMatrix(const cMatrix4x4& Src)
:cMatrix4x4((const cMatrix4x4&) Src)
{
}
inline cSIMDMatrix::cSIMDMatrix(const D3DXMATRIX& Src)
:cMatrix4x4((const cMatrix4x4&) Src)
{
}

inline cSIMDMatrix::cSIMDMatrix(const float* _16_floats)
:cMatrix4x4((FLOAT*) _16_floats)
{
}

inline cSIMDMatrix::cSIMDMatrix(float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44)
:cMatrix4x4(_11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44)
{
}

inline cMatrix4x4* MatrixMultiply(cMatrix4x4* dest, const cMatrix4x4* src_a, const cMatrix4x4* src_b)
{
	D3DXMatrixMultiply(dest, src_a, src_b);
	return dest;
}

//. Accessors ............................................................................



//- End of cMatrix4x4 ---------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cMatrix4x4.h )

