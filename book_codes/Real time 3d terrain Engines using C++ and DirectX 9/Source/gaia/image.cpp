#define IMAGE_CPP
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

#include "image.h"
#include "game_host.h"
#include "core\perlin_noise.h"
#include "core\midpoint_noise.h"

using namespace gaia;

// innitialize the resource (called once)
bool cImage::createResource()
{
	debug_assert(!isResourceCreated(), "The resource has already been created.");

	m_width=(D3DX_DEFAULT);
	m_height=(D3DX_DEFAULT);
	m_d3dFormat=(k_32bit);
	m_d3dFilter=(D3DX_DEFAULT);
	m_d3dColorKey=(0);
	m_pPalette=(0);
	m_pImage=(0);

	memset(&m_d3dImageInfo, 0, sizeof(m_d3dImageInfo));

	// signal that the resource is now created,
	// but has not been enabled
	notifyCreated();
	notifyDisabled();
	return(true);
}

// destroy the resource
bool cImage::destroyResource()
{
	//
	// we permit redundant calls to destroy, so there are no asserts here
	//
	if (isResourceCreated())
	{
		// disable the resource
		disableResource();

		safe_release(m_pImage);

		notifyUnloaded();
		notifyDestroyed();

		return(true);
	}
	return(false);
}

// prepare the resource for use (create any volatile memory objects needed)					
bool cImage::restoreResource()
{				
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyRestored();
	return(true);
}

// purge the resource from device-dependant memory
bool cImage::disableResource()					
{
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyDisabled();
	return(true);
}


// load the resource from a file (or NULL to use the resource name)
bool cImage::loadResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	// attempt to load the texture info
	HRESULT hr = D3DXGetImageInfoFromFile(filename, &m_d3dImageInfo);
	
	if (SUCCEEDED(hr))
	{
		if (!m_pImage)
		{
			if (m_width == D3DX_DEFAULT)
			{
				m_width = m_d3dImageInfo.Width;
			}
			if (m_height == D3DX_DEFAULT)
			{
				m_height = m_d3dImageInfo.Height;
			}

			if (m_d3dImageInfo.Format == k_8bit)
			{
				m_d3dFormat = k_8bit;
				m_bytesPerPixel = 1;
			}
			else if (m_d3dImageInfo.Format == k_24bit)
			{
				m_d3dFormat = k_24bit;
				m_bytesPerPixel = 3;
			}
			else
			{
				m_d3dFormat = k_32bit;
				m_bytesPerPixel = 4;
			}

			hr = TheGameHost.d3dDevice()->CreateOffscreenPlainSurface( 
				m_width, m_height,
				(D3DFORMAT)m_d3dFormat,
				D3DPOOL_SCRATCH,
				&m_pImage, NULL );
		}

		if (SUCCEEDED(hr))
		{
			hr = D3DXLoadSurfaceFromFile(
				m_pImage,
				m_pPalette, // palette
				0, // dest rect
				filename,
				0, // src rect
				D3DX_DEFAULT, // filter
				m_d3dColorKey, // color key
				0); // image data
		}
		else 
		{
			debug_error(hr);
		}
	}
	else 
	{
		debug_error(hr);
	}

	return SUCCEEDED(hr);
}; 


// save the resource to a file (or NULL to use the resource name)
bool cImage::saveResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	HRESULT hr = -1;

	if (m_pImage)
	{
		hr = D3DXSaveSurfaceToFile(filename,
			D3DXIFF_DDS,
			m_pImage,
			m_pPalette,
			0);	
	}

	return SUCCEEDED(hr);
};	

void cImage::destroy()
{
	safe_release(m_pImage);
}

bool cImage::create(uint32 width, uint32 height, eValidFormats format)
{
	safe_release(m_pImage);

	m_width = width;
	m_height = height;
	m_d3dFormat = format;

	if (m_d3dFormat == k_8bit)
	{
		m_bytesPerPixel = 1;
	}
	else if (m_d3dFormat == k_24bit)
	{
		m_bytesPerPixel = 3;
	}
	else
	{
		m_bytesPerPixel = 4;
	}

	HRESULT hr = TheGameHost.d3dDevice()->CreateOffscreenPlainSurface( 
		m_width, m_height,
		(D3DFORMAT)m_d3dFormat,
		D3DPOOL_SCRATCH,
		&m_pImage, NULL );

	return SUCCEEDED(hr);

}

