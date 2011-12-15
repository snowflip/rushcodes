#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
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
#include "light_scattering_data.h"


//	Name Space declaration
namespace gaia
{


/*	cLightManager
-----------------------------------------------------------------
    
    The light manager contains atmospheric
	light scattering data as well as sunlight
	parameters. A single instance of this
	class is contained within the cGameHost

	Time in this class is measured from 0-1, where
	0 is midnight, and 1.0 is midnight on the
	following day. 0.5f is the noon value.
    
-----------------------------------------------------------------
*/

class cLightManager 
{
public:

	// Data Types & Constants...

    cLightManager();
    ~cLightManager(){};

	cLightScatteringData& lightScatteringData();
	const cVector4& sunColor()const;
	const cVector4& sunVector()const;
	const cVector4& ambientHue()const;

	void setTimeOfDay(float time);
	void setTimePerTick(float time_scale);

	void update();

private:

	cLightScatteringData m_lightScatteringData;
	float m_timeOfDay;
	float m_timeStep;
	float m_sunIntensity;
	cVector4 m_sunColor;
	cVector4 m_sunVector;
	cVector4 m_ambientHue;
	cVector4 m_dawnSunColor;
	cVector4 m_noonSunColor;
	cVector4 m_deltaSunColor;

	void recalculateSunlightParameters();
	void cLightManager::computeSunlightColor(
		float timeOfDay, // 0-1
		float turbidity, // must be >= 2
		cVector3& output);
};


inline cLightScatteringData& cLightManager::lightScatteringData()
{
	return m_lightScatteringData;
}

inline const cVector4& cLightManager::sunColor()const
{
	return m_sunColor;
}

inline const cVector4& cLightManager::sunVector()const
{
	return m_sunVector;
}

inline const cVector4& cLightManager::ambientHue()const
{
	return m_ambientHue;
}

//- End of cLightManager -------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cLightManager.h )

//----------------------------------------------------------
//$Log: $