#include "waypointobject.h"
//#include "cylinder.h"
#include "model.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "3dmath.h"

extern Object* world;

WaypointObject::WaypointObject(string newName) : Object(newName)
{
	c = 0;
}
WaypointObject::~WaypointObject()
{
	if (c != 0)
		delete c;
	connections.clear();	
	connectionNames.clear();
}
WaypointObject::WaypointObject(Object *o) : Object(o)
{
	connections.clear();	
	connectionNames.clear();
	c = new CylinderObject("WaypointCylinder", 0.1, 0.1, 10);
	float a[3] = { 90, 0, 0 };
	c->setFixedAngles(a);
}

void WaypointObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		DebugText(EMSG, "*****Invalid start of waypoint object params at line: %d : expected { found ", lineNumber, line.c_str());
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
		
		if (line.find("postAction") == 0)
		{
			lineStream >> text >> postAction;
		}
		else if (line.find("actionAngle") == 0)
		{	
			lineStream >> text >> actionAngle;
		}
		else if (line.find("action") == 0)
		{
			lineStream >> text >> action;
		}
		else if (line.find("connect") == 0)
		{
			std::string name;
			lineStream >> text >> name;
			connectionNames.push_back(name);
		}

		if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	file.clear();

}

WaypointObject *WaypointObject::findNearestTo(VECTOR3 position, float &d)
{
	std::vector<WaypointObject *> v;
	return findNearestToInternal(position, d, v);
}
WaypointObject *WaypointObject::findNearestToInternal(VECTOR3 position, float &d, std::vector<WaypointObject *> &v)
{
	// Check if i am in the v array
	for (int i = 0; i < v.size(); i++)
		if (v[i] == this)
			return NULL;
	
	v.push_back(this);
	
	float o[3]; 
	this->findObjectWorldCoords(o[0], o[1], o[2]);
	//getOrigin(o);
	
	WaypointObject *wo = this;
	VECTOR3 vo(o[0], o[1], o[2]);
	float wod = Distance(vo, position);
	if (wod < d)
	{
		wo = this;
		d = wod;
	}
	//float wod = 9999999;
	// Now check child connections
	for (int i = 0; i < connections.size(); i++)
	{
		WaypointObject *two = connections[i]->findNearestToInternal(position, wod, v); 
		if (wod < d && two != NULL)
		{
			wo = two;
			d = wod;
		}
	}
	return wo;
}

bool WaypointObject::findPathTo(WaypointObject *waypoint, std::vector<WaypointObject *> &v, std::stack<WaypointObject *> &s)
{
	// Find a path to the target waypoint from ourselves
	//WaypointObject *wo = world->findObjectByName(waypoint->getName());
	// Check if i am in the v array
	for (int i = 0; i < v.size(); i++)
		if (v[i] == this)
			return false;

	v.push_back(this);

	if (waypoint == this)
	{
//		s.push(this);
		return true;
	}
		
	
	for (int i = 0; i < connections.size(); i++)
	{
		bool b = connections[i]->findPathTo(waypoint, v, s);
		if (b)
		{
			s.push(connections[i]);
			if (this == v[0])
				s.push(this);
			return true;
		}
	}
	return false;
}
void WaypointObject::shapeDraw()
{
	// Draw a cylinder straight up
	glColor3f(0,1,0);
	c->draw(0);

//	DebugText(EMSG, "connections: %d", connections.size());
	if (connections.size() > 0)
	{
		float to[3]; 
		this->findObjectWorldCoords(to[0], to[1], to[2]);
		glTranslatef(0, 5, 0);
		glBegin(GL_LINES);
		glColor3f(0, 0, 1.0);
		// Draw connections
		for (int i = 0; i < connections.size(); i++)
		{
			WaypointObject *wo = connections[i]; 
			float o[3]; 
			wo->findObjectWorldCoords(o[0], o[1], o[2]);

			VECTOR3 vwo(o[0], o[1], o[2]);
			VECTOR3 vto(to[0], to[1], to[2]);
			VECTOR3 a = vwo - vto;
			float d = Distance(vwo, vto);
			a = Normalize(a);
			a = a * d;
		
			glVertex3f(0,0,0);
			glVertex3f(a.x, a.y, a.z);

		}
		glEnd();
			
		return;
	}
	
	// Resolve connections
	DebugText(EMSG, "Creating links from %s", getName().c_str());
	for (int i = 0; i < connectionNames.size(); i++)
	{
		string wayname = string("object ") + connectionNames[i];
		WaypointObject *wo = CastTo<WaypointObject>(world->findObjectByName(wayname));
		if (wo == NULL)
			DebugText(EMSG, "Warning: connnection '%s' was NOT a waypoint!", wayname.c_str());
		else	
		{
			DebugText(EMSG, "Connecting to %s", wo->getName().c_str());	
			connections.push_back(wo);
		}
	}
}
