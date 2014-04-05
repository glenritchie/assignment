#ifndef __lightobject_h__
#define __lightobject_h__

#include "object.h"
#include "light.h"

class LightObject : public Object
{
public:
		LightObject(string newName);
		LightObject(Object *o);
		~LightObject();
		void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
protected:
		void shapeDraw();
private:
		Light *l;
};

#endif //__lightobject_h__

