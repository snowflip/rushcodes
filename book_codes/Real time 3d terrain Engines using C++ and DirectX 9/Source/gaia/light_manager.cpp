#define CLIGHTMANAGER_CPP
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

#include "light_manager.h"
#include <math\math.h>

using namespace gaia;

// time scales from 0.0 to 1.0 each day
// 0.0 = dawn,
// 360 = dawn the next day
// the following defines help set day values
#define TIME_CYCLE TWO_PI
#define TIME_NOON (TIME_CYCLE*0.0f)
#define TIME_DAWN (TIME_CYCLE*0.30f)
#define TIME_DUSK (TIME_CYCLE*0.70f)
#define TIME_MIDNIGHT 0.5f

cLightManager::cLightManager()
:m_timeOfDay(0.5f)
,m_timeStep(0.001f)
,m_sunIntensity(1.0f)
,m_sunColor(1.0f,0.8f,0.8f, 1.0f)
,m_sunVector(0.0f,0.0f,1.0f,0.0f)
,m_ambientHue(0.0f,0.0f,0.0f,0.0f)
{
	computeSunlightColor(TIME_DAWN, 2, m_dawnSunColor);
	computeSunlightColor(TIME_NOON, 2, m_noonSunColor);

	m_dawnSunColor.w =100.0f;
	m_noonSunColor.w =100.0f;

	m_deltaSunColor = m_noonSunColor-m_dawnSunColor;
	setTimeOfDay(TIME_NOON);
}

void cLightManager::setTimeOfDay(float time)
{
	m_timeOfDay = time;
	
	m_timeOfDay = modulus(m_timeOfDay, TIME_CYCLE);
	
	recalculateSunlightParameters();
}

void cLightManager::setTimePerTick(float time_scale)
{
	m_timeStep = time_scale;
}

void cLightManager::update()
{
	if (m_timeStep != 0.0f)
	{
		//
		// un-comment this line for automatic sun movement
		//
//		setTimeOfDay(m_timeOfDay + m_timeStep);
	}


	if (GetAsyncKeyState(VK_PRIOR) & 0x8000) 
	{
		m_timeOfDay = m_timeOfDay + 0.1f;
		setTimeOfDay(m_timeOfDay);
	}
	else if (GetAsyncKeyState(VK_NEXT) & 0x8000)
	{
		m_timeOfDay = m_timeOfDay - 0.1f;
		setTimeOfDay(m_timeOfDay);
	}
	
	if (GetAsyncKeyState('N') & 0x8000) 
	{
		m_lightScatteringData.setMieScale(
			m_lightScatteringData.getMieScale()-0.01f);
	}
	else if (GetAsyncKeyState('M') & 0x8000)
	{
		m_lightScatteringData.setMieScale(
			m_lightScatteringData.getMieScale()+0.01f);
	}

	if (GetAsyncKeyState('V') & 0x8000) 
	{
		m_lightScatteringData.setRayleighScale(
			m_lightScatteringData.getRayleighScale()-0.01f);
	}
	else if (GetAsyncKeyState('B') & 0x8000)
	{
		m_lightScatteringData.setRayleighScale(
			m_lightScatteringData.getRayleighScale()+0.01f);
	}

	if (GetAsyncKeyState('O') & 0x8000) 
	{
		m_lightScatteringData.setHenyeyG(
			m_lightScatteringData.getHenyeyG()-0.001f);
	}
	else if (GetAsyncKeyState('P') & 0x8000)
	{
		m_lightScatteringData.setHenyeyG(
			m_lightScatteringData.getHenyeyG()+0.001f);
	}

	if (GetAsyncKeyState('K') & 0x8000) 
	{
		m_lightScatteringData.setInscatteringScale(
			m_lightScatteringData.getInscatteringScale()-0.01f);
	}
	else if (GetAsyncKeyState('L') & 0x8000)
	{
		m_lightScatteringData.setInscatteringScale(
			m_lightScatteringData.getInscatteringScale()+0.01f);
	}

	if (GetAsyncKeyState('H') & 0x8000) 
	{
		m_lightScatteringData.setExtinctionScale(
			m_lightScatteringData.getExtinctionScale()-0.01f);
	}
	else if (GetAsyncKeyState('J') & 0x8000)
	{
		m_lightScatteringData.setExtinctionScale(
			m_lightScatteringData.getExtinctionScale()+0.01f);
	}
}


void cLightManager::recalculateSunlightParameters()
{
	// the sun rotates on the world Y axis
	m_sunVector.set(
		0.0f,
		(float)sin(m_timeOfDay),
		(float)cos(m_timeOfDay),
		1.0f
	);

	// we approximate the color of the sun by 
	// interpolating between the noon sun
	// (bright yellow) and a sunrise\sunset
	// reddish sun.

	// ramp up to the noon sun color
	// as the sun reaches the highest
	// point in the sky
	float zenithFactor 
		= clamp(m_sunVector.z, 0.0f, 1.0f);

	m_sunColor = 
		m_dawnSunColor
		+(m_deltaSunColor*zenithFactor);

	float fadeOut = 1.0f;

	if (m_sunVector.z < 0.0f)
	{
		fadeOut = -m_sunVector.z* 5.0f;
		fadeOut = clamp(1.0f-fadeOut, 0.0f, 1.0f);
	}

	m_sunColor.w = m_sunIntensity * 100.0f * fadeOut;

	m_ambientHue.set(
		zenithFactor*0.3f, 
		zenithFactor*0.2f, 
		0.25f,
		0.0f);

}


void cLightManager::computeSunlightColor(
	float timeOfDay, // 0-2pi
	float turbidity, // must be >= 2
	cVector3& output)
{
	// compute theta as the angle from
	// the noon (zenith) position in
	// radians
	float theta = 
		timeOfDay<PI ? timeOfDay:TWO_PI-timeOfDay;

	// angles greater than the horizon are clamped
	theta = clamp(theta, 0.0f, HALF_PI);

	// beta is a measure of aerosols.
	float beta = 
		0.04608365822050f * 
		turbidity - 0.04586025928522f;
    float opticalMass = 
		-1.0f/((float)cos(theta) 
		+ 0.15f
		*(float)pow(93.885f-theta/PI*180.0f,-1.253f));

	// constants for lambda
	// provided by Preetham et al,
	cVector3 lambda(
		0.65f, 
		0.57f,
		0.475f
	);

	// compute each color channel
	float tauR, tauA;
    for(int i=0; i<3; ++i)
	{
		// Rayleigh Scattering
		tauR = (float)exp(opticalMass 
				* 0.008735f 
				* (float)pow(lambda[i], 
					-4.08f));

		// Aerosal (water + dust) attenuation
		// paticle size ratio set at (1.3) 
		tauA = (float)exp(opticalMass 
				* beta 
				* (float)pow(lambda[i], 
					-1.3f)); 


		output[i] = tauR * tauA; 
    }

	output.normalize();
}
//***************************************************************
// end of file      ( cLightManager.cpp )

//----------------------------------------------------------
//$Log: $