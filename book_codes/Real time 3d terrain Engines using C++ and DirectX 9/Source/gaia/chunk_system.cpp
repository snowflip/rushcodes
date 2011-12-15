#define CROAMSYSTEM_CPP
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

#include "chunk_system.h"
#include "game_host.h"

using namespace gaia;

bool cChunkTerrain::create(
	cSceneNode* pRootNode, 
	cTexture* heightMap, 
	const cRect3d& worldExtents, 
	uint8 shift)
{
	// chunk terrain sections 
	// are forced to be 16x16 cells
	shift = 5;

	// construct the base class
	bool result = 
		cTerrain::create(
			pRootNode, 
			heightMap, 
			worldExtents, 
			shift);

	if (!result) destroy();

	return result;
}

void cChunkTerrain::destroy()
{
	safe_delete_array(m_pChunkSectionArray);

	for (int i=0; i<k_maxDetailLevels; ++i)
	{
		safe_release(m_indexBufferList[k_chunk][i]);
		safe_release(m_indexBufferList[k_skirt][i]);
	}

	cTerrain::destroy();
}

bool cChunkTerrain::allocateSectors()
{
	m_pChunkSectionArray = new cChunkTerrainSection[m_sectorCountX*m_sectorCountY];

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
			
			if (!m_pChunkSectionArray[index].create(
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

cChunkTerrainSection* cChunkTerrain::findSection(int sectionX, int sectionY)
{
	cChunkTerrainSection* pSection = 0;

	if (sectionX >=0 && sectionX<m_sectorCountX
		&& sectionY >=0 && sectionY<m_sectorCountY)
	{
		pSection = 
			&m_pChunkSectionArray[(sectionY*m_sectorCountX)+sectionX];
	}
	else
	{
		// if we had additional cChunkTerrain objects,
		// we could reach out here to link with neighbors
	}

	return pSection;
}

void cChunkTerrain::submitSection(cTerrainSection* pSection)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cChunkTerrain_submitSection);
		int total_passes = pEffectFile->totalPasses();
		cChunkTerrainSection* pChunkSection=(cChunkTerrainSection*)pSection;

		// how many render entries does the sector have?
		uint32 totalEntries = pChunkSection->totalRenderEntries();
		for (uint32 iEntry=0; iEntry<totalEntries; ++iEntry)
		{
			int iPass;
			cChunkTerrainSection::sRenderEntry renderEntry= 
				pChunkSection->renderEntry(iEntry);

			// submit the chunks
			renderEntry.skirtOrTile = k_chunk;
			for (iPass=0; iPass<total_passes; ++iPass)
			{
				cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
				
				pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
				pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
				pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
				pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
				pRenderEntry->modelParamA = pChunkSection->sectorVertices()->resourceHandle();
				pRenderEntry->modelParamB = 
					m_indexBufferList[k_chunk][renderEntry.level]->resourceHandle();
				pRenderEntry->renderPass = iPass;
				pRenderEntry->object = (cSceneNode*)pSection;
				pRenderEntry->userData = renderEntry.value;

				DisplayManager.closeRenderQueue(pRenderEntry);
			}

			// submit the skirts
			renderEntry.skirtOrTile = k_skirt;
			for (iPass=0; iPass<total_passes; ++iPass)
			{
				cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
				
				pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
				pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
				pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
				pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
				pRenderEntry->modelParamA = pChunkSection->sectorVertices()->resourceHandle();
				pRenderEntry->modelParamB = 
					m_indexBufferList[k_skirt][renderEntry.level]->resourceHandle();
				pRenderEntry->renderPass = iPass;
				pRenderEntry->object = (cSceneNode*)pSection;
				pRenderEntry->userData = renderEntry.value;

				DisplayManager.closeRenderQueue(pRenderEntry);
			}

		}
	}
}

void cChunkTerrain::renderSection(
	cTerrainSection* pSection, 
	u32Flags activationFlags,
	const cRenderEntry* pEntry)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		cChunkTerrainSection* pChunkSection=(cChunkTerrainSection*)pSection;
		cChunkTerrainSection::sRenderEntry renderEntry;
		renderEntry.value = pEntry->userData;

		cIndexBuffer* pIndexBuffer = 
			m_indexBufferList[renderEntry.skirtOrTile][renderEntry.level];

		profile_scope(cTerrainSystem_renderSection);
		LPDIRECT3DDEVICE9 d3dDevice = TheGameHost.d3dDevice();
		
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

		uint16 vertexStride = (1<<m_sectorShift)+1;
		uint16 baseVertex = (renderEntry.offsetY*vertexStride)+renderEntry.offsetX;

		HRESULT hr = d3dDevice->DrawIndexedPrimitive(
			pIndexBuffer->primitiveType(),
			baseVertex,
			0,
			m_sectorVerts*m_sectorVerts,
			0,
			pIndexBuffer->primitiveCount());
	}
}


