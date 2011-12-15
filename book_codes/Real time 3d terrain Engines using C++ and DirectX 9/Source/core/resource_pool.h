#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H
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
#include "data_types.h"
#include "string.h"
#include "debug.h"
#include "data_pool.h"
#include "resource_pool_item.h"
#include "resource_pool_manager.h"

#ifndef STD_MAP_H
#define STD_MAP_H
#pragma warning(disable : 4786)
#include <map>
#endif


//	Name Space declaration
namespace gaia
{

class cResourcePoolInterface
{
public:

	union FILE_EXTENSION
	{
		char ext[4];
		uint32 fourcc;
	};

	typedef std::map<cString, cPoolHandle> NAME_LOOKUP_MAP;

	cResourcePoolInterface();
	virtual ~cResourcePoolInterface(){};

	// these functions must be proviided in the derived class (cResourceType)
	virtual void initialize(uint16 growSize)=0;
	virtual void destroy()=0;
	virtual bool isInitialized()const=0;

	virtual void destroyAll()=0;
	virtual void disableAll()=0;
	virtual void restoreAll()=0;
	virtual void clean()=0;

	cResourcePoolItem* createResource(const cString& resourceName);
	cResourcePoolItem* loadResource(const cString& filename);
	cResourcePoolItem* getResource(cPoolHandle handle);
	cResourcePoolItem* findResource(const cString& Name);
	void destroyResource(cResourcePoolItem* pResource);

	bool saveResource(cResourcePoolItem* pResource);

	cPoolHandle findResourceHandle(const cString& Name);
	const cString* findResourceName(cPoolHandle handle)const;
	void setResourceName(cPoolHandle handle, const tchar* name);

	void registerResourcePool(cResourceCode code);
	void unregisterResourcePool();
	
