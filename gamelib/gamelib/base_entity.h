#ifndef _BASE_ENTITY_
#define _BASE_ENTITY_

#include "utils.h"
#include "vector.h"
#include <string>
#include "master_class.h"

extern unsigned long entity_vertices;

/** The base entity class */
class CBaseEntity : public MasterClass
{
public:
		/** Constructor */
		CBaseEntity();
		/** Get a string representing this class 
		 *	@return A string that represents this class
		 */
		virtual std::string toString();

		/** Deconstructor that does nothing */
		virtual ~CBaseEntity();

		void init();				
		
		/** Object die function <br>
		 * Does nothing by default. Should be overridden if different functionality is required */
		virtual void die();

		/** Object think function 	- abstract */
		virtual void Think() = 0;    							
		
		/** Object pre-render function 	- abstract */
		virtual void PreRender() = 0;							
		
		/** Object render function 	- abstract */
		virtual void Render() = 0;								
		
		/** Object collide function 	- abstract 
		 *	@param other The other entity that hit this ent
		 */
		virtual void Collision(CBaseEntity *other) = 0;			
		
		/** Cleanup all resources - abstract */
		virtual void Cleanup() = 0;								
		
		/** Return bounding box of ent in OBJECT space ( NOT WORLD SPACE )
		  * @param min Minumum points
		  * @param max Maximum points
		  */
		virtual void getbbox(VECTOR3 &min, VECTOR3 &max) = 0;	
		//virtual bool TestCollide(CBaseEntity *other) = 0;

		/** A function to create an instance of this class 
		 * @return A CBaseEntity representing this class
		 */
		virtual CBaseEntity *create();
		/** Returns the class name of this class 
		  * @return A constant null terminated character string representing the class name
		  */
		const char* getClassname() const;
		
		/** Set the friendly name of this class
		  * @param newname The new name of this class
     	  */
		void setFriendlyName(std::string newname);
		/** Get the friendly name of this class
		  * @return A constant null terminated character string representing the class name
		  */
		const char* getFriendlyName();

		/** Set the script for this object
		  * @param newscript The filename of the script
		  */
		void setScript(std::string newscript);

		/** Get the filename of the script associated with this object
		  * @return The filename of the script
		  */
		const char* getScript();

		/** Set the class name of this class
		  * @param newclassname The new name of this class
		  */
		void setClassname(std::string newclassname);



	bool	solid;				/// Is this ent solid?
	VECTOR3 	origin;			/// Entity origin
	VECTOR3		angles; 		/// The eular angles


	VECTOR3 oldOrigin;
	VECTOR3 oldAngles;
		
	bool		thinking;

	VECTOR3		forward;
	VECTOR3		right;
	VECTOR3		up;
	float		camera_distance;				// Distance from camera

	bool		display;
	bool		has_alpha;						// Has an alpha component somewhere
	bool		remove;							// Request flag to remove item for Entity List
	bool		visible;
	float		health;
	float		total_health;
		
	/** Specifies that this object will be created/saved/loaded by another object and it should NOT be saved in the
	 * normal circumstances */
	bool changed;
	bool parentCreation;
private:
	std::string friendlyName;
	std::string Classname;
	std::string scriptName;
protected:
	virtual void subinit() = 0;				
	
};

#endif //_BASE_ENTITY_ 

