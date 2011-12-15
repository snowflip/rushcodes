#define INDEXBUFFER_CPP
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
#include "index_buffer.h"
#include "game_host.h"

using namespace gaia;

 
bool cIndexBuffer::create(D3DPRIMITIVETYPE type, uint16 count, u16Flags flags, uint16* pData)
{
	debug_assert(m_d3dBuffer == 0, "d3d buffer already allocated");
	debug_assert(m_backupCopy == 0, "backup buffer already allocated");

	uint32 usageFlags = 0;

	m_memberCount = count;
	m_typeFlags = flags;
	m_stateFlags = 0;
	m_byteSize = m_stride*count;
	m_primitiveType = type;

	// determine if the buffer should reside in RAM or VRAM
	if (TEST_BIT(m_typeFlags,nSoftwareBit) || !TheGameHost.hardwareVertexShadersAllowed())
	{
		m_typeFlags.setBit(nSoftwareBit);
		usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;
	}

	// dynamic overwite buffers are fully-overwritten on each lock (no ram copy needed)
	if (TEST_BIT(m_typeFlags,nDynamicOverwriteBit))
	{
		debug_assert(!TEST_BIT(m_typeFlags,nDynamicUpdateBit), "nDynamicOverwriteBit and nDynamicUpdateBit are mutually exclusive");
		usageFlags |= D3DUSAGE_DYNAMIC;
	}

	// dynamic update buffers are NOT fully-overwritten on each lock (ram copy needed)
	if (TEST_BIT(m_typeFlags,nDynamicUpdateBit))
	{
		debug_assert(!TEST_BIT(m_typeFlags,nDynamicOverwriteBit), "nDynamicOverwriteBit and nDynamicUpdateBit are mutually exclusive");
		usageFlags |= D3DUSAGE_DYNAMIC;
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
			m_typeFlags.setBit(nDynamicOverwriteBit);
		}
	}

	// determine if the buffer can be write-only
	if (TEST_BIT(m_typeFlags,nRamBackupBit)
		|| !TEST_BIT(m_typeFlags,nReadableBit))
	{
		// we are backed up in RAM, or not indended to be read
		usageFlags |= D3DUSAGE_WRITEONLY;
	}

	D3DPOOL d3dPool = D3DPOOL_MANAGED;
	if (usageFlags & D3DUSAGE_DYNAMIC)
	{
		d3dPool = D3DPOOL_DEFAULT;
		m_stateFlags.setBit(nVolatile);
	}
	else
	{
		m_stateFlags.clearBit(nVolatile);
	}

	// if a ram backup is needed, allocate it
	if (TEST_BIT(m_typeFlags,nRamBackupBit))
	{
		// the buffer is now readable
		m_typeFlags.setBit(nReadableBit);

		m_backupCopy = (uint16*)alloc_aligned_memory(m_byteSize, memory::ALIGN_16);
		debug_error(0);
		destroy();
		return (false);
	}

	// create the D3D Buffer
	HRESULT result= TheGameHost.d3dDevice()->CreateIndexBuffer(m_byteSize,
																usageFlags,
																D3DFMT_INDEX16,
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

	// if data was provided, internalize it
	if (pData != 0)
	{
		uint16* pBuffer = lock(nWriteLock, 0, m_memberCount);
		memcpy(pBuffer, pData, m_byteSize);
		unlock();
	}
	return (true);
}

