#define RESOURCEMANAGER_CPP
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

#include "resource_pool_manager.h"
#include "resource_pool.h"

using namespace gaia;


cResourcePoolManager::cResourcePoolManager()
:cSingleton<cResourcePoolManager>(*this)
{
	// paranoid pre-cleaning of STL collections
	m_resourceTypeMap.clear();
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		m_resourceFamilyList[i].clear();
	}
};

void cResourcePoolManager::registerResourcePool(cResourceCode code, cResourcePoolInterface* pInterface)
{
	debug_assert(pInterface, "invalid cResourcePoolInterface pointer");
	debug_assert(code.family >= 0, "invalid family index");
	debug_assert(code.family < k_nTotalResourceFamilies, "invalid family index");
	debug_assert(m_resourceTypeMap.find(code) == m_resourceTypeMap.end(), "Resource type code already registered");

	m_resourceTypeMap[code] = pInterface;
	m_resourceFamilyList[code.family].push_back(pInterface);
}

cResourcePoolInterface* cResourcePoolManager::unregisterResourcePool(cResourceCode code)
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
void cResourcePoolManager::destroyAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		destroyResourceFamily(i);
	}
}

void cResourcePoolManager::restoreAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		restoreResourceFamily(i);
	}
}

void cResourcePoolManager::disableAll()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		disableResourceFamily(i);
	}
}

void cResourcePoolManager::clean()
{
	for(int i=0; i<k_nTotalResourceFamilies; ++i)
	{
		cleanResourceFamily(i);
	}
}


// operations on specific resource types
void cResourcePoolManager::destroyResourceFamily(int family)
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

void cResourcePoolManager::restoreResourceFamily(int family)
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

void cResourcePoolManager::disableResourceFamily(int family)
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

void cResourcePoolManager::cleanResourceFamily(int family)
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
 

void cResourcePoolManager::destroyResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->destroyAll();
	}
}

void cResourcePoolManager::restoreResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->restoreAll();
	}
}

void cResourcePoolManager::disableResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->disableAll();
	}
}

void cResourcePoolManager::cleanResourceType(cResourceCode code)
{
	// find the code in our map
	ResourcePoolTypeMap::iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		(*iter).second->clean();
	}
}
 

cResourcePoolInterface* cResourcePoolManager::findResourcePool(cResourceCode code)const
{
	cResourcePoolInterface* result = 0;
	ResourcePoolTypeMap::const_iterator iter= m_resourceTypeMap.find(code);
	if (iter != m_resourceTypeMap.end())
	{
		result = (*iter).second;
	}
	return result;
}

cPoolHandle cResourcePoolManager::findResourceHandle(cResourceCode code, const cString& Name)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cPoolHandle handle;

	CLEAR_HANDLE(handle);

	if (pool)
	{
		handle = pool->findResourceHandle(Name);
	}

	return handle;
}

cResourcePoolItem* cResourcePoolManager::findResource(cResourceCode code, const cString& Name)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cResourcePoolItem* result=0;

	if (pool)
	{
		cPoolHandle handle = pool->findResourceHandle(Name);
		if (VALID_HANDLE(handle))
		{
			result = pool->getResource(handle);
		}
	}

	return result;
}

cResourcePoolItem* cResourcePoolManager::findResource(cResourceCode code, cPoolHandle handle)const
{
	cResourcePoolInterface* pool = findResourcePool(code);
	cResourcePoolItem* result=0;

	if (pool)
	{
		if (VALID_HANDLE(handle))
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