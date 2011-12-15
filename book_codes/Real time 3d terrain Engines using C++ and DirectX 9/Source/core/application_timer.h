#ifndef CAPPLICATIONTIMER_H
#define CAPPLICATIONTIMER_H
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
#include "timer.h"

//	Name Space declaration
namespace gaia
{


/*	cApplicationTimer
-----------------------------------------------------------------
    
    The application timer is a global instance of a cTimer.
	It runs non-stop for the duration of the program
    
-----------------------------------------------------------------
*/

class cApplicationTimer : public cTimer
{
public:

    cApplicationTimer():cTimer()
	{
		// start the timer immediatly upon creation
		cTimer::start();
	}

    ~cApplicationTimer()
	{
		// stop the timer on destruction
		cTimer::stop();
	}
};
extern cApplicationTimer applicationTimer;



//- End of cApplicationTimer ------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cApplicationTimer.h )

//----------------------------------------------------------
//$Log: $