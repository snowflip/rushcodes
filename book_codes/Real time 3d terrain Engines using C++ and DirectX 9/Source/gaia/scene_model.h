#ifndef CSCENEMODEL_H
#define CSCENEMODEL_H
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
#include "scene_object.h"


//	Name Space declaration
namespace gaia
{

struct D3DXMESHCONTAINER_DERIVED;

class cSceneModel : public cSceneObject
{
public:

	struct sUserData
	{
		uint16 material;
		uint16 subset;
	};

	// Creators...
	cSceneModel();
	virtual ~cSceneModel();

	// Mutators...
	virtual bool create();
	virtual void destroy();
	virtual void prepareForRender();
	virtual void render();
	virtual void renderCallback(cRenderEntry* entry, u32Flags activationFlags);

	virtual void setModelResource(cModelResource* pModel, uint16 frameIndex);

	// Accessors...
	const D3DXMESHCONTAINER_DERIVED* meshContainer()const;

private:

	// bounding box information
	cModelResource* m_pModelResource;
	uint16			m_modelFrameIndex;
	uint32			m_totalBoneMatrices;
    cSIMDMatrix*	m_boneMatrixList;
	int				m_lod;
};

//- Inline Functions ---------------------------------------------------------------------

/*	cSceneModel
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/

inline cSceneModel::cSceneModel()
:cSceneObject()
,m_pModelResource(0)
,m_modelFrameIndex(0)
,m_totalBoneMatrices(0)
,m_boneMatrixList(0)
{
}

/*	~cSceneModel
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cSceneModel::~cSceneModel()
{
	destroy();
}

//. Accessors ............................................................................


inline const D3DXMESHCONTAINER_DERIVED* cSceneModel::meshContainer()const
{
	if (m_pModelResource)
	{
		return (D3DXMESHCONTAINER_DERIVED*)m_pModelResource->frame(m_modelFrameIndex)->pMeshContainer;
	}
	else
	{
		return 0;
	}
}

//- End of cSceneModel -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSceneModel.h )

//----------------------------------------------------------
//$Log: $