#define CANIMATIONCONTROLLER_CPP
#define GAIA_DLL
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

#include "animation_controller.h"
#include "subnode_group.h"

using namespace gaia;

void cAnimationController::clearAllChannelData()
{
	// clear all channel data
	memset(m_channelList, 0, sizeof(m_channelList));

	// set the d3dx track indicies
	for (int i=0; i<k_totalChannels; ++i)
	{
		for (int j=0; j<k_maxTracksPerChannel; ++j)
		{
			m_channelList[i].animTrack[j].d3dxTrackIndex = (i*k_maxTracksPerChannel)+j;
		}
	}
}

HRESULT cAnimationController::create(cSubNodeGroup* pNodeGroup, cPoolHandle ModelResource)
{
	// destroy any current information
	destroy();

	HRESULT hr = S_OK;

	if (pNodeGroup)
	{
		m_pParentGroup = pNodeGroup;
		m_pModelResource = (cModelResource*)TheGameHost.displayManager().modelPool().getResource(ModelResource);

		hr = recreate();
	}
	return hr;
}

HRESULT cAnimationController::recreate()
{
	HRESULT hr = S_OK;

	if (m_pModelResource && m_pModelResource->animationController())
	{
		// create a local animation controller
		safe_release(m_pAnimController);
		if (m_pModelResource->animationController())
		{
			hr = m_pModelResource->animationController()->CloneAnimationController(          
				m_pModelResource->animationController()->GetMaxNumMatrices(),
				m_pModelResource->animationController()->GetMaxNumAnimationSets(),
				k_totalAnimationTracks,
				m_pModelResource->animationController()->GetMaxNumEvents(),
				&m_pAnimController
			);

			if (FAILED(hr))
			{
				debug_error(hr);
				destroy();
				return hr;
			}

			m_pAnimController->SetPriorityBlend(0.0f);

			for (int iChannel=0; iChannel<k_totalChannels; ++iChannel)
			{
				for (int iTrack=0; iTrack<k_maxTracksPerChannel; ++iTrack)
				{
					uint32 d3dxTrack = m_channelList[iChannel].animTrack[iTrack].d3dxTrackIndex;

					D3DXTRACK_DESC trackDesc;
					m_pAnimController->GetTrackDesc(d3dxTrack, &trackDesc);
					trackDesc.Flags = (iChannel != k_primaryChannel) ? D3DXTF_HIGHPRIORITY:D3DXTF_LOWPRIORITY;
					m_pAnimController->SetTrackDesc(d3dxTrack, &trackDesc);
				}
			}

			// register all named nodes with this controller
			uint32 totalNodes = m_pParentGroup->totalSubNodes();
			debug_assert(totalNodes == m_pModelResource->totalFrames(), "node counts do not match");

			for (uint32 i=0; i<totalNodes; ++i)
			{
				cSceneNode* pNode = m_pParentGroup->subNodePtr(i);

				cSIMDMatrix* pLocalMatrix = pNode->accessLocalMatrix();

				// if the source was a named node, register with the animation controller
				if (m_pModelResource->frame((uint16)i)->Name)
				{
					hr = m_pAnimController->RegisterMatrix(m_pModelResource->frame((uint16)i)->Name, pLocalMatrix);

					if (FAILED(hr))
					{
						debug_error(hr);
						destroy();
						return hr;
					}
				}
			}

			// set the innitial playback state
			m_stateFlags = FLAG(k_active);

			// clear all tracks
			uint32 totalTracks = m_pAnimController->GetMaxNumTracks();
			for (uint32 iTrack=0; iTrack<totalTracks; ++iTrack)
			{
				m_pAnimController->SetTrackEnable(iTrack, FALSE);
			}

		}
	}

	return hr;
}


void cAnimationController::destroy()
{
	m_stateFlags = 0;

	if (m_pAnimController)
	{
		// clear all tracks
		uint32 totalTracks = m_pAnimController->GetMaxNumTracks();
		for (uint32 iTrack=0; iTrack<totalTracks; ++iTrack)
		{
			m_pAnimController->SetTrackEnable(iTrack, FALSE);
		}
	}

	safe_release(m_pAnimController);
	clearAllChannelData();

	// release our reference to the source model
	safe_release(m_pModelResource);

	m_pParentGroup=0;
}

