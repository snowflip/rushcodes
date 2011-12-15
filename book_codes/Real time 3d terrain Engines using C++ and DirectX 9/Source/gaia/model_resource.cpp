#define CMODELRESOURCE_CPP
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

#include "game_host.h"
#include "model_resource.h"
#include "animation_controller.h"

using namespace gaia;

static HRESULT meshContainerCallback(D3DXMESHCONTAINER_DERIVED *pMeshContainer, void* userData)
{
	cModelResource* pModelResource= (cModelResource*)userData;

	return pModelResource->addMeshContainer(pMeshContainer);
}

static HRESULT d3dFrameCallback(D3DXFRAME_DERIVED *pFrame, void* userData)
{
	cModelResource* pModelResource= (cModelResource*)userData;

	return pModelResource->addSubFrame(pFrame);
}

// innitialize the resource (called once)
bool cModelResource::createResource()
{
	return true;
}

// destroy the resource
bool cModelResource::destroyResource()
{
    if (m_pFrameRoot)
	{
		cD3DXFrameManager Allocator(meshContainerCallback, d3dFrameCallback, this);
		D3DXFrameDestroy(m_pFrameRoot, &Allocator);
		m_pFrameRoot=0;
	}

    safe_release(m_pAnimController);

	return true;
}

// purge the resource from volatile memory
bool cModelResource::disableResource()
{
	return true;
}
				
// prepare the resource for use (create any volatile memory objects needed)
bool cModelResource::restoreResource()
{
	return true;
}

HRESULT prepLoadedMesh(D3DXFRAME_DERIVED* pFrame, D3DXMESHCONTAINER_DERIVED *pMeshContainer, void* userData)
{
	cModelResource* pModelResource= (cModelResource*)userData;

	if (pMeshContainer)
	{
		//
		// Check the render methods
		//
		for (uint32 iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			if (!pMeshContainer->ppRenderMethodList[iMaterial])
			{
				cRenderMethod* pRenderMethod = 
					TheGameHost.displayManager().renderMethodPool().createResource(cString("method_%i", pMeshContainer));
				
				// choose a default effect file to use
				cEffectFile* pEffectFile=0;
				if (pMeshContainer->pSkinInfo != NULL)
				{
					pEffectFile = DisplayManager.effectFilePool().loadResource("media\\shaders\\DefaultSkinnedMesh.fx");
				}
				else
				{
					pEffectFile = DisplayManager.effectFilePool().loadResource("media\\shaders\\DefaultMesh.fx");
				}

				// create a run-time material
				cSurfaceMaterial *pMaterial = 
					TheGameHost.displayManager().surfaceMaterialPool().createResource(cString("material_%i", pMeshContainer));

				// if the x-file has a texture, load it into the material
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename)
				{
					cString  strTexturePath(pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					
					cTexture* pTexture = TheGameHost.displayManager().texturePool().loadResource(strTexturePath);
					if (pTexture)
					{
						// put the texture is every material slot
						// place the texture in the surface material
						pMaterial->setTexture(0, pTexture);
						// release our local reference to the texture. 
						// It is now owned by the surface material
						//TheGameHost.displayManager().texturePool().releaseResource(pTexture);
						safe_release(pTexture);
					}
				}

				// also copy the material settings
				pMaterial->setD3DMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);

				// now load both into the default render slot
				pRenderMethod->setEffect(0, pEffectFile);
				pRenderMethod->setMaterial(0, pMaterial);

				// release our references to these objects
				safe_release(pEffectFile);
				safe_release(pMaterial);

				// set the render method
				pModelResource->setRenderMethod(
					pMeshContainer, 
					iMaterial, 
					pRenderMethod);

				// we can now release our local copy
				safe_release(
					pRenderMethod);
			}
			else
			{
				cRenderMethod* pRenderMethod = pMeshContainer->ppRenderMethodList[iMaterial];
				pRenderMethod->AddRef();

				// set the render method
				pModelResource->setRenderMethod(
					pMeshContainer, 
					iMaterial, 
					pRenderMethod);

				// we can now release our local copy
				safe_release(
					pRenderMethod);
			}
		}
	}

	return S_OK;
}

