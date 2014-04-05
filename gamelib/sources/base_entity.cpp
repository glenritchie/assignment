#include "3dmath.h"
#include "base_entity.h"
unsigned int uid = 0;

CBaseEntity::CBaseEntity()
{
	origin.x=origin.y=origin.z=angles.x=angles.y=angles.z=/*specangles.x=specangles.y=specangles.z=*/0;
	oldOrigin.x=oldOrigin.y=oldOrigin.z=oldAngles.x=oldAngles.y=oldAngles.z=1;
	//oldSpecAngles.x=oldSpecAngles.y=oldSpecAngles.z=1;
	changed = true;
	remove=false;
	parentCreation = false;
	thinking=true;
	health=100;
	total_health = 100;
	has_alpha = false;
	visible=false;
	display = true;
	//			velocity = 0;
	solid = true;
	char msg[100];
	sprintf(msg, "NULL%d", ++uid);
	friendlyName = msg;
}
CBaseEntity::~CBaseEntity()
{

}
void CBaseEntity::die()
{
	// Do nothing.
}

void CBaseEntity::init() 
{
	this->subinit();

}

CBaseEntity *CBaseEntity::create() 
{ 
	DebugText(EMSG, "%s class does not implement the create function!", getClassname());
	return NULL; 
}

void CBaseEntity::setClassname(std::string newclassname)
{ 
	Classname = newclassname; 
}
const char* CBaseEntity::getClassname() const {return (char*)Classname.c_str();} 
const char* CBaseEntity::getFriendlyName() { return (char*)friendlyName.c_str(); }
const char* CBaseEntity::getScript()
{
	return this->scriptName.c_str();
}
void CBaseEntity::setScript(std::string newscript)
{
	this->scriptName = newscript;
}
void CBaseEntity::setFriendlyName(std::string newname) 
{ 
	friendlyName = newname; 

	/*	posX=1
	posY=5
	posZ=-
	side=0
	angleX=0
	angleY=0
	angleZ=90
	mesh=light_tank_body2.x
	selectable=1
	script=""
	*/
}

std::string CBaseEntity::toString()
{ 
	return std::string("CBaseEntity:: TODO"); 
}
