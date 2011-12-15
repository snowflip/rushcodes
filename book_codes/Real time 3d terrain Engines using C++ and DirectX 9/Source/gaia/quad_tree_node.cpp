#define QUADTREENODE_CPP
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

#include "quad_tree_node.h"
#include "scene_object.h"

using namespace gaia;

void cQuadTreeNode::setup(cQuadTreeNode* parent, cQuadTreeNode* child0, cQuadTreeNode* child1, cQuadTreeNode* child2, cQuadTreeNode* child3)
{
	debug_assert(m_pParentNode == 0, "Parent Node Already Set");
	m_pParentNode = parent;
	m_pChildNode[0] = child0;
	m_pChildNode[1] = child1;
	m_pChildNode[2] = child2;
	m_pChildNode[3] = child3;
}

uint32 cQuadTreeNode::addOrUpdateMember(cSceneObject* member, const cQuadTreeRect& rect)
{
	u32Flags zMask = ZMASK(rect.z0, rect.z1);

	// is this node not already a member?
	if (member->quadTreeNode() != this)
	{
		// remove the member from it's previous quad tree node (if any)
		if (member->quadTreeNode())
		{
			member->quadTreeNode()->removeMember(member);
		}

		// account for the new addition
		if (!m_pFirstMember)
		{
			m_pFirstMember = member;
		}
		else 
		{
			// prepend this member to our list
			member->setRearTreeLink(0);
			member->setForwardTreeLink(m_pFirstMember);
			m_pFirstMember->setRearTreeLink(member);
			m_pFirstMember = member;
		}

		// update our zMask
		m_zMask.setFlags(zMask);
		m_zLocalMask.setFlags(zMask);

		// notify our parent of the addition
		if (m_pParentNode)
		{
			m_pParentNode->descendantMemberAdded(m_zMask);
		}
	}
	else
	{
		// refresh our zMask for all members
		rebuildLocalZMask();
	}

	member->setQuadTreeData(this, zMask);

	// update the member's zMask
	return zMask;
}

void cQuadTreeNode::removeMember(cSceneObject* member)
{
	// make sure this is one of ours
	debug_assert(member->quadTreeNode() == this, "error removing quad tree member");

	// remove this member from it's chain
	if (member->forwardTreeLink())
	{
		member->forwardTreeLink()->setRearTreeLink(member->rearTreeLink());
	}
	if (member->rearTreeLink())
	{
		member->rearTreeLink()->setForwardTreeLink(member->forwardTreeLink());
	}

	// if this was our first member, advance our pointer to the next member
	if (m_pFirstMember == member)
	{
		m_pFirstMember = member->forwardTreeLink();
	}

	// clear the former members links
	member->setRearTreeLink(0);
	member->setForwardTreeLink(0);

	// rebuild our z mask
	rebuildLocalZMask();

	// notify our parent
	if (m_pParentNode)
	{
		m_pParentNode->descendantMemberRemoved();
	}
}

void cQuadTreeNode::rebuildLocalZMask()
{
	m_zLocalMask.clear();

	// add add any local members
	cSceneObject* node = m_pFirstMember;
	while(node)
	{
		m_zLocalMask.setFlags(node->zMask());
		node = node->forwardTreeLink();
	}

	// the combined zMask must now be updated
	rebuildZMask();
}

void cQuadTreeNode::rebuildZMask()
{
	// reset our overall z mask to the mask 
	// defined by our local members only
	m_zMask = m_zLocalMask;

	// sum up the masks of our children
	for (int i=0;i<4;++i)
	{
		if (m_pChildNode[i])
		{
			m_zMask.setFlags(m_pChildNode[i]->zMask());
		}
	}
}

void cQuadTreeNode::descendantMemberAdded(u32Flags zMask)
{
	// update our zMask
	m_zMask.setFlags(zMask);

	// notify our parent of the addition
	if (m_pParentNode)
	{
		m_pParentNode->descendantMemberAdded(zMask);
	}
}

void cQuadTreeNode::descendantMemberRemoved()
{
	// update our zMask
	rebuildZMask();

	// notify our parent of the removal
	if (m_pParentNode)
	{
		m_pParentNode->descendantMemberRemoved();
	}
}



void cQuadTreeNode::testLocalMembersForSearchResults(
	cSceneObject** pResultList, 
	cSceneObject** pResultListTail, 
	u32Flags zMask, 
	float zMin, 
	float zMax,
	const cFrustum* pOptionalFrustum)
{
	// calling this function assumes that the 
	// 2D search rectangle contains this node completely,
	// so all we need to test against is the 
	// z range specified and the optional frustum
	cSceneObject* object;
	if (m_zLocalMask.testAny(zMask))
	{
		for(object = m_pFirstMember; object; object = object->forwardTreeLink())
		{
			if (object->zMask().testAny(zMask))
			{
				if (!pOptionalFrustum
					|| pOptionalFrustum->testRect(object->worldBounds()))
				{
					if (*pResultListTail)
					{
						object->attachToSearchResult(*pResultListTail, 0);
						*pResultListTail = object;
					}
					else
					{
						object->clearSearchResults();
						*pResultListTail = object;
						*pResultList = object;
					}
				}
			}
		}
	}
}

void cQuadTreeNode::testLocalMembersForSearchResults(
	cSceneObject** pResultList, 
	cSceneObject** pResultListTail, 
	u32Flags zMask, 
	const cRect3d& trueRect,
	const cFrustum* pOptionalFrustum)
{
	// calling this function assumes that 
	// the 2D search rectangle intersects this node,
	// so we need to test against is the zMask 
	// bit patterns as well as the search 
	// area for our local members
	cSceneObject* object;
	cRect3d result;

	if (m_zLocalMask.testAny(zMask))
	{
		for(object = m_pFirstMember; object; object = object->forwardTreeLink())
		{
			if (object->zMask().testAny(zMask))
			{
				if (intersectRect3d(trueRect, object->worldBounds(), result))
				{
					if (!pOptionalFrustum
						|| pOptionalFrustum->testRect(object->worldBounds()))
					{
						if (*pResultListTail)
						{
							object->attachToSearchResult(*pResultListTail, 0);
							*pResultListTail = object;
						}
						else
						{
							object->clearSearchResults();
							*pResultListTail = object;
							*pResultList = object;
						}
					}
				}
			}
		}
	}
}



//***************************************************************
// end of file      ( cQuadTreeNode.cpp )

//----------------------------------------------------------
//$Log: $