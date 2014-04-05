#include "globals.h"

CTexture *helpWindowTexture = NULL;
/** Display a help window */
void displayHelp()
{

	if (helpWindowTexture == NULL)
		CResourceManager::getInstance().createResource(helpWindowTexture, "window_background");
	
	// Change to ortho mode
	Ortho();
	
	// Reset the matrix
	glLoadIdentity();
	
	// Figure out the bounds of the box
	int w2 = width / 2;
	int h2 = height / 2;
	
	int widthOfHelp = 500;
	int heightOfHelp = 500;
	
	int startX = w2 - (widthOfHelp / 2);
	int startY = 100;
	
	// Draw the box
	glColor4f(0.5, 0.5, 1.0, 0.5);
	helpWindowTexture->ActivateTexture(0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2i(startX, startY);
		glTexCoord2f(0, 0); glVertex2i(startX, startY + heightOfHelp);
		glTexCoord2f(1, 0); glVertex2i(startX + widthOfHelp, startY + heightOfHelp);
		glTexCoord2f(1, 1); glVertex2i(startX + widthOfHelp, startY);
	glEnd();

	// Print text
	char *text[] = {
		"                Help menu                         ",
		"                                                  ",
		"Q, ESC             Quit                           ",
		"I                  Turn interpolation on/off      ",
		"W                  Reload model/frame data from file",
		"F                  Switch between Texture-Mapped  ",
		"                      fonts and GLUT based fonts  ",
		"F1                 Pause                          ",
		"F2                 Turn wireframe on/off          ",
		"F3                 Turn texturing on/off          ",
		"F9                 Turn light 0 on/off (white)    ",
		"F10                Turn light 1 on/off (purple)   ",
		"F11                Turn light 2 on/off (blue)     ",
		"                                                  ",
		"ACTIONS                                           ",
		"-------                                           ",
		"R                  Right kick                     ",
		"L                  Left kick                      ",
		"A                  What's the time?               ",
		"H                  Drop kick own head.            ",
		"                                                  ",
  	"                                                  ",
		"Author: Glen Ritchie                              ",
		"Subject: CP2060 with Lindsay Ward                 ",
		"Program: A computer graphics assignment for uni.  ",
		"         It's written in OpenGL and shows a humanoid",
		"         figure interacting with it's environment.",
		"                                                  ",
		"Date Compiled:                                    ",
		__DATE__,
		__TIME__,
		"                                                  ",
		"Credits:                                          ",
		"  www.nehe.dev          Mark J. Kilgard for developing GLUT ",
		"  www.flipcode.com      www.markmorley.com         ",
		"  www.gamedev.net       Nate Robins                ",
		"  Lindsay Ward(CP2060)                             ",
		"  Matrix help:                                     ",
		"    www.cs.ualberta.ca/~andreas/math/matrfaq_latest.html",
		"  Coke(how else could i stay up at night?) ",
		0
	};
	
	// Print the text on the box
	int i = 0;
	int gutter = startX + 10;
	int y = startY + 20;
	while (text[i] != 0)
	{
		Draw2DText(gutter, y, 0xFFFFFFFF, text[i]); 
		y += 12;
		i++;
	};

	glEnable(GL_DEPTH_TEST);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	// Go back to perspective mode
	Perspective();

}


