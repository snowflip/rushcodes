
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

	cTerrain m_terrainSystem;
	cTexture* m_pHeightMap;
	cTexture* m_pBlendMap;
	cTexture* m_pGrass;
	cTexture* m_pRock;
	cTexture* m_pDirt;
	cTexture* m_pSnow;
	cTexture* m_pNoise;

	cRenderMethod* m_pRenderMethod;
	cSurfaceMaterial* m_pSurfaceMaterial;

	cSkyModel m_skyModel;
	cTexture* m_pSkyMap;
	cRenderMethod* m_pSkyMethod;
	cSurfaceMaterial* m_pSkyMaterial;

};

HRESULT cMyHost::OneTimeSceneInit() 
{
	setupWorldQuadTree(
		cRect3d(-2000.0f,
				2000.0f,
				-2000.0f,
				2000.0f,
				-750.0f,
				750.0f));
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
		0.015f, 5, 0.6f);

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
		"media\\shaders\\multipass_terrain.fx");

	// generate three elevation structures
    cTerrain::elevationData elevation[4];

	// grass (all elevations and slopes)
	elevation[0].minElevation = -750;
	elevation[0].maxElevation = 750;
	elevation[0].minNormalZ = -1.0f;
	elevation[0].maxNormalZ = 1.0f;
	elevation[0].strength = 1.0f;

	// rock (all elevations, steep slopes)
	elevation[1].minElevation = -750;
	elevation[1].maxElevation = 750;
	elevation[1].minNormalZ = 0.0f;
	elevation[1].maxNormalZ = 0.85f;
	elevation[1].strength = 10.0f;

	// dirt (low elevation, flat slope)
	elevation[2].minElevation = -750;
	elevation[2].maxElevation = 100;
	elevation[2].minNormalZ = 0.75f;
	elevation[2].maxNormalZ = 1.0f;
	elevation[2].strength = 5.0f;

	// snow (max elevation, flat slope)
	elevation[3].minElevation = 300;
	elevation[3].maxElevation = 750;
	elevation[3].minNormalZ = 0.75f;
	elevation[3].maxNormalZ = 1.0f;
	elevation[3].strength = 20.0f;

	// generate the blend image
	cImage* pBlendImage;
	pBlendImage = 
		displayManager()
		.imagePool()
		.createResource(cString("image map 3"));
	pBlendImage->create(
		256, 
		256, 
		cImage::k_32bit);

	m_terrainSystem.generateBlendImage(
		pBlendImage, 
		elevation, 4);

	// upload the blend image to a texture
	m_pBlendMap = 
		displayManager()
		.texturePool()
		.createResource(cString("image map"));
	
	m_pBlendMap->createTexture(
		256, 256, 
		1, 0, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED);

	m_pBlendMap->uploadImage(pBlendImage);
	safe_release(pBlendImage);

	// load the ground surface textures
	m_pGrass = 
		displayManager()
		.texturePool()
		.createResource(cString("grass"));
	m_pRock = 
		displayManager()
		.texturePool()
		.createResource(cString("rock"));
	m_pDirt = 
		displayManager()
		.texturePool()
		.createResource(cString("dirt"));
	m_pSnow = 
		displayManager()
		.texturePool()
		.createResource(cString("snow"));
	m_pNoise = 
		displayManager()
		.texturePool()
		.createResource(cString("noise"));

	m_pGrass->loadResource(
		"media\\textures\\grass.dds");
	m_pRock->loadResource(
		"media\\textures\\rock.dds");
	m_pDirt->loadResource(
		"media\\textures\\dirt.dds");
	m_pSnow->loadResource(
		"media\\textures\\snow.dds");
	m_pNoise->loadResource(
		"media\\textures\\noise.dds");

	// create a surface material
	// and load our textures into it
	m_pSurfaceMaterial = 
		displayManager()
		.surfaceMaterialPool()
		.createResource(cString("ground material"));
	m_pSurfaceMaterial->setTexture(0, m_pBlendMap);
	m_pSurfaceMaterial->setTexture(1, m_pGrass);
	m_pSurfaceMaterial->setTexture(2, m_pRock);
	m_pSurfaceMaterial->setTexture(3, m_pDirt);
	m_pSurfaceMaterial->setTexture(4, m_pSnow);
	m_pSurfaceMaterial->setTexture(5, m_pNoise);

	// give the render method and material to the terrain
	m_pRenderMethod->setMaterial(0, m_pSurfaceMaterial);
	m_terrainSystem.setRenderMethod(m_pRenderMethod);

	// generate the sky box
	m_skyModel.create(cSkyModel::k_box);

	m_pSkyMethod = 
		TheGameHost
		.displayManager()
		.renderMethodPool()
		.createResource("sky box method");
	m_pSkyMethod->loadEffect(
		cRenderMethod::k_defaultMethod,
		"media\\shaders\\simple_skybox.fx");

	m_pSkyMap = 
		TheGameHost
		.displayManager()
		.texturePool()
		.createResource("sky box texture");
	m_pSkyMap->loadResource("media\\textures\\sky_box1.dds");


	m_pSkyMaterial = 
		TheGameHost
		.displayManager()
		.surfaceMaterialPool()
		.createResource("sky box material");
	m_pSkyMaterial->setTexture(0, m_pSkyMap);
	
	m_pSkyMethod->setMaterial(0, m_pSkyMaterial);
	m_skyModel.setRenderMethod(m_pSkyMethod);

	// force-update the camera to make sure
	// we start above the terrain
	defaultCamera().update();
	updateCamera(
		10.0f, 
		0.1f, 
		&m_terrainSystem,
		30.0f, 
		true); //<- true forces an update

	// since we have a sky box, we can turn of clearing each frame
	DisplayManager.enableFrameClearing(false);

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
	m_skyModel.destroy();

	safe_release(m_pHeightMap);
	safe_release(m_pBlendMap);
	safe_release(m_pRenderMethod);
	safe_release(m_pSurfaceMaterial);

	safe_release(m_pGrass);
	safe_release(m_pRock);
	safe_release(m_pDirt);
	safe_release(m_pSnow);
	safe_release(m_pNoise);

	safe_release(m_pSkyMap);
	safe_release(m_pSkyMethod);
	safe_release(m_pSkyMaterial);

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

	m_skyModel.update();

	return cGameHost::updateScene();
}

HRESULT cMyHost::renderScene()
{
	profile_scope(scene_render);

	m_skyModel.render();

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

