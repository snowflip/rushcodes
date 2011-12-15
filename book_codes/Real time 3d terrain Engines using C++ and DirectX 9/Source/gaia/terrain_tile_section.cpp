#define TERRAINSECTION_CPP
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

#include "terrain_tile_section.h"
#include "scene_node.h"
#include "model_resource.h"
#include "game_host.h"
#include "terrain_tile_system.h"
#include "camera.h"

using namespace gaia;

bool cTiledTerrainSection::create(cSceneNode* pRootNode,
							 cTerrain* pParentSystem, 
							 uint16 sectorX, uint16 sectorY, 
							 uint16 heightMapX, uint16 heightMapY,
							 uint16 xVerts, uint16 yVerts, 
							 const cRect2d& worldRect)
{
	bool result= cTerrainSection::create(pRootNode,
							 pParentSystem, 
							 sectorX, sectorY, 
							 heightMapX, heightMapY,
							 xVerts, yVerts, 
							 worldRect);

	
	// determine our error metrics
	computeErrorMetricTable();

	return result;
}

void cTiledTerrainSection::computeErrorMetricTable()
{
	int vertCount =1;
	int stepCount = 8;

	for (int lod=0; lod<k_totalDetailLevels; ++lod)
	{
		// compute the local errorMetric.
		// m_heightMapX and m_heightMapY
		// are the pixel location in the
		// height map for this section.
		// m_xVerts and m_yVerts are the
		// width and height of the section 
		// in vertices
		m_errorMetric[lod] = 
			 m_pTerrainSystem->computeErrorMetricOfGrid(
				vertCount+1,	// width of grid
				vertCount+1,	// height of grid
				stepCount-1,	// horz vertex count per cell
				stepCount-1,	// vert vertex count per cell
				m_heightMapX,	// starting index X
				m_heightMapY);// starting index Y

		vertCount <<=1;
		stepCount >>=1;
	}

	// make sure each error metric represents all the higher lods
	m_errorMetric[1] = maximum(m_errorMetric[1], m_errorMetric[2]);
	m_errorMetric[0] = maximum(m_errorMetric[0], m_errorMetric[1]);
}


void cTiledTerrainSection::destroy()
{
	cTerrainSection::destroy();
}


void cTiledTerrainSection::prepareForRender()
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

	// compute min distance as the test value
	float dist = minimum(distance0, distance1);
	dist = minimum(dist, distance2);
	dist = minimum(dist, distance3);

	// make sure the minimum distance is non-zero
	dist = maximum(dist, 0.0001f);

	// find the lowest lod which will suffice
	m_lod = 0;
	bool finished = false;

	float vScale = m_pTerrainSystem->lodErrorScale();
	float vLimit = m_pTerrainSystem->lodRatioLimit();

	while (!finished)
	{
		// find the ratio of variance over distance
		float variance = m_errorMetric[m_lod];
		float vRatio = (variance*vScale)/(dist);

		// if we exceed the ratio limit, move to the next lod
		if (vRatio > vLimit
			&& m_lod+1 < k_totalDetailLevels)
		{
			++m_lod;
		}
		else
		{
			finished=true;
		}
	}
}



//****************************************************************************************
// end of file      ( Math.cpp )

