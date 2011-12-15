#define COCEANMANAGER_CPP
#define GAIA_DLL
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

#include "ocean_manager.h"
#include "game_host.h"

using namespace gaia;

static float ocean_tile_size = 2500.0f;
static float ocean_tile_height = 20.0f;

static D3DVERTEXELEMENT9 vertex_description[]=
{
	// base data (stream 0)
	{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

	// anim data (stream 1)
	{ 1, 0, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
	{ 1, 4, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },

	// anim data (stream 2)
	{ 2, 0, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 2 },
	{ 2, 4, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1 },
	D3DDECL_END()
};

//
// Default parameter values borrowed from
// Carsten Wenzel's implementation of
// deep ocean waves 
//
static const float s_fDefaultWindDirection( D3DX_PI * 0.5f );
static const float s_fDefaultWindSpeed( 32.0f );
static const float s_fDefaultWaveHeight( 384.0f );

// increasing this value will align the waves
// with the wind direction. The value MUST be
// a multiple of 2.0 and positive. 
// ie. range = (2, 2^32) in steps of 2.0f
static const float s_fDefaultDirectionalDependence( 4.0f );

// increasing this value will smooth the waves
static const float s_fDefaultSuppressSmallWavesFactor( 0.00005f );


bool cOceanManager::create()
{
	// set innitial parameters
    setOceanParameters( 
		4357, 
		s_fDefaultWindDirection, 
		s_fDefaultWindSpeed,
        s_fDefaultWaveHeight, 
		s_fDefaultDirectionalDependence,
		s_fDefaultSuppressSmallWavesFactor );

	m_pBaseBuffer = DisplayManager.vertexBufferPool().createResource("cOceanManager Base Buffer");
	m_pIndexBuffer = DisplayManager.indexBufferPool().createResource("cOceanManager Buffer");

	sBaseVertex* pBaseVerts = new sBaseVertex[k_grid_size*k_grid_size];
	sBaseVertex* pVertices = pBaseVerts;
	if (pVertices)
	{
		float u = 0.0f;
		float v = 0.0f;
		float uvStep = 
			1.0f/(float) ( k_grid_size - 1 );

		for( int y=0; y<k_grid_size; ++y )
		{
			for( int x=0; x<k_grid_size; ++x )
			{
				pVertices->vPos.x = u*ocean_tile_size;
				pVertices->vPos.y = v*ocean_tile_size;

				++pVertices;
				u += uvStep;
			}       
			v += uvStep;
			u = 0.0f;
		}
	}

	m_pBaseBuffer->create(
		k_grid_area, 
		sizeof(sBaseVertex), 
		0, 
		pBaseVerts);

	delete []pBaseVerts;

	// create the vertex description
	m_pBaseBuffer->setVertexDescription(7,vertex_description);

	m_pIndexBuffer->createSingleStripGrid(
		k_grid_size,
		k_grid_size,
		1,1,
		k_grid_size, 0);

	//
	// prepare for the first frame
	//
	m_fTime = 1.0f;
	animateHeightTable();
	animateNormalTable();
	horizontalFFT(m_colH );    
	horizontalFFT(m_colN );    
	verticalFFT(m_colH );    
	verticalFFT(m_colN );    
	m_tickCounter = 0;

	// build the dynamic vertex buffers
	for (int i=0; i<k_total_water_meshes; ++i)
	{
		cString name;
		name.format("cOceanManager Buffer %i", i);
		m_pAnimBuffer[i] = DisplayManager.vertexBufferPool().createResource(name);

		m_pAnimBuffer[i]->create(
			k_grid_area, 
			sizeof(sAnimVertex), 
			FLAG(cIndexBuffer::nDynamicOverwriteBit), 
			0);
		fillVertexBuffer(m_pAnimBuffer[i]);
	}


	return true;
}

void cOceanManager::destroy()
{
	safe_release(m_pIndexBuffer);
	safe_release(m_pBaseBuffer);
	safe_release(m_pRenderMethod);

	for (int i=0; i<k_total_water_meshes; ++i)
	{
		safe_release(m_pAnimBuffer[i]);
	}
}

float cOceanManager::phillipsSpectrum( float fKx, float fKy )
{
	// normalize the 2D vector defined by (fKx, fKy)
    float fKLength(SquareRoot((fKx*fKx) + (fKy*fKy)));
	fKLength = maximum(fKLength, 1e-8f);
    float fScale( 1.0f / fKLength );
    fKx *= fScale;
    fKy *= fScale;

	// perform the phillipsSpectrum calculation
    return ( m_fWaveHeight * 
             expf( -1.0f / Square( fKLength * m_fLargestPossibleWave ) - Square( fKLength * m_fSuppressSmallWaves ) ) * 
             powf( fKx * m_fWindX + fKy * m_fWindY, m_fDirectionalDependence ) / powf( fKLength, 4.0f ) );
}


void cOceanManager::setOceanParameters( 
	unsigned long ulSeed, 
	float fWindDirection, 
	float fWindSpeed,
	float fWaveHeight, 
	float fDirectionalDependence, 
	float fSuppressSmallWavesFactor )

{
    m_fWindX                    = Cosine( fWindDirection );    
    m_fWindY                    = Sine( fWindDirection );
    m_fWindSpeed                = fWindSpeed;
    m_fWaveHeight               = fWaveHeight;
    m_fDirectionalDependence    = fDirectionalDependence;
    m_fSuppressSmallWavesFactor = fSuppressSmallWavesFactor;

    m_fLargestPossibleWave =  m_fWindSpeed * m_fWindSpeed / GRAVITY_CONSTANT;   
    m_fSuppressSmallWaves  = m_fLargestPossibleWave * m_fSuppressSmallWavesFactor;

	// reset the global random seed
	set_random_seed(ulSeed);

    // init H0
    for( int j = -k_half_grid_size;j<= k_half_grid_size; ++j )
    {
        for( int i = -k_half_grid_size;i<= k_half_grid_size; ++i )
        {
            sComplex cmpRnd;
			GaussRandomPair(cmpRnd.real, cmpRnd.imag);

			float scalar = 
				INVERSE_ROOT_2 * 
				SquareRoot( phillipsSpectrum(getKx(i), getKy(j)));

            cmpRnd.real *= scalar;
			cmpRnd.imag *= scalar;

            m_colH0[ getIndexH0(i,j) ] = cmpRnd;
        }
    } 

    // init angular frequencies
    for(j= -k_half_grid_size;j< k_half_grid_size; ++j )
    {
        for( int i = -k_half_grid_size;i< k_half_grid_size; ++i )
        {
            float fKLength( SquareRoot(Square(getKx(i)) + Square(getKy(j))) );
            m_colAngularFreq[getIndex(i,j)] = getAngularFrequency(fKLength);
        }
    }
}

void cOceanManager::FFT(sComplex* pCmpTable)
{
    long nn,i,i1,j,k,i2,l,l1,l2;
    float c1,c2,treal,timag,t1,t2,u1,u2,z;
    
    nn = k_grid_size;
    
	sComplex* pCmpi;
	sComplex* pCmpj;
	sComplex* pCmpi1;

	// bit reversal
    i2 = nn >> 1;
    j= 0;
    for(i= 0;i< nn - 1; ++i )
    {
        if(i<j)
        {
			pCmpi = &pCmpTable[i];
			pCmpj = &pCmpTable[j];

            treal = pCmpi->real;
            timag = pCmpi->imag;
            pCmpi->real = pCmpj->real;
            pCmpi->imag = pCmpj->imag;
            pCmpj->real = treal;
            pCmpj->imag = timag;
        }

        k = i2;
        while( k <=j)
        {
           j-= k;
            k >>= 1;
        }

       j+= k;
    }
    
    // Compute the FFT
    c1 = -1.0f;
    c2 = 0.0f;
    l2 = 1;
    for( l = 0; l < k_log_grid_size; ++l )
    {
        l1 = l2;
        l2 <<= 1;
        u1 = 1.0;
        u2 = 0.0;
        for(j= 0;j< l1; ++j )
        {
            for(i= j;i< nn;i+= l2 )
            {
                i1 =i+ l1;

				pCmpi = &pCmpTable[i];
				pCmpi1 = &pCmpTable[i1];

                t1 = u1 * pCmpi1->real- u2 * pCmpi1->imag;
                t2 = u1 * pCmpi1->imag+ u2 * pCmpi1->real;
                pCmpi1->real = pCmpi->real - t1;
                pCmpi1->imag = pCmpi->imag - t2;
                pCmpi->real += t1;
                pCmpi->imag += t2;
            }

            z =  u1 * c1 - u2 * c2;
            u2 = u1 * c2 + u2 * c1;
            u1 = z;
        }

        c2 = fast_sqrt( ( 1.0f - c1 ) * 0.5f );
        c1 = fast_sqrt( ( 1.0f + c1 ) * 0.5f );
    }
}

void cOceanManager::horizontalFFT(sComplex* pCmpTable )
{
    // Transform the rows
	for( int j = 0;j< k_grid_size; ++j )
    {
		FFT(&pCmpTable[k_grid_size*j]);
    }
}

void cOceanManager::verticalFFT(sComplex* pCmpTable )
{
    sComplex tableCopy[ k_grid_size ];

    // Transform the columns
    for( int i = 0;i< k_grid_size; ++i )
    {
        for( int j = 0;j< k_grid_size; ++j )
        {
            tableCopy[j] = pCmpTable[ getOffset(i,j) ];
        }
        
        FFT(tableCopy);
        
        for(j= 0;j< k_grid_size; ++j )
        {
            pCmpTable[ getOffset(i,j) ] = tableCopy[j];
        }
    }
}

void cOceanManager::update()
{
	debug_assert(
		m_tickCounter >= k_animateHeight, 
		"invalid tick counter");
	debug_assert(
		m_tickCounter < k_total_process_stages, 
		"invalid tick counter");

	switch(m_tickCounter)
	{
		case k_animateHeight:
			m_fTime = 
				applicationTimer.elapsedTime()*0.15f;
			animateHeightTable();
			break;
		case k_animateNormal:
			animateNormalTable();
			break;
		case k_heightFFTv:
			verticalFFT(m_colH );    
			break;
		case k_heightFFTh:
			horizontalFFT(m_colH );    
			break;
		case k_normalFFTv:
			verticalFFT(m_colN );    
			break;
		case k_normalFFTh:
			horizontalFFT(m_colN );    
			break;
		case k_uploadBuffer:
			fillVertexBuffer(
				m_pAnimBuffer[m_activeBuffer]);
			break;
		default:
			++m_activeBuffer;
			if (m_activeBuffer 
				>= k_total_water_meshes)
			{
				m_activeBuffer = 0;
			}
			break;
	};

	// compute the interpolation factor for
	// the vertices currently in use
	m_fInterpolation = //0.0f;
		(float)(m_tickCounter+1)
		/(float)(k_total_process_stages);

	++m_tickCounter;
	if (m_tickCounter == k_total_process_stages)
	{
		m_tickCounter = k_animateHeight;
		m_fInterpolation = 0.0f;
	}
}

void cOceanManager::animateHeightTable()
{
	for(int j = -k_half_grid_size;
		j< k_half_grid_size; 
		++j )
    {
        for(int i = -k_half_grid_size;
			i< k_half_grid_size; 
			++i )
        {
            float fAngularFreq=
				m_colAngularFreq[ getIndex(i,j)] 
				* m_fTime;
            float fCos=FastCos(fAngularFreq);
            float fSin=FastSine(fAngularFreq);

			int indexFFT = getIndexFFT(i,j);
			int indexHO = getIndexH0(i,j);
			int indexHOn = getIndexH0(-i,-j);

            // update the tale of complex numbers 
			// affecting water surface height.
            m_colH[indexFFT].real= 
				( m_colH0[indexHO].real
					+ m_colH0[indexHOn].real ) * fCos -
                ( m_colH0[indexHO].imag 
					+ m_colH0[indexHOn].imag ) * fSin;

            m_colH[indexFFT].imag= 
				( m_colH0[indexHO].real
					- m_colH0[indexHOn].real ) * fSin + 
                ( m_colH0[indexHO].imag 
					- m_colH0[indexHOn].imag ) * fCos;
		}
	}
}

void cOceanManager::animateNormalTable()
{
	for( int j = -k_half_grid_size;j< k_half_grid_size; ++j )
    {
        for( int i = -k_half_grid_size;i< k_half_grid_size; ++i )
        {
            // update normal
            float fKx( getKx(i) );
            float fKy( getKy(j) );

            // Put the two functions for n(Kx) and n(Ky) into one complex array
            // (that is, multiply the n(Ky) by i and add it to n(Kx) ) to do one FFT instead of two.
            // This assumes that the inverse of both transforms are real functions (as it is the case here)!
            // After the FFT the x component of the normal will be stored in the real part, the y component 
            // in the complex part of m_colN respectively.
            m_colN[ getIndexFFT(i,j) ].real= 
				-m_colH[ getIndexFFT(i,j) ].imag * fKx - m_colH[ getIndexFFT(i,j) ].real * fKy;
            m_colN[ getIndexFFT(i,j) ].imag=  
				m_colH[ getIndexFFT(i,j) ].real * fKx - m_colH[ getIndexFFT(i,j) ].imag * fKy;
		}
	}
}

void cOceanManager::fillVertexBuffer(cVertexBuffer* pVertexBuffer)
{
	sAnimVertex* pStartVertex = 
		(sAnimVertex*)pVertexBuffer->lock(nWriteLock, 0, k_grid_area);

	sAnimVertex* pVertices = pStartVertex;
	if (pVertices)
	{
		for( int y=0; y<k_grid_size; ++y )
		{
			for( int x=0; x<k_grid_size; ++x )
			{
				pVertices->zPos = getOceanHeight(x, y) * ocean_tile_height;

				pVertices->vNormal.x = getOceanNormalX(x, y);
				pVertices->vNormal.y = getOceanNormalY(x, y);
			//	pVertices->vNormal.z = 24.0f;

			//	pVertices->vNormal.normalize();

				++pVertices;
			}       
		}

		// make sure all edge data is the same
		sAnimVertex* pLeftVert = pStartVertex;
		sAnimVertex* pTopVert = pStartVertex;
		sAnimVertex* pRightVert = &pStartVertex[k_grid_size-1];
		sAnimVertex* pBottomVert = &pStartVertex[(k_grid_size-1)*k_grid_size];
		sAnimVertex* pEndVert = &pStartVertex[(k_grid_size*k_grid_size)-1];

		pLeftVert->zPos = pRightVert->zPos;
		pLeftVert->vNormal = pRightVert->vNormal;
		pBottomVert->zPos = pRightVert->zPos;
		pBottomVert->vNormal = pRightVert->vNormal;
		pEndVert->zPos = pRightVert->zPos;
		pEndVert->vNormal = pRightVert->vNormal;

		for( int i=0; i<k_grid_size; ++i )
		{
			pLeftVert->zPos = pRightVert->zPos;
			pLeftVert->vNormal = pRightVert->vNormal;
			pTopVert->zPos = pBottomVert->zPos;
			pTopVert->vNormal = pBottomVert->vNormal;

			pTopVert++;
			pBottomVert++;
			pLeftVert += k_grid_size;
			pRightVert += k_grid_size;
		}

		pVertexBuffer->unlock();
	}
}

void cOceanManager::setRenderMethod(cRenderMethod* pMethod)
{
	safe_release(m_pRenderMethod);
	m_pRenderMethod = pMethod;

	if (m_pRenderMethod)
	{
		m_pRenderMethod->AddRef();
	}

}


void cOceanManager::render()
{
	const cCamera* pCamera = TheGameHost.activeCamera();

	static cVector4 uvOffset(
		1.0f,
		3.0f,
		0.0f,
		0.0f);
	uvOffset.z += 0.01f;
	uvOffset.w += 0.01f;

	// determine how many ocean tiles to draw to fill the camera view
	cRect3d world = pCamera->searchRect();
	
	// align the world rect to our ocean tile grid
	world.rect2d /= ocean_tile_size;
	int x0 = realToInt32_floor(world.x0);
	int x1 = realToInt32_ceiling(world.x1);
	int y0 = realToInt32_floor(world.y0);
	int y1 = realToInt32_ceiling(world.y1);

	if (m_pRenderMethod)
	{
		cEffectFile* pEffectFile = 
			m_pRenderMethod->getActiveEffect();
		cSurfaceMaterial* pSurfaceMaterial = 
			m_pRenderMethod->getActiveMaterial();

		if (pEffectFile)
		{
			// unlike regular objects, we do not render through the render
			// queue. Instead, we render on-demand using our internal settings
			if (pEffectFile->begin())
			{
				LPDIRECT3DDEVICE9 d3dDevice = 
					TheGameHost.d3dDevice();

				// supply our matrix to the render method
				pEffectFile->setMatrix(
					cEffectFile::k_worldViewProjMatrix, 
					&pCamera->viewProjMatrix());

				// supply our material to the render method
				pEffectFile->applySurfaceMaterial(
					pSurfaceMaterial);

				// activate the geometry buffers
				m_pIndexBuffer->activate();
				m_pBaseBuffer->activate(0, 0, true);
				
				int primary_buffer = m_activeBuffer -2;
				int secondary_buffer = m_activeBuffer -1;

				if (primary_buffer <0) primary_buffer += k_total_water_meshes;
				if (secondary_buffer <0) secondary_buffer += k_total_water_meshes;

				m_pAnimBuffer[primary_buffer]->activate(1, 0, false);
				m_pAnimBuffer[secondary_buffer]->activate(2, 0, false);

				pEffectFile->setParameter(
					cEffectFile::k_uvScaleOffset,
					&uvOffset);

				int totalPasses = pEffectFile->totalPasses();
				for (int i=0; i<totalPasses; ++i)
				{
					pEffectFile->activatePass(i);

					for (int y=y0; y<y1; ++y)
					{
						for (int x=x0; x<x1; ++x)
						{
							cVector4 scaleOffset(
								m_fInterpolation,
								1.0f/8000.0f,
								x * ocean_tile_size,
								y * ocean_tile_size);

							pEffectFile->setParameter(
								cEffectFile::k_posScaleOffset,
								&scaleOffset);

							HRESULT hr = d3dDevice->DrawIndexedPrimitive(
								m_pIndexBuffer->primitiveType(),
								0,
								0,
								k_grid_area,
								0,
								m_pIndexBuffer->primitiveCount());

						}
					}
				}
				pEffectFile->end();
			}
		}
	}
}

//***************************************************************
// end of file      ( cOceanManager.cpp )

//----------------------------------------------------------
//$Log: $