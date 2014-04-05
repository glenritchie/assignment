#ifdef WIN32
#include <windows.h>
#endif

#include "model.h"


#include "bezier.h"
#include "box.h"
#include "cylinder.h"
#include "sphere.h"
#include "ground.h"
#include "file.h"
#include "lightobject.h"
#include "spotlight.h"
#include "waypointobject.h"

Object *createObject(string &type, Object *oldObject)
{
	if (type.find("bezier") == 0)
		return new BezierObject(oldObject);
	else if (type.find("box") == 0)
		return new BoxObject(oldObject);
	else if (type.find("cylinder") == 0)
		return new CylinderObject(oldObject);
	else if (type.find("sphere") == 0)
		return new SphereObject(oldObject);
	else if (type.find("ground") == 0)
		return new GroundObject(oldObject);
	else if (type.find("file") == 0)
		return new FileObject(oldObject);
	else if (type.find("light") == 0)
		return new LightObject(oldObject);
//	else if (type.find("spotlight") == 0)
//		return new SpotLightObject(oldObject);
	else if (type.find("waypoint") == 0)
		return new WaypointObject(oldObject);

	return NULL;
}
