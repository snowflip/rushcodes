#define CROAMSECTION_CPP
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

#include "roam_section.h"
#include "roam_system.h"
#include "camera.h"
#include "game_host.h"

using namespace gaia;

// Mutators...
bool cRoamTerrainSection::create(
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

	cString name;
	name.format("ROAM Index Buffer %i %i",sectorX,sectorY);
	m_pIndexBuffer = DisplayManager.indexBufferPool().createResource(name);

	m_maxIndices = xVerts*yVerts*2*3;

	m_pIndexBuffer->create(
		D3DPT_TRIANGLELIST, 
		m_maxIndices, 
		FLAG(cIndexBuffer::nDynamicOverwriteBit), 
		0);

	// find base neighbors
	cRoamTerrain* pRoamTerrain = roamTerrain();

	m_leftNeighborOfA = 0;
	m_rightNeighborOfA = 0;
	m_leftNeighborOfB = 0;
	m_rightNeighborOfB = 0;

	cRoamTerrainSection* pNorthSection = 
		pRoamTerrain->findSection(sectorX, sectorY-1);
	cRoamTerrainSection* pSouthSection = 
		pRoamTerrain->findSection(sectorX, sectorY+1);
	cRoamTerrainSection* pEastSection = 
		pRoamTerrain->findSection(sectorX+1, sectorY);
	cRoamTerrainSection* pWestSection = 
		pRoamTerrain->findSection(sectorX-1, sectorY);

	if (pNorthSection)
	{
		m_leftNeighborOfA = 
			pNorthSection->triangleB();
	}
	if (pSouthSection)
	{
		m_leftNeighborOfB = 
			pSouthSection->triangleA();
	}
	if (pEastSection)
	{
		m_rightNeighborOfB = 
			pEastSection->triangleA();
	}
	if (pWestSection)
	{
		m_rightNeighborOfA = 
			pWestSection->triangleB();
	}

	// establish basic links
	reset();

	// build the variance trees
	computeVariance();

	return result;
}

void cRoamTerrainSection::destroy()
{
	// we assume the whole terrain is
	// being destroyed, so we do not
	// need to do any cleanup. If we were in 
	// a dynamic terrain, we would unlink from
	// our neighbors before destruction
	safe_release(m_pIndexBuffer);
	cTerrainSection::destroy();
}

void cRoamTerrainSection::reset()
{
	// unlink the base traingles from any children
	m_rootTriangleA.leftChild = 0;
	m_rootTriangleA.rightChild = 0;
	m_rootTriangleB.leftChild = 0;
	m_rootTriangleB.rightChild = 0;

	// link our base triangles together
	m_rootTriangleA.baseNeighbor = &m_rootTriangleB;
	m_rootTriangleB.baseNeighbor = &m_rootTriangleA;

	// link to our neighbors
	m_rootTriangleA.leftNeighbor = m_leftNeighborOfA;
	m_rootTriangleA.rightNeighbor = m_rightNeighborOfA;
	m_rootTriangleB.leftNeighbor = m_leftNeighborOfB;
	m_rootTriangleB.rightNeighbor = m_rightNeighborOfB;

}

void cRoamTerrainSection::computeVariance()
{
	uint16 tableWidth = roamTerrain()->tableWidth();
	uint16 tableHeight = roamTerrain()->tableHeight();

	uint16 index0 = 
		roamTerrain()->tableIndex(
		m_heightMapX, 
		m_heightMapY);

	uint16 index1 = 
		roamTerrain()->tableIndex(
		m_heightMapX, 
		m_heightMapY+m_yVerts-1);

	uint16 index2 = 
		roamTerrain()->tableIndex(
		m_heightMapX+m_xVerts-1, 
		m_heightMapY+m_yVerts-1);

	uint16 index3 = 
		roamTerrain()->tableIndex(
		m_heightMapX+m_xVerts-1, 
		m_heightMapY);


	float height0 = roamTerrain()->readWorldHeight(index0);
	float height1 = roamTerrain()->readWorldHeight(index1);
	float height2 = roamTerrain()->readWorldHeight(index2);
	float height3 = roamTerrain()->readWorldHeight(index3);

	recursiveComputeVariance(
		index1, index2, index0,
		height1, height2, height0,
		m_varianceTreeA, 1);

	recursiveComputeVariance(
		index3, index0, index2,
		height3, height0, height2,
		m_varianceTreeB, 1);
}

