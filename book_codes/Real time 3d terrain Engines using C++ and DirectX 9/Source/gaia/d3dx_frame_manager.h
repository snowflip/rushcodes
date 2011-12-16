#ifndef CD3DXFRAMEMANAGER_H
#define CD3DXFRAMEMANAGER_H
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
#include "gaia\surface_material.h"
#include "gaia\render_method.h"
#include <d3dx9.h>
#include <dxfile.h>

//	Name Space declaration
namespace gaia
{

//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can 
//       add some app-specific
//       info that will be stored with each frame
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	uint16	frameIndex;
	uint16	parentIndex;
};


//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we 
//		 can add some app-specific
//       info that will be stored with each mesh
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	// SkinMesh info      
	D3DXMESHDATA		 RenderMeshData;
    DWORD                NumAttributeGroups; 
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumBoneInfluences;
    int*				 pBoneIndexList;
    LPD3DXBUFFER         pBoneCombinationBuf;
    DWORD                NumBoneMatrices;
    cSIMDMatrix*         pBoneOffsetMatrices;
	cRenderMethod**		 ppRenderMethodList;
};

#define MESH_INTERFACE(md) ((ID3DXBaseMesh*)md.pMesh)
/*	cD3DXFrameManager
-----------------------------------------------------------------
    
	Custom version of ID3DXAllocateHierarchy with custom methods 
	to create frames and meshcontainers.  
	  
-----------------------------------------------------------------
*/

typedef HRESULT (*MeshContainerCallback)(D3DXMESHCONTAINER_DERIVED *pMeshContainer, void* userData);
typedef HRESULT (*D3DFrameCallback)(D3DXFRAME_DERIVED *pMeshContainer, void* userData);

class cD3DXFrameManager: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
 
    cD3DXFrameManager(MeshContainerCallback meshContainerCallback=0, 
		D3DFrameCallback frameCallback=0, 
		void* callbackData = 0)
		:m_meshContainerCallback(meshContainerCallback)
		,m_frameCallback(frameCallback)
		,m_callbackData(callbackData){};

	~cD3DXFrameManager(){};

	MeshContainerCallback m_meshContainerCallback;
	D3DFrameCallback m_frameCallback;
	void* m_callbackData;
};

class cAnimationData;

class cD3DXLoadUserData: public ID3DXLoadUserData
{
    STDMETHOD(LoadTopLevelData)(LPDIRECTXFILEDATA pXofChildData);
                            
    STDMETHOD(LoadFrameChildData)(LPD3DXFRAME pFrame, 
                            LPDIRECTXFILEDATA pXofChildData);
                            
    STDMETHOD(LoadMeshChildData)(LPD3DXMESHCONTAINER pMeshContainer, 
                            LPDIRECTXFILEDATA pXofChildData);   
	
};

class cD3DXSaveUserData: public ID3DXSaveUserData
{
    STDMETHOD(AddFrameChildData)(LPD3DXFRAME pFrame, 
                            LPDIRECTXFILESAVEOBJECT pXofSave, 
                            LPDIRECTXFILEDATA pXofFrameData);
                            
    STDMETHOD(AddMeshChildData)(LPD3DXMESHCONTAINER pMeshContainer, 
                            LPDIRECTXFILESAVEOBJECT pXofSave, 
                            LPDIRECTXFILEDATA pXofMeshData);
                            
    // NOTE: this is called once per Save.  All top level objects should be added using the 
    //    provided interface.  One call adds objects before the frame hierarchy, the other after
    STDMETHOD(AddTopLevelDataObjectsPre)(LPDIRECTXFILESAVEOBJECT pXofSave); 
    STDMETHOD(AddTopLevelDataObjectsPost)(LPDIRECTXFILESAVEOBJECT pXofSave);                             

    // callbacks for the user to register and then save templates to the XFile
    STDMETHOD(RegisterTemplates)(LPDIRECTXFILE pXFileApi);                             
    STDMETHOD(SaveTemplates)(LPDIRECTXFILESAVEOBJECT pXofSave);                             
};

//-----------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName );

//- End of cD3DXFrameManager ------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cD3DXFrameManager.h )

//----------------------------------------------------------
//$Log: $