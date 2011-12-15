#define MEMORY_CPP
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
#include "memory.h"

#define ENABLE_MEMORY_DEBUGGING

using namespace gaia;

static memory::ALIGNMENT s_defaultAlignment = memory::ALIGN_NONE;
static memory::sPreBufferData* s_topMemoryBlock = 0;

static uint32 s_allocCount=0;				// total number of allocations made
static uint32 s_releaseCount=0;			// total number of allocations released
static uint32 s_totalBytesRequested=0;	// current total memory requested
static uint32 s_totalBytesUsed=0;			// current total memory in use
static uint32 s_totalBytesNeeded=0;		// totalBytesUsed minus debug information
static uint32 s_maximumBytesRequested=0;	// maximum total memory requested
static uint32 s_maximumBytesUsed=0;		// maximum total memory in use
static uint32 s_maximumBytesNeeded=0;		// maximumBytesUsed minus debug information

#ifdef ENABLE_MEMORY_DEBUGGING
static const uint32 s_preBufferSize = sizeof(memory::sPreBufferData);
static const uint32 s_postBufferSize = sizeof(memory::sPostBufferData);
#else
static const uint32 s_preBufferSize = 0;
static const uint32 s_postBufferSize = 0;
#endif

static memory::sMemoryBlockHeader* getMemoryBlockHeader(void* pClientMem)
{
	return (memory::sMemoryBlockHeader*) 
		((char*)pClientMem - sizeof(memory::sMemoryBlockHeader)-s_preBufferSize);
}

static void verifyPadding(memory::sPreBufferData* pPreBufferData)
{
	uint8* prePattern = pPreBufferData->bytePattern;
	uint8* postPattern = (uint8*)pPreBufferData + s_preBufferSize + pPreBufferData->requestedSize;
	for (int i=0;i<memory::PATTERN_SIZE;++i)
	{
		debug_assert(prePattern[i]==(uint8)memory::PRE_PATTERN, "memory overrun detected");
		debug_assert(postPattern[i]==(uint8)memory::POST_PATTERN, "memory overrun detected");
	}
}

static uint16 computeChecksum(memory::sPreBufferData* pPreBufferData)
{
	uint16 checksum=0;
	uint8* data = (uint8*)pPreBufferData + s_preBufferSize;
	for (uint32 i=0;i<pPreBufferData->requestedSize;++i)
	{
		checksum += *(data++);
	}
	return checksum;
}


// memory management functions
void memory::setDefaultAlignment(ALIGNMENT align)
{
	s_defaultAlignment = align;
}

void* memory::alloc(size_t Size, ALIGNMENT Align, const char* FileName, int LineNumber)
{
	if (!Size)
	{
		return 0;
	}
	// we store the memory size requested in 24 bits
	debug_assert(Size < (1<<24), "allocating a block too large");

	if (Align == ALIGN_DEFAULT)
	{
		Align = s_defaultAlignment;
	}

	// compute the size of the total allocation
	uint32 trueSize = Size + Align + sizeof(sMemoryBlockHeader);
	trueSize += s_preBufferSize + s_postBufferSize;

	// allocate the required memory
	char* pRealMem = (char*)malloc(trueSize);
	debug_assert(pRealMem, "catastrophic memory allocation error!");

	// find the aligned memory position we will give back to the caller
	char* pClientMem = (char*) (pRealMem + Align + sizeof(sMemoryBlockHeader));
	pClientMem += s_preBufferSize;
	if (Align)
	{
		pClientMem = (char*)((int)(pClientMem) & ~(Align-1));
	}

	// we can now write the memory block header
	sMemoryBlockHeader* pHeader = getMemoryBlockHeader(pClientMem);
	pHeader->actualSize = trueSize;
	pHeader->pointerOffset = (char*)pHeader - (char*)pRealMem;

#ifdef ENABLE_MEMORY_DEBUGGING
	// write the prebuffer and postbuffer 
	// debug information
	sPreBufferData* pPreBufferData = (sPreBufferData*)(pHeader+1);
	pPreBufferData->nextHeader = s_topMemoryBlock;
	pPreBufferData->previousHeader = 0;
	pPreBufferData->requestedSize = Size;
	pPreBufferData->userChecksum = 0;
	pPreBufferData->fileLine = LineNumber;

	if (s_topMemoryBlock)
	{
		s_topMemoryBlock->previousHeader = pPreBufferData;
	}
	s_topMemoryBlock = pPreBufferData;

	if (FileName)
	{
		strncpy(pPreBufferData->fileName, FileName, MAX_MEMORY_FILENAME); // filename of the caller
	}
	else
	{
		strncpy(pPreBufferData->fileName, "unknown", MAX_MEMORY_FILENAME); // filename of the caller
	}

	uint8* prePattern = pPreBufferData->bytePattern;
	uint8* postPattern = (uint8*)pClientMem + Size;
	for (int i=0;i<memory::PATTERN_SIZE;i++)
	{
		prePattern[i]=(char)memory::PRE_PATTERN;
		postPattern[i]=(char)memory::POST_PATTERN;
	}

	// update our memory statistics
	s_allocCount++;
	s_totalBytesRequested += Size;
	s_totalBytesUsed += trueSize;
	s_totalBytesNeeded += (trueSize-s_preBufferSize-s_postBufferSize);
	
	s_maximumBytesRequested = 
		maximum(s_maximumBytesRequested,
				s_totalBytesRequested);

	s_maximumBytesUsed = 
		maximum(s_maximumBytesUsed,
				s_totalBytesUsed);

	s_maximumBytesNeeded = 
		maximum(s_maximumBytesNeeded,
				s_totalBytesNeeded);

#endif

	// return the client portion of the allocation
	return (pClientMem);
}