void cRoamTerrainSection::prepareForRender()
{
	cCamera* pCamera = TheGameHost.activeCamera();

	// compute a 2d point for each corner of the section
	cVector2 corner0(m_worldRect.x0, m_worldRect.y0);
	cVector2 corner1(m_worldRect.x0, m_worldRect.y1);
	cVector2 corner2(m_worldRect.x1, m_worldRect.y1);
	cVector2 corner3(m_worldRect.x1, m_worldRect.y0);

	cVector2 viewPoint= pCamera->worldPosition().vec2();

	// compute view distance to our 4 corners
	m_distance0 = viewPoint.distance(corner0);
	m_distance1 = viewPoint.distance(corner1);
	m_distance2 = viewPoint.distance(corner2);
	m_distance3 = viewPoint.distance(corner3);

	// compute min distance as our sort value
	m_queueSortValue = minimum(m_distance0, m_distance1);
	m_queueSortValue = minimum(m_queueSortValue, m_distance2);
	m_queueSortValue = minimum(m_queueSortValue, m_distance3);

	// submit to the tessellation queue of our parent
	roamTerrain()->addToTessellationQueue(this);
}

void cRoamTerrainSection::tessellate(float vScale, float vLimit)
{
	recursiveTessellate( 
		&m_rootTriangleA, 
		m_distance1, m_distance2, m_distance0, 
		m_varianceTreeA, 1,
		vScale, vLimit);

	recursiveTessellate( 
		&m_rootTriangleB, 
		m_distance3, m_distance0, m_distance2, 
		m_varianceTreeB, 1,
		vScale, vLimit);
}

void cRoamTerrainSection::buildTriangleList()
{
	// lock the dynamic index buffer
	m_pIndexList = m_pIndexBuffer->lock(
		nWriteLock, 0, 0);
	m_totalIndices=0;

	// add all the triangles to the roamTerrain
	// in root triangle A
	recursiveBuildTriangleList( 
		&m_rootTriangleA, 
		0, 16, 16*17);

	// add all the triangles to the roamTerrain
	// in root triangle B
	recursiveBuildTriangleList( 
		&m_rootTriangleB, 
		(17*17)-1, 16*17, 16);

	// unlock the index buffer
	m_pIndexBuffer->unlock();
	m_pIndexList=0;
}



// recursive functions used internally
void cRoamTerrainSection::split( cTriTreeNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->leftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->baseNeighbor && (tri->baseNeighbor->baseNeighbor != tri) )
		split(tri->baseNeighbor);

	// Create children and link into mesh
	tri->leftChild  = roamTerrain()->requestTriNode();
	tri->rightChild = roamTerrain()->requestTriNode();

	debug_assert(tri->leftChild != tri, "recursive link");
	debug_assert(tri->rightChild != tri, "recursive link");

	// If creation failed, just exit.
	if ( !tri->leftChild 
		|| !tri->rightChild)
	{
		tri->leftChild  = 0;
		tri->rightChild = 0;
		return;
	}

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->leftChild->baseNeighbor  = tri->leftNeighbor;
	tri->leftChild->leftNeighbor  = tri->rightChild;

	tri->rightChild->baseNeighbor  = tri->rightNeighbor;
	tri->rightChild->rightNeighbor = tri->leftChild;

	// Link our Left Neighbor to the new children
	if (tri->leftNeighbor != NULL)
	{
		if (tri->leftNeighbor->baseNeighbor == tri)
			tri->leftNeighbor->baseNeighbor = tri->leftChild;
		else if (tri->leftNeighbor->leftNeighbor == tri)
			tri->leftNeighbor->leftNeighbor = tri->leftChild;
		else if (tri->leftNeighbor->rightNeighbor == tri)
			tri->leftNeighbor->rightNeighbor = tri->leftChild;
		else
		{
			debug_assert(0, "Invalid Left Neighbor!");
		}
	}

	// Link our Right Neighbor to the new children
	if (tri->rightNeighbor != NULL)
	{
		if (tri->rightNeighbor->baseNeighbor == tri)
			tri->rightNeighbor->baseNeighbor = tri->rightChild;
		else if (tri->rightNeighbor->rightNeighbor == tri)
			tri->rightNeighbor->rightNeighbor = tri->rightChild;
		else if (tri->rightNeighbor->leftNeighbor == tri)
			tri->rightNeighbor->leftNeighbor = tri->rightChild;
		else
		{
			debug_assert(0, "Invalid Right Neighbor!");
		}
	}

	// Link our Base Neighbor to the new children
	if (tri->baseNeighbor != NULL)
	{
		if ( tri->baseNeighbor->leftChild )
		{
			tri->baseNeighbor->leftChild->rightNeighbor = tri->rightChild;
			tri->baseNeighbor->rightChild->leftNeighbor = tri->leftChild;
			tri->leftChild->rightNeighbor = tri->baseNeighbor->rightChild;
			tri->rightChild->leftNeighbor = tri->baseNeighbor->leftChild;
		}
		else
			split( tri->baseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	}
	else
	{
		// An edge triangle, trivial case.
		tri->leftChild->rightNeighbor = 0;
		tri->rightChild->leftNeighbor = 0;
	}
}


