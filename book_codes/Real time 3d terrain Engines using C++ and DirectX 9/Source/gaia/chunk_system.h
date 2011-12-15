#ifndef CHUNKTERRAIN_H
#define CHUNKTERRAIN_H
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
#include "chunk_section.h"


//	Name Space declaration
namespace gaia
{



class cChunkTerrain: public cTerrain
{
public:

	enum e_constants
	{
		k_maxDetailLevels = cChunkTerrainSection::k_maxDetailLevels,
		k_minTessellationShift = cChunkTerrainSection::k_minTessellationShift,
	};

	enum e_index_type
	{
		k_chunk = 0,
		k_skirt,
		k_totalIndexTypes
	};

	// Creators...
    cChunkTerrain();
    ~cChunkTerrain();

	// Operators...
	virtual bool create(
		cSceneNode* pRootNode, 
		cTexture* heightMap, 
		const cRect3d& worldExtents, 
		uint8 shift=5);

	virtual void destroy();

	virtual void submitSection(
		cTerrainSection* pSection)const;

	virtual void renderSection(
		cTerrainSection* pSection, 
		u32Flags activationFlags, 
		const cRenderEntry* entry)const;

	// Accessors...
	cChunkTerrainSection* findSection(int sectionX, int sectionY);
	uint32 totalLOD()const {return m_detailLevels;}

private:

	cChunkTerrainSection* m_pChunkSectionArray;
	cIndexBuffer* m_indexBufferList[k_totalIndexTypes][k_maxDetailLevels];
	uint32	m_detailLevels;

	// Private Functions...
	virtual bool allocateSectors();
	virtual bool buildVertexBuffer();
	virtual bool buildIndexBuffer();

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cChunkTerrain
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cChunkTerrain::cChunkTerrain()
:cTerrain()
,m_pChunkSectionArray(0)
,m_detailLevels(0)
{
	memset(m_indexBufferList, 0, sizeof(m_indexBufferList));
}

/*	~cChunkTerrain
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cChunkTerrain::~cChunkTerrain()
{
}

//- End of cChunkTerrain -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cChunkTerrain.h )

//----------------------------------------------------------
//$Log: $