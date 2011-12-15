#define CD3DFRAMEMANAGER_CPP
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

#include "d3dx_frame_manager.h"
#include "game_host.h"

#include <initguid.h>
#ifdef XFILETEMPLATES_H
#undef XFILETEMPLATES_H
#endif
#include "xfile_templates.h"

using namespace gaia;

struct GaiaExtendedData
{
	const char* renderMethodName;
	DWORD materialIndex;
};

//-----------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT gaia::AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
    UINT cbLength;

    if (Name != NULL)
    {
        cbLength = lstrlen(Name) + 1;
        *pNewName = new TCHAR[cbLength];
        if (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy(*pNewName, Name, cbLength*sizeof(TCHAR));
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: cD3DXFrameManager::CreateFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT cD3DXFrameManager::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;

    *ppNewFrame = NULL;

    pFrame = new D3DXFRAME_DERIVED;
    if (pFrame != NULL)
	{
		hr = AllocateName(Name, &pFrame->Name);
		if (SUCCEEDED(hr))
		{
			// initialize other data members of the frame
			D3DXMatrixIdentity(&pFrame->TransformationMatrix);

			pFrame->pMeshContainer = NULL;
			pFrame->pFrameSibling = NULL;
			pFrame->pFrameFirstChild = NULL;

			*ppNewFrame = pFrame;

			if (m_frameCallback)
			{
				hr = m_frameCallback(pFrame, m_callbackData);
				if (FAILED(hr))
				{
					*ppNewFrame = 0;
					safe_delete_array( pFrame->Name );
					safe_delete( pFrame );
					hr = E_FAIL;
				}
			}
		}
		else
		{
			safe_delete_array( pFrame->Name );
			safe_delete( pFrame );
		}
	}
	else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name: cD3DXFrameManager::CreateMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT cD3DXFrameManager::CreateMeshContainer(LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	uint32 x;

    ID3DXBaseMesh* pMesh = 0;

    *ppNewMeshContainer = NULL;


    // this app does not handle patch meshes, 
	// so fail when one is found
    if (pMeshData->Type ==  D3DXMESHTYPE_PATCHMESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
    
	if (pMeshData->Type ==  D3DXMESHTYPE_MESH)
    {
	    // get the pMesh interface pointer out of the mesh data structure
		pMesh = (ID3DXBaseMesh*)pMeshData->pMesh;
	}
	else
	{
	    // get the pMesh interface pointer out of the mesh data structure
		pMesh = (ID3DXBaseMesh*)pMeshData->pPMesh;
	}

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

    // make sure and copy the name.  
	// All memory as input belongs to caller, 
	// interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        

    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();

	// remove circular links from the adjacency information
	for (x=0;x<pMesh->GetNumFaces()*3;x+=3)
	{
		DWORD a = pAdjacency[x];
		DWORD b = pAdjacency[x+1];
		DWORD c = pAdjacency[x+2];

		if (a == b || a == c)
		{
			pAdjacency[x] = 0xffffffff;
		}
		if (b == c)
		{
			pAdjacency[x+1] = 0xffffffff;
		}
	}

	if (pMeshData->Type ==  D3DXMESHTYPE_MESH)
    {
	    // get the pMesh interface pointer out of the mesh data structure
		pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
	}
	else
	{
	    // get the pMesh interface pointer out of the mesh data structure
		pMeshContainer->MeshData.pPMesh = pMeshData->pPMesh;
	}
    pMeshContainer->MeshData.Type = pMeshData->Type;

    pMesh->AddRef();

    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];

    
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->pEffects = new D3DXEFFECTINSTANCE[pMeshContainer->NumMaterials];
	pMeshContainer->ppRenderMethodList = new cRenderMethod*[pMeshContainer->NumMaterials];
	
	
	if ((pMeshContainer->pAdjacency == NULL) 
		|| (pMeshContainer->pMaterials == NULL)
		|| (pMeshContainer->pEffects == NULL)
		|| (pMeshContainer->ppRenderMethodList == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppRenderMethodList, 0, sizeof(cRenderMethod*) * pMeshContainer->NumMaterials);
    memset(pMeshContainer->pEffects, 0, sizeof(D3DXEFFECTINSTANCE) * pMeshContainer->NumMaterials);

    // if materials provided, copy them
    if (NumMaterials > 0)            
    {
		// copy all the material data in bulk
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		// individually copy the material names
		for (iMaterial=0; iMaterial<NumMaterials;++iMaterial)
		{
			AllocateName(pMaterials[iMaterial].pTextureFilename, &pMeshContainer->pMaterials[iMaterial].pTextureFilename);
		}

		// copy the effect instances (if any)
		if (pEffectInstances)
		{
			for (iMaterial=0; iMaterial<NumMaterials; ++iMaterial)
			{
				const D3DXEFFECTINSTANCE* pSrcEffect = &pEffectInstances[iMaterial];
				D3DXEFFECTINSTANCE* pDestEffect = &pMeshContainer->pEffects[iMaterial];
				
				if (pSrcEffect && pDestEffect && pSrcEffect->pEffectFilename)
				{
					AllocateName(pSrcEffect->pEffectFilename, &pDestEffect->pEffectFilename);

					pDestEffect->NumDefaults = pSrcEffect->NumDefaults;

					if (pSrcEffect->NumDefaults)
					{
						pDestEffect->pDefaults = new D3DXEFFECTDEFAULT[pSrcEffect->NumDefaults];
						memcpy(pDestEffect->pDefaults, pSrcEffect->pDefaults, sizeof(D3DXEFFECTDEFAULT)*pSrcEffect->NumDefaults);
					
						for (uint32 iDefault =0; iDefault<pSrcEffect->NumDefaults; ++iDefault)
						{
							pDestEffect->pDefaults[iDefault].pParamName=0;
							AllocateName(pSrcEffect->pDefaults[iDefault].pParamName, &pDestEffect->pDefaults[iDefault].pParamName);
						}
					}
				}
			}
		}
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();


        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new cSIMDMatrix[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            memcpy(&pMeshContainer->pBoneOffsetMatrices[iBone],
					pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone),
					sizeof(cSIMDMatrix));
        }
    }

    // The Callback will take the general mesh information 
	// and transform it to the desired format
	if (m_meshContainerCallback)
	{
		hr = m_meshContainerCallback(pMeshContainer, m_callbackData);
		if (FAILED(hr))
			goto e_Exit;
	}

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;
e_Exit:
    safe_release(pd3dDevice);

    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name: cD3DXFrameManager::DestroyFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT cD3DXFrameManager::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	D3DXFRAME_DERIVED* derivedFrame = (D3DXFRAME_DERIVED*)pFrameToFree;

    safe_delete_array( derivedFrame->Name );
    safe_delete( derivedFrame );
    return S_OK; 
}




//-----------------------------------------------------------------------------
// Name: cD3DXFrameManager::DestroyMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT cD3DXFrameManager::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    safe_delete_array( pMeshContainer->Name );
    safe_delete_array( pMeshContainer->pAdjacency );
    safe_delete_array( pMeshContainer->pBoneOffsetMatrices );

    if (pMeshContainer->ppRenderMethodList != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
			safe_release(pMeshContainer->ppRenderMethodList[iMaterial]);
        }
    }
	safe_delete_array( pMeshContainer->ppRenderMethodList);

    if (pMeshContainer->pEffects != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
			D3DXEFFECTINSTANCE* pEffect = &pMeshContainer->pEffects[iMaterial];
			safe_delete_array(pEffect->pEffectFilename);

			for (uint32 iDefault =0; iDefault<pEffect->NumDefaults; ++iDefault)
			{
				safe_delete_array(pEffect->pDefaults[iDefault].pParamName);
			}
        }
    }
	safe_delete_array(pMeshContainer->pEffects);

    for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
    {
		safe_delete(pMeshContainer->pMaterials[iMaterial].pTextureFilename);
	}

    safe_delete_array( pMeshContainer->pMaterials );
    safe_delete_array( pMeshContainer->pBoneIndexList );
    safe_release( pMeshContainer->pBoneCombinationBuf );
    safe_release( pMeshContainer->pSkinInfo );

	if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
	{
		safe_release( pMeshContainer->MeshData.pMesh );
	}
	else
	{
		safe_release( pMeshContainer->MeshData.pPMesh );
	}

 	if (pMeshContainer->RenderMeshData.Type == D3DXMESHTYPE_MESH)
	{
		safe_release( pMeshContainer->RenderMeshData.pMesh );
	}
	else
	{
		safe_release( pMeshContainer->RenderMeshData.pPMesh );
	}
 
	safe_delete( pMeshContainer );

    return S_OK;
}



