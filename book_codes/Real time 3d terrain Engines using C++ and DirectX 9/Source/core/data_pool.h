#ifndef DATAPOOL_H
#define DATAPOOL_H
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
#include "memory.h"

#ifndef STD_LIST_H
#define STD_LIST_H
#pragma warning(disable : 4786)
#include <list>
#endif

//	Name Space declaration
namespace gaia
{

typedef uint16 cPoolHandle;

#define INVALID_INDEX 0xffff
#define CLEAR_HANDLE(h) (h=INVALID_INDEX)
#define VALID_HANDLE(h) (h!=INVALID_INDEX)

template <class T>
class cPoolGroup
{
public:

	cPoolGroup(uint16 maxCount);
	~cPoolGroup(){};
	
	void		create();
	void		destroy();

	uint16		addMember(const T& member);
	uint16		nextMember();
	void		release(uint16 index);

	uint16		totalOpen()const;
	uint16		totalUsed()const;
	uint16		firstOpen()const;
	bool		isOpen(uint16 index)const;
	T&			member(uint16 index);
	const T&	member(uint16 index)const;
	T*			memberPtr(uint16 index);
	const T*	memberPtr(uint16 index)const;

private:
	uint16			m_totalOpen;
	uint16			m_firstOpen;
	uint16			m_maxCount;
	uint16*			m_nextOpenList;
	T*				m_memberList;
};

//
// Member Group Functions
//
template <class T>
inline cPoolGroup<T>::cPoolGroup(uint16 maxCount)
:m_totalOpen(0)
,m_firstOpen(0)
,m_maxCount(maxCount)
,m_nextOpenList(0)
,m_memberList(0)
{
}



template <class T>
inline void cPoolGroup<T>::create()
{
	debug_assert(!m_memberList && !m_nextOpenList, "Group has already been created");

	m_nextOpenList = (uint16*)alloc_aligned_memory(sizeof(uint16)*m_maxCount, memory::ALIGN_32);//new(memory::ALIGN_32) int16[m_maxCount];
	debug_assert(m_nextOpenList, "tragic memory allocation failure!");

	m_memberList = new T[m_maxCount];
	debug_assert(m_memberList, "tragic memory allocation failure!");

	int i;
	for (i=0; i<m_maxCount-1; ++i)
	{
		m_nextOpenList[i] = i+1;
	}
	m_nextOpenList[i] = i;

	m_totalOpen = m_maxCount;
	m_firstOpen = 0;
}

template <class T>
inline void cPoolGroup<T>::destroy()
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(m_totalOpen == m_maxCount, "Group is not empty");

	delete[](m_memberList);
	free_memory(m_nextOpenList);

	m_totalOpen = 0;
	m_firstOpen = 0;
}


template <class T>
inline uint16 cPoolGroup<T>::nextMember()
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(m_totalOpen, "no open slots");

	// return the first member of our
	// open list, and move our internal
	// handle to the next member
	uint16 slot = m_firstOpen;
	m_firstOpen = m_nextOpenList[slot];
	--m_totalOpen;

	debug_assert(m_firstOpen != INVALID_INDEX, 
		"Invalid Open Index");

	debug_assert(isOpen(slot), "invalid index");

	// signal this member as being is use
	m_nextOpenList[slot] = INVALID_INDEX;

	return(slot);
}

template <class T>
inline uint16 cPoolGroup<T>::addMember(const T& member)
{
	uint16 slot = nextMember();
	m_memberList[slot] = member;

	return(slot);
}

template <class T>
inline void cPoolGroup<T>::release(uint16 index)
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	debug_assert(!isOpen(index), "invalid index to release");

	m_nextOpenList[index] = m_totalOpen ? m_firstOpen:index;
	++m_totalOpen;
	m_firstOpen = index;
}


template <class T>
inline uint16 cPoolGroup<T>::totalOpen()const
{
	return m_totalOpen;
}

template <class T>
inline uint16 cPoolGroup<T>::totalUsed()const
{
	return m_maxCount-m_totalOpen;
}

