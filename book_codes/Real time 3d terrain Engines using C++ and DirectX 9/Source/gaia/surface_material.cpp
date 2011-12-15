#define CSURFACEMATERIAL_CPP
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

#include "surface_material.h"
#include "game_host.h"
#include "effect_file.h"

using namespace gaia;

// innitialize the resource (called once)
bool cSurfaceMaterial::createResource()
{
	debug_assert(!isResourceCreated(), "The resource has already been created.");

	m_totalTextures = 0;
	m_textureFlags = 0;
	m_textureMatrixFlags = 0;
	memset(m_pTexture,0,sizeof(m_pTexture));
	memset(m_pTextureMatrix,0,sizeof(m_pTextureMatrix));
	setD3DMaterial(0);

	// signal that the resource is now created,
	// but has not been enabled
	notifyCreated();
	notifyDisabled();
	return(true);
}

// destroy the resource
bool cSurfaceMaterial::destroyResource()
{
	//
	// we permit redundant calls to destroy, so there are no asserts here
	//
	for (uint32 i=0;i<k_maxTexturesPerSurface;++i)
	{
		safe_release(m_pTexture[i]);
		safe_delete(m_pTextureMatrix[i]);
		m_textureFlags.clearBit(i);
		m_textureMatrixFlags.clearBit(i);
	}

	m_totalTextures=0;
	if (isResourceCreated())
	{
		// disable the resource
		disableResource();

		notifyUnloaded();
		notifyDestroyed();

		return(true);
	}
	return(false);
}

// prepare the resource for use (create any volatile memory objects needed)					
bool cSurfaceMaterial::restoreResource()
{				
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyRestored();
	return(true);
}

// purge the resource from device-dependant memory
bool cSurfaceMaterial::disableResource()					
{
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyDisabled();
	return(true);
}


// load the resource from a file (or NULL to use the resource name)
bool cSurfaceMaterial::loadResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	// load all textures and matrices
	cFile file(filename, cFile::READ_ONLY);
	
	if (!file.open())
	{
		return false;
	}

	destroyResource();
	createResource();

	file.read(&m_d3dMaterial, sizeof(m_d3dMaterial));
	file.read(m_totalTextures);
	file.read(m_textureFlags.value);
	file.read(m_textureMatrixFlags.value);

	SetCurrentDirectory(TheGameHost.rootPath());
	for (int i=0; i<k_maxTexturesPerSurface; ++i)
	{
		if (TEST_BIT(m_textureFlags,i))
		{
			cString textureName;
			file.read(textureName);
			m_pTexture[i] = DisplayManager.texturePool().loadResource(textureName);

			if (!m_pTexture[i])
			{
				m_textureFlags.clearBit(i);
			}
		}
		if (TEST_BIT(m_textureMatrixFlags,i))
		{
			m_pTextureMatrix[i] = new cMatrix4x4;
			file.read(m_pTextureMatrix[i], sizeof(cMatrix4x4));
		}
	}

	file.close();
	return true;
}; 


// save the resource to a file (or NULL to use the resource name)
bool cSurfaceMaterial::saveResource(const tchar* filename)
{
	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	// save out all data (texture resource names)
	cFile file(filename, cFile::WRITE_ONLY);
	
	if (!file.open())
	{
		if (!file.create())
		{
			return false;
		}
	}

	file.write(&m_d3dMaterial, sizeof(m_d3dMaterial));
	file.write(m_totalTextures);
	file.write(m_textureFlags.value);
	file.write(m_textureMatrixFlags.value);

	for (int i=0; i<k_maxTexturesPerSurface; ++i)
	{
		if (TEST_BIT(m_textureFlags,i))
		{
			const cString* textureName = m_pTexture[i]->findResourceName();
			file.write(*textureName);
		}
		if (TEST_BIT(m_textureMatrixFlags,i))
		{
			file.write(m_pTextureMatrix[i], sizeof(cMatrix4x4));
		}
	}

	file.close();
	return true;
};	

bool cSurfaceMaterial::setD3DMaterial(D3DMATERIAL9* pMaterial)
{
	if (pMaterial)
	{
		memcpy(&m_d3dMaterial, pMaterial, sizeof(m_d3dMaterial));
	}
	else
	{
		// set default material
        memset(&m_d3dMaterial, 0, sizeof(m_d3dMaterial));
        m_d3dMaterial.Diffuse.r = 0.5f;
        m_d3dMaterial.Diffuse.g = 0.5f;
        m_d3dMaterial.Diffuse.b = 0.5f;
        m_d3dMaterial.Specular = m_d3dMaterial.Diffuse;
	}
	return true;
}

bool cSurfaceMaterial::setTexture(uint32 index, const tchar* texture)
{
	debug_assert(index<k_maxTexturesPerSurface , "invalid texture slot");

	if (m_pTexture[index])
	{
		//TheGameHost.displayManager().texturePool().releaseResource(m_pTexture[index]);
		safe_release(m_pTexture[index]);
		m_textureFlags.clearBit(index);
		--m_totalTextures;
	}

	if (texture)
	{
		m_pTexture[index] = DisplayManager.texturePool().loadResource(texture);

		if (m_pTexture[index])
		{
			m_textureFlags.setBit(index);
			++m_totalTextures;
		}
	}
	return true;
}

bool cSurfaceMaterial::setTexture(uint32 index, cTexture* texture)
{
	debug_assert(index<k_maxTexturesPerSurface , "invalid texture slot");

	if (!m_pTexture[index] || texture != m_pTexture[index])
	{
		if (m_pTexture[index])
		{
			//TheGameHost.displayManager().texturePool().releaseResource(m_pTexture[index]);
			safe_release(m_pTexture[index]);
			m_textureFlags.clearBit(index);
			--m_totalTextures;
		}

		m_pTexture[index] = texture;

		if (m_pTexture[index])
		{
			m_pTexture[index]->AddRef();
			m_textureFlags.setBit(index);
			++m_totalTextures;
		}
	}
	return true;
}

bool cSurfaceMaterial::setTexture(uint32 index, cPoolHandle textureHandle)
{
	debug_assert(index<k_maxTexturesPerSurface , "invalid texture slot");

	if (!m_pTexture[index] || m_pTexture[index]->resourceHandle() != textureHandle)
	{
		if (m_pTexture[index])
		{
			//TheGameHost.displayManager().texturePool().releaseResource(m_pTexture[index]);
			safe_release(m_pTexture[index]);
			m_textureFlags.clearBit(index);
			--m_totalTextures;
		}

		m_pTexture[index] = (cTexture*)TheGameHost.displayManager().texturePool().getResource(textureHandle);

		if (m_pTexture[index])
		{
			m_textureFlags.setBit(index);
			++m_totalTextures;
		}
	}
	return true;
}


bool cSurfaceMaterial::setTextureMatrix(uint32 index, const cMatrix4x4& matrix)
{
	debug_assert(index<k_maxTexturesPerSurface , "invalid texture slot");

	if (!m_pTextureMatrix[index])
	{
		// allocate space to hold the matrix locally
		m_pTextureMatrix[index] = new cMatrix4x4;
	}
	memcpy(m_pTextureMatrix[index], &matrix, sizeof(cMatrix4x4));
	m_textureMatrixFlags.setBit(index);
	return true;
}

//***************************************************************
// end of file      ( cSurfaceMaterial.cpp )

//----------------------------------------------------------
//$Log: $