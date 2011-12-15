#ifndef SUBNODEGROUP_H
#define SUBNODEGROUP_H
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

#include <d3dx9.h>
#include "model_resource.h"
#include "scene_node.h"
#include "animation_controller.h"


//	Name Space declaration
namespace gaia
{


class cSubNodeGroup
{
public:

	// Data Types & Constants...

	// Public Data...

	// Creators...

    cSubNodeGroup();
    ~cSubNodeGroup();

	// Mutators...
	bool create(cSceneNode* pRootNode, cPoolHandle ModelResource);
	void destroy();

	void update();
	void adjustForAnimationStep();

	// Accessors...
	const cModelResource* modelResource()const;
	uint32 totalSubNodes()const;
    cSceneNode* subNodePtr(uint32 index);

	uint32 totalNamedSubNodes()const;
    cSceneNode* namedSubNodePtr(uint32 index);

	cAnimationController& animController();

private:

	// Private Data...
	cSceneNode*					m_pParentNode;
	cModelResource*				m_pModelResource;
	uint32						m_totalSubNodes;
    cSceneNode**				m_subNodePtrList;
	uint32						m_totalNamedSubNodes;
    cSceneNode**				m_namedSubNodePtrList;
    cAnimationController		m_animController;

	// Private Functions...

	// Unimplemented Functions...
    cSubNodeGroup(const cSubNodeGroup& Src);
    cSubNodeGroup& operator=(const cSubNodeGroup& Src);

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cSubNodeGroup
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cSubNodeGroup::cSubNodeGroup()
:m_pParentNode(0)
,m_pModelResource(0)
,m_totalSubNodes(0)
,m_subNodePtrList(0)
//,m_pAnimController(0)
,m_totalNamedSubNodes(0)
,m_namedSubNodePtrList(0)
{
}

/*	~cSubNodeGroup
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cSubNodeGroup::~cSubNodeGroup()
{
}

//. Accessors ...................................................
inline const cModelResource* cSubNodeGroup::modelResource()const
{
	return m_pModelResource;
}

inline uint32 cSubNodeGroup::totalSubNodes()const
{
	return m_totalSubNodes;
}

inline cSceneNode* cSubNodeGroup::subNodePtr(uint32 index)
{
	debug_assert(index< m_totalSubNodes, "invalid subnode index");
	return m_subNodePtrList[index];
}

inline uint32 cSubNodeGroup::totalNamedSubNodes()const
{
	return m_totalNamedSubNodes;
}

inline cSceneNode* cSubNodeGroup::namedSubNodePtr(uint32 index)
{
	debug_assert(index< m_totalNamedSubNodes, "invalid subnode index");
	return m_namedSubNodePtrList[index];
}

inline cAnimationController& cSubNodeGroup::animController()
{
	return m_animController;
}

//- End of cSubNodeGroup ---------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSubNodeGroup.h )

//----------------------------------------------------------
//$Log: $