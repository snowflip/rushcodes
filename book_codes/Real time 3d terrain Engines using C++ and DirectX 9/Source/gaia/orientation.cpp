#define CORIENTATION_CPP
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

#include "orientation.h"

using namespace gaia;

void cOrientation::setIdentity()
{
	if (m_typeFlags.value != 0)
	{
		m_position.set(0.0f,0.0f,0.0f);
		m_right.set(1.0f,0.0f,0.0f);
		m_up.set(0.0f,1.0f,0.0f);
		m_forward.set(0.0f,0.0f,1.0f);
		m_scale.set(0.0f,0.0f,0.0f);
		
		m_dirtyFlags.value = 0;

		if (TEST_BIT(m_typeFlags,k_translation))
		{
			m_dirtyFlags.setBit(k_translation);
		}

		if (TEST_BIT(m_typeFlags,k_rotation))
		{
			m_dirtyFlags.setBit(k_rotation);
		}

		if (TEST_BIT(m_typeFlags,k_scale))
		{
			m_dirtyFlags.setBit(k_scale);
		}

		m_typeFlags.value = 0;
	}
}

void cOrientation::clearPosition()
{
	if (TEST_BIT(m_typeFlags,k_translation))
	{
		m_position.set(0.0f,0.0f,0.0f);
		m_dirtyFlags.setBit(k_translation);
		m_typeFlags.clearBit(k_translation);
	}
}

void cOrientation::clearRotation()
{
	if (TEST_BIT(m_typeFlags,k_rotation))
	{
		m_right.set(1.0f,0.0f,0.0f);
		m_up.set(0.0f,1.0f,0.0f);
		m_forward.set(0.0f,0.0f,1.0f);
		m_dirtyFlags.setBit(k_rotation);
		m_typeFlags.clearBit(k_rotation);
	}
}

void cOrientation::clearScale()
{
	if (TEST_BIT(m_typeFlags,k_scale))
	{
		m_scale.set(1.0f,0.0f,0.0f);
		m_dirtyFlags.setBit(k_scale);
		m_typeFlags.clearBit(k_scale);
	}
}