template <class T>
inline uint16 cPoolGroup<T>::firstOpen()const
{
	return m_firstOpen;
}

template <class T>
inline bool cPoolGroup<T>::isOpen(uint16 index)const
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	return m_nextOpenList[index] != INVALID_INDEX;
}

template <class T>
inline T& cPoolGroup<T>::member(uint16 index)
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	return m_memberList[index];
}

template <class T>
inline const T& cPoolGroup<T>::member(uint16 index)const
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	return m_memberList[index];
}

template <class T>
inline T* cPoolGroup<T>::memberPtr(uint16 index)
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	return &m_memberList[index];
}

template <class T>
inline const T* cPoolGroup<T>::memberPtr(uint16 index)const
{
	debug_assert(m_memberList && m_nextOpenList, "Group has not been created");
	debug_assert(index < m_maxCount, "invalid index");

	return &m_memberList[index];
}


/*	cDataPool
-----------------------------------------------------------------
    
    cDataPool is a template class used to store a fixed amount of
	type T. The cDataPool must be created with a count of members
	to hold so it can innitialize an array to store the data 
	internally. Data must be added using the add() and remove()
	functions. This allows the class to track unused slots.

	The class assumes a max size of MAX_INT16, so the returned
	handles can be 16 bits in size. Also, type T must be at least
	as large as the index type (uint16) for this class to function
    
-----------------------------------------------------------------
*/
class cDataPoolInterface
{
public:
	typedef void (*MEMBER_CALLBACK)(cDataPoolInterface* pPool, cPoolHandle handle, void* member);

	cDataPoolInterface():m_initialized(false){};
	virtual ~cDataPoolInterface(){};

	virtual void initialize(uint16 growSize)=0;;
	virtual void destroy()=0;;
	virtual void clear()=0;
	virtual cPoolHandle nextHandle()=0;
	virtual void release(cPoolHandle* pHandle)=0;
	virtual void forEach(MEMBER_CALLBACK function)=0;

	virtual void* getGenericPtr(cPoolHandle index)=0;

	bool isInitialized()const {return m_initialized;}

protected:
	bool m_initialized;
};

template <class T>
class cDataPool : public cDataPoolInterface
{
public:

	// Data Types & Constants...
	typedef T DATA_TYPE;
	typedef cPoolGroup<T> MemberGroup;
	typedef std::list<MemberGroup*> MemberGroupList;
	typedef cDataPoolInterface::MEMBER_CALLBACK MEMBER_CALLBACK;

	// Creators...
    cDataPool();
    ~cDataPool();

	void initialize(uint16 growSize);
	void destroy();

	const T& operator[](cPoolHandle handle)const {return get(handle);}
	T& operator[](cPoolHandle handle) {return get(handle);}


	// Mutators...
	cPoolHandle add(const T& member);
	void clear();
	cPoolHandle nextHandle();
	void release(cPoolHandle* pHandle);
	void forEach(MEMBER_CALLBACK function);


	// Accessors...
	bool isHandleValid(cPoolHandle index)const;
	const T& get(cPoolHandle index)const;
	T* getPtr(cPoolHandle index);
	void* getGenericPtr(cPoolHandle index);

	uint16 totalMembers()const {return m_totalMembers;}
	uint16 totalOpen()const {return m_totalOpen;}
	uint16 totalUsed()const {return m_totalMembers-m_totalOpen;}

private:

	// Private Data...
	MemberGroupList m_groupList;
	uint16			m_totalMembers;
	uint16			m_totalOpen;
	uint16			m_groupCount;
	uint32			m_indexMask;
	int				m_indexShift;

	// Private Functions...
    explicit cDataPool(const cDataPool& Src);
    cDataPool& operator=(const cDataPool& Src);

	cPoolGroup<T>*	addGroup();
	cPoolGroup<T>*	findOpenGroup(unsigned* groupNumber);
	cPoolGroup<T>*	getGroup(unsigned index);
	const cPoolGroup<T>*	getGroup(unsigned index)const;

