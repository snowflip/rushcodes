#define CCAMERA_CPP
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

#include "camera.h"

using namespace gaia;


bool cCamera::create()
{
	bool result=cSceneNode::create();

	if (result)
	{
		m_targetPos = orientation().getForward();
		m_targetPos.negate();

		setProjParams(
			m_fFOV, 
			m_fAspect,
			m_fNearPlane,
			m_fFarPlane);

		recalcMatrices();
	}
	return result;
}

void cCamera::setProjParams( 
	float fFOV, 
	float fAspect, 
	float fNearPlane,
    float fFarPlane )
{
    // Set attributes for the projection matrix
    m_fFOV        = fFOV;
    m_fAspect     = fAspect;
    m_fNearPlane  = fNearPlane;
    m_fFarPlane   = fFarPlane;
	m_type = k_FOV;

	// create the regular projection matrix
    D3DXMatrixPerspectiveFovLH( 
		&m_matProj, 
		fFOV, 
		fAspect, 
		fNearPlane, 
		fFarPlane );

	// create a unit-space matrix 
	// for sky box geometry.
	// this ensures that the 
	// near and far plane enclose 
	// the unit space around the camera
    D3DXMatrixPerspectiveFovLH( 
		&m_matUnitProj, 
		fFOV, 
		fAspect, 
		0.01f, 
		2.0f );
}

void cCamera::setOrthoParams(
	float fWidth,
	float fHeight,
	float fNearPlane,
	float fFarPlane)
{
    m_fWidth = fWidth;
    m_fHeight = fHeight;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;
	m_type = k_ORTHO;

	// create the regular projection matrix
    D3DXMatrixOrthoLH( 
		&m_matProj, 
		fWidth, 
		fHeight, 
		fNearPlane, 
		fFarPlane);

	// create a unit-space matrix 
	// for sky box geometry.
	// this ensures that the 
	// near and far plane enclose 
	// the unit space around the camera
    D3DXMatrixOrthoLH( 
		&m_matUnitProj, 
		fWidth, 
		fHeight, 
		0.01f, 
		2.0f );
}

void cCamera::setOffsetOrthoParams(
	float minX,
	float maxX,
	float minY,
	float maxY,
	float fNearPlane,
	float fFarPlane)
{
	m_fMinX = minX;
	m_fMaxX = maxX;
	m_fMinY = minY;
	m_fMaxY = maxY;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;
	m_type = k_OFFSET_ORTHO;

	// create the regular projection matrix
    D3DXMatrixOrthoOffCenterLH( 
		&m_matProj, 
		minX, 
		maxX, 
		minY, 
		maxY, 
		fNearPlane, 
		fFarPlane);

	// create a unit-space matrix 
	// for sky box geometry.
	// this ensures that the 
	// near and far plane enclose 
	// the unit space around the camera
    D3DXMatrixOrthoOffCenterLH( 
		&m_matUnitProj, 
		minX, 
		maxX, 
		minY, 
		maxY, 
		0.01f, 
		2.0f );
}

void cCamera::recalcMatrices()
{
	m_targetPos = orientation().getForward();
	m_targetPos.negate();

	// the camera view matrix is the
	// inverse of the world matrix
	m_matView = inverseWorldMatrix();

	// sky boxes use the inverse 
	// world matrix of the camera (the
	// camera view matrix) without 
	// any translation information.
	m_matSkyBox = m_matView;
	m_matSkyBox._41 = 0.0f;
	m_matSkyBox._42 = 0.0f;
	m_matSkyBox._43 = 0.0f;

	// this is combined with the unit
	// space projection matrix to form
	// the sky box viewing matrix
	D3DXMatrixMultiply(
		&m_matSkyBox, 
		&m_matSkyBox, 
		&m_matUnitProj);

	// billboard objects use our world matrix
	// without translation
	m_matBillboard = worldMatrix();
	m_matBillboard._41 = 0.0f;
	m_matBillboard._42 = 0.0f;
	m_matBillboard._43 = 0.0f;

	// our view proj matrix is the inverse of our world matrix
	// multiplied by the projection matrix
	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);

	D3DXMATRIX invProj;
	D3DXMATRIX invCamera;
	D3DXMatrixInverse(&invProj, NULL, &m_matProj);
	D3DXMatrixMultiply(&invCamera, &invProj, &worldMatrix());

	cVector3 worldPos = worldPosition();

	static cVector3 p0(-1.0f,1.0f,1.0f);
	static cVector3 p1(-1.0f,-1.0f,1.0f);
	static cVector3 p2(1.0f,-1.0f,1.0f);
	static cVector3 p3(1.0f,1.0f,1.0f);
	static cVector3 p4(-1.0f,1.0f,0.0f);
	static cVector3 p5(-1.0f,-1.0f,0.0f);
	static cVector3 p6(1.0f,-1.0f,0.0f);
	static cVector3 p7(1.0f,1.0f,0.0f);

	D3DXVec3TransformCoord(&m_farPlanePoints[0], &p0, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[1], &p1, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[2], &p2, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[3], &p3, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[4], &p4, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[5], &p5, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[6], &p6, &invCamera);
	D3DXVec3TransformCoord(&m_farPlanePoints[7], &p7, &invCamera);

	// build a box around our frustum
	m_searchRect.set(
		worldPos.x, worldPos.x, 
		worldPos.y, worldPos.y, 
		worldPos.z, worldPos.z);

	m_searchRect.unionPoint(m_farPlanePoints[0]);
	m_searchRect.unionPoint(m_farPlanePoints[1]);
	m_searchRect.unionPoint(m_farPlanePoints[2]);
	m_searchRect.unionPoint(m_farPlanePoints[3]);
	m_searchRect.unionPoint(m_farPlanePoints[4]);
	m_searchRect.unionPoint(m_farPlanePoints[5]);
	m_searchRect.unionPoint(m_farPlanePoints[6]);
	m_searchRect.unionPoint(m_farPlanePoints[7]);

	m_frustum.extractFromMatrix(m_matViewProj);

	m_matRenderStageProj = m_matProj;
	m_matRenderStageViewProj = m_matViewProj;

}

void cCamera::update()
{
	cSceneNode::update();

	if (isWorldMatrixNew())
	{
		recalcMatrices();
	}
}

void cCamera::applyRenderStageBias(int stage)
{
	float z_bias = stage > 1 ? 0.001f:0.0f;

	m_matRenderStageProj = m_matProj;
	m_matRenderStageViewProj = m_matViewProj;

	m_matRenderStageProj._43 -= z_bias;
	m_matRenderStageViewProj._43 -= z_bias;
}

//***************************************************************
// end of file      ( cCamera.cpp )

//----------------------------------------------------------
//$Log: $