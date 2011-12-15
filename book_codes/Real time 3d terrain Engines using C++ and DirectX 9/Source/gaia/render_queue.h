#ifndef CRENDERQUEUE_H
#define CRENDERQUEUE_H
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
#include "effect_file.h"

//	Name Space declaration
namespace gaia
{
class cRenderEntry;
class cRenderQueue;
class cSceneNode;


/*	cRenderEntry
-----------------------------------------------------------------
    
    A Render Entry is a 20 byte piece of data used to represent 
	a desired render operation in the queue. The top 12 bytes
	represent a numerical value which allows us to sort these
	objects into an optimal render order. 

	Render Entries are sorted in the queue by the following data

	cPoolHandle hEffectFile;// which effect file to use
	uint8 renderPass;		// which render pass to use
	uint8 renderParam :6;	// a user-defined render parameter
	uint8 modelType	  :2;	// how to interperet the model data
	cPoolHandle hModel;		// primary vertex buffer or model index
	uint16 modelParamA;		// secondary vertex buffer or model frame
	uint16 modelParamB;		// index buffer or model subset
	cPoolHandle hSurfaceMaterial;	// the surface material used

	modelType describes whether the hModel, modelParamA and 
	modelParamB values contain actual model data or raw
	vertex and index buffer indices. The modelType value itself
	is taken from the eTypeFlags enum in cRenderEntry.

-----------------------------------------------------------------
*/

// these flags are passed to the render callbacks to let the
// object know which of it's render components need to be activated
enum eActivationFlagBits
{
	k_activateRenderMethod = 0,
	k_activateRenderMethodPass,
	k_activateRenderMethodParam,
	k_activateRenderMethodLOD,
	k_activateModel,
	k_activateModelParamA,
	k_activateModelParamB,
	k_activateSurfaceMaterial,

	k_totalActivationFlags
};


class cRenderEntry
{
public:

// we turn on byte packing to ensure a tight fit
#pragma pack(1)

	// FIELDS USED TO SORT ENTRY (12 bytes)
	union
	{
		// this union allows us to sort our
		// render parameters as 3 dword values
		struct
		{
			uint32 sortValueA;
			uint32 sortValueB;
			uint32 sortValueC;
		};

		struct
		{
			// The following members map to
			// sortValueA (first 32 bits)
			// (listed in reverse priority)

			// user-defined render parameter
			// packed together with the
			// model type and a detail
			// level indentifier (1 byte total)
			uint8 detailLevel	: 4;		
			uint8 modelType		: 2;		
			uint8 boneCount		: 2;		
			// which render pass to use
			uint8 renderPass;			
			// which effect file to use
			cPoolHandle hEffectFile;	

			// The following members map to
			// sortValueB (second 32 bits)
			// (listed in reverse priority)

			// secondary vertex buffer or model frame
			uint16 modelParamA;	
			// primary vertex buffer or model index
			cPoolHandle hModel;		

			// The following members map to
			// sortValueC (third 32 bits)
			// (listed in reverse priority)

			// the surface material used
			cPoolHandle hSurfaceMaterial;	
			// index buffer or model subset
			uint16 modelParamB;		
		};
	};

	// ADDITIONAL UNSORTED FIELDS (8 bytes)

	cSceneNode*	object;
	uint32	userData;

// we can go back to default packing now
#pragma pack()

	// these enum values are used to set
	// the modelType value above. This
	// tells the queue if the model
	// data represents a model resource 
	// or a set of vertex and index buffers
	enum eTypeFlags
	{
		k_bufferEntry = 0,	
		k_modelEntry,		
	};

	cRenderEntry(){};
	~cRenderEntry(){};


	// clear the entry to default values
	void clear()
	{
		sortValueA = 0;
		sortValueB = 0;
		sortValueC = 0;
	}
};

/*	cRenderQueue
-----------------------------------------------------------------
    
    The render queue is a collection of render entries which
	are submitted by the program, then sorted for more
	optimal rendering.
    
-----------------------------------------------------------------
*/

class cRenderQueue
{
public:

	friend cRenderEntry;

	enum eConstants
	{
		// 1k of render entries max (32k total mem)
		k_maxRenderEntries = 1024 
	};

    cRenderQueue();
    ~cRenderQueue(){};

	cRenderEntry* lockRenderEntry();
	void unlockRenderEntry(cRenderEntry* entry);

	void reset();
	void execute();

private:

	// Private Data...
	cRenderEntry m_entryPool[k_maxRenderEntries];
	cRenderEntry* m_entryList[k_maxRenderEntries];

	uint32	m_activeEntries;

	void sortEntryList();

    cRenderQueue(const cRenderQueue& Src);
    cRenderQueue& operator=(const cRenderQueue& Src);
};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cRenderQueue
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cRenderQueue::cRenderQueue()
:m_activeEntries(0)
{
	memset(m_entryList, 0, sizeof(m_entryList));
}

//- End of cRenderQueue -----------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cRenderQueue.h )

//----------------------------------------------------------
//$Log: $