#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
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
#include "geometry\geometry.h"

#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <D3D9.h>

#include <gaia\texture.h>
#include <gaia\vertex_buffer.h>
#include <gaia\index_buffer.h>
#include <gaia\effect_file.h>
#include <gaia\render_method.h>
#include <gaia\model_resource.h>
#include <gaia\image.h>
#include <gaia\surface_material.h>
#include <gaia\render_queue.h>

#ifndef STD_VECTOR_H
#define STD_VECTOR_H
#pragma warning(disable : 4786)
#include <vector>
#endif

//	Name Space declaration
namespace gaia
{


/*	cDisplayManager
-----------------------------------------------------------------
    
    the cDisplayManager manages the DirectX display
    
-----------------------------------------------------------------
*/

class cDisplayManager 
{
public:

	// Creators...

    cDisplayManager();
    ~cDisplayManager();

	// Mutators...
	bool initialize();
	void destroy();
	void enableFrameClearing(bool setting);

	void clearDepthBuffer();
	void clearRenderSurface();
	void clearScreen();

	bool beginRenderSession();
	bool setRenderWindow(const cRect3d& windowRectangle);
	bool endRenderSession();

	cRenderEntry* openRenderQueue();
	void closeRenderQueue(cRenderEntry* entry);
	void processRenderQueue();

	// testing
	void drawString(uint32 x, uint32 y, const char* String);
	void setDefaultRenderStates();

	void enable();
	void disable();

    uint32 createDeviceResources();
    uint32 destroyDeviceResources();
    uint32 disableDeviceResources();
    uint32 restoreDeviceResources();

	bool checkResourceFormatSupport(D3DFORMAT fmt, D3DRESOURCETYPE resType, DWORD dwUsage);

	// Accessors...
    bool clearingEachFrame()const;
	bool writeToTGA(const char* FileName, uint8 *pData, uint16 width, uint16 height, uint32 stride);

	cTextureManager&		texturePool() {return m_texturePool;}
	cSurfaceMaterialManager& surfaceMaterialPool() {return m_surfaceMaterialPool;}
	cVertexBufferManager&	vertexBufferPool() {return m_vertexBufferPool;}
	cIndexBufferManager&	indexBufferPool() {return m_indexBufferPool;}
	cEffectFileManager&	effectFilePool() {return m_effectFilePool;}
	cRenderMethodManager&	renderMethodPool() {return m_renderMethodPool;} 
	cModelManager&	modelPool() {return m_modelPool;}
	cImageManager&	imagePool() {return m_imagePool;}

	cRenderQueue& queue() {return m_renderQueue;}

protected:

	// Private Data...
	bool	m_bEnabled;
	bool	m_bClearEachFrame; // true to clear each frame to black before rendering

	// device-bound resource pools
	cTextureManager			m_texturePool;
	cSurfaceMaterialManager m_surfaceMaterialPool;
	cEffectFileManager		m_effectFilePool;
	cRenderMethodManager	m_renderMethodPool;
	cVertexBufferManager	m_vertexBufferPool;
	cIndexBufferManager		m_indexBufferPool;
	cModelManager			m_modelPool;
	cImageManager			m_imagePool;

	// device-bound resources
	cTexture*				m_pFontTexture;

	cRenderQueue			m_renderQueue;

    void registerDeviceResources();
    void unregisterDeviceResources();

	void handleSceneRecording();

	// Invalid Functions...
    cDisplayManager(const cDisplayManager& Src);
    cDisplayManager& operator=(const cDisplayManager& Src);
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cDisplayManager
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cDisplayManager::cDisplayManager()
:m_bClearEachFrame(true)
,m_bEnabled(false)
,m_pFontTexture(0)
{
	m_texturePool.initialize(16);
	m_vertexBufferPool.initialize(16);
	m_indexBufferPool.initialize(1);
	m_effectFilePool.initialize(16);
	m_renderMethodPool.initialize(16);
	m_modelPool.initialize(16);
	m_imagePool.initialize(16);
	m_surfaceMaterialPool.initialize(16);
}

/*	~cDisplayManager
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cDisplayManager::~cDisplayManager()
{
	m_texturePool.destroy();
	m_vertexBufferPool.destroy();
	m_indexBufferPool.destroy();
	m_effectFilePool.destroy();
	m_renderMethodPool.destroy();
	m_modelPool.destroy();
	m_imagePool.destroy();
	m_surfaceMaterialPool.destroy();
}

//. Mutators ...................................................
inline void cDisplayManager::enable()
{
	m_bEnabled = true;
}

inline void cDisplayManager::disable()
{
	m_bEnabled =false;
}

inline void cDisplayManager::enableFrameClearing(bool clear)
{
	m_bClearEachFrame = clear;
}

//. Accessors ............................................................................

//- End of cDisplayManager ----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cDisplayManager.h )

//----------------------------------------------------------
//$Log: $