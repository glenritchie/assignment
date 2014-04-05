#ifndef _cresource_h_
#define _cresource_h_


#ifdef _MSC_VER

// Disable warning 4786:

// "identifier was truncated to '255' characters in the debug information"

#pragma warning( disable : 4786)

#endif


#include <string>
#include "master_class.h"
class CResourceManager;
extern veryLong currentTime;
/** A resouce is an object that is used and usually has a limited lifespan in the program */
class CResource : public MasterClass
{
	// Needed so ResourceManager can create and instance
	friend class CResourceManager;		
public:
	/**
	 * Test if this resource is the same as another
	 * @param other The other resource
   */
	bool operator==(CResource *other)
	{
		return (this->resource_name == other->resource_name);
	}
	/**
	 * Test if this resource is less than another
	 * @param other The other resource
   */
	bool operator<(CResource *other)
	{
		return (this->resource_name < other->resource_name);
	}
	/**
	 * Test if this resource is greater than another
	 * @param other The other resource
   */
	bool operator>(CResource *other)
	{
		return (this->resource_name > other->resource_name);
	}

	/// Release one reference of resource
	int	Release() {						
		if (ref_count>0) 
			ref_count--;				// Keep reference count above 0
//		if (ref_count == 0)				// If we are all out of references
//			UnloadResource();			// Unload the resource
		return ref_count;				// Return the reference count
	}

	/** Is this resource currently in memory?
	 * @return Is this resource in memory
	 */
	bool			InMemory()			      { return in_memory;		}	
	/** Get the name of the resource. May or may not be a filename.
	 * @return A string holding the name of this resource
	 */
	std::string		GetResourceName()	{ return resource_name;	}
	/** Get the last time this resource was accessed
	 * @return The last time this resource was accessed 
	 */
	veryLong GetLastAccess()				{ return last_access;	}
	/** Get the number of times this resource has been referenced
	 * @return The number of times this resource has been referenced 
	 */
	long unsigned int GetRefCount()		{ return ref_count;		}
	/** Update the last access time to the current time. */
	void UpdateLastAccessTime()			{ last_access = currentTime; usageCount++; }
	/** Destroy this resource */
	virtual ~CResource() { ; }	
	/** Order this resource to be removed on the resource manager's next think cycle */
	void die() { remove = true; }	
	/** Convert this resource to a string */
	virtual std::string toString()
	{
		std::string final;
		char tmp[1000];
#ifdef sprintf_s
		sprintf_s(tmp, 1000,", in_memory: %d, name: %s, ref_count: %d, remove: %d",
			in_memory, resource_name.c_str(), ref_count, remove);
#elif snprintf
		snprintf(tmp, 1000, ", in_memory: %d, name: %s, ref_count: %d, remove: %d",
			in_memory, resource_name.c_str(), ref_count, remove);
#endif
		final.append(tmp);
		return final;
	}
protected:
	// static CResource *Create() { last_access = 0; in_memory = false;}

	/** Private construct and destruct so that the programmer CAN NOT 
	 * create or delete his own resources. He must go through the manager
	 * this let's the manager handle deletion and allocation. neat huh? :)
	 */
	CResource()	: last_access(0), ref_count(0), in_memory(false), remove(false), usageCount(0) { ; }

	/// Allow resource to think
	virtual void Think() = 0;						
	
	/// Loads resource (back?) into memory
	virtual bool LoadResource() = 0;				
	
	/// Unloads the resource from memory ( does not destroy ptr though )
	virtual void UnloadResource() = 0;				

	/// Is the resource still in memory?
	bool in_memory;									

	/* Get the number of times was this texture accessed during it's life 
	 * @return The number of times this resource was accessed.
	 */
	long getUsageCount() { return usageCount; }
private:
	/// The number of times this resource was accessed.
	long usageCount;
	/// Last access time
	veryLong last_access;							
	
	/// Name of resource
	std::string resource_name;						
	
	/// Number of times this resource has been referenced
	long unsigned int ref_count;					
	/// Force this resource to remove itself
	bool remove;
};
#include "resource_manager.h"

#endif // _cresource_h_
