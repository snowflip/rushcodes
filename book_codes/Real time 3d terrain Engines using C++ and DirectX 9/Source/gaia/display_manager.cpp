#define DISPLAYMANAGER_CPP
#define CORE_DLL
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

#include "display_manager.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "game_host.h"
#include "error.h"

using namespace gaia;

bool cDisplayManager::initialize()
{
	registerDeviceResources();
	return true;
}

void cDisplayManager::destroy()
{
	unregisterDeviceResources();
}

void cDisplayManager::clearRenderSurface()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	const D3DPRESENT_PARAMETERS& pd3dpp = TheGameHost.presentParams();

	if (pd3dDevice)
	{

		DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;

		if (pd3dpp.AutoDepthStencilFormat == D3DFMT_D15S1 
			|| pd3dpp.AutoDepthStencilFormat == D3DFMT_D24S8 
			|| pd3dpp.AutoDepthStencilFormat == D3DFMT_D24X4S4 )
		{
			flags |= D3DCLEAR_STENCIL;
		}

		pd3dDevice->Clear( 0, NULL, flags, D3DCOLOR_ARGB(0,127,127,127), 1.0f, 0 );
	}
}

void cDisplayManager::clearDepthBuffer()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	const D3DPRESENT_PARAMETERS& pd3dpp = TheGameHost.presentParams();

	if (pd3dDevice)
	{
		DWORD flags = D3DCLEAR_ZBUFFER;

		if (pd3dpp.AutoDepthStencilFormat == D3DFMT_D15S1 
			|| pd3dpp.AutoDepthStencilFormat == D3DFMT_D24S8 
			|| pd3dpp.AutoDepthStencilFormat == D3DFMT_D24X4S4 )
		{
			flags |= D3DCLEAR_STENCIL;
		}

		pd3dDevice->Clear( 0, NULL, flags, 0, 1.0f, 0 );
	}
}

void cDisplayManager::clearScreen()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();

	if (pd3dDevice)
	{
		// clear the backbuffer
		clearRenderSurface();

		// display it
		pd3dDevice->Present( 0, 0, 0, 0 );
	}
}


void cDisplayManager::registerDeviceResources()
{
	debug_print( "Registering Video Device Resources\n");
	m_texturePool.registerResourcePool(cResourceCode(k_nVideoResource, k_nTextureResource));
	m_vertexBufferPool.registerResourcePool(cResourceCode(k_nVideoResource, k_nVBufferResource));
	m_indexBufferPool.registerResourcePool(cResourceCode(k_nVideoResource, k_nIBufferResource));
	m_effectFilePool.registerResourcePool(cResourceCode(k_nVideoResource, k_nRenderResource));
	m_renderMethodPool.registerResourcePool(cResourceCode(k_nVideoResource, k_nRenderSetResource));
	m_modelPool.registerResourcePool(cResourceCode(k_nVideoResource, k_nModelResource));
	m_imagePool.registerResourcePool(cResourceCode(k_nVideoResource, k_nImageResource));
	m_surfaceMaterialPool.registerResourcePool(cResourceCode(k_nVideoResource, k_nSMaterialResource));
	
}

void cDisplayManager::unregisterDeviceResources()
{
	debug_print( "Unregistering Video Device Resources\n");
	m_texturePool.unregisterResourcePool();
	m_vertexBufferPool.unregisterResourcePool();
	m_indexBufferPool.unregisterResourcePool();
	m_effectFilePool.unregisterResourcePool();
	m_renderMethodPool.unregisterResourcePool();
	m_modelPool.unregisterResourcePool();
	m_imagePool.unregisterResourcePool();
	m_surfaceMaterialPool.unregisterResourcePool();
}

uint32 cDisplayManager::createDeviceResources()                        
{ 
	m_pFontTexture = m_texturePool.loadResource("media\\textures\\font.tga");

	return GAIA_OK; 
}

