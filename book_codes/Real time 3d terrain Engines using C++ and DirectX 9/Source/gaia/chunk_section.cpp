#define CCHUNKSECTION_CPP
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

#include "chunk_section.h"
#include "chunk_system.h"
#include "camera.h"
#include "game_host.h"

using namespace gaia;

#define LEVEL_SIDE_LENGTH(i) (1<<i)
#define LEVEL_COUNT(i) (LEVEL_SIDE_LENGTH(i)*LEVEL_SIDE_LENGTH(i))
#define SKIRT_HEIGHT 50.0f

// Mutators...
bool cChunkTerrainSection::create(
	cSceneNode* pRootNode,
	cTerrain* pParentSystem, 
	uint16 sectorX, uint16 sectorY,
	uint16 heightMapX, uint16 heightMapY,
	uint16 xVerts, uint16 yVerts, 
	const cRect2d& worldRect)
{
	// build the base class section
	bool result= cTerrainSection::create(pRootNode,
							 pParentSystem, 
							 sectorX, sectorY, 
							 heightMapX, heightMapY,
							 xVerts, yVerts, 
							 worldRect);


	m_totalLevels = (uint16)chunkTerrain()->totalLOD();

	// allocate the errorMetric tree
	for (int i=0; i<k_maxDetailLevels;++i)
	{
		 m_errorMetricTree[i] = new float[LEVEL_COUNT(i)];
	}

	// build the errorMetric trees
	buildErrorMetricTree();

	return result;
}

void cChunkTerrainSection::destroy()
{
	for (int i=0; i<k_maxDetailLevels;++i)
	{
		 safe_delete_array(m_errorMetricTree[i]);
	}

	cTerrainSection::destroy();
}

void cChunkTerrainSection::buildErrorMetricTree()
{
	// the sector shift tells us how large our 
	// root node is in terms of vertices
	int shift = m_pTerrainSystem->sectorShift();
	int stride = (1<<shift)+1;

	// this information is used to setup our initial
	// step size and vertex count information
	int stepSize = 
		stride>>k_minTessellationShift;
	int vertCount = 
		(1<<k_minTessellationShift)+1;

	// we can now step through the levels
	// of detail and determine an error
	// metric for each node of the quad
	// tree. This data is stored in the
	// error metric tree for later use
	int i;
	for (i=m_totalLevels-1; i>=0;--i)
	{
		int localStep = (stepSize>>i);
		int xSpan = (vertCount-1)*localStep;
		int ySpan = (vertCount-1)*localStep;

		int side_count = LEVEL_SIDE_LENGTH(i);

		for (int y=0; y<side_count;++y)
		{
			for (int x=0; x<side_count;++x)
			{
				// compute the local errorMetric.
				// m_heightMapX and m_heightMapY
				// are the pixel location in the
				// height map for this section 
				float errorMetric = 
					 m_pTerrainSystem->computeErrorMetricOfGrid(
						vertCount,	// width of grid
						vertCount,	// height of grid
						localStep-1,	// horz vertex count per cell
						localStep-1,	// vert vertex count per cell
						m_heightMapX+
						(x*xSpan),	// starting index X
						m_heightMapY+
						(y*ySpan));// starting index Y

				// max with the errorMetric of our children
				if (i+1 < m_totalLevels)
				{
					int nextLevel = i+1;
					int nX = x<<1;
					int nY = y<<1;
					int dim = side_count<<1;

					errorMetric = maximum(
						errorMetric,
						m_errorMetricTree[nextLevel][(nY*dim)+nX]);
					errorMetric = maximum(
						errorMetric,
						m_errorMetricTree[nextLevel][(nY*dim)+nX+1]);
					errorMetric = maximum(
						errorMetric,
						m_errorMetricTree[nextLevel][((nY+1)*dim)+nX]);
					errorMetric = maximum(
						errorMetric,
						m_errorMetricTree[nextLevel][((nY+1)*dim)+nX+1]);
				}

				m_errorMetricTree[i][(y*side_count)+x] = 
					errorMetric;
			}
		}
	}
}

void cChunkTerrainSection::prepareForRender()
{
	cCamera* pCamera = TheGameHost.activeCamera();

	// compute a 2d point for each corner of the section
	cVector2 corner0(m_worldRect.x0, m_worldRect.y0);
	cVector2 corner1(m_worldRect.x0, m_worldRect.y1);
	cVector2 corner2(m_worldRect.x1, m_worldRect.y1);
	cVector2 corner3(m_worldRect.x1, m_worldRect.y0);

	cVector2 viewPoint= pCamera->worldPosition().vec2();

	// compute view distance to our 4 corners
	float distance0 = viewPoint.distance(corner0);
	float distance1 = viewPoint.distance(corner1);
	float distance2 = viewPoint.distance(corner2);
	float distance3 = viewPoint.distance(corner3);

	//clear the render list
	m_totalRenderEntries=0;

	// recursivly tessellate and add 
	// to the internal render list
	recursiveTessellate( 
		distance0, distance1, distance2, distance3,
		0, 0, 0,
		chunkTerrain()->lodErrorScale(),
		chunkTerrain()->lodRatioLimit());
}

