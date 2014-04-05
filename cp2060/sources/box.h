#ifndef __box_h__
#define __box_h__
#include "object.h"

class BoxObject : public Object
{
public:
	BoxObject(string newName, float width, float depth, float height);
	BoxObject(Object *o);
	void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);

	
//	void generateLightMap(VECTOR3 &light);
	void generateLightMaps(VECTOR3 lights[], unsigned int lightCount);
private:
	Object *subRayCollide(VECTOR3 point, VECTOR3 target, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal);
	Object* subSphereCollide( const VECTOR3 &center, const float &radius, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal);
//	void transformChanged();
	void shapeDraw();
private:
	void genFaces();
	float depth, width, height;
	float density;

	VECTOR3 vPoly1[6][3];
	VECTOR3 vPoly2[6][3];
	double planes[6][4];
	VECTOR3 norm[6];
	bool doneNorms[6];
	double radius;	
	float hx, hy, hz;
//	VECTOR3 lightPoints[100];
//	int savedPoints;
//	float result[100];

	// VECTOR3 norm[6];
	bool doPoly;

	float faceTexScale[6][2];
	float faceTexShift[6][3];
	unsigned char *imageData[6];
	unsigned int tid[6];
};


#endif // __box_h__
