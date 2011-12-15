#ifndef GAMEHOST_H
#define GAMEHOST_H
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
#include "gaia\display_manager.h"
#include "gaia\light_manager.h"
#include "gaia\camera.h"
#include "gaia\scene_node.h"
#include <D3DX9.h>
#include "gaia\framework\DXUtil.h"
#include "gaia\framework\D3DEnumeration.h"
#include "gaia\framework\D3DSettings.h"
#include "gaia\framework\D3DApp.h"
#include "gaia\framework\D3DFont.h"
#include "gaia\framework\D3DUtil.h"


#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <D3D9.h>

#ifndef STD_VECTOR_H
#define STD_VECTOR_H
#pragma warning(disable : 4786)
#include <vector>
#endif

//	Name Space declaration
namespace gaia
{


/*	cGameHost
------------------------------------------------------------------------------------------
	
	The Game Host object represents the complete host environment for an application.
	It contains the main program window, unless one is provided externally

	The cGameHost is declared as a singleton, so it can be globally accessed by the
	application using the defined title 'TheGameHost'. The application must create exactly
	one instance of the cGameHost object. Any additional cGameHost objects will cause
	the compile or run-time to fail.
	
------------------------------------------------------------------------------------------
*/
#define TheGameHost cGameHost::instance()
#define DisplayManager cGameHost::instance().displayManager()
#define LightManager cGameHost::instance().lightManager()

#define k_millisecondsPerTick 0.0333f
class cTerrain;

class cGameHost : public CD3DApplication, public cSingleton<cGameHost>
{
public:

	// Creators...
    cGameHost();
    ~cGameHost();

	// Mutators...
	const cSystemInfo& systemInfo()const;
	cResourcePoolManager& resourceManager();
	cDisplayManager& displayManager();
	cLightManager& lightManager();

	void setupWorldQuadTree(const cRect3d& worldExtents);

    const D3DPRESENT_PARAMETERS& presentParams()const;  // Parameters for CreateDevice/Reset
    HWND  appWindow()const;            // The main app window
    HWND  focusWindow()const;          // The D3D focus window (usually same as m_hWnd)
 	LPDIRECT3D9 d3dInterface()const;
    LPDIRECT3DDEVICE9 d3dDevice()const;   // The D3D rendering device
    const D3DCAPS9& d3dCaps()const;    // Caps for the device
    const D3DSURFACE_DESC& d3dsdBackBuffer()const; // Surface desc of the backbuffer
    const RECT& rcWindowClient()const; // Saved client area size for mode switches
	const TCHAR* windowTitle()const;
	bool hardwareVertexShadersAllowed()const;
	void setActiveCamera(cCamera* pCamera);
	void beginRenderStage(uint8 stage);
	void endRenderStage();

	const tchar* rootPath()const {return m_rootPath;}
	cCamera& defaultCamera() {return m_defaultCamera;}
	cCamera* activeCamera() {return m_pActiveCamera;}
	cSceneNode& rootNode() {return m_rootNode;}
	cQuadTree& quadTree() {return m_quadTree;}
	cRect3d& worldExtents() {return m_worldExtents;}

	uint32 currentRenderStage()const {return m_activeRenderStage;}

protected:

	virtual HRESULT OneTimeSceneInit();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT Render();
	virtual HRESULT FrameMove();
	virtual HRESULT FinalCleanup();
    virtual HRESULT ConfirmDevice(
		D3DCAPS9* pCaps,
		DWORD behavior,
		D3DFORMAT display,
		D3DFORMAT backbuffer);

	virtual HRESULT updateScene();
	virtual HRESULT renderScene();

	// helper function to provide basic user input
	virtual void updateCamera(
				float lateralSpeed, 
				float rotationSpeed, 
				cTerrain* pTerrain = 0,
				float groundOffset = 0,
				bool forceUpdate = 0);

private:

	// Private Data...
	cSystemInfo m_systemInfo;
	CD3DFont* m_pFont;                    
	float	m_updateTimeCount;
	uint32 m_activeRenderStage;

	cResourcePoolManager	m_resourceManager;
	cDisplayManager		m_displayManager;
	cLightManager		m_lightManager;

	cSceneNode		m_rootNode;
	cCamera			m_defaultCamera;
	cCamera*		m_pActiveCamera;
	cQuadTree		m_quadTree;
	cRect3d			m_worldExtents;

	tchar	m_rootPath[MAX_PATH];

	bool	m_frameReady;

	// Invalid Functions...
    cGameHost(const cGameHost& Src);
    cGameHost& operator=(const cGameHost& Src);
};

//- Inline Functions --------------------------------------------

/*	cGameHost
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/
inline cGameHost::cGameHost()
:cSingleton<cGameHost>(*this)
,CD3DApplication()
,m_pFont(0)
,m_updateTimeCount(0.0f)
,m_worldExtents(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f)
,m_activeRenderStage(0)
,m_pActiveCamera(&m_defaultCamera)
,m_frameReady(false)
{
   m_strWindowTitle = _T("Gaia Engine");
   m_d3dEnumeration.AppUsesDepthBuffer = TRUE;
   m_bShowCursorWhenFullscreen      = FALSE;
   m_bStartFullscreen               = FALSE;

   m_dwCreationWidth    = 800;
   m_dwCreationHeight   = 600;


   m_d3dEnumeration.AppMinDepthBits = 24;

   GetCurrentDirectory(MAX_PATH, m_rootPath);
   debug_assert(m_systemInfo.cpuFlags().testBit(cSystemInfo::SUPPORTS_SSE), "No SSE Support Found");
}

/*	~cGameHost
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cGameHost::~cGameHost()
{
}

inline const cSystemInfo& cGameHost::systemInfo()const 
{
	return m_systemInfo;
}

inline cResourcePoolManager& cGameHost::resourceManager()
{
	return m_resourceManager;
}

inline cDisplayManager& cGameHost::displayManager() 
{
	return m_displayManager;
}

inline cLightManager& cGameHost::lightManager() 
{
	return m_lightManager;
}

inline const D3DPRESENT_PARAMETERS& cGameHost::presentParams()const
{
	return m_d3dpp;
}

inline HWND  cGameHost::appWindow()const
{
	return m_hWnd;
}

inline HWND  cGameHost::focusWindow()const
{
	return m_hWndFocus;
}

inline LPDIRECT3D9 cGameHost::d3dInterface()const
{
	return m_pD3D;
}

inline LPDIRECT3DDEVICE9 cGameHost::d3dDevice()const
{
	return m_pd3dDevice;
}

inline const D3DCAPS9& cGameHost::d3dCaps()const
{
	return m_d3dCaps;
}

inline const D3DSURFACE_DESC& cGameHost::d3dsdBackBuffer()const
{
	return m_d3dsdBackBuffer;
}

inline const RECT& cGameHost::rcWindowClient()const
{
	return m_rcWindowClient;
}

inline const TCHAR* cGameHost::windowTitle()const
{
	return m_strWindowTitle;
}

inline bool cGameHost::hardwareVertexShadersAllowed()const
{
	return m_dwCreateFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING
		|| m_dwCreateFlags & D3DCREATE_MIXED_VERTEXPROCESSING;
}


//- End of cGameHost ---------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cGameHost.h )

//----------------------------------------------------------
//$Log: $