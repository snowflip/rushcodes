#ifndef MEMORY_H
#define MEMORY_H
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
#include "numeric_tools.h"
#include "debug.h"


//
// Global Interface Definitions
//
// These are the allocation routines clients should use
//
#define alloc_aligned_memory(size, align)			gaia::memory::alloc(size, align, __FILE__, __LINE__)
#define alloc_memory(size)							gaia::memory::alloc(size, gaia::memory::ALIGN_DEFAULT, __FILE__, __LINE__)
#define realloc_aligned_memory(ptr, size, align)	gaia::memory::realloc(ptr, size, align, __FILE__, __LINE__)
#define realloc_memory(ptr, size)					gaia::memory::realloc(ptr, size, gaia::memory::ALIGN_DEFAULT, __FILE__, __LINE__)
#define free_memory(p)       { if(p) { gaia::memory::free(p);     (p)=0; } }


#define alloc_aligned_array(type, count, align)		new(align) type[(count)];

#define safe_delete(p)       { if(p) { delete (p);     (p)=0; } }
#define safe_delete_array(p) { if(p) { delete[] (p);   (p)=0; } }

#define MAX_MEMORY_FILENAME 128

//	Name Space declaration
namespace gaia
{

/*	memory
------------------------------------------------------------------------------------------
	
	memory block definition and allocation routines. For additional debug support, define
	GAIA_MEM_DEBUG in ALL code using these allocation routines. 
	
------------------------------------------------------------------------------------------
*/
namespace memory
{
	// Data Types & Constants...
	enum ALIGNMENT
	{
		ALIGN_DEFAULT= -1,
		ALIGN_NONE	= 0,
		ALIGN_4		= 4,
		ALIGN_8		= 8,
		ALIGN_16	= 16,
		ALIGN_32	= 32
	};

	enum MEM_CONSTANTS
	{
		MAX_CALL_DEPTH		= 16,	// depth of the recorded callstack
		MAX_CLIENT_FILENAME = 32,	// number of letters recorded for the client filename
		PATTERN_SIZE = 32,
	};

	enum MEM_PATERNS
	{
		PRE_PATTERN		= 0xab,	// memory pattern written ahead of allocated client space (GAIA_MEM_DEBUG only)	
		POST_PATTERN	= 0xef,	// memory pattern written after allocated client space (GAIA_MEM_DEBUG only)
	};

	struct sMemoryBlockHeader
	{
		uint32	actualSize : 24;		// the true size of the allocation
		uint32	pointerOffset : 8;	// an offset to the top of the allocation
	};

	struct sPreBufferData
	{
		sPreBufferData* nextHeader;				//linked-list information
		sPreBufferData* previousHeader;
		uint32	requestedSize;					// the size requested by the caller
		uint32	userChecksum;					// a checksum built and tested on demand
		char	fileName[MAX_MEMORY_FILENAME];	// filename of the caller
		uint32	fileLine;						// line number of the caller
		uint8	bytePattern[PATTERN_SIZE];				// underwriting test space
	};

	struct sPostBufferData
	{
		uint8	bytePattern[PATTERN_SIZE];				// overwriting test space
	};

	// memory management functions
	void setDefaultAlignment(ALIGNMENT align);
	void* alloc(size_t Size, ALIGNMENT Align = ALIGN_DEFAULT, const char* FileName =0, int LineNumber =0);
	void free(void* pMem);
	void* realloc(void* pMem, size_t Size, ALIGNMENT Align = ALIGN_DEFAULT, const char* FileName =0, int LineNumber =0);

	// memory debugging functions
	void checkPadding(void* pMem);
	void buildChecksum(void* pMem);
	void testChecksum(void* pMem);

	void checkAllPadding();
	void outputStatistics(uint32 channelMask = cDebugMessageHandler::k_defaultMessageFlag);

} //End NameSpace: memory
} //End NameSpace: gaia


//=============================================================================================
//	memory allocation operator overloads.
//=============================================================================================

// operator new overload
//------------------------------------------------------------------------------
inline void* operator new(size_t size, gaia::memory::ALIGNMENT Align)
{
	return(alloc_aligned_memory(size,Align));
}

//------------------------------------------------------------------------------
// operator delete overload
//------------------------------------------------------------------------------
inline void operator delete(void* pMem, gaia::memory::ALIGNMENT Align)
{
	//
	//	Safeguard against a NULL pointer, since it is legal to pass NULL to "operator delete".
	//
	if(pMem)
	{
		free_memory(pMem);
	}
}

//- End of memory ------------------------------------------------------------------------

//****************************************************************************************







#endif  // end of file      ( memory.h )

