#define GAMEHOST_CPP
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

#include "game_host.h"
#include "d3dx9.h"
#include "error.h"
#include "terrain_system.h"

using namespace gaia;


void cGameHost::setupWorldQuadTree(const cRect3d& worldExtents)
{
	m_worldExtents = worldExtents;
	m_quadTree.create(m_worldExtents, 4);
}

HRESULT cGameHost::OneTimeSceneInit()
{
	// initialize managers
	if (m_displayManager.initialize())
	{
		m_pFont = new CD3DFont( _T("Arial"), 8, D3DFONT_BOLD );
		if (m_pFont)
		{
			return S_OK;
		}

	}
    return E_FAIL;
}

HRESULT cGameHost::InitDeviceObjects()
{
	m_pFont->InitDeviceObjects( m_pd3dDevice );
	m_displayManager.createDeviceResources();

	// create the root node 
	// for our scene and a camera

	m_rootNode.create();
	m_defaultCamera.create();
	m_defaultCamera.attachToParent(&m_rootNode);

	// setup the default scene camera
	m_defaultCamera.orientation().setPosition(
		cVector3(0.0f,0.0f,0.0f));
	m_defaultCamera.orientation().setRotation(
		cVector3(1.0f,0.0f,0.0f), 
		cVector3(0.0f,0.0f,1.0f));

	m_defaultCamera.setProjParams(
		D3DX_PI/3.0f, 
		800.0f/600.0f, 
		5.0f, 3000.0f);

	return S_OK;
}

HRESULT cGameHost::RestoreDeviceObjects()
{
	m_pFont->RestoreDeviceObjects();
	m_displayManager.restoreDeviceResources();
	return S_OK;
}

HRESULT cGameHost::InvalidateDeviceObjects()
{
   m_pFont->InvalidateDeviceObjects();
	m_displayManager.disableDeviceResources();
	return S_OK;
}

HRESULT cGameHost::DeleteDeviceObjects()
{
   m_pFont->DeleteDeviceObjects();
	m_displayManager.destroyDeviceResources();

	m_defaultCamera.destroy();
	m_rootNode.destroy();

	return S_OK;
}

HRESULT cGameHost::Render()
{
	if (!m_frameReady)
	{
		return S_OK;
	}

	if (m_displayManager.beginRenderSession())
	{
		cRect3d renderWindowRect(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);

		if (m_displayManager.setRenderWindow(renderWindowRect))
		{
			// render the scene
			renderScene();
		}

		// process the contents of the render queue
		m_displayManager.processRenderQueue();

        m_pFont->DrawText(   2,  0, D3DCOLOR_ARGB(255,0,0,0), m_strFrameStats );
        m_pFont->DrawText(   2, 20, D3DCOLOR_ARGB(255,0,0,0), m_strDeviceStats );

		m_displayManager.endRenderSession();
	}
	return S_OK;
}

HRESULT cGameHost::updateScene()
{
	// update our global lighting values
	m_lightManager.update();

	// update the scene attached to the root node
	m_rootNode.recursiveUpdate();

	m_frameReady = true;
	return S_OK;
}

HRESULT cGameHost::renderScene()
{
	m_pFont->DrawText (2, 10, D3DCOLOR_ARGB (128,255,255,255), "testing");
	return S_OK;
}

HRESULT cGameHost::FrameMove()
{
	static HANDLE hThread = GetCurrentThread();

	// add the real time elapsed to our
	// internal delay counter
	m_updateTimeCount += m_fElapsedTime;

	// is there an update ready to happen?
	if (m_updateTimeCount > k_millisecondsPerTick)
	{
		// bump up our priority for thread activation
		// while we are executing a game tick
		SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);

		// perform the proper number of updates
		while(m_updateTimeCount > k_millisecondsPerTick)
		{
			HRESULT result;

			// update the scene
			if(FAILED( result = updateScene()))
				return result;

			// subtract the time interval 
			// emulated with each tick
			m_updateTimeCount -= k_millisecondsPerTick;
		}

		// return to normal priority 
		// while we wait for the next tick
		SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);

		// we know our next logic execution will not occur
		// until a full tick worth of time has elapsed,
		// so we forefit the rest of the time slice
		// alotted to us by Windows and let any waiting 
		// threads execute. This prevents us from consuming
		// 100% of the CPU waiting for time to pass.
		Sleep(0);
	}

	return S_OK;
}

HRESULT cGameHost::FinalCleanup()
{
    SAFE_DELETE (m_pFont);

	// destroy managers
	m_displayManager.destroy();
	m_quadTree.destroy();
	return S_OK;
}

