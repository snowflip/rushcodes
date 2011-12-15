#define CEFFECTFILE_CPP
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

#include "effect_file.h"
#include "game_host.h"
#include "light_scattering_data.h"
#include "camera.h"

using namespace gaia;

bool cEffectFile::createResource()
{
	return true;
}

// destroy the resource
bool cEffectFile::destroyResource()
{
	safe_release(m_pEffect);
	return true;
}

// purge the resource from volatile memory
bool cEffectFile::disableResource()
{
	if (m_pEffect)
	{
		m_pEffect->OnLostDevice();
	}
	return true;
}

// prepare the resource for use (create any volatile memory objects needed)
bool cEffectFile::restoreResource()
{
	if (m_pEffect)
	{
		m_pEffect->OnResetDevice();
	}
	return true;
}

// load the resource from a file
bool cEffectFile::loadResource(const tchar* filename)
{
	safe_release(m_pEffect);

	if (!filename)
	{
		const cString* pString = findResourceName();

		if (pString)
		{
			filename = pString->c_str();
		}
	}

	tchar fullPath[MAX_PATH];

	// D3DXCreateEffectFromFile can not load a relative path
	_tfullpath(fullPath, filename, MAX_PATH);

    LPD3DXBUFFER pBufferErrors = NULL;
    HRESULT result = D3DXCreateEffectFromFile(TheGameHost.d3dDevice(), fullPath, NULL, NULL, 0, NULL, &m_pEffect, &pBufferErrors );
    if( FAILED( result ) )
	{
		debug_error(result);
		safe_release(pBufferErrors);
        return false;
	}
	safe_release(pBufferErrors);

    // get the description
	m_pEffect->GetDesc( &m_EffectDesc );

	// find the best possible technique
	result = m_pEffect->FindNextValidTechnique(NULL, &m_hTechnique);
    if( FAILED( result ) )
	{
		debug_error(result);
		safe_release(m_pEffect);
        return false;
	}

	// get some info about the technique
	result = m_pEffect->GetTechniqueDesc(m_hTechnique, &m_techniqueDesc);
    if( FAILED( result ) )
	{
		debug_error(result);
		safe_release(m_pEffect);
        return false;
	}

	// activate it
	result = m_pEffect->SetTechnique(m_hTechnique);
    if( FAILED( result ) )
	{
		debug_error(result);
		safe_release(m_pEffect);
        return false;
	}

	// parse the effect parameters to build a list of handles
	parseParameters();

	return true;
}

// save the resource to the file and return the size written
bool cEffectFile::saveResource(const tchar* filename)
{
	return 0;
}

bool cEffectFile::begin()
{
	debug_assert(m_pEffect, "This cEffectFile is not valid");
	HRESULT result = m_pEffect->Begin(0, D3DXFX_DONOTSAVESTATE|D3DXFX_DONOTSAVESHADERSTATE);
    if( FAILED( result ) )
	{
		debug_error(result);
        return false;
	}

	// set the lighting parameters
	// from the global light manager
	applyGlobalLightingData();

	// set the camera matricies
	applyCameraMatrices(
		TheGameHost.activeCamera());

    return true;
}

bool cEffectFile::activatePass(int pass)
{
	HRESULT result = m_pEffect->Pass(pass);
    if( FAILED( result ) )
	{
		debug_error(result);
        return false;
	}
    return true;
}

void cEffectFile::end()
{
	debug_assert(m_pEffect, "This cEffectFile is not valid");
	HRESULT result = m_pEffect->End();
    if( FAILED( result ) )
	{
		debug_error(result);
	}
}


