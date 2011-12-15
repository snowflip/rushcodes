#ifndef CEFFECTFILE_H
#define CEFFECTFILE_H
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
#include <D3D9.h>
#include <D3Dx9.h>
#include "texture.h"
#include "surface_material.h"


//	Name Space declaration
namespace gaia
{

class cLightScatteringData;
class cCamera;

/*	cEffectFile
-----------------------------------------------------------------
    
    cEffectFile is a wrapper for the D3DEffectPool. 
	This wrapper class allows us to store the interface in a 
	DataPool and treat it like a resource object
    
-----------------------------------------------------------------
*/

class cEffectFile : public cResourcePoolItem
{
public:

	// Data Types & Constants...
	enum eMatrixHandles
	{
		k_worldMatrix = 0,
		k_viewMatrix,
		k_projMatrix,
		
		k_worldViewMatrix,
		k_viewProjMatrix,
		k_worldViewProjMatrix,
		
		k_worldMatrixArray,

		k_max_matrix_handles,
	};
		
	enum eParameterHandles
	{
		k_boneInfluenceCount=0,

		k_ambientMaterialColor,
		k_diffuseMaterialColor,
		k_emissiveMaterialColor,
		k_specularMaterialColor,
		k_specularMaterialPower,

		k_PointLightPos0,
		k_PointlightVec0,
		k_PointlightColor0,

		k_sunVector,
		k_sunColor,
		k_cameraPos,
		k_cameraDistances,
		k_cameraFacing,
		k_ambientLight,

		k_patchCorners,
		k_atmosphericLighting,

		k_posScaleOffset,
		k_uvScaleOffset,

		k_lensFlareColor,

		k_max_param_handles,
	};
	
	enum eTextureHandles
	{
		k_max_texture_handles=cSurfaceMaterial::k_maxTexturesPerSurface,
	};

	// Public Data...

	// Creators...

    cEffectFile();
    virtual ~cEffectFile();

	// Operators...


	// Mutators...
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	bool begin();
	bool activatePass(int pass);
	void end();

	// Accessors...
	LPD3DXEFFECT effect()const;
	int totalPasses()const;

	bool isParameterUsed(eParameterHandles index)const;
	bool isMatrixUsed(eMatrixHandles index)const;
	bool isTextureUsed(int index)const;
	bool isTextureMatrixUsed(int index)const;
	bool isShadowTextureUsed()const;

	bool setParameter(eParameterHandles index, const void* data, int32 size=D3DX_DEFAULT)const;
	bool setMatrix(eMatrixHandles index, const D3DXMATRIX* data)const;
	bool setMatrixArray(eMatrixHandles index, const D3DXMATRIX* data, uint32 count)const;
	bool setMatrixInArray(eMatrixHandles index, uint32 element, const D3DXMATRIX* data)const;
	bool setFloatArray(eParameterHandles index, const float* data, uint32 count)const;
	bool setFloatInArray(eParameterHandles index, uint32 element, float data)const;
	bool setTexture(int index, const cTexture* data)const;
	bool setTextureMatrix(int index, const D3DXMATRIX* data)const;
	bool setShadowTexture(const cTexture* data)const;

	void applySurfaceMaterial(const cSurfaceMaterial* pSurfaceMaterial);
	void applyCameraMatrices(cCamera* pCamera);
	void applyGlobalLightingData();

private:

	// Private Data...
	LPD3DXEFFECT m_pEffect;
    D3DXEFFECT_DESC m_EffectDesc;
    D3DXHANDLE m_hTechnique;
	D3DXTECHNIQUE_DESC m_techniqueDesc;
    D3DXHANDLE m_paramHandle[k_max_param_handles];
    D3DXHANDLE m_matrixHandle[k_max_matrix_handles];
    D3DXHANDLE m_textureHandle[k_max_texture_handles];
    D3DXHANDLE m_textureMatrixHandle[k_max_texture_handles];
	D3DXHANDLE m_shadowTextureHandle;

	// Private Functions...
	void parseParameters();