HRESULT cD3DXLoadUserData::LoadTopLevelData(LPDIRECTXFILEDATA pXofChildData)
{
	return S_OK;
}

HRESULT cD3DXLoadUserData::LoadFrameChildData(LPD3DXFRAME pFrame, 
                            LPDIRECTXFILEDATA pXofChildData)
{

	HRESULT hr = S_OK;

	const GUID *pGuid;
	hr=pXofChildData->GetType(&pGuid);

	if (SUCCEEDED(hr))
	{
		if (memcmp(pGuid, &DXFILEOBJ_ExtendedMaterial, sizeof(GUID)) == 0)
		{
			if (pFrame->pMeshContainer)
			{
				LoadMeshChildData(pFrame->pMeshContainer, pXofChildData);	
			}
		}
	}
	return hr;
}

HRESULT cD3DXLoadUserData::LoadMeshChildData(LPD3DXMESHCONTAINER pMeshContainerBase, 
                            LPDIRECTXFILEDATA pXofChildData)
{
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	const GUID *pGuid;

	HRESULT hr=pXofChildData->GetType(&pGuid);

	if (SUCCEEDED(hr))
	{
		if (memcmp(pGuid, &DXFILEOBJ_ExtendedMaterial, sizeof(GUID)) == 0)
		{
			DWORD dataSize;
			void* pData;

			hr=pXofChildData->GetData(NULL, &dataSize, &pData);

			if (SUCCEEDED(hr) && dataSize == sizeof(GaiaExtendedData))
			{
				GaiaExtendedData extendedData;
				memcpy(&extendedData, pData, dataSize);

				SetCurrentDirectory(TheGameHost.rootPath());

				cRenderMethod* pRenderMethod = 
					DisplayManager.renderMethodPool().loadResource(extendedData.renderMethodName);

				if (pRenderMethod)
				{
					safe_release(pMeshContainer->ppRenderMethodList[extendedData.materialIndex]);
					pMeshContainer->ppRenderMethodList[extendedData.materialIndex] = pRenderMethod;
				}
			}
		}
	}
	return hr;
}





