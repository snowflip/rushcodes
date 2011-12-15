#define DEBUGCHANNEL_CPP
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
#include "debug_channel.h"
#include "stack_trace.h"
#include <windows.h>
#include <dxerr9.h>

using namespace gaia;

// standard output channel
bool cDefaultOutputChannel::write(const tchar* pText)
{
	OutputDebugString(pText);
	return true;
}


bool cFileOutputChannel::open() 
{
	return m_file.open();
}

void cFileOutputChannel::close() 
{
	m_file.close();
}

bool cFileOutputChannel::write(const tchar* pText) 
{
	m_file.write(pText, lstrlen(pText) * sizeof(tchar));
	return true;
}


//***************************************************************
// end of file      ( debug_channel.cpp )

//----------------------------------------------------------
//$Log: $