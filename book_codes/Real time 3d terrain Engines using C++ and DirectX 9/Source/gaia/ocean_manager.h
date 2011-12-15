#ifndef OCEANSYSTEM_H
#define OCEANSYSTEM_H
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
#include "math\math.h"
#include "geometry\geometry.h"
#include "scene_object.h"
#include "vertex_buffer.h"
#include "index_buffer.h"


//	Name Space declaration
namespace gaia
{



class cOceanManager
{
public:

	enum eProcessingStages
	{
		k_animateHeight = 0,
		k_animateNormal,
		k_heightFFTv,
		k_heightFFTh,
		k_normalFFTv,
		k_normalFFTh,
		k_uploadBuffer,
		k_rotateBuffers,
		k_total_process_stages,
	};

	enum eWaterMeshes
	{
		k_total_water_meshes = 3
	};
	// typedef floating-point complex numbers
	// to hold pairs of real 
	// and imaginary numbers
	struct sComplex
	{
		float real;
		float imag;

		sComplex& operator = (const sComplex& src)
		{
			real = src.real;
			imag = src.imag;
			return *this;
		}
	};

	enum eConstants
	{
		k_grid_size = 64,
		k_half_grid_size = 32,
		k_log_grid_size = 6,
		k_grid_area = 64*64,
		k_freq_area = 65*65,
	};

	struct sBaseVertex
	{
		cVector2 vPos;
	};
	struct sAnimVertex
	{
		float zPos;
		cVector2 vNormal;
	};

	// Creators...
	cOceanManager();
	~cOceanManager();

	// Mutators...
	bool create();
	void destroy();
	void update();

	void setRenderMethod(cRenderMethod* pMethod);
	void render();

	void setOceanParameters( 
		unsigned long ulSeed, 
		float fWindDirection, 
		float fWindSpeed,
        float fWaveHeight, 
		float fDirectionalDependence, 
		float fSuppressSmallWavesFactor );

	cIndexBuffer* indexBuffer()const;
	cVertexBuffer* vertexBuffer()const;
	cRenderMethod* renderMethod()const;

private:

	cIndexBuffer* m_pIndexBuffer;
	cVertexBuffer* m_pBaseBuffer;
	cVertexBuffer* m_pAnimBuffer[k_total_water_meshes];
	cRenderMethod* m_pRenderMethod;

    float m_fWindX;
    float m_fWindY;
    float m_fWindSpeed;
    float m_fWaveHeight;
    float m_fDirectionalDependence;
    float m_fSuppressSmallWavesFactor;
    float m_fLargestPossibleWave; 
    float m_fSuppressSmallWaves;

	int m_activeBuffer;
	int m_tickCounter;
	float m_fInterpolation;
	float m_fTime;

    sComplex m_colH0[k_freq_area]; // base freq. and amplitudes
    sComplex m_colH[k_grid_area];	// height values
    sComplex m_colN[k_grid_area]; // normals
	float m_colAngularFreq[k_grid_area]; // angular frequencies

	float getOceanHeight(int x, int y);
	float getOceanNormalX(int x, int y);
	float getOceanNormalY(int x, int y);

	float getKx(int x);
	float getKy(int y);
	float getAngularFrequency(float fKLength);

	int getIndexH0(int x, int y);
	int getIndex(int x, int y);
	int getIndexFFT(int x, int y);
	int getOffset(int x, int y);
	int getOffsetWrap(int x, int y);

	float phillipsSpectrum(float fKx, float fKy);
	void FFT(sComplex* pCmpTable );
	void horizontalFFT(sComplex* pCmpTable );
	void verticalFFT(sComplex* pCmpTable );

	void animateHeightTable();
	void animateNormalTable();
	void fillVertexBuffer(cVertexBuffer* pVertexBuffer);
};

//- Inline Functions ---------------------------------------------------------------------

/*	cOceanManager
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cOceanManager::cOceanManager()
:m_pBaseBuffer(0)
,m_pIndexBuffer(0)
,m_pRenderMethod(0)
,m_tickCounter(0)
,m_activeBuffer(0)
{
	m_pAnimBuffer[0] = 0;
	m_pAnimBuffer[1] = 0;
}

/*	~cOceanManager
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cOceanManager::~cOceanManager()
{
	destroy();
}

inline cIndexBuffer* cOceanManager::indexBuffer()const
{
	return m_pIndexBuffer;
}

inline cRenderMethod* cOceanManager::renderMethod()const
{
	return m_pRenderMethod;
}

inline float cOceanManager::getOceanHeight( int x, int y )
{
	int index = getOffsetWrap(x, y);
    return(-m_colH[index].real);
}

inline float cOceanManager::getOceanNormalX( int x, int y )
{
	int index = getOffsetWrap(x, y);
    return( m_colN[index].real );
}
 
inline float cOceanManager::getOceanNormalY( int x, int y )
{
	int index = getOffsetWrap(x, y);
    return( m_colN[index].imag );
}

inline float cOceanManager::getKx( int x )
{
    return (TWO_PI * x );
}

inline float cOceanManager::getKy( int y )
{
    return (TWO_PI * y );
}

inline float cOceanManager::getAngularFrequency(float fKLength)
{
    // special version for deep water
    return( sqrtf( GRAVITY_CONSTANT * fKLength ) );

	// Note: If we wanted to take the water depth into account, we could
	// use the slower equation:
	// sqrtf( GRAVITY_CONSTANT * fKLength * tanhf( fKLength * depth ) )
}

inline int cOceanManager::getIndexH0( int x, int y )
{
    return( x + k_half_grid_size + ( k_grid_size + 1 ) * ( y + k_half_grid_size) );
}

inline int cOceanManager::getIndex( int x, int y )
{
    return( x + ( k_half_grid_size ) + k_grid_size * ( y + k_half_grid_size) );
}

inline int cOceanManager::getIndexFFT( int x, int y )
{
    // get the appropriate index to store data in the right order for the 2d fft
    // (that is, dc is stored at (0, 0) and not at ( k_half_grid_size, k_half_grid_size ) !!!)
    return( ( x & ( k_grid_size - 1 ) ) + k_grid_size * ( y & ( k_grid_size - 1 ) ) );
}

inline int cOceanManager::getOffset( int x, int y )
{
    return( x + k_grid_size * y );
}

inline int cOceanManager::getOffsetWrap( int x, int y )
{
    return( getOffset( x & ( k_grid_size - 1 ), y & ( k_grid_size - 1 ) ) );
}

//- End of cOceanManager -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cOceanManager.h )

//----------------------------------------------------------
//$Log: $