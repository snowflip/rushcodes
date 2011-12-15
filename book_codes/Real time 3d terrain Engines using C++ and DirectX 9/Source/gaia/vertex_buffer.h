#ifndef D3DBUFFER_H
#define D3DBUFFER_H
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
#include <D3D9.h>

//	Name Space declaration
namespace gaia
{


/*	cVertexBuffer
-----------------------------------------------------------------
    
    Add_Description_Here
    
-----------------------------------------------------------------
*/
class cVertexBuffer : public cResourcePoolItem 
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

	enum eConstants
	{
		nMaxElements = 16,			// maximum number of vertex elements stored
	};

	// Public Data...

	// Creators...

    cVertexBuffer();
    virtual ~cVertexBuffer();

	// Mutators...
	bool create(uint16 count, uint32 stride, u16Flags flags, void* pData);
	void destroy();

	void* lock(eLockType lockType, uint16 index, uint16 count);
	void unlock();

	void preLoad();
	void activate(int stream, uint16 index, bool setDeclaration = true);

	bool clone(cVertexBuffer* src);

	// Base Class Function Overrides
	bool createResource();					// innitialize the resource (called once)
	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const tchar* filename=0); // load the resource from a file (or NULL to use the resource name)
	bool saveResource(const tchar* filename=0);	// save the resource to a file (or NULL to use the resource name)

	bool setVertexDescription(int elementCount, const D3DVERTEXELEMENT9* elementList);

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

	LPDIRECT3DVERTEXBUFFER9 buffer()const;
	uint32		stride()const;
	u16Flags	typeFlags()const;
	u16Flags	stateFlags()const;
	uint16		count()const;
	uint32		byteSize()const;
	uint32		vertexElementCount()const;
	const D3DVERTEXELEMENT9*	vertexElements()const;

private:

	// Private Data...
	LPDIRECT3DVERTEXBUFFER9	 m_d3dBuffer;	// pointer to the actual DirectX Buffer
	uint8*		m_backupCopy;
	u16Flags	m_typeFlags;				// flags describing the buffer's type
	u16Flags	m_stateFlags;				// flags describing the buffer's state
	uint16		m_memberCount;
	uint32		m_stride;					// size of a single member in bytes
	uint32		m_byteSize;
	uint32		m_lockOffset;
	uint32		m_lockSize;
	uint32		m_usageFlags;

	uint32		m_vertexElementCount;
	D3DVERTEXELEMENT9 m_vertexElement[nMaxElements];
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	// Private Functions...
	bool createD3DBuffer();
	uint8* lockD3DBuffer(eLockType lockType, uint32 byteOffset, uint32 byteCount);
	void unlockD3DBuffer();

	// Invalid Functions...
    cVertexBuffer(const cVertexBuffer& Src);
    cVertexBuffer& operator=(const cVertexBuffer& Src);

};

typedef cResourcePool<cVertexBuffer> cVertexBufferManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cVertexBuffer
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cVertexBuffer::cVertexBuffer()
:m_d3dBuffer(0)
,m_backupCopy(0)
,m_typeFlags(0)
,m_stateFlags(0)
,m_vertexElementCount(0)
,m_pVertexDeclaration(0)
{
}

/*	~cVertexBuffer
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cVertexBuffer::~cVertexBuffer()
{
	debug_assert(m_d3dBuffer == 0, "d3d buffer left allocated. Call destroy()");
	debug_assert(m_backupCopy == 0, "backup buffer left allocated. Call destroy()");
}

//. Mutators ....................................................



//. Accessors ...................................................

inline bool cVertexBuffer::isValid()const
{
	// if at least one buffer is allocated, this object is valid
	return(m_d3dBuffer ? true:false);
}

inline bool cVertexBuffer::isReady()const
{
	// if at least one buffer is allocated, this object is valid
	return(TEST_BIT(m_stateFlags,nDataInitialized));
}

inline bool cVertexBuffer::isDynamic()const
{
	return (TEST_BIT(m_typeFlags,nDynamicOverwriteBit) || TEST_BIT(m_typeFlags,nDynamicUpdateBit));
}

inline bool cVertexBuffer::isDynamicOverwrite()const
{
	return (TEST_BIT(m_typeFlags,nDynamicOverwriteBit));
}

inline bool cVertexBuffer::isDynamicUpdate()const
{
	return (TEST_BIT(m_typeFlags,nDynamicUpdateBit));
}

inline bool cVertexBuffer::isReadable()const
{
	return (TEST_BIT(m_typeFlags,nReadableBit));
}

inline bool cVertexBuffer::isRAMBufferPresent()const
{
	return (m_backupCopy ? true:false);
}

inline bool cVertexBuffer::isSoftware()const
{
	return (TEST_BIT(m_typeFlags,nSoftwareBit));
}

inline bool cVertexBuffer::isLocked()const
{
	return (TEST_BIT(m_stateFlags,nLockedReadBit) || TEST_BIT(m_stateFlags,nLockedWriteBit));
}

inline LPDIRECT3DVERTEXBUFFER9 cVertexBuffer::buffer()const
{
	return m_d3dBuffer;
}

inline u16Flags cVertexBuffer::typeFlags()const
{
	return m_typeFlags;
}

inline u16Flags cVertexBuffer::stateFlags()const
{
	return m_stateFlags;
}

inline uint16 cVertexBuffer::count()const
{
	return m_memberCount;
}

inline uint32 cVertexBuffer::stride()const
{
	return m_stride;
}

inline uint32 cVertexBuffer::byteSize()const
{
	return m_byteSize;
}

inline uint32 cVertexBuffer::vertexElementCount()const
{
	return m_vertexElementCount;
}

inline const D3DVERTEXELEMENT9* cVertexBuffer::vertexElements()const
{
	return m_vertexElement;
}

//- End of cVertexBuffer ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cVertexBuffer.h )

//----------------------------------------------------------
//$Log: $