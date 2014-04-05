#include "cylinder.h"
#include "model.h"
#include <stdlib.h>
#include "utils.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

CylinderObject::CylinderObject(string newName, float newBase, float newTop, float newHeight) 
: height(newHeight), baseRadius(newBase), topRadius(newTop), Object(newName)
{
	slices = 5;
	stacks = 5;
//	tex = 0;
	// Do nothing.
}
CylinderObject::CylinderObject(Object *o) : Object(o)
{
	CylinderObject *c = CastTo<CylinderObject>(o);
	if (c == NULL)
	{
		height = 0;
		baseRadius = 0;
		topRadius = 0;
		slices = 5;
		stacks = 5;
		return;
	}
	height = c->height;
	baseRadius = c->baseRadius;
	topRadius = c->topRadius;
	stacks = c->stacks;
	slices = c->slices;
}
void CylinderObject::shapeDraw()
{
	GLUquadric *q = gluNewQuadric();
//	if (isTransparent())
//		glDisable(GL_DEPTH_TEST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glRotatef(90, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	gluQuadricTexture(q, true);
	gluCylinder(q, baseRadius, topRadius, height, slices, stacks);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	gluDeleteQuadric(q);
}
void CylinderObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{  
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of cylinder parms at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	//cout << "Reading cylinder types num:" << lineNumber << endl;
	do
	{
		file.clear();
		string text;
//		text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("height") == 0)
		{
			lineStream >> text >> this->height;
		}
		else if (line.find("topRadius") == 0)
		{
			lineStream >> text >> this->topRadius;
		}
		else if (line.find("slices") == 0)
		{
			lineStream >> text >> this->slices;
		}
		else if (line.find("stacks") == 0)
		{
			lineStream >> text >> this->stacks;
		}
		else if (line.find("baseRadius") == 0)
		{
			lineStream >> text >> this->baseRadius;
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	//cout << "End reading cylinder types: level: " << level << endl;
	file.clear();
}


