#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#pragma once
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    $Workfile: $

    sample code from the book...
    3D Terrain Engines From the Ground Up.

    Created 10/11/2002 by Greg Snook
    greg@mightystudios.com

    ------------------------------------------------------------ 
    Last Revision:
    $Author: $
    $Date: $
    $Revision: $

\***************************************************************/
#include "data_types.h"
#include "singleton.h"
#include "resource.h"

#ifndef STD_MAP_H
#define STD_MAP_H
#pragma warning(disable : 4786)
#include <map>
#endif

#ifndef STD_LIST_H
#define STD_LIST_H
#pragma warning(disable : 4786)
#include <list>
#endif

//	Name Space declaration
namespace gaia
{

enum RESOURCE_FAMILY
{
	k_nVideoResource=0,
	k_nAudioResource,
	k_nGameResource,
	k_nTotalResourceFamilies
};

enum VIDEO_RESOURCES
{
	k_nTextureResource=0,
	k_nVBufferResource,
	k_nIBufferResource,
	k_nRenderResource,
	k_nModelResource,
	k_nImageResource,
	k_nTotalVideoResources
};

/*	cResourceManager
-----------------------------------------------------------------
    
    Add_Description_Here
    
-----------------------------------------------------------------
*/
class cResourcePoolInterface;
#define ResourceManager cResourceManager::instance()

class cResourceManager : public cSingleton<cResourceManager>
{
public:

	typedef std::map<cResourceCode, cResourcePoolInterface*> ResourcePoolTypeMap;
	typedef std::list<cResourcePoolInterface*> ResourcePoolFamilyList;

	// Creators...
    cResourceManager();
    ~cResourceManager(){};

	// registration of resource manager interfaces
	void registerResourcePool(cResourceCode code, cResourcePoolInterface* pInterface);
	cResourcePoolInterface*  unregisterResourcePool(cResourceCode code);

	// operations for all resource types
	void destroyAll();
	void disableAll();
	void restoreAll();
	void clean(); // delete items no longer referenced

	// operations on specific resource types
	void destroyResourceFamily(int family);
	void disableResourceFamily(int family);
	void restoreResourceFamily(int family);
	void cleanResourceFamily(int family); 

	void destroyResourceType(cResourceCode code);
	void disableResourceType(cResourceCode code);
	void restoreResourceType(cResourceCode code);
	void cleanResourceType(cResourceCode code); 

	cResourcePoolInterface* findResourcePool(cResourceCode code)const;
	cPoolHandle findResourceHandle(cResourceCode code, const cString& Name)const;
	cResource* findResource(cResourceCode code, const cString& Name)const;
	cResource* findResource(cResourceCode code, cPoolHandle handle)const;

private:

	ResourcePoolFamilyList m_resourceFamilyList[k_nTotalResourceFamilies];
	ResourcePoolTypeMap	m_resourceTypeMap;
};

//- End of cResourceManager --------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( ResourceSystem.h )

//----------------------------------------------------------
//$Log: $