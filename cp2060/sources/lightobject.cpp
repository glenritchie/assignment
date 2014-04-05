#ifdef WIN32
#include <windows.h>
#endif
#include "lightobject.h"
#include "lightmanager.h"
#include "model.h"
#include <GL/gl.h>
#include <GL/glu.h>


LightObject::LightObject(string newName) : Object(newName)
{
	cout << "LightObjectC(1)" << endl;
	l = new Light(255, 255, 255);
	lightManager.addLight(l);
	DebugText(EMSG, "(1)Created light: 0x%x", l);
//	l->relative = true;
//	red = 255;
//	blue = 255;
//	green = 255;
}
LightObject::~LightObject()
{
	lightManager.removeLight(l);
}
LightObject::LightObject(Object *o) : Object(o)
{
	l = 0;
	cout << "LightObjectC(2)" << endl;
	LightObject *lo = CastTo<LightObject>(o);
	if (lo != NULL)
	{
		cout << "Notice:::: lo != NULL !! ??? " << endl;
		this->l = lo->l;
	}
	DebugText(EMSG, "l ==== 0x%x", l);
	if (l == 0)
	{
		l = new Light(255, 255, 255);
		lightManager.addLight(l);
		DebugText(EMSG, "------(2)Created light: 0x%x", l);
	}
	l->setStatic(true);
	//l->relative = true;
}

void LightObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of light object params at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	do
	{
		file.clear();
		string text;
//		text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		float fr, fg, fb;
		int r, g, b;
		if (line.find("redf") == 0)
		{
			lineStream >> text >> fr;
			this->l->setRed(int(255.0 * fr));
		}
		else if (line.find("bluef") == 0)
		{
			lineStream >> text >> fb;
			this->l->setBlue(int(255.0 * fb));
		}
		else if (line.find("greenf") == 0)
		{
			lineStream >> text >> fg;
			this->l->setGreen(int(255.0 * fg));
		}
		else if (line.find("static") == 0)
		{
			bool st = false;
			lineStream >> text >> st;
			this->l->setStatic(st);
		}
		else if (line.find("red") == 0)
		{
			lineStream >> text >> r;
			this->l->setRed(r);
		}
		else if (line.find("green") == 0)
		{
			lineStream >> text >> g;
			this->l->setGreen(r);
		}
		else if (line.find("blue") == 0)
		{
			lineStream >> text >> b;
			this->l->setBlue(r);
		}


		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	file.clear();

}

void LightObject::shapeDraw()
{
	// We don't need to draw anything. The lightmanager handles this itself
//	if (l == 0)
	float x, y, z;
	this->findObjectWorldCoords(x, y, z);
	extern float maxDistance;
	//glPushMatrix();
	//glLoadIdentity();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
	//	glTranslatef(x, y, z);
		GLUquadric *q = gluNewQuadric();
		gluQuadricDrawStyle(q, GLU_LINE);
//		gluSphere(q, maxDistance, 30, 30);
		gluDeleteQuadric(q);

//	glMatrixMode(GL_TEXTURE);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);

		glPopAttrib();
	//	glPopMatrix();

	l->origin.x = x; l->origin.y = y; l->origin.z = z;		
}
