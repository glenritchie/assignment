#ifndef __waypointObject_h__
#define __waypointObject_h__

#include "object.h"
#include "cylinder.h"
#include <vector>
#include <stack>
class WaypointObject : public Object
{
public:
		WaypointObject(string newName);
		WaypointObject(Object *o);
		~WaypointObject();
		void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);

		WaypointObject *findNearestTo(VECTOR3 position, float &d);

		
		bool findPathTo(WaypointObject *waypoint, std::vector<WaypointObject *> &v, std::stack<WaypointObject *> &s);
		std::string getAction() { return action; }
		std::string getPostAction() { return postAction; }
		float getActionAngle() { return actionAngle; }
protected:
		void shapeDraw();
private:
		WaypointObject *findNearestToInternal(VECTOR3 position, float &d, std::vector<WaypointObject *> &v);
		CylinderObject *c;
		std::vector<WaypointObject *>	connections;	
		std::vector<std::string>	connectionNames;	
		float actionAngle;
		std::string action;
		std::string postAction;
};
#endif //__lightobject_h__