void cRoamTerrainSection::recursiveTessellate( 
	cTriTreeNode *tri, 
	float distA, float distB, float distC, 
	float* pVTree, uint16 vIndex,
	float vScale, float vLimit)
{
	if ((vIndex<<1)+1 < k_totalVariances)
	{
		float midDist = (distB + distC)*0.5f;
		
		// if we have not been split already, 
		// see if we should
		if (!tri->leftChild)
		{
			float vRatio = (pVTree[vIndex]*vScale)/(midDist+0.0001f);

			if (vRatio > vLimit)
			{
				// subdivide this triangle
				split(tri);
			}
		}

		// if we have a set of children, continue
		if (tri->leftChild)
		{
			debug_assert(tri->leftChild, "invalid triangle node");
			debug_assert(tri->rightChild, "invalid triangle node");

			recursiveTessellate(tri->leftChild,
				midDist, distA, distB,
				pVTree, vIndex<<1,
				vScale, vLimit);

			recursiveTessellate(tri->rightChild,
				midDist, distA, distB,
				pVTree, (vIndex<<1)+1,
				vScale, vLimit);
		}
	}
}

void cRoamTerrainSection::recursiveBuildTriangleList( 
	cTriTreeNode *tri, 
	uint16 iCornerA, uint16 iCornerB, uint16 iCornerC)
{
	// if there are children, we draw them instead
	if (tri->leftChild)
	{
		debug_assert(
			tri->rightChild, 
			"invalid triangle node");
		
		uint16 iMidpoint = (iCornerB+iCornerC)>>1;
		recursiveBuildTriangleList(
			tri->leftChild,
			iMidpoint, iCornerA, iCornerB);
		recursiveBuildTriangleList(
			tri->rightChild,
			iMidpoint, iCornerC, iCornerA);

	}
	else if (m_totalIndices + 3 < m_maxIndices)
	{
		// add the local triangle to the index list
		m_pIndexList[m_totalIndices++]=iCornerC;
		m_pIndexList[m_totalIndices++]=iCornerB;
		m_pIndexList[m_totalIndices++]=iCornerA;
	}

}


float cRoamTerrainSection::recursiveComputeVariance(	
	uint16 iCornerA, uint16 iCornerB, uint16 iCornerC,
	float heightA, float heightB, float heightC,
	float* pVTree, uint16 vIndex)
{
	if (vIndex < k_totalVariances)
	{
		uint16 iMidpoint = (iCornerB+iCornerC)>>1;
		float midHeight = roamTerrain()->readWorldHeight(iMidpoint);
		float interpolatedHeight = (heightB+heightC)*0.5f;
		float variance = absoluteValue(midHeight - interpolatedHeight);

		// find the variance of our children
		float vLeft = recursiveComputeVariance(
			iMidpoint, iCornerA, iCornerB,
			midHeight, heightA, heightB,
			pVTree, vIndex<<1);

		float vRight = recursiveComputeVariance(
			iMidpoint, iCornerC, iCornerA,
			midHeight, heightC, heightA,
			pVTree, 1+(vIndex<<1));

		// local variance is the minimum of all three
		variance = maximum(variance, vLeft); 
		variance = maximum(variance, vRight); 

		// store the variance as 1/(variance+1)
		pVTree[vIndex] = variance;

		return variance;
	}
	// return a value which will be ignored by the parent
	// (because the minimum function is used with this result)
	return 0;
}

//***************************************************************
// end of file      ( cRoamTerrainSection.cpp )

//----------------------------------------------------------
//$Log: $