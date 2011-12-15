#define RESOURCEMANAGER_CPP
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

    Created 10/12/2002 by Greg Snook
    greg@mightystudios.com

    ------------------------------------------------------------ 
    Last Revision:
    $Author: $
    $Date: $
    $Revision: $

\***************************************************************/

#include "resource_manager.h"
#include "resource_pool.h"

using namespace gaia;


cResourceManager::cResourceManager()
:cSingleton<cResourceManager>(*this)
{
	// paranoid pre-cleaning of STL collections
	m_resourceTypeMap.clear();
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		m_resourceFamilyList[i].clear();
	}
};

void cResourceManager::registerResourcePool(cResourceCode code, cResourcePoolInterface* pInterface)
{
	debug_assert(pInterface, "invalid cResourcePoolInterface pointer");
	debug_assert(code.family >= 0, "invalid family index");
	debug_assert(code.family < k_nTotalResourceFamilies, "invalid family index");
	debug_assert(m_resourceTypeMap.find(code) == m_resourceTypeMap.end(), "Resource type code already registered");

	m_resourceTypeMap[code] = pInterface;
	m_resourceFamilyList[code.family].push_back(pInterface);
}

cResourcePoolInterface* cResourceManager::unregisterResourcePool(cResourceCode code)
{
	debug_assert(code.family >= 0, "invalid family index");
	debug_assert(code.family < k_nTotalResourceFamilies, "invalid family index");

	cResourcePoolInterface* pInterface = 0;
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		pInterface = (*iter).second;
		m_resourceTypeMap.erase(iter);
	}

	if (pInterface)
	{
		for(ResourcePoolFamilyList::iterator listIter = m_resourceFamilyList[code.family].begin();
			listIter != m_resourceFamilyList[code.family].end();
			++listIter)
		{
			if ((*listIter) == pInterface)
			{
				m_resourceFamilyList[code.family].erase(listIter);
				return(pInterface);
			}
		}
	}
	return(pInterface);
}


// operations for all resource types
void cResourceManager::destroyAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		destroyResourceFamily(i);
	}
}

void cResourceManager::restoreAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		restoreResourceFamily(i);
	}
}

void cResourceManager::disableAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		disableResourceFamily(i);
	}
}

void cResourceManager::clean()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		cleanResourceFamily(i);
	}
}


// operations on specific resource types
void cResourceManager::destroyResourceFamily(int family)
{
	debug_assert(family >= 0, "invalid family index");
	debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for(ResourcePoolFamilyList::iterator iter = m_resourceFamilyList[family].begin();
		iter != m_resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->destroyAll();
	}
}

void cResourceManager::restoreResourceFamily(int family)
{
	debug_assert(family >= 0, "invalid family index");
	debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for(ResourcePoolFamilyList::iterator iter = m_resourceFamilyList[family].begin();
		iter != m_resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->restoreAll();
	}
}

void cResourceManager::disableResourceFamily(int family)
{
	debug_assert(family >= 0, "invalid family index");
	debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for(ResourcePoolFamilyList::iterator iter = m_resourceFamilyList[family].begin();
		iter != m_resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->disableAll();
	}
}

void cResourceManager::cleanResourceFamily(int family)
{
	debug_assert(family >= 0, "invalid family index");
	debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for(ResourcePoolFamilyList::iterator iter = m_resourceFamilyList[family].begin();
		iter != m_resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->clean();
	}
}
 

void cResourceManager::destroyResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->destroyAll();
	}
}

void cResourceManager::restoreResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->restoreAll();
	}
}

void cResourceManager::disableResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->disableAll();
	}
}

void cResourceManager::cleanResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->clean();
	}
}
 

cResourcePoolInterface* cResourceManager::findResourcePool(cResourceCode code)const
{
	cResourcePoolInterface* result = 0;
	ResourcePoolTypeMap::const_iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		result = (*iter).second;
	}
	return result;
}

cPoolHandle cResourceManager::findResourceHandle(cResourceCode code, const cString& Name)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cPoolHandle handle;

	handle.clear();

	if (pool)
	{
		handle = pool->findResourceHandle(Name);
	}

	return handle;
}

cResource* cResourceManager::findResource(cResourceCode code, const cString& Name)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cResource* result=0;

	if (pool)
	{
		cPoolHandle handle = pool->findResourceHandle(Name);
		if (handle.isValid())
		{
			result = pool->getResource(handle);
		}
	}

	return result;
}

cResource* cResourceManager::findResource(cResourceCode code, cPoolHandle handle)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cResource* result=0;

	if (pool)
	{
		if (handle.isValid())
		{
			result = pool->getResource(handle);
		}
	}

	return result;
}

//***************************************************************
// end of file      ( resource_registry.cpp )

//----------------------------------------------------------
//$Log: $