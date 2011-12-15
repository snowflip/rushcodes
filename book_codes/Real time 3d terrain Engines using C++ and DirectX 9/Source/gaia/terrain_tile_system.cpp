#define CTERRAINTILESYSTEM_CPP
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

#include "terrain_tile_system.h"
#include "terrain_tile_shapes.h"
#include "terrain_tile_section.h"
#include "game_host.h"
#include "camera.h"

using namespace gaia;

int cTiledTerrain::s_indexBufferRefCount=0;
cTiledTerrain::sDetailLevel cTiledTerrain::s_detailLevel[cTiledTerrain::k_totalDetailLevels];

bool cTiledTerrain::create(cSceneNode* pRootNode, 
								cTexture* heightMap, 
								const cRect3d& worldExtents, 
								uint8 shift)
{
	// terrain tiles are forced to be 8x8 cells
	shift = 3;

	bool result = cTerrain::create(pRootNode, heightMap, worldExtents, shift);

	if (result)
	{
		if (!s_indexBufferRefCount)
		{
			memset(&s_detailLevel, 0, sizeof(s_detailLevel));
			result = generateDetailLevels();

			if (!result)
			{
				destroy();
				return false;
			}
		}
		++s_indexBufferRefCount;
	}

	return result;
}

void cTiledTerrain::destroy()
{
	--s_indexBufferRefCount;

	// last one out turn off the lights...
	if (!s_indexBufferRefCount)
	{
		for (int i=0; i<k_totalDetailLevels; ++i)
		{
			for (int s=0; s<k_totalShapes; ++s)
			{
				safe_release(s_detailLevel[i].tileBodies[s]);
			}
			for (s=0; s<k_totalSides; ++s)
			{
				for (int l=0; l<k_totalDetailLevels; ++l)
				{
					safe_release(s_detailLevel[i].tileConnectors[s][l]);
				}
			}
		}
	}

	safe_delete_array(m_pTileSectorArray);

	cTerrain::destroy();
}

bool cTiledTerrain::allocateSectors()
{
	m_pTileSectorArray = new cTiledTerrainSection[m_sectorCountX*m_sectorCountY];

	// create the sector objects themselves
	for (int y=0; y<m_sectorCountY; ++y)
	{
		for (int x=0; x<m_sectorCountX; ++x)
		{
			cVector2 sectorPos(
				m_worldExtents.x0+(x*m_sectorSize.x),
				m_worldExtents.y0+(y*m_sectorSize.y));

			cRect2d sectorRect(
				sectorPos.x, sectorPos.x+m_sectorSize.x,
				sectorPos.y, sectorPos.y+m_sectorSize.y);

			uint16 xPixel = x<<m_sectorShift;
			uint16 yPixel = y<<m_sectorShift;
			uint16 index = (y*m_sectorCountX)+x;
			
			if (!m_pTileSectorArray[index].create(
				m_pRootNode, 
				this, 
				x, y, 
				xPixel, yPixel, 
				m_sectorVerts, 
				m_sectorVerts, 
				sectorRect))
			{
				return false;
			}
		}
	}

	return true;
}

bool cTiledTerrain::createIndexBuffer(cIndexBuffer** ppIndexBuffer, const detail_index_data* source)
{
	bool result =false;
	debug_assert(!*ppIndexBuffer, "cTiledTerrain index buffer already created");

	// create an index buffer (our resource system requires a unique name)
	cString tempName;
	tempName.format("terrain_tile_system_%i", ppIndexBuffer);
	*ppIndexBuffer = TheGameHost.displayManager().indexBufferPool().createResource(tempName);

	debug_assert(*ppIndexBuffer, "cTiledTerrain index buffer allocation failed");

	if (*ppIndexBuffer)
	{
		cIndexBuffer& indexBuffer = **ppIndexBuffer;

		D3DPRIMITIVETYPE primitiveType = source->flag == STRIP_LIST ? D3DPT_TRIANGLESTRIP : D3DPT_TRIANGLELIST;

		if (indexBuffer.create(primitiveType, source->vertex_count, 0, source->index_table))
		{
			result = true;
		}
		else
		{
			debug_assert(0, "cTiledTerrain index buffer creation failed");
			safe_release(*ppIndexBuffer);
		}
	}

	return result;
}