void memory::free(void* pClientMem)
{
	debug_assert(pClientMem, "attempting to free a null pointer");

	sMemoryBlockHeader* pHeader = getMemoryBlockHeader(pClientMem);
	char* pRealPtr = (char*)pHeader - pHeader->pointerOffset;

#ifdef ENABLE_MEMORY_DEBUGGING
	sPreBufferData* pPreBufferData = (sPreBufferData*)(pHeader+1);
	
	// verify overwrite patterns
	verifyPadding(pPreBufferData);

	// update statistics
	s_releaseCount++;
	s_totalBytesRequested -= pPreBufferData->requestedSize;
	s_totalBytesUsed -= pHeader->actualSize;
	s_totalBytesNeeded -= (pHeader->actualSize-s_preBufferSize-s_postBufferSize);

	if (s_topMemoryBlock == pPreBufferData)
	{
		s_topMemoryBlock = s_topMemoryBlock->nextHeader;
	}
	if (pPreBufferData->nextHeader)
	{
		pPreBufferData->nextHeader->previousHeader = pPreBufferData->previousHeader;
	}
	if (pPreBufferData->previousHeader)
	{
		pPreBufferData->previousHeader->nextHeader = pPreBufferData->nextHeader;
	}

#endif

	// free the actual memory
	::free(pRealPtr);
}

// memory debugging functions
void memory::checkPadding(void* pClientMem)
{
#ifdef ENABLE_MEMORY_DEBUGGING
	debug_assert(pClientMem, "null pointer encountered");

	if (s_preBufferSize)
	{
		sMemoryBlockHeader* pHeader = getMemoryBlockHeader(pClientMem);
		sPreBufferData* pPreBufferData = (sPreBufferData*)(pHeader+1);
		// verify overwrite patterns
		verifyPadding(pPreBufferData);
	}
#endif
}

void memory::buildChecksum(void* pClientMem)
{
#ifdef ENABLE_MEMORY_DEBUGGING
	debug_assert(pClientMem, "null pointer encountered");

	if (s_preBufferSize)
	{
		sMemoryBlockHeader* pHeader = getMemoryBlockHeader(pClientMem);
		sPreBufferData* pPreBufferData = (sPreBufferData*)(pHeader+1);
		// build the current checksum
		pPreBufferData->userChecksum = computeChecksum(pPreBufferData);
	}
#endif
}

void memory::testChecksum(void* pClientMem)
{
#ifdef ENABLE_MEMORY_DEBUGGING
	debug_assert(pClientMem, "null pointer encountered");

	if (s_preBufferSize)
	{
		sMemoryBlockHeader* pHeader = getMemoryBlockHeader(pClientMem);
		sPreBufferData* pPreBufferData = (sPreBufferData*)(pHeader+1);
		// build the current checksum
		uint16 checksum = computeChecksum(pPreBufferData);

		// verify our previous checksum value
		debug_assert(pPreBufferData->userChecksum == checksum,
			"checksum test failed");
	}
#endif
}

void memory::checkAllPadding()
{
#ifdef ENABLE_MEMORY_DEBUGGING
	sPreBufferData* pBlock = s_topMemoryBlock;

	while(pBlock)
	{
		verifyPadding(pBlock);
		pBlock = pBlock->nextHeader;
	}
#endif
}

void memory::outputStatistics(uint32 channelMask)
{
#ifdef ENABLE_MEMORY_DEBUGGING
	Debug.output(channelMask, "\n*** Memory Use Statistics ***\n");
	Debug.output(channelMask, "Total Allocations: %i\n", s_allocCount);
	Debug.output(channelMask, "Total Deallocations: %i\n", s_releaseCount);
	Debug.output(channelMask, "Maximum memory used (including debug info): %i\n", s_maximumBytesUsed);
	Debug.output(channelMask, "Maximum memory used by Client: %i\n", s_maximumBytesRequested);
	Debug.output(channelMask, "Maximum memory required: %i\n", s_maximumBytesNeeded);
	Debug.output(channelMask, "Current memory used (including debug info): %i\n", s_totalBytesUsed);
	Debug.output(channelMask, "Current memory used by Client: %i\n", s_totalBytesRequested);
	Debug.output(channelMask, "Current memory required: %i\n", s_totalBytesNeeded);
	Debug.output(channelMask, "*** End Memory Use Statistics ***\n");
#else
	Debug.output(channelMask, "\n*** memory use statistics not available***\n\n");
#endif
}

//****************************************************************************************
// end of file      ( memory.cpp )

