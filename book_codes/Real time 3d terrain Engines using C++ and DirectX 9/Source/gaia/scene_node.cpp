#define SCENENODE_CPP
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

#include "scene_node.h"
#include "scene_object.h"
#include "scene_model.h"
#include "subnode_group.h"

using namespace gaia;

/*	cSceneNode
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

cSceneNode::cSceneNode()
:m_updateFlags(FLAG(k_newLocalMatrix))
,m_pParent(0)
,m_pSibling(0)
,m_pChild(0)
,m_pSubNodeGroupData(0)
,m_pSubNodeGroupOwner(0)
,m_Inheritance(k_inheritAll)
{
}

/*	~cSceneNode
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
cSceneNode::~cSceneNode()
{
	destroy();
}


bool cSceneNode::create()
{
	m_localMatrix.SetIdentity();
	m_worldMatrix.SetIdentity();

	return true;
}

bool cSceneNode::createFromResource(cPoolHandle ModelResource)
{
	// dump any current animation data
	releaseGroupData();

	m_pSubNodeGroupData = new cSubNodeGroup;
	if (m_pSubNodeGroupData->create(this, ModelResource))
	{
		return true;
	}
	else
	{
		safe_delete(m_pSubNodeGroupData);
	}

	return false;
}

void cSceneNode::releaseGroupData()
{
	if (m_pSubNodeGroupData)
	{
		m_pSubNodeGroupData->destroy();
		safe_delete(m_pSubNodeGroupData);
	}
}

/*	Destroy
------------------------------------------------------------------------------------------
	
	Destroys the object. The object is removed from it's parent (if any) and all Children
	Objects are orphaned (parent set to NULL).
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::destroy()
{
	// release any group information
	releaseGroupData();

	// destroy anything attached to this node
//	destroySceneObject();

	// promote any children up to our parent
	promoteChildren();

	// now remove ourselves from our parent
	detachFromParent();

	// we should now be removed from the tree, and have no dependants
	debug_assert(referenceCount()==0, "Attempting to delete a scene node which is still in use");
	debug_assert(m_pSibling==0, "Failure Destroying Node");
	debug_assert(m_pChild==0, "Failure Destroying Node");

	// clear link information
	m_pSubNodeGroupOwner = 0;
}

/*	SetFirstSibling
------------------------------------------------------------------------------------------
	
	Sets the internal pointer to the First sibling object
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::setSibling(cSceneNode* pNode)
{
	m_pSibling=pNode;
}

/*	SetFirstChild
------------------------------------------------------------------------------------------
	
	Sets the internal pointer to the First child object
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::setChild(cSceneNode* pNode)
{
	m_pChild=pNode;
}

/*	addSibling
------------------------------------------------------------------------------------------
	
	Adds the provided ModelSpace object to the descendant list of this object. The provided
	ModelSpace object is removed from any parent it may already belong to.
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::addSibling(cSceneNode* pSibling)
{
	if (pSibling)
	{
		// replace objects current sibling pointer with this new one
		pSibling->setSibling(m_pSibling);
		setSibling(pSibling);
	}
}



/*	addChild
------------------------------------------------------------------------------------------
	
	Adds the provided ModelSpace object to the descendant list of this object. The provided
	ModelSpace object is removed from any parent it may already belong to.
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::addChild(cSceneNode* pChild)
{
	if (pChild)
	{
		// Replace the new child's sibling pointer with our old first child.
		pChild->setSibling(m_pChild);

		// the new child becomes our first child pointer.
		m_pChild = pChild;
	}
}

/*	removeChild
------------------------------------------------------------------------------------------
	
	Removes a specified child object from this parent object. If the child is not the 
	FirstChild of this object, all of the Children are searched to find the object to remove.
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::removeChild(cSceneNode* pChild)
{
	if (m_pChild && pChild)
	{
		if (m_pChild == pChild)
		{
			m_pChild = pChild->sibling();
			pChild->setSibling(0);
		}
		else
		{
			cSceneNode* pTempNode=m_pChild;

			// keep searching until we find the node who's sibling is our target
			// or we reach the end of the sibling chain
			while (pTempNode && (pTempNode->sibling() != pChild))
			{
				pTempNode = pTempNode->sibling();
			}

			// if we found the proper item, set it's FirstSibling to be the FirstSibling of the child
			// we are removing
			if (pTempNode)
			{
				pTempNode->setSibling(pChild->sibling());
				pChild->setSibling(0);
			}
		}
	}
}

/*	removeAllChildren
------------------------------------------------------------------------------------------
	
	Removes all Children from this parent object
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::removeAllChildren()
{
	// keep removing children until end of chain is reached
	while (m_pChild != 0)
	{
		cSceneNode* NextSibling = m_pChild->sibling();
		m_pChild->detachFromParent();
		m_pChild = NextSibling;
	}
}

/*	attachToParent
------------------------------------------------------------------------------------------
	
	Attaches this object ot a new parent. Same as calling the parent's addChild() routine.
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::attachToParent(cSceneNode* pParent)
{
	if (pParent != m_pParent)
	{
		detachFromParent();

		if (pParent)
		{
			m_pParent = pParent;
			m_pParent->addChild((cSceneNode*)this);
			m_pParent->AddRef();

			// adjust my local matrix to be relative to this new parent
			cSIMDMatrix invertedParentMatrix(m_pParent->m_worldMatrix);
			invertedParentMatrix.invert();

			MatrixMultiply(&m_localMatrix, &m_localMatrix, &invertedParentMatrix);
		}
	}
}

void cSceneNode::setInheritance(eInheritance setting)
{
	m_Inheritance = setting;
}


/*	detachFromParent
------------------------------------------------------------------------------------------
	
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::detachFromParent()
{
	// tell our current parent to release us
	if (m_pParent)
	{
		m_pParent->removeChild(this);
		//m_pParent->releaseReference();
		safe_release(m_pParent);
		m_pParent=0;

		// my world matrix is now my local matrix
		m_localMatrix = m_worldMatrix;

		// and the world matrix is the identity
		m_worldMatrix.SetIdentity(); 
	}
}


/*	promoteChildren
------------------------------------------------------------------------------------------
	
	Attaches this object's children to it's parent, promoting them up the tree.
	
------------------------------------------------------------------------------------------
*/