void cEffectFile::parseParameters()
{
    // Look at parameters for semantics and annotations that we know how to interpret
    D3DXPARAMETER_DESC ParamDesc;
    D3DXHANDLE hParam;
	static char numerals[] = {'0','1','2','3','4','5','6','7','8','9'};

	memset(m_paramHandle, 0, sizeof(m_paramHandle));
	memset(m_matrixHandle, 0, sizeof(m_matrixHandle));
	memset(m_textureHandle, 0, sizeof(m_textureHandle));
	memset(m_textureMatrixHandle, 0, sizeof(m_textureMatrixHandle));

    for( UINT iParam = 0; iParam < m_EffectDesc.Parameters; iParam++ )
    {
        hParam = m_pEffect->GetParameter ( NULL, iParam );
        m_pEffect->GetParameterDesc( hParam, &ParamDesc );
        if( ParamDesc.Semantic != NULL && 
            ( ParamDesc.Class == D3DXPC_MATRIX_ROWS || ParamDesc.Class == D3DXPC_MATRIX_COLUMNS ) )
        {
            if( strcmpi( ParamDesc.Semantic, "world" ) == 0 )
                m_matrixHandle[k_worldMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "view" ) == 0 )
                m_matrixHandle[k_viewMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "projection" ) == 0 )
                m_matrixHandle[k_projMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldview" ) == 0 )
                m_matrixHandle[k_worldViewMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "viewprojection" ) == 0 )
                m_matrixHandle[k_viewProjMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldviewprojection" ) == 0 )
                m_matrixHandle[k_worldViewProjMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldmatrixarray" ) == 0 )
                m_matrixHandle[k_worldMatrixArray] = hParam;

			// look for texture matrices which are named texMatX
			if (strnicmp( ParamDesc.Semantic, "texmat", 6) ==0)
			{
				cString name(ParamDesc.Name);
				int iPos = name.find_first_of (numerals, 0, sizeof(numerals));

				if (iPos != cString::npos)
				{
					int iTexture = atoi(&ParamDesc.Name[iPos]);
					if (iTexture>=0 && iTexture<k_max_texture_handles)
					{
						m_textureMatrixHandle[iTexture] = hParam;
					}
				}
			}
        }

        if( ParamDesc.Semantic != NULL && 
            ( ParamDesc.Class == D3DXPC_VECTOR ))
        {
            if( strcmpi( ParamDesc.Semantic, "materialambient" ) == 0 )
                m_paramHandle[k_ambientMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialdiffuse" ) == 0 )
                m_paramHandle[k_diffuseMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialspecular" ) == 0 )
                m_paramHandle[k_specularMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialemissive" ) == 0 )
                m_paramHandle[k_emissiveMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "sunvector" ) == 0 )
                m_paramHandle[k_sunVector] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "suncolor" ) == 0 )
                m_paramHandle[k_sunColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldcamerapos" ) == 0 )
                m_paramHandle[k_cameraPos] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "viewdistances" ) == 0 )
                m_paramHandle[k_cameraDistances] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldviewvector" ) == 0 )
                m_paramHandle[k_cameraFacing] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "ambientlight" ) == 0 )
                m_paramHandle[k_ambientLight] = hParam;

            else if( strcmpi( ParamDesc.Semantic, "posScaleOffset" ) == 0 )
                m_paramHandle[k_posScaleOffset] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "uvScaleOffset" ) == 0 )
                m_paramHandle[k_uvScaleOffset] = hParam;

            else if( strcmpi( ParamDesc.Semantic, "flareColor" ) == 0 )
                m_paramHandle[k_lensFlareColor] = hParam;
			
       }

        if(ParamDesc.Class == D3DXPC_SCALAR)
        {
			if( ParamDesc.Semantic == NULL)
			{
				if( strcmpi( ParamDesc.Name, "curnumbones" ) == 0 )
				{
					m_paramHandle[k_boneInfluenceCount] = hParam;
				}
			}
			else
			{
				if( strcmpi( ParamDesc.Semantic, "materialpower" ) == 0 )
					m_paramHandle[k_specularMaterialPower] = hParam;
			}
		}

        if( ParamDesc.Class == D3DXPC_OBJECT )
        {
			cString name(ParamDesc.Name);
			
			if (ParamDesc.Type == D3DXPT_TEXTURE
				&& name == "shadow")
			{
				m_shadowTextureHandle = hParam;
			}
			else if (ParamDesc.Type == D3DXPT_TEXTURE
				|| ParamDesc.Type == D3DXPT_TEXTURE2D
				|| ParamDesc.Type == D3DXPT_TEXTURE3D
				|| ParamDesc.Type == D3DXPT_TEXTURECUBE)
			{
				int iPos = name.find_first_of (numerals, 0, sizeof(numerals));

				if (iPos != cString::npos)
				{
					int iTexture = atoi(&ParamDesc.Name[iPos]);
					if (iTexture>=0 && iTexture<k_max_texture_handles)
					{
						m_textureHandle[iTexture] = hParam;
					}
				}
			}
        }


		if ( ParamDesc.Class == D3DXPC_STRUCT)
		{
			if( strcmpi( ParamDesc.Semantic, "AtmosphericLighting" ) == 0 )
				m_paramHandle[k_atmosphericLighting] = hParam;
			else if ( strcmpi( ParamDesc.Semantic, "patchCorners") == 0 )
				m_paramHandle[k_patchCorners] = hParam;
		}
	}
}


