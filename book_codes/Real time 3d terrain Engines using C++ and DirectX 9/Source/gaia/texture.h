#ifndef TEXTURE_H
#define TEXTURE_H
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
#include <core\core.h>
#define DIRECT3D_VERSION         0x0900
#include <d3d9.h>
#include <d3dx9.h>


//	Name Space declaration
namespace gaia
{


class cImage;


class cTexture : public cResourcePoolItem
{
public:

	// Data Types & Constants...
	enum eTextureFlags
	{
		k_cubeMap=0,
		k_dynamicTexture,
		k_renderTarget,
		k_paletized,
		k_mipMaps,
	};

	enum eForcedFormatFlags
	{
		k_forceMipLevels=0,
		k_forceFormat,
		k_forceSize,
	};

	// Creators...

	cTexture();
	~cTexture();

	// base resource overrides
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	bool createTexture(uint32 width, uint32 height, uint32 mipLevels, uint32 usage, D3DFORMAT Format, D3DPOOL pool);
	bool createCubeTexture(uint32 size, uint32 mipLevels, uint32 usage, D3DFORMAT Format, D3DPOOL pool);
	void generateMidpointNoise(float falloff);
	void generatePerlinNoise(float scale, int octaves, float falloff);
	bool generateNormalMap(LPDIRECT3DTEXTURE9 heightMap, uint32 channel, uint32 flags, float amplitude);
	bool generateNormalizationCubeMap();
	bool maskWithImage(cImage* pImage);

	void releaseTexture();

	bool convertToNormalMap(
		uint32 channel, 
		uint32 flags, 
		float amplitude);

	// Accessors...
	LPDIRECT3DTEXTURE9		getTexture()const;
	bool uploadImage(const cImage* pImage, bool copyAll = true);
	bool uploadCubeFace(const cImage* pImage, D3DCUBEMAP_FACES face, bool copyAll = true);

	uint32		width()const{return m_width;}
    uint32		height()const{return m_height;}
    uint32		mipLevels()const{return m_mipLevels;}
    uint32		d3dUsage()const{return m_d3dUsage;}
    D3DFORMAT	d3dFormat()const{return m_d3dFormat;}
    D3DPOOL		d3dPool()const{return m_d3dPool;}

	bool	getSurfaceLevel(int level, LPDIRECT3DSURFACE9* ppSurface);
private:

	// Data...
	uint32					m_width;
    uint32					m_height;
    uint32					m_mipLevels;
    uint32					m_d3dUsage;
    D3DFORMAT				m_d3dFormat;
    D3DPOOL					m_d3dPool;
    DWORD					m_d3dFilter;
    DWORD					m_d3dMipFilter;
    D3DCOLOR				m_d3dColorKey;
    D3DXIMAGE_INFO			m_d3dImageInfo;
    PALETTEENTRY*			m_pPalette;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DCUBETEXTURE9	m_pCubeTexture;


	bool loadFromResourceFile(cFile& InputFile);
	bool configureImageSettings();
	bool checkTextureRequirements();
	bool checkCubeTextureRequirements();

	bool loadTextureFromImageFile(const tchar* filename);
	bool loadTextureFromMemory(uint8* memory, uint32 size);
	bool loadCubeTextureFromImageFile(const tchar* filename);
	bool loadCubeTextureFromMemory(uint8* memory, uint32 size);
	
	// Nonexistant Functions...
	cTexture( const cTexture& Src);
	cTexture& operator=( const cTexture& Src);

};

typedef cResourcePool<cTexture> cTextureManager;


//- Inline Functions ---------------------------------------------------------------------

//. Creators .............................................................................

/*	cTexture
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/
inline cTexture::cTexture()
:m_width(D3DX_DEFAULT)
,m_height(D3DX_DEFAULT)
,m_mipLevels(D3DX_DEFAULT)
,m_d3dUsage(0)
,m_d3dFormat(D3DFMT_UNKNOWN)
,m_d3dPool(D3DPOOL_MANAGED)
,m_d3dFilter(D3DX_DEFAULT)
,m_d3dMipFilter(D3DX_DEFAULT)
,m_d3dColorKey(0)
,m_pPalette(0)
,m_pTexture(0)
,m_pCubeTexture(0)
{
	memset(&m_d3dImageInfo, 0, sizeof(m_d3dImageInfo));
}

/*	~cTexture
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cTexture::~cTexture()
{
}

//. Operators ............................................................................

//. Mutators .............................................................................
inline LPDIRECT3DTEXTURE9 cTexture::getTexture()const
{
	if (m_pCubeTexture)
	{
		return (LPDIRECT3DTEXTURE9)m_pCubeTexture;
	}
	return m_pTexture;
}


//- End of cTexture -----------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( texture.h )

