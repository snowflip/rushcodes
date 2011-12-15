#define CPLANTMANAGER_CPP
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

#include "plant_manager.h"
#include "game_host.h"

using namespace gaia;

struct gridLoc
{
	uint16 x;
	uint16 y;
	uint8 used;
};

void cPlantManager::create(cTerrain* pTerrain,
			cModelResource* pModel,
			int maxPlants,
			float minElevation,
			float maxElevation,
			float minNormalZ,
			float maxNormalZ,
			int spacing,
			bool randomRotation)
{
	m_pPlantModel = pModel;
	m_pPlantModel->AddRef();

	cPoolHandle modelHandle = m_pPlantModel->resourceHandle();

	const cRect3d& worldRect = pTerrain->worldExtents();
	const cVector3& worldScale = pTerrain->mapScale();

	int gridSizeX = pTerrain->tableWidth() / spacing;
	int gridSizeY = pTerrain->tableHeight() / spacing;

	int maxLoc = gridSizeX * gridSizeY;
	int totalLoc = 0;
	gridLoc* tempTable = new gridLoc[maxLoc];
	memset(tempTable, 0, sizeof(gridLoc)*maxLoc);

	// find all valid positions
	for (int x=0; x< gridSizeX; ++x)
	{
		for (int y=0; y< gridSizeY; ++y)
		{
			int locX = x *spacing;
			int locY = y *spacing;

			// is this a valid spot for a plant?
			float height = pTerrain->readWorldHeight(
				locX,
				locY);
			const cVector3& normal = pTerrain->readWorldNormal(
				locX,
				locY);

			if (height > minElevation && height < maxElevation)
			{
				if (normal.z > minNormalZ
					&& normal.z < maxNormalZ)
				{
					tempTable[totalLoc].x = locX;				
					tempTable[totalLoc].y = locY;	
					++totalLoc;
				}
			}
		}
	}

	maxPlants = minimum(maxPlants, totalLoc);

	// pick some random plant locations
	m_plantArray = new cSceneModel*[maxPlants];
	memset(m_plantArray, 0, sizeof(cSceneModel*)*maxPlants);
	m_plantCount = 0;
	for(int i=0; i<maxPlants;++i)
	{
		// find a location that's available
		int index = 0;
		int sanity_counter = 0;

		while(tempTable[index].used)
		{
			index = RandomNumberInRange(0, totalLoc-1);

			debug_assert(index >=0, "invalid_index");
			debug_assert(index <totalLoc, "invalid_index");

			
			++sanity_counter;
			if (sanity_counter > 1000)
			{
				// stop making plants
				delete [] tempTable;
				return;
			}
		}

		tempTable[index].used = 1;

		m_plantArray[m_plantCount]= new cSceneModel;
		m_plantArray[m_plantCount]->createFromResource(modelHandle);
		m_plantArray[m_plantCount]->attachToParent(&TheGameHost.rootNode());

		// get the terrain height at this point
		float height = pTerrain->readWorldHeight(
			tempTable[index].x,
			tempTable[index].y);

		cVector3 modelPos(
			(tempTable[index].x*worldScale.x)+worldRect.x0,
			(tempTable[index].y*worldScale.y)+worldRect.y0,
			height);

		m_plantArray[m_plantCount]->orientation().setPosition(
			modelPos);

		// rotate the tree to some random orientation
		float rotationAngle =0.0f;
		if (randomRotation)
		{
			rotationAngle = RandomNumberInRange(0.0f, TWO_PI);
		}
		m_plantArray[m_plantCount]->orientation().setRotation(
			cVector3(0.0f,0.0f,1.0f), rotationAngle);

		++m_plantCount;
	}

	delete [] tempTable;
}

void cPlantManager::destroy()
{
	for (int i=0;i<m_plantCount;++i)
	{
		if (m_plantArray[i])
		{
			m_plantArray[i]->destroy();
		}
	}
	safe_delete_array(m_plantArray);
	m_plantCount=0;

	safe_release(m_pPlantModel);
}

//***************************************************************
// end of file      ( cPlantManager.cpp )

//----------------------------------------------------------
//$Log: $