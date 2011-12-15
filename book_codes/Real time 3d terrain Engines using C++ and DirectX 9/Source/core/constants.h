#ifndef CONSTANTS_H
#define CONSTANTS_H
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
#include "bit_flags.h"

//	Name Space declaration
namespace gaia
{


/*	Constants
-----------------------------------------------------------------
    
    constant values used throughout the engine
    
-----------------------------------------------------------------
*/

enum eLockFlagBits
{
	nLockReadBit	=0,	// lock data for reading
	nLockWriteBit,		// lock data for selective writing (not all data is changed)
	nLockReplaceBit,	// lock data to completely overwrite all data
	nLockExclusiveBit,		// allow no other locks while active
	nLockWaitBit,			// wait until data is available
};

enum eLockFlags
{
	nLockRead			=FLAG(nLockReadBit),
	nLockWriteFlag		=FLAG(nLockWriteBit),
	nLockReplaceFlag	=FLAG(nLockWriteBit),
	nLockExclusive		=FLAG(nLockExclusiveBit),
	nLockWait			=FLAG(nLockWaitBit),
};

enum eLockType
{
	nReadLock=0,
	nWriteLock,
	nReadWriteLock,
};

} //End NameSpace: gaia

#endif  // end of file      ( Constants.h )

//----------------------------------------------------------
//$Log: $