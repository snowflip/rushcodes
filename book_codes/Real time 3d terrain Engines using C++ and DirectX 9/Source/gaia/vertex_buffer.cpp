#define VERTEXBUFFER_CPP
#define SYSTEM_DLL
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
#include "vertex_buffer.h"
#include "game_host.h"

using namespace gaia;

 
bool cVertexBuffer::create(uint16 count, uint32 stride, u16Flags flags, void* pData)
{
	debug_assert(m_d3dBuffer == 0, "d3d buffer already allocated");
	debug_assert(m_backupCopy == 0, "backup buffer already allocated");

	m_usageFlags = 0;

	m_stride = stride;
	m_memberCount = count;
	m_typeFlags = flags;
	m_stateFlags = 0;
	m_byteSize = stride*count;

	// determine if the buffer should reside in RAM or VRAM
	if (TEST_BIT(m_typeFlags,nSoftwareBit) || !TheGameHost.hardwareVertexShadersAllowed())
	{
		m_typeFlags.setBit(nSoftwareBit);
		m_usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;
	}

	// dynamic overwite buffers are fully-overwritten on each lock (no ram copy needed)
	if (TEST_BIT(m_typeFlags,nDynamicOverwriteBit))
	{
		debug_assert(!TEST_BIT(m_typeFlags,nDynamicUpdateBit), "nDynamicOverwriteBit and nDynamicUpdateBit are mutually exclusive");
		m_usageFlags |= D3DUSAGE_DYNAMIC;
	}

	// is a RAM buffer needed or requested?
	if (TEST_BIT(m_typeFlags,nSoftwareBit))
	{
		// Software surfaces do not need RAM backups
		m_typeFlags.clearBit(nRamBackupBit);

		// but the following are implied
		m_typeFlags.setBit(nReadableBit);
		m_typeFlags.setBit(nDynamicUpdateBit);
		m_typeFlags.setBit(nDynamicOverwriteBit);
	}
	else
	{
		// Software surfaces do need RAM backups if any of the following are set
		if (TEST_BIT(m_typeFlags,nDynamicUpdateBit) 
			|| TEST_BIT(m_typeFlags,nReadableBit)
			|| TEST_BIT(m_typeFlags,nRamBackupBit))
		{
			// set the RAM buffer flag
			m_typeFlags.setBit(nRamBackupBit);

			// having a RAM buffer also implies that the following are also true
			m_typeFlags.setBit(nReadableBit);
			m_typeFlags.setBit(nDynamicUpdateBit);
		}
	}

	// dynamic update buffers are NOT fully-overwritten on each lock (ram copy needed)
	if (TEST_BIT(m_typeFlags,nDynamicUpdateBit))
	{
		debug_assert(!TEST_BIT(m_typeFlags,nDynamicOverwriteBit), "nDynamicOverwriteBit and nDynamicUpdateBit are mutually exclusive");
		m_usageFlags |= D3DUSAGE_DYNAMIC;
	}
	
	// determine if the buffer can be write-only
	if (TEST_BIT(m_typeFlags,nRamBackupBit)
		|| !TEST_BIT(m_typeFlags,nReadableBit))
	{
		// we are backed up in RAM, or not indended to be read
		m_usageFlags |= D3DUSAGE_WRITEONLY;
	}


	// if a ram backup is needed, allocate it
	if (TEST_BIT(m_typeFlags,nRamBackupBit))
	{
		// the buffer is now readable
		m_typeFlags.setBit(nReadableBit);

		m_backupCopy = (uint8*)alloc_aligned_memory(m_byteSize, memory::ALIGN_16);
	}

	// record the volatility of this resource
	m_stateFlags.setBit(nVolatile, (m_usageFlags & D3DUSAGE_DYNAMIC)?true:false);

	// create the D3D Buffer
	if (createD3DBuffer())
	{
		// if data was provided, internalize it
		if (pData != 0)
		{
			void* pBuffer = lock(nWriteLock, 0, m_memberCount);
			memcpy(pBuffer, pData, m_byteSize);
			unlock();
		}
		return (true);
	}


	return (false);
}