void cSceneNode::promoteChildren()
{
	// Do I have any children to promote?
	while (m_pChild != 0)
	{
		cSceneNode* NextSibling = m_pChild->sibling();
		m_pChild->attachToParent(m_pParent);
		m_pChild = NextSibling;
	}
}

void cSceneNode::relocateChildren(cSceneNode* pParent)
{
	if (pParent != this)
	{
		// Do I have any children to relocate?
		while (m_pChild != 0)
		{
			cSceneNode* NextSibling = m_pChild->sibling();
			m_pChild->attachToParent(pParent);
			m_pChild = NextSibling;
		}
	}
}

/*	isASibling
------------------------------------------------------------------------------------------
	
	Checks to see if the provided item is a sibling of this object
	
------------------------------------------------------------------------------------------
*/
bool cSceneNode::isASibling(const cSceneNode* pSibling) const
{
	if (!pSibling) return (false);

	// if the sibling we are looking for is me, or my FirstSibling, return true
	if (this == pSibling || m_pSibling == pSibling)
	{
		return (true);
	}
	
	// if we have a sibling, continue searching
	if (m_pSibling)
	{
		return (m_pSibling->isASibling(pSibling));
	}

	// it's not us, and we have no sibling to check. This is not a sibling of ours.
	return (false);
}

/*	isAChild
------------------------------------------------------------------------------------------
	
	Checks to see if the provided item is a child of this object. (one branch depth only)
	
------------------------------------------------------------------------------------------
*/

bool cSceneNode::isAChild(const cSceneNode* pChild) const
{
	if (!pChild) return (false);

	// if the sibling we are looking for is my FirstChild return true
	if (m_pChild == pChild )
	{
		return (true);
	}
	
	// if we have a child, continue searching
	if (m_pChild)
	{
		return (m_pChild->isASibling(pChild));
	}

	// it's not us, and we have no child to check. This is not a sibling of ours.
	return (false);
}

/*	isInFamily
------------------------------------------------------------------------------------------
	
	Checks to see if the provided item is a child or sibling of this object. If SearchEntireTree
	is TRUE, the check is done recursivly through all siblings and children. SearchEntireTree
	is FALSE by default.
	
------------------------------------------------------------------------------------------
*/

