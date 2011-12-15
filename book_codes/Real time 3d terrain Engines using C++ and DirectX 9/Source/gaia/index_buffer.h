#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
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
#ifndef CORE_H
#include "core\core.h"
#endif

#include <D3D9.h>

//	Name Space declaration
namespace gaia
{


/*	cIndexBuffer
-----------------------------------------------------------------
    
    Add_Description_Here
    
-----------------------------------------------------------------
*/
class cIndexBuffer : public cResourcePoolItem
{
public:

	// Data Types & Constants...
	enum eBufferFlagBits
	{
		nDynamicOverwriteBit =0,	// each lock completely replaces thew buffer
		nDynamicUpdateBit,			// each lock may update *some* data
		nReadableBit,				// the buffer may be read
		nRamBackupBit,				// a backup of the buffer is stored in RAM
		nSoftwareBit,				// this buffer is not for hardware rendering
	};

	enum eBufferStateBits
	{
		nLockedReadBit,				// the buffer is currently locked for reading
		nLockedWriteBit,			// the buffer is currently locked for writing
		nDataInitialized,			// data has been placed in the buffer
		nVolatile,					// this buffer must be restored by hand
	};


	// Public Data...

	// Creators...

    cIndexBuffer();
    virtual ~cIndexBuffer();

	// Mutators...
	bool create(D3DPRIMITIVETYPE type, uint16 count, u16Flags flags, uint16* pData);
	bool createSingleStripGrid(
		uint16 xVerts,	// width of grid
		uint16 yVerts,	// height of grid
		uint16 xStep,	// horz vertex count per cell
		uint16 yStep,	// vert vertex count per cell
		uint16 stride,	// horz vertex count in vbuffer
		uint16 flags);

	void destroy();

	uint16* lock(eLockType lockType, uint16 index, uint16 count);
	void unlock();

	void activate();
	bool clone(cIndexBuffer* src);

	// Base Class Function Overrides
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	// Accessors...
	bool isValid()const;
	bool isReady()const;
	bool isDynamic()const;
	bool isDynamicOverwrite()const;
	bool isDynamicUpdate()const;
	bool isReadable()const;
	bool isRAMBufferPresent()const;
	bool isSoftware()const;
	bool isLocked()const;

	LPDIRECT3DINDEXBUFFER9 buffer()const;
	uint16		stride()const;
	u16Flags	typeFlags()const;
	u16Flags	stateFlags()const;
	uint16		count()const;
	uint32		byteSize()const;
	D3DPRIMITIVETYPE		primitiveType()const;
	uint32		primitiveCount(uint16 indexCount)const;
	uint32		primitiveCount()const;

private:

	// Private Data...
	LPDIRECT3DINDEXBUFFER9	 m_d3dBuffer;	// pointer to the actual DirectX Buffer
	uint16*		m_backupCopy;
	D3DPRIMITIVETYPE		m_primitiveType;			// list, strip, fan?
	u16Flags		m_typeFlags;				// flags describing the buffer's type
	u16Flags	m_stateFlags;				// flags describing the buffer's state
	uint16		m_memberCount;
	uint16		m_stride;					// size of a single member in bytes
	uint32		m_byteSize;
	uint32		m_lockOffset;
	uint32		m_lockSize;

	// Private Functions...
	uint16* lockD3DBuffer(eLockType lockType, uint32 byteOffset, uint32 byteCount);
	void unlockD3DBuffer();

	// Invalid Functions...
    cIndexBuffer(const cIndexBuffer& Src);
    cIndexBuffer& operator=(const cIndexBuffer& Src);

};

typedef cResourcePool<cIndexBuffer> cIndexBufferManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cIndexBuffer
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cIndexBuffer::cIndexBuffer()
:m_d3dBuffer(0)
,m_backupCopy(0)
,m_typeFlags(0)
,m_stateFlags(0)
,m_stride(2)
,m_primitiveType(D3DPT_TRIANGLELIST)
{
}

/*	~cIndexBuffer
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cIndexBuffer::~cIndexBuffer()
{
	debug_assert(m_d3dBuffer == 0, "d3d buffer left allocated. Call destroy()");
	debug_assert(m_backupCopy == 0, "backup buffer left allocated. Call destroy()");
}

//. Mutators ....................................................



//. Accessors ...................................................

inline bool cIndexBuffer::isValid()const
{
	// if at least one buffer is allocated, this object is valid
	return(m_d3dBuffer ? true:false);
}

inline bool cIndexBuffer::isReady()const
{
	// if at least one buffer is allocated, this object is valid
	return(TEST_BIT(m_stateFlags,nDataInitialized));
}

inline bool cIndexBuffer::isDynamic()const
{
	return (TEST_BIT(m_typeFlags,nDynamicOverwriteBit) || TEST_BIT(m_typeFlags,nDynamicUpdateBit));
}

inline bool cIndexBuffer::isDynamicOverwrite()const
{
	return (TEST_BIT(m_typeFlags,nDynamicOverwriteBit));
}

inline bool cIndexBuffer::isDynamicUpdate()const
{
	return (TEST_BIT(m_typeFlags,nDynamicUpdateBit));
}

inline bool cIndexBuffer::isReadable()const
{
	return (TEST_BIT(m_typeFlags,nReadableBit));
}

inline bool cIndexBuffer::isRAMBufferPresent()const
{
	return (m_backupCopy ? true:false);
}

inline bool cIndexBuffer::isSoftware()const
{
	return (TEST_BIT(m_typeFlags,nSoftwareBit));
}

inline bool cIndexBuffer::isLocked()const
{
	return (TEST_BIT(m_stateFlags,nLockedReadBit) || TEST_BIT(m_stateFlags,nLockedWriteBit));
}

inline LPDIRECT3DINDEXBUFFER9 cIndexBuffer::buffer()const
{
	return m_d3dBuffer;
}

inline u16Flags cIndexBuffer::typeFlags()const
{
	return m_typeFlags;
}

inline u16Flags cIndexBuffer::stateFlags()const
{
	return m_stateFlags;
}

inline uint16 cIndexBuffer::count()const
{
	return m_memberCount;
}

inline uint16 cIndexBuffer::stride()const
{
	return m_stride;
}

inline uint32 cIndexBuffer::byteSize()const
{
	return m_byteSize;
}

inline D3DPRIMITIVETYPE cIndexBuffer::primitiveType()const
{
	return m_primitiveType;
}

inline uint32 cIndexBuffer::primitiveCount(uint16 indexCount)const
{
	uint32 count = indexCount/3;

	if (m_primitiveType == D3DPT_TRIANGLESTRIP
		|| m_primitiveType == D3DPT_TRIANGLEFAN)
	{
		count = indexCount-2;
	}

	return count;
}

inline uint32 cIndexBuffer::primitiveCount()const
{
	// when no count is specified, use the total count of indices
	return primitiveCount(count());
}
//- End of cIndexBuffer ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cIndexBuffer.h )

//----------------------------------------------------------
//$Log: $