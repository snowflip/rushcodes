#ifndef CSKYBOX_H
#define CSKYBOX_H
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
#include "geometry\geometry.h"


//	Name Space declaration
namespace gaia
{

class cCamera;
class cSurfaceMaterial;
class cRenderMethod;

/*	cSkyModel
-----------------------------------------------------------------
    
    A Sky Box is a simple cube oriented around the origin. It is
	intended to be used with a cube map texture to render
	distant scenery around the camera
    
-----------------------------------------------------------------
*/

class cSkyModel
{
public:

	// Data Types & Constants...
	enum eModelType
	{
		k_box = 0,
		k_hemisphere,
		k_sphere,
		k_dome,
	};
	// Public Data...

	// Creators...

    cSkyModel()
	:m_pMesh(0)
	,m_pRenderMethod(0){};

    virtual ~cSkyModel(){};

	virtual void create(eModelType type);
	virtual void destroy();

	void setSurfaceMaterial(cSurfaceMaterial* pMaterial);
	void setRenderMethod(cRenderMethod* pMethod);
	void update();
	void render();

protected:

	// Private Data...
	eModelType m_type;
	LPD3DXMESH m_pMesh;
	cRenderMethod* m_pRenderMethod;
	cVector4 m_uvScaleOffset;
	D3DXHANDLE m_hUVSettings;

	// Unimplemented Functions...
	cSkyModel(const cSkyModel& Src);
	cSkyModel& operator=(const cSkyModel& Src);
};



//- End of cSkyModel ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSkyModel.h )

//----------------------------------------------------------
//$Log: $