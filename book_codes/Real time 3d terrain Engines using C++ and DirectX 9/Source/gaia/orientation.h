#ifndef CORIENTATION_H
#define CORIENTATION_H
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


class cOrientation
{
public:

	// Data Types & Constants...
	enum e_flag_bits
	{
		k_translation,
		k_rotation,
		k_scale,
	};

	// Public Data...

	// Creators...
    cOrientation();
    cOrientation(const cOrientation& Src);
    ~cOrientation(){};

	// Operators...
    cOrientation& operator=(const cOrientation& Src);

	// Mutators...
	void setIdentity();

	void clearPosition();
	void clearRotation();
	void clearScale();

	void setRelRotation(const cMatrix4x4& matrix);
	void addRelRotation(float yaw, float pitch, float roll);
	void addRelRotation(const cMatrix4x4& matrix);

	void setRelPosition(const cVector3& pos);
	void addRelPosition(const cVector3& pos);

	void setPosition(const cVector3& pos);
	void setRotation(const cVector3& forward, const cVector3& up);
	void setRotation(const cVector3& axis, float angle);
	void setRotation(float yaw, float pitch, float roll);
	void setRotation(const cMatrix4x4& matrix);
	void setScale(const cVector3& scale);
	void setScale(float scale);

	void addPosition(const cVector3& pos);
	void addRotation(const cVector3& forward, const cVector3& up);
	void addRotation(const cVector3& axis, float angle);
	void addRotation(float yaw, float pitch, float roll);
	void addRotation(const cMatrix4x4& matrix);
	void addScale(const cVector3& scale);
	void addScale(float scale);

	// Accessors...
	const cVector3& getPosition()const;
	const cVector3& getScale()const;
	const cVector3& getForward()const;
	const cVector3& getRight()const;
	const cVector3& getUp()const;
	const u16Flags& typeFlags()const;
	const u16Flags& dirtyFlags()const;

	void buildMatrix(cMatrix4x4& matrix);
	bool updateMatrix(cMatrix4x4& matrix, bool clearDirtyFlag = true);

private:

	// Private Data...
	cVector3	m_right;
	cVector3	m_up;
	cVector3	m_forward;
	cVector3	m_position;
	cVector3	m_scale;
	u16Flags	m_typeFlags;
	u16Flags	m_dirtyFlags;

	// Private Functions...

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

inline cOrientation::cOrientation()
:m_right(1.0f,0.0f,0.0f)
,m_up(0.0f,1.0f,0.0f)
,m_forward(0.0f,0.0f,1.0f)
,m_position(0.0f,0.0f,0.0f)
,m_scale(1.0f,1.0f,1.0f)
,m_typeFlags(0)
,m_dirtyFlags(0)
{
}

/*	cOrientation(const cOrientation& Src)
-----------------------------------------------------------------
    
    Copy Constructor
    
-----------------------------------------------------------------
*/
inline cOrientation::cOrientation(const cOrientation& Src)
:m_right(Src.m_right)
,m_up(Src.m_up)
,m_forward(Src.m_forward)
,m_position(Src.m_position)
,m_scale(Src.m_scale)
,m_typeFlags(Src.m_typeFlags)
,m_dirtyFlags(Src.m_dirtyFlags)
{
}

//. Operators ...................................................

/*	operator=(const cOrientation& Src)
-----------------------------------------------------------------
    
    Copy Operator
    
-----------------------------------------------------------------
*/
inline cOrientation& cOrientation::operator=(const cOrientation& Src)
{
	m_position = Src.m_position;
	m_right = Src.m_right;
	m_up = Src.m_up;
	m_forward = Src.m_forward;
	m_scale = Src.m_scale;
	m_typeFlags = Src.m_typeFlags;
	m_dirtyFlags = Src.m_dirtyFlags;

    return(*this);
}

//. Accessors ...................................................
inline const cVector3& cOrientation::getPosition()const
{
	return m_position;
}

inline const cVector3& cOrientation::getScale()const
{
	return m_scale;
}

inline const cVector3& cOrientation::getForward()const
{
	return m_forward;
}

inline const cVector3& cOrientation::getRight()const
{
	return m_right;
}

inline const cVector3& cOrientation::getUp()const
{
	return m_up;
}

inline const u16Flags& cOrientation::typeFlags()const
{
	return m_typeFlags;
}

inline const u16Flags& cOrientation::dirtyFlags()const
{
	return m_dirtyFlags;
}

//- End of cOrientation -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cOrientation.h )

//----------------------------------------------------------
//$Log: $