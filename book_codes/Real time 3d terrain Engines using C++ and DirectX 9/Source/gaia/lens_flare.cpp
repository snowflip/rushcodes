#define CLENSFLARE_CPP
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

#include "lens_flare.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "effect_file.h"
#include "surface_material.h"
#include "camera.h"
#include "game_host.h"

using namespace gaia;

struct flare_vertex
{
	cVector2 pos;
	cVector2 uv;
};

static D3DVERTEXELEMENT9 vertex_description[]=
{
	// local data (stream 0)
	{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};


void cLensFlare::create()
{
	// create a vertex buffer containing a simple quad
	cString tempName;
	tempName.format("lens_flare_quad");

	m_pVertexBuffer = 
		DisplayManager.vertexBufferPool().
		createResource(tempName);

	// we assume the texture page contains
	// a 3x3 grid of flare images. So,
	// define 4 vertices that use 
	// 1/3 of a texture. We can offset
	// the UVs in the pixel shader to use
	// the proper texture location
	flare_vertex quadVert[4];
	quadVert[0].pos.set(-1.0f,-1.0f);
	quadVert[0].uv.set(0.0f,0.0f);
	quadVert[1].pos.set(-1.0f,1.0f);
	quadVert[1].uv.set(0.0f,0.33f);
	quadVert[2].pos.set(1.0f,-1.0f);
	quadVert[2].uv.set(0.33f,0.0f);
	quadVert[3].pos.set(1.0f,1.0f);
	quadVert[3].uv.set(0.33f,0.33f);

	m_pVertexBuffer->create(
		4, 
		sizeof(flare_vertex), 
		0, quadVert);

	// create the vertex description
	m_pVertexBuffer->setVertexDescription(3,vertex_description);

	// create the index buffer used to draw the quad
	m_pIndexBuffer = 
		DisplayManager.indexBufferPool().
		createResource(tempName);

	uint16 index_list[4]={0,1,2,3};

	m_pIndexBuffer->create(
		D3DPT_TRIANGLESTRIP,
		4, 0, index_list);


	// load our effect file
	m_pEffectFile = 
		DisplayManager
		.effectFilePool()
		.createResource("lens_flare");
	m_pEffectFile->loadResource(
		"media\\shaders\\lens_flare.fx");

	// load the material
	m_pSurfaceMaterial = 
		DisplayManager
		.surfaceMaterialPool()
		.createResource(cString("lens_flare"));
	m_pSurfaceMaterial->loadResource(
		"media\\materials\\lens_flare.mat");

	// create the flare elements
	// the first is hard-coded
	m_flareElements[0].linePos = 1.0f;
	m_flareElements[0].scale = 0.25f;
	m_flareElements[0].brightness = 1.0f;
	m_flareElements[0].texU = 0.0f;
	m_flareElements[0].texV = 0.0f;

	float flareStep = 2.0f/k_totalFlares;
	float flarePos = 1.0f;

	int xPos = 0;
	int yPos = 0;

	// the rest are random
	for (int i=1; i<k_totalFlares; ++i)
	{
		flarePos -= flareStep;
		float posJitter = 
			RandomNumberInRange(-0.2f, 0.2f);

		m_flareElements[i].linePos =
			flarePos + posJitter;

		clamp(
			m_flareElements[i].linePos,
			-1.0f, 1.0f);

		m_flareElements[i].scale =
			RandomNumberInRange(0.3f, 2.5f);

		m_flareElements[i].scale *= 
			absoluteValue(m_flareElements[i].linePos);

		m_flareElements[i].brightness = 
			RandomNumberInRange(0.75f, 2.0f);

		++xPos;
		if (xPos>2)
		{
			xPos=0;
			++yPos;
		}
		if (yPos>2)
		{
			yPos=0;
		}

		m_flareElements[i].texU = 0.33f*xPos;
		m_flareElements[i].texV = 0.33f*yPos;
	}

	// okay, I lied. We force the last one to the
	// to the position opposite the sun
	m_flareElements[k_totalFlares-1].linePos = -1.0f;

}

void cLensFlare::destroy()
{
	safe_release(m_pSurfaceMaterial);
	safe_release(m_pEffectFile);
	safe_release(m_pVertexBuffer);
	safe_release(m_pIndexBuffer);
}

void cLensFlare::update(const cVector3& sunlightVector)
{
	cCamera* pCamera = TheGameHost.activeCamera();
	
	// compute a sun position relative to the camera
	const cVector3& camPos = pCamera->worldPosition();
	cVector3 sunPos(
		camPos.x - sunlightVector.x,
		camPos.y - sunlightVector.y,
		camPos.z - sunlightVector.z);

	// transform to screen coordinates
	const cSIMDMatrix& matrix = pCamera->viewProjMatrix();
	
	D3DXVec3TransformCoord(&m_screenPos, &sunPos, &matrix);

	// is the sun in view?
	if (m_screenPos.x > -1.2f 
		&& m_screenPos.x < 1.2f
		&& m_screenPos.y > -1.2f 
		&& m_screenPos.y < 1.2f
		&& m_screenPos.z > 0.0f)
	{
		m_visible = true;
	}
	else
	{
		m_visible = false;
	}

	// if in view, is it also high enough 
	// in the sky?
	if (m_visible)
	{
		m_flareColor = 
			TheGameHost.lightManager().sunColor();

		m_flareColor.w = 
			TheGameHost.lightManager().sunVector().z * 2.0f;

		m_flareColor.w -= 0.15f;
		m_flareColor.w = clamp(m_flareColor.w, 0.0f, 1.0f);

		float screen_scale = 1.0f - m_screenPos.vec2().length();
		screen_scale = clamp(screen_scale, 0.0f, 1.0f);
		m_flareColor.w *= screen_scale;

	}
}

void cLensFlare::render()
{
	if (m_visible)
	{
		const cCamera* pCamera = TheGameHost.activeCamera();

		// activate the effect
		m_pEffectFile->begin();
		m_pEffectFile->applySurfaceMaterial(m_pSurfaceMaterial);
		m_pEffectFile->setParameter(cEffectFile::k_lensFlareColor, m_flareColor);

		// activate the geometry buffers
		m_pIndexBuffer->activate();
		m_pVertexBuffer->activate(0, 0, true);

		int totalPasses = m_pEffectFile->totalPasses();
		LPDIRECT3DDEVICE9 d3dDevice = TheGameHost.d3dDevice();

		for (int iPass=0;
			iPass < totalPasses;
			++iPass)
		{
			m_pEffectFile->activatePass(iPass);

			// render each flare
			for (int i=0; i<k_totalFlares; ++i)
			{
				cVector4 posScaleOffset(
					m_flareElements[i].scale,
					m_flareElements[i].scale,
					m_screenPos.x * m_flareElements[i].linePos,
					m_screenPos.y * m_flareElements[i].linePos);

				cVector4 texScaleOffset(
					1.0f,
					1.0f,
					m_flareElements[i].texU,
					m_flareElements[i].texV);

				m_pEffectFile->setParameter(
					cEffectFile::k_posScaleOffset,
					&posScaleOffset);

				m_pEffectFile->setParameter(
					cEffectFile::k_uvScaleOffset,
					&texScaleOffset);

				// render!!!
				HRESULT hr = d3dDevice->DrawIndexedPrimitive(
					m_pIndexBuffer->primitiveType(),
					0,
					0,
					m_pVertexBuffer->count(),
					0,
					m_pIndexBuffer->primitiveCount());

			}
		}
		m_pEffectFile->end();
	}
}

//***************************************************************
// end of file      ( cLensFlare.cpp )

//----------------------------------------------------------
//$Log: $