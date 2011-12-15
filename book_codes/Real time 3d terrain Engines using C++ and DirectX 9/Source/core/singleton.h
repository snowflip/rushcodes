#ifndef SINGLETON_H
#define SINGLETON_H
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
#include "debug.h"

//	Name Space declaration
namespace gaia
{


/*	cSingleton
-----------------------------------------------------------------
    
	A singleton is a type of class of which only one instance may 
	exist. This is commonly used for management classes used to 
	control system-wide resources. 
	
-----------------------------------------------------------------
*/
// disable the warning regarding 'this' pointers being used in 
// base member initializer list. Singletons rely on this action
#pragma warning(disable : 4355)

template <class T>
class cSingleton
{
public:

    // the singleton must be constructed
    // with a reference to the controlled object
    cSingleton(T& rObject)
    {
        debug_assert(!s_pInstance, "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }

    // the singleton accessor
    ~cSingleton()
    {
        debug_assert(s_pInstance, "The singleton instance is invalid.");
        s_pInstance = 0;
    }

    // the singleton accessor
    static T& instance()
    {
        debug_assert(s_pInstance, "The singleton has not yet been created.");
        return (*s_pInstance);
    }

private:

    // Data...
    static T* s_pInstance;

    // Nonexistant Functions...
    cSingleton( const cSingleton& Src);
    cSingleton& operator=( const cSingleton& Src);

};
template <class T> T* cSingleton<T>::s_pInstance = 0;

//- End of cSingleton --------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cSingleton.h )

//----------------------------------------------------------
//$Log: $