	cEffectFile(const cEffectFile& Src);
    cEffectFile& operator=(const cEffectFile& Src);

};

typedef cResourcePool<cEffectFile> cEffectFileManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cEffectFile
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cEffectFile::cEffectFile()
:m_pEffect(0)
,m_shadowTextureHandle(0)
{
	memset(m_paramHandle, 0, sizeof(m_paramHandle));
	memset(m_matrixHandle, 0, sizeof(m_matrixHandle));
	memset(m_textureHandle, 0, sizeof(m_textureHandle));
	memset(m_textureMatrixHandle, 0, sizeof(m_textureMatrixHandle));
}


/*	~cEffectFile
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cEffectFile::~cEffectFile()
{
	debug_assert(!m_pEffect, "cEffectFile is still active");
}


//. Accessors ...................................................
inline LPD3DXEFFECT cEffectFile::effect()const
{
	return m_pEffect;
}

inline int cEffectFile::totalPasses()const
{
	debug_assert(m_pEffect, "cEffectFile is not valid");
	return m_techniqueDesc.Passes;
}

inline bool cEffectFile::isParameterUsed(eParameterHandles index)const
{
	return m_paramHandle[index] != 0;
}

inline bool cEffectFile::isMatrixUsed(eMatrixHandles index)const
{
	return m_matrixHandle[index] != 0;
}

inline bool cEffectFile::isTextureUsed(int index)const
{
	return m_textureHandle[index] != 0;
}

inline bool cEffectFile::isShadowTextureUsed()const
{
	return m_shadowTextureHandle != 0;
}

inline bool cEffectFile::isTextureMatrixUsed(int index)const
{
	return m_textureMatrixHandle[index] != 0;
}


inline bool cEffectFile::setParameter(eParameterHandles index, const void* data, int32 size)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		bool result= SUCCEEDED(m_pEffect->SetValue(m_paramHandle[index], data, size));

		debug_assert(result, "effect file error");
		return result;
	}
	return false;
}

inline bool cEffectFile::setMatrix(eMatrixHandles index, const D3DXMATRIX* data)const
{
	if (m_pEffect && isMatrixUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetMatrix(m_matrixHandle[index], data));
	}
	return false;
}

inline bool cEffectFile::setMatrixArray(eMatrixHandles index, const D3DXMATRIX* data, uint32 count)const
{
	if (m_pEffect && isMatrixUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetMatrixArray(m_matrixHandle[index], data, count));
	}
	return false;
}

inline bool cEffectFile::setMatrixInArray(eMatrixHandles index, uint32 element, const D3DXMATRIX* data)const
{
	if (m_pEffect && isMatrixUsed(index))
	{
		D3DXHANDLE subHandle = m_pEffect->GetParameterElement(m_matrixHandle[index],element);
		return SUCCEEDED(m_pEffect->SetMatrix(subHandle, data));
	}
	return false;
}

inline bool cEffectFile::setFloatArray(eParameterHandles index, const float* data, uint32 count)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetFloatArray(m_paramHandle[index], data, count));
	}
	return false;
}

inline bool cEffectFile::setFloatInArray(eParameterHandles index, uint32 element, float data)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		D3DXHANDLE subHandle = m_pEffect->GetParameterElement(m_paramHandle[index],element);
		return SUCCEEDED(m_pEffect->SetFloat(subHandle, data));
	}
	return false;
}


inline bool cEffectFile::setTexture(int index, const cTexture* data)const
{
	if (m_pEffect && isTextureUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetTexture(m_textureHandle[index], data->getTexture()));
	}
	return false;
}

inline bool cEffectFile::setShadowTexture(const cTexture* data)const
{
	if (m_pEffect && isShadowTextureUsed())
	{
		return SUCCEEDED(m_pEffect->SetTexture(m_shadowTextureHandle, data->getTexture()));
	}
	return false;
}

inline bool cEffectFile::setTextureMatrix(int index, const D3DXMATRIX* data)const
{
	if (m_pEffect && isTextureMatrixUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetMatrix(m_textureMatrixHandle[index], data));
	}
	return false;
}

//- End of cEffectFile -----------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cEffectFile.h )

//----------------------------------------------------------
//$Log: $