void cAnimationController::forcePlay(uint32 animIndex, uint32 d3dxtrack, bool onOff)
{
	if (onOff)
	{
		LPD3DXANIMATIONSET animationSet;
		m_pAnimController->GetAnimationSet(
			animIndex,
			&animationSet);

		// move the new animation into the primary track
		m_pAnimController->SetTrackEnable(d3dxtrack, TRUE);
		m_pAnimController->SetTrackAnimationSet(d3dxtrack, animationSet);
		m_pAnimController->SetTrackSpeed(d3dxtrack, 1.0f);
		m_pAnimController->SetTrackAnimTime(d3dxtrack, 0.0f);
		m_pAnimController->SetTrackWeight(d3dxtrack, 1.0f);
		
		safe_release(animationSet);
	}
	else
	{
		m_pAnimController->SetTrackEnable(d3dxtrack, FALSE);
	}
}

void cAnimationController::forceTrackWeight(uint32 d3dxtrack, float weight)
{
	m_pAnimController->SetTrackWeight(d3dxtrack, weight);
}

void cAnimationController::forcePriorityBlend(float weight)
{
	m_pAnimController->SetPriorityBlend(weight);
	m_overlayStrength = weight;
}

bool cAnimationController::playAnimation(
							uint32 animIndex, 
							eAnimChannel channel,
							sPlaybackSettings* settings)
{
	if (TEST_BIT(m_stateFlags,k_active))
	{
		uint32 totalAnimations = (uint32)m_pAnimController->GetNumAnimationSets();
		if (animIndex < totalAnimations)
		{
			if (!settings)
			{
				settings = &m_defaultSettings;
			}

			// get a reference to the channel we will be working with
			sAnimationChannel& animChannel = m_channelList[channel];

			// what is the weight scale for this channel?
			float weightScale = channel == k_overlayChannel ?
									m_overlayStrength : 1.0f-m_overlayStrength;

			// swap the active index number and reverse the cross fade
			animChannel.activeIndex = !animChannel.activeIndex;
			animChannel.decayStrength = 1.0f;

			// setup some shortcuts to the two tracks we will be adjusting
			sAnimationTrack& primaryTrack = animChannel.animTrack[animChannel.activeIndex];
			sAnimationTrack& decayTrack = animChannel.animTrack[!animChannel.activeIndex];

			// assume we will not be performing a decay unless
			// the tests below pass
			bool performDecay = false;

			// do we perform a transition ?
			if (!TEST_BIT(settings->playbackFlags, k_noStartTransition)
				&& !TEST_BIT(decayTrack.settings.playbackFlags,k_noEndTransition))
			{
				// if there is something playing in the decay track
				// setup our blend weights
				if (TEST_BIT(decayTrack.stateFlags,k_active))
				{
					// how much time does this track have left to play?
					float timeRemaining = decayTrack.globalEndTime - m_globalTime;
					float transitionTime = settings->transitionTime;
					float transitionTicks = transitionTime/k_millisecondsPerTick;
					
					// is there at least one full tick of transition
					if (transitionTicks >= 1.0f)
					{
						// set this track to decay over time until finished
						decayTrack.stateFlags.setBit(k_decaying);
						decayTrack.settings.durationControl = k_timeLimit;
						decayTrack.globalEndTime = m_globalTime + transitionTime;
						animChannel.decayStep = -animChannel.decayStrength/transitionTicks;
						performDecay = true;
					}

					// send an interrupt callback
					if (!TEST_BIT(decayTrack.stateFlags,k_expired))
					{
						if (decayTrack.settings.func)
						{
							decayTrack.settings.func(
								k_playEventInterrupt, 
								channel, 
								decayTrack.animIndex, 
								animIndex, 
								decayTrack.settings.user_data);
						}
					}
				}
			}

			// if we are not performing the decay, clear the animation
			// currently in the decay track
			if (!performDecay)
			{
				animChannel.decayStrength = 0.0f;
				animChannel.decayStep = 0.0f;

				// disable this track
				m_pAnimController->SetTrackEnable(decayTrack.d3dxTrackIndex, FALSE);
				decayTrack.stateFlags = 0;
			}


			// get the desired animation from the controller
			LPD3DXANIMATIONSET animationSet;
			m_pAnimController->GetAnimationSet(
				animIndex,
				&animationSet);

			// move the new animation into the primary track
			m_pAnimController->SetTrackAnimationSet(primaryTrack.d3dxTrackIndex, animationSet);
			m_pAnimController->SetTrackEnable(primaryTrack.d3dxTrackIndex, TRUE);
			m_pAnimController->SetTrackSpeed(primaryTrack.d3dxTrackIndex, 1.0f);
			m_pAnimController->SetTrackAnimTime(primaryTrack.d3dxTrackIndex, 0.0f);
			
			// set the two track weights
			float primaryWeight	=(1.0f-animChannel.decayStrength);
			float decayWeight		=(animChannel.decayStrength);
			m_pAnimController->SetTrackWeight(primaryTrack.d3dxTrackIndex, primaryWeight);
			m_pAnimController->SetTrackWeight(decayTrack.d3dxTrackIndex, decayWeight);

			// setup the playback information
			memcpy(&primaryTrack.settings, settings, sizeof(sPlaybackSettings));

			primaryTrack.animIndex = animIndex;
			primaryTrack.stateFlags = FLAG(k_active)|FLAG(k_playing);
			primaryTrack.currentPlayCount = 0;
			primaryTrack.localTime = 0.0f;
			primaryTrack.localDuration = (float)animationSet->GetPeriod();


			safe_release(animationSet);

			// compute the amount of time to play this animation
			if (settings->durationControl == k_timeLimit)
			{
				primaryTrack.globalEndTime 
					= m_globalTime + settings->playTime;
			}
			else
			{
				primaryTrack.globalEndTime 
					= m_globalTime + (primaryTrack.localDuration * settings->playCount);
			}

			debug_print("\n");
			debug_print("playing animation in track %i\n", primaryTrack.d3dxTrackIndex);
			debug_print(" time =  %.02f\n", m_globalTime);
			debug_print(" duration= %.02f\n", primaryTrack.localDuration);
			debug_print(" ending at= %.02f\n", primaryTrack.globalEndTime);
			if (performDecay)
			{
				debug_print(" transition ending= %.02f\n", decayTrack.globalEndTime);
			}
			else
			{
				debug_print(" no transition\n");
			}
			debug_print(" track %i weight= %.02f\n", primaryTrack.d3dxTrackIndex, primaryWeight);
			debug_print(" track %i weight= %.02f\n", decayTrack.d3dxTrackIndex, decayWeight);
			debug_print("\n");

			// send a begin callback
			if (primaryTrack.settings.func)
			{
				primaryTrack.settings.func(k_playEventBegin, 
					channel, 
					primaryTrack.animIndex, 
					0, 
					primaryTrack.settings.user_data);
			}

			return true;
		}
	}
	return false;
}

