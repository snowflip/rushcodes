#ifndef CHUNKSECTION_H
#define CHUNKSECTION_H
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
#include "terrain_section.h"


//	Name Space declaration
namespace gaia
{


class cChunkTerrain;
class cCamera;

class cChunkTerrainSection : public cTerrainSection
{
public:

	enum e_constants
	{
		k_minTessellationShift = 2,
		k_maxDetailLevels = 3,
		k_topLod = 2,
		k_maxRenderEntries = 64,
		k_cellShift = 2,
	};

	// must be 32bits total
	union sRenderEntry
	{
		struct
		{
			uint8 level;
			uint8 offsetX;
			uint8 offsetY;
			uint8 skirtOrTile;
		};
		uint32 value;
	};

	// Creators...
	cChunkTerrainSection();
	~cChunkTerrainSection();

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

	void buildErrorMetricTree();
	void buildSkirtBuffers();

	// recursive functions used internally
	void	recursiveTessellate( 
		float distA, float distB, float distC, float distD,
		int level, int levelX, int levelY,
		float vScale, float vLimit);

	// Accessors...
	cChunkTerrain* chunkTerrain()const 
		{return (cChunkTerrain*)terrainSystem();}

	uint32 totalRenderEntries()const {return m_totalRenderEntries;}
	const sRenderEntry& renderEntry(uint32 i)const;

private:

	float* m_errorMetricTree[k_maxDetailLevels];
	sRenderEntry m_renderList[k_maxRenderEntries];
	uint16 m_totalRenderEntries;
	uint16 m_totalLevels;

	virtual bool buildVertexBuffer();
};

//- Inline Functions ---------------------------------------------------------------------

/*	cChunkTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cChunkTerrainSection::cChunkTerrainSection()
:cTerrainSection()
,m_totalRenderEntries(0)
{
	// setup our base information
	memset(m_renderList, 0, sizeof(m_renderList));
	memset(m_errorMetricTree, 0, sizeof(m_errorMetricTree));
}

/*	~cChunkTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cChunkTerrainSection::~cChunkTerrainSection()
{
	destroy();
}

inline const cChunkTerrainSection::sRenderEntry& 
cChunkTerrainSection::renderEntry(uint32 i)const
{
	debug_assert(i<m_totalRenderEntries, "invalid render entry requested");
	debug_assert(m_renderList, "invalid render entry list");
	return m_renderList[i];
}

//- End of cChunkTerrainSection -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cChunkTerrainSection.h )

//----------------------------------------------------------
//$Log: $