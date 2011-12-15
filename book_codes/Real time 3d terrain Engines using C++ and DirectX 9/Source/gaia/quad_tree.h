#ifndef QUADTREE_H
#define QUADTREE_H
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
#include "quad_tree_node.h"


//	Name Space declaration
namespace gaia
{


class cQuadTree
{
public:

	// Data Types & Constants...
	enum eConstants
	{
		k_minimumTreeDepth = 1, 
		k_maximumTreeDepth = 9, //  must be a value between 1 and 9
	};


	// Public Data...

	// Creators...

    cQuadTree();
    ~cQuadTree();

	void create(const cRect3d& worldExtents, int depth);
	void destroy();

	// Operators...

	// Mutators...
	cSceneObject* buildSearchResults(
		const cRect3d& worldRect, 
		const cFrustum* pOptionalFrustum=0);
	u32Flags addOrUpdateSceneObject(cSceneObject* newNode);

	// Accessors...
	bool isReady()const;

private:

	// Private Data...
	cQuadTreeNode* m_levelNodes[k_maximumTreeDepth];
	cVector3 m_worldExtents;
	cVector3 m_worldScale;
	cVector3 m_worldOffset;
	int m_depth;
	uint32 m_memorySize;

	// Private Functions...
	void findTreeNodeInfo(const cQuadTreeRect& worldByteRect, int& level, int& levelX, int& levelY);
	cQuadTreeNode* findTreeNode(const cQuadTreeRect& worldByteRect);
	cQuadTreeNode* getNodeFromLevelXY(int level, int x, int y);
	void buildByteRect(const cRect3d& worldRect, cQuadTreeRect& worldByteRect);
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cQuadTree
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cQuadTree::cQuadTree()
:m_depth(0)
,m_memorySize(0)
{
	memset(m_levelNodes,0,sizeof(m_levelNodes));
}


/*	~cQuadTree
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cQuadTree::~cQuadTree()
{
	destroy();
}

//. Accessors ...................................................
inline bool cQuadTree::isReady()const
{
	return m_depth && m_levelNodes != 0;
}

inline cQuadTreeNode* cQuadTree::getNodeFromLevelXY(int level, int x, int y)
{
	debug_assert(isReady(), "the quad tree has not been created");

	if (level>=0 && level<m_depth)
	{
		return &m_levelNodes[level][(y<<level)+x];
	}
	return 0;
}

//- End of cQuadTree --------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cQuadTree.h )

//----------------------------------------------------------
//$Log: $