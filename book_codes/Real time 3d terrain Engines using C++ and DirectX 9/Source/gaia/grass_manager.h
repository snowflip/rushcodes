#ifndef CGRASSMANAGER_H
#define CGRASSMANAGER_H
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
#include "gaia\terrain_system.h"
#include "gaia\model_resource.h"
#include "gaia\grass_patch.h"

//	Name Space declaration
namespace gaia
{


class cTerrain;

class cGrassManager 
{
public:

    cGrassManager();
    ~cGrassManager();

	void create(cTerrain* pTerrain,
				cModelResource* pModel,
				int maxPlants,
				float minElevation,
				float maxElevation,
				float minNormalZ,
				float maxNormalZ,
				int spacing);
	void destroy();


private:

	cGrassPatch** m_plantArray;
	int m_plantCount;
	cModelResource* m_pPlantModel;

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cGrassManager
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cGrassManager::cGrassManager()
:m_plantArray(0)
,m_plantCount(0)
,m_pPlantModel(0)
{
}

/*	~cGrassManager
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cGrassManager::~cGrassManager()
{
}

//- End of cGrassManager ----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cGrassManager.h )

//----------------------------------------------------------
//$Log: $