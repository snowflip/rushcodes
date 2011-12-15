#define SCENEOBJECT_CPP
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
#include "scene_object.h"
#include "scene_node.h"
#include "model_resource.h"
#include "game_host.h"
#include "subnode_group.h"

using namespace gaia;


bool cSceneObject::create()
{
	bool result = cSceneNode::create();

	if (result)
	{
		attachToQuadTree(&TheGameHost.quadTree());
	}

	return result;
}

/*	Destroy
------------------------------------------------------------------------------------------
	
	Destroys the object. The object is removed from it's parent (if any) and all Children
	Objects are orphaned (parent set to NULL).
	
------------------------------------------------------------------------------------------
*/

void cSceneObject::destroy()
{
	// remove ourselves from the quad tree (if any)
	detachFromQuadTree();

	cSceneNode::destroy();
}

void cSceneObject::prepareForUpdate()
{
	cSceneNode::prepareForUpdate();

	// clear the temporary flags
	m_objectFlags.clearFlags(FLAG(k_newLocalBounds)
							|FLAG(k_newWorldBounds));
}

void cSceneObject::update()
{
	cSceneNode::update();

	// do we need to update our local matrix?

	// derived classes update the local matrix
	// then call this base function to complete
	// the update
	recalcWorldBounds();
	refreshQuadTreeMembership();
}

void cSceneObject::recalcWorldBounds()
{
	// nodes only get their bounds updated
	// as nessesary
	if (TEST_BIT(m_objectFlags,k_newLocalBounds) 
		|| isWorldMatrixNew())
	{
		// transform our local rectangle 
		// by the current world matrix
		m_worldBounds = m_localBounds;

		// make sure we have some degree of thickness
		if (m_pQuadTree)
		{
			m_worldBounds.x1 = maximum(m_worldBounds.x1, m_worldBounds.x0+0.01f);
			m_worldBounds.y1 = maximum(m_worldBounds.y1, m_worldBounds.y0+0.01f);
			m_worldBounds.z1 = maximum(m_worldBounds.z1, m_worldBounds.z0+0.01f);
		}

		m_worldBounds.transform(&worldMatrix());

		// set the flag that our bounding box has changed
		m_objectFlags.setBit(k_newWorldBounds);
	}
}

void cSceneObject::refreshQuadTreeMembership()
{
	if (m_pQuadTree 
		&& TEST_BIT(m_objectFlags,k_newWorldBounds))
	{
		m_pQuadTree->addOrUpdateSceneObject(this);
	}
}

void cSceneObject::attachToQuadTree(cQuadTree* parentTree)
{
	detachFromQuadTree();
	m_pQuadTree = parentTree;
		m_pQuadTree->addOrUpdateSceneObject(this);
}

void cSceneObject::detachFromQuadTree()
{
	if (m_pQuadTreeNode)
	{
		m_pQuadTreeNode->removeMember(this);
		m_pQuadTreeNode=0;
	}
	m_pQuadTree=0;
	m_pForwardTreeLink=0;
	m_pRearTreeLink=0;
}


// Seach Functions

void cSceneObject::attachToSearchResult(cSceneObject* pRearLink, cSceneObject* pForwardLink)
{
	m_pForwardSearchLink = pForwardLink;
	m_pRearSearchLink = pRearLink;

	if (m_pForwardSearchLink)
	{
		m_pForwardSearchLink->setRearSearchLink(this);
	}
	if (m_pRearSearchLink)
	{
		m_pRearSearchLink->setForwardSearchLink(this);
	}

}

void cSceneObject::detachFromSearchResult()
{
	if (m_pForwardSearchLink)
	{
		m_pForwardSearchLink->setRearSearchLink(m_pRearSearchLink);
	}
	if (m_pRearSearchLink)
	{
		m_pRearSearchLink->setForwardSearchLink(m_pForwardSearchLink);
	}
	m_pForwardSearchLink = 0;
	m_pRearSearchLink = 0;
}

void cSceneObject::clearSearchResults()
{
	m_pRearSearchLink=0;
	m_pForwardSearchLink=0;
}

void cSceneObject::prepareForRender()
{
	// default implementation does nothing
}

void cSceneObject::render()
{
	cSceneNode::render();
}

void cSceneObject::renderCallback(cRenderEntry* entry, u32Flags activationFlags)
{
	// if we queued ourselved for rendering with the 
	// display manager, we will get this function
	// called when it is our turn to render

	// activationFlags contains a set of bit flags 
	// held in the eActivationFlagBits enum (render_queue.h)
	// which tell us what resources we need to activate 
	// in order to render ourselves.
}

//****************************************************************************************
// end of file      ( Math.cpp )

