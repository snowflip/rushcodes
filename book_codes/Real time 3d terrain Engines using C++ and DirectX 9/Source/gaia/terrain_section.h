#ifndef TERRAINSECTION_H
#define TERRAINSECTION_H
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


//	Name Space declaration
namespace gaia
{

class cTerrain;

class cTerrainSection : public cSceneObject
{
public:

	// Creators...
	cTerrainSection();
	~cTerrainSection();

	// Mutators...
	virtual bool create(
		cSceneNode* pRootNode,
		cTerrain* pParentSystem, 
		uint16 sectorX, uint16 sectorY,
		uint16 heightMapX, uint16 heightMapY,
		uint16 xVerts, uint16 yVerts, 
		const cRect2d& worldRect);
	virtual void destroy();
	virtual void render();
	virtual void renderCallback(cRenderEntry* entry, u32Flags activationFlags);


	// Accessors...
	uint16 sectorX()const{return m_sectorX;}
	uint16 sectorY()const{return m_sectorY;}

	cTerrain* terrainSystem()const {return m_pTerrainSystem;}
	cVertexBuffer* sectorVertices() {return m_pSectorVerts;}

protected:

	cTerrain* m_pTerrainSystem;
	cVertexBuffer* m_pSectorVerts;
	uint16 m_heightMapX;
	uint16 m_heightMapY;
	uint16 m_sectorX; 
	uint16 m_sectorY; 
	uint16 m_xVerts; 
	uint16 m_yVerts; 
	cRect3d m_worldRect;

	virtual bool buildVertexBuffer();

};

//- Inline Functions ---------------------------------------------------------------------

/*	cTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cTerrainSection::cTerrainSection()
:cSceneObject()
,m_pTerrainSystem(0)
,m_pSectorVerts(0)
,m_xVerts(0)
,m_yVerts(0)
{
}

/*	~cTerrainSection
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cTerrainSection::~cTerrainSection()
{
	destroy();
}

//. Accessors ............................................................................


//- End of cTerrainSection -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cTerrainSection.h )

//----------------------------------------------------------
//$Log: $