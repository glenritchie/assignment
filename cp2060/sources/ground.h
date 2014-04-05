#ifndef __ground_h__
#define __ground_h__

#include "object.h"
#include "displaylist.h"
//#include "ctexture.h"

class GroundObject : public Object
{
public:
		GroundObject(string newName, float newWidth, float newHeight, 
								 float newDensity);
		GroundObject(Object *o);

		void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
		float getHeight(int x, int y);
protected:
		void shapeDraw();
private:
		VECTOR3 getnorm(int x, int y);	
		void create();

		float width;
		float height;
		float density;

		DisplayList *groundDisplayList;
		int hmap[100][100];
		VECTOR3 hnormals[100][100];
//		CTexture *groundTexture;
//		string textureName;
};

#endif //__ground_h__

