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

#include "terrain_section.h"
#include "scene_node.h"
#include "model_resource.h"
#include "game_host.h"
#include "terrain_system.h"
#include "camera.h"

using namespace gaia;

bool cTerrainSection::create(cSceneNode* pRootNode,
							 cTerrain* pParentSystem, 
							 uint16 sectorX, uint16 sectorY, 
							 uint16 heightMapX, uint16 heightMapY,
							 uint16 xVerts, uint16 yVerts, 
							 const cRect2d& worldRect)
{
	bool result = cSceneObject::create();

	attachToParent(pRootNode);

	if (result)
	{
		//
		// Build a vertex buffer to 
		// hold the height and surface
		// normal for this area of the terrain
		//
		m_pTerrainSystem = pParentSystem;
		m_xVerts = xVerts;
		m_yVerts = yVerts;
		m_sectorX = sectorX;
		m_sectorY = sectorY;
		m_worldRect.rect2d = worldRect;
		m_heightMapX = heightMapX;
		m_heightMapY = heightMapY;

		result = buildVertexBuffer();

		// set the scene object bounds data
		*accessLocalBounds() = m_worldRect;

	}

	return result;
}

//
// Derived classes can override this function
// to build custom geometry
//
bool cTerrainSection::buildVertexBuffer()
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

	if (m_pSectorVerts)
	{
		// read in the height and normal for each vertex
		cTerrain::sSectorVertex* pVerts = 
			new cTerrain::sSectorVertex[m_xVerts*m_yVerts];

		for (uint16 y = 0; y<m_yVerts; ++y)
		{
			for (uint16 x = 0; x<m_xVerts; ++x)
			{
				float height = 
					m_pTerrainSystem->readWorldHeight(
						m_heightMapX+x, 
						m_heightMapY+y);
				pVerts[(y*m_xVerts)+x].height = 
					height;
				pVerts[(y*m_xVerts)+x].normal = 
					m_pTerrainSystem->readWorldNormal(
						m_heightMapX+x, 
						m_heightMapY+y);

				m_worldRect.z0 = 
					minimum(m_worldRect.z0, height);
				m_worldRect.z1 = 
					maximum(m_worldRect.z1, height);
			}
		}

		result = SUCCEEDED(m_pSectorVerts->create(
			m_xVerts*m_yVerts, 
			sizeof(cTerrain::sSectorVertex), 
			FLAG(cVertexBuffer::nRamBackupBit),
			pVerts));

		safe_delete_array(pVerts);
	}
	else
	{
		result = false;
	}

	return result;
}

void cTerrainSection::destroy()
{
	cSceneObject::destroy();

	safe_release(m_pSectorVerts);
}

void cTerrainSection::render()
{
	// ask our parent to complete the submission
	m_pTerrainSystem->submitSection(this);
}

void cTerrainSection::renderCallback(
	cRenderEntry* entry, 
	u32Flags activationFlags)
{
	// ask our parent to complete the render
	m_pTerrainSystem->renderSection(
		this, activationFlags, entry);
}

//****************************************************************************************
// end of file      ( Math.cpp )

