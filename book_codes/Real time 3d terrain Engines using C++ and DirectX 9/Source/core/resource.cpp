#define CRESOURCE_CPP
#define CORE_DLL
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    $Workfile: $

    sample code from the book...
    3D Terrain Engines From the Ground Up.

    Created 12/18/2002 by Greg Snook
    greg@mightystudios.com

    ------------------------------------------------------------ 
    Last Revision:
    $Author: $
    $Date: $
    $Revision: $

\***************************************************************/

#include "resource.h"
#include "resource_pool.h"

using namespace gaia;

void cResource::setResourceName(const tchar* name)
{
	if (m_resourcePool)
	{
		m_resourcePool->setResourceName(m_resourceHandle, name);
	}
}

const cString* cResource::findResourceName()const
{
	if (m_resourcePool)
	{
		return m_resourcePool->findResourceName(m_resourceHandle);
	}

	return 0;
}

int32 cResource::Release()
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
// end of file      ( cResource.cpp )

//----------------------------------------------------------
//$Log: $