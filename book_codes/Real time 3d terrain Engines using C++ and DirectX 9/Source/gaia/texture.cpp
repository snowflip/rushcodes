#define TEXTUREOBJECT_CPP
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

#include "texture.h"
#include "game_host.h"
#include "image.h"
#include "core\midpoint_noise.h"
#include "core\perlin_noise.h"

using namespace gaia;

// innitialize the resource (called once)
bool cTexture::createResource()
{
	debug_assert(!isResourceCreated(), "The resource has already been created.");

	m_width=(D3DX_DEFAULT);
	m_height=(D3DX_DEFAULT);
	m_mipLevels=(D3DX_DEFAULT);
	m_d3dUsage=(0);
	m_d3dFormat=(D3DFMT_UNKNOWN);
	m_d3dPool=(D3DPOOL_MANAGED);
	m_d3dFilter=(D3DX_DEFAULT);
	m_d3dMipFilter=(D3DX_DEFAULT);
	m_d3dColorKey=(0);
	m_pPalette=(0);
	m_pTexture=(0);
	m_pCubeTexture=(0);

	memset(&m_d3dImageInfo, 0, sizeof(m_d3dImageInfo));

	// signal that the resource is now created,
	// but has not been enabled
	notifyCreated();
	notifyDisabled();
	return(true);
}

void cTexture::releaseTexture()
{
	safe_release(m_pTexture);
	safe_release(m_pCubeTexture);
}

// destroy the resource
bool cTexture::destroyResource()
{
	//
	// we permit redundant calls to destroy, so there are no asserts here
	//
	if (isResourceCreated())
	{
		// disable the resource
		disableResource();

		releaseTexture();

		notifyUnloaded();
		notifyDestroyed();

		return(true);
	}
	return(false);
}

// prepare the resource for use (create any volatile memory objects needed)					
bool cTexture::restoreResource()
{				
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyRestored();
	return(true);
}

// purge the resource from device-dependant memory
bool cTexture::disableResource()					
{
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyDisabled();
	return(true);
}

bool cTexture::configureImageSettings()
{
	// interrogate the image info
	if (m_d3dImageInfo.ResourceType == D3DRTYPE_VOLUMETEXTURE)
	{
		// unsupported texture type
		return false;
	}

	safe_release(m_pTexture);
	safe_release(m_pCubeTexture);


	// copy the image information to our local members
	m_width = m_d3dImageInfo.Width;
	m_height = m_d3dImageInfo.Height;
	m_mipLevels = m_d3dImageInfo.MipLevels;
	m_d3dFormat = m_d3dImageInfo.Format;

	return true;
}

bool cTexture::checkTextureRequirements()
{
	HRESULT hr = D3DXCheckTextureRequirements(TheGameHost.d3dDevice(),
										(UINT*)&m_width,
										(UINT*)&m_height,
										(UINT*)&m_mipLevels,
										m_d3dUsage,
										&m_d3dFormat,
										m_d3dPool);		


	if (SUCCEEDED(hr))
	{
		// check for paletized formats
		safe_delete_array(m_pPalette);
		if (m_d3dFormat == D3DFMT_P8)
		{
			m_pPalette = new PALETTEENTRY[256];
		}
		return true;
	}
	return false;
}

bool cTexture::checkCubeTextureRequirements()
{
	HRESULT hr = D3DXCheckCubeTextureRequirements(TheGameHost.d3dDevice(),
										(UINT*)&m_width,
										(UINT*)&m_mipLevels,
										m_d3dUsage,
										&m_d3dFormat,
										m_d3dPool);		


	if (SUCCEEDED(hr))
	{
		// check for paletized formats
		safe_delete_array(m_pPalette);
		if (m_d3dFormat == D3DFMT_P8)
		{
			m_pPalette = new PALETTEENTRY[256];
		}
		return true;
	}
	return false;
}

bool cTexture::loadTextureFromImageFile(const tchar* filename)
{
	HRESULT hr;

	// attempt to load the texture
	hr = D3DXCreateTextureFromFileEx(	TheGameHost.d3dDevice(), 
										filename,
										m_width,
										m_height,
										m_mipLevels,
										m_d3dUsage,
										m_d3dFormat,
										m_d3dPool,
										m_d3dFilter,
										m_d3dMipFilter,
										m_d3dColorKey,
										&m_d3dImageInfo,
										m_pPalette,
										&m_pTexture);

	return SUCCEEDED(hr);
}