HRESULT cGameHost::ConfirmDevice(
		D3DCAPS9* pCaps,
		DWORD behavior,
		D3DFORMAT display,
		D3DFORMAT backbuffer)
{
   // Device must support ps_1_1...
   if( pCaps->PixelShaderVersion >= D3DPS_VERSION(1,1) )
   {
      return S_OK;
   }

   return E_FAIL;
}

void cGameHost::updateCamera(
	float lateralSpeed, 
	float rotationSpeed, 
	cTerrain* pTerrain,
	float groundOffset,
	bool forceUpdate)
{
	//
	// This function reads the keyboard
	// and moves the default camera
	//
	const cVector3& cameraUp = m_defaultCamera.orientation().getUp();

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) 
	{
		m_defaultCamera.orientation().addRotation(
			0.0f, 0.0f, 
			cameraUp.z >0.0f ? rotationSpeed: -rotationSpeed);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_defaultCamera.orientation().addRotation( 
			0.0f, 0.0f, 
			cameraUp.z >0.0f ? -rotationSpeed: rotationSpeed);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000) 
	{
		m_defaultCamera.orientation().addRelRotation(
			0.0f, rotationSpeed, 0.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_defaultCamera.orientation().addRelRotation(
			0.0f, -rotationSpeed, 0.0f);
	}

	cVector3 offset(0.0f,0.0f,0.0f);
	bool cameraMoved = false;

	if (GetAsyncKeyState('D') & 0x8000) 
	{
		offset.x = lateralSpeed;
		cameraMoved = true;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		offset.x = -lateralSpeed;
		cameraMoved = true;
	}

	if (GetAsyncKeyState('R') & 0x8000) 
	{
		offset.y = lateralSpeed;
		cameraMoved = true;
	}
	else if (GetAsyncKeyState('F') & 0x8000)
	{
		offset.y = -lateralSpeed;
		cameraMoved = true;
	}

	if (GetAsyncKeyState('W') & 0x8000) 
	{
		offset.z = lateralSpeed;
		cameraMoved = true;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		offset.z = -lateralSpeed;
		cameraMoved = true;
	}
	
	if(cameraMoved || forceUpdate)
	{
		// if a terrain was provided, make sure we are above it
		if (pTerrain)
		{
			cVector3 cameraWorldPos = m_defaultCamera.worldPosition();
			float groundLevel = pTerrain->calcWorldHeight(cameraWorldPos.x, cameraWorldPos.y);
			float minCameraZ = groundLevel + groundOffset;

			if (minCameraZ > cameraWorldPos.z)
			{
				offset.y = minCameraZ-cameraWorldPos.z;
			}
		}
		m_defaultCamera.orientation().addRelPosition(offset);
	}

}

void cGameHost::setActiveCamera(cCamera* pCamera)
{
	m_pActiveCamera = pCamera;

	if (!m_pActiveCamera)
	{
		m_pActiveCamera = &m_defaultCamera;
	}
}

void cGameHost::beginRenderStage(uint8 stage)
{
	debug_assert(
		stage < cRenderMethod::k_max_render_stages,
		"invalid render stage");

	m_activeRenderStage = stage;

	if (m_activeRenderStage == 
		cRenderMethod::k_bumpMapStage)
	{
		// during the bump map stage,
		// we write only to the alpha
		// channel of the destination
		d3dDevice()->SetRenderState(
			D3DRS_COLORWRITEENABLE, 
			D3DCOLORWRITEENABLE_ALPHA);	
	}
	else if (m_activeRenderStage == 
		cRenderMethod::k_lightingStage)
	{
		// during the lighting stage,
		// we write only to the color
		// channel of the destination
		d3dDevice()->SetRenderState(
			D3DRS_COLORWRITEENABLE, 
				D3DCOLORWRITEENABLE_BLUE |
				D3DCOLORWRITEENABLE_GREEN|
				D3DCOLORWRITEENABLE_RED);	
	}

	// to keep the results of each render stage sorting correctly
	// in the z-buffer, we apply a bias to the camera matrices for
	// each stage. We could also do this with the D3DRS_DEPTHBIAS
	// render state, but it is not widly supported
	m_pActiveCamera->applyRenderStageBias(stage);
}

void cGameHost::endRenderStage()
{
	if (m_activeRenderStage == 
		cRenderMethod::k_bumpMapStage)
	{
		// re-enable rendering to all
		// color channels
		d3dDevice()->SetRenderState(
			D3DRS_COLORWRITEENABLE, 
				D3DCOLORWRITEENABLE_ALPHA|
				D3DCOLORWRITEENABLE_BLUE |
				D3DCOLORWRITEENABLE_GREEN|
				D3DCOLORWRITEENABLE_RED);
	}

	m_activeRenderStage = 0;
}
//***************************************************************
// end of file      ( cGameHost.cpp )

//----------------------------------------------------------
//$Log: $