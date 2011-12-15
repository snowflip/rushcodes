#ifndef CROAMTERRAIN_H
#define CROAMTERRAIN_H
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
#include "terrain_system.h"
#include "roam_section.h"


//	Name Space declaration
namespace gaia
{


class cRoamTerrain: public cTerrain
{
public:

	enum e_constants
	{
		k_totalDetailLevels = 9,
		k_maxTriTreeNodes = (1024*64), //64k triangle nodes
		k_tessellationQueueSize = 1024,
	};

	// Public Data...

	// Creators...
    cRoamTerrain();
    ~cRoamTerrain();

	// Operators...
	virtual bool create(cSceneNode* pRootNode, cTexture* heightMap, const cRect3d& worldExtents, uint8 shift=4);
	virtual void destroy();

	virtual void submitSection(
		cTerrainSection* pSection)const;

	virtual void renderSection(
		cTerrainSection* pSection, 
		u32Flags activationFlags, 
		const cRenderEntry* entry)const;

	void reset();
	bool addToTessellationQueue(cRoamTerrainSection* pSection);
	void processTessellationQueue();

	// Accessors...
	cTriTreeNode* requestTriNode();
	cRoamTerrainSection* findSection(int sectionX, int sectionY);

private:

	cRoamTerrainSection* m_pRoamSectionArray;
	cTriTreeNode* m_pTriangleNodePool;
	uint32 m_nextTriNode;
	cRoamTerrainSection** m_tessellationQueue;
	uint32 m_tessellationQueueCount;

	// Private Functions...
	virtual bool allocateSectors();

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cRoamTerrain
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cRoamTerrain::cRoamTerrain()
:cTerrain()
,m_pRoamSectionArray(0)
,m_pTriangleNodePool(0)
,m_nextTriNode(0)
,m_tessellationQueue(0)
,m_tessellationQueueCount(0)
{
}

/*	~cRoamTerrain
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cRoamTerrain::~cRoamTerrain()
{
}

//- End of cRoamTerrain -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRoamTerrain.h )

//----------------------------------------------------------
//$Log: $