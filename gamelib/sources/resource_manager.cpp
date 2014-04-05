// Disable warning 4786:
// "identifier was truncated to '255' characters in the debug information"
#pragma warning( disable : 4786)
#include "utils.h"
#include "resource_manager.h"
#include "cresource.h"
#include "log.h"
#include "common.h"
#include <fstream>
#include <time.h>
using namespace std;

extern veryLong currentTime;

// Declarations of static stuff
CResourceManager *CResourceManager::resourceManager = NULL;
const long unsigned int CResourceManager::MAX_RESOURCE_LIFE = 30*CLOCKS_PER_SEC; // 30 seconds
 

CResourceManager::CResourceManager()
{
 	strcpy(resourceUsageLog, "log/resource_usage.txt"); 

}
// Resource manager functions
void CResourceManager::preCache()
{
	ifstream rfile;	

	rfile.open(resourceUsageLog);
	if (rfile.fail())
	{
		DebugText(EMSG, "Unable to begin precache, the precache log dosn't exist");
		return;
	}

	//std::string line;
	char line[500];
	char msg[200];
	
	rfile.seekg( 0, ios_base::end );
	long fileSize = rfile.tellg();
	rfile.seekg( 0, ios_base::beg );


	// Skip the first two lines
//	rfile.getline(line, 500);
//	rfile.getline(line, 500);

	DebugText(EMSG, ">>>>>BEGIN PRECACHE<<<<<");
	long count;
//	extern std::string subActivity;
//	extern float subPercentageDone;
//	subPercentageDone = 0;
	while (!rfile.eof() || !rfile.fail())
	{
		unsigned int ptr = rfile.tellg();
		float fptr = (float)ptr;
		float fsize = (float)fileSize;
		if (ptr > fileSize)
			break;
	//	subPercentageDone = (fptr/fsize) * 100.0f;



		fill(line, &line[499], 0);
		rfile.getline(line, 500);
		if (strlen(line) <= 3)
			break;
		sscanf(line, "%s %ld", msg, &count);
//		subActivity = std::string(msg);
		// Find the resource associated with this item
		
		TResourceList::iterator iter;
		for (iter = ResourceList.begin(); iter != ResourceList.end(); iter++)
			if ((*iter)->GetResourceName() == std::string(msg))
				break;
		if (iter == ResourceList.end())
		{
			// Item dosn't exist
			DebugText(EMSG, "Can't precache resource: %s", msg);
		}
		else
		{
			//if (!(*iter)->InMemory())
			(*iter)->UnloadResource();
			(*iter)->LoadResource();
		}
	}
	DebugText(EMSG, ">>>>>END PRECACHE<<<<<");
}

void CResourceManager::add2UsageMap(std::string name)
{
	usageMap.insert(std::pair<std::string, long>(name, 0));
}
void CResourceManager::Think()
{
	// Only used when wanting to show resources on the screen
	int y = 200;
	int x = 10;
	
	// A list to hold resources to be deleted
	// We don't immediatly remove items from the vector/list because
	// the current iterator becomes invalidated by an erase() call
	// so we store items to be deleted on this temp list and
	// iterate through this list to delete the items
	TResourceList ResourceListtemp;
	// Just a iterator to hold the current location in the vector
	TResourceList::iterator iter;
	
	// Go through each item in the list
	for (iter = ResourceList.begin(); iter != ResourceList.end(); iter++)
	{
		// Get a ptr to the CResource item in the list
		// by dereferencing the iterator ( don't u love templates )
		CResource *item = *iter;
		
		veryLong lastAccess = item->GetLastAccess(); 													// Get the time that resource was accessed last
		veryLong dieTime = (lastAccess + MAX_RESOURCE_LIFE); 									// Get the time the resource is to die
		veryLong time_2_live = dieTime - currentTime;													// Get the resources time left on this earth
		bool exceededLife = (currentTime > (lastAccess + MAX_RESOURCE_LIFE));	// Set to true if the resource has exceeded it's life
		
		// If the item is in memory and we want to see the 
		// resources then show em!
		extern bool show_resources;
		if (show_resources && item->InMemory())
		{
			// Just a debug function to show how the resources are working
			Draw2DText(x, y, COLOR_RGB(255,255,255),"%s: inMem: %d, dieTime: %u, pastLife: %d, ttl: %d, ref: %d",
				item->resource_name.c_str(), item->in_memory, 
				(unsigned int)dieTime,
				exceededLife,
				time_2_live,
				item->ref_count);

			y+=12;
		}
 
		// If the resource has no time left on this earth
		if (exceededLife)
		{
			if (item->remove)				// If this resource has been asked to be removed from the list
				item->Release();			// Release one instance of it
	
			// If an item has no references.. let's get rid of it
			if (item->GetRefCount() <= 0)
			{
				// Add them to the map
				long usageCount = item->getUsageCount();
				std::string name = item->GetResourceName();
				// See if this item already exists in the map
				TUsageResourceMap::iterator i = usageMap.find(name);
				if (i == usageMap.end())
				{
					// Item dosn't exist!! BAD!!
					DebugText(ERROR_NONCRITICAL, "Warning. Resource of name '%s' dosn't exist in the usage map!", name.c_str());
				}
				// Add the usage count to that element
				//usageCount += i->second;
				//usageMap.erase(name);
				//usageMap.insert(std::pair<std::string, long>(name, usageCount));
				i->second += usageCount;

				item->UnloadResource();			// Unload the resource
				//ResourceManager->ResourceList.erase(iter);
				//iter = ResourceList.begin();	// RESET iterator to start. Otherwise errors!
				ResourceListtemp.push_back(item);
			}
		}
		
		// If the current time has gone past what the resource was supposed to live to and it's currently in memory
		// then unload it
		if (exceededLife && (item->InMemory() == true))
			item->UnloadResource();
	
		// Allow the Resource to think.. so animated textures to move etc
		item->Think();				
	}

	// Take items for the temporary resource list which are a list of items to remove
	// and remove them for the real list.
	TResourceList::iterator iter2;
	for (iter = ResourceListtemp.begin(); iter != ResourceListtemp.end(); iter++)
	{
		CResource *item=*iter;
		for (iter2 = ResourceList.begin(); iter2 != ResourceList.end(); iter2++)
		{
			CResource *item2=*iter2;
			if (item2 == item)
			{
				ResourceList.erase(iter2);
				break;
			}
		}
		delete item;
	}
}