	int getGroupNumber(cPoolHandle handle)const;
	int getItemIndex(cPoolHandle handle)const;
	cPoolHandle buildHandle(int group, int index)const;


};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cDataPool
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
template <class T>
inline cDataPool<T>::cDataPool()
:m_totalMembers(0)
,m_totalOpen(0)
{
	m_groupList.clear();
}

/*	~cDataPool
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
template <class T>
inline cDataPool<T>::~cDataPool()
{
	debug_assert(!isInitialized(), "the cDataPool has not been properly destroyed");
}

//. Generic Interface Methods ...................................
template <class T>
inline void cDataPool<T>::initialize(uint16 growSize)
{
	debug_assert(!isInitialized(), "the cDataPool is already initialized");
	m_initialized = true;

	m_groupCount = nearestPowerOfTwo(growSize); 
	m_indexShift = lowestBitSet(m_groupCount); 
	m_indexShift = clamp(m_indexShift, 1, 15);
	m_groupCount = 1 << m_indexShift;
	m_indexMask = m_groupCount-1;

}

template <class T>
inline void cDataPool<T>::destroy()
{
	clear();
	m_initialized = false;
}

template <class T>
inline int cDataPool<T>::getGroupNumber(cPoolHandle handle)const
{
	return handle>>m_indexShift;
}

template <class T>
inline int cDataPool<T>::getItemIndex(cPoolHandle handle)const
{
	return handle & m_indexMask;
}

template <class T>
inline cPoolHandle cDataPool<T>::buildHandle(int group, int index)const
{
	return (group<<m_indexShift) + index;
}


template <class T>
inline void cDataPool<T>::forEach(MEMBER_CALLBACK function)
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	// iterate through every group
	int	groupNumber = 0;
	for(MemberGroupList::iterator groupIter = m_groupList.begin();
		groupIter!=m_groupList.end();
		++groupIter)
	{
		// check all indexes in this group
		cPoolGroup<T>* pGroup = *groupIter;

		int callbackCount = pGroup->totalUsed();
		uint16	itemIndex = 0;
		while(callbackCount && itemIndex<m_groupCount)
		{
			// is the member in use?
			if (!pGroup->isOpen(itemIndex))
			{
				// issue the callback
				function(this, buildHandle(groupNumber,itemIndex), (void*)&(pGroup->member(itemIndex)));
				--callbackCount;
			}
			++itemIndex;
		}
		++groupNumber;
	}
}

//. Mutators ....................................................

template <class T>
inline void cDataPool<T>::clear()
{
	// destroy all groups in the list
	for (MemberGroupList::iterator iter = m_groupList.begin(); 
			iter != m_groupList.end(); 
			++iter)
	{
		cPoolGroup<T>* pGroup = *iter;
		pGroup->destroy();
		delete pGroup;
	}

	// now clear the list itself
	m_groupList.clear();
}

template <class T>
inline cPoolGroup<T>* cDataPool<T>::addGroup()
{
	// append a new group to the list to start things off
	cPoolGroup<T>* pNewGroup = new cPoolGroup<T>(m_groupCount);
	m_groupList.insert(m_groupList.end(), pNewGroup);

	// gain access to the new group and innitialize it
	cPoolGroup<T>* pGroup = m_groupList.back();
	pGroup->create();

	// increment our internal counters
	m_totalMembers += m_groupCount;
	m_totalOpen += m_groupCount;

	// return the new group pointer
	return(pGroup);
}

template <class T>
inline cPoolGroup<T>* cDataPool<T>::findOpenGroup(unsigned* groupNumber)
{
	// find and return the first group with an open slot
	*groupNumber = 0;
	for (MemberGroupList::iterator iter = 
			m_groupList.begin(); 
			iter != m_groupList.end(); 
			++iter)
	{
		cPoolGroup<T>* pGroup = *iter;
		if (pGroup->totalOpen())
		{
			// an open slot has been found
			return(pGroup);
		}
		++(*groupNumber);
	}

	// there are no open slots, 
	// so we need to add a new cPoolGroup
	// before we make a new group, make sure 
	// we have not reached our max 
	// of MAX_UINT16 members
	debug_assert(m_groupList.size()*(m_groupCount+1) 
				< (uint16)MAX_UINT16, 
				"the cDataPool is full!!!!");

	// create the new group
	return(addGroup());
}

template <class T>
inline cPoolGroup<T>* cDataPool<T>::getGroup(unsigned index)
{
	debug_assert(index < m_groupList.size(), "Invalid group index requested");

	// find and return the desired group
	MemberGroupList::iterator iter = m_groupList.begin();
	for(;index;--index)
	{
		++iter;
	}

	return(*iter);
}

template <class T>
inline const cPoolGroup<T>* cDataPool<T>::getGroup(unsigned index)const
{
	debug_assert(index < m_groupList.size(), "Invalid group index requested");

	// find and return the desired group
	MemberGroupList::const_iterator iter = m_groupList.begin();
	for(;index;--index)
	{
		++iter;
	}

	return(*iter);
}

template <class T>
inline cPoolHandle cDataPool<T>::add(const T& member)
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	unsigned groupNumber=0;
	cPoolGroup<T>* openGroup = findOpenGroup(&groupNumber);

	int index = openGroup->addMember(member);

	--m_totalOpen;

	return buildHandle(groupNumber, index);
}

template <class T>
inline cPoolHandle cDataPool<T>::nextHandle()
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	// find or create a group with an available slot
	unsigned groupNumber=0;
	cPoolGroup<T>* openGroup = findOpenGroup(&groupNumber);

	// find the first open slot within the group
	int index = openGroup->nextMember();
	--m_totalOpen;

	// build a handle to return
	return buildHandle(groupNumber, index);
}

template <class T>
inline void cDataPool<T>::release(cPoolHandle* pHandle)
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");
	debug_assert(pHandle, "A valid handle must be provided");

	if (isHandleValid(*pHandle))
	{
		debug_assert(m_groupList.size(), 
			"The cDataPool has not been properly created");

		// dissect the handle into it's
		// group and index values
		int groupIndex = getGroupNumber(*pHandle);
		int itemIndex = getItemIndex(*pHandle);

		cPoolGroup<T>* group = getGroup(groupIndex);

		// tell the group to remove the member
		group->release(itemIndex);

		// clear the caller's handle
		CLEAR_HANDLE(*pHandle);

		// try and see if we can remove the last group
		cPoolGroup<T>* pGroup=m_groupList.back();
		if (pGroup->totalOpen() == m_groupCount)
		{
			pGroup->destroy();
			delete pGroup;
			m_groupList.pop_back();
		}

		++m_totalOpen;
	}
}


// Accessors...
template <class T>
inline bool cDataPool<T>::isHandleValid(cPoolHandle handle)const
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	if (VALID_HANDLE(handle))
	{
		debug_assert(m_groupList.size(), "The cDataPool has not been properly created");
	
		const cPoolGroup<T>* group = getGroup(getGroupNumber(handle));

		// make sure the magic numbers match
		return(!group->isOpen(getItemIndex(handle)));
		
	}
	return false;
}

template <class T>
inline const T& cDataPool<T>::get(cPoolHandle handle)const
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	debug_assert(m_groupList.size(), "The cDataPool has not been properly created");

	const cPoolGroup<T>* group = getGroup(getGroupNumber(handle));

	int itemIndex = getItemIndex(handle);

	// tell the group to return the member
	return group->member(itemIndex);
}

template <class T>
inline T* cDataPool<T>::getPtr(cPoolHandle handle)
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	debug_assert(m_groupList.size(), "The cDataPool has not been properly created");

	int groupNumber = getGroupNumber(handle);
	cPoolGroup<T>* group = getGroup(groupNumber);

	int itemIndex = getItemIndex(handle);

	// tell the group to return the member
	return group->memberPtr(itemIndex);
}

template <class T>
inline void* cDataPool<T>::getGenericPtr(cPoolHandle index)
{
	debug_assert(isInitialized(), "the cDataPool is not initialized");

	return((void*)getPtr(index));
}

//. Accessors ...................................................

//- End of cDataPool ----------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cDataPool.h )

//----------------------------------------------------------
//$Log: $