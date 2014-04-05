#include "light.h"
#include "resource_manager.h"
#include "camera.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#include <GL/glut.h>
//#include "csprite.h"

Light::Light(int red, int green, int blue, float range)
{
	relative = false;
	r = red; g = green; b = blue;
//	s = 0;
	setRange(range);
	this->has_alpha = true;
	enableLight(true);
	setClassname("CLight");
	lightTexture = 0;
	staticLight = true;
}
void Light::subinit()
{
	

}
int Light::getRed()		{ return r; }
int Light::getBlue()	{ return b; }
int Light::getGreen()	{ return g; }

float Light::getFloatRed()		{ return ((float)r/255); }
float Light::getFloatBlue()		{ return ((float)b/255); }
float Light::getFloatGreen()	{ return ((float)g/255); }

void Light::setRed(int newRed)		{ r = newRed; }
void Light::setGreen(int newGreen)	{ g = newGreen; }
void Light::setBlue(int newBlue)	{ b = newBlue; }

void Light::enableLight(bool newState)
{	
	enabled = newState;
}
bool Light::isLightEnabled()
{
	return enabled;
}

void Light::setRange(float newRange)
{
	range = newRange;
}
float Light::getRange()
{
	return range;
}

void Light::Render()
{ 
	if (!isLightEnabled())
		return;
	if (lightTexture == 0)
	{
		DebugText(EMSG, "Loading light texture...");
		CResourceManager::getInstance().createResource(lightTexture, "lens_flare1.tga");
		if (lightTexture == 0)
		{
			DebugText(EMSG, "lightTexture is NULL!!");
			return;
		}
		lightTexture->SetNumFrames(1);
		lightTexture->SetFPS(10);
		lightTexture->SetFrameSize(64, 64);
		lightTexture->SetFrame(0);
		lightTexture->StartAnimation();
	}
	extern CCamera camera;
	// Display a light sprite as a billboarded texture

	float oldDiff[4] = { 0, 0, 0, 0 };
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDiff);
	float diff[4] = { getFloatRed(), getFloatGreen(), getFloatBlue(), 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);

	VECTOR3 right = camera.right;
	VECTOR3 up = camera.up;
	VECTOR3 f = camera.forward;

	VECTOR3 bl, br, tr, tl;
	tl = (right*-1) + up;
	bl = (right*-1) + (up*-1);
	br = right + (up*-1);
	tr = right + up;

	glPushMatrix();
	if (relative)
	{
		glLoadMatrixf((float*)m);
	}
	else 
		glTranslatef(origin.x, origin.y, origin.z);
	lightTexture->ActivateTexture(0);
	glColor3f(	getFloatRed(), getFloatGreen(), getFloatBlue());
	float u[2], v[2];
	lightTexture->GetTexcoords(u[0], u[1], v[0], v[1]);
	
	glBegin(GL_TRIANGLES);
		glTexCoord2f(u[0], v[0]); glVertex3f(bl.x, bl.y, bl.z); 		
		glTexCoord2f(u[1], v[0]); glVertex3f(br.x, br.y, br.z); 	
 		glTexCoord2f(u[0], v[1]); glVertex3f(tl.x, tl.y, tl.z);
	
		glTexCoord2f(u[1], v[0]); glVertex3f(br.x, br.y, br.z);
		glTexCoord2f(u[1], v[1]); glVertex3f(tr.x, tr.y, tr.z); 
		glTexCoord2f(u[0], v[1]); glVertex3f(tl.x, tl.y, tl.z);
	


/*		glTexCoord2f(0.0, 0.0); glVertex3f(bl.x, bl.y, bl.z); 		
		glTexCoord2f(1.0, 0.0); glVertex3f(br.x, br.y, br.z); 	
 		glTexCoord2f(0.0, 1.0); glVertex3f(tl.x, tl.y, tl.z);
	
		glTexCoord2f(1.0, 0.0); glVertex3f(br.x, br.y, br.z);
		glTexCoord2f(1.0, 1.0); glVertex3f(tr.x, tr.y, tr.z); 
		glTexCoord2f(0.0, 1.0); glVertex3f(tl.x, tl.y, tl.z);
		*/
		

	glEnd();

	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, oldDiff);




/*	if (isLightEnabled() == false)
		return;
	// Draw a sprite for like a lens flare
	CVector2 scale(1,1);	
	if (s == 0)
	{
		//ResourceManager->CreateResource(s, "LampLensFlare");
		s = new CSprite();
		s->BindTexture("lens_flare1.tga");
	}
	s->Render3D(origin, 0, scale);
	*/
}
void Light::PreRender()					
{ 
	;	
}
void Light::Cleanup()
{ 
//	if (s)
//		s->Cleanup();
	/* remove from light manager?*/  
//	s = 0;
}
void Light::Think()						
{ 
}
void Light::Collision(CBaseEntity *other)	{ ; }

void Light::getbbox(VECTOR3 &min, VECTOR3 &max) { ; }
