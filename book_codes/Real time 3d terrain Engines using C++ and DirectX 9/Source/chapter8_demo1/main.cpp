
#include "gaia\gaia.h"
#include "resource.h"


using namespace gaia;

			
class cMyHost:public cGameHost
{
public:
	cMyHost(){};
	~cMyHost(){};

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT updateScene();
	HRESULT renderScene();
	HRESULT OneTimeSceneInit(); 

    LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	cTiledTerrain m_terrainSystem;
	cTexture* m_pHeightMap;

	cRenderMethod* m_pRenderMethod;
	cSurfaceMaterial* m_pSurfaceMaterial;

};

HRESULT cMyHost::OneTimeSceneInit() 
{
	setupWorldQuadTree(
		cRect3d(-500.0f,
				500.0f,
				-500.0f,
				500.0f,
				0.0f,
				500.0f));
	return cGameHost::OneTimeSceneInit();
}

HRESULT cMyHost::InitDeviceObjects()
{
	cGameHost::InitDeviceObjects();

	// generate a random height map
	m_pHeightMap = 
		displayManager()
		.texturePool()
		.createResource(cString("height map"));
	m_pHeightMap->createTexture(
		128, 128, 
		1, 0, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED);
	m_pHeightMap->generatePerlinNoise(
		0.01f, 5, 0.6f);

	// create a terrain from this map
	m_terrainSystem.create(
		&rootNode(), 
		m_pHeightMap, 
		worldExtents(), 
		3);

	// load our render method
	m_pRenderMethod = 
		TheGameHost
		.displayManager()
		.renderMethodPool()
		.createResource("terrain method");
	m_pRenderMethod->loadEffect(
		cRenderMethod::k_defaultMethod,
		"media\\shaders\\single_texture_terrain.fx");

	// create a surface material
	// and load our height map into it
	// as a texture
	m_pSurfaceMaterial = 
		displayManager()
		.surfaceMaterialPool()
		.createResource(cString("ground material"));
	m_pSurfaceMaterial->setTexture(0, m_pHeightMap);

	m_pRenderMethod->setMaterial(0, m_pSurfaceMaterial);

	// give the render method and material to the terrain
	m_terrainSystem.setRenderMethod(m_pRenderMethod);


	// force-update the camera to make sure
	// we start above the terrain
	defaultCamera().update();
	updateCamera(
		10.0f, 
		0.1f, 
		&m_terrainSystem,
		30.0f, 
		true); //<- true forces an update

	return S_OK;
}

HRESULT cMyHost::RestoreDeviceObjects()
{
	cGameHost::RestoreDeviceObjects();

	return S_OK;
}

HRESULT cMyHost::InvalidateDeviceObjects()
{
	cGameHost::InvalidateDeviceObjects();

	return S_OK;
}

HRESULT cMyHost::DeleteDeviceObjects()
{
	cGameHost::DeleteDeviceObjects();

	m_terrainSystem.destroy();

	safe_release(m_pHeightMap);
	safe_release(m_pRenderMethod);
	safe_release(m_pSurfaceMaterial);

	return S_OK;
}


HRESULT cMyHost::updateScene()
{
	profile_scope(scene_update);

	// call the basic camera UI 
	// function of cGameHost
	updateCamera(
		10.0f, 
		0.1f, 
		&m_terrainSystem,
		30.0f, 
		false);

	// call the UI function of the terrain
	m_terrainSystem.readUserInput();

	return cGameHost::updateScene();
}

HRESULT cMyHost::renderScene()
{
	profile_scope(scene_render);

	cSceneObject* pFirstMember = 
		quadTree().buildSearchResults(activeCamera()->searchRect());

	cSceneObject* pRenderList = pFirstMember;

	// prepare all objects for rendering
	while(pFirstMember)
	{
		pFirstMember->prepareForRender();
		pFirstMember = pFirstMember->nextSearchLink();
	}

	// render all objects (puts them in the render queue)
	pFirstMember = pRenderList;
	while(pFirstMember)
	{
		pFirstMember->render();
		pFirstMember = pFirstMember->nextSearchLink();
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT cMyHost::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam )
{
    // Pass remaining messages to default handler
    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}



//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	cMyHost d3dApp;

	srand(timeGetTime());

	//  InitCommonControls();
	if (FAILED (d3dApp.Create (hInst)))
	 return 0;

	INT result= d3dApp.Run();

	cCodeTimer::RootTimer.outputAllTimers(0xffffffff);

	return result;
}