bool cTiledTerrain::generateDetailLevels()
{
	//
	// Generate Tile Bodies and Connectors for our 4 sample detail levels
	//

	//
	// Detail Level 0 (lowest detail level)
	//
	if (!createIndexBuffer(&s_detailLevel[0].tileBodies[0], &base_level_0))
	{
		return false;
	}

	//
	// Detail Level 1-3, main body tiles
	//
	for (int body=0;body<16;++body)
	{
		if (!createIndexBuffer(&s_detailLevel[1].tileBodies[body], &base_level_1[body]))
		{
			return false;
		}
		if (!createIndexBuffer(&s_detailLevel[2].tileBodies[body], &base_level_2[body]))
		{
			return false;
		}
		if (!createIndexBuffer(&s_detailLevel[3].tileBodies[body], &base_level_3[body]))
		{
			return false;
		}
	}

	// create the tile connectors
	for (int side=0;side<k_totalSides;++side)
	{
		if (!createIndexBuffer(&s_detailLevel[1].tileConnectors[side][0], &connect_level_1_to_0[side]))
		{
			return false;
		}

		if (!createIndexBuffer(&s_detailLevel[2].tileConnectors[side][0], &connect_level_2_to_0[side]))
		{
			return false;
		}
		if (!createIndexBuffer(&s_detailLevel[2].tileConnectors[side][1], &connect_level_2_to_1[side]))
		{
			return false;
		}

		if (!createIndexBuffer(&s_detailLevel[3].tileConnectors[side][0], &connect_level_3_to_0[side]))
		{
			return false;
		}
		if (!createIndexBuffer(&s_detailLevel[3].tileConnectors[side][1], &connect_level_3_to_1[side]))
		{
			return false;
		}
		if (!createIndexBuffer(&s_detailLevel[3].tileConnectors[side][2], &connect_level_3_to_2[side]))
		{
			return false;
		}
	}

	return true;

}

void cTiledTerrain::renderSection(
	cTerrainSection* pBaseSection, 
	u32Flags activationFlags,
	const cRenderEntry* pEntry)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cTerrainTileSystem_renderSection);
		cTiledTerrainSection* pSection = (cTiledTerrainSection*)pBaseSection;
		LPDIRECT3DDEVICE9 d3dDevice = TheGameHost.d3dDevice();
		
		// find out which index buffer we are using
		sUserData bitFields;
		bitFields.value = pEntry->userData;
		cIndexBuffer* pIndexBuffer = 0;

		if (bitFields.bodyTileFlag)
		{
			pIndexBuffer = s_detailLevel[bitFields.detailLevel].tileBodies[bitFields.shapeNumber];
		}
		else
		{
			pIndexBuffer = s_detailLevel[bitFields.detailLevel].tileConnectors[bitFields.side][bitFields.connectionLevel];
		}

		// do we need to activate the render method?
		if (TEST_BIT(activationFlags,k_activateRenderMethod))
		{
			pEffectFile->begin();
		}

		// do we need to activate the render pass?
		if (TEST_BIT(activationFlags,k_activateRenderMethodPass))
		{
			pEffectFile->activatePass(pEntry->renderPass);
		}

		// do we need to activate the primary vertex buffer
		if (TEST_BIT(activationFlags,k_activateModel))
		{
			m_pVertexGrid->activate(0,0, true);
		}

		// do we need to activate the secondary vertex buffer
		if (TEST_BIT(activationFlags,k_activateModelParamA))
		{
			pSection->sectorVertices()->activate(1,0, false);
		}

		// do we need to activate the index buffer
		if (TEST_BIT(activationFlags,k_activateModelParamB))
		{
			pIndexBuffer->activate();
		}

		// do we need to activate the surface material
		if (TEST_BIT(activationFlags,k_activateSurfaceMaterial))
		{
			pEffectFile->applySurfaceMaterial(pSurfaceMaterial);
		}

		// apply our render settings to the method
		int sectorX = pSection->sectorX();
		int sectorY = pSection->sectorY();

		cVector4 sectorOffset(
			1.0f,
			1.0f,
			m_worldExtents.x0 +(m_sectorSize.x * sectorX),
			m_worldExtents.y0 +(m_sectorSize.y * sectorY));


		cVector4 uvScaleOffset(
			(float)1.0f/(m_sectorCountX+1),
			(float)1.0f/(m_sectorCountY+1),
			(float)sectorX,
			(float)sectorY);

		pEffectFile->setParameter(cEffectFile::k_posScaleOffset, (D3DXVECTOR4*)&sectorOffset);
		pEffectFile->setParameter(cEffectFile::k_uvScaleOffset, (D3DXVECTOR4*)&uvScaleOffset);

		// render!!!
		HRESULT hr = d3dDevice->DrawIndexedPrimitive(
			pIndexBuffer->primitiveType(),
			0,
			0,
			m_sectorVerts*m_sectorVerts,
			0,
			pIndexBuffer->primitiveCount());
	}
}