void cEffectFile::applySurfaceMaterial(const cSurfaceMaterial* pSurfaceMaterial)
{
	if (pSurfaceMaterial)
	{
		// set material properties
		const D3DMATERIAL9& d3dMaterial = pSurfaceMaterial->d3dMaterial();
		if (isParameterUsed(k_ambientMaterialColor))
		{
			setParameter(k_ambientMaterialColor, &d3dMaterial.Ambient);
		}

		if (isParameterUsed(k_diffuseMaterialColor))
		{
			setParameter(k_diffuseMaterialColor, &d3dMaterial.Diffuse);
		}

		if (isParameterUsed(k_specularMaterialColor))
		{
			setParameter(k_specularMaterialColor, &d3dMaterial.Specular);
		}

		if (isParameterUsed(k_emissiveMaterialColor))
		{
			setParameter(k_specularMaterialColor, &d3dMaterial.Emissive);
		}

		if (isParameterUsed(k_specularMaterialPower))
		{
			setParameter(k_specularMaterialPower, &d3dMaterial.Power);
		}

		// set textures
		for (uint32 i=0;i<cSurfaceMaterial::k_maxTexturesPerSurface;++i)
		{
			if (TEST_BIT(pSurfaceMaterial->textureFlags(),i))
			{
				setTexture(i, pSurfaceMaterial->texture(i));
			}
			if (TEST_BIT(pSurfaceMaterial->textureMatrixFlags(),i))
			{
				setTextureMatrix(i, pSurfaceMaterial->textureMatrix(i));
			}
		}
	}
}


void cEffectFile::applyGlobalLightingData()
{
	// pass the lighting structure to the shader
	setParameter(
		k_atmosphericLighting, 
		LightManager.lightScatteringData().getShaderData());

	// pass the lighting structure to the shader
	setParameter(
		k_sunColor, 
		&LightManager.sunColor());

	setParameter(
		k_sunVector, 
		&LightManager.sunVector());

	setParameter(
		k_ambientLight, 
		&LightManager.ambientHue());
	
}

void cEffectFile::applyCameraMatrices(cCamera* pCamera)
{
	if (pCamera)
	{
		// set the view matrix
		setMatrix(k_viewMatrix, &pCamera->viewMatrix());
		// set the projection matrix
		setMatrix(k_projMatrix, &pCamera->projectionMatrix());
		// set the combined matrix
		setMatrix(k_viewProjMatrix, &pCamera->viewProjMatrix());
		// set the world camera pos
		setParameter(k_cameraPos, &pCamera->worldPosition());
		// set the world camera facing vector
		setParameter(k_cameraFacing, &pCamera->worldForward());


		cVector4 camDistances(
			pCamera->nearPlane(),
			pCamera->farPlane(),
			pCamera->farPlane()-pCamera->nearPlane(),
			1.0f/pCamera->farPlane());
			
		setParameter(k_cameraDistances, &camDistances);
	}
}

//***************************************************************
// end of file      ( RenderMethod.cpp )

//----------------------------------------------------------
//$Log: $