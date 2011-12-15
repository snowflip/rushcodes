#ifndef COM_HELPERS_H
#define COM_HELPERS_H
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

#ifndef DEBUG_H
#include "debug.h"
#endif


//	Name Space declaration
namespace gaia
{


/*	com_helpers
-----------------------------------------------------------------
    
    Simple tools to emulate IUnknown functionality in COM
    
-----------------------------------------------------------------
*/
template <class T> inline int remove_reference(T *& p)
{
	if (p)
	{
		return((p)->Release());
	}
	return(0);
}

template <class T> inline int safe_release(T *& p)
{
	if (p)
	{
		int safe_release_refcount = (p)->Release();
		if (safe_release_refcount !=0) 
		{
			Debug.output(0, "WARNING: non-zero reference count on release (%i)\n",safe_release_refcount);
		}
		p = 0;

		return(safe_release_refcount);
	}
	return(0);
}

//- End of com_helpers ------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( com_helpers.h )

//----------------------------------------------------------
//$Log: $