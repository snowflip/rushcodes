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

#include "roam_system.h"
#include "game_host.h"

using namespace gaia;

bool cRoamTerrain::create(cSceneNode* pRootNode, cTexture* heightMap, const cRect3d& worldExtents, uint8 shift)
{
	// roam terrain sections are forced to be 16x16 cells
	shift = 4;

	bool result = cTerrain::create(pRootNode, heightMap, worldExtents, shift);

	if (result)
	{
		m_pTriangleNodePool = new cTriTreeNode[k_maxTriTreeNodes];
		m_nextTriNode = 0;


		m_tessellationQueue = new cRoamTerrainSection*[k_tessellationQueueSize];
		m_tessellationQueueCount = 0;

		memset(m_tessellationQueue, 0, sizeof(m_tessellationQueue));
		memset(m_pTriangleNodePool, 0, sizeof(cTriTreeNode)*k_maxTriTreeNodes);
	}

	return result;
}

void cRoamTerrain::destroy()
{
	safe_delete_array(m_pRoamSectionArray);
	safe_delete_array(m_pTriangleNodePool);
	safe_delete_array(m_tessellationQueue);

	m_nextTriNode = 0;
	m_tessellationQueueCount = 0;

	cTerrain::destroy();
}

bool cRoamTerrain::allocateSectors()
{
	m_pRoamSectionArray = new cRoamTerrainSection[m_sectorCountX*m_sectorCountY];

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
			
			if (!m_pRoamSectionArray[index].create(
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

//
// reset is called at the start of each frame
// to return all counters to zero, preparing
// both the triangle node pool and the
// tessellation queue for new entries
//
void cRoamTerrain::reset()
{
	// reset internal counters
	m_tessellationQueueCount = 0;
	m_nextTriNode=0;

	// reset each section
	int total = m_sectorCountY*m_sectorCountX;
	for (int i=0; i<total; ++i)
	{
		m_pRoamSectionArray[i].reset();
	}

}

//
// As sections are pulled from the quad tree, 
// they add themselves to the tessellation queue
//
bool cRoamTerrain::addToTessellationQueue(
	cRoamTerrainSection* pSection)
{
	if (m_tessellationQueueCount 
		< k_tessellationQueueSize)
	{
		m_tessellationQueue[m_tessellationQueueCount] = 
			pSection;
		++m_tessellationQueueCount;
		return true;
	}

	// while we handle this failure gracefully 
	// in release builds, we alert ourselves 
	// to the situation with an assert in debug
	// builds so we can increase the queue size
	debug_assert(
		0, 
		"increase the size of the ROAM tessellation queue");
	return false;
}

// local sorting functor used 
// by the quick sort algorythm
typedef cRoamTerrainSection* LPRoamSection;
struct sort_less
{
	bool operator()(
		const cRoamTerrainSection*& a, 
		const cRoamTerrainSection*& b)const
	{
		return a->queueSortValue() 
			< b->queueSortValue();
	}
};

//
// This function is called to sort the queue and
// allow each section to tessellate in order
//
void cRoamTerrain::processTessellationQueue()
{
	// sort the tessellation list
	// see "core\quick_sort.h"
	// for details
	QuickSort(m_tessellationQueue, 
		m_tessellationQueueCount, 
		sort_less());

	// tessellate each section
	uint32 i;
	for (i=0; i<m_tessellationQueueCount; ++i)
	{
		// split triangles based on the
		// scale and limit values
		m_tessellationQueue[i]->tessellate(
			m_vScale, m_vLimit);
	}

	// gather up all the triangles into
	// a final index buffer per section
	for (i=0; i<m_tessellationQueueCount; ++i)
	{
		m_tessellationQueue[i]->buildTriangleList();
	}
}

// this function handles client requests for 
// cTriTreeNode objects from our local pool.
// we use m_nextTriNode to store the next 
// available index in the pool. When this
// index reaches the end of the pool, all
// nodes are in use.
cTriTreeNode* cRoamTerrain::requestTriNode()
{
	cTriTreeNode* pNode = 0;

	if (m_nextTriNode < k_maxTriTreeNodes)
	{
		// pull a node from the pool
		// and erase any old data in it
		pNode = &m_pTriangleNodePool[m_nextTriNode];
		memset(pNode, 0 ,sizeof(cTriTreeNode));
		++m_nextTriNode;
	}

	// this may be zero. Callers must
	// handle this possibility
	return pNode;
}

cRoamTerrainSection* cRoamTerrain::findSection(int sectionX, int sectionY)
{
	cRoamTerrainSection* pSection = 0;

	if (sectionX >=0 && sectionX<m_sectorCountX
		&& sectionY >=0 && sectionY<m_sectorCountY)
	{
		pSection = 
			&m_pRoamSectionArray[(sectionY*m_sectorCountX)+sectionX];
	}
	else
	{
		// if we had additional cRoamTerrain objects,
		// we could reach out here to link with neighbors
	}

	return pSection;
}

void cRoamTerrain::submitSection(cTerrainSection* pSection)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cTerrainSystem_submitSection);
		int total_passes = pEffectFile->totalPasses();
		cRoamTerrainSection* pRoamSection=(cRoamTerrainSection*)pSection;

		// check the neighbor sectors for connection needs
		uint16 sX = pSection->sectorX();
		uint16 sY = pSection->sectorY();

		int index = (sY*m_sectorCountX)+sX;

		for (int iPass=0; iPass<total_passes; ++iPass)
		{
			cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
			
			pRenderEntry->hEffectFile = (uint8)pEffectFile->resourceHandle();
			pRenderEntry->hSurfaceMaterial = pSurfaceMaterial->resourceHandle();
			pRenderEntry->modelType = cRenderEntry::k_bufferEntry;
			pRenderEntry->hModel = m_pVertexGrid->resourceHandle();
			pRenderEntry->modelParamA = pRoamSection->sectorVertices()->resourceHandle();
			pRenderEntry->modelParamB = pRoamSection->indexBuffer()->resourceHandle();
			pRenderEntry->renderPass = iPass;
			pRenderEntry->object = (cSceneNode*)pSection;
			pRenderEntry->userData = 0;

			DisplayManager.closeRenderQueue(pRenderEntry);
		}
	}
}

void cRoamTerrain::renderSection(
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
		cRoamTerrainSection* pRoamSection=(cRoamTerrainSection*)pSection;

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
			pRoamSection->indexBuffer()->activate();
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
		uint16 totalPolys = pRoamSection->totalIndices()/3;

		if (totalPolys)
		{
			HRESULT hr = d3dDevice->DrawIndexedPrimitive(
				pRoamSection->indexBuffer()->primitiveType(),
				0,
				0,
				m_sectorVerts*m_sectorVerts,
				0,
				totalPolys);
		}
	}
}



//***************************************************************
// end of file      ( cRoamSystem.cpp )

//----------------------------------------------------------
//$Log: $