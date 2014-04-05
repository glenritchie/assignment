#ifndef __cylinder_h__
#define __cylinder_h__
#include "object.h"

class CylinderObject : public Object
{
public:
	CylinderObject(string newName, float newBase, float newTop, float newHeight);
	CylinderObject(Object *o);
	void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
protected:
	void shapeDraw();
private:
	int stacks;
	int slices;
	
	float baseRadius;
	float topRadius;
	float height;
};

#endif // __cylinder_h_