void cTiledTerrain::submitSection(cTerrainSection* pBaseSection)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cTerrainTileSystem_submitSection);
		cTiledTerrainSection* pSection = (cTiledTerrainSection*)pBaseSection;
		int total_passes = pEffectFile->totalPasses();
		sUserData userData;

		// check the neighbor sectors for connection needs
		uint16 sX = pSection->sectorX();
		uint16 sY = pSection->sectorY();

		int index = (sY*m_sectorCountX)+sX;
		int localLOD = pSection->getLod();
		uint16 bodyTile = 0;

		cIndexBuffer* pIndexBuffer;

		if (localLOD)
		{
			if (sX)
			{
				int sideLOD = m_pTileSectorArray[index-1].getLod();
				if (sideLOD < localLOD)
				{
					bodyTile += 1<<k_left;

					pIndexBuffer = s_detailLevel[localLOD].tileConnectors[k_left][sideLOD];

					for (int iPass=0; iPass<total_passes; ++iPass)
					{
						cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
						
						pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
						pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
						pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
						pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
						pRenderEntry->modelParamA = pSection->sectorVertices()->resourceHandle();
						pRenderEntry->modelParamB = pIndexBuffer->resourceHandle();
						pRenderEntry->renderPass = iPass;
						pRenderEntry->object = (cSceneNode*)pSection;

						userData.bodyTileFlag = 0;
						userData.detailLevel = localLOD;
						userData.side = k_left;
						userData.connectionLevel = sideLOD;

						pRenderEntry->userData = userData.value;

						DisplayManager.closeRenderQueue(pRenderEntry);
					}
				}
			}

			if (sY)
			{
				int sideLOD = m_pTileSectorArray[index-m_sectorCountX].getLod();
				if (sideLOD < localLOD)
				{
					bodyTile += 1<<k_top;

					pIndexBuffer = s_detailLevel[localLOD].tileConnectors[k_top][sideLOD];

					for (int iPass=0; iPass<total_passes; ++iPass)
					{
						cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
						
						pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
						pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
						pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
						pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
						pRenderEntry->modelParamA = pSection->sectorVertices()->resourceHandle();
						pRenderEntry->modelParamB = pIndexBuffer->resourceHandle();
						pRenderEntry->renderPass = iPass;
						pRenderEntry->object = (cSceneNode*)pSection;

						userData.bodyTileFlag = 0;
						userData.detailLevel = localLOD;
						userData.side = k_top;
						userData.connectionLevel = sideLOD;

						pRenderEntry->userData = userData.value;

						DisplayManager.closeRenderQueue(pRenderEntry);
					}
				}
			}

			if (sX<(m_sectorCountX-1))
			{
				int sideLOD = m_pTileSectorArray[index+1].getLod();
				if (sideLOD < localLOD)
				{
					bodyTile += 1<<k_right;

					pIndexBuffer = s_detailLevel[localLOD].tileConnectors[k_right][sideLOD];

					for (int iPass=0; iPass<total_passes; ++iPass)
					{
						cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
						
						pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
						pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
						pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
						pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
						pRenderEntry->modelParamA = pSection->sectorVertices()->resourceHandle();
						pRenderEntry->modelParamB = pIndexBuffer->resourceHandle();
						pRenderEntry->renderPass = iPass;
						pRenderEntry->object = (cSceneNode*)pSection;

						userData.bodyTileFlag = 0;
						userData.detailLevel = localLOD;
						userData.side = k_right;
						userData.connectionLevel = sideLOD;

						pRenderEntry->userData = userData.value;

						DisplayManager.closeRenderQueue(pRenderEntry);
					}
				}
			}

			if (sY<(m_sectorCountY-1))
			{
				int sideLOD = m_pTileSectorArray[index+m_sectorCountX].getLod();
				if (sideLOD < localLOD)
				{
					bodyTile += 1<<k_bottom;

					pIndexBuffer = s_detailLevel[localLOD].tileConnectors[k_bottom][sideLOD];

					for (int iPass=0; iPass<total_passes; ++iPass)
					{
						cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
						
						pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
						pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
						pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
						pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
						pRenderEntry->modelParamA = pSection->sectorVertices()->resourceHandle();
						pRenderEntry->modelParamB = pIndexBuffer->resourceHandle();
						pRenderEntry->renderPass = iPass;
						pRenderEntry->object = (cSceneNode*)pSection;

						userData.bodyTileFlag = 0;
						userData.detailLevel = localLOD;
						userData.side = k_bottom;
						userData.connectionLevel = sideLOD;

						pRenderEntry->userData = userData.value;

						DisplayManager.closeRenderQueue(pRenderEntry);
					}
				}
			}
		}
		else
		{
			localLOD=0;
		}

		
		if (localLOD != 1 || bodyTile != 15) // Lod 1, tile 15 does not need to be drawn
		{
			pIndexBuffer = s_detailLevel[localLOD].tileBodies[bodyTile];

			for (int iPass=0; iPass<total_passes; ++iPass)
			{
				cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
				
				pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
				pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
				pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
				pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
				pRenderEntry->modelParamA = pSection->sectorVertices()->resourceHandle();
				pRenderEntry->modelParamB = pIndexBuffer->resourceHandle();
				pRenderEntry->renderPass = iPass;
				pRenderEntry->object = (cSceneNode*)pSection;

				userData.bodyTileFlag = 1;
				userData.detailLevel = localLOD;
				userData.shapeNumber = bodyTile;

				pRenderEntry->userData = userData.value;

				DisplayManager.closeRenderQueue(pRenderEntry);
			}
		}
	}
}



//***************************************************************
// end of file      ( cTiledTerrain.cpp )

//----------------------------------------------------------
//$Log: $