
#include "gaia\gaia.h"
#include "resource.h"

#include "gaia\plant_manager.h"

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
	cTexture* m_pBlendMap;
	cTexture* m_pBumpMask;

	cRenderMethod* m_pRenderMethod;
	cSurfaceMaterial* m_pColorMaterial;
	cSurfaceMaterial* m_pBumpMaterial;

	cSkyModel m_skyDome;
	cSkyModel m_cloudLayer;
	cTexture* m_pSkyClouds;
	cRenderMethod* m_pSkyMethod;
	cRenderMethod* m_pCloudMethod;
	cSurfaceMaterial* m_pSkyMaterial;

	cLensFlare m_lensFlare;

	cPlantManager m_plantManagerA;
	cPlantManager m_plantManagerB;
	cGrassManager m_grassManager;
	cModelResource* m_pTreeA;
	cModelResource* m_pTreeB;
	cModelResource* m_pGrass;

protected:
    HRESULT ConfirmDevice(
		D3DCAPS9* pCaps,
		DWORD behavior,
		D3DFORMAT display,
		D3DFORMAT backbuffer);
};

HRESULT cMyHost::OneTimeSceneInit() 
{
	setupWorldQuadTree(
		cRect3d(-4000.0f,
				4000.0f,
				-4000.0f,
				4000.0f,
				-500.0f,
				1500.0f));
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
		256, 256, 
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
		0, "media\\shaders\\simple_terrain.fx");
	m_pRenderMethod->loadEffect(
		1, "media\\shaders\\simple_terrain_ambient.fx");
	m_pRenderMethod->loadEffect(
		2, "media\\shaders\\simple_terrain_bump.fx");
	m_pRenderMethod->loadEffect(
		3, "media\\shaders\\simple_terrain_sunlight.fx");

	// generate three elevation structures
    cTerrain::elevationData elevation[3];

	// grass (all elevations and slopes)
	elevation[0].minElevation = -500;
	elevation[0].maxElevation = 1000;
	elevation[0].minNormalZ = -1.0f;
	elevation[0].maxNormalZ = 1.0f;
	elevation[0].strength = 1.0f;

	// rock (all elevations, steep slopes)
	elevation[1].minElevation = 0;
	elevation[1].maxElevation = 1500;
	elevation[1].minNormalZ = 0.0f;
	elevation[1].maxNormalZ = 0.85f;
	elevation[1].strength = 10.0f;

	// dirt (high elevation, flat slope)
	elevation[2].minElevation = 800;
	elevation[2].maxElevation = 1500;
	elevation[2].minNormalZ = 0.75f;
	elevation[2].maxNormalZ = 1.0f;
	elevation[2].strength = 20.0f;

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
		elevation, 3);

	pBlendImage->randomChannelNoise(3, 200, 255);

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

	// now convert the blend image to
	// a mask for bump-mapping. First, clear
	// the alpha channel back to zero
	pBlendImage->setChannel(3,0);
	pBlendImage->convertToBumpMask();

	// create the bump mask 
	// texture and upload
	m_pBumpMask = 
		displayManager()
		.texturePool()
		.createResource(cString("image mask"));
	
	m_pBumpMask->createTexture(
		256, 256, 
		1, 0, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED);

	m_pBumpMask->uploadImage(pBlendImage);

	// we are now done with the blend image
	safe_release(pBlendImage);

	// load the ground surface materials

	// create a surface material
	// and load our textures into it
	m_pColorMaterial = 
		displayManager()
		.surfaceMaterialPool()
		.loadResource("media\\materials\\terrain.mat");
	m_pColorMaterial->setTexture(0, m_pBlendMap);

	m_pBumpMaterial = 
		displayManager()
		.surfaceMaterialPool()
		.loadResource("media\\materials\\terrain_bump.mat");
	m_pBumpMaterial->setTexture(0, m_pBumpMask);

	m_pRenderMethod->setMaterial(
		0, m_pColorMaterial);
	m_pRenderMethod->setMaterial(
		1, m_pColorMaterial);
	m_pRenderMethod->setMaterial(
		2, m_pBumpMaterial);
	m_pRenderMethod->setMaterial(
		3, m_pColorMaterial);

	// give the final render method to the terrain
	m_terrainSystem.setRenderMethod(m_pRenderMethod);

	// generate the sky box
	m_skyDome.create(cSkyModel::k_hemisphere);
	m_cloudLayer.create(cSkyModel::k_dome);

	m_pSkyMethod = 
		TheGameHost
		.displayManager()
		.renderMethodPool()
		.createResource("sky box method");
	m_pSkyMethod->loadEffect(
		cRenderMethod::k_defaultMethod,
		"media\\shaders\\sunlit_skydome.fx");

	
	m_pCloudMethod = 
		TheGameHost
		.displayManager()
		.renderMethodPool()
		.createResource("sky cloud method");
	m_pCloudMethod->loadEffect(
		cRenderMethod::k_defaultMethod,
		"media\\shaders\\animated_clouds.fx");

	m_pSkyClouds = 
		TheGameHost
		.displayManager()
		.texturePool()
		.createResource("sky cloud texture");
	m_pSkyClouds->loadResource("media\\textures\\clouds.dds");

	m_pSkyMaterial = 
		TheGameHost
		.displayManager()
		.surfaceMaterialPool()
		.createResource("sky box material");
	m_pSkyMaterial->setTexture(0, m_pSkyClouds);

	m_pCloudMethod->setMaterial(
		0, m_pSkyMaterial);
	
	m_skyDome.setRenderMethod(m_pSkyMethod);
	m_cloudLayer.setRenderMethod(m_pCloudMethod);

	// force-update the camera to make sure
	// we start above the terrain
	defaultCamera().update();
	updateCamera(
		10.0f, 
		0.1f, 
		&m_terrainSystem,
		75.0f, 
		true); //<- true forces an update


	m_lensFlare.create();


	m_pTreeA = 	TheGameHost
		.displayManager()
		.modelPool()
		.createResource("tree type A");
	m_pTreeA->loadResource(
		"media\\models\\pine_tree.x");


	m_plantManagerA.create(
		&m_terrainSystem,
		m_pTreeA,
		64,
		25.0f,
		800.0f,
		0.5f,
		2.0f,
		2);

	m_pTreeB = 	TheGameHost
		.displayManager()
		.modelPool()
		.createResource("tree type B");
	m_pTreeB->loadResource(
		"media\\models\\large_tree.x");


	m_plantManagerB.create(
		&m_terrainSystem,
		m_pTreeB,
		64,
		25.0f,
		800.0f,
		0.5f,
		2.0f,
		4);


	m_pGrass = 	TheGameHost
		.displayManager()
		.modelPool()
		.createResource("grass patch");
	m_pGrass->loadResource(
		"media\\models\\grass_patch.x");


	m_grassManager.create(
		&m_terrainSystem,
		m_pGrass,
		256,
		25.0f,
		800.0f,
		0.5f,
		2.0f,
		4);


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
	m_skyDome.destroy();
	m_cloudLayer.destroy();

	safe_release(m_pHeightMap);
	safe_release(m_pBlendMap);
	safe_release(m_pBumpMask);
	safe_release(m_pRenderMethod);
	safe_release(m_pColorMaterial);
	safe_release(m_pBumpMaterial);

	safe_release(m_pSkyClouds);
	safe_release(m_pSkyMethod);
	safe_release(m_pCloudMethod);
	safe_release(m_pSkyMaterial);
	safe_release(m_pTreeA);
	safe_release(m_pTreeB);
	safe_release(m_pGrass);

	m_lensFlare.destroy();
	m_plantManagerA.destroy();
	m_plantManagerB.destroy();
	m_grassManager.destroy();
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
		75.0f, 
		false);


	// call the UI function of the terrain
	m_terrainSystem.readUserInput();

	m_skyDome.update();
	m_cloudLayer.update();

	m_lensFlare.update(lightManager().sunVector());

	return cGameHost::updateScene();
}