void cAnimationController::play(bool on_off)
{
	if (TEST_BIT(m_stateFlags,k_active))
	{
		m_stateFlags.setBit(k_playing, on_off);
	}
}

void cAnimationController::update()
{
	if (TEST_BIT(m_stateFlags,k_active) && TEST_BIT(m_stateFlags,k_playing))
	{
		// update the player
		float timeStep = k_millisecondsPerTick;
		m_globalTime += timeStep;

		// update our global cross-fade weight
		m_overlayStrength += m_overlayStep;

		if (m_overlayStrength < 0.0f)
		{
			m_overlayStrength = 0.0f;
			m_overlayStep = 0.0f;
		}
		else if (m_overlayStrength > 1.0f)
		{
			m_overlayStrength = 1.0f;
			m_overlayStep = 0.0f;
		}

		int totalActiveTracks = 0;
		int totalUpdatedTracks = 0;

		// forcast any ending animations and remove or adjust them 
		// prior to updating the animation playback time
		for (int channel=0; channel<k_totalChannels; ++channel)
		{
			int totalActiveTracksInChannel = 0;

			// get a reference to the channel we will be working with
			sAnimationChannel& animChannel = m_channelList[channel];

			// what is the weight scale for this channel?
			float weightScale = channel == k_overlayChannel ?
									m_overlayStrength : 1.0f-m_overlayStrength;

			// update the fade-out weight for this channel
			if (animChannel.decayStep)
			{
				animChannel.decayStrength += animChannel.decayStep;
				
				if (animChannel.decayStrength < 0.0f)
				{
					animChannel.decayStrength = 0.0f;
					animChannel.decayStep = 0.0f;

					sAnimationTrack& decayTrack = animChannel.animTrack[!animChannel.activeIndex];

					if (TEST_BIT(decayTrack.stateFlags,k_active))
					{
						debug_print("track %i finished decay at %.02f\n", decayTrack.d3dxTrackIndex, m_globalTime);
						decayTrack.stateFlags=0;
						m_pAnimController->SetTrackEnable(decayTrack.d3dxTrackIndex, FALSE);
					}
				}
			}


			// adjust any tracks which are active
			for (int track=0; track<k_maxTracksPerChannel; ++track)
			{
				sAnimationTrack& currentTrack = animChannel.animTrack[track];
				bool playbackExpired = false;

				if (TEST_BIT(currentTrack.stateFlags,k_active))
				{
					if (TEST_BIT(currentTrack.stateFlags,k_expired)
						&& !TEST_BIT(currentTrack.stateFlags,k_decaying))
					{
						currentTrack.stateFlags = 0;
					//	m_pAnimController->SetTrackEnable(currentTrack.d3dxTrackIndex, FALSE);
					}

					if (TEST_BIT(currentTrack.stateFlags,k_playing))
					{
						++totalUpdatedTracks;
						
						// update local time for this track
						currentTrack.localTime += timeStep;

						// look for loop condition
						if (currentTrack.localTime+0.01f >= currentTrack.localDuration)
						{
							// increment the internal loop count
							currentTrack.currentPlayCount++;

							// have we exceeded our play limit?
							if (currentTrack.settings.durationControl == k_loopLimit)
							{
								if (currentTrack.currentPlayCount 
									>= currentTrack.settings.playCount)
								{
									playbackExpired = true;
									debug_print("track %i completed looping\n", currentTrack.d3dxTrackIndex, m_globalTime);
								}
							}

							// do we loop or stick on the last frame?
							if (!TEST_BIT(currentTrack.settings.playbackFlags,k_allowLooping)
								|| playbackExpired)
							{
								// stick at the end of the animation
								currentTrack.localTime = 
									maximum(0.0f, currentTrack.localDuration-0.01f);

								// hand-set the animation time
								m_pAnimController->SetTrackAnimTime(
									currentTrack.d3dxTrackIndex, 
									currentTrack.localTime);

								// stop time advancing on this track
								m_pAnimController->SetTrackSpeed(
									currentTrack.d3dxTrackIndex, 
									0.0f);

								// this track is no longer animating
								currentTrack.stateFlags.clearBit(k_playing);
							}
							else
							{
								// keep track of our local time after the loop
								currentTrack.localTime -= currentTrack.localDuration;

								// send a loop callback
								if (currentTrack.settings.func)
								{
									currentTrack.settings.func(
										k_playEventLoop, 
										channel,
										currentTrack.animIndex,
										0, 
										currentTrack.settings.user_data);
								}

							}
						}
					}

					// check global time for playback limits
					if (currentTrack.settings.durationControl == k_timeLimit)
					{
						if (m_globalTime >= currentTrack.globalEndTime)
						{
							playbackExpired = true;

							debug_print("track %i expired at %.02f\n", currentTrack.d3dxTrackIndex, m_globalTime);
						}
					}

					// step the track if expired
					if (playbackExpired || !TEST_BIT(currentTrack.stateFlags,k_playing))
					{
						// set the expiration flags
						currentTrack.stateFlags.setBit(k_expired);
						currentTrack.stateFlags.clearBit(k_playing);
						
						// stop time advancing on this track
						m_pAnimController->SetTrackSpeed(currentTrack.d3dxTrackIndex, 0.0f);
							
						if (track == animChannel.activeIndex)
						{
								// issue the callback for this animation
								if (currentTrack.settings.func)
								{
									currentTrack.settings.func(
										k_playEventEnd, 
										channel,
										currentTrack.animIndex,
										0, 
										currentTrack.settings.user_data);
								}
						}
					}
					

					{
						++totalActiveTracks;
						++totalActiveTracksInChannel;

						float localWeight = (track == animChannel.activeIndex)
							?  1.0f - animChannel.decayStrength :animChannel.decayStrength;

						m_pAnimController->SetTrackWeight(currentTrack.d3dxTrackIndex, localWeight);
					//	debug_print("setting track %i weight to %.02f\n", currentTrack.d3dxTrackIndex, localWeight);
					}
				}
			}

		}

		// if there were any tracks left alive, update the controller
		// and frame matrices
//		if (totalUpdatedTracks)
		{
			// advance the playback controller
			m_pAnimController->SetTime(m_globalTime);

			// tell the subnodes to flag their named matrices as dirty
			m_pParentGroup->adjustForAnimationStep();
		}
	}
}

float cAnimationController::primaryTime()const
{
	if (m_pAnimController)
	{
		D3DXTRACK_DESC desc;

		HRESULT hr = m_pAnimController->GetTrackDesc(0, &desc);

		if (SUCCEEDED(hr) && desc.Enable)
		{
			return (float)desc.AnimTime;
		}
	}
	return 0.0f;
}

float cAnimationController::overlayTime()const
{
	if (m_pAnimController)
	{
		 D3DXTRACK_DESC desc;

		 m_pAnimController->GetTrackDesc(k_maxTracksPerChannel, &desc);
		 return (float)desc.AnimTime;
	}
	return 0.0f;
}


//***************************************************************
// end of file      ( cAnimationController.cpp )

//----------------------------------------------------------
//$Log: $