bool cSceneNode::isInFamily(const cSceneNode* pNode, bool SearchEntireTree) const
{
	if (!pNode) return (false);

	// if the model we are looking for is me or my immediate family, return true
	if (this == pNode || m_pChild == pNode || m_pSibling == pNode)
	{
		return (true);
	}

	// if not set to seach entire tree, just check my siblings and kids
	if (!SearchEntireTree)
	{
		if (isASibling(pNode))
		{
			return (true);
		}

		if (m_pChild && m_pChild->isASibling(pNode))
		{
			return (true);
		}
	}
	else // seach entire Tree!!!
	{
		if (m_pSibling && m_pSibling->isInFamily(pNode, SearchEntireTree))
		{
			return (true);
		}

		if (m_pChild && m_pChild->isInFamily(pNode, SearchEntireTree))
		{
			return (true);
		}
	}

	return (false);
}

/*	siblingCount
------------------------------------------------------------------------------------------
	Returns the current number of siblings of this object.
------------------------------------------------------------------------------------------
*/

int cSceneNode::siblingCount() const
{
	int		count = 0;
	
	if(m_pParent)
	{
		cSceneNode* pNextSibling = m_pParent->child();
		if(pNextSibling)
		{
			while(pNextSibling)
			{
				pNextSibling = pNextSibling->sibling();
				++count;
			}
		}
	}
	return(count);
}

/*	childCount
------------------------------------------------------------------------------------------
	Returns the current number of children of this object.
------------------------------------------------------------------------------------------
*/

int cSceneNode::childCount() const
{
	int		count = 0;
	
	cSceneNode* pNextChild = child();
	if(pNextChild)
	{
		++count;
		while(pNextChild)
		{
			pNextChild = pNextChild->sibling();
			++count;
		}
	}
	return(count);
}


void cSceneNode::update()
{
	if (!TEST_BIT(m_updateFlags,k_ignoreOrientation))
	{
		// has the orientation data changed?
		if (m_orientation.updateMatrix(m_localMatrix))
		{
			m_updateFlags.setBit(k_newLocalMatrix);
		}
	}

	// derived classes update the local matrix
	// then call this base function to complete
	// the update
	recalcWorldMatrix();

	// if there is a group attached, update it
	if (m_pSubNodeGroupData)
	{
		m_pSubNodeGroupData->update();
	}

	// if there is an object attached, update it
/*
	if (m_pSceneObject)
	{
		m_pSceneObject->update();
	}
*/
}

void cSceneNode::prepareForUpdate()
{
	// clear the temporary flags
	m_updateFlags.clearFlags(FLAG(k_newLocalMatrix)
							|FLAG(k_newWorldMatrix));

	// notify the object
/*
	if (m_pSceneObject)
	{
		m_pSceneObject->prepareForUpdate();
	}
*/
}

void cSceneNode::prepareForRender()
{
	// base class does nothing. Derived classes
	// should set their LOD levels and prepare
	// for a call to render
}

void cSceneNode::recursiveUpdate()
{
	// update myself
	update();

	// update my sibling
	if (sibling())
	{
		sibling()->recursiveUpdate();
	}

	// update my child
	if (child())
	{
		child()->recursiveUpdate();
	}

	// clear the flags from the previous update
	prepareForUpdate();
}

void cSceneNode::render()
{
	// by default, scene nodes do not render.
	// derived classes must provide 
	// any functionality needed.
}

void cSceneNode::renderCallback(cRenderEntry* entry, u32Flags activationFlags)
{
	// if we queued ourselved for rendering with the 
	// display manager, we will get this function
	// called when it is our turn to render

	// activationFlags contains a set of bit flags 
	// held in the eActivationFlagBits enum (render_queue.h)
	// which tell us what resources we need to activate 
	// in order to render ourselves.
}

// this function renders everything in the scene.
// it is only used by the model viewing tool
void cSceneNode::recursiveRender()
{
	// render myself
	prepareForRender();
	render();

	// render my sibling
	if (sibling())
	{
		sibling()->recursiveRender();
	}

	// render my child
	if (child())
	{
		child()->recursiveRender();
	}
}