void CResourceManager::Cleanup()
{
	DebugText(EMSG, "CResourceManager::Cleanup():: Garbage collecting left over resources....");
	char msg[200];
	sprintf(msg, "|%-40s | %-10s|", "Resource Name", "Count");
	
	DebugText(EMSG, "Showing resource reference counts...");
	DebugText(EMSG, msg);
	fill(msg, &msg[strlen(msg)], '-');
	DebugText(EMSG, msg);
	TResourceList::iterator iter;
	int count = 0;
	for (iter = ResourceList.begin(); iter != ResourceList.end(); iter++)
	{
		CResource *item = (*iter);
		if (item->GetRefCount() != 0)
			DebugText(EMSG, "|%-40s | %9d |", item->GetResourceName().c_str(), item->GetRefCount());
		// Ensure all resources are fully released.
		while (item->GetRefCount() != 0)
		{
			item->Release();
		}
		item->UnloadResource();			// Unload the resource

		// Add them to the map
		long usageCount = item->getUsageCount();
		std::string name = item->GetResourceName();
		// See if this item already exists in the map
		TUsageResourceMap::iterator i = usageMap.find(name);
		if (i == usageMap.end())
		{
			// Item dosn't exist!! BAD!!
			DebugText(ERROR_NONCRITICAL, "Warning. Resource of name '%s' dosn't exist in the usage map!", name.c_str());
		}
		// Add the usage count to that element
		//usageCount += i->second;
		//usageMap.erase(name);
		//usageMap.insert(std::pair<std::string, long>(name, usageCount));
		i->second += usageCount;

		ResourceList.erase(iter);
		delete item;
		iter = ResourceList.begin();
	}
	
	//CLog resourceUsageLog("log\\resource_usage.txt");
	std::ofstream rfile("log/resource_usage.txt");
	TUsageResourceMap::iterator i;
	for (i = usageMap.begin(); i != usageMap.end(); i++)
	{
		std::string name = i->first;
		long count = i->second;
		char msg[100];
		sprintf(msg, "%s %ld\n", name.c_str(), count);
		//resourceUsageLog.Output(msg);
		rfile << msg;
	
	}

	
	
	DebugText(EMSG, msg);
	DebugText(EMSG, "Done garbage collection...");
}

void CResourceManager::setLowerCaseName(CResource *r, const char *filename)	
{
	// Get the resource file. Note that this dosn't need to be an actual file
	std::string tex_name;
		
	char file[266];
	strcpy(file, filename);
	// Convert string to lower case
	for (unsigned int a = 0;a<strlen(file);a++)
	{
		file[a] = tolower(file[a]);
	}

	tex_name.assign(file);

	r->resource_name = tex_name;		// Set resource to new name
}
void CResourceManager::add(CResource *r)
{
	r->ref_count = 1;				// Set initial reference type
	ResourceList.push_back(r);	// Add to list
	add2UsageMap(r->resource_name);
}

CResource *CResourceManager::findResource(std::string &name)
{
	TResourceList::iterator iter;
	// Iterate through the list of stuff we already have. If it's already
	// been loaded then don't create a new one, just add a new reference to it
	// and return it's pointer, otherwise create a new resource.
	for (iter = ResourceList.begin(); iter != ResourceList.end(); iter++)
	{
		CResource *item = *iter;

		// Found item by the same name. return the ptr
		if (item->resource_name == name)
			return  item;
	}
	return NULL;
}
void CResourceManager::invalidateAllResources()
{
	TResourceList::iterator iter;
	for (iter = ResourceList.begin(); iter != ResourceList.end(); iter++)
	{
		CResource *item = (*iter);
		item->UnloadResource();
	}
}

void CResourceManager::updatetlist()
{
}
