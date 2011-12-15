#ifndef RESOURCE_H
#define RESOURCE_H
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
#include "bit_flags.h"
#include "reference_counter.h"
#include "file.h"
#include "data_pool.h"

//	Name Space declaration
namespace gaia
{

// forward declarations
class cResourcePoolInterface;



/*	cResourceCode
-----------------------------------------------------------------
    
    A resource code is a 32bit identifier for a resource type
    
-----------------------------------------------------------------
*/
class cResourceCode
{
public:

	enum
	{
		INVALID_CODE = 0xffffffff,
	};

	union
	{
		struct
		{
			uint16		family;
			uint16		type;
		};
		uint32 value;
	};

    cResourceCode():value(INVALID_CODE){};
    cResourceCode(const cResourceCode& src):value(src.value){};
    cResourceCode(uint32 src):value(src){};
    cResourceCode(uint16 _family, uint16 _type):family(_family),type(_type){};
	~cResourceCode(){};

    cResourceCode& operator=(const cResourceCode& src)
	{
		value = src.value;
		return *this;
	};

	// operater < is provided for sorting purposes
    bool operator<(const cResourceCode& rhs)
	{
		return value < rhs.value;
	};

	operator uint32()const {return value;}

	void setInvalid() {value = INVALID_CODE;}
};


/*	cResourcePoolItem
------------------------------------------------------------------------------------------
	
	A cResourcePoolItem is a simple base class for a shared resource such as a texture, 
	animation or vertex buffer. 

	Note that cResourcePoolItem is derived from cReferenceCounter. The cResourcePoolManager uses
	the value of the cReferenceCounter to decide when objects are no longer in use and can
	be destroyed during the cResourcePoolManager::clean() function. This means the application
	must update the reference count of the cResourcePoolItem whenever links between objects
	are created or removed between objects. The cReferenceCounter base class provides the
	functionality to manipulate the reference count.
	
------------------------------------------------------------------------------------------
*/

class cResourcePoolItem : public cReferenceCounter
{
public:

	// Data Types & Constants...
	enum
	{
		nCreated=0,	// the resource has been created
		nLoaded,	// the resource is filled with data and ready for use
		nDisabled,	// the resource is currently disabled for use
		nAltered,	// the resource has been altered since loaded
		nTotalResourceFlags
	};

	// Creators...
	cResourcePoolItem();
	virtual ~cResourcePoolItem();

	// User Functions
	// These function prototypes are to be used by resources that use volatile or system-specific resources.
	// Examples would be video-memory resident textures or hardware vertex buffers.
	virtual bool createResource()=0;					// innitialize the resource (called once)
	virtual bool destroyResource()=0;					// destroy the resource
	virtual bool disableResource()=0;					// purge the resource from volatile memory
	virtual bool restoreResource()=0;					// restore the resource to volatile memory
	virtual bool loadResource(const tchar* filename=0)=0; // load the resource from a file (or NULL to use the resource name)
	virtual bool saveResource(const tchar* filename=0)=0;	// save the resource to a file (or NULL to use the resource name)

	// Accessors...
	cResourceCode			resourceCode()const;
	cResourcePoolInterface* resourcePool()const;
	cPoolHandle				resourceHandle()const;
	u32Flags				resourceFlags()const;

	bool isResourceCreated()const;
	bool isResourceDisabled()const;
	bool isResourceLoaded()const;
	const cString* findResourceName()const;

	void setResourceName(const tchar* name);
	void setAlteredFlag(bool onOff);
	bool alteredFlag()const;

	// mimic COM interfaces
    virtual int32 Release();

protected:

	// only derrived classes are permitted to modify internal members
	void setResourceCode(cResourceCode code);
	void setResourcePool(cResourcePoolInterface* interfaePtr);
	void setResourceHandle(cPoolHandle handle);
	void setResourceFlag(int flagBit, bool Setting);

	void notifyCreated();
	void notifyDisabled();
	void notifyLoaded();
	void notifyUnloaded();
	void notifyRestored();
	void notifyDestroyed();
	void notifySaved();

private:

	// Data...
	cResourceCode			m_resourceCode;
	cResourcePoolInterface* m_resourcePool;
	cPoolHandle				m_resourceHandle;
	u32Flags				m_resourceFlags;

	// Private Functions...

	// Nonexistant Functions...
	cResourcePoolItem( const cResourcePoolItem& Src);
	cResourcePoolItem& operator=( const cResourcePoolItem& Src);

	friend cResourcePoolInterface;
};

inline cResourcePoolItem::cResourcePoolItem()
:m_resourceCode(0)
,m_resourcePool(0)
,m_resourceHandle(0)
,m_resourceFlags(0)
{
}

inline cResourcePoolItem::~cResourcePoolItem()
{
//	debug_assert(!isResourceEnabled(), "this resource has not been disabled");
	debug_assert(!isResourceCreated(), "this resource has not been destroyed");
}


inline void cResourcePoolItem::setResourceCode(cResourceCode code)
{
	m_resourceCode = code;
}

inline void cResourcePoolItem::setResourcePool(cResourcePoolInterface* interfacePtr)
{
	m_resourcePool = interfacePtr;
}

inline void cResourcePoolItem::setResourceHandle(cPoolHandle handle)
{
	m_resourceHandle = handle;
}

inline void cResourcePoolItem::setResourceFlag(int flagBit, bool setting)
{
	m_resourceFlags.setBit(flagBit, setting);
}


inline void cResourcePoolItem::notifyCreated()
{
	setResourceFlag(nCreated, true);
}

inline void cResourcePoolItem::notifyDestroyed()
{
	setResourceFlag(nCreated, false);
}

inline void cResourcePoolItem::notifyLoaded()
{
	setAlteredFlag(false);
	setResourceFlag(nLoaded, false);
}

inline void cResourcePoolItem::notifyUnloaded()
{
	setResourceFlag(nLoaded, false);
}

inline void cResourcePoolItem::notifyRestored()
{
	setResourceFlag(nDisabled, false);
}

inline void cResourcePoolItem::notifyDisabled()
{
	setResourceFlag(nDisabled, true);
}

inline void cResourcePoolItem::notifySaved()
{
	setAlteredFlag(false);
}

inline cResourceCode cResourcePoolItem::resourceCode()const
{
	return m_resourceCode;
}

inline cResourcePoolInterface* cResourcePoolItem::resourcePool()const
{
	return m_resourcePool;
}

inline cPoolHandle cResourcePoolItem::resourceHandle()const
{
	return m_resourceHandle;
}

inline u32Flags cResourcePoolItem::resourceFlags()const
{
	return m_resourceFlags;
}

inline bool cResourcePoolItem::isResourceCreated()const
{
	return TEST_BIT(m_resourceFlags,nCreated);
}

inline bool cResourcePoolItem::isResourceLoaded()const
{
	return TEST_BIT(m_resourceFlags,nLoaded);
}

inline bool cResourcePoolItem::isResourceDisabled()const
{
	return TEST_BIT(m_resourceFlags,nDisabled);
}

inline void cResourcePoolItem::setAlteredFlag(bool onOff)
{
	m_resourceFlags.setBit(nAltered, onOff);
}

inline bool cResourcePoolItem::alteredFlag()const
{
	return TEST_BIT(m_resourceFlags,nAltered);
}


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cResourcePoolItem.h )

//----------------------------------------------------------
//$Log: $