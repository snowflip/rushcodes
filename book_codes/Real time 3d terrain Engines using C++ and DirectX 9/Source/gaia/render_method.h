#ifndef CRENDERMETHOD_H
#define CRENDERMETHOD_H
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
#include "math\math.h"


//	Name Space declaration
namespace gaia
{

class cEffectFile;
class cSurfaceMaterial;

class cRenderMethod: public cResourcePoolItem
{
public:

	// Data Types & Constants...
	enum eRenderStages
	{
		k_defaultMethod = 0,
		k_ambientColorStage,
		k_bumpMapStage,
		k_lightingStage,

		k_max_render_stages,
	};
		
    cRenderMethod();
    virtual ~cRenderMethod();

	// Mutators...
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	void setEffect(uint32 renderStage, cEffectFile* method);
	void setMaterial(uint32 renderStage, cSurfaceMaterial* method);

	void loadEffect(
		uint8 stage,
		const tchar* effectFile);

	cEffectFile* getEffect(uint32 renderStage);
	cSurfaceMaterial* getMaterial(uint32 renderStage);
	cEffectFile* getActiveEffect();
	cSurfaceMaterial* getActiveMaterial();

private:

	// Private Data...
	cEffectFile* m_effectList[k_max_render_stages];
	cSurfaceMaterial* m_materialList[k_max_render_stages];
};

inline cRenderMethod::cRenderMethod()
{

	memset(m_effectList, 0, sizeof(m_effectList));
}

inline cRenderMethod::~cRenderMethod()
{
	for (int i=0; i<k_max_render_stages;++i)
	{
		debug_assert(
			m_effectList[i]==0, 
			"destructor called before resource was properly destroyed");
	}
}

inline cEffectFile* cRenderMethod::getEffect(uint32 renderStage)
{
	debug_assert(
		renderStage<k_max_render_stages, 
		"invalid render stage");

	return m_effectList[renderStage];
}

inline cSurfaceMaterial* cRenderMethod::getMaterial(uint32 renderStage)
{
	debug_assert(
		renderStage<k_max_render_stages, 
		"invalid render stage");

	return m_materialList[renderStage];
}


typedef cResourcePool<cRenderMethod> cRenderMethodManager;

//- End of cRenderMethod -------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRenderMethod.h )

//----------------------------------------------------------
//$Log: $