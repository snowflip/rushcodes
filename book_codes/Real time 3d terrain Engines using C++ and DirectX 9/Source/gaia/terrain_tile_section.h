#ifndef TERRAINTILESECTION_H
#define TERRAINTILESECTION_H
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

class cTiledTerrain;
class cCamera;

class cTiledTerrainSection : public cTerrainSection
{
public:

	enum e_constants
	{
		k_totalDetailLevels = 4,
	};

	// Creators...
	cTiledTerrainSection();
	~cTiledTerrainSection();

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
	int getLod()const {return m_lod;}

	// Accessors...
	cTiledTerrain* terrainTileSystem()const {return (cTiledTerrain*)terrainSystem();}

private:

	int m_lod;
	float m_errorMetric[k_totalDetailLevels];


	void computeErrorMetricTable();
};

//- Inline Functions ---------------------------------------------------------------------

/*	cTiledTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cTiledTerrainSection::cTiledTerrainSection()
:cTerrainSection()
,m_lod(0)
{
}

/*	~cTiledTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cTiledTerrainSection::~cTiledTerrainSection()
{
	destroy();
}

//. Accessors ............................................................................


//- End of cTiledTerrainSection -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cTiledTerrainSection.h )

//----------------------------------------------------------
//$Log: $