#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
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
#ifndef DATATYPES_H
#include "data_types.h"
#endif

#ifndef NUMERICTOOLS_H
#include "numeric_tools.h"
#endif
#include "bit_flags.h"

//	Name Space declaration
namespace gaia
{


/*	cSystemInfo
-----------------------------------------------------------------
    
    cSystemInfo is a simple singleton object that can be used to 
	query information about the host system. It reports OS type,
	CPU features and memory stats.
    
-----------------------------------------------------------------
*/

class cSystemInfo
{
public:

	// Data Types & Constants...
	enum WINDOWS_VERSION
	{
		UNKNOWN	=0,					
		WINDOWS_95,				
		WINDOWS_95_SR2,	
		WINDOWS_98,			
		WINDOWS_98_SR2,			
		WINDOWS_ME,
		WINDOWS_NT,
		WINDOWS_2K,
		WINDOWS_XP,
		// the final value is for 
		// versions of windows 
		// yet unreleased
		WINDOWS_FUTURE
	};	

	struct OS_VERSION
	{
		uint32 MajorVersion; 
		uint32 MinorVersion;
		uint32 Build;
	};

	enum CPU_FLAGS
	{
		SUPPORTS_MMX	= 23,	// processor supports MMX extensions
		SUPPORTS_SSE	= 25,	// processor supports Intel SSE extensions
		SUPPORTS_SSE2	= 26,	// processor supports Intel SSE2 extensions
	};


	// Creators...

    cSystemInfo();
    ~cSystemInfo(){};

	void readCPUCounter(uint64 *pCounter);

	// Accessors...
	u32Flags			cpuFlags();
	int					platform();
	const OS_VERSION&	osVersion();
	uint32				totalMemory();
	uint32				physicalMemory();
	uint32				availableMemory();

private:

	// Private Data...
	u32Flags		m_cpuFlags;
	int				m_platform;
	OS_VERSION		m_osVersion;
	uint32			m_totalMemory;
	uint32			m_physicalMemory;
	uint32			m_processorSpeed;

	void querySystemInformation();
	bool verifyCPUID();
	void verifySSE();
	void verifySSE2();
	void readCpuFlags();

	void computeProcessorSpeed();

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

inline cSystemInfo::cSystemInfo()
{
	querySystemInformation();
}

//. Accessors ...................................................

inline u32Flags cSystemInfo::cpuFlags()
{
	return(m_cpuFlags);
}

inline int cSystemInfo::platform()
{
	return(m_platform);
}

inline const cSystemInfo::OS_VERSION& cSystemInfo::osVersion()
{
	return(m_osVersion);
}

inline uint32 cSystemInfo::totalMemory()
{
	return(m_totalMemory);
}

inline uint32 cSystemInfo::physicalMemory()
{
	return(m_physicalMemory);
}

//- End of cSystemInfo -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSystemInfo.h )

//----------------------------------------------------------
//$Log: $