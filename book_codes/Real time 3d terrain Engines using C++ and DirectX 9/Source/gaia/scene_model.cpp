#define SCENEMODEL_CPP
#define CORE_DLL
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

#include "scene_model.h"
#include "scene_node.h"
#include "model_resource.h"
#include "game_host.h"
#include "subnode_group.h"
#include "camera.h"

using namespace gaia;


bool cSceneModel::create()
{
	return cSceneObject::create();
}

void cSceneModel::destroy()
{
	safe_release(m_pModelResource);
	safe_delete_array(m_boneMatrixList);
	m_totalBoneMatrices = 0;
	m_modelFrameIndex = 0;

	cSceneObject::destroy();
}


void cSceneModel::setModelResource(cModelResource* pModel, uint16 frameIndex)
{
	safe_release(m_pModelResource);
	safe_delete_array(m_boneMatrixList);
	m_totalBoneMatrices = 0;
	m_modelFrameIndex = frameIndex;

	m_pModelResource=pModel;

	if (m_pModelResource)
	{
		m_pModelResource->AddRef();

		const D3DXMESHCONTAINER_DERIVED* pContainer=meshContainer();

		if (pContainer && pContainer->pSkinInfo)
		{
			m_totalBoneMatrices = pContainer->pSkinInfo->GetNumBones();
			m_boneMatrixList = new cSIMDMatrix[m_totalBoneMatrices];
		}

		// update our local bounding box
		*accessLocalBounds() = pModel->boundingBox();

	}
}

void cSceneModel::prepareForRender()
{
	if (m_pModelResource && m_pModelResource->containsProgressiveMesh())
	{
		cCamera* pCamera = TheGameHost.activeCamera();

		// compute our distance to the camera
		cVector3 worldPos = worldPosition();
		cVector3 camPos = pCamera->worldPosition();

		float dist = camPos.distanceSquared(worldPos)/
			(pCamera->farPlane()*pCamera->farPlane());

		m_lod = realToInt32((1.0f-dist) * cModelResource::k_maxLOD);
	}
}

