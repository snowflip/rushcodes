#define CSKYBOX_CPP
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

#include "sky_model.h"
#include "game_host.h"
#include "camera.h"
#include "render_method.h"
#include "effect_file.h"
#include "surface_material.h"

using namespace gaia;

static D3DVERTEXELEMENT9 vertex_description[]=
{
	// local data (stream 0)
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	D3DDECL_END()
};

void cSkyModel::create(eModelType type)
{
	safe_release(m_pMesh);

	DWORD g_dwNumMaterials;

	m_type = type;
	cString modelPath;

	switch (m_type)
	{
		case k_box:
			modelPath = "media\\models\\unit_cube.x";
			break;

		case k_sphere:
			modelPath = "media\\models\\unit_sphere.x";
			break;

		case k_hemisphere:
			modelPath = "media\\models\\unit_hemisphere.x";
			break;

		case k_dome:
			modelPath = "media\\models\\unit_dome.x";
			break;
	}

	if( FAILED( D3DXLoadMeshFromX(
		modelPath.c_str(), 
		D3DXMESH_MANAGED, 
		TheGameHost.d3dDevice(), 
		NULL, 
		NULL, 
		NULL,
		&g_dwNumMaterials, 
		&m_pMesh ) ) )
	{
		MessageBox(NULL, "Could not find sky model",
				   "Meshes.exe", MB_OK);
	}

	m_uvScaleOffset.set(0.0f,0.0f,0.0f,0.0f);
}

void cSkyModel::destroy()
{
	safe_release(m_pMesh);
	safe_release(m_pRenderMethod);
}

void cSkyModel::setRenderMethod(cRenderMethod* pMethod)
{
	safe_release(m_pRenderMethod);
	m_pRenderMethod = pMethod;

	if (m_pRenderMethod)
	{
		m_pRenderMethod->AddRef();
	}

}

void cSkyModel::update()
{
	m_uvScaleOffset.x += 0.00025f;
	m_uvScaleOffset.y += 0.00035f;
	m_uvScaleOffset.z += 0.0004f;
	m_uvScaleOffset.w += 0.0005f;
}

void cSkyModel::render()
{
	if (m_pRenderMethod && m_pMesh)
	{
		cEffectFile* pEffectFile = 
			m_pRenderMethod->getActiveEffect();
		cSurfaceMaterial* pSurfaceMaterial = 
			m_pRenderMethod->getActiveMaterial();

		if (pEffectFile)
		{
			// unlike regular objects, we do not render through the render
			// queue. Instead, we render on-demand using our internal settings
			if (pEffectFile->begin())
			{
				const cCamera* pCamera = TheGameHost.activeCamera();

				// supply our matrix to the render method
				pEffectFile->setMatrix(
					cEffectFile::k_worldViewProjMatrix, 
					&pCamera->skyBoxMatrix());

				// supply our material to the render method
				pEffectFile->applySurfaceMaterial(
					pSurfaceMaterial);

				pEffectFile->setParameter(
					cEffectFile::k_uvScaleOffset,
					(D3DXVECTOR4*)&m_uvScaleOffset);

				HRESULT hr;
				int totalPasses = pEffectFile->totalPasses();
				for (int i=0; i<totalPasses; ++i)
				{
					pEffectFile->activatePass(i);
					hr = m_pMesh->DrawSubset(0);
				}
				pEffectFile->end();
			}
		}
	}
}

//***************************************************************
// end of file      ( cSkyModel.cpp )

//----------------------------------------------------------
//$Log: $