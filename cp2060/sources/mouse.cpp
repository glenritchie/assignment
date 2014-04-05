#include "globals.h"

//float oldcamx = 0;
//float oldcamy = 0;

int startX, startY;
int mx, my;
void drawMouseLine()
{
	if (mouseDown || middleMouseDown)
	{
		// Draw a line from where the click start and where it is now
		// Goto ortho mode
		Ortho();
		glLoadIdentity();
		
		// X axis
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
			glVertex2i(startX, startY);
			glVertex2i(mx, my);
		glEnd();	
	
		// Go back to perspective
		Perspective();
	}

}
void motion(int x, int y)
{
		mx = x; my = y;
		diffX = (float)(startX - x) / 50.0;
		diffY = (float)(startY - y) / 50.0;
}
void mouse(int button, int state, int x, int y)
{
	mx = x;
	my = y;
	switch (button) 
	{
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				startX = x;
				startY = y;
				middleMouseDown = true;
				//cout << "DOWN" << endl//;
			}
			else if (state == GLUT_UP)
				middleMouseDown = false;
			break;
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				startX = x;
				startY = y;
				mouseDown = true;
			}
			else if (state == GLUT_UP)
				mouseDown = false;
			break;
	}
	//glutPostRedisplay();
}


