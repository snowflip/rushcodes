#ifndef CGRASSPATCH_H
#define CGRASSPATCH_H
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
#include "gaia\scene_model.h"


//	Name Space declaration
namespace gaia
{

class cTerrain;


class cGrassPatch : public cSceneModel
{
public:

	// Creators...
  cGrassPatch();
  ~cGrassPatch();

	void setup(cTerrain* pTerrain, uint16 x, uint16 y);
	void renderCallback(cRenderEntry* entry, u32Flags activationFlags);
	virtual cSceneNode* createSubNode(cModelResource* pModelResource, int frameIndex);

	void setPoints(cVector4* pPoints);

private:

	cVector4 m_points[16];
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cGrassPatch
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cGrassPatch::cGrassPatch()
:cSceneModel()
{
}

/*	~cGrassPatch
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cGrassPatch::~cGrassPatch()
{
}


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cGrassPatch.h )

//----------------------------------------------------------
//$Log: $