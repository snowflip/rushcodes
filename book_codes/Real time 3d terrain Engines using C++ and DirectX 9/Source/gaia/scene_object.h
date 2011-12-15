#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H
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
#include "scene_node.h"


//	Name Space declaration
namespace gaia
{

class cSceneNode;

class cSceneObject : public cSceneNode
{
public:

	// Data Types & Constants...
	enum eObjectFlagBits
	{
		k_newLocalBounds=0,
		k_newWorldBounds,
	};

	// Creators...
	cSceneObject();
	virtual ~cSceneObject();

	// Mutators...
	virtual bool create();
	virtual void destroy();
	virtual void prepareForUpdate();
	virtual void update();
	virtual void prepareForRender();
	virtual void render();
	virtual void renderCallback(cRenderEntry* entry, u32Flags activationFlags);

	cRect3d* accessLocalBounds();
	virtual void recalcWorldBounds();

	// QuadTree Functions
	void attachToQuadTree(cQuadTree* parentTree);
	void detachFromQuadTree();

	// Seach Functions
	void attachToSearchResult(cSceneObject* pRearLink, cSceneObject* pForwardLink);
	void detachFromSearchResult();
	void clearSearchResults();
	cSceneObject*	nextSearchLink() {return m_pForwardSearchLink;}

	// Accessors...
	bool isWorldBoundsNew()const;

	u32Flags	objectFlags()const;

	cQuadTreeNode* quadTreeNode()const;
	u32Flags zMask()const;
	cSceneObject* forwardTreeLink()const;
	cSceneObject* rearTreeLink()const;

	const cRect3d& localBounds()const;
	const cRect3d& worldBounds()const;

	//
	// temporary functions
	//
	void setWorldBounds(const cRect3d& box)
	{
		m_worldBounds = box;
	}


private:

	// bounding box information
	u32Flags m_objectFlags;
	cRect3d m_localBounds;
	cRect3d m_worldBounds;

	// world QuadTree membership information
	cQuadTree*		m_pQuadTree;
	cQuadTreeNode*  m_pQuadTreeNode;
	u32Flags		m_quadTreeZMask;
	cSceneObject*	m_pForwardTreeLink;
	cSceneObject*	m_pRearTreeLink;

	// QuadTree search result links
	cSceneObject*	m_pForwardSearchLink;
	cSceneObject*	m_pRearSearchLink;

	// private functions
	void setForwardSearchLink(cSceneObject* pForwardLink);
	void setRearSearchLink(cSceneObject* pForwardLink);
	virtual void refreshQuadTreeMembership();

	friend cQuadTreeNode;
	void setQuadTreeData(cQuadTreeNode* parentNode, u32Flags zMask);
	void setForwardTreeLink(cSceneObject* link);
	void setRearTreeLink(cSceneObject* link);
};

//- Inline Functions ---------------------------------------------------------------------

/*	cSceneObject
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cSceneObject::cSceneObject()
//:m_pParentNode(0)
:cSceneNode()
,m_objectFlags(0)
,m_localBounds(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
,m_worldBounds(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
,m_pQuadTreeNode(0)
,m_pQuadTree(0)
,m_quadTreeZMask(0)
,m_pForwardTreeLink(0)
,m_pRearTreeLink(0)
,m_pForwardSearchLink(0)
,m_pRearSearchLink(0)
{
}

/*	~cSceneObject
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cSceneObject::~cSceneObject()
{
	destroy();
}



inline void cSceneObject::setForwardSearchLink(cSceneObject* pForwardLink)
{
	m_pForwardSearchLink = pForwardLink;
}

inline void cSceneObject::setRearSearchLink(cSceneObject* pRearLink)
{
	m_pRearSearchLink = pRearLink;
}

//. Accessors ............................................................................

inline u32Flags	cSceneObject::objectFlags()const
{
	return (m_objectFlags);
}

inline cRect3d* cSceneObject::accessLocalBounds()
{
	m_objectFlags.setBit(k_newLocalBounds);
	return &m_localBounds;
}

inline cQuadTreeNode* cSceneObject::quadTreeNode()const
{
	return m_pQuadTreeNode;
}

inline u32Flags cSceneObject::zMask()const
{
	return m_quadTreeZMask;
}

inline cSceneObject* cSceneObject::forwardTreeLink()const
{
	return m_pForwardTreeLink;
}

inline cSceneObject* cSceneObject::rearTreeLink()const
{
	return m_pRearTreeLink;
}

inline void cSceneObject::setQuadTreeData(cQuadTreeNode* parentNode, u32Flags zMask)
{
	m_pQuadTreeNode = parentNode;
	m_quadTreeZMask = zMask;
}

inline void cSceneObject::setForwardTreeLink(cSceneObject* link)
{
	m_pForwardTreeLink = link;
}

inline void cSceneObject::setRearTreeLink(cSceneObject* link)
{
	m_pRearTreeLink = link;
}

inline const cRect3d& cSceneObject::localBounds()const
{
	return m_localBounds;
}

inline const cRect3d& cSceneObject::worldBounds()const
{
	return m_worldBounds;
}

inline bool cSceneObject::isWorldBoundsNew()const
{
	return TEST_BIT(m_objectFlags,k_newWorldBounds);
}

//- End of cSceneObject -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSceneObject.h )

//----------------------------------------------------------
//$Log: $