HRESULT cMyHost::renderScene()
{
	if (activeCamera())
	{
		profile_scope(scene_render);

		beginRenderStage(0);
		m_skyDome.render();
		m_cloudLayer.render();
		endRenderStage();

		cSceneObject* pFirstMember = 
			quadTree().buildSearchResults(
				activeCamera()->searchRect(),
				&activeCamera()->worldFrustum());

		cSceneObject* pRenderList = pFirstMember;

		// prepare all objects for rendering
		while(pFirstMember)
		{
			pFirstMember->prepareForRender();
			pFirstMember = pFirstMember->nextSearchLink();
		}



		for (int stage=1; stage<4;++stage)
		{
			beginRenderStage(stage);
			// render all objects (puts them in the render queue)
			pFirstMember = pRenderList;
			while(pFirstMember)
			{
				pFirstMember->render();
				pFirstMember = pFirstMember->nextSearchLink();
			}

			// process the contents of the render queue
			displayManager().processRenderQueue();
			endRenderStage();
		}

		// now render the lens flare on top
		beginRenderStage(0);
		m_lensFlare.render();
		endRenderStage();
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

HRESULT cMyHost::ConfirmDevice(
		D3DCAPS9* pCaps,
		DWORD behavior,
		D3DFORMAT display,
		D3DFORMAT backbuffer)
{
     // we require an alpha channel in the backbuffer
	 if (backbuffer != D3DFMT_A8R8G8B8)
	 {
		return E_FAIL;
	 }

	// allow the base class to continue 
	// verification of the device
	return cGameHost::ConfirmDevice(
	   pCaps, 
	   behavior, 
	   display, 
	   backbuffer );
}
