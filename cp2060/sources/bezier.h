#ifndef __bezier_h__
#define __bezier_h__
#include "object.h"
#include <GL/glu.h>
// Include textures
#include "ctexture.h"

const int DEGREE = 8;
class BezierObject : public Object
{
public:
	BezierObject(string newName); 
	BezierObject(Object *o); 
	~BezierObject();
	void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
protected:
	void shapeDraw();
private:
	void readControlPath(ifstream &file, unsigned int index, unsigned int &lineNumber, unsigned int &level);
	float controlPoints[50][DEGREE][3];
	float uKnots[20];
	float vKnots[20];
	unsigned int uKnotCount;
	unsigned int vKnotCount;
	unsigned int numControlPaths;
	int density;
	GLUnurbsObj *surfName;
};

#endif // __bezier_h__