uint32 cDisplayManager::destroyDeviceResources()                        
{ 
	// first disable...
	disableDeviceResources();

	safe_release(m_pFontTexture);

	// then destroy...
	debug_print( "Destroying Video Device Resources\n");
	ResourceManager.destroyResourceFamily(k_nVideoResource);

	return GAIA_OK; 
}

uint32 cDisplayManager::restoreDeviceResources()                        
{ 
	debug_print( "Restoring Video Device Resources\n");
	ResourceManager.restoreResourceFamily(k_nVideoResource);
	return GAIA_OK; 
}

uint32 cDisplayManager::disableDeviceResources()                       
{ 
	debug_print( "Disabling Video Device Resources\n");
	ResourceManager.disableResourceFamily(k_nVideoResource);
	return GAIA_OK; 
}


bool cDisplayManager::beginRenderSession()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	// set the default render window
	cRect3d renderWindowRect(0.0f,1.0f, 0.0f,1.0f,0.0f,1.0f);
	setRenderWindow(renderWindowRect);

	// clear screen if requested
	if (m_bClearEachFrame)
	{
		clearRenderSurface();
	}

	setDefaultRenderStates();
	m_renderQueue.reset();

	return SUCCEEDED(pd3dDevice->BeginScene());
}


bool cDisplayManager::setRenderWindow(const cRect3d& windowRectangle)
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	const D3DPRESENT_PARAMETERS& pd3dpp = TheGameHost.presentParams();

	cVector2 windowSize((float)pd3dpp.BackBufferWidth, (float)pd3dpp.BackBufferHeight);
	cRect2d realPixelRect = windowRectangle.rect2d * windowSize;
	RECT dwordPixelRect = { realToInt32_chop(realPixelRect.x0), 
							realToInt32_chop(realPixelRect.y0), 
							realToInt32_chop(realPixelRect.x1), 
							realToInt32_chop(realPixelRect.y1)};

	D3DVIEWPORT9 viewData = 
	{ 
		dwordPixelRect.left, 
		dwordPixelRect.top, 
		dwordPixelRect.right-dwordPixelRect.left, 
		dwordPixelRect.bottom-dwordPixelRect.top, 
		windowRectangle.z0,
		windowRectangle.z1
	};

	HRESULT hr = pd3dDevice->SetViewport(&viewData);
	if(FAILED(hr))
	{
		debug_error(hr);
		return false;
	}
	return (true);
}

bool cDisplayManager::endRenderSession()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	//
	// End Scene
	//
	
	pd3dDevice->EndScene();

	return(true);
}

void cDisplayManager::setDefaultRenderStates()
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();

	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE );

	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

 	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
 	pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE );

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x02020202);

	pd3dDevice->SetRenderState( D3DRS_COLORVERTEX ,   FALSE );

	pd3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);	
	pd3dDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE , D3DMCS_MATERIAL);	
	pd3dDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE  , D3DMCS_MATERIAL);	
	pd3dDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE , D3DMCS_MATERIAL);
	
	// initialize the world transform matrix as the identity
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	pd3dDevice->SetTransform(D3DTS_WORLD , (D3DMATRIX*)&matrix);
	pd3dDevice->SetTransform(D3DTS_VIEW , (D3DMATRIX*)&matrix);
	pd3dDevice->SetTransform(D3DTS_PROJECTION  , (D3DMATRIX*)&matrix);
}

