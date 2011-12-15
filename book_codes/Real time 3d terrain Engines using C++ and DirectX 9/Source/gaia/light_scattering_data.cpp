#define LIGHTSCATTERINGDATA_CPP
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

#include "light_scattering_data.h"
#include <math\math.h>

using namespace gaia;

//
// light scattering constants
//

// Rayleigh scattering coefficient
static const cVector4 vRayleighBeta(
	0.000697153f,
	0.00117891f,
	0.00244460f, 1.0f); 

// Rayleigh Angular scattering 
// coefficient without phase term.
static const cVector4 vRayleighAngularBeta(
	4.16082e-005f,
	7.03612e-005f,
	0.000145901f, 1.0f);

//Mie scattering coefficient
static const cVector4 vMieBeta(
	0.00574060f,
	0.00739969f,
	0.0105143f, 1.0f);

//Mie Angular scattering
//coefficient without phase term.
static const cVector4 vMieAngularBeta(
	0.00133379f,
	0.00173466f,
	0.00249762f, 1.0f);

// Reflective spectrum data for soil.
// As calculated by Hoffman, Mitchell 
// and Preetham
static const cVector4 vSoilReflectivity(
	0.138f,
	0.113f, 
	0.08f, 1.0f);


/*	cLightScatteringData constructor
-----------------------------------------------------------------
    
    The scalar values provided allow for real-time
	changes to the atmosphere. As with Hoffman,
	Mitchell and Preetham's original work, we
	provide these controls for experimentation.
    
-----------------------------------------------------------------
*/
cLightScatteringData::cLightScatteringData()
:m_henyeyG(0.98f)
,m_rayleighBetaMultiplier(0.06f)
,m_mieBetaMultiplier(0.001f)
,m_inscatteringMultiplier(0.27f)
,m_extinctionMultiplier(1.33f)
,m_reflectivePower(0.1f)
{
	recalculateShaderData();
}


void cLightScatteringData::recalculateShaderData()
{
	float InvLog2 = 1.0f/(float)log(2.0f);
	//
	// rebuild the structure used by vertex shaders
	// to perform light scattering
	//
	m_shaderParams.vBeta1 = 
		vRayleighBeta * m_rayleighBetaMultiplier;

	m_shaderParams.vBeta2 = 
		vMieBeta * m_mieBetaMultiplier;

	m_shaderParams.vBetaD1 = 
		vRayleighAngularBeta * m_rayleighBetaMultiplier;

	m_shaderParams.vBetaD2 = 
		vMieAngularBeta * m_mieBetaMultiplier;

	m_shaderParams.vSumBeta1Beta2 = 
		m_shaderParams.vBeta1 +
		m_shaderParams.vBeta2;

	m_shaderParams.vLog2eBetaSum = 
		m_shaderParams.vSumBeta1Beta2 *
		InvLog2;

	m_shaderParams.vRcpSumBeta1Beta2.set(
		1.0f / m_shaderParams.vSumBeta1Beta2.x,
		1.0f / m_shaderParams.vSumBeta1Beta2.y,
		1.0f / m_shaderParams.vSumBeta1Beta2.z,
		0.0f);

	// precalculate values used by the shader
	// for Henyey Greenstein approximation
	m_shaderParams.vHG.set(
		1.0f-m_henyeyG*m_henyeyG, 
		1.0f + m_henyeyG, 
		2.0f * m_henyeyG,
		1.0f);

	// set some basic constants used by the shader
	m_shaderParams.vConstants.set(
		1.0f,
		InvLog2,
		0.5f,
		0.0f);

	// set the inscatter and extinction terms
	m_shaderParams.vTermMultipliers.set(
		m_inscatteringMultiplier,
		m_extinctionMultiplier,
		2.0f, // another constant
		0.0f);

	// set the reflective power of soil
	m_shaderParams.vSoilReflectivity = 
		vSoilReflectivity * m_reflectivePower;

}


void cLightScatteringData::setHenyeyG(float g)
{
	m_henyeyG = g;
	recalculateShaderData();
}

void cLightScatteringData::setRayleighScale(float s)
{
	m_rayleighBetaMultiplier = s;
	recalculateShaderData();
}

void cLightScatteringData::setMieScale(float s)
{
	m_mieBetaMultiplier = s;
	recalculateShaderData();
}

void cLightScatteringData::setInscatteringScale(float s)
{
	m_inscatteringMultiplier = s;
	recalculateShaderData();
}

void cLightScatteringData::setExtinctionScale(float s)
{
	m_extinctionMultiplier = s;
	recalculateShaderData();
}

void cLightScatteringData::setTerrainReflectionScale(float s)
{
	m_reflectivePower = s;
	recalculateShaderData();
}


//***************************************************************
// end of file      ( cOutdoorLighting.cpp )

//----------------------------------------------------------
//$Log: $