bool cTexture::loadTextureFromMemory(uint8* memory, uint32 size)
{
	HRESULT hr;

	// attempt to load the texture
	hr = D3DXCreateTextureFromFileInMemoryEx(	TheGameHost.d3dDevice(), 
										memory,
										size,
										m_width,
										m_height,
										m_mipLevels,
										m_d3dUsage,
										m_d3dFormat,
										m_d3dPool,
										m_d3dFilter,
										m_d3dMipFilter,
										m_d3dColorKey,
										&m_d3dImageInfo,
										m_pPalette,
										&m_pTexture);

	return SUCCEEDED(hr);
}

bool cTexture::loadCubeTextureFromImageFile(const tchar* filename)
{
	HRESULT hr;

	// attempt to load the texture
	hr = D3DXCreateCubeTextureFromFileEx(	TheGameHost.d3dDevice(), 
										filename,
										m_width,
										m_mipLevels,
										m_d3dUsage,
										m_d3dFormat,
										m_d3dPool,
										m_d3dFilter,
										m_d3dMipFilter,
										m_d3dColorKey,
										&m_d3dImageInfo,
										m_pPalette,
										&m_pCubeTexture);

	return SUCCEEDED(hr);

}


bool cTexture::loadCubeTextureFromMemory(uint8* memory, uint32 size)
{
	HRESULT hr;

	// attempt to load the texture
	hr = D3DXCreateCubeTextureFromFileInMemoryEx(	TheGameHost.d3dDevice(), 
										memory,
										size,
										m_width,
										m_mipLevels,
										m_d3dUsage,
										m_d3dFormat,
										m_d3dPool,
										m_d3dFilter,
										m_d3dMipFilter,
										m_d3dColorKey,
										&m_d3dImageInfo,
										m_pPalette,
										&m_pCubeTexture);

	return SUCCEEDED(hr);
}

// load the resource from a file (or NULL to use the resource name)
bool cTexture::loadResource(const tchar* filename)
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
	
	// HACK!!!
	// we convert paletized bitmaps (D3DX_P8)
	if (m_d3dImageInfo.Format == D3DFMT_P8)
	{
		m_d3dImageInfo.Format = D3DFMT_A8;
	}


	if (SUCCEEDED(hr))
	{
		if (configureImageSettings())
		{
			// attempt to load the real texture
			if (m_d3dImageInfo.ResourceType == D3DRTYPE_CUBETEXTURE)
			{
				if (checkCubeTextureRequirements())
				{
					hr = loadCubeTextureFromImageFile(filename);
				}
			}
			else if (m_d3dImageInfo.ResourceType == D3DRTYPE_TEXTURE)
			{
				if (checkTextureRequirements())
				{
					hr = loadTextureFromImageFile(filename);
				}
			}
			else
			{
				hr = -1;
			}

			// if loading failed, free the palette
			if (FAILED(hr))
			{
				safe_delete_array(m_pPalette);
			}
		}
	}
	return SUCCEEDED(hr);
}; 


// save the resource to a file (or NULL to use the resource name)
bool cTexture::saveResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	LPDIRECT3DBASETEXTURE9 baseTexture = (LPDIRECT3DBASETEXTURE9)getTexture();

	HRESULT hr = D3DXSaveTextureToFile(filename, 
										D3DXIFF_DDS, 
										baseTexture,
										m_pPalette);
	return SUCCEEDED(hr);
};	

bool cTexture::uploadImage(const cImage* pImage, bool copyAll)
{
	IDirect3DSurface9* pSurface;
	HRESULT hr = -1;
	
	if (m_pTexture)
	{
		int total = copyAll ? 1 : m_pTexture->GetLevelCount();

		for (int i=0; i<total; ++i)
		{
			hr = m_pTexture->GetSurfaceLevel(i,&pSurface);

			if (SUCCEEDED(hr))
			{
				hr = D3DXLoadSurfaceFromSurface(
					pSurface,
					0,
					0,
					pImage->getSurface(),
					0,
					0,
					D3DX_FILTER_LINEAR,
					0
				);	
				
				safe_release(pSurface);
			}
		}
	}

	return SUCCEEDED(hr);
}

bool cTexture::uploadCubeFace(const cImage* pImage, D3DCUBEMAP_FACES face, bool copyAll)
{
	IDirect3DSurface9* pSurface;
	HRESULT hr = -1;
	
	if (m_pCubeTexture)
	{
		int total = copyAll ? 1 : m_pCubeTexture->GetLevelCount();

		for (int i=0; i<total; ++i)
		{
			hr = m_pCubeTexture->GetCubeMapSurface(face, i,&pSurface);

			if (SUCCEEDED(hr))
			{
				hr = D3DXLoadSurfaceFromSurface(
					pSurface,
					0,
					0,
					pImage->getSurface(),
					0,
					0,
					D3DX_FILTER_LINEAR,
					0
				);	
				
				safe_release(pSurface);
			}
		}
	}

	return SUCCEEDED(hr);
}