// load the resource from a file
bool cModelResource::loadResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
    HRESULT hr;

	cD3DXFrameManager Allocator(meshContainerCallback, d3dFrameCallback, this);
	cD3DXLoadUserData loadUserData;
	uint32 maxTracks =0;
	m_bProgressive = false;

    hr = D3DXLoadMeshHierarchyFromX(filename, 
			D3DXMESH_MANAGED, 
			pd3dDevice, 
			&Allocator, 
			&loadUserData, 
			(D3DXFRAME**)&m_pFrameRoot, 
			&m_pAnimController);

    if (FAILED(hr))
        goto ERROR_EXIT;


	hr = BuildFrameList();
    if (FAILED(hr))
        goto ERROR_EXIT;
	
	enumMeshContainers(prepLoadedMesh, this);

	hr = SetupBoneMatrixPointers(m_pFrameRoot);
    if (FAILED(hr))
        goto ERROR_EXIT;

	m_boundingSphere.radius = 0.0f;
    hr = D3DXFrameCalculateBoundingSphere(m_pFrameRoot, &m_boundingSphere.center, &m_boundingSphere.radius);
    if (FAILED(hr))
        goto ERROR_EXIT;

	m_boundingBox.clear();
    hr = CalculateBoundingRect(m_pFrameRoot, m_boundingBox);
    if (FAILED(hr))
        goto ERROR_EXIT;

	// D3DXFrameCalculateBoundingSphere seems to have trouble with some PMESH objects.
	// if the radius is zero, we use the box we built to define the sphere
	if (m_boundingSphere.radius == 0.0f)
	{
		m_boundingSphere.center = m_boundingBox.midpoint();
		cVector3 cornerOffset = 
			m_boundingBox.maxPoint() - m_boundingBox.midpoint();
		m_boundingSphere.radius = cornerOffset.length();
	}

	// set the highest LOD
	m_currentLOD = k_minLOD;
	setLOD(k_maxLOD);
	return true;

ERROR_EXIT:

	debug_error(hr);
	return false;
}
	
HRESULT prepForSave(D3DXFRAME_DERIVED* pFrame, D3DXMESHCONTAINER_DERIVED *pMeshContainer, void* userData)
{
	if (pMeshContainer)
	{
		for (uint32 iMaterial=0;iMaterial<pMeshContainer->NumMaterials; ++iMaterial)
		{
			cRenderMethod* pMethod = 
				pMeshContainer->ppRenderMethodList[iMaterial];
		
			if (pMethod)
			{
				// copy	the base shader file name and texture name
				// to the x-files standard locations
				cSurfaceMaterial* pMaterial = pMethod->getMaterial(0);
				cEffectFile* pEffect = pMethod->getEffect(0);

				if (pMaterial)
				{
					memcpy(&pMeshContainer->pMaterials[iMaterial].MatD3D, 
							&pMaterial->d3dMaterial(),
							sizeof(D3DMATERIAL9));
					
					// copy the material name into the mesh container
					safe_delete(pMeshContainer->pMaterials[iMaterial].pTextureFilename);

					const cString* pName = pMaterial->findResourceName();

					if (pName)
					{
						AllocateName(
							pName->c_str(), 
							&pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					}
				}

				if (pEffect)
				{
					// copy the effect file name
					safe_delete(pMeshContainer->pEffects[iMaterial].pEffectFilename);

					const cString* pName = pEffect->findResourceName();

					if (pName)
					{
						AllocateName(
							pName->c_str(), 
							&pMeshContainer->pEffects[iMaterial].pEffectFilename);
					}
				}
			}
		}
	}
	return S_OK;
}

// save the resource to the file and return the size written
bool cModelResource::saveResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	// prepare the data for saving
	enumMeshContainers(prepForSave, 0);
	cD3DXSaveUserData saveUserData;

	HRESULT hr = D3DXSaveMeshHierarchyToFile(filename,
		DXFILEFORMAT_TEXT,
		(LPD3DXFRAME) m_pFrameRoot,
		m_pAnimController,
		&saveUserData);


	if (SUCCEEDED(hr))
	{
		return true;
	}
	else
	{
		debug_error(hr);
	}
	
	return false;
}


