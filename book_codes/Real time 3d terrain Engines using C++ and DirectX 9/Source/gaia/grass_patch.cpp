#define CGRASSPATCH_CPP
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

#include "grass_patch.h"
#include "terrain_system.h"
#include "game_host.h"

using namespace gaia;

void cGrassPatch::setup(cTerrain* pTerrain, uint16 x, uint16 y)
{
	float minHeight = MAX_REAL32;
	float maxHeight = MIN_REAL32;

	for (int j=0;j<4;++j)
	{
		for(int i=0; i<4; ++i)
		{
			int idx = (j<<2)+i;

			m_points[idx].vec3() = pTerrain->readWorldNormal(x+i,y+j);
			m_points[idx].w = pTerrain->readWorldHeight(x+i,y+j);

			minHeight = minimum(minHeight, m_points[idx].w); 
			maxHeight = maximum(maxHeight, m_points[idx].w); 
		}
	}

	// build the local matrix
	cSIMDMatrix* pMatrix = accessLocalMatrix();

	// scale the patch by 3
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 
		pTerrain->mapScale().x,//*3.0f,
		pTerrain->mapScale().y,//*3.0f,
		pTerrain->mapScale().x);

	const cRect3d& worldRect = pTerrain->worldExtents();

	D3DXMATRIX location;
	D3DXMatrixTranslation(
		&location, 
		(x*pTerrain->mapScale().x)+worldRect.x0,
		(y*pTerrain->mapScale().y)+worldRect.y0,
		minHeight/pTerrain->mapScale().x);


	// choose a random spot on the mesh
	D3DXMatrixMultiply(pMatrix, &scale, &location);

	// update our local bounding box
	cRect3d* plocalBounds = accessLocalBounds();
	plocalBounds->set(0.0f,1.0f,0.0f,1.0f,0.0f,maxHeight/pTerrain->mapScale().x);

}

void cGrassPatch::renderCallback(cRenderEntry* entry, u32Flags activationFlags)
{
	UINT iMaterial = entry->userData;
	const D3DXMESHCONTAINER_DERIVED* pMeshContainer = meshContainer();
	cRenderMethod* pMethod = pMeshContainer->ppRenderMethodList[iMaterial];
	cEffectFile* pEffect = pMethod->getEffect(TheGameHost.currentRenderStage());

	if (pEffect)
	{
		pEffect->setParameter(cEffectFile::k_patchCorners, (void*)m_points, sizeof(m_points));
	}

	cSceneModel::renderCallback(entry, activationFlags);
}

cSceneNode* cGrassPatch::createSubNode(cModelResource* pModelResource, int frameIndex)
{
	cGrassPatch* object = new cGrassPatch;
	cSceneNode* node = (cSceneNode*)object;

	node->create();
	object->setModelResource(pModelResource, frameIndex);
	node->setSubNodeGroupOwner(this);
	object->setPoints((cVector4*)m_points);
	return node;
}

void cGrassPatch::setPoints(cVector4* pPoints)
{
	memcpy(m_points, pPoints, sizeof(cVector4)*16);
}


//***************************************************************
// end of file      ( cGrassPatch.cpp )

//----------------------------------------------------------
//$Log: $