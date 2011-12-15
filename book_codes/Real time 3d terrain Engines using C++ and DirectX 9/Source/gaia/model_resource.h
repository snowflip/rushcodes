#ifndef CMODELRESOURCE_H
#define CMODELRESOURCE_H
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
#include "d3dx_frame_manager.h"


//	Name Space declaration
namespace gaia
{


class cModelResource : public cResourcePoolItem
{
public:

	// Data Types & Constants...
	typedef HRESULT (*ModelCallback)(D3DXFRAME_DERIVED* pFrame, D3DXMESHCONTAINER_DERIVED *pMeshContainer, void* userData);

	// Public Data...
	enum eLODLevels
	{
		k_minLOD = 0,
		k_maxLOD = 3,
		k_totalLODlevels
	};

	// Creators...
    cModelResource();
    virtual ~cModelResource();

	// Resource Functions...
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	// callbacks for the construction process.
	HRESULT addMeshContainer(D3DXMESHCONTAINER_DERIVED *pMeshContainer);
	HRESULT addSubFrame(D3DXFRAME_DERIVED *pMeshContainer);

	// Accessors...
	uint16 totalFrames()const;
	uint16 totalBoneMatrices()const;
	const LPD3DXANIMATIONCONTROLLER animationController()const;
	const D3DXFRAME_DERIVED* frame(uint16 index)const;
	const cSphere& boundingSphere()const;
	const cRect3d& boundingBox()const;

	HRESULT enumMeshContainers(ModelCallback callback, void* userData);

	cSurfaceMaterial* getSurfaceMaterial(D3DXMESHCONTAINER_DERIVED *pMeshContainer, int materialIndex);

	HRESULT setRenderMethod(D3DXMESHCONTAINER_DERIVED *pMeshContainer, int materialIndex, cRenderMethod* pMethodSet);
//	cRenderMethod* getRenderMethodSet(D3DXMESHCONTAINER_DERIVED *pMeshContainer, int materialIndex);

	D3DXFRAME_DERIVED* getRootFrame()const;

	void renderModelSubset(int iFrame, int iSubset);

	void insertAnimation(uint32 insertBefore, LPD3DXANIMATIONSET animSet);
	void addAnimations(LPD3DXANIMATIONCONTROLLER pExternalController);
	void removeAnimation(uint32 slot);
	void moveAnimation(uint32 indexFrom, uint32 indexTo);

	uint32 totalAnimations()const;
	const tchar* animationName(uint32 slot)const;

	bool findFirstModelFrame(uint16& frame)const;
	bool findNextModelFrame(uint16& frame, uint16 lastFrame)const;

	bool containsProgressiveMesh()const;
	void setLOD(int newLOD);
	void setLODScale(float zeroToOne);
	int getLOD()const;
	float getLODScale()const;

private:

	// Private Data...
	uint16						m_totalFrames;
    uint16                      m_totalBoneMatrices;
    D3DXFRAME_DERIVED*		    m_pFrameRoot;
    D3DXFRAME_DERIVED**		    m_pFrameList;
    LPD3DXANIMATIONCONTROLLER   m_pAnimController;
	cSphere						m_boundingSphere;
	cRect3d						m_boundingBox;

	// LOD information (only valid for progressive meshes)
	bool	m_bProgressive; // true when progressive mesh is present
	int8	m_currentLOD;

	// Private Functions...
	HRESULT CalculateBoundingRect(LPD3DXFRAME pFrame, cRect3d& rect);
    HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainer );
    HRESULT SetupBoneMatrixPointers(D3DXFRAME_DERIVED* pFrame);
	HRESULT EnumerateFrames(D3DXFRAME_DERIVED* pFrame, uint16 parent, uint16& index);
	HRESULT BuildFrameList();

	// Unimplemented Functions...
    explicit cModelResource(const cModelResource& Src);
    cModelResource& operator=(const cModelResource& Src);

};

typedef cResourcePool<cModelResource> cModelManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cModelResource
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cModelResource::cModelResource()
:m_totalFrames(0)
,m_totalBoneMatrices(0)
,m_pFrameRoot(0)
,m_pFrameList(0)
,m_pAnimController(0)
,m_boundingSphere(0.0f,0.0f,0.0f,0.0f)
,m_boundingBox(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f)
,m_currentLOD(k_maxLOD)
,m_bProgressive(0)
{
}

/*	~cModelResource
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cModelResource::~cModelResource()
{
}

inline uint16 cModelResource::totalFrames()const
{
	return m_totalFrames;
}

inline uint16 cModelResource::totalBoneMatrices()const
{
	return m_totalBoneMatrices;
}

inline const LPD3DXANIMATIONCONTROLLER cModelResource::animationController()const
{
	return m_pAnimController;
}

inline const D3DXFRAME_DERIVED* cModelResource::frame(uint16 index)const
{
	debug_assert(index<m_totalFrames, "invalid frame index");
	return m_pFrameList[index];
}

inline const cSphere& cModelResource::boundingSphere()const
{
	return m_boundingSphere;
}

inline const cRect3d& cModelResource::boundingBox()const
{
	return m_boundingBox;
}

inline D3DXFRAME_DERIVED* cModelResource::getRootFrame()const
{
	return m_pFrameRoot;
}

inline int cModelResource::getLOD()const
{
	return m_currentLOD;
}

inline float cModelResource::getLODScale()const
{
	return (float)m_currentLOD/(float)k_maxLOD;
}

inline bool cModelResource::containsProgressiveMesh()const
{
	return m_bProgressive;
}

//- End of cModelResource ----------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cModelResource.h )

//----------------------------------------------------------
//$Log: $