HRESULT cD3DXSaveUserData::AddFrameChildData(LPD3DXFRAME pFrameBase, 
                            LPDIRECTXFILESAVEOBJECT pXofSave, 
                            LPDIRECTXFILEDATA pXofFrameData)
{
    return S_OK;
}

HRESULT cD3DXSaveUserData::AddMeshChildData(LPD3DXMESHCONTAINER pMeshContainerBase, 
                            LPDIRECTXFILESAVEOBJECT pXofSave, 
                            LPDIRECTXFILEDATA pXofMeshData)
{
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	if (pMeshContainer->NumMaterials && pMeshContainer->ppRenderMethodList)
	{
		for (uint32 i=0; i<pMeshContainer->NumMaterials; ++i)
		{
			cRenderMethod* pMethod = pMeshContainer->ppRenderMethodList[i];

			GaiaExtendedData materialData;
			materialData.materialIndex = i;

			if (pMethod)
			{
				const cString* pMethodName = pMethod->findResourceName();
				materialData.renderMethodName = pMethodName->c_str();
			}
			else
			{
				materialData.renderMethodName = 0;
			}

			LPDIRECTXFILEDATA fileData;
			HRESULT hr = pXofSave->CreateDataObject(DXFILEOBJ_ExtendedMaterial,
										NULL,
										NULL,
										sizeof(materialData),
										(LPVOID)&materialData,
										&fileData);

			if (SUCCEEDED(hr))
			{
				pXofMeshData->AddDataObject(fileData);
			}
			else
			{
				return hr;
			}
		}
	}
    return S_OK;
}

HRESULT cD3DXSaveUserData::AddTopLevelDataObjectsPre(LPDIRECTXFILESAVEOBJECT pXofSave)
{
    return S_OK;
}

HRESULT cD3DXSaveUserData::AddTopLevelDataObjectsPost(LPDIRECTXFILESAVEOBJECT pXofSave)
{
    return S_OK;
}

HRESULT cD3DXSaveUserData::RegisterTemplates(LPDIRECTXFILE pXFileApi)
{
	const char * szTemplates = XGAIA_TEMPLATES;
	return pXFileApi->RegisterTemplates((void*)szTemplates, strlen(szTemplates));
}

HRESULT cD3DXSaveUserData::SaveTemplates(LPDIRECTXFILESAVEOBJECT pXofSave)
{
	const GUID *aIds[] = {
		&DXFILEOBJ_ExtendedMaterial};

	return pXofSave->SaveTemplates(1, aIds);

}

//***************************************************************
// end of file      ( cD3DFrameManager.cpp )

//----------------------------------------------------------
//$Log: $