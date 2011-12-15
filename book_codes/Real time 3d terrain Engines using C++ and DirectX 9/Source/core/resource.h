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

    $Workfile: $

    sample code from the book...
    3D Terrain Engines From the Ground Up.

    Created 10/7/2002 by Greg Snook
    greg@mightystudios.com

    ------------------------------------------------------------ 
    Last Revision:
    $Author: $
    $Date: $
    $Revision: $

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
    
    Add_Description_Here
    
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


/*	cResource
------------------------------------------------------------------------------------------
	
	A cResource is a simple base class for a shared resource such as a texture, 
	animation or vertex buffer. 

	Note that cResource is derived from cReferenceCounter. The cResourceManager uses
	the value of the cReferenceCounter to decide when objects are no longer in use and can
	be destroyed during the cResourceManager::clean() function. This means the application
	must update the reference count of the cResource whenever links between objects
	are created or removed between objects. The cReferenceCounter base class provides the
	functionality to manipulate the reference count.
	
------------------------------------------------------------------------------------------
*/

class cResource : public cReferenceCounter
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
	cResource();
	virtual ~cResource();

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
	cResource( const cResource& Src);
	cResource& operator=( const cResource& Src);

	friend cResourcePoolInterface;
};

inline cResource::cResource()
:m_resourceCode(0)
,m_resourcePool(0)
,m_resourceHandle(0)
,m_resourceFlags(0)
{
}

inline cResource::~cResource()
{
//	debug_assert(!isResourceEnabled(), "this resource has not been disabled");
	debug_assert(!isResourceCreated(), "this resource has not been destroyed");
}


inline void cResource::setResourceCode(cResourceCode code)
{
	m_resourceCode = code;
}

inline void cResource::setResourcePool(cResourcePoolInterface* interfacePtr)
{
	m_resourcePool = interfacePtr;
}

inline void cResource::setResourceHandle(cPoolHandle handle)
{
	m_resourceHandle = handle;
}

inline void cResource::setResourceFlag(int flagBit, bool setting)
{
	m_resourceFlags.setBit(flagBit, setting);
}


inline void cResource::notifyCreated()
{
	setResourceFlag(nCreated, true);
}

inline void cResource::notifyDestroyed()
{
	setResourceFlag(nCreated, false);
}

inline void cResource::notifyLoaded()
{
	setAlteredFlag(false);
	setResourceFlag(nLoaded, false);
}

inline void cResource::notifyUnloaded()
{
	setResourceFlag(nLoaded, false);
}

inline void cResource::notifyRestored()
{
	setResourceFlag(nDisabled, false);
}

inline void cResource::notifyDisabled()
{
	setResourceFlag(nDisabled, true);
}

inline void cResource::notifySaved()
{
	setAlteredFlag(false);
}

inline cResourceCode cResource::resourceCode()const
{
	return m_resourceCode;
}

inline cResourcePoolInterface* cResource::resourcePool()const
{
	return m_resourcePool;
}

inline cPoolHandle cResource::resourceHandle()const
{
	return m_resourceHandle;
}

inline u32Flags cResource::resourceFlags()const
{
	return m_resourceFlags;
}

inline bool cResource::isResourceCreated()const
{
	return m_resourceFlags.testBit(nCreated);
}

inline bool cResource::isResourceLoaded()const
{
	return m_resourceFlags.testBit(nLoaded);
}

inline bool cResource::isResourceDisabled()const
{
	return m_resourceFlags.testBit(nDisabled);
}

inline void cResource::setAlteredFlag(bool onOff)
{
	m_resourceFlags.setBit(nAltered, onOff);
}

inline bool cResource::alteredFlag()const
{
	return m_resourceFlags.testBit(nAltered);
}


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cResource.h )

//----------------------------------------------------------
//$Log: $