void cSceneModel::render()
{
	cSceneObject::render();

	const D3DXMESHCONTAINER_DERIVED* pMeshContainer = meshContainer();

	if (pMeshContainer != NULL && pMeshContainer->ppRenderMethodList)
	{
		if (pMeshContainer->pSkinInfo != NULL)
		{
			cSceneNode* pAnimationOwner = subNodeGroupOwner();
			debug_assert(pAnimationOwner, "no animation owner found for skin");

			cSubNodeGroup* pAnimData = pAnimationOwner->subNodeGroupData();
			debug_assert(pAnimData, "no animation data found for skin");

			// compute all the bone matrices
			for (uint32 iBone=0; iBone<m_totalBoneMatrices; ++iBone)
			{
				uint16 boneIndex = pMeshContainer->pBoneIndexList[iBone];
				const cSceneNode* pSceneNode = pAnimData->subNodePtr(boneIndex);
				const cSIMDMatrix& rBoneMatrix = pSceneNode->worldMatrix();

				D3DXMatrixMultiply(&m_boneMatrixList[iBone], &pMeshContainer->pBoneOffsetMatrices[iBone], &rBoneMatrix);
			}

			//
			// submit for rendering
			//
			int numBoneInfluences = pMeshContainer->NumBoneInfluences -1;
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			for (UINT iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{ 
				UINT iMaterial = pBoneComb[iAttrib].AttribId;

				cRenderMethod* pMethod = pMeshContainer->ppRenderMethodList[iMaterial];
				if (pMethod)
				{
					cEffectFile* pEffect = pMethod->getEffect(TheGameHost.currentRenderStage());
					cSurfaceMaterial* pMaterial = pMethod->getMaterial(TheGameHost.currentRenderStage());

					if (pEffect && pMaterial)
					{
						uint16 numPasses = pEffect->totalPasses();
						for( uint16 iPass = 0; iPass < numPasses; iPass++ )
						{
							cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
							
							pRenderEntry->hEffectFile = (uint8)pEffect->resourceHandle();
							pRenderEntry->boneCount = (uint8)numBoneInfluences;
							pRenderEntry->detailLevel = m_lod;
							pRenderEntry->hSurfaceMaterial = pMaterial->resourceHandle();
							pRenderEntry->modelType = cRenderEntry::k_modelEntry;
							pRenderEntry->hModel = m_pModelResource->resourceHandle();
							pRenderEntry->modelParamA = m_modelFrameIndex;
							pRenderEntry->modelParamB = iAttrib;
							pRenderEntry->renderPass = (uint8)iPass;
							pRenderEntry->object = (cSceneNode*)this;
							pRenderEntry->userData = iMaterial;

							DisplayManager.closeRenderQueue(pRenderEntry);
						}
					}
				}
			}
		}
		else
		{
			for (UINT iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
			{
				cRenderMethod* pMethod = pMeshContainer->ppRenderMethodList[iMaterial];
				if (pMethod)
				{
					cEffectFile* pEffect = pMethod->getEffect(TheGameHost.currentRenderStage());
					cSurfaceMaterial* pMaterial = pMethod->getMaterial(TheGameHost.currentRenderStage());

					if (pEffect && pMaterial)
					{
						uint16 numPasses = pEffect->totalPasses();
						for( uint16 iPass = 0; iPass < numPasses; iPass++ )
						{
							cRenderEntry* pRenderEntry = DisplayManager.openRenderQueue();
							
							pRenderEntry->hEffectFile = (uint8)pEffect->resourceHandle();
							pRenderEntry->hSurfaceMaterial = pMaterial->resourceHandle();
							pRenderEntry->detailLevel = m_lod;
							pRenderEntry->modelType = cRenderEntry::k_modelEntry;
							pRenderEntry->hModel = m_pModelResource->resourceHandle();
							pRenderEntry->modelParamA = m_modelFrameIndex;
							pRenderEntry->modelParamB = iMaterial;
							pRenderEntry->renderPass = (uint8)iPass;
							pRenderEntry->object = (cSceneNode*)this;
							pRenderEntry->userData = iMaterial;

							DisplayManager.closeRenderQueue(pRenderEntry);
						}
					}
				}
			}
		}
	}
}

void cSceneModel::renderCallback(cRenderEntry* entry, u32Flags activationFlags)
{
	// if we queued ourselved for rendering with the 
	// display manager, we will get this function
	// called when it is our turn to render

	// activationFlags contains a set of bit flags 
	// held in the eActivationFlagBits enum (render_queue.h)
	// which tell us what resources we need to activate 
	// in order to render ourselves.
	profile_scope(cTerrainSystem_renderSection);
	LPDIRECT3DDEVICE9 d3dDevice = TheGameHost.d3dDevice();
	const D3DXMESHCONTAINER_DERIVED* pMeshContainer = meshContainer();
	bool skinModel=pMeshContainer->pSkinInfo != NULL;

	UINT iMaterial = entry->userData;
	cRenderMethod* pMethod = pMeshContainer->ppRenderMethodList[iMaterial];
	cEffectFile* pEffect = pMethod->getEffect(TheGameHost.currentRenderStage());
	cSurfaceMaterial* pMaterial = pMethod->getMaterial(TheGameHost.currentRenderStage());

	if (pEffect && pMaterial)
	{
		// do we need to activate the render method?
		if (TEST_BIT(activationFlags,k_activateRenderMethod))
		{
			pEffect->begin();
		}

		// do we need to activate the render pass?
		if (TEST_BIT(activationFlags,k_activateRenderMethodPass)
			|| TEST_BIT(activationFlags,k_activateRenderMethodParam)
			|| TEST_BIT(activationFlags,k_activateRenderMethodLOD))
		{
			m_pModelResource->setLOD(m_lod);
			if (skinModel)
			{
				int numBoneInfluences = pMeshContainer->NumBoneInfluences -1;
				pEffect->setParameter(cEffectFile::k_boneInfluenceCount, &numBoneInfluences);
			}
			pEffect->activatePass(entry->renderPass);
		}

		// do we need to activate the surface material
		if (TEST_BIT(activationFlags,k_activateSurfaceMaterial))
		{
			pEffect->applySurfaceMaterial(pMaterial);
		}

		// skin models need to load their bone matrices
		if (skinModel)
		{
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			UINT iAttrib = entry->modelParamB;
			UINT iMaterial = entry->userData;
			UINT iMatrixIndex;
			UINT iPaletteEntry;

			// load the proper matrices
			for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumBoneMatrices; ++iPaletteEntry)
			{
				iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				
				if (iMatrixIndex != UINT_MAX)
				{
					pEffect->setMatrixInArray(cEffectFile::k_worldMatrixArray, iPaletteEntry, &m_boneMatrixList[iMatrixIndex]);
				}
			}

			// Set CurNumBones to select the correct vertex shader for the number of bones
			//pEffect->SetInt( "CurNumBones", pMeshContainer->NumInfl -1);
			int numBoneInfluences = pMeshContainer->NumBoneInfluences -1;
			pEffect->setParameter(cEffectFile::k_boneInfluenceCount, &numBoneInfluences);
		}
		else
		{
			const cCamera* pCamera = TheGameHost.activeCamera();
			D3DXMATRIX matWorldViewProj =  (D3DXMATRIX)worldMatrix() * (D3DXMATRIX)pCamera->viewProjMatrix();
			// set the view matrix
			pEffect->setMatrix(cEffectFile::k_worldViewProjMatrix, &matWorldViewProj);
		}
		pEffect->setMatrix(cEffectFile::k_worldMatrix, &worldMatrix());


		// draw the mesh subset
		m_pModelResource->renderModelSubset( entry->modelParamA, entry->modelParamB );
	}
}

//****************************************************************************************
// end of file      ( Math.cpp )