bool cIndexBuffer::createSingleStripGrid(
	uint16 xVerts,	// width of grid
	uint16 yVerts,	// height of grid
	uint16 xStep,	// horz vertex count per cell
	uint16 yStep,	// vert vertex count per cell
	uint16 stride,	// horz vertex count in vbuffer
	uint16 flags)
{
	int total_strips = 
		yVerts-1;
	int total_indexes_per_strip = 
		xVerts<<1;

	// the total number of indices is equal
	// to the number of strips times the
	// indices used per strip plus one
	// degenerate triangle between each strip
	int total_indexes = 
		(total_indexes_per_strip * total_strips) 
		+ (total_strips<<1) - 2;

	unsigned short* pIndexValues = 
		new unsigned short[total_indexes];

	unsigned short* index = pIndexValues;
	unsigned short start_vert = 0;
	unsigned short lineStep = yStep*stride;

	for (int j=0;j<total_strips;++j)
	{
		int k=0;
		unsigned short vert=start_vert;

		// create a strip for this row
		for (k=0;k<xVerts;++k)
		{
			*(index++) = vert;
			*(index++) = vert + lineStep;
			vert += xStep;
		}
		start_vert += lineStep;

		if (j+1<total_strips)
		{
			// add a degenerate to attach to 
			// the next row
			*(index++) = (vert-xStep)+lineStep;
			*(index++) = start_vert;
		}
	}

	// finally, use the indices we created above
	// to fill the Direct3D index buffer
	bool result=
		create(D3DPT_TRIANGLESTRIP, total_indexes, flags, pIndexValues);

	// destroy our local data and return
	delete [] pIndexValues;
	return result;
}



void cIndexBuffer::destroy()
{
	debug_assert(!isLocked(), "You forgot to unlock the buffer");
	safe_release(m_d3dBuffer);
	safe_delete(m_backupCopy);
	m_typeFlags=0;
	m_stateFlags=0;
	notifyUnloaded();
}

bool cIndexBuffer::clone(cIndexBuffer* src)
{
	bool success = false;

	// destroy any local data
	destroy();

	// lock the source to get access to it's data
	uint16* pData = src->lock(nReadLock, 0, src->m_memberCount);

	if (pData)
	{
		// recreate in the format of our source
		success= create(src->m_primitiveType, src->m_memberCount, src->m_typeFlags, pData);

		src->unlock();
	}

	return success;
}

uint16* cIndexBuffer::lock(eLockType lockType, uint16 index, uint16 count)
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

	uint16* result = 0;

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
			result =  m_backupCopy + index;
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


void cIndexBuffer::unlock()
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
			uint16* d3dBuffer = lockD3DBuffer(nWriteLock, m_lockOffset, m_lockSize);
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

uint16* cIndexBuffer::lockD3DBuffer(eLockType lockType, uint32 byteOffset, uint32 byteCount)
{
	debug_assert(isValid(), "Attempting to unlock an invalid buffer");
	uint16* pData = 0;

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

void cIndexBuffer::unlockD3DBuffer()
{
	debug_assert(isValid(), "Attempting to unlock an invalid buffer");

	HRESULT result= m_d3dBuffer->Unlock();

	if (result != D3D_OK)
	{
		debug_error(result);
	}
}

void cIndexBuffer::activate()
{
	debug_assert(isValid(), "Attempting to activate an invalid buffer");
	debug_assert(isReady(), "Attempting to activate an uninitialized buffer");

	HRESULT result= TheGameHost.d3dDevice()->SetIndices(m_d3dBuffer);

	if (result != D3D_OK)
	{
		debug_error(result);
	}
}

// innitialize the resource (called once)
bool cIndexBuffer::createResource()
{
	// nothing to do
	notifyCreated();
	return true;
}
					
// destroy the resource
bool cIndexBuffer::destroyResource()
{
	destroy();
	notifyDestroyed();
	return true;
}
					
// prepare the resource for use (create any volatile memory objects needed)
bool cIndexBuffer::restoreResource()
{
	notifyRestored();
	return true;
}
					
// purge the resource from volatile memory
bool cIndexBuffer::disableResource()
{
	notifyDisabled();
	return true;
}
					
// load the resource from a file
bool cIndexBuffer::loadResource(const tchar* filename)
{
	return true;
}
	

// save the resource to the file and return the size written
bool cIndexBuffer::saveResource(const tchar* filename)
{
	return true;
}


//- End of cIndexBuffer ----------------------------------------------

//***************************************************************


//----------------------------------------------------------
//$Log: $