bool cVertexBuffer::createD3DBuffer()
{
	debug_assert(!m_d3dBuffer, "The buffer is already present");

	D3DPOOL d3dPool = D3DPOOL_MANAGED;
	if (m_usageFlags & D3DUSAGE_DYNAMIC)
	{
		d3dPool = D3DPOOL_DEFAULT;
	}

	// create the D3D Buffer
	HRESULT result= TheGameHost.d3dDevice()->CreateVertexBuffer(m_byteSize,
																m_usageFlags,
																0,
																d3dPool,
																&m_d3dBuffer,
																0);
	if (result != D3D_OK)
	{
		m_d3dBuffer=0;
		debug_error(result);
		destroy();
		return (false);
	}
	
	return (true);
}

void cVertexBuffer::destroy()
{
	debug_assert(!isLocked(), "You forgot to unlock the buffer");
	safe_release(m_d3dBuffer);
	safe_release(m_pVertexDeclaration);
	free_memory(m_backupCopy);
	m_vertexElementCount=0;
	m_typeFlags=0;
	m_stateFlags=0;
	notifyUnloaded();

}


bool cVertexBuffer::clone(cVertexBuffer* src)
{
	bool success = false;

	// destroy any local data
	destroy();

	// lock the source to get access to it's data
	void* pData = src->lock(nReadLock, 0, src->m_memberCount);

	if (pData)
	{
		// recreate in the format of our source
		success= create(src->m_memberCount, src->m_stride, src->m_typeFlags, pData);

		src->unlock();
	}

	return success;
}

void* cVertexBuffer::lock(eLockType lockType, uint16 index, uint16 count)
{
	debug_assert(isValid(), "Attempting to lock an invalid buffer");
	debug_assert(!isLocked(), "Attempting to lock a locked buffer");
	debug_assert(index + count <= m_memberCount, "Attempting to lock an invalid range");

	if (index == 0 && count == 0)
	{
		// lock the entire range
		count = m_memberCount;
	}


	bool attemptingToRead = lockType != nWriteLock;
	bool attemptingToWrite = lockType != nReadLock;

	uint8* result = 0;

	// make sre we can read if that is our desire
	if (attemptingToRead && !TEST_BIT(m_typeFlags,nReadableBit))
	{
		// can't read an unreadable buffer
		result = 0;
	}
	else if (attemptingToWrite 
				&& !TEST_BIT(m_typeFlags,nDynamicOverwriteBit)
				&& !TEST_BIT(m_typeFlags,nDynamicUpdateBit)
				&& TEST_BIT(m_stateFlags,nDataInitialized))
	{
		// can't write to a static buffer more than once
		result = 0;
	}
	else 
	{
		m_lockOffset = (index * m_stride);
		m_lockSize = (count * m_stride);

		if (m_backupCopy)
		{
			// return the RAM copy
			result =  m_backupCopy + m_lockOffset;
		}
		else
		{
			// lock the D3D Buffer and return it to the caller
			uint32 byteSize = count * m_stride;
			result = lockD3DBuffer(lockType, m_lockOffset, m_lockSize);
		}
	}

	if (result)
	{
		m_stateFlags.setBit(nLockedReadBit, attemptingToRead);
		m_stateFlags.setBit(nLockedWriteBit, attemptingToWrite);
	}

	return result;
}


void cVertexBuffer::unlock()
{
	debug_assert(isValid(), "Attempting to unlock an invalid buffer");
	debug_assert(isLocked(), "Attempting to unlock an unlocked buffer");

	bool writePerformed = TEST_BIT(m_stateFlags,nLockedWriteBit);

	if (m_backupCopy)
	{
		// the user was given the RAM copy to play with.
		// if it was written to, we need to copy it to the d3d buffer
		if(writePerformed)
		{
			uint8* d3dBuffer = lockD3DBuffer(nWriteLock, m_lockOffset, m_lockSize);
			memcpy(d3dBuffer, m_backupCopy+m_lockOffset, m_lockSize);
			unlockD3DBuffer();
		}
	}
	else
	{
		// the user was using the D3D buffer directly, so unlock it
		unlockD3DBuffer();
	}

	if (writePerformed)
	{
		// record that data has been, well... recorded
		notifyLoaded();
		m_stateFlags.setBit(nDataInitialized);
	}

	// clear the lock flags
	m_stateFlags.clearBit(nLockedReadBit);
	m_stateFlags.clearBit(nLockedWriteBit);
}

