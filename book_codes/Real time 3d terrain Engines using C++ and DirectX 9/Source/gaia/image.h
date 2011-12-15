#ifndef IMAGE_H
#define IMAGE_H
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


/*	cImage
------------------------------------------------------------------------------------------
	
	cImage is a system-memory resource for image data. It is mainly a wrapper around
	system memory Direct3D Surfaces
	
------------------------------------------------------------------------------------------
*/

class cImage : public cResourcePoolItem
{
public:

	enum eValidFormats
	{
		k_8bit = D3DFMT_A8,
		k_24bit = D3DFMT_R8G8B8,
		k_32bit = D3DFMT_A8R8G8B8,
	};

	cImage();
	~cImage();

	// base resource overrides
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	bool create(uint32 width, uint32 height, eValidFormats format);
	void destroy();
//	void generateMidpointNoise(float falloff);
//	void generatePerlinNoise(float scale, int octaves, float falloff);

	uint8* lock(const RECT* pRect=0, int* pitch=0);
	void unlock();
	void randomChannelNoise(uint8 channel, uint8 minRange, uint8 maxRange);
	void setChannel(uint8 channel, uint8 value);
	void convertToBumpMask();
	void setColor(uint16 x, uint16 y, uint32 value);

	// Accessors...
	uint8 getColor(uint16 x, uint16 y)const;
	void getColor(uint16 x, uint16 y, uint32& color)const;

	uint8 sampleColor(float x, float y)const;
	void sampleColor(float x, float y, uint32& color)const;

	
	
	// Accessors...
	LPDIRECT3DSURFACE9		getSurface()const;
	uint32 width()const {return m_width;}
	uint32 height()const {return m_height;}


private:

	// Data...
	uint32					m_width;
    uint32					m_height;
    eValidFormats			m_d3dFormat;
    DWORD					m_d3dFilter;
    D3DCOLOR				m_d3dColorKey;
    D3DXIMAGE_INFO			m_d3dImageInfo;
    PALETTEENTRY*			m_pPalette;
	LPDIRECT3DSURFACE9		m_pImage;
	
	uint16	m_bytesPerPixel;
	uint16	m_bytesPerLine;
	uint8*  m_rawImage;

	// Nonexistant Functions...
	cImage( const cImage& Src);
	cImage& operator=( const cImage& Src);

};

typedef cResourcePool<cImage> cImageManager;


//- Inline Functions ---------------------------------------------------------------------

//. Creators .............................................................................

/*	cImage
------------------------------------------------------------------------------------------
	
	Default Object Constructor
	
------------------------------------------------------------------------------------------
*/
inline cImage::cImage()
:m_width(D3DX_DEFAULT)
,m_height(D3DX_DEFAULT)
,m_d3dFormat(k_32bit)
,m_d3dFilter(D3DX_DEFAULT)
,m_d3dColorKey(0)
,m_pPalette(0)
,m_pImage(0)
,m_rawImage(0)
{
	memset(&m_d3dImageInfo, 0, sizeof(m_d3dImageInfo));
}

/*	~cImage
------------------------------------------------------------------------------------------
	
	Default Object Destructor
	
------------------------------------------------------------------------------------------
*/
inline cImage::~cImage()
{
}

//. Operators ............................................................................

//. Mutators .............................................................................
inline LPDIRECT3DSURFACE9 cImage::getSurface()const
{
	return m_pImage;
}


//- End of cImage -----------------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( texture.h )

