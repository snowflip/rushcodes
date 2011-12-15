#ifndef LIGHTSCATTERINGDATA_H
#define LIGHTSCATTERINGDATA_H
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
#include <bin\media\shaders\light_scattering_constants.h>


//	Name Space declaration
namespace gaia
{


/*	cLightScatteringData
-----------------------------------------------------------------
    
    The cLightScatteringData class contains atmospheric
	lighting calculations for outdoor light
	scattering. This class is based on the work
	of Nathaniel Hoffman, Kenneth J. Mitchell 
	and Arcot J. Preetham. Details on their work
	can be found at 

	http://www.ati.com/developer/
	dx9/ATI-LightScattering.pdf

	This class is responsible for building a
	set of vertex shader paramters which
	can be used by the cEffectFile to
	perform light scattering for outdoor
	scenes.
    
-----------------------------------------------------------------
*/

class cLightScatteringData
{
public:

	// Data Types & Constants...

    cLightScatteringData();
    ~cLightScatteringData(){};


	void setHenyeyG(float g);
	void setRayleighScale(float s);
	void setMieScale(float s);
	void setInscatteringScale(float s);
	void setExtinctionScale(float s);
	void setTerrainReflectionScale(float s);

	float getHenyeyG()const;
	float getRayleighScale()const;
	float getMieScale()const;
	float getInscatteringScale()const;
	float getExtinctionScale()const;
	float getTerrainReflectionScale()const;

	const sLightScatteringShaderParams* getShaderData()const;

private:

	// 'g' values used in the approximation
	// function designed by Henyey Greenstein
	float m_henyeyG;		

	// scalars to control the size of
	// coefficients used for Rayleigh
	// and Mie light scattering
	float m_rayleighBetaMultiplier;
	float m_mieBetaMultiplier;

	// scalars to control the overall
	// amount of light scattering for
	// both inscattering and
	// extinction
	float m_inscatteringMultiplier;
	float m_extinctionMultiplier;

	// a scalar to adjust the reflective
	// nature of the terrain itself
	float m_reflectivePower;

	// the current light scattering
	// structure provided to the
	// vertex shaders
	sLightScatteringShaderParams m_shaderParams;

	// the private function which updates the
	// the internal structure
	void recalculateShaderData();

};




//
// Accessors
//
inline float cLightScatteringData::getHenyeyG()const
{
	return m_henyeyG;
}

inline float cLightScatteringData::getRayleighScale()const
{
	return m_rayleighBetaMultiplier;
}

inline float cLightScatteringData::getMieScale()const
{
	return m_mieBetaMultiplier;
}

inline float cLightScatteringData::getInscatteringScale()const
{
	return m_inscatteringMultiplier;
}

inline float cLightScatteringData::getExtinctionScale()const
{
	return m_extinctionMultiplier;
}

inline float cLightScatteringData::getTerrainReflectionScale()const
{
	return m_reflectivePower;
}

inline const sLightScatteringShaderParams* cLightScatteringData::getShaderData()const
{
	return &m_shaderParams;
}

//- End of cLightScatteringData -------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cLightScatteringData.h )

//----------------------------------------------------------
//$Log: $