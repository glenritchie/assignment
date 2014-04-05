#include "lightmanager.h"
#include "camera.h"						// Camera stuff
//#include "entitylist.h"
//#include "entity_utils.h"
#include <set>
#include <vector>
#include <algorithm>
#include "light.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
//#include <GL/glext.h>
//#include "ui\BaseUI.h"
//extern CBaseUI *UI;

LightManager lightManager;
std::vector<Light *> lightVector;

//extern LPDIRECT3DDEVICE8 g_pd3dDevice;

LightManager::LightManager()
{
	maxLights = 8;
}

void LightManager::removeLight(Light *light)
{
	std::vector<Light *>::iterator i;
	for (i = lightVector.begin(); i != lightVector.end(); i++)
	{
		Light *l = (*i);
		if (l == light)
		{
			lightVector.erase(i);
			delete light;
			return;
		}
	}
}

void LightManager::addLight(Light *newLight)
{
	//lightVector.push_back(newLight);
//	EntList.Add(newLight);
	lightVector.push_back(newLight);
	
}
unsigned int LightManager::getNumberOfLights()
{
//	std::vector<CBaseEntity *> lightVector = EntList.getVisibleEnts("CLight");
	return lightVector.size();
}
Light *LightManager::getLight(unsigned int index)
{
//	std::vector<CBaseEntity *> lightVector = EntList.getVisibleEnts("CLight");
//	if (index < lightVector.size())
		return CastTo<Light>(lightVector[index]);
	
	return NULL;
}
bool LightSortCB(Light *p1, Light *p2)
{
	// Is p1 greater than p2
	extern CCamera camera;
	CVector3 &campos = camera.origin;
	p1->camera_distance = Distance(campos, p1->origin);
	p2->camera_distance = Distance(campos, p2->origin);
	return p1->camera_distance < p2->camera_distance ? true : false;
}

void LightManager::setupLightsForFrame()
{
//	extern bool EntSortCB(CBaseEntity *p1, CBaseEntity *p2);
	//std::set<CBaseEntity *> *visEnts = EntList.getVisibleEnts();
	// Remove ents of any type except Lights
	//visEnts->erase(	
//	std::vector<Light *> &lightVector = ;
	std::sort(lightVector.begin(), lightVector.end(), LightSortCB);	

	unsigned int lightsProcessed = 0;
	
	for (long unsigned int i = 0; i < lightVector.size(); i++)
	{
		Light *l =lightVector[i];
		if (l->isLightEnabled() == false)
		{
			continue;
		}
		if (lightsProcessed > maxLights)
			continue;
		GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse[] = { l->getFloatRed(), l->getFloatGreen(), l->getFloatBlue(), 1.0 };
		GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_position[] = {  l->origin.x, l->origin.y, l->origin.z, 1.0 };

		glLightfv(GL_LIGHT0 + lightsProcessed, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0 + lightsProcessed, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0 + lightsProcessed, GL_SPECULAR, light_specular);
		if (!l->relative)
		{
			glLightfv(GL_LIGHT0 + lightsProcessed, GL_POSITION, light_position);
		}
		else
		{
			glPushMatrix();
			glLoadIdentity();
			//GLfloat lpos[] = {  0, 0, 0, 1.0 };
			//glLoadMatrixf((float *)l->m);
//			glTranslatef(
			//glLightfv(GL_LIGHT0 + lightsProcessed, GL_POSITION, lpos);
			glLightfv(GL_LIGHT0 + lightsProcessed, GL_POSITION, light_position);
			glPopMatrix();
		}

//		extern float ca;
//		extern float la;
//		extern float qa;
//	glLightf(GL_LIGHT0 + lightsProcessed, GL_CONSTANT_ATTENUATION, ca);		
//		  glLightf(GL_LIGHT0 + lightsProcessed, GL_LINEAR_ATTENUATION, la);		
//		  glLightf(GL_LIGHT0 + lightsProcessed, GL_QUADRATIC_ATTENUATION, qa);		
		glEnable(GL_LIGHT0 + lightsProcessed);

/*		D3DLIGHT8 m_light;                 // Description of the D3D light
		ZeroMemory( &m_light, sizeof(m_light) );
		m_light.Type = D3DLIGHT_POINT;
		m_light.Diffuse.r  = l->getFloatRed();
		m_light.Diffuse.g  = l->getFloatGreen();
		m_light.Diffuse.b  = l->getFloatBlue();
		m_light.Range      = l->getRange();
		m_light.Position     = D3DXVECTOR3( l->origin.x, l->origin.y, l->origin.z );
		m_light.Theta        = 0.5f;
		m_light.Phi          = 1.0f;
		m_light.Falloff		= 1.0f;
		m_light.Attenuation0 = 0.5f;
	     //   m_light.Position     = 4.5f * D3DXVECTOR3( x, y, z );
		m_light.Attenuation1 = 0.4f;
		g_pd3dDevice->LightEnable( lightsProcessed, true );
		g_pd3dDevice->SetLight( lightsProcessed, &m_light );
	*/	
		lightsProcessed++;
//				DebugText(EMSG, "Setting light %d to location %.2f, %.2f, %.2f",
//			i, l->origin.x, l->origin.y, l->origin.z);
	}
//	g_pd3dDevice->LightEnable( lightsProcessed, false );
		glDisable(GL_LIGHT0 + lightsProcessed);
}

