#ifndef CLENSFLARE_H
#define CLENSFLARE_H
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
#include <math\math.h>
#include <geometry\geometry.h>


//	Name Space declaration
namespace gaia
{

class cVertexBuffer;
class cIndexBuffer;
class cSurfaceMaterial;
class cEffectFile;
class cCamera;


class cLensFlare
{
public:

	enum e_constants
	{
		k_totalFlares = 9,
	};

	struct sFlareElement
	{
		float linePos;		// position on line (-1 to 1)
		float scale;		// screen-based scale
		float brightness;	// emissive property (0-1)
		float texU;		// x location in texture
		float texV;		// y location in texture
	};

	// Public Data...

	// Creators...

    cLensFlare();
    ~cLensFlare(){};

	void create();
	void destroy();
	void update(const cVector3& sunlightVector);
	void render();

private:

	cVertexBuffer* m_pVertexBuffer;
	cIndexBuffer* m_pIndexBuffer;
	cSurfaceMaterial* m_pSurfaceMaterial;
	cEffectFile* m_pEffectFile;
	sFlareElement m_flareElements[k_totalFlares];
	bool m_visible;
	cVector3 m_screenPos;
	cVector4 m_flareColor;
	
	// Private Functions...

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cLensFlare
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cLensFlare::cLensFlare()
:m_pVertexBuffer(0)
,m_pIndexBuffer(0)
,m_pSurfaceMaterial(0)
,m_pEffectFile(0)
,m_visible(false)
{
}

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cLensFlare.h )

//----------------------------------------------------------
//$Log: $