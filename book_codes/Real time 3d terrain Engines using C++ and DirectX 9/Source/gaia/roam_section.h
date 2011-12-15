#ifndef CROAMSECTION_H
#define CROAMSECTION_H
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
#include "quad_tree.h"
#include "model_resource.h"
#include "scene_object.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "terrain_section.h"


//	Name Space declaration
namespace gaia
{

/*	cTriTreeNode
-----------------------------------------------------------------
    
    This is a single node within the ROAM 
	binrary triangle tree. A pool of these
	structures are stored within the 
	cRoamTerrain class for allocation.
    
-----------------------------------------------------------------
*/

struct cTriTreeNode
{
	cTriTreeNode *baseNeighbor;
	cTriTreeNode *leftNeighbor;
	cTriTreeNode *rightNeighbor;
	cTriTreeNode *leftChild;
	cTriTreeNode *rightChild;
};

class cRoamTerrain;
class cCamera;

class cRoamTerrainSection : public cTerrainSection
{
public:

	enum e_constants
	{
		k_totalDetailLevels = 9,
		k_totalVariances = 1<<9,
	};

	// Creators...
	cRoamTerrainSection();
	~cRoamTerrainSection();

	// Mutators...
	virtual bool create(
		cSceneNode* pRootNode,
		cTerrain* pParentSystem, 
		uint16 sectorX, uint16 sectorY,
		uint16 heightMapX, uint16 heightMapY,
		uint16 xVerts, uint16 yVerts, 
		const cRect2d& worldRect);
	virtual void destroy();

	virtual void prepareForRender();

	void reset();
	void tessellate(float vScale, float vLimit);
	void buildTriangleList();
	void computeVariance();

	// recursive functions used internally
	void	split( cTriTreeNode *tri);
	void	recursiveTessellate( 
		cTriTreeNode *tri, 
		float distA, float distB, float distC, 
		float* pVTree, uint16 vIndex,
		float vScale, float vLimit);

	void	recursiveBuildTriangleList( 
		cTriTreeNode *tri, 
		uint16 iCornerA, 
		uint16 iCornerB, 
		uint16 iCornerC);

	float	recursiveComputeVariance(	
		uint16 iCornerA, uint16 iCornerB, uint16 iCornerC,
		float heightA, float heightB, float heightC,
		float* pVTree, uint16 vIndex);

	// Accessors...
	cRoamTerrain* roamTerrain()const 
		{return (cRoamTerrain*)terrainSystem();}

	cTriTreeNode* triangleA() {return &m_rootTriangleA;}
	cTriTreeNode* triangleB() {return &m_rootTriangleB;}

	float queueSortValue()const {return m_queueSortValue;}
	uint16 totalIndices()const {return m_totalIndices;}
	cIndexBuffer* indexBuffer() {return m_pIndexBuffer;}

private:

	cTriTreeNode m_rootTriangleA;
	cTriTreeNode m_rootTriangleB;
	float m_varianceTreeA[k_totalVariances];
	float m_varianceTreeB[k_totalVariances];
	float m_distance0;
	float m_distance1;
	float m_distance2;
	float m_distance3;
	float m_queueSortValue;
	uint16 m_maxIndices;
	uint16 m_totalIndices;
	uint16* m_pIndexList;
	cIndexBuffer* m_pIndexBuffer;

	cTriTreeNode* m_leftNeighborOfA;
	cTriTreeNode* m_rightNeighborOfA;
	cTriTreeNode* m_leftNeighborOfB;
	cTriTreeNode* m_rightNeighborOfB;

};

//- Inline Functions ---------------------------------------------------------------------

/*	cRoamTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cRoamTerrainSection::cRoamTerrainSection()
:cTerrainSection()
,m_maxIndices(0)
,m_totalIndices(0)
,m_pIndexList(0)
,m_pIndexBuffer(0)
{
	// setup our base information
	memset(&m_rootTriangleA, 0, sizeof(m_rootTriangleA));
	memset(&m_rootTriangleB, 0, sizeof(m_rootTriangleB));
	memset(m_varianceTreeA, 0, sizeof(m_varianceTreeA));
	memset(m_varianceTreeB, 0, sizeof(m_varianceTreeB));
}

/*	~cRoamTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cRoamTerrainSection::~cRoamTerrainSection()
{
	destroy();
}


//- End of cRoamTerrainSection -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRoamTerrainSection.h )

//----------------------------------------------------------
//$Log: $