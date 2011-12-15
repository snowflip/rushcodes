#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H
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


//	Name Space declaration
namespace gaia
{


/*	cReferenceCounter
-----------------------------------------------------------------
    
	A cReferenceCounter is a base class which allows the program
	to count open instances.
	
-----------------------------------------------------------------
*/

class cReferenceCounter
{
public:

    // Construction and Destruction...
    cReferenceCounter()
    {
        m_nReferenceCount = 0;
    }

    cReferenceCounter( const cReferenceCounter& Src)
    {
        // Reference counts are tied to a specific instance
        // i.e. they are never copied
        m_nReferenceCount = 0;
    }

    virtual ~cReferenceCounter()
    {
        // assert if this object is still in use
        assert(m_nReferenceCount == 0);
    }

    // Operators...
    cReferenceCounter& operator=(const cReferenceCounter& Src)
    {
        // this function is provided in order to allow 
        // derived classes to provide copy operators.
        // the reference count itself is never copied.
        return *this;
    }

    // Mutators...
    virtual int32 AddRef() 
    {
        assert(m_nReferenceCount != MAX_INT32);
        ++m_nReferenceCount;
		return(m_nReferenceCount);
    };

    virtual int32 Release()
    {
        assert(m_nReferenceCount > 0);
        --m_nReferenceCount;
		return(m_nReferenceCount);
    }

    // Accessors...
    int32 referenceCount()const {return m_nReferenceCount;}

private:

    // the internal reference count, stored in 32bits 
    int32 m_nReferenceCount;
}; 

//- End of cReferenceCounter --------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cReferenceCounter.h )

//----------------------------------------------------------
//$Log: $