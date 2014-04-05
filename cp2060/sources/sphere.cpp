#include "sphere.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "model.h"

SphereObject::SphereObject(string newName, float newRadius) 
	: radius(newRadius), Object(newName)
{
	// Do nothing.
}

void SphereObject::shapeDraw()
{
	GLUquadric *q = gluNewQuadric();
//	glMatrixMode(GL_TEXTURE);
//	glPushMatrix();
//	glRotatef(180, 1, 1, 0);
//	glMatrixMode(GL_MODELVIEW);

	gluQuadricTexture(q, true);
	gluSphere(q, radius, 10, 10);
	gluDeleteQuadric(q);

//	glMatrixMode(GL_TEXTURE);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
}
void SphereObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level) 
{  
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of sphere parms at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	//cout << "Reading sphere types num:" << lineNumber << endl;
	do
	{
		file.clear();
		string text;
//		text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("radius") == 0)
		{
			lineStream >> text >> this->radius;
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	//cout << "End reading sphere types: level: " << level << endl;
	file.clear();
}


