#define CRESOURCE_CPP
#define CORE_DLL
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

#include "resource_pool_item.h"
#include "resource_pool.h"

using namespace gaia;

void cResourcePoolItem::setResourceName(const tchar* name)
{
	if (m_resourcePool)
	{
		m_resourcePool->setResourceName(m_resourceHandle, name);
	}
}

const cString* cResourcePoolItem::findResourceName()const
{
	if (m_resourcePool)
	{
		return m_resourcePool->findResourceName(m_resourceHandle);
	}

	return 0;
}

int32 cResourcePoolItem::Release()
{
	int32 refCount = cReferenceCounter::Release();
	
	if (refCount == 0)
	{
		// if we have a manager, ask it to kill us
		if (m_resourcePool)
		{
			m_resourcePool->destroyResource(this);
		}
	}

	return refCount;
}

//***************************************************************
// end of file      ( cResourcePoolItem.cpp )

//----------------------------------------------------------
//$Log: $