void cChunkTerrainSection::recursiveTessellate( 
	float distA, float distB, float distC, float distD,
	int level, int levelX, int levelY,
	float vScale, float vLimit)
{
	bool split = false;

	// can we attempt to split?
	if (level+1 < m_totalLevels)
	{
		int index = (levelY*LEVEL_SIDE_LENGTH(level))+levelX;
		float errorMetric = m_errorMetricTree[level][index];

		// find the shortest distance
		float dist = minimum(distA, distB);
		dist = minimum(dist, distC);
		dist = minimum(dist, distD);

		// find the ratio of errorMetric over distance
		float vRatio = (errorMetric*vScale)/(dist+0.0001f);

		// if we exceed the ratio limit, split
		if (vRatio > vLimit)
		{
			int nextLevel = level+1;
			int startX = levelX<<1;
			int startY = levelY<<1;

			// compute midpoint distances
			float midAB = (distA + distB)*0.5f;
			float midBC = (distB + distC)*0.5f;
			float midCD = (distC + distD)*0.5f;
			float midDA = (distD + distA)*0.5f;
			float midQuad = (distA + distC)*0.5f;

			// recurse through the four children
			recursiveTessellate(
				distA, midAB, midQuad, midDA,
				nextLevel, startX, startY,
				vScale, vLimit);

			recursiveTessellate(
				midAB, distB, midBC, midQuad,
				nextLevel, startX, startY+1,
				vScale, vLimit);

			recursiveTessellate(
				midBC, distC, midCD, midQuad,
				nextLevel, startX+1, startY+1,
				vScale, vLimit);

			recursiveTessellate(
				midAB, midQuad, midCD, distD,
				nextLevel, startX+1, startY,
				vScale, vLimit);

			// remember that we split
			split = true;

		}
	}

	// did we split?
	if (!split)
	{
		// add ourselves to the render list
		if (m_totalRenderEntries < k_maxRenderEntries)
		{
			sRenderEntry& entry = 
				m_renderList[m_totalRenderEntries++];

			int lodShift = 5 - level;

			entry.level = level;
			entry.offsetX = (levelX<<lodShift);
			entry.offsetY = (levelY<<lodShift);
		}
	}

}


bool cChunkTerrainSection::buildVertexBuffer()
{
	bool result = true;
	//
	// Build a vertex buffer and determine
	// the min\max size of the sector
	//
	cString tempName;
	tempName.format(
		"terrain_section_%i_%i", 
		m_sectorX, 
		m_sectorY);

	m_pSectorVerts = 
		TheGameHost.displayManager().
		vertexBufferPool().createResource(tempName);

	m_worldRect.z0 = MAX_REAL32;
	m_worldRect.z1 = MIN_REAL32;

	//
	// for chunked terrain, we build a vertex buffer
	// with twice as many entries as normal. These
	// are called 'pages'. The first page is the
	// true vertex buffer, the second page is an
	// offset version of each vertex used when 
	// rendering the skirts
	//
	uint32 pageSize = m_xVerts*m_yVerts;
	uint32 bufferSize = pageSize<<1;

	if (m_pSectorVerts)
	{
		// read in the height and normal for each vertex
		cTerrain::sSectorVertex* pVerts = 
			new cTerrain::sSectorVertex[bufferSize];

		for (uint16 y = 0; y<m_yVerts; ++y)
		{
			for (uint16 x = 0; x<m_xVerts; ++x)
			{
				float height = 
					m_pTerrainSystem->readWorldHeight(
						m_heightMapX+x, 
						m_heightMapY+y);
				cVector3 normal = 
					m_pTerrainSystem->readWorldNormal(
						m_heightMapX+x, 
						m_heightMapY+y);

				int vertIndex = (y*m_xVerts)+x;
				pVerts[vertIndex].height = 
					height;
				pVerts[vertIndex].normal = 
					normal;

				height -= SKIRT_HEIGHT;

				pVerts[vertIndex+pageSize].height = 
					height;
				pVerts[vertIndex+pageSize].normal = 
					normal;

				m_worldRect.z0 = 
					minimum(m_worldRect.z0, height);
				m_worldRect.z1 = 
					maximum(m_worldRect.z1, height);
			}
		}

		result = result &&
			m_pSectorVerts->create(
			(uint16)bufferSize, 
			sizeof(cTerrain::sSectorVertex), 
			FLAG(cVertexBuffer::nRamBackupBit),
			pVerts);

		safe_delete_array(pVerts);
	}
	else
	{
		result = false;
	}

	return result;
}
//***************************************************************
// end of file      ( cChunkTerrainSection.cpp )

//----------------------------------------------------------
//$Log: $