void cOrientation::setPosition(const cVector3& pos)
{
	m_position = pos;
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::setRelPosition(const cVector3& pos)
{
	m_position = 
		(m_right*pos.x)
		+(m_up*pos.y)
		+(m_forward*pos.z);
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::addRelPosition(const cVector3& pos)
{
	m_position += 
		(m_right*pos.x)
		+(m_up*pos.y)
		+(m_forward*pos.z);
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::setRotation(const cMatrix4x4& rotation)
{
	m_right.set(rotation._11, rotation._12, rotation._13);
	m_up.set(rotation._21, rotation._22, rotation._23);
	m_forward.set(rotation._31, rotation._32, rotation._33);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::setRelRotation(const cMatrix4x4& rotation)
{
	m_right.set(rotation._11, rotation._21, rotation._31);
	m_up.set(rotation._12, rotation._22, rotation._32);
	m_forward.set(rotation._13, rotation._23, rotation._33);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::setRotation(const cVector3& forward, const cVector3& up)
{
	m_forward = forward;
	m_up = up;
	m_right.crossProduct(m_up, forward);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);
}

void cOrientation::setRotation(const cVector3& axis, float angle)
{
	cMatrix4x4 rotation;
	D3DXMatrixRotationAxis(&rotation, &axis, angle);
	setRotation(rotation);
}

void cOrientation::setRotation(float yaw, float pitch, float roll)
{
	cMatrix4x4 rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	setRotation(rotation);
}

void cOrientation::addRelRotation(float yaw, float pitch, float roll)
{
	cMatrix4x4 rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	addRelRotation(rotation);
}

void cOrientation::setScale(const cVector3& scale)
{
	m_scale = scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::setScale(float scale)
{
	m_scale.set(scale, scale, scale);
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::addPosition(const cVector3& pos)
{
	m_position += pos;
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::addRotation(const cMatrix4x4& rotation)
{
	m_right.set(
		dotProduct(m_right, cVector3(rotation._11, rotation._21, rotation._31)),
		dotProduct(m_right, cVector3(rotation._12, rotation._22, rotation._32)),
		dotProduct(m_right, cVector3(rotation._13, rotation._23, rotation._33)));

	m_forward.set(
		dotProduct(m_forward, cVector3(rotation._11, rotation._21, rotation._31)),
		dotProduct(m_forward, cVector3(rotation._12, rotation._22, rotation._32)),
		dotProduct(m_forward, cVector3(rotation._13, rotation._23, rotation._33)));

	m_up.set(
		dotProduct(m_up, cVector3(rotation._11, rotation._21, rotation._31)),
		dotProduct(m_up, cVector3(rotation._12, rotation._22, rotation._32)),
		dotProduct(m_up, cVector3(rotation._13, rotation._23, rotation._33)));

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}


void cOrientation::addRotation(const cVector3& forward, const cVector3& up)
{
	cVector3 right;
	right.crossProduct(forward, up);

	m_right.set(
		dotProduct(m_right, cVector3(right.x, up.x, forward.x)),
		dotProduct(m_right, cVector3(right.y, up.y, forward.y)),
		dotProduct(m_right, cVector3(right.z, up.z, forward.z)));

	m_forward.set(
		dotProduct(m_forward, cVector3(right.x, up.x, forward.x)),
		dotProduct(m_forward, cVector3(right.y, up.y, forward.y)),
		dotProduct(m_forward, cVector3(right.z, up.z, forward.z)));

	m_up.set(
		dotProduct(m_up, cVector3(right.x, up.x, forward.x)),
		dotProduct(m_up, cVector3(right.y, up.y, forward.y)),
		dotProduct(m_up, cVector3(right.z, up.z, forward.z)));

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);
}

void cOrientation::addRelRotation(const cMatrix4x4& rotation)
{
	cVector3 col0(m_right.x, m_up.x, m_forward.x);
	cVector3 col1(m_right.y, m_up.y, m_forward.y);
	cVector3 col2(m_right.z, m_up.z, m_forward.z);

	m_right.set(
		dotProduct(col0, cVector3(rotation._11, rotation._12, rotation._13)),
		dotProduct(col1, cVector3(rotation._11, rotation._12, rotation._13)),
		dotProduct(col2, cVector3(rotation._11, rotation._12, rotation._13)));

	m_up.set(
		dotProduct(col0, cVector3(rotation._21, rotation._22, rotation._23)),
		dotProduct(col1, cVector3(rotation._21, rotation._22, rotation._23)),
		dotProduct(col2, cVector3(rotation._21, rotation._22, rotation._23)));

	m_forward.set(
		dotProduct(col0, cVector3(rotation._31, rotation._32, rotation._33)),
		dotProduct(col1, cVector3(rotation._31, rotation._32, rotation._33)),
		dotProduct(col2, cVector3(rotation._31, rotation._32, rotation._33)));


	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::addRotation(const cVector3& axis, float angle)
{
	cMatrix4x4 rotation;

	D3DXMatrixRotationAxis(&rotation, &axis, angle);
	addRotation(rotation);
}

void cOrientation::addRotation(float yaw, float pitch, float roll)
{
	cMatrix4x4 rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	addRotation(rotation);
}

void cOrientation::addScale(const cVector3& scale)
{
	m_scale += scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::addScale(float scale)
{
	m_scale.x += scale;
	m_scale.y += scale;
	m_scale.z += scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::buildMatrix(cMatrix4x4& matrix)
{
	matrix._11 = m_right.x * m_scale.x;
	matrix._12 = m_right.y;
	matrix._13 = m_right.z;
	matrix._14 = 0.0f;

	matrix._21 = m_up.x;
	matrix._22 = m_up.y * m_scale.y;
	matrix._23 = m_up.z;
	matrix._24 = 0.0f;

	matrix._31 = m_forward.x;
	matrix._32 = m_forward.y;
	matrix._33 = m_forward.z * m_scale.z;
	matrix._34 = 0.0f;

	matrix._41 = m_position.x;
	matrix._42 = m_position.y;
	matrix._43 = m_position.z;
	matrix._44 = 1.0f;
}

bool cOrientation::updateMatrix(cMatrix4x4& matrix, bool clearDirtyFlag)
{
	if (m_dirtyFlags.value != 0)
	{
		buildMatrix(matrix);

		if (clearDirtyFlag)
		{
			m_dirtyFlags.value = 0;
		}
		return true;
	}
	return false;
}

//***************************************************************
// end of file      ( cOrientation.cpp )

//----------------------------------------------------------
//$Log: $