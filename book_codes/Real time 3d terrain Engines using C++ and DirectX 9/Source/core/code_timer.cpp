#define CODETIMER_CPP
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

#include "application_timer.h"
#include "code_timer.h"
#include "windows.h"

using namespace gaia;

cCodeTimer cCodeTimer::RootTimer("_ROOT_");
cCodeTimer* cCodeTimer::s_previousTimer = 0;

cCodeTimer::cCodeTimer(const tchar* name_string)
:m_nextProfile(0)
,m_lastProfile(s_previousTimer)
,m_totalTime(0.0f)
,m_totalCalls(0)
,m_maximumTimeSample(0.0f)
,m_minimumTimeSample(MAX_REAL32)
,m_startTime(0.0f)
{
	debug_assert(name_string, "A name must be provided to the code timer");
	lstrcpyn(m_name,name_string, k_maxNameLength);

	if (s_previousTimer) s_previousTimer->m_nextProfile = this;
	s_previousTimer = this;
}


void cCodeTimer::beginSession()
{
	++m_totalCalls;

	if (!m_startTime)
	{
		m_startTime = applicationTimer.elapsedTime();
	}
}

void cCodeTimer::endSession()
{
	if (m_startTime)
	{
		float endTime = applicationTimer.elapsedTime();

		debug_assert(endTime >= m_startTime, "we moved backwards in time!!!?");

		float sample = endTime - m_startTime;
		m_totalTime += sample;

		m_maximumTimeSample = maximum(m_maximumTimeSample, sample);
		m_minimumTimeSample = minimum(m_minimumTimeSample, sample);

		m_startTime = 0.0f;
	}
}

void cCodeTimer::reset()
{
	m_totalTime = 0.0f;
	m_totalCalls = 0;
	m_maximumTimeSample = 0.0f;
	m_minimumTimeSample = MAX_REAL32;
	m_startTime = 0.0f;
}

void cCodeTimer::output(u32Flags messageFlags)
{
	Debug.output(messageFlags,
				"%s, %.4f, %u, %.4f, %.4f, %.4f\n",
				m_name,
				m_totalTime,
				m_totalCalls,
				m_totalCalls ? m_totalTime/(float)m_totalCalls : 0.0f,
				m_minimumTimeSample,
				m_maximumTimeSample);

	reset();
}

void cCodeTimer::outputAllTimers(u32Flags messageFlags)
{
	output(messageFlags);
	if (m_nextProfile) m_nextProfile->outputAllTimers(messageFlags);
}

void cCodeTimer::resetAllTimers()
{
	reset();
	if (m_nextProfile) m_nextProfile->resetAllTimers();
}


//***************************************************************
// end of file      ( Timer.cpp )

//----------------------------------------------------------
//$Log: $