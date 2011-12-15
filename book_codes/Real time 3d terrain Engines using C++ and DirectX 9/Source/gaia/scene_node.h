#ifndef CSCENENODE_H
#define CSCENENODE_H
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
#include "orientation.h"
#include "render_queue.h"


//	Name Space declaration
namespace gaia
{

class cSubNodeGroup;
class cCamera;

class cSceneNode : public cReferenceCounter
{
public:

	// Data Types & Constants...
	enum eUpdateDataFlagBits
	{
		k_setForDestruction = 0,
		k_newLocalMatrix,
		k_newWorldMatrix,
		k_rebuildInverseWorldMatrix,
		k_rebuildWorldVectors,
		k_ignoreOrientation,
	};

	enum eInheritance
	{
		k_inheritPositionOnly = 0,
		k_inheritRotScaleOnly,
		k_inheritAll
	};

	// Creators...
	cSceneNode();
	virtual ~cSceneNode();

	// Virtual Mutators...
	virtual bool create();
	virtual void destroy();
	virtual void prepareForUpdate();
	virtual void prepareForRender();
	virtual void update();
	virtual void render();
	virtual void renderCallback(cRenderEntry* entry, u32Flags activationFlags);

	virtual void recursiveUpdate();
	virtual void recursiveRender();

	// alternate creation method
	virtual bool createFromResource(cPoolHandle ModelResource);

	// factory functions
	virtual cSceneNode* createSubNode();
	virtual cSceneNode* createSubNode(cModelResource* pModelResource, int frameIndex);
	virtual void destroySubNode(cSceneNode*& pSubNode);

	// Family Functions
	void	attachToParent(cSceneNode* pParent);
	void	detachFromParent();
	void	promoteChildren();
	void	relocateChildren(cSceneNode* pParent);

	void setInheritance(eInheritance setting);

	// update functions
	void setUpdatedLocalMatrixFlag();
	cSIMDMatrix* accessLocalMatrix();

	

	virtual void recalcWorldMatrix();

	// Accessors...
	bool isASibling(const cSceneNode* pSibling)const;
	bool isAChild(const cSceneNode* pChild)const;
	bool isInFamily(const cSceneNode* pNode, bool SearchEntireTree)const;

	cSceneNode*		parent()const;
	cSceneNode*		sibling()const;
	cSceneNode*		child()const;

	bool	hasParent()const;
	bool	hasChild()const;
	bool	hasSibling()const;
	
	int		childCount()const;
	int		siblingCount()const;

	const cSIMDMatrix&	worldMatrix()const;
	const cSIMDMatrix&	localMatrix()const;
	const cSIMDMatrix&	inverseWorldMatrix();
	bool isWorldMatrixNew()const;

	u32Flags updateFlags()const;
	cSubNodeGroup*  subNodeGroupData()const;
	cSceneNode*  subNodeGroupOwner()const;

	cOrientation& orientation() {return m_orientation;}

	const cVector3& worldPosition();
	const cVector3& worldRight();
	const cVector3& worldUp();
	const cVector3& worldForward();

	void setSubNodeGroupOwner(cSceneNode* owner);

private:

	// Data...
	cSIMDMatrix	m_localMatrix;
	cSIMDMatrix	m_worldMatrix;
	cSIMDMatrix	m_inverseWorldMatrix;
	u32Flags	m_updateFlags;
	cSceneNode*	m_pParent;			// pointer to the parent
	cSceneNode*	m_pSibling;			// pointer to the sibling of this node
	cSceneNode*	m_pChild;			// pointer to the child

	cSubNodeGroup*	 m_pSubNodeGroupData;
	cSceneNode*		 m_pSubNodeGroupOwner;

	cOrientation	m_orientation;
	eInheritance	m_Inheritance;

	cVector3		m_worldPosition;
	cVector3		m_worldRight;
	cVector3		m_worldUp;
	cVector3		m_worldForward;

	// private functions
	void setSibling(cSceneNode* pNode);
	void setChild(cSceneNode* pNode);
	void setForwardSearchLink(cSceneNode* pForwardLink);
	void setRearSearchLink(cSceneNode* pForwardLink);

	void addSibling(cSceneNode* pSibling);
	void addChild(cSceneNode* pChild);
	void removeChild(cSceneNode* pChild);
	void removeAllChildren();

	void releaseGroupData();
	void updateWorldVectors();

};

//- Inline Functions ---------------------------------------------------------------------




//. Accessors ............................................................................


/*	parent
------------------------------------------------------------------------------------------
	Returns the current parent pointer of this object.
------------------------------------------------------------------------------------------
*/

inline cSceneNode* cSceneNode::parent() const
{
	return(m_pParent);
}

/*	sibling
------------------------------------------------------------------------------------------
	Returns the current sibling pointer of this object.
------------------------------------------------------------------------------------------
*/

inline cSceneNode* cSceneNode::sibling() const
{
	return(m_pSibling);
}

/*	child
------------------------------------------------------------------------------------------
	Returns the current child pointer of this object.
------------------------------------------------------------------------------------------
*/

inline cSceneNode* cSceneNode::child() const
{
	return(m_pChild);
}


inline bool cSceneNode::hasParent() const
{
	return(m_pParent == 0 ? false : true);
}


inline bool cSceneNode::hasChild() const
{
	return(m_pChild == 0 ? false : true);
}


inline bool cSceneNode::hasSibling() const
{
	return(m_pSibling == 0 ? false : true);
}

inline const cSIMDMatrix& cSceneNode::worldMatrix()const
{
	return (m_worldMatrix);
}

inline const cSIMDMatrix& cSceneNode::inverseWorldMatrix()
{
	// if the inverse matrix is out of date, compute it now
	if (TEST_BIT(m_updateFlags,k_rebuildInverseWorldMatrix))
	{
		m_inverseWorldMatrix.invert(m_worldMatrix);
		m_updateFlags.clearBit(k_rebuildInverseWorldMatrix);
	}
	return (m_inverseWorldMatrix);
}

inline void cSceneNode::setUpdatedLocalMatrixFlag()
{
	m_updateFlags.setBit(k_newLocalMatrix);
}

inline cSIMDMatrix* cSceneNode::accessLocalMatrix()
{
	setUpdatedLocalMatrixFlag();
	m_updateFlags.setBit(k_ignoreOrientation);

	return (&m_localMatrix);
}

inline const cSIMDMatrix& cSceneNode::localMatrix()const
{
	return (m_localMatrix);
}

inline u32Flags	cSceneNode::updateFlags()const
{
	return (m_updateFlags);
}

inline bool cSceneNode::isWorldMatrixNew()const
{
	return TEST_BIT(m_updateFlags,k_newWorldMatrix);
}

inline cSubNodeGroup* cSceneNode::subNodeGroupData()const
{
	return m_pSubNodeGroupData;
}

inline cSceneNode* cSceneNode::subNodeGroupOwner()const
{
	return m_pSubNodeGroupOwner;
}

inline void cSceneNode::setSubNodeGroupOwner(cSceneNode* owner)
{
	m_pSubNodeGroupOwner = owner;
}

//- End of cSceneNode -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSceneNode.h )

//----------------------------------------------------------
//$Log: $