#define CQUADTREE_CPP
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
#include "quad_tree.h"
#include "math\math.h"
#include "geometry\geometry.h"
#include "scene_object.h"

using namespace gaia;


void cQuadTree::create(const cRect3d& worldBoundingBox, int depth)
{
	debug_assert(!isReady(), "the quad tree has already been created");
	debug_assert(depth>=k_minimumTreeDepth, "invalid tree depth");
	debug_assert(depth<=k_maximumTreeDepth, "invalid tree depth");

	m_depth = depth;
	m_worldExtents = worldBoundingBox.size();
	m_worldOffset = -worldBoundingBox.minPoint();

	m_worldScale.x = 256.0f/m_worldExtents.x;
	m_worldScale.y = 256.0f/m_worldExtents.y;
	m_worldScale.z = 32.0f/m_worldExtents.z;

	// allocate the nodes
	m_memorySize=0;
	int i;
	for (i=0; i<depth; ++i)
	{
		int nodeCount = (1<<i)*(1<<i);
		uint32 size = sizeof(cQuadTreeNode)*nodeCount;

		m_levelNodes[i] = (cQuadTreeNode*)new cQuadTreeNode[nodeCount];
	
		m_memorySize += size;
	}

	// setup each node
	for (i=0; i<depth; ++i)
	{
		int levelDimension = (1<<i);
		int levelIndex=0;
		for (int y=0; y<levelDimension; ++y)
		{
			for (int x=0; x<levelDimension; ++x)
			{
				m_levelNodes[i][levelIndex].setup(
					getNodeFromLevelXY(i-1, (x>>1),  (y>>1)),
					getNodeFromLevelXY(i+1, (x<<1),	 (y<<1)),
					getNodeFromLevelXY(i+1, (x<<1)+1,(y<<1)),
					getNodeFromLevelXY(i+1, (x<<1),  (y<<1)+1),
					getNodeFromLevelXY(i+1, (x<<1)+1,(y<<1)+1));

				levelIndex++;
			}
		}
	}

}

void cQuadTree::destroy()
{
	for (int i=0; i<k_maximumTreeDepth; ++i)
	{
		safe_delete_array(m_levelNodes[i]);
		m_levelNodes[i] = 0;
	}
	m_depth = 0;
}


// Mutators...
void cQuadTree::findTreeNodeInfo(const cQuadTreeRect& worldByteRect, int& level, int& levelX, int& levelY)
{
	int xPattern = worldByteRect.x0 ^ worldByteRect.x1;
	int yPattern = worldByteRect.y0 ^ worldByteRect.y1;

	int bitPattern = maximum(xPattern, yPattern);
	int highBit = bitPattern ? highestBitSet(bitPattern)+1: 0;

	level = k_maximumTreeDepth-highBit-1;

	level = minimum(level, m_depth-1);

	int shift = k_maximumTreeDepth-level-1;

	levelX = worldByteRect.x1 >> shift;
	levelY = worldByteRect.y1 >> shift;
}

cQuadTreeNode* cQuadTree::findTreeNode(const cQuadTreeRect& worldByteRect)
{
	int level,levelX,levelY;

	findTreeNodeInfo(worldByteRect, level, levelX, levelY);

	return getNodeFromLevelXY(level, levelX, levelY);
}

void cQuadTree::buildByteRect(const cRect3d& worldRect, cQuadTreeRect& worldByteRect)
{
	// convert to integer values, taking the floor of each real
	worldByteRect.convert(worldRect, m_worldOffset, m_worldScale);
}

u32Flags cQuadTree::addOrUpdateSceneObject(cSceneObject* newNode)
{
	cQuadTreeRect byteRect;
	buildByteRect(newNode->worldBounds(), byteRect);

	cQuadTreeNode* node = findTreeNode(byteRect);

	debug_assert(node, "failed to locate quad tree node");

	return(node->addOrUpdateMember(newNode, byteRect));
}

cSceneObject* cQuadTree::buildSearchResults(
	const cRect3d& worldRect, 
	const cFrustum* pOptionalFrustum)
{
	cSceneObject* resultListStart = 0;
	cSceneObject* resultListEnd = 0;

	cQuadTreeRect byteRect;
	buildByteRect(worldRect, byteRect);
	
	u32Flags zMask = ZMASK(byteRect.z0, byteRect.z1);

	bool continue_search = true;
	int level = 0;

	while(level < m_depth && continue_search)
	{
		int shift_count = 8-level;
		cQuadTreeRect localRect(	byteRect.x0 >> shift_count,
							byteRect.x1 >> shift_count,
							byteRect.y0 >> shift_count,
							byteRect.y1 >> shift_count,
							0,0);


		// do not continue unless a populated node is found
		continue_search = false;

		for (int y=localRect.y0; y<=localRect.y1; ++y)
		{
			for (int x=localRect.x0; x<=localRect.x1; ++x)
			{
				cQuadTreeNode* node = getNodeFromLevelXY(level, x, y);
				
				if (node->zMask() & zMask)
				{
					// a populated node has been found
					continue_search = true;

					// search all the edge cells with the full world rectangle,
					// all non-edge cells are contained within the search rect 
					// and be called with just the z extents
					if (y == localRect.y0 
						|| y == localRect.y1
						|| x == localRect.x0 
						|| x == localRect.x1)
					{
						// test all members of this node against the world rect
						node->testLocalMembersForSearchResults(
							&resultListStart, 
							&resultListEnd, 
							zMask, 
							worldRect, 
							pOptionalFrustum);
					}
					else
					{
						// test all members of this node against 
						// the world Z extents only
						node->testLocalMembersForSearchResults(
							&resultListStart, 
							&resultListEnd, 
							zMask, 
							worldRect.z0, 
							worldRect.z1, 
							pOptionalFrustum);
					}
				}
			}
		}

		// step up to the next level of the tree
		++level;
	}

	return resultListStart;
}


//***************************************************************
// end of file      ( cQuadTree.cpp )

//----------------------------------------------------------
//$Log: $