uint8* cImage::lock(const RECT* pRect, int* pitch)
{
	debug_assert(m_pImage, "no image to lock");

	D3DLOCKED_RECT lockedRect;

	HRESULT hr = m_pImage->LockRect(
		&lockedRect,
		pRect,
		0);

	if (FAILED(hr))
	{
		return 0;
	}

	m_bytesPerLine = lockedRect.Pitch;
	m_rawImage = (uint8*)lockedRect.pBits;

	if (pitch) *pitch = m_bytesPerLine;
	return (uint8*)lockedRect.pBits;
}

void cImage::unlock()
{
	m_pImage->UnlockRect();
	m_rawImage=0;
}

void cImage::setColor(uint16 x, uint16 y, uint32 color)
{
	debug_assert(m_rawImage, "no image");
	debug_assert(x < m_width, "invalid x index");
	debug_assert(y < m_height, "invalid y index");

	uint8* startOfColor = &m_rawImage[(y*m_bytesPerLine)+(x*m_bytesPerPixel)];
	uint8* output = (uint8*)&color;

	uint16 copyCount = minimum((uint16)sizeof(color), m_bytesPerPixel);
	for (int i=0; i<copyCount; ++i)
	{
		*startOfColor++ = *output++;
	}
}

uint8 cImage::getColor(uint16 x, uint16 y)const
{
	debug_assert(m_rawImage, "no image");
	debug_assert(x < m_width, "invalid x index");
	debug_assert(y < m_height, "invalid y index");

	return m_rawImage[(y*m_bytesPerLine)+(x*m_bytesPerPixel)];
}

void cImage::getColor(uint16 x, uint16 y, uint32& color)const
{
	debug_assert(m_rawImage, "no image");
	debug_assert(x < m_width, "invalid x index");
	debug_assert(y < m_height, "invalid y index");

	uint8* startOfColor = &m_rawImage[(y*m_bytesPerLine)+(x*m_bytesPerPixel)];
	uint8* output = (uint8*)&color;

	color = 0;

	uint16 copyCount = minimum((uint16)sizeof(color), m_bytesPerPixel);
	for (int i=0; i<copyCount; ++i)
	{
		*output++ = *startOfColor++;
	}
}

uint8 cImage::sampleColor(float u, float v)const
{
	debug_assert(m_rawImage, "no image");
	float xloc = u * m_width;
	float yloc = v * m_height;

	int xpos = realToInt32_chop(xloc);
	int ypos = realToInt32_chop(yloc);

	float xscale = xloc-xpos;
	float yscale = yloc-ypos;

	// read 4 samples
	float a = getColor((uint16)(xpos % m_width), (uint16)(ypos % m_height));
	float b = getColor((uint16)((xpos+1)%m_width), (uint16)(ypos%m_height));
	float c = getColor((uint16)(xpos%m_width), (uint16)((ypos+1)%m_height));
	float d = getColor((uint16)((xpos+1)%m_width), (uint16)((ypos+1)%m_height));

	a = (a*(1-xscale)) + (b*(xscale));
	b = (c*(1-xscale)) + (d*(xscale));

	a = (a*(1-yscale)) + (b*(yscale));
	int result = realToInt32_chop(a);
	result = clamp(result, 0, 255);

	return result;
}

