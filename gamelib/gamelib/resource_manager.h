#ifndef _Resource_manager_h_
#define _Resource_manager_h_
#include <string>
#include <list>
#include <map>

#include <stdlib.h>
#include <string.h>

#include "cresource.h"


// TODO: Use own memory manager instead of default heap
/** 
 * Handles creation, deletion and loading of resources<br> 
 * To use.. call CResourceManager.getInstance().createResource<br>
 * <br>
 * NOTE: This class is a singleton!
 */
class CResourceManager
{
private:
	/// Make a typedef to say the Resource list
	typedef std::list<CResource *> TResourceList;
	/// Make a typedef to say the Resource list
	typedef std::list<CResource *> TResourceTypeList;
	/// Make a typedef
	typedef std::map<std::string, long> TUsageResourceMap;
	
	
	/** Static resource manager instance */
	static CResourceManager *resourceManager;
	
	/**
   * The maximum time a resource can live for
	 */
	static const long unsigned int MAX_RESOURCE_LIFE;
	
	/**
   * The file used for keeping a list of resources to pre-cache
	 */
	char resourceUsageLog[255];
public:
	/**
  *
	*/
	static CResourceManager& getInstance() 
	{
		if (resourceManager == NULL)
			resourceManager = new CResourceManager();
		return *resourceManager;	
	}
	
	void Think();
	void Cleanup();
	
	/**
	 * After the resource manager is destroyed using the Cleanup function a file 
	 * is created thatc contains a list of resources that were created during it's existance.
	 * This function will load in these resources before they are needed.. so that the
	 * objects don't interrupt the program.
	*/
	void preCache();

	/** If i use the folloiwng declaration the 'Stupid' friggin Visual C++ WITH Service pack 5 wont let me reference it! Arrggh!!!
		template <typename T>	
		void CreateResource(CResource *tptr, const char *filename);
		If i try this---
		ResourceManager->CreateResource<CAnimatedResource>(ptr,"anim.tga");
		it freaks out even though the ANSI C++ standard is supposed to let me!
		AAAAHHH! Stupid non-conformant VC++ 6.0!!!!!!!!!!!!!!!!
		So i have to use this stupid way!
		23/4/03 - Extra note:
		It turns out this way is actually better.. I just pass a pointer and 
		the compiler figures it out for me
	 	@param tptr A pointer to a class to put the created instance in
	 	@param filename The filename of the resource to load
	 */
	template <typename T>
		void createResource(T *(&tptr), const char *filename)
	{
		// Create a new resource using the resources own static create function
		// This means we don't care WHAT resource it is or HOW it works, just so
		// it creates it! Cool huh? i think they call this a factory pattern in C++ OOP
		T *newResource = T::Create();

		// If failed for any reason then quit.. Should start using throw
		if (!newResource)
			return;

		setLowerCaseName(newResource, filename);
	
		// Iterate through the list of stuff we already have. If it's already
		// been loaded then don't create a new one, just add a new reference to it
		// and return it's pointer, otherwise create a new resource.
		CResource *oldResource = findResource(newResource->resource_name);
		if (oldResource != NULL)
		{
			delete newResource;											// Delete any temp stuff we had
			oldResource->ref_count++;							// Add reference count
			tptr = dynamic_cast<T*>(oldResource);		// Dynamically cast into correct type
			return;						
		}
		
		// Not found. Add new
		add(newResource);
		tptr = newResource;						// Set memory
	}
	/** Cause all resources to unload and be forceably reloaded from
	 file. This is useful if certain settings have changed ( eg. graphics settings )
	 and you wish these changes to affect the current resources.
	 */
	void invalidateAllResources();	
private:	
	/** Constructor - Private coz we NEVER want a programmer to create one */
	CResourceManager() ;
	
	/** Get the resource list
	 * @return Get the list of resources
	 */
	TResourceList getRList() { return ResourceList; }

	/** Find a resource with the given name
	 * @param name The LOWERCASE name of the resource to find
	 * @return The resource
	 */
	CResource *findResource(std::string &name); 
	/**
	 * Add a new resource for me to manage
	 * @param r The resource to manage
	 */
	void add(CResource *r);
	/** Set the resources' name to the lowercase version of filename
	 * @param r The resource
	 * @param filename The name to set to lowercase
	 */
	void setLowerCaseName(CResource *r, const char *filename);
	

	void add2UsageMap(std::string name);
	void updatetlist() ;
	CResource *LoadResource(const char *filename) ;

	TResourceTypeList ResourceTypeList;
	TResourceList ResourceList;
	TUsageResourceMap usageMap;
};

//extern CResourceManager *ResourceManager;

#endif // _Resource_manager_h_