	cResourceCode registrationCode()const;

protected:
	cResourceCode m_registrationCode;
	NAME_LOOKUP_MAP m_nameMap;

private:
	// Private Functions...
	virtual cPoolHandle internalCreateResource(const cString& resourceName)=0;
	virtual void internalDestroyResource(cPoolHandle handle)=0;
	virtual cResourcePoolItem* internalGetResource(cPoolHandle handle)=0;
};

inline cResourcePoolInterface::cResourcePoolInterface()
:m_registrationCode(cResourceCode::INVALID_CODE)
{
	m_nameMap.clear();
};

inline void cResourcePoolInterface::registerResourcePool(cResourceCode code)
{
	m_registrationCode = code;
	ResourceManager.registerResourcePool(m_registrationCode, this);
}

inline void cResourcePoolInterface::unregisterResourcePool()
{
	ResourceManager.unregisterResourcePool(m_registrationCode);
	m_registrationCode.setInvalid();
}


inline cPoolHandle cResourcePoolInterface::findResourceHandle(const cString& Name)
{
	cPoolHandle newHandle(0);

	CLEAR_HANDLE(newHandle);

	// look up the name in our map
	NAME_LOOKUP_MAP::const_iterator iter = m_nameMap.find(Name);
	if (iter != m_nameMap.end())
	{
		newHandle = iter->second;
	}

	return newHandle;
}


inline const cString* cResourcePoolInterface::findResourceName(cPoolHandle handle)const
{
	NAME_LOOKUP_MAP::const_iterator iter = m_nameMap.begin();
	for (;iter != m_nameMap.end();++iter)
	{
		if (handle == iter->second)
		{
			return &iter->first;
		}
	}

	return 0;
}

inline void cResourcePoolInterface::setResourceName(cPoolHandle handle, const tchar* name)
{
	NAME_LOOKUP_MAP::iterator iter = m_nameMap.begin();
	for (;iter != m_nameMap.end();++iter)
	{
		if (handle == iter->second)
		{
			iter = m_nameMap.erase(iter);
			break;
		}
	}

	m_nameMap[cString(name)] = handle;
}

inline cResourcePoolItem* cResourcePoolInterface::createResource(const cString& resourceName)
{
	cPoolHandle handle = internalCreateResource(resourceName);

	if (VALID_HANDLE(handle))
	{
		cResourcePoolItem* pResource = getResource(handle);

		pResource->setResourcePool(this);
		pResource->setResourceHandle(handle);
		pResource->setResourceCode(m_registrationCode);
		return pResource;
	}

	return 0;
}

inline cResourcePoolItem* cResourcePoolInterface::loadResource(const cString& resourceName)
{
	// does the resource already exist?
	cResourcePoolItem* pResource=findResource(resourceName);

	if (!pResource)
	{
		// create a new resource
		pResource=createResource(resourceName);

		if (pResource)
		{
			// attempt to load the desired data
			if (pResource->loadResource())
			{
				// ok!
				return pResource;
			}

			// loading failed.
			// destroy the resource we created
			//destroyResource(pResource);
			pResource->Release();
			pResource=0;
		}
	}

	return pResource;
}

inline bool cResourcePoolInterface::saveResource(cResourcePoolItem* pResource)
{
	if (pResource)
	{
		// save the resource using it's own name as the file path
		return pResource->saveResource(0);
	}
	return false;
}

inline cResourcePoolItem* cResourcePoolInterface::findResource(const cString& Name)
{
	cPoolHandle newHandle(0);

	CLEAR_HANDLE(newHandle);

	// look up the name in our map
	NAME_LOOKUP_MAP::const_iterator iter = m_nameMap.find(Name);
	if (iter != m_nameMap.end())
	{
		newHandle = iter->second;

		if (VALID_HANDLE(newHandle))
		{
			cResourcePoolItem* pResource = getResource(newHandle);
			return pResource;
		}
	}
	
	return 0;
}

inline cResourcePoolItem* cResourcePoolInterface::getResource(cPoolHandle handle)
{
	cResourcePoolItem* resource = internalGetResource(handle);

	if (resource)
	{
		resource->AddRef();
	}

	return resource;
}

inline void cResourcePoolInterface::destroyResource(cResourcePoolItem* pResource)
{
	if (pResource)
	{
		int32 referenceCount = pResource->referenceCount();

		debug_assert(referenceCount==0, "destruction of non-zero reference count!");

		if (referenceCount <= 0)
		{
			cPoolHandle handle=pResource->resourceHandle();
			internalDestroyResource(handle);
		}
	}
}


template <class T>
class cResourcePool : public cResourcePoolInterface
{
public:

	typedef T DataType;
	typedef cDataPool<T> DataPool;

	// Creators...
	cResourcePool(){};
	~cResourcePool(){};

	// Base Class Overrides...
	void initialize(uint16 growSize);
	void destroy();
	bool isInitialized()const;

	void destroyAll();
	void disableAll();
	void restoreAll();
	void clean(); // delete items no longer referenced

	DataType* createResource(const cString& resourceName);
	DataType* loadResource(const cString& filename);
	DataType* getResource(cPoolHandle handle);
	DataType* findResource(const cString& Name);

	// static data pool callbacks
	static void callbackDestroy(cDataPoolInterface* pPool, cPoolHandle handle, void* resource);
	static void callbackRestore(cDataPoolInterface* pPool, cPoolHandle handle, void* resource);
	static void callbackDisable(cDataPoolInterface* pPool, cPoolHandle handle, void* resource);
	static void callbackClean(cDataPoolInterface* pPool, cPoolHandle handle, void* resource);

private:

	// Data...
	DataPool		m_dataPool;

	// Private Functions...
	cPoolHandle internalCreateResource(const cString& resourceName);
	void internalDestroyResource(cPoolHandle handle);
	cResourcePoolItem* internalGetResource(cPoolHandle handle);

	// Nonexistant Functions...

