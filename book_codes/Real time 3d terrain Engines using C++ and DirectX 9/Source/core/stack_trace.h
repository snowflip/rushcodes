#ifndef STACKTRACE_H
#define STACKTRACE_H
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

#ifndef STRING_H
#include "string.h"
#endif

//	Name Space declaration
namespace gaia
{


/*	Stack Tracing
------------------------------------------------------------------------------------------
	
	The stack tracing code is based on an article by John Robbins
	that appeared in Microsoft Systems Journal Bugslayer Column - Feb 99

	The article can be found at:
	http://www.microsoft.com/msj/defaultframe.asp?page=/msj/0299/bugslayer/bugslayer0299.htm

------------------------------------------------------------------------------------------
*/

void buildStackTrace(tchar* szString, uint32 dwSize, uint32 dwNumSkip);

} //End NameSpace: gaia

#endif  // end of file      ( cDebug.h )

//----------------------------------------------------------
//$Log: $