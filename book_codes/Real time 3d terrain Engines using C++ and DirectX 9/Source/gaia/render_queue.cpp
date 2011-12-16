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
#include "render_queue.h"
#include "game_host.h"
#include "scene_node.h"
#include "core\quick_sort.h"


//	Name Space declaration
using namespace gaia;

cRenderEntry* cRenderQueue::lockRenderEntry()
{
	if (m_activeEntries + 1 == k_maxRenderEntries)
	{
		// we are out of space.
		// execute (and reset) 
		// the current queue
		execute();
	}

	cRenderEntry* entry = &m_entryPool[m_activeEntries];

	entry->clear();
	return entry;
}

void cRenderQueue::unlockRenderEntry(cRenderEntry* entry)
{
	m_entryList[m_activeEntries] = entry;
	++m_activeEntries;
}

// cRenderEntry sorting functor
typedef cRenderEntry* LPRenderEntry;
struct sort_less
{
	bool operator()(const LPRenderEntry& a, const LPRenderEntry& b)const
	{
		if (a->sortValueA 
			> b->sortValueA)
		{
			return false;
		}
		else if (a->sortValueA 
			< b->sortValueA)
		{
			return true;
		}

		if (a->sortValueB 
			> b->sortValueB)
		{
			return false;
		}
		else if (a->sortValueB 
			< b->sortValueB)
		{
			return true;
		}

		if (a->sortValueC 
			> b->sortValueC)
		{
			return false;
		}
		else if (a->sortValueC 
			< b->sortValueC)
		{
			return true;
		}

		return false;
	};
};


void cRenderQueue::sortEntryList()
{
	//
	// Perform a standard quick-sort using the
	// sort_less functor above
	//
	profile_scope(cRenderQueue_sortEntryList);

	QuickSort(m_entryList, m_activeEntries, sort_less());
}

void cRenderQueue::reset()
{
	m_activeEntries = 0;
}

//
// This function is responsive to executing 
// the sorted render queue
//
void cRenderQueue::execute()
{
	profile_scope(cRenderQueue_execute);

	if (m_activeEntries)
	{
		cDisplayManager& displayManager = 
			TheGameHost.displayManager();
		LPDIRECT3DDEVICE9 d3dDevice = 
			TheGameHost.d3dDevice();
		
		// sort the entry list
		sortEntryList();


		// issue the callback to render
		// the first item in the queue with all
		// activation flags set
		u32Flags activationFlags(0xffffffff);
		m_entryList[0]->object->renderCallback(
			m_entryList[0], 
			activationFlags);

		// render any additional items,
		// sending only the flags for resources
		// which must be activated
		for (uint32 i=1; i<m_activeEntries; ++i)
		{
			cRenderEntry* currentEntry = m_entryList[i];
			cRenderEntry* previousEntry = m_entryList[i-1];

			activationFlags.value=0;

			//
			// check for effect changes
			//
			if (previousEntry->hEffectFile 
					!= currentEntry->hEffectFile)
			{
				// end the last render method
				cEffectFile* pLastMethod = 
					displayManager.effectFilePool().
					getResource(previousEntry->hEffectFile);
				if (pLastMethod)
				{
					pLastMethod->end();
					safe_release(pLastMethod);
				}

				SET_BIT(activationFlags, k_activateRenderMethod);
				SET_BIT(activationFlags, k_activateRenderMethodPass);
				SET_BIT(activationFlags, k_activateRenderMethodParam);
			}
			else if (previousEntry->renderPass 
					!= currentEntry->renderPass)
			{
				SET_BIT(activationFlags, k_activateRenderMethodPass);
				SET_BIT(activationFlags, k_activateRenderMethodParam);
			}		
			else 
			{
				if (previousEntry->boneCount 
					!= currentEntry->boneCount)
				{
					SET_BIT(activationFlags, k_activateRenderMethodParam);
				}		
				if (previousEntry->detailLevel 
					!= currentEntry->detailLevel)
				{
					SET_BIT(activationFlags, k_activateRenderMethodLOD);
				}		
			}


			//
			// check for model changes
			//
			if (previousEntry->hModel 
					!= currentEntry->hModel
				||
				previousEntry->modelType 
					!= currentEntry->modelType)
			{
				SET_BIT(activationFlags, k_activateModel);
				SET_BIT(activationFlags, k_activateModelParamA);
				SET_BIT(activationFlags, k_activateModelParamB);
			}
			else 
			{
				if (previousEntry->modelParamA 
					!= currentEntry->modelParamA)
				{
					SET_BIT(activationFlags, k_activateModelParamA);
				}		
				if (previousEntry->modelParamB 
						!= currentEntry->modelParamB)
				{
					SET_BIT(activationFlags, k_activateModelParamB);
				}		
			}

			//
			// Check for surface material changes
			//
			if (previousEntry->hSurfaceMaterial 
					!= currentEntry->hSurfaceMaterial)
			{
				SET_BIT(activationFlags, k_activateSurfaceMaterial);
			}		

			//
			// issue the callback to render
			//
			currentEntry->object->renderCallback(
				currentEntry, 
				activationFlags);
		}

		// end the last render method
		cRenderEntry* lastEntry = 
			m_entryList[m_activeEntries-1];
		cEffectFile* pLastMethod = 
			DisplayManager.effectFilePool().
			getResource((cPoolHandle)lastEntry->hEffectFile);

		if (pLastMethod)
		{
			pLastMethod->end();
			safe_release(pLastMethod);
		}
	}
	
	// reset for the next frame
	reset();
}