void cSceneNode::recalcWorldMatrix()
{
	bool parentMoved = m_pParent && m_pParent->isWorldMatrixNew();
	bool weMoved = TEST_BIT(m_updateFlags,k_newLocalMatrix);

	if (weMoved || parentMoved)
	{
		// multiply our local matrix by our parent
		if (m_pParent)
		{
			if (m_Inheritance==k_inheritAll)
			{
				MatrixMultiply(
					&m_worldMatrix, 
					&m_localMatrix, 
					&m_pParent->worldMatrix());
			}
			else if (m_Inheritance==k_inheritPositionOnly)
			{
				const cSIMDMatrix& pMat =
					m_pParent->worldMatrix();

				cSIMDMatrix parentMatrix;
				parentMatrix.SetIdentity();

				parentMatrix._41 = pMat._41;
				parentMatrix._42 = pMat._42;
				parentMatrix._43 = pMat._43;

				MatrixMultiply(
					&m_worldMatrix, 
					&m_localMatrix, 
					&parentMatrix);
			}
			else if (m_Inheritance==k_inheritRotScaleOnly)
			{
				cSIMDMatrix parentMatrix(m_pParent->worldMatrix());
				parentMatrix._41 = 0.0f;
				parentMatrix._42 = 0.0f;
				parentMatrix._43 = 0.0f;

				MatrixMultiply(
					&m_worldMatrix, 
					&m_localMatrix, 
					&parentMatrix);
			}
		}
		else
		{
			m_worldMatrix = m_localMatrix;
		}

		// set the flag that our world matrix has changed
		m_updateFlags.setBit(k_newWorldMatrix);

		// and it's inverse & vectors are out of date
		m_updateFlags.setBit(k_rebuildInverseWorldMatrix);
		m_updateFlags.setBit(k_rebuildWorldVectors);
		
	}
}

cSceneNode* cSceneNode::createSubNode()
{
	cSceneNode* node = new cSceneNode;
	node->create();
	node->m_pSubNodeGroupOwner = this;
	return node;
}

cSceneNode* cSceneNode::createSubNode(cModelResource* pModelResource, int frameIndex)
{
	cSceneModel* object = new cSceneModel;
	cSceneNode* node = (cSceneNode*)object;

	node->create();
	object->setModelResource(pModelResource, frameIndex);
	node->m_pSubNodeGroupOwner = this;
	return node;
}

void cSceneNode::destroySubNode(cSceneNode*& pSubNode)
{
	pSubNode->destroy();
	pSubNode->m_pSubNodeGroupOwner=0;
	safe_delete(pSubNode);
}


void cSceneNode::updateWorldVectors()
{
	// we only do this when nessesary
	if (TEST_BIT(m_updateFlags,k_rebuildWorldVectors))
	{
		cVector4 row;

		m_worldMatrix.getRow(0, &row);
		if (row.w != 0.0f)
		{
			row.x /= row.w;
			row.y /= row.w;
			row.z /= row.w;
		}
		m_worldRight = row.vec3();
		m_worldRight.normalize();

		m_worldMatrix.getRow(1, &row);
		if (row.w != 0.0f)
		{
			row.x /= row.w;
			row.y /= row.w;
			row.z /= row.w;
		}
		m_worldUp = row.vec3();
		m_worldUp.normalize();

		m_worldMatrix.getRow(2, &row);
		if (row.w != 0.0f)
		{
			row.x /= row.w;
			row.y /= row.w;
			row.z /= row.w;
		}
		m_worldForward = row.vec3();
		m_worldForward.normalize();

		m_worldMatrix.getRow(3, &row);
		if (row.w != 0.0f)
		{
			row.x /= row.w;
			row.y /= row.w;
			row.z /= row.w;
		}
		m_worldPosition = row.vec3();
		
		m_updateFlags.clearBit(k_rebuildWorldVectors);
	}
}

const cVector3& cSceneNode::worldPosition()
{
	updateWorldVectors();
	return m_worldPosition;
}

const cVector3& cSceneNode::worldRight()
{
	updateWorldVectors();
	return m_worldRight;
}

const cVector3& cSceneNode::worldUp()
{
	updateWorldVectors();
	return m_worldUp;
}

const cVector3& cSceneNode::worldForward()
{
	updateWorldVectors();
	return m_worldForward;
}

//****************************************************************************************
// end of file      ( Math.cpp )

