#ifndef CCAMERA_H
#define CCAMERA_H
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
#include "scene_node.h"


//	Name Space declaration
namespace gaia
{



class cCamera : public cSceneNode
{
public:

	enum e_type
	{
		k_FOV = 0,
		k_ORTHO,
		k_OFFSET_ORTHO
	};

    cCamera();
    virtual ~cCamera();

	virtual bool create();
	virtual void update();

    void setProjParams( float fFOV, float fAspect, float fNearPlane,
                        float fFarPlane );
	void setOrthoParams(
		float width,
		float height,
		float near,
		float far);

	void setOffsetOrthoParams(
		float minX,
		float maxX,
		float minY,
		float maxY,
		float near,
		float far);

	void applyRenderStageBias(int stage);

	const cSIMDMatrix& viewMatrix()const {return m_matView;}  
	const cSIMDMatrix& projectionMatrix()const {return m_matRenderStageProj;}  
	const cSIMDMatrix& viewProjMatrix()const {return m_matRenderStageViewProj;}  
	const cSIMDMatrix& billboardMatrix()const {return m_matBillboard;}  
	const cSIMDMatrix& skyBoxMatrix()const {return m_matSkyBox;}  
 	const cSIMDMatrix& internalProjectionMatrix()const {return m_matProj;}  
	const cSIMDMatrix& internalViewProjMatrix()const {return m_matViewProj;}  

	const cVector3& targetPos()const {return m_targetPos;}  

    float fov()const {return m_fFOV;}     
    float aspect()const {return m_fAspect;}
    float nearPlane()const {return m_fNearPlane;}
    float farPlane()const {return m_fFarPlane;}
	float viewDistance()const {return m_fFarPlane-m_fNearPlane;}

	const cRect3d& searchRect()const {return m_searchRect;}
	const cVector3* farPlanePoints()const {return m_farPlanePoints;}
	const cFrustum& worldFrustum()const {return m_frustum;}

protected:

	e_type		m_type;

    cSIMDMatrix  m_matView;		
    cSIMDMatrix  m_matProj;		// internal, un-biased projection matrix
    cSIMDMatrix  m_matUnitProj;
    cSIMDMatrix  m_matViewProj; // internal, un-biased view+projection matrix
	cSIMDMatrix  m_matBillboard; // Special matrix for billboarding effects
	cSIMDMatrix  m_matSkyBox; // Special matrix for sky box effects

    cSIMDMatrix  m_matRenderStageProj;		// biased for use during current render stage
    cSIMDMatrix  m_matRenderStageViewProj;  // biased for use during current render stage

	cRect3d		 m_searchRect;
    cVector3	 m_targetPos;

    float       m_fFOV;         // Attributes for projection matrix
    float       m_fAspect;
    float       m_fNearPlane;
    float       m_fFarPlane;
    float       m_fWidth;
    float       m_fHeight;
    float       m_fMinX;
    float       m_fMaxX;
    float       m_fMinY;
    float       m_fMaxY;

	cVector3	m_farPlanePoints[8];
	cFrustum	m_frustum;

	// Private Functions...
	void recalcMatrices();

    explicit cCamera(const cCamera& Src);
    cCamera& operator=(const cCamera& Src);
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cCamera
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cCamera::cCamera()
:cSceneNode()
,m_type(k_FOV)
,m_fFOV(D3DX_PI/5)
,m_fAspect(640/480)
,m_fNearPlane(0.1f)
,m_fFarPlane(500.0f)
{
}

/*	~cCamera
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cCamera::~cCamera()
{
	destroy();
}


//- End of cCamera ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cCamera.h )

//----------------------------------------------------------
//$Log: $