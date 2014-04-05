#include "spotlight.h"
#include "lightmanager.h"
#include "model.h"

SpotLightObject::SpotLightObject(string newName) : Object(newName)
{
	cout << "SpotLightObjectC(1)" << endl;
	l = new SpotLight();
	//l->setExp(
	lightManager.addLight(l);
	DebugText(EMSG, "(1)Created light: 0x%x", l);
//	l->relative = true;
//	red = 255;
//	blue = 255;
//	green = 255;
}
SpotLightObject::~SpotLightObject()
{
	lightManager.removeLight(l);
}
SpotLightObject::SpotLightObject(Object *o) : Object(o)
{
	l = 0;
	cout << "SpotLightObjectC(2)" << endl;
	SpotLightObject *lo = CastTo<SpotLightObject>(o);
	if (lo != NULL)
	{
		cout << "Notice:::: lo != NULL !! ??? " << endl;
		this->l = lo->l;
	}
	DebugText(EMSG, "l ==== 0x%x", l);
	if (l == 0)
	{
		l = new SpotLight();
		lightManager.addLight(l);
		DebugText(EMSG, "------(2)Created light: 0x%x", l);
	}
	//l->relative = true;
}

void SpotLightObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
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
		else if (line.find("exponent") == 0)
		{
			float exp;
			lineStream >> text >> exp;
			this->l->setExp(exp);
		}
		else if (line.find("direction") == 0)
		{
			float a = 0, b = 0, c = -1;
			lineStream >> text >> a >> b >> c;
			this->l->setDir(a, b, c);
		}
		else if (line.find("cutoff") == 0)
		{
			float cutoff;
			lineStream >> text >> cutoff;
			this->l->setCutOff(cutoff);
		}
		else if (line.find("greenf") == 0)
		{
			lineStream >> text >> fg;
			this->l->setGreen(int(255.0 * fg));
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

void SpotLightObject::shapeDraw()
{
	// We don't need to draw anything. The lightmanager handles this itself
//	if (l == 0)
	float x, y, z;
	this->findObjectWorldCoords(x, y, z);
	
	l->origin.x = x; l->origin.y = y; l->origin.z = z;		
}