	cResourcePool( const cResourcePool& Src);
	cResourcePool& operator=( const cResourcePool& Src);
};


template <class T>
inline void cResourcePool<T>::initialize(uint16 growSize)
{
	m_dataPool.initialize(growSize);
}

template <class T>
inline void cResourcePool<T>::destroy()
{
	m_dataPool.destroy();
}

template <class T>
inline bool cResourcePool<T>::isInitialized()const
{
	return m_dataPool.isInitialized();
}

template <class T>
inline cPoolHandle cResourcePool<T>::internalCreateResource(const cString& resourceName)
{
	cPoolHandle handle(m_dataPool.nextHandle());

	// make sure this name is not already in use
	debug_assert(m_nameMap.find(resourceName)==m_nameMap.end() ,"A resource with this name already exists");

	// add this resource name to our map of handles
	m_nameMap[resourceName] = handle;

	// get a pointer to the resource and call it's creation function
	T* pResource = m_dataPool.getPtr(handle);
	pResource->createResource();
	
	return handle;
}

template <class T>
inline void cResourcePool<T>::internalDestroyResource(cPoolHandle handle)
{
	// get a pointer to the resource and call it's destruction handler
	T* pResource = m_dataPool.getPtr(handle);
	pResource->destroyResource();

	// find the resource name and remove it from the map
	NAME_LOOKUP_MAP::iterator iter = m_nameMap.begin();
	for (;iter != m_nameMap.end();++iter)
	{
		if (handle == iter->second)
		{
			iter = m_nameMap.erase(iter);
			break;
		}
	}

	// free the resource slot associated with the handle
	m_dataPool.release(&handle);
}

//
// static callbacks
//
template <class T>
void cResourcePool<T>::callbackDestroy(cDataPoolInterface* pPool, cPoolHandle handle, void* resource)
{
	((cResourcePoolItem*)resource)->destroyResource();
}

template <class T>
void cResourcePool<T>::callbackDisable(cDataPoolInterface* pPool, cPoolHandle handle, void* resource)
{
	((cResourcePoolItem*)resource)->disableResource();
}

template <class T>
void cResourcePool<T>::callbackRestore(cDataPoolInterface* pPool, cPoolHandle handle, void* resource)
{
	((cResourcePoolItem*)resource)->restoreResource();
}


template <class T>
void cResourcePool<T>::callbackClean(cDataPoolInterface* pPool, cPoolHandle handle, void* resource)
{
	// if the reference count is zero, destroy the resource and clear the handle
	if (((cResourcePoolItem*)resource)->referenceCount() == 0)
	{
		pPool->release(&handle);
	}
}


template <class T>
inline void cResourcePool<T>::destroyAll()
{
	m_dataPool.forEach(callbackDestroy);
}

template <class T>
inline void cResourcePool<T>::restoreAll()
{
	m_dataPool.forEach(callbackRestore);
}

template <class T>
inline void cResourcePool<T>::disableAll()
{
	m_dataPool.forEach(callbackDisable);
}

template <class T>
inline void cResourcePool<T>::clean()
{
	m_dataPool.forEach(callbackClean);
}

template <class T>
inline cResourcePoolItem* cResourcePool<T>::internalGetResource(cPoolHandle handle)
{
	return m_dataPool.getPtr(handle);
}


template <class T>
inline cResourcePool<T>::DataType* cResourcePool<T>::createResource(const cString& resourceName)
{
	return (DataType*)cResourcePoolInterface::createResource(resourceName);
}

template <class T>
inline cResourcePool<T>::DataType* cResourcePool<T>::loadResource(const cString& filename)
{
	return (DataType*)cResourcePoolInterface::loadResource(filename);
}

template <class T>
inline cResourcePool<T>::DataType* cResourcePool<T>::getResource(cPoolHandle handle)
{
	return (DataType*)cResourcePoolInterface::getResource(handle);
}

template <class T>
inline cResourcePool<T>::DataType* cResourcePool<T>::findResource(const cString& Name)
{
	return (DataType*)cResourcePoolInterface::findResource(Name);
}

//- End of cResourcePool ---------------------------------------------------------------

//****************************************************************************************

} //End NameSpace: ghost

#endif  // end of file      ( cResourcePool.h )

