#ifndef __displaylist_h__
#define __displaylist_h__

#include "cresource.h"
/**
 * A class to encapsulate a display list.<br>
 * A display list records a series of commands and allows them
 * to be played back again later. This optimizes display speed because
 * the commands are uploaded to the display card(server) and do 
 * not need to be sent every frame.
*/
class DisplayList : public CResource
{
public:
	/** Begin recording commands into the display list 
	 * @param execute A boolean specifying whether the commands
	 * show be executed at the same time they are compiled.
	 * @return None
	*/
	void record(bool execute);
	/** Stop recording and save the commands
	 * @return None
	 */
	void stop();
	/** Display the recorded commands 
	 * @return None
	 */
	void display();
protected:
	static DisplayList *Create() { return new DisplayList(); }
	friend class CResourceManager;

	DisplayList();
	virtual ~DisplayList(); 

	bool LoadResource()  		{ return true; } // Ignore this command
	void UnloadResource()   { ; } // Ignore this command as well

	virtual void Think() { ; }
private:
	int listID;

};	

#endif // __displaylist_h__