void cImage::sampleColor(float u, float v, uint32& color)const
{
	debug_assert(m_rawImage, "no image");
	float xloc = u * m_width;
	float yloc = v * m_height;

	int xpos = realToInt32_chop(xloc);
	int ypos = realToInt32_chop(yloc);

	float xscale = xloc-xpos;
	float yscale = yloc-ypos;

	// read 4 samples
	uint32 c0,c1,c2,c3;

	getColor((uint16)(xpos % m_width), (uint16)(ypos % m_height), c0);
	getColor((uint16)((xpos+1)%m_width), (uint16)(ypos%m_height), c1);
	getColor((uint16)(xpos%m_width), (uint16)((ypos+1)%m_height), c2);
	getColor((uint16)((xpos+1)%m_width), (uint16)((ypos+1)%m_height), c3);

	cVector4 a((float)((c0>>24)&0xff), (float)((c0>>16)&0xff), (float)((c0>>8)&0xff), (float)(c0&0xff));
	cVector4 b((float)((c1>>24)&0xff), (float)((c1>>16)&0xff), (float)((c1>>8)&0xff), (float)(c1&0xff));
	cVector4 c((float)((c2>>24)&0xff), (float)((c2>>16)&0xff), (float)((c2>>8)&0xff), (float)(c2&0xff));
	cVector4 d((float)((c3>>24)&0xff), (float)((c3>>16)&0xff), (float)((c3>>8)&0xff), (float)(c3&0xff));

	a = (a*(1-xscale)) + (b*(xscale));
	b = (c*(1-xscale)) + (d*(xscale));

	a = (a*(1-yscale)) + (b*(yscale));

	int alpha = realToInt32_chop(a.x);
	int red = realToInt32_chop(a.y);
	int green = realToInt32_chop(a.z);
	int blue = realToInt32_chop(a.w);

	alpha = clamp(alpha, 0, 255);
	red = clamp(red, 0, 255);
	green = clamp(green, 0, 255);
	blue = clamp(blue, 0, 255);

	color = (alpha<<24)|(red<<16)|(green<<8)|blue;
}

void cImage::randomChannelNoise(uint8 channel, uint8 minRange, uint8 maxRange)
{
	int pitch;
	uint8* pBits = lock(0, &pitch);
	int channelShift = channel*8;
	int channelMask = 0xff<<channelShift;

	debug_assert(channel<4, "invalid image channel");
	if (pBits)
	{
		uint8* pOut = pBits;
		for (uint16 y=0;y<m_height;++y)
		{
			for (uint16 x=0;x<m_width;++x)
			{
				uint32 color;
				uint8 Noise = 
					RandomNumberInRange(minRange, maxRange); 
				
				getColor(x, y, color);

				color &= ~channelMask;
				color |= Noise<<channelShift;

				setColor(x, y, color);
			}

			pBits += pitch;
		}
	}
	unlock();
}


void cImage::setChannel(uint8 channel, uint8 value)
{
	int pitch;
	uint8* pBits = lock(0, &pitch);
	int channelShift = channel*8;
	int channelMask = 0xff<<channelShift;

	debug_assert(channel<4, "invalid image channel");
	if (pBits)
	{
		uint8* pOut = pBits;
		for (uint16 y=0;y<m_height;++y)
		{
			for (uint16 x=0;x<m_width;++x)
			{
				uint32 color;
				
				getColor(x, y, color);

				color &= ~channelMask;
				color |= value<<channelShift;

				setColor(x, y, color);
			}

			pBits += pitch;
		}
	}
	unlock();
}

void cImage::convertToBumpMask()
{
	int pitch;
	uint8* pBits = lock(0, &pitch);

	if (pBits)
	{
		uint8* pOut = pBits;
		for (uint16 y=0;y<m_height;++y)
		{
			for (uint16 x=0;x<m_width;++x)
			{
				uint32 color;
				getColor(x, y, color);

				uint8 r = (uint8)(color&0xff);
				uint8 g = (uint8)((color>>8)&0xff);
				uint8 b = (uint8)((color>>16)&0xff);
				uint8 a = (uint8)((color>>24)&0xff);

				// there can be only one!
				if (r>=g && r>=b && r>=a)
				{
					r=0xff; g=0; b=0; a=0;
				}
				else if (g>=r && g>=b && g>=a)
				{
					g=0xff; r=0; b=0; a=0;
				}
				else if (b>=r && b>=g && b>=a)
				{
					b=0xff; r=0; g=0; a=0;
				}
				else
				{
					a=0xff; r=0; g=0; b=0;
				}
				color = (a<<24)+(b<<16)+(g<<8)+r;
				setColor(x, y, color);
			}

			pBits += pitch;
		}
	}
	unlock();
}

//****************************************************************************************
// end of file      ( cImage.cpp )

