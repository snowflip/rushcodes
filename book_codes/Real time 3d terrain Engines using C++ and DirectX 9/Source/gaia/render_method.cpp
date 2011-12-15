#define CRENDERMETHOD_CPP
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

#include "render_method.h"
#include "effect_file.h"
#include "game_host.h"

using namespace gaia;

static const uint32 sFileID = 0x34032471;

// innitialize the resource (called once)
bool cRenderMethod::createResource()
{
	debug_assert(!isResourceCreated(), "The resource has already been created.");

	memset(m_effectList, 0, sizeof(m_effectList));
	memset(m_materialList, 0, sizeof(m_effectList));

	// signal that the resource is now created,
	// but has not been enabled
	notifyCreated();
	notifyDisabled();
	return(true);
}

// destroy the resource
bool cRenderMethod::destroyResource()
{
	//
	// we permit redundant calls to destroy, so there are no asserts here
	//
	for (uint32 i=0;i<k_max_render_stages;++i)
	{
		safe_release(m_effectList[i]);
		safe_release(m_materialList[i]);
	}

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
bool cRenderMethod::restoreResource()
{				
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyRestored();
	return(true);
}

// purge the resource from device-dependant memory
bool cRenderMethod::disableResource()					
{
	debug_assert(isResourceCreated(), "The resource has not been created.");

	notifyDisabled();
	return(true);
}


// load the resource from a file (or NULL to use the resource name)
bool cRenderMethod::loadResource(const tchar* filename)
{
	bool result = true;

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

	u32Flags stageFlags(0);
	uint32 fileID(0);

	// verify the file ID
	file.read(fileID);
	if (sFileID != fileID)
	{
		debug_print("WARNING!!! unable to load %s", filename);
		result = false;
	}
	else
	{
		int i;
		SetCurrentDirectory(TheGameHost.rootPath());
		
		// load in all effect files
		file.read(stageFlags.value);
		for (i=0; i<k_max_render_stages; ++i)
		{
			if (TEST_BIT(stageFlags,i))
			{
				cString shaderName;
				file.read(shaderName);
				m_effectList[i] = DisplayManager.effectFilePool().loadResource(shaderName);

				if (!m_effectList[i])
				{
					debug_print("WARNING!!! unable to load %s", shaderName);
					result = false;
				}
			}
		}

		// load in all surface materials
		file.read(stageFlags.value);
		for (i=0; i<k_max_render_stages; ++i)
		{
			if (TEST_BIT(stageFlags,i))
			{
				cString materialName;
				file.read(materialName);
				m_materialList[i] = DisplayManager.surfaceMaterialPool().loadResource(materialName);

				if (!m_materialList[i])
				{
					debug_print("WARNING!!! unable to load %s", materialName);
					result = false;
				}
			}
		}
	}
	file.close();
	return result;
}; 


// save the resource to a file (or NULL to use the resource name)
bool cRenderMethod::saveResource(const tchar* filename)
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

	u32Flags stageFlags(0);

	// build a set of flags describing 
	// the effect file slots we use
	file.write(sFileID);
	int i;
	for (i=0;i<k_max_render_stages;++i)
	{
		stageFlags.setBit(i, m_effectList[i] != 0);
	}
	file.write(stageFlags.value);

	// write the names of the effect files
	for (i=0; i<k_max_render_stages; ++i)
	{
		if (TEST_BIT(stageFlags,i))
		{
			const cString* shaderName = 
				m_effectList[i]->findResourceName();
			file.write(*shaderName);
		}
	}

	// build a set of flags describing 
	// the material slots we use
	for (i=0;i<k_max_render_stages;++i)
	{
		stageFlags.setBit(i, m_materialList[i] != 0);
	}
	file.write(stageFlags.value);

	// write the names of the material files
	for (i=0; i<k_max_render_stages; ++i)
	{
		if (TEST_BIT(stageFlags,i))
		{
			const cString* matName = 
				m_materialList[i]->findResourceName();
			file.write(*matName);
		}
	}

	file.close();
	return true;
};	

void cRenderMethod::setEffect(uint32 renderStage, cEffectFile* method)
{
	debug_assert(
		renderStage<k_max_render_stages, 
		"invalid render stage");

	safe_release(m_effectList[renderStage]);
	m_effectList[renderStage] = method;
	if (method) method->AddRef();
}

void cRenderMethod::setMaterial(uint32 renderStage, cSurfaceMaterial* material)
{
	debug_assert(
		renderStage<k_max_render_stages, 
		"invalid render stage");

	safe_release(m_materialList[renderStage]);
	m_materialList[renderStage] = material;
	if (material) material->AddRef();
}

void cRenderMethod::loadEffect(
	uint8 stage, 
	const tchar* effectFile)
{
	debug_assert(stage < k_max_render_stages, "invalid render stage");

	safe_release(m_effectList[stage]);

	m_effectList[stage] 
		= DisplayManager.effectFilePool().loadResource(effectFile);
}

cEffectFile* cRenderMethod::getActiveEffect()
{
	return m_effectList[TheGameHost.currentRenderStage()];
}

cSurfaceMaterial* cRenderMethod::getActiveMaterial()
{
	return m_materialList[TheGameHost.currentRenderStage()];
}

//***************************************************************
// end of file      ( cRenderMethod.cpp )

//----------------------------------------------------------
//$Log: $