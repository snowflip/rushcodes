#ifndef SURFACEMATERIAL_H
#define SURFACEMATERIAL_H
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
#include "texture.h"
#include "xfile_templates.h"
#include "math\math.h"


//	Name Space declaration
namespace gaia
{

class cEffectFile;

/*	cSurfaceMaterial
-----------------------------------------------------------------
    
    cSurfaceMaterial is a wrapper for the D3DEffectPool. 
	This wrapper class allows us to store the interface in a 
	DataPool and treat it like a resource object
    
-----------------------------------------------------------------
*/

class cSurfaceMaterial : public cResourcePoolItem
{
public:

	// Data Types & Constants...
	enum eConstants
	{
		k_maxTexturesPerSurface = EXTENDED_TEXTURE_COUNT,
		k_TextureFileVersion = 1,
	};

	// Creators...

    cSurfaceMaterial();
    virtual ~cSurfaceMaterial();

	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	// Mutators...
	bool setD3DMaterial(D3DMATERIAL9* pMaterial);
	bool setTexture(uint32 index, const tchar* texture);
	bool setTexture(uint32 index, cTexture* texture);
	bool setTexture(uint32 index, cPoolHandle textureHandle);
	bool setTextureMatrix(uint32 index, const cMatrix4x4& matrix);

	// Accessors...
	uint32 totalTextures()const;
	cTexture* texture(uint32 slot)const;
	u32Flags textureFlags()const;
	cMatrix4x4* textureMatrix(uint32 slot)const;
	u32Flags textureMatrixFlags()const;
	const D3DMATERIAL9& d3dMaterial()const;
	D3DMATERIAL9& d3dMaterial();

private:

	// Private Data...
	D3DMATERIAL9 m_d3dMaterial;
	uint32 m_totalTextures;
	u32Flags m_textureFlags;
	u32Flags m_textureMatrixFlags;
	cTexture* m_pTexture[k_maxTexturesPerSurface];
	cMatrix4x4* m_pTextureMatrix[k_maxTexturesPerSurface];

	// Private Functions...
    cSurfaceMaterial(const cSurfaceMaterial& Src);
    cSurfaceMaterial& operator=(const cSurfaceMaterial& Src);

};

typedef cResourcePool<cSurfaceMaterial> cSurfaceMaterialManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cSurfaceMaterial
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cSurfaceMaterial::cSurfaceMaterial()
:m_totalTextures(0)
,m_textureFlags(0)
,m_textureMatrixFlags(0)
{
	memset(m_pTexture,0,sizeof(m_pTexture));
	memset(m_pTextureMatrix,0,sizeof(m_pTextureMatrix));
	setD3DMaterial(0);
}


/*	~cSurfaceMaterial
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cSurfaceMaterial::~cSurfaceMaterial()
{
}


//. Accessors ...................................................
inline uint32 cSurfaceMaterial::totalTextures()const
{
	return m_totalTextures;
}

inline cTexture* cSurfaceMaterial::texture(uint32 slot)const
{
	debug_assert(slot < k_maxTexturesPerSurface, "invalid texture slot");
	return m_pTexture[slot];
}

inline u32Flags cSurfaceMaterial::textureFlags()const
{
	return m_textureFlags;
}

inline cMatrix4x4* cSurfaceMaterial::textureMatrix(uint32 slot)const
{
	debug_assert(slot < k_maxTexturesPerSurface, "invalid texture slot");
	return m_pTextureMatrix[slot];
}

inline u32Flags cSurfaceMaterial::textureMatrixFlags()const
{
	return m_textureMatrixFlags;
}


inline const D3DMATERIAL9& cSurfaceMaterial::d3dMaterial()const
{
	return m_d3dMaterial;
}
inline D3DMATERIAL9& cSurfaceMaterial::d3dMaterial()
{
	return m_d3dMaterial;
}

//- End of cSurfaceMaterial -----------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSurfaceMaterial.h )

//----------------------------------------------------------
//$Log: $