bool cTexture::createTexture(uint32 width, uint32 height, uint32 mipLevels, uint32 usage, D3DFORMAT Format, D3DPOOL pool)
{
	safe_release(m_pCubeTexture);
	safe_release(m_pTexture);

	HRESULT hr = 
	D3DXCheckTextureRequirements(          
		TheGameHost.d3dDevice(),
		(UINT*)&width,
		(UINT*)&height,
		(UINT*)&mipLevels,
		usage,
		&Format,
		pool
	);

	if (SUCCEEDED(hr))
	{
		hr = D3DXCreateTexture
		(          
			TheGameHost.d3dDevice(),
			width,
			height,
			mipLevels,
			usage,
			Format,
			pool,
			&m_pTexture
		);
		
		if (SUCCEEDED(hr))
		{
			m_width		=width;
			m_height	=height;
			m_mipLevels =mipLevels;
			m_d3dUsage	=usage;
			m_d3dFormat =Format;
			m_d3dPool	=pool;
		}
	}

	return SUCCEEDED(hr);
}

bool cTexture::createCubeTexture(uint32 size, uint32 mipLevels, uint32 usage, D3DFORMAT Format, D3DPOOL pool)
{
	safe_release(m_pCubeTexture);
	safe_release(m_pTexture);

	HRESULT hr = 
	D3DXCheckCubeTextureRequirements(          
		TheGameHost.d3dDevice(),
		(UINT*)&size,
		(UINT*)&mipLevels,
		usage,
		&Format,
		pool
	);

	if (SUCCEEDED(hr))
	{
		hr = D3DXCreateCubeTexture
		(          
			TheGameHost.d3dDevice(),
			size,
			mipLevels,
			usage,
			Format,
			pool,
			&m_pCubeTexture
		);
		
		if (SUCCEEDED(hr))
		{
			m_width		=size;
			m_height	=size;
			m_mipLevels =mipLevels;
			m_d3dUsage	=usage;
			m_d3dFormat =Format;
			m_d3dPool	=pool;
		}
	}

	return SUCCEEDED(hr);
}


struct MidpointPacket
{
	cMidpointNoise midpoint;
	float falloff;
	uint16 sizeX;
	uint16 sizeY;
};

static void WINAPI midpointCallback(
	D3DXVECTOR4* pOut, 
	CONST D3DXVECTOR2* pTexCoord, 
	CONST D3DXVECTOR2* pTexelSize, 
	LPVOID pData)
{
	MidpointPacket* pNoise = (MidpointPacket*)pData;

	uint16 x = (uint16)realToInt32_chop(pTexCoord->x * (pNoise->sizeX-1));
	uint16 y = (uint16)realToInt32_chop(pTexCoord->y * (pNoise->sizeY-1));

	float value = pNoise->midpoint.noise(x, y, pNoise->sizeX, pNoise->sizeY, pNoise->falloff);

	pOut->x = value;
	pOut->y = value;
	pOut->z = value;
	pOut->w = value;
}


void cTexture::generateMidpointNoise(float falloff)
{
	MidpointPacket midpoint;

	midpoint.falloff = falloff;
	midpoint.sizeX = (uint16)m_width;
	midpoint.sizeY = (uint16)m_height;

	if (m_pTexture)
	{
		D3DXFillTexture(          
			m_pTexture,
			midpointCallback,
			(LPVOID) &midpoint
		);
	}

}

struct PerlinPacket
{
	cPerlinNoise perlin;
	float scale;
	int octaves;
	float falloff;
	uint16 sizeX;
	uint16 sizeY;
};

