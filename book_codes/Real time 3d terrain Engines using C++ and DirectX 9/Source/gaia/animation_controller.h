#ifndef CANIMATIONCONTROLLER_H
#define CANIMATIONCONTROLLER_H
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
#include "core\core.h"
#include "math\math.h"
#include "geometry\geometry.h"

#include <d3dx9.h>
#include "d3dx_frame_manager.h"
//#include "animation_data.h"
#include "game_host.h"


//	Name Space declaration
namespace gaia
{


class cSubNodeGroup;

class cAnimationController
{
public:

	typedef void (*EVENT_CALLBACK)(	int eventCode, 
									uint32 animChannel,
									uint32 animIndex, 
									uint32 lParam, 
									uint32 userData);

	// Data Types & Constants...
	enum eAnimChannel
	{
		k_primaryChannel=0,
		k_overlayChannel,
		k_totalChannels
	};

	enum eConstants
	{
		k_NONE = -1,
		k_maxTracksPerChannel = 2,
		k_totalAnimationTracks = k_totalChannels * k_maxTracksPerChannel,
	};

	enum eDurationControl
	{
		k_loopLimit = 0,
		k_timeLimit,
	};

	enum ePlaybackSettingBits
	{
		k_allowLooping = 0,
		k_noStartTransition,
		k_noEndTransition,
	};

	enum ePlaybackStateBits
	{
		k_active = 0,
		k_playing,
		k_expired,
		k_decaying
	};

	enum eCallbackEventFlags
	{
		k_playEventBegin = 0,
		k_playEventEnd,
		k_playEventInterrupt,
		k_playEventLoop,
	};

	struct sPlaybackSettings
	{
		eDurationControl durationControl;
		u32Flags playbackFlags;
		uint32 playCount;
		float playTime;
		float playSpeed;
		float transitionTime;
		EVENT_CALLBACK func;
		uint32 user_data;	
	};

	struct sAnimationTrack
	{
		uint32 animIndex;
		uint32 d3dxTrackIndex;
		u16Flags stateFlags;
		uint16 currentPlayCount;
		float localTime;
		float localDuration;
		float globalEndTime;
		sPlaybackSettings settings;	
	};

	struct sAnimationChannel
	{
		u16Flags stateFlags;
		uint16 activeIndex;
		float decayStrength;
		float decayStep;
		sAnimationTrack animTrack[k_maxTracksPerChannel];
	};

	// Public Data...

	// Creators...

    cAnimationController();
    ~cAnimationController();

	// Operators...


	// Mutators...
	HRESULT create(cSubNodeGroup* pNodeGroup, cPoolHandle ModelResource);
	HRESULT recreate();
	void destroy();

	bool playAnimation(uint32 animIndex, 
						eAnimChannel channel = k_primaryChannel,
						sPlaybackSettings* settings = 0);

	void play(bool on_off);
	void update();

	void forcePlay(uint32 animIndex, uint32 d3dxtrack, bool onOff);
	void forceTrackWeight(uint32 d3dxtrack, float weight);
	void forcePriorityBlend(float blend);

	// Accessors...
	bool playing()const;
	void copyDefaultSettings(sPlaybackSettings* settings)const;

	float primaryTime()const;
	float overlayTime()const;

private:

	// Private Data...
	cSubNodeGroup*	m_pParentGroup;
	cModelResource*	m_pModelResource;

	u16Flags m_stateFlags;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	float m_overlayStrength;
	float m_overlayStep;
	float m_globalTime;
	sAnimationChannel m_channelList[k_totalChannels];
	sPlaybackSettings m_defaultSettings;

	// Private Functions...
	void clearAllChannelData();

	// unused
    cAnimationController(const cAnimationController& Src);
    cAnimationController& operator=(const cAnimationController& Src);

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cAnimationController
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cAnimationController::cAnimationController()
:m_pParentGroup(0)
,m_pModelResource(0)
,m_stateFlags(0)
,m_pAnimController(0)
,m_overlayStrength(0.0f)
,m_overlayStep(0.0f)
,m_globalTime(0.0f)
{
	m_defaultSettings.durationControl = k_loopLimit;
	m_defaultSettings.playbackFlags = 0;
	m_defaultSettings.playCount = 1;
	m_defaultSettings.playTime = 0.0f;
	m_defaultSettings.playSpeed = 1.0f;
	m_defaultSettings.transitionTime = 6.0f;
	m_defaultSettings.func = 0;
	m_defaultSettings.user_data = 0;
	
	clearAllChannelData();
}

/*	~cAnimationController
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cAnimationController::~cAnimationController()
{
	destroy();
}

inline bool cAnimationController::playing()const
{
	return TEST_BIT(m_stateFlags,k_active) && TEST_BIT(m_stateFlags,k_playing);
}

inline void cAnimationController::copyDefaultSettings(sPlaybackSettings* settings)const
{
	debug_assert(settings, "a valid pointer must be provided");

	memcpy(settings, &m_defaultSettings, sizeof(m_defaultSettings));
}



//- End of cAnimationController ---------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cAnimationController.h )

//----------------------------------------------------------
//$Log: $