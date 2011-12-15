#define SUBNODEGROUP_CPP
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

#include "subnode_group.h"
#include "game_host.h"
#include "scene_object.h"

using namespace gaia;

bool cSubNodeGroup::create(cSceneNode* pRootNode, cPoolHandle ModelResource)
{
	// destroy any local data
	destroy();

	m_pParentNode = pRootNode;
	m_pModelResource = (cModelResource*)TheGameHost.displayManager().modelPool().getResource(ModelResource);
	if (m_pModelResource)
	{
		// clone the frame tree
		m_totalSubNodes = m_pModelResource->totalFrames();
		m_totalNamedSubNodes = 0;
		m_subNodePtrList = new cSceneNode*[m_totalSubNodes];
		m_namedSubNodePtrList = new cSceneNode*[m_totalSubNodes];

		// copy the frame nodes locally
		for (uint16 i=0; i<m_totalSubNodes; ++i)
		{
			// is a scene object attached to this node?
			D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)m_pModelResource->frame(i)->pMeshContainer;

			// create this node
			if (pMeshContainer)
			{
				m_subNodePtrList[i] = pRootNode->createSubNode(m_pModelResource, i);
			}
			else
			{
				m_subNodePtrList[i] = pRootNode->createSubNode();
			}

			// link to the proper parent node
			int parentIndex = m_pModelResource->frame(i)->parentIndex;

			debug_assert(parentIndex == MAX_UINT16
						|| parentIndex < m_pModelResource->frame(i)->frameIndex,
						"invalid model resource");
			
			if(parentIndex == MAX_UINT16)
			{
				m_subNodePtrList[i]->attachToParent(pRootNode);
			}
			else
			{
				m_subNodePtrList[i]->attachToParent(m_subNodePtrList[parentIndex]);
			}
			// set the local matrix of the sub node
			cSIMDMatrix* pLocalMatrix = m_subNodePtrList[i]->accessLocalMatrix();
			memcpy(pLocalMatrix, m_pModelResource->frame(i)->TransformationMatrix, sizeof(cSIMDMatrix));

			// if the source was a named node, register with the animation controller
			if (m_pModelResource->frame(i)->Name)
			{
				m_namedSubNodePtrList[m_totalNamedSubNodes] = m_subNodePtrList[i];
				++m_totalNamedSubNodes;
			}
		}
		m_animController.create(this, ModelResource);

		return true;
	}

	return false;
}

void cSubNodeGroup::destroy()
{
//	SAFE_RELEASE(m_pAnimController);

	m_animController.destroy();

	// delete our allocations
	if (m_subNodePtrList)
	{
		for (uint16 i=0; i<m_totalSubNodes; ++i)
		{
			m_pParentNode->destroySubNode(m_subNodePtrList[i]);
		}
	}
	safe_delete_array(m_subNodePtrList);
	safe_delete_array(m_namedSubNodePtrList);
	m_totalSubNodes = 0;
	m_totalNamedSubNodes = 0;

	// release our reference to the source model
	safe_release(m_pModelResource);
}

void cSubNodeGroup::update()
{
    // play the current animation
	m_animController.update();
}

void cSubNodeGroup::adjustForAnimationStep()
{
	// signal all the subnodes to recalc
	for (uint16 i=0; i<m_totalNamedSubNodes; ++i)
	{
		m_namedSubNodePtrList[i]->setUpdatedLocalMatrixFlag();
	}
}

//***************************************************************
// end of file      ( cSubNodeGroup.cpp )

//----------------------------------------------------------
//$Log: $