#ifndef __sphere_h__
#define __sphere_h__
#include "object.h"

class SphereObject : public Object
{
public:
	SphereObject(string newName, float newRadius); 
	SphereObject(Object *o)  : Object(o) { ; }
	void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);

	/** Get the radius of this object 
	 * @param The radius of this object
	*/
	float getRadius() { return radius; }
protected:
	void shapeDraw();
private:
	float radius;
};


#endif // __sphere_h__