static void WINAPI perlinCallback(
	D3DXVECTOR4* pOut, 
	CONST D3DXVECTOR2* pTexCoord, 
	CONST D3DXVECTOR2* pTexelSize, 
	LPVOID pData)
{
	PerlinPacket* pNoise = (PerlinPacket*)pData;
	float x = (pTexCoord->x * pNoise->sizeX);
	float y = (pTexCoord->y * pNoise->sizeY);

//	float accum = pNoise->perlin.noise(x,y,pNoise->scale);
//	accum *= 2.0f;

	float accum = 0;
	float frequency = pNoise->scale;//*2.0f;
	float amplitude = 1.0f;

	for (int i=0; i<pNoise->octaves; ++i)
	{
		accum += pNoise->perlin.noise(x,y,frequency) * amplitude;
		amplitude *= pNoise->falloff;
		frequency *= 2.0f;
	}

	accum = clamp(accum, -1.0f, 1.0f);
	accum *= 0.5f;
	accum += 0.5f;

	pOut->x = accum;
	pOut->y = accum;
	pOut->z = accum;
	pOut->w = accum;
}

void cTexture::generatePerlinNoise(float scale, int octaves, float falloff)
{
	PerlinPacket packet;
	packet.scale = scale;
	packet.octaves = octaves;
	packet.falloff = falloff;
	packet.sizeX = (uint16)m_width;
	packet.sizeY = (uint16)m_height;

	if (m_pTexture)
	{
		D3DXFillTexture(          
			m_pTexture,
			perlinCallback,
			(LPVOID) &packet
		);
	}
}

bool cTexture::generateNormalMap(
	LPDIRECT3DTEXTURE9 heightMap, 
	uint32 channel, 
	uint32 flags, 
	float amplitude)
{
	HRESULT hr = -1;
	if (m_pTexture)
	{
		hr = D3DXComputeNormalMap(          
			m_pTexture,
			heightMap,
			0,
			flags,
			channel,
			amplitude);
	}

	return SUCCEEDED(hr);
}

bool cTexture::convertToNormalMap(
	uint32 channel, 
	uint32 flags, 
	float amplitude)
{
	bool success = false;

	if (m_pTexture)
	{
		// create a new texture
		LPDIRECT3DTEXTURE9 temp;
		HRESULT hr = D3DXCreateTexture
		(          
			TheGameHost.d3dDevice(),
			m_width,
			m_height,
			m_mipLevels,
			m_d3dUsage,
			m_d3dFormat,
			m_d3dPool,
			&temp
		);

		if (SUCCEEDED(hr))
		{
			// swap pointers with the new texture
			swap(m_pTexture, temp);

			if (generateNormalMap(temp, channel, flags, amplitude))
			{
				success= true;
			}
			safe_release(temp);
		}
	}

	return success;
}


static void WINAPI normalizationCubeCallback(
	D3DXVECTOR4* pOut, 
	CONST D3DXVECTOR3* pTexCoord, 
	CONST D3DXVECTOR3* pTexelSize, 
	LPVOID pData)
{
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal,pTexCoord); 

	normal *= 0.5f;

	pOut->x = normal.x+0.5f;
	pOut->y = normal.y+0.5f;
	pOut->z = normal.z+0.5f;
	pOut->w = 1.0f;
}


bool cTexture::generateNormalizationCubeMap()
{
	if (m_pCubeTexture)
	{
		if (SUCCEEDED(D3DXFillCubeTexture(          
			m_pCubeTexture,
			normalizationCubeCallback,
			0)))
		{
			return true;
		}
	}
	return false;
}

bool cTexture::getSurfaceLevel(int level, LPDIRECT3DSURFACE9* ppSurface)
{
	if (m_pTexture)
	{
		return SUCCEEDED(
			m_pTexture->GetSurfaceLevel(level, ppSurface));
	}
	return false;
}

bool cTexture::maskWithImage(cImage* pImage)
{
	
	pImage->lock(0,0);

	// first, build a table of heights
	D3DLOCKED_RECT lockedRect;
	if(SUCCEEDED(
		m_pTexture->LockRect(0, &lockedRect, 
			0, 0)))
	{
		uint8* pPixel = (uint8*)lockedRect.pBits;

		for(uint32 y=0; y<m_height; ++y)
		{
			for(uint32 x=0; x<m_width; ++x)
			{
				float u = (float)x/(float)(m_width-1);
				float v = (float)y/(float)(m_height-1);

				int index = (y*lockedRect.Pitch)+(x*4);
				uint8 iPixel = 
					pPixel[index];

				uint8 maxValue = pImage->sampleColor(u,v);

				iPixel = minimum(iPixel, maxValue);

				pPixel[index] = iPixel;
				pPixel[index+1] = iPixel;
				pPixel[index+2] = iPixel;
				pPixel[index+3] = iPixel;
			}
		}

		pImage->unlock();
		m_pTexture->UnlockRect(0);

		return true;
	}

	return false;
}


//****************************************************************************************
// end of file      ( cTexture.cpp )