void cDisplayManager::drawString(uint32 x, uint32 y, const char* String)
{
	LPDIRECT3DDEVICE9 pd3dDevice = TheGameHost.d3dDevice();
	struct TLVERTEX
	{
		FLOAT    x, y, z, rhw;
		D3DCOLOR specular, diffuse;
		FLOAT    tu, tv;
	};

	const DWORD VertexFVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |
							 D3DFVF_SPECULAR | D3DFVF_TEX1 );
	TLVERTEX TLVertex[4];
	uint32 MaxX = 800;
	uint32 MaxY = 600;
	
	uint32 PolySize=MaxX/80;
	uint32 CurrentHandle=NULL;

	// make sure innitial values will be on screen
	if ((y>=MaxY) || (y+PolySize>=MaxY) 
		|| (x>=MaxX) || (x+PolySize>=MaxX))
	{
		return;
	}

	// First square
	TLVertex[0].x = (float)x;
	TLVertex[0].y = (float)y+PolySize;
	TLVertex[0].z = 0.0f;
	TLVertex[0].rhw = 1.0f;
	TLVertex[0].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
	TLVertex[0].specular = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);

	TLVertex[1].x = (float)x+PolySize;
	TLVertex[1].y = (float)y+PolySize;
	TLVertex[1].z = 0.0f;
	TLVertex[1].rhw = 1.0f;
	TLVertex[1].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
	TLVertex[1].specular = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);

	TLVertex[2].x = (float)x;
	TLVertex[2].y = (float)y;
	TLVertex[2].z = 0.0f;
	TLVertex[2].rhw = 1.0f;
	TLVertex[2].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
	TLVertex[2].specular = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);

	TLVertex[3].x = (float)x+PolySize;
	TLVertex[3].y = (float)y;
	TLVertex[3].z = 0.0f;
	TLVertex[3].rhw = 1.0f;
	TLVertex[3].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
	TLVertex[3].specular = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);


	pd3dDevice->SetTexture( 0, m_pFontTexture->getTexture());

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	pd3dDevice->SetVertexShader(0);
	pd3dDevice->SetFVF(VertexFVF);

	char* pText = (char*)String;
	int TotalLetters = lstrlen(String);

	for (int i=0;i<TotalLetters;++i)
	{
		char chr=*(pText++);

		// if we are in range
		if (TLVertex[3].x < MaxX)
		{
			if( chr>='a' && chr<='z' )
				chr-=('a'-'A');

			if( chr>32 && chr<96 )
			{
				float xpos,ypos;

				chr-=32;
				xpos=(float)(chr&7);
				ypos=(float)((chr>>3)&7);
				
				float step = 1.0f/8.0f;
				xpos = xpos* step;
				ypos = 1.0f - (ypos* step);

				TLVertex[0].tu =(float)xpos;
				TLVertex[0].tv =(float)ypos - step;
				TLVertex[1].tu =(float)xpos + step;
				TLVertex[1].tv =(float)ypos - step;
				TLVertex[2].tu =(float)xpos;
				TLVertex[2].tv =(float)ypos;
				TLVertex[3].tu =(float)xpos + step;
				TLVertex[3].tv =(float)ypos;

				pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &TLVertex[0], sizeof(TLVERTEX));
			}
		}

		// move to next poly
		TLVertex[0].x += PolySize;
		TLVertex[1].x += PolySize;
		TLVertex[2].x += PolySize;
		TLVertex[3].x += PolySize;
	}

	return;
}

cRenderEntry* cDisplayManager::openRenderQueue()
{
	return m_renderQueue.lockRenderEntry();
}

void cDisplayManager::closeRenderQueue(cRenderEntry* entry)
{
	m_renderQueue.unlockRenderEntry(entry);
}

void cDisplayManager::processRenderQueue()
{
	m_renderQueue.execute();
}

bool cDisplayManager::checkResourceFormatSupport(D3DFORMAT fmt, D3DRESOURCETYPE resType, DWORD dwUsage)
{
	bool result = true;
	LPDIRECT3D9 pD3D = TheGameHost.d3dInterface();
	const D3DCAPS9& devCaps = TheGameHost.d3dCaps();
	const D3DSURFACE_DESC& displayDesc = TheGameHost.d3dsdBackBuffer();
	
	if (FAILED(
		pD3D->CheckDeviceFormat(
		devCaps.AdapterOrdinal, 
		devCaps.DeviceType, 
		displayDesc.Format, 
		dwUsage, 
		resType, 
		fmt)))
	{
		result=false;
	}
	
	return result;
}


//***************************************************************
// end of file      ( cDisplayManager.cpp )

//----------------------------------------------------------
//$Log: $