//-----------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: Called to setup the pointers for a given bone to its transformation matrix
//-----------------------------------------------------------------------------
HRESULT cModelResource::CalculateBoundingRect(LPD3DXFRAME pFrame, cRect3d& rect)
{
    HRESULT hr;

    if (pFrame->pMeshContainer != NULL)
    {
        // get the base mesh interface
		LPD3DXBASEMESH pBaseMesh = 0;
		
		if (pFrame->pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
		{
			pBaseMesh = (LPD3DXBASEMESH)pFrame->pMeshContainer->MeshData.pMesh;
		}
		else
		{
			pBaseMesh = (LPD3DXBASEMESH)pFrame->pMeshContainer->MeshData.pPMesh;
		}

		if (pBaseMesh)
		{
			DWORD count = pBaseMesh->GetNumVertices();
			DWORD stride = pBaseMesh->GetNumBytesPerVertex();
			void* pVerts=0;

			if (SUCCEEDED(pBaseMesh->LockVertexBuffer(D3DLOCK_READONLY,&pVerts)))
			{
				HRESULT boxResult;
				
				cVector3 minPoint;
				cVector3 maxPoint;
				boxResult = D3DXComputeBoundingBox((LPD3DXVECTOR3)pVerts,
							count,
							stride,
							&minPoint,
							&maxPoint);

				if (SUCCEEDED(boxResult))
				{
					rect.unionPoint(minPoint);
					rect.unionPoint(maxPoint);
				}
				else
				{
					debug_error(boxResult);
				}

				pBaseMesh->UnlockVertexBuffer();
			}
		}
    }

    if (pFrame->pFrameSibling != NULL)
    {
        hr = CalculateBoundingRect(pFrame->pFrameSibling, rect);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        hr = CalculateBoundingRect(pFrame->pFrameFirstChild, rect);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetupBoneMatrixPointersOnMesh()
// Desc: Called to setup the pointers for a given bone to its transformation matrix
//-----------------------------------------------------------------------------
HRESULT cModelResource::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    // if there is a skinmesh, then setup the bone matrices
    if (pMeshContainer->pSkinInfo != NULL)
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();

        pMeshContainer->pBoneIndexList = new int[cBones];
        if (pMeshContainer->pBoneIndexList == NULL)
            return E_OUTOFMEMORY;

        for (iBone = 0; iBone < cBones; iBone++)
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
            if (pFrame == NULL)
                return E_FAIL;

            pMeshContainer->pBoneIndexList[iBone] = pFrame->frameIndex;
        }
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: Called to setup the pointers for a given bone to its transformation matrix
//-----------------------------------------------------------------------------
HRESULT cModelResource::SetupBoneMatrixPointers(D3DXFRAME_DERIVED* pFrame)
{
    HRESULT hr;

    if (pFrame->pMeshContainer != NULL)
    {
        hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        hr = SetupBoneMatrixPointers((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        hr = SetupBoneMatrixPointers((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumerateFrames()
// Desc: Assign index values to the frames in sibling-child order
//		and copy them to our frame pointer table
//-----------------------------------------------------------------------------
HRESULT cModelResource::EnumerateFrames(D3DXFRAME_DERIVED* pFrame, uint16 parent, uint16& index)
{
    HRESULT hr;

	uint16 localIndex = index;
	index++;

	pFrame->frameIndex = localIndex;
	pFrame->parentIndex = parent;
	m_pFrameList[localIndex] = pFrame;

    if (pFrame->pFrameSibling != NULL)
    {
        hr = EnumerateFrames((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, parent, index);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
		parent = index;
        hr = EnumerateFrames((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, localIndex, index);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}

HRESULT cModelResource::BuildFrameList()
{
	safe_delete(m_pFrameList);
	m_pFrameList = new D3DXFRAME_DERIVED*[m_totalFrames];
	uint16 index=0;

	return EnumerateFrames((D3DXFRAME_DERIVED*)m_pFrameRoot, MAX_UINT16, index);
}

HRESULT cModelResource::setRenderMethod(D3DXMESHCONTAINER_DERIVED *pMeshContainer, int iMaterial, cRenderMethod* pRenderMethod)
{
    HRESULT hr = S_OK;

	safe_release(pMeshContainer->ppRenderMethodList[iMaterial]);
	pMeshContainer->ppRenderMethodList[iMaterial] = pRenderMethod;

	if (pRenderMethod)
	{
		pRenderMethod->AddRef();
		BOOL bUsesTangents = TRUE;
		BOOL bUsesBinormal = TRUE;

		for (int iMethod=0; iMethod<cRenderMethod::k_max_render_stages; ++iMethod)
		{
			cEffectFile* pEffectFile = 
				pRenderMethod->getEffect(iMethod);

			if (pEffectFile && pEffectFile->effect())
			{
				LPD3DXEFFECT pEffect = pEffectFile->effect();

				// Look for tangents semantic
				D3DXEFFECT_DESC EffectDesc;
				D3DXHANDLE hTechnique;
				D3DXTECHNIQUE_DESC TechniqueDesc;
				D3DXHANDLE hPass;
				D3DXPASS_DESC PassDesc;

				pEffect->GetDesc( &EffectDesc );

				for( UINT iTech = 0; 
					iTech < EffectDesc.Techniques; 
					iTech++ )
				{
					hTechnique = pEffect->GetTechnique( iTech );
					pEffect->GetTechniqueDesc( hTechnique, &TechniqueDesc );

					for( UINT iPass = 0; 
						iPass < TechniqueDesc.Passes; 
						iPass++ )
					{
						hPass = pEffect->GetPass( hTechnique, iPass );
						pEffect->GetPassDesc( hPass, &PassDesc );

						for( UINT iSem = 0; 
							iSem < PassDesc.VSSemanticsUsed; 
							iSem++ )
						{
							if( PassDesc.VSSemantics[iSem].Usage == D3DDECLUSAGE_TANGENT )
							{
								bUsesTangents = TRUE;
							}
							if( PassDesc.VSSemantics[iSem].Usage == D3DDECLUSAGE_BINORMAL )
							{
								bUsesBinormal = TRUE;
							}
						}
					}
				}
			}
		}
		//
		// compute binormal and tanget data if needed
		//
		//LPD3DXMESH& pOrigMesh = pMeshContainer->MeshData.pMesh;
        // get the base mesh interface
		LPD3DXBASEMESH pOrigMesh = 0;
		
		if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
		{
			pOrigMesh = (LPD3DXBASEMESH)pMeshContainer->MeshData.pMesh;
		}
		else
		{
			pOrigMesh = (LPD3DXBASEMESH)pMeshContainer->MeshData.pPMesh;
		}

		BOOL bRebuildRenderMesh = !pMeshContainer->RenderMeshData.pMesh;
		if (bUsesTangents || bUsesBinormal)
		{
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			D3DVERTEXELEMENT9 End = D3DDECL_END();
			int iElem;
 
			pOrigMesh->GetDeclaration( Declaration );
 
			BOOL bHasTangents = FALSE;
			BOOL bHasBinormal = FALSE;
			for( iElem=0; Declaration[iElem].Stream != End.Stream; iElem++ )
			{   
				if( Declaration[iElem].Usage == D3DDECLUSAGE_TANGENT )
				{
					bHasTangents = TRUE;
				}
				if( Declaration[iElem].Usage == D3DDECLUSAGE_BINORMAL )
				{
					bHasBinormal = TRUE;
				}
			}
 
			bool cloneMesh=false;
			WORD currentOffset = (WORD)pOrigMesh->GetNumBytesPerVertex();

			// Update Mesh Semantics if changed
			if( bUsesTangents && !bHasTangents ) 
			{
				Declaration[iElem].Stream = 0;
				Declaration[iElem].Offset = currentOffset;
				Declaration[iElem].Type = D3DDECLTYPE_FLOAT3;
				Declaration[iElem].Method = D3DDECLMETHOD_DEFAULT;
				Declaration[iElem].Usage = D3DDECLUSAGE_TANGENT;
				Declaration[iElem].UsageIndex = 0;
				Declaration[iElem+1] = End;
				++iElem;
				currentOffset += sizeof(float)*3;
				cloneMesh = true;
			}

			if( bUsesBinormal && !bHasBinormal ) 
			{
				Declaration[iElem].Stream = 0;
				Declaration[iElem].Offset = currentOffset;
				Declaration[iElem].Type = D3DDECLTYPE_FLOAT3;
				Declaration[iElem].Method = D3DDECLMETHOD_DEFAULT;
				Declaration[iElem].Usage = D3DDECLUSAGE_BINORMAL;
				Declaration[iElem].UsageIndex = 0;
				Declaration[iElem+1] = End;
				++iElem;
				currentOffset += sizeof(float)*3;
				cloneMesh = true;
			}
			Declaration[iElem]=End;

			if (cloneMesh)
			{
				if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_PMESH)
				{
					// set to the highest possible LOD
					pMeshContainer->MeshData.pPMesh->SetNumVertices(0xffffffff);
				}

				// clone a mesh with a shared VB
				if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
				{
					LPD3DXMESH pTempMesh;
					hr = pMeshContainer->MeshData.pMesh->CloneMesh( D3DXMESH_SYSTEMMEM, Declaration, TheGameHost.d3dDevice(), &pTempMesh );
					if( SUCCEEDED( hr ) )
					{
						// do the work on the clone. The shared VB will ensure that
						// the original mesh is updated as well
						hr = D3DXComputeTangent( pTempMesh, 0, 0, 0, TRUE, pMeshContainer->pAdjacency );
    
						if (FAILED(hr))
						{
							debug_error(hr);
						}
						SAFE_RELEASE( pOrigMesh );
						pMeshContainer->MeshData.pMesh = pTempMesh;
						pOrigMesh = (LPD3DXBASEMESH)pTempMesh;
					}
				}
				bRebuildRenderMesh = true;
			}
		}

		if (bRebuildRenderMesh)
		{
			if (pMeshContainer->RenderMeshData.Type == D3DXMESHTYPE_PMESH)
			{
				SAFE_RELEASE( pMeshContainer->RenderMeshData.pPMesh );
			}
			else
			{
				SAFE_RELEASE( pMeshContainer->RenderMeshData.pMesh );
			}
			SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

			if (pMeshContainer->pSkinInfo != NULL)
			{
				if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_PMESH)
				{
					// set to the highest possible LOD
					pMeshContainer->MeshData.pPMesh->SetNumVertices(0xffffffff);
				}

				// clone a mesh with a shared VB
				D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
				pOrigMesh->GetDeclaration(Declaration);
				LPD3DXMESH pTempMesh;
				hr = pOrigMesh->CloneMesh( D3DXMESH_SYSTEMMEM, Declaration, TheGameHost.d3dDevice(), &pTempMesh );
				if( SUCCEEDED( hr ) )
				{
					// Get palette size
					// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
					// (96 - 9) /3 i.e. Maximum constant count - used constants 
					UINT MaxMatrices = 26; 
					pMeshContainer->NumBoneMatrices = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

					DWORD Flags = D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED | D3DXMESH_WRITEONLY;
					LPD3DXMESH pNewMesh;
					hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
															(
															pTempMesh,
															Flags, 
															pMeshContainer->NumBoneMatrices, 
															pMeshContainer->pAdjacency, 
															NULL, NULL, NULL,             
															&pMeshContainer->NumBoneInfluences,
															&pMeshContainer->NumAttributeGroups, 
															&pMeshContainer->pBoneCombinationBuf, 
															&pNewMesh);
					if (SUCCEEDED(hr))
					{
						//
						// Build semantics for the new mesh
						//
						D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
						LPD3DVERTEXELEMENT9 pDeclCur;
						hr = pNewMesh->GetDeclaration(pDecl);
						if (SUCCEEDED(hr))
						{
							// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
							//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
							//          this is more of a "cast" operation
							pDeclCur = pDecl;
							while (pDeclCur->Stream != 0xff)
							{
								if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
								{
									pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
								}
								pDeclCur++;
							}

							hr = pNewMesh->UpdateSemantics(pDecl);

							// do we copy the new mesh or convert to a PMesh?
							if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
							{
								pMeshContainer->RenderMeshData.pMesh = pNewMesh;
							}
							else
							{
								hr = D3DXGeneratePMesh(          
									pNewMesh,
									pMeshContainer->pAdjacency,
									NULL,
									NULL,
									pMeshContainer->MeshData.pPMesh->GetMinFaces(),
									D3DXMESHSIMP_FACE,
									&pMeshContainer->RenderMeshData.pPMesh
								);

								if (SUCCEEDED(hr))
								{
									pMeshContainer->RenderMeshData.Type = 
										pMeshContainer->MeshData.Type;

									SAFE_RELEASE(pNewMesh);
								}
								else
								{
									pMeshContainer->RenderMeshData.Type = 
										D3DXMESHTYPE_MESH;
									pMeshContainer->RenderMeshData.pMesh = pNewMesh;
								}
							}
						}
					}
					SAFE_RELEASE(pTempMesh);
				}
			}
			else
			{
				DWORD Flags = D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;

				if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
				{
			
/*					hr = pMeshContainer->MeshData.pMesh->Optimize(	
												Flags, 
												pMeshContainer->pAdjacency,
												NULL, 
												NULL, 
												NULL,
												&pMeshContainer->RenderMeshData.pMesh);
*/
					D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
					pMeshContainer->MeshData.pMesh->GetDeclaration( Declaration );

					hr = pMeshContainer->MeshData.pMesh->CloneMesh(	
												D3DXMESH_MANAGED ,
												Declaration, 
												TheGameHost.d3dDevice(),
												&pMeshContainer->RenderMeshData.pMesh);
				}
				else
				{
					D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
					pMeshContainer->MeshData.pPMesh->GetDeclaration( Declaration );
				
					hr = pMeshContainer->MeshData.pPMesh->ClonePMesh(	
												D3DXMESH_MANAGED ,
												Declaration, 
												TheGameHost.d3dDevice(),
												&pMeshContainer->RenderMeshData.pPMesh);
				}
			
				pMeshContainer->RenderMeshData.Type = 
					pMeshContainer->MeshData.Type;

			}
		}

		if (FAILED(hr))
		{
			debug_error(hr);
		}
	}

	return hr;
}


//-----------------------------------------------------------------------------
// Name: GenerateSkinnedMesh()
// Desc: Called either by CreateMeshContainer when loading a skin mesh, or when 
//       changing methods.  This function uses the pSkinInfo of the mesh 
//       container to generate the desired drawable mesh and bone combination 
//       table.
//-----------------------------------------------------------------------------
HRESULT cModelResource::addMeshContainer(D3DXMESHCONTAINER_DERIVED *pMeshContainer)
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	const D3DCAPS9& pd3dCaps = TheGameHost.d3dCaps();

    HRESULT hr = S_OK;

	SetCurrentDirectory(TheGameHost.rootPath());

	if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_PMESH)
	{
		m_bProgressive = true;
	}


	
    if (pMeshContainer->pSkinInfo != NULL)
	{
		// keep track of the total number of bone matrices requires
		if (m_totalBoneMatrices < pMeshContainer->pSkinInfo->GetNumBones())
		{
			m_totalBoneMatrices = (uint16)pMeshContainer->pSkinInfo->GetNumBones();
		}
	}

    return hr;
}

HRESULT cModelResource::addSubFrame(D3DXFRAME_DERIVED *pd3dxFrame)
{
	m_totalFrames++;
    return S_OK;
}

HRESULT cModelResource::enumMeshContainers(ModelCallback callback, void* userData)
{
	if (m_pFrameList)
	{
		for (int i=0;i<m_totalFrames;++i)
		{
			D3DXFRAME_DERIVED *pFrame = m_pFrameList[i];
			if (pFrame->pMeshContainer != NULL)
			{
				HRESULT hr = callback(pFrame, 
									  (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer, 
									  userData);
				if (FAILED(hr))
					return hr;
			}
		}
	}
    return S_OK;
}

uint32 cModelResource::totalAnimations()const
{
	uint32 total = 0;
	if (m_pAnimController)
	{
		total = m_pAnimController->GetNumAnimationSets();
	}
	return total;
}

const tchar* cModelResource::animationName(uint32 slot)const
{
	debug_assert(slot < totalAnimations(), "invalid animation slot");
	const tchar* name=0;

	if (m_pAnimController)
	{
		LPD3DXANIMATIONSET pAnimationSet = 0;

		m_pAnimController->GetAnimationSet(slot, &pAnimationSet);

		if (pAnimationSet)
		{
			name = pAnimationSet->GetName();
			safe_release(pAnimationSet);
		}
	}
	return name;
}

void cModelResource::insertAnimation(uint32 insertBefore, LPD3DXANIMATIONSET animSet)
{
	uint32 animationCount = totalAnimations();

	if (m_pAnimController)
	{
		HRESULT hr;
		uint32 maxCount = m_pAnimController->GetMaxNumAnimationSets();

		if (animationCount+1 >= maxCount)
		{
			LPD3DXANIMATIONCONTROLLER newController;

			hr = m_pAnimController->CloneAnimationController(
				m_pAnimController->GetMaxNumMatrices(),
				animationCount+1,
				m_pAnimController->GetMaxNumTracks(),
				m_pAnimController->GetMaxNumEvents(),
				&newController
			);

			if (FAILED(hr))
			{
				return;
			}

			safe_release(m_pAnimController);
			m_pAnimController = newController;
		}

		// if we are inserting after the end of the list, simply append this member
		if (insertBefore >= animationCount)
		{
			m_pAnimController->RegisterAnimationSet(animSet);
		}
		else
		{
			// how many animation sets do we have to store
			// to perform the insert?
			const uint32 storageCount = animationCount - insertBefore;
			LPD3DXANIMATIONSET* pStorage = new LPD3DXANIMATIONSET[storageCount];

			// store and remove these animation sets
			uint32 count;
			for (count = 0; count<storageCount; ++count)
			{
				uint32 iStorage = insertBefore + count;
				m_pAnimController->GetAnimationSet(iStorage, &pStorage[count]);
			}

			for (count = 0; count<storageCount; ++count)
			{
				m_pAnimController->UnregisterAnimationSet(pStorage[count]);
			}

			// add our new animation
			m_pAnimController->RegisterAnimationSet(animSet);

			// now replace the temporary storage copies
			for (count = 0; count<storageCount; ++count)
			{
				uint32 iStorage = insertBefore + count;
				m_pAnimController->RegisterAnimationSet(pStorage[count]);
				safe_release(pStorage[count]);
			}

			safe_delete_array(pStorage);
		}
	}
}

void cModelResource::moveAnimation(uint32 indexFrom, uint32 indexTo)
{
	debug_assert(indexFrom < totalAnimations(), "invalid animation slot");

	if (m_pAnimController)
	{
		LPD3DXANIMATIONSET pAnimationSet = 0;
		m_pAnimController->GetAnimationSet(indexFrom, &pAnimationSet);
		m_pAnimController->UnregisterAnimationSet(pAnimationSet);

		// re-insert at the new location
		insertAnimation(indexTo, pAnimationSet);
		safe_release(pAnimationSet);
	}
}

void cModelResource::addAnimations(LPD3DXANIMATIONCONTROLLER pExternalController)
{
	debug_assert(pExternalController, "invalid animation controller");
	
	if (m_pAnimController)
	{
		HRESULT hr;
		uint32 animationCount = totalAnimations();
		uint32 countToAdd = pExternalController->GetNumAnimationSets();
		uint32 maxCount = m_pAnimController->GetMaxNumAnimationSets();

		if (animationCount+countToAdd >= maxCount)
		{
			LPD3DXANIMATIONCONTROLLER newController;

			hr = m_pAnimController->CloneAnimationController(
				m_pAnimController->GetMaxNumMatrices(),
				animationCount+countToAdd,
				m_pAnimController->GetMaxNumTracks(),
				m_pAnimController->GetMaxNumEvents(),
				&newController
			);

			if (FAILED(hr))
			{
				return;
			}

			safe_release(m_pAnimController);
			m_pAnimController = newController;
		}

		uint32 iAnimSet;
		for (iAnimSet = 0; iAnimSet<countToAdd; ++iAnimSet)
		{
			LPD3DXANIMATIONSET pAnimationSet = 0;
			pExternalController->GetAnimationSet(iAnimSet, &pAnimationSet);
			m_pAnimController->RegisterAnimationSet(pAnimationSet);
			safe_release(pAnimationSet);
		}
	}
}

void cModelResource::removeAnimation(uint32 slot)
{
	debug_assert(slot < totalAnimations(), "invalid animation slot");
	const tchar* name=0;

	if (m_pAnimController)
	{
		LPD3DXANIMATIONSET pAnimationSet = 0;

		m_pAnimController->GetAnimationSet(slot, &pAnimationSet);

		if (pAnimationSet)
		{
			m_pAnimController->UnregisterAnimationSet(pAnimationSet);
			safe_release(pAnimationSet);
		}
	}
}

void cModelResource::renderModelSubset(int iFrame, int iSubset)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer= 
		(D3DXMESHCONTAINER_DERIVED*)frame(iFrame)->pMeshContainer;

	if (pMeshContainer)
	{
		if (pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
		{
			if (pMeshContainer->RenderMeshData.pMesh)
			{
				pMeshContainer->RenderMeshData.pMesh->DrawSubset(iSubset);
			}
		}
		else
		{
			if (pMeshContainer->RenderMeshData.pPMesh)
			{
				pMeshContainer->RenderMeshData.pPMesh->DrawSubset(iSubset);
			}
		}
	}
}

bool cModelResource::findFirstModelFrame(uint16& frame)const
{
	for (int i=0; i<m_totalFrames;++i)
	{
		D3DXFRAME_DERIVED* pFrame = m_pFrameList[i];
		if (pFrame->pMeshContainer)
		{
			frame = i;
			return true;
		}
	}
	return false;
}

bool cModelResource::findNextModelFrame(uint16& frame, uint16 lastFrame)const
{
	for (int i=lastFrame+1; i<m_totalFrames;++i)
	{
		D3DXFRAME_DERIVED* pFrame = m_pFrameList[i];
		if (pFrame->pMeshContainer)
		{
			frame = i;
			return true;
		}
	}
	return false;
}

void cModelResource::setLODScale(float zeroToOne)
{
	// skip if no progressive meshes were found
	if (!m_bProgressive) return;

	int8 newLOD = (int8)realToInt32(zeroToOne * (float)k_maxLOD);
	setLOD(newLOD);
}

void cModelResource::setLOD(int newLOD)
{
	// skip if no progressive meshes were found
	if (!m_bProgressive) return;

	newLOD = clamp(newLOD, (int)k_minLOD, (int)k_maxLOD);
	float detailScale = (float)newLOD/k_maxLOD;

	if (m_currentLOD != newLOD)
	{
		m_currentLOD = newLOD;

		// set the LOD for all attached Progressive Meshes
		for (int i=0; i<m_totalFrames;++i)
		{
			D3DXFRAME_DERIVED* pFrame = m_pFrameList[i];
			if (pFrame->pMeshContainer)
			{
				D3DXMESHCONTAINER_DERIVED* pMeshContainer = 
					(D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

				if (pMeshContainer->RenderMeshData.Type == D3DXMESHTYPE_PMESH)
				{
					LPD3DXPMESH pPMesh = 
						pMeshContainer->RenderMeshData.pPMesh;

					if (pPMesh)
					{
						uint32 minFaces = pPMesh->GetMinFaces();
						uint32 maxFaces = pPMesh->GetMaxFaces();
						float fDelta = (maxFaces - minFaces)*detailScale;
						uint32 faceCount = realToInt32(fDelta) + minFaces;

						pPMesh->SetNumFaces(faceCount);
					}
				}
			}
		}
	}
}

//***************************************************************
// end of file      ( cModelResource.cpp )

//----------------------------------------------------------
//$Log: $