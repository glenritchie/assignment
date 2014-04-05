#include "globals.h"

bool clip = false;
float oldDistance = 50;
bool genLightOnce = false;

Object *rObject = NULL;
int rayTracingLight = -1;
float objectProgress = 0;
float faceProgress = 0;
int rayTracingFace = -1;
float oldFP = -2;	
int rX = 0, rY = 0;
unsigned int tid = 0;
int etw = 0, eth = 0;
unsigned char *rColor;
void generateLightMapsDisplay()
{
	if (fabs(faceProgress - oldFP) < (1.0/100))
		return;
	oldFP = faceProgress;	
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Goto ortho mode
	Ortho();
	glLoadIdentity();
	
	string name("");
	if (rObject != NULL)
		name = rObject->getName();
	Draw2DText(10, 70, 0xFFFFFFFF, "Doing Object: %s : ObjectProgress: %.2f",  name.c_str(), objectProgress*100);
	Draw2DText(10, 82, 0xFFFFFFFF, "   Light: %d, face: %d: progress: %.2f", rayTracingLight, rayTracingFace, faceProgress*100);
	glEnable(GL_TEXTURE_2D);

	// Draw what we got so far
	if (tid != 0)
		glDeleteTextures(1, &tid);
	glGenTextures(1, &tid);

	glBindTexture(GL_TEXTURE_2D, tid);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, etw, eth, GL_RGB, GL_UNSIGNED_BYTE, rColor);

	float startX = 10;
	float startY = 100;
	float endX = startX + 450;
	float endY = startY + 450;
	glBindTexture(GL_TEXTURE_2D, tid);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0,1);	glVertex2f(startX, endY);
	glTexCoord2f(1,1);	glVertex2f(endX, endY);
	
	glTexCoord2f(1,0);	glVertex2f(endX, startY);
	glTexCoord2f(0,0); 	glVertex2f(startX, startY);
	
	glEnd();
	glEnable(GL_CULL_FACE);

//	glRasterPos2f(startX, startY);
//	glDrawPixels(1024, 1024, GL_RGB, GL_INT, rColor);
//	glRasterPos2f(0,0);
	


	// Go back to perspective
	Perspective();

	drawFinal2DText();
		
	glDisable(GL_TEXTURE_2D);
	glFinish();
	glutSwapBuffers();
}


