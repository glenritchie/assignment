#ifndef __spotlightobject_h__
#define __spotlightobject_h__

#include "object.h"
#include "light.h"

class SpotLightObject : public Object
{
public:
		SpotLightObject(string newName);
		SpotLightObject(Object *o);
		~SpotLightObject();
		void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
protected:
		void shapeDraw();
private:
		SpotLight *l;
};

#endif //__spotlightobject_h__

