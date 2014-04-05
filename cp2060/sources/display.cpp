#include "globals.h"
#include "mouse.h"
/**
 * Orbit around the given target
 * @param target The target to orbit around
 * @param pitch The pitch in degrees
 * @param yaw The yaw in degrees
 * @param distance The distance from the point
 * @param p The point 
*/
void orbit(float target[3], float pitch, float yaw, float distance, float p[3])
{
		// Figure out eye co-ords
		// x  = cosf(ang) * distance
		// y  = sinf(ang) * distance
		pitch = deg2rad(pitch);
		yaw = deg2rad(yaw);
			
		float sp = sinf(pitch);
		float cp = cosf(pitch);
		float sy = sinf(yaw);
		float cy = cosf(yaw);
		// Work out the point on the virtual sphere the point is at
		p[0] = cy * cp * distance + target[0];
		p[1] = sp * distance + target[1];
		p[2] = sy * cp * distance + target[2];
}

void display() 
{
	// If we arn't ready yet, don't render anything
	if (!ready)
		return;
	
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Draw a line showing how much the user is dragging the camera around
	drawMouseLine();
	
	// Load the identity
	glLoadIdentity();
		
	WaypointObject *wo = CastTo<WaypointObject>(world->findObjectByName("object chairwaypoint"));
	string wayname("Can't find a waypoint!"); 
	float waydist = 9999999;
	if (wo)
	{
		wo = wo->findNearestTo(VECTOR3(botOrigin[0], botOrigin[1], botOrigin[2]), waydist);
		if (wo != NULL)
			wayname = wo->getName();
		else
			wayname = "findNearest failed!";
	}

	// Draw path
	if (!pathStack.empty())
	{
		std::vector<WaypointObject *> refill;
		int y = 82;
		Draw2DText(10, 100, 0xFFFFFFFF, "Found Path!", wayname.c_str(), waydist);
		do{
			WaypointObject *w = pathStack.top();
			refill.push_back(w);
			pathStack.pop();
			Draw2DText(10, y, 0xFFFFFFFF, "  -> %s", w->getName().c_str());
			y+=12;
		} while(!pathStack.empty());	
		// Refill the stack
		for (int i = refill.size()-1; i >= 0; i--)
		{
			pathStack.push(refill[i]);
		}
	}
	else 
			Draw2DText(10, 100, 0xFFFFFFFF, "NOT Found path!");

	selectedObject = world->findObjectByName(selectedName);
	Draw2DText(10, 10, 0xFFFFFFFF, "Selected Object: %s", selectedName.c_str());
	Draw2DText(10, 20, 0xFFFFFFFF, "currentFrameFile: %s", currentFrameFile.c_str());
	Draw2DText(10, 34, 0xFFFFFFFF, "Frame: %d Use Interpolation: %s", frame, useInterp ? "yes" : "no");
	Draw2DText(10, 46, 0xFFFFFFFF, "animation fps: %.2f", fps);
	Draw2DText(10, 58, 0xFFFFFFFF, "Closest Waypoint: %s: dist: %.2f", wayname.c_str(), waydist);

	double *viewMat = camera.GetViewMatrix()->GetMatrix();
	camera.updateFrustum();
	glLoadMatrixd(viewMat);
	
//		light[0]->origin.y = 1.0f;
	float target[3] = { 0, 0, 0 };	
	float pos[3];
	orbit(target, -10, lightRot[1], 20, pos);
	light[1]->origin.x = pos[0]; 
	light[1]->origin.y = pos[1]; 
	light[1]->origin.z = pos[2];

	world->generateMatrix(frame);
	
		
	//DebugText(EMSG, "vx: %.2f, vy: %.2f, vz: %.2f", v.x, v.y, v.z);
	glDisable(GL_LIGHTING);


	lightManager.setupLightsForFrame();




	glEnable(GL_LIGHTING);

	if (useTextures)
		glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0 , 1.0);
	float diffD[] = { 0.8, 0.8, 0.8, 1.0 };
	float ambD[] = { 0.2, 0.2, 0.2, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffD);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambD);

	world->draw(frame);
		
	if (useTextures)
		glEnable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	// Sort lights by distance to cam
	for (int i = lightManager.getNumberOfLights()-1; i != -1; i--)
	{
		lightManager.getLight(i)->Render();
	}
	//l[0]->Render();
	//l[1]->Render();
	//l[2]->Render();

	// Draw any text output
	glDisable(GL_LIGHTING);
	if (pauseSystem)
		displayHelp();
	drawFinal2DText();
	
	glDisable(GL_TEXTURE_2D);
	glFinish();
	glutSwapBuffers();		

	CResourceManager::getInstance().Think();
}

