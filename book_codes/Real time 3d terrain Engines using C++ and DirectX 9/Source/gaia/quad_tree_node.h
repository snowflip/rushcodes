#ifndef QUADTREENODE_H
#define QUADTREENODE_H
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


//	Name Space declaration
namespace gaia
{

class cSceneObject;
class cQuadTree;

/*	cQuadTreeNode
-----------------------------------------------------------------
    
    cQuadTreeNode represents a single node in our quad tree. Even
	Though the QuadTree is a 2D spatial organization, each member 
	of the node is given a 32bit number (called a zMask) 
	representing the areas of Z that they occupy in the world. 
	The world Z extent is divided into 32 areas, each one represented 
	by a bit in the zMask. Therefore, when testing with a 3D rectange,
	the node can set a zMask bit pattern representing the Z areas
	being searched and cull it's members by And'ing the search
	zMask with the members zMask to see if there is any overlap 
    
-----------------------------------------------------------------
*/

inline uint32 ZMASK(uint8 zmin, uint8 zmax)
{
	uint32 high = (1<<zmax);
	uint32 low = (1<<zmin);
	uint32 setMask = high-1;
	uint32 clearMask = low-1;

	uint32 result = setMask;
	if (zmin)
	{
		result &= ~clearMask;
	}
	result |= high;
	result |= low;

	return result;
}

class cQuadTreeRect
{
public:
	int x0;
	int x1;
	int y0; 
	int y1;
	int z0; 
	int z1;

	cQuadTreeRect(){};
	~cQuadTreeRect(){};

cQuadTreeRect(const cQuadTreeRect& src)
		:x0(src.x0)
		,x1(src.x1)
		,y0(src.y0)
		,y1(src.y1)
		,z0(src.z0)
		,z1(src.z1)
	{};


	cQuadTreeRect(	int _x0,
					int _x1,
					int _y0, 
					int _y1,
					int _z0, 
					int _z1)
		:x0(_x0)
		,x1(_x1)
		,y0(_y0)
		,y1(_y1)
		,z0(_z0)
		,z1(_z1)
	{};

	void convert(const cRect3d& worldRect, const cVector3& offset, const cVector3& scale)
	{
		cRect3d convertedRect(worldRect);

		// reposition and scale world coordinates to quad tree coordinates
		convertedRect += offset;
		convertedRect *= scale;

		// reduce by a tiny amount to handle tiled data
		convertedRect.x1 -= 0.01f;
		convertedRect.y1 -= 0.01f;
		convertedRect.z1 -= 0.01f;

		convertedRect.x1 = maximum(convertedRect.x1, convertedRect.x0);
		convertedRect.y1 = maximum(convertedRect.y1, convertedRect.y0);
		convertedRect.z1 = maximum(convertedRect.z1, convertedRect.z0);

		// convert to integer values, taking the floor of each real
		x0 = realToInt32_floor(convertedRect.x0);
		x1 = realToInt32_floor(convertedRect.x1);
		y0 = realToInt32_floor(convertedRect.y0);
		y1 = realToInt32_floor(convertedRect.y1);
		z0 = realToInt32_floor(convertedRect.z0);
		z1 = realToInt32_floor(convertedRect.z1);

		// we must be positive
		x0 = clamp(x0, 0, 254);
		y0 = clamp(y0, 0, 254);
		z0 = clamp(z0, 0, 30);

		// we must be at least one unit large
		x1 = clamp(x1, x0+1, 255);
		y1 = clamp(y1, y0+1, 255);
		z1 = clamp(z1, z0+1, 31);

	};
};

class cQuadTreeNode
{
public:


    cQuadTreeNode();
    ~cQuadTreeNode();

	uint32 addOrUpdateMember(cSceneObject* member, const cQuadTreeRect& rect);
	void removeMember(cSceneObject* member);

	void testLocalMembersForSearchResults(
		cSceneObject** pResultList, 
		cSceneObject** pResultListTail, 
		u32Flags zMask, 
		float zMin, 
		float zMax,
		const cFrustum* pOptionalFrustum=0);

	void testLocalMembersForSearchResults(
		cSceneObject** pResultList, 
		cSceneObject** pResultListTail, 
		u32Flags zMask, 
		const cRect3d& trueRect,
		const cFrustum* pOptionalFrustum=0);

	bool empty()const;
	u32Flags zMask()const;
	u32Flags zLocalMask()const;

private:

	// Private Data...
	cQuadTreeNode* m_pChildNode[4];
	cQuadTreeNode* m_pParentNode;
	cSceneObject* m_pFirstMember;

	u32Flags m_zLocalMask;
	u32Flags m_zMask;

	// Private Functions...
	void descendantMemberAdded(u32Flags zMask);
	void descendantMemberRemoved();
	void rebuildLocalZMask();
	void rebuildZMask();


	// functions available to the quad tree
	friend cQuadTree;
	void setup(cQuadTreeNode* parent, cQuadTreeNode* child0, cQuadTreeNode* child1, cQuadTreeNode* child2, cQuadTreeNode* child3);
};

//. Creators ....................................................

/*	cQuadTreeNode
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cQuadTreeNode::cQuadTreeNode()
:m_pParentNode(0)
,m_pFirstMember(0)
,m_zMask(0)
,m_zLocalMask(0)
{
	memset(m_pChildNode,0,sizeof(m_pChildNode));
}


/*	~cQuadTreeNode
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cQuadTreeNode::~cQuadTreeNode()
{
	debug_assert(m_zMask.value == 0, "the quad tree still has members");
}

//. Accessors ...................................................
inline bool cQuadTreeNode::empty()const
{
	return(m_zMask.value == 0);
}

inline u32Flags cQuadTreeNode::zMask()const
{
	return(m_zMask);
}

inline u32Flags cQuadTreeNode::zLocalMask()const
{
	return(m_zLocalMask);
}

//- End of cQuadTree --------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cQuadTree.h )

//----------------------------------------------------------
//$Log: $