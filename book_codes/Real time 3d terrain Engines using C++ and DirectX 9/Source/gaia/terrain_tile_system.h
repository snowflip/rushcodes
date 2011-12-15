#ifndef CTERRAINTILESYSTEM_H
#define CTERRAINTILESYSTEM_H
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
#include "terrain_system.h"


//	Name Space declaration
namespace gaia
{

class detail_index_data;
class cTiledTerrainSection;

class cTiledTerrain: public cTerrain
{
public:

	enum e_constants
	{
		k_totalDetailLevels = 4,
		k_totalShapes = 16,
	};

	enum e_side_flags
	{
		k_top = 0,
		k_left,
		k_right,
		k_bottom,
		k_totalSides
	};

	// Data Types & Constants...
	struct sDetailLevel
	{
		cIndexBuffer*  tileBodies[k_totalShapes];
		cIndexBuffer*  tileConnectors[k_totalSides][k_totalDetailLevels];
	};

	// this set of bit fields is used to record which of our
	// cIndexBuffers we wish to render. This is passed as user
	// data to the render queue, so we can render the proper
	// index buffer when the render callback is triggered
	union sUserData
	{
		struct
		{
			uint32 bodyTileFlag		:1;
			uint32 detailLevel		:2;
			uint32 side				:2;
			uint32 connectionLevel	:4;
			uint32 shapeNumber		:4;
		};
		uint32 value;
	};

	// Public Data...

	// Creators...

    cTiledTerrain();
    virtual ~cTiledTerrain();

	// Operators...


	// Mutators...
	virtual bool create(cSceneNode* pRootNode, cTexture* heightMap, const cRect3d& worldExtents, uint8 shift=3);
	virtual void destroy();

	virtual void submitSection(
		cTerrainSection* pSection)const;

	virtual void renderSection(
		cTerrainSection* pSection, 
		u32Flags activationFlags, 
		const cRenderEntry* entry)const;

	// Accessors...

private:

	cTiledTerrainSection* m_pTileSectorArray;

	// Private Data...
	static int s_indexBufferRefCount;
	static sDetailLevel s_detailLevel[k_totalDetailLevels];

	// Private Functions...
	virtual bool allocateSectors();
	bool createIndexBuffer(cIndexBuffer** ppIndexBuffer, const detail_index_data* source);
	bool generateDetailLevels();

    cTiledTerrain(const cTiledTerrain& Src);
    cTiledTerrain& operator=(const cTiledTerrain& Src);

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cTiledTerrain
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cTiledTerrain::cTiledTerrain()
:cTerrain()
,m_pTileSectorArray(0)
{
}

/*	~cTiledTerrain
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cTiledTerrain::~cTiledTerrain()
{
}

//- End of cTiledTerrain -----------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cTiledTerrain.h )

//----------------------------------------------------------
//$Log: $