bool cChunkTerrain::buildVertexBuffer()
{
	cString tempName;
	tempName.format("terrain_system_%i", this);

	// create the vertex buffer
	// shared by the sectors
	m_pVertexGrid = 
		DisplayManager.vertexBufferPool().
		createResource(tempName);

	cVector2 cellSize(
		m_sectorSize.x/m_sectorUnits,
		m_sectorSize.y/m_sectorUnits);

	int pageSize = m_sectorVerts*m_sectorVerts;
	int bufferSize = pageSize<<1;

	cVector2 vert(0.0f,0.0f);
	sLocalVertex* pVerts = 
		new sLocalVertex[bufferSize];

	// fill the vertex stream with x,y positions and
	// uv coordinates. All other data (height and
	// surface normals) are stored in the vertex
	// buffers of each terrain section
	for (int y=0; y<m_sectorVerts; ++y)
	{
		vert.set(0.0f, y*cellSize.y);

		for (int x=0; x<m_sectorVerts; ++x)
		{
			int index = (y*m_sectorVerts)+x;
			cVector2 UV(
				(float)x/(float)(m_sectorVerts-1),
				(float)y/(float)(m_sectorVerts-1));

			pVerts[index].xyPosition = vert;
			pVerts[index].localUV = UV;

			// duplicate this data into the
			// second page as well
			pVerts[index+pageSize].xyPosition = vert;
			pVerts[index+pageSize].localUV = UV;

			vert.x += cellSize.x;
		}
	}

	// now that we have built the data,
	// create one of our vertex buffer
	// resource objects with it
	bool result = 
		m_pVertexGrid->create(
		bufferSize, 
		sizeof(sLocalVertex), 
		0, pVerts);

	safe_delete_array(pVerts);

	return result;
}

bool cChunkTerrain::buildIndexBuffer()
{
	bool result = true;
	// the default index buffer is not used
	// in chunked terrain, Instead, we build
	// an entire set of index buffers for
	// each detail level and possible
	// skirt edge

	int stride = (1<<m_sectorShift)+1;
	int stepSize = stride>>k_minTessellationShift;
	int vertCount = (1<<k_minTessellationShift)+1;

	m_detailLevels = 0;

	while (stepSize 
		&& result
		&& m_detailLevels<k_maxDetailLevels)
	{
		cString tempName;
		tempName.format(
			"chunk_index_buffer_%i", 
			m_detailLevels);

		m_indexBufferList[k_chunk][m_detailLevels] = 
			DisplayManager.indexBufferPool().
			createResource(tempName);

		result = result &&
		m_indexBufferList
			[k_chunk][m_detailLevels]->createSingleStripGrid(
			vertCount,	// width of grid
			vertCount,	// height of grid
			stepSize,	// horz vertex count per cell
			stepSize,	// vert vertex count per cell
			stride,	// horz vertex count in vbuffer
			0);

		stepSize>>=1;
		++m_detailLevels;
	}

	// build an index buffer for each skirt.
	// Consider each square detail level of
	// the tile as a square with corners
	// A,B,C & D as in the figure below

/*
		D -------- C
	^	|          |
	|	|          |
	p	|          |
	o	|          |
	s	|          |
		A -------- B
	y
		pos x ->
*/
	// the following code generates a single strip 
	// for each skirt following the edges
	// AB, BC, CD and DA
	int sideLength = (1<<m_sectorShift)+1;
	int pageSize = sideLength*sideLength;

	for (uint32 iLevel=0; result && iLevel<m_detailLevels; ++iLevel)
	{
		cString tempName;
		tempName.format(
			"chunk_skirt_index_buffer_%i", 
			iLevel);

		m_indexBufferList[k_skirt][iLevel] = 
			DisplayManager.indexBufferPool().
			createResource(tempName);
	
		int skirtSide = (1<<k_minTessellationShift)+1;
		int indexCount=skirtSide<<3;
		uint16* indexList = new uint16[indexCount];
		uint16* pIndex = indexList;

		int vStep;
		int vIndex;
		int count;
		int horzStep = 
			(sideLength>>iLevel)>>k_minTessellationShift;
		int vertStep = 
			sideLength*horzStep;

		// side AB
		vIndex=0;
		vStep = vertStep;
		for(count=0; count<skirtSide;++count)
		{
			*(pIndex++)=vIndex;
			*(pIndex++)=vIndex+pageSize;
			vIndex +=vStep;
		}

		// side BC
		vIndex -= vStep;
		vStep = horzStep;
		for(count=0; count<skirtSide;++count)
		{
			*(pIndex++)=vIndex;
			*(pIndex++)=vIndex+pageSize;
			vIndex +=vStep;
		}

		// side CD
		vIndex -= vStep;
		vStep = -vertStep;
		for(count=0; count<skirtSide;++count)
		{
			*(pIndex++)=vIndex;
			*(pIndex++)=vIndex+pageSize;
			vIndex +=vStep;
		}

		// side DA
		vIndex -= vStep;
		vStep = -horzStep;
		for(count=0; count<skirtSide;++count)
		{
			*(pIndex++)=vIndex;
			*(pIndex++)=vIndex+pageSize;
			vIndex +=vStep;
		}

		result = result &&
		m_indexBufferList[k_skirt][iLevel]->create(
			D3DPT_TRIANGLESTRIP , 
			indexCount, 
			0, 
			indexList);

		safe_delete_array(indexList);
	}

	return result;
}

//***************************************************************
// end of file      ( cChunkSystem.cpp )

//----------------------------------------------------------
//$Log: $