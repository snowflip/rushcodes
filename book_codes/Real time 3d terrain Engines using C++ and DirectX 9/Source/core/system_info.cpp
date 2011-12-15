#define SYSTEMINFO_CPP
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

#include "code_timer.h"
#include "system_info.h"
#include "timer.h"
#include <windows.h>

using namespace gaia;


bool cSystemInfo::verifyCPUID()
{
	uint32 result=0;
	_asm{
        pushfd                   // Get original EFLAGS
        pop         eax
        mov         ecx, eax
        xor         eax, 200000h // Flip ID bit in EFLAGS
        push        eax          // Save new EFLAGS value on stack        									
        popfd                    // Replace current EFLAGS value
        pushfd                   // Get new EFLAGS
        pop         eax          // Store new EFLAGS in EAX
        xor         eax, ecx                  
        jz          THE_END      // Failure - NO CPUID Support

        // The processor supports the CPUID instruction.
        mov         result,1
	THE_END:
	}

	return result ? true:false;
}

void cSystemInfo::readCpuFlags()
{
	m_cpuFlags = 0;

	if(verifyCPUID())
	{
		uint32 flags=0;
		_asm
		{
			pushad
			mov		eax,1			; select feature flags
			cpuid
			mov		flags,edx
			popad
		}

		m_cpuFlags = flags;
	}
}

// report if OS allows Intel SIMD extensions
void cSystemInfo::verifySSE()
{
	// if the SSE support bit is set, verify support with the OS
	if (m_cpuFlags.testBit(cSystemInfo::SUPPORTS_SSE))
	{
		__try
		{
			__asm
			{
				pushad;
				orps xmm1,xmm1; 
				popad;
			}		
		}
		__except(1)
		{
			// clear the bit and return
			m_cpuFlags.clearBit(cSystemInfo::SUPPORTS_SSE2);
			return;
		}
	}
}

// report if OS allows Intel SIMD2 extensions
void cSystemInfo::verifySSE2()
{
	// if the SSE2 support bit is set, verify support with the OS
	if (m_cpuFlags.testBit(cSystemInfo::SUPPORTS_SSE2))
	{
		__try
		{
			__asm
			{
				pushad;
				paddq xmm1, xmm2
				popad;
			}		
		}
		__except(1)
		{
			// clear the bit and return
			m_cpuFlags.clearBit(cSystemInfo::SUPPORTS_SSE2);
			return;
		}
	}
}

void cSystemInfo::readCPUCounter(uint64 *pCounter)
{
	_asm
	{
		RDTSC
		mov edi, pCounter
		mov DWORD PTR [edi], eax
		mov DWORD PTR [edi+4], edx
	};
}

void cSystemInfo::computeProcessorSpeed()
{
	uint64 startTime, endTime;
	cTimer localTimer;

	// start the timer
	localTimer.start();
	// sample the cpu counter
	readCPUCounter(&startTime);
	// waste some time
	Sleep(100);
	// resample the cpu counter
	readCPUCounter(&endTime);
	// stop the clock
	localTimer.stop();

	// compute the CPU speed 
	// as ticks per millisecond
	uint64 sampleDelta = 
		endTime - startTime;
	uint32 elapsedMilliseconds = 
		localTimer.elapsedMilliseconds();
	m_processorSpeed = 
		(uint32)sampleDelta/elapsedMilliseconds;
}

void cSystemInfo::querySystemInformation()
{
	SYSTEM_INFO		SysInfo;
	MEMORYSTATUS	MemStatus;
	OSVERSIONINFO 	OSVersion;

	// read the memory status
	MemStatus.dwLength = sizeof (MemStatus);
	GlobalMemoryStatus(&MemStatus);

	// read the OS Version data
	OSVersion.dwOSVersionInfoSize =sizeof(OSVersion);
	GetVersionEx(&OSVersion);

	// read the System Info
	GetSystemInfo(&SysInfo);

	// fill in our data members
	m_physicalMemory			=MemStatus.dwTotalPhys;
	m_totalMemory				=MemStatus.dwAvailPhys+ MemStatus.dwAvailPageFile;
	m_osVersion.MajorVersion	=OSVersion.dwMajorVersion;
	m_osVersion.MinorVersion	=OSVersion.dwMinorVersion;


	//
	// Figure out which OS this is
	//
	if (OSVersion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
	{
		m_osVersion.Build=LOWORD(OSVersion.dwBuildNumber);

		m_platform=WINDOWS_95;

		if (m_osVersion.MinorVersion==0 && m_osVersion.Build>950)
		{
			m_platform=WINDOWS_95_SR2;
		}
		else if (m_osVersion.MinorVersion==10)
		{
			m_platform=WINDOWS_98;
		}
		else if (m_osVersion.MinorVersion>10)
		{
			m_platform=WINDOWS_ME;
		}
	}
	else if (OSVersion.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{
		m_osVersion.Build =OSVersion.dwBuildNumber;

		if (m_osVersion.MajorVersion<4)
		{
			m_platform=WINDOWS_NT;
		}
		else if (m_osVersion.MajorVersion == 4)
		{
			m_platform=WINDOWS_2K;
		}
		else if (m_osVersion.MajorVersion == 5)
		{
			m_platform=WINDOWS_XP;
		}
		else
		{
			m_platform=WINDOWS_FUTURE;
		}
	}
	else
	{
		m_platform			=UNKNOWN;
		m_osVersion.Build	=OSVersion.dwBuildNumber;
	}

	//
	// Check for extended CPU facilities
	//
	readCpuFlags();
	verifySSE();
	verifySSE2();

	computeProcessorSpeed();
}

uint32	cSystemInfo::availableMemory()
{
	MEMORYSTATUS	MemStatus;

	// read the memory status
	MemStatus.dwLength = sizeof (MemStatus);
	GlobalMemoryStatus(&MemStatus);

	// return the amount of available, physical memory
	return(MemStatus.dwAvailPhys);
}


//***************************************************************
// end of file      ( SystemInfo.cpp )

//----------------------------------------------------------
//$Log: $