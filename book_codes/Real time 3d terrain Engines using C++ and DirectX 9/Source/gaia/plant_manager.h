#ifndef CPLANTMANAGER_H
#define CPLANTMANAGER_H
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

class cPlantManager 
{
public:

    cPlantManager();
    ~cPlantManager();

	void create(cTerrain* pTerrain,
				cModelResource* pModel,
				int maxPlants,
				float minElevation,
				float maxElevation,
				float minNormalZ,
				float maxNormalZ,
				int spacing,
				bool randomRotation=true);
	void destroy();


private:

	cSceneModel** m_plantArray;
	int m_plantCount;
	cModelResource* m_pPlantModel;

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cPlantManager
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cPlantManager::cPlantManager()
:m_plantArray(0)
,m_plantCount(0)
,m_pPlantModel(0)
{
}


/*	~cPlantManager
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cPlantManager::~cPlantManager()
{
}

//- End of cPlantManager ----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cPlantManager.h )

//----------------------------------------------------------
//$Log: $