#ifndef CODETIMER_H
#define CODETIMER_H
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
#include "data_types.h"
#include "bit_flags.h"
#include "debug.h"


//	Name Space declaration
namespace gaia
{

#ifdef _CODE_TIMING
#define begin_profile(name) static cCodeTimer _ct_##name(_text(#name));\
								_ct_##name.beginSession();

#define end_profile(name) _ct_##name.endSession();

#define profile_scope(name) 	static cCodeTimer _ct_##name(_text(#name));\
						cFunctionTimer _ft_name(&_ct_##name);

#else

#define begin_profile(name) 
#define end_profile(name) 
#define profile_scope(name) 
	
#endif

/*	cCodeTimer
-----------------------------------------------------------------
    
    Code Timers are used to time fuctions and sections of code
    
-----------------------------------------------------------------
*/

class cCodeTimer
{
public:

	static cCodeTimer RootTimer;

	// Data Types & Constants...
	enum eConstants
	{
		k_maxNameLength = 32
	};

	// Public Data...

	// Creators...

    cCodeTimer(const tchar* name_string);
    ~cCodeTimer(){};

	// Operators...

	// Mutators...
	void beginSession();
	void endSession();
	void reset();
	void output(u32Flags messageFlags);

	void outputAllTimers(u32Flags messageFlags);
	void resetAllTimers();

	// Accessors...
	float	averageTime()const;
	float	totalTime()const;
	uint32	totalCalls()const;

	float	maximumTimeSample()const;
	float	minimumTimeSample()const;

	const tchar* name()const;

private:

	// Private Data...
	cCodeTimer* m_nextProfile;
	cCodeTimer* m_lastProfile;

	float	m_totalTime;
	uint32	m_totalCalls;

	float	m_maximumTimeSample;
	float	m_minimumTimeSample;

	tchar	m_name[k_maxNameLength];
	float	m_startTime;

	static cCodeTimer* s_previousTimer;
};


inline float cCodeTimer::averageTime()const
{
	if (m_totalCalls)
	{
		return m_totalTime/(float)m_totalCalls;
	}

	return 0.0f;
}

inline float cCodeTimer::totalTime()const
{
	return m_totalTime;
}

inline uint32 cCodeTimer::totalCalls()const
{
	return m_totalCalls;
}


inline float cCodeTimer::maximumTimeSample()const
{
	return m_maximumTimeSample;
}

inline float cCodeTimer::minimumTimeSample()const
{
	return m_minimumTimeSample;
}

inline const tchar* cCodeTimer::name()const
{
	return m_name;
}



/*	cFunctionTimer
-----------------------------------------------------------------
    
    Add_Description_Here
    
-----------------------------------------------------------------
*/

class cFunctionTimer
{
public:
    cFunctionTimer(cCodeTimer* timer)
	:m_internalTimerLink(timer)
	{
		debug_assert(m_internalTimerLink, "A timer link must be provided");
		m_internalTimerLink->beginSession();
	}

    ~cFunctionTimer()
	{
		m_internalTimerLink->endSession();
	}

private:
	cCodeTimer* m_internalTimerLink;
};


//- End of cCodeTimer -------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cCodeTimer.h )

//----------------------------------------------------------
//$Log: $