uint8* cVertexBuffer::lockD3DBuffer(eLockType lockType, uint32 byteOffset, uint32 byteCount)
{
	debug_assert(isValid(), "Attempting to unlock an invalid buffer");
	uint8* pData = 0;

	uint32 lockFlags = 0;

	if (lockType == nReadLock)
	{
		lockFlags = D3DLOCK_READONLY;
	}
	else if (lockType == nWriteLock)
	{
		if (isDynamic())
		{
			lockFlags = D3DLOCK_DISCARD;
		}
	}

	HRESULT result= m_d3dBuffer->Lock(byteOffset, byteCount, (VOID**)&pData, lockFlags);

	if (result != D3D_OK)
	{
		debug_error(result);
		pData = 0;
	}
	return (pData);
}

void cVertexBuffer::unlockD3DBuffer()
{
	debug_assert(isValid(), "Attempting to unlock an invalid buffer");

	HRESULT result= m_d3dBuffer->Unlock();

	if (result != D3D_OK)
	{
		debug_error(result);
	}

}

void cVertexBuffer::preLoad()
{
	debug_assert(isValid(), "Attempting to activate an invalid buffer");
	debug_assert(isReady(), "Attempting to activate an uninitialized buffer");

	m_d3dBuffer->PreLoad();
}

void cVertexBuffer::activate(int stream, uint16 index, bool setDeclaration)
{
	debug_assert(isValid(), "Attempting to activate an invalid buffer");
	debug_assert(isReady(), "Attempting to activate an uninitialized buffer");

	HRESULT result= TheGameHost.d3dDevice()->SetStreamSource(	stream,
																m_d3dBuffer,
																index * m_stride,
																m_stride);

	if (result != D3D_OK)
	{
		debug_error(result);
	}

	if (stream == 0 && setDeclaration && m_pVertexDeclaration)
	{
		result= TheGameHost.d3dDevice()->SetVertexDeclaration( m_pVertexDeclaration );
		
		if (result != D3D_OK)
		{
			debug_error(result);
		}
	}

}

// innitialize the resource (called once)
bool cVertexBuffer::createResource()
{
	// nothing to do
	notifyCreated();
	return true;
}
					
// destroy the resource
bool cVertexBuffer::destroyResource()
{
	destroy();
	notifyDestroyed();
	return true;
}
					
// purge the resource from volatile memory
bool cVertexBuffer::disableResource()
{
	notifyDisabled();
	if (TEST_BIT(m_stateFlags,nVolatile))
	{
		// release the buffer
		safe_release(m_d3dBuffer);
	}
	return true;
}

// prepare the resource for use (create any volatile memory objects needed)
bool cVertexBuffer::restoreResource()
{
	notifyRestored();
	
	if (TEST_BIT(m_stateFlags,nVolatile)
		&& !m_d3dBuffer)
	{
		// re-create the buffer
		if (createD3DBuffer())
		{
			// reload our data (if we have any)
			if (m_backupCopy)
			{
				uint8* d3dBuffer = lockD3DBuffer(nWriteLock, 0, m_byteSize);
				memcpy(d3dBuffer, m_backupCopy, m_byteSize);
				unlockD3DBuffer();
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}
					
					
// load the resource from a file (or NULL to use the resource name)
bool cVertexBuffer::loadResource(const tchar* filename)
{
	return false;
}; 


// save the resource to a file (or NULL to use the resource name)
bool cVertexBuffer::saveResource(const tchar* filename)
{
	return false;
};

bool cVertexBuffer::setVertexDescription(int elementCount, const D3DVERTEXELEMENT9* elementList)
{
	debug_assert(elementCount < nMaxElements, "too many vertex elements");

	int i=0;
	for (i=0; i<elementCount; ++i)
	{
		m_vertexElement[i] = elementList[i];
	}
	m_vertexElementCount = elementCount;

	safe_release(m_pVertexDeclaration);
	return SUCCEEDED(
		TheGameHost.d3dDevice()->CreateVertexDeclaration( 
			&m_vertexElement[0], 
			&m_pVertexDeclaration ));
}
	

//- End of cVertexBuffer ----------------------------------------------

//***************************************************************


//----------------------------------------------------------
//$Log: $