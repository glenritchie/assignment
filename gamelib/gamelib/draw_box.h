#ifdef _MSC_VER

// Disable warning 4786:

// "identifier was truncated to '255' characters in the debug information"

#pragma warning( disable : 4786)

#endif



#ifndef __draw_box_h__
#define __draw_box_h__
//#include "window/control.h"
#ifndef WIN32
#ifndef DWORD
#define DWORD unsigned int
#endif
#else
#include <windows.h>
#endif
#include "vector.h"
/**
	Do i really need to comment on these functions?? Really???
	The only real thing to note is that every box is centered at the origin.
*/
//xtern "C"
//{
//void draw2DBox(int top, int left, int right, int bottom, DWORD colour[4]);
void draw2DBox(float top, float left, float right, float bottom, DWORD colour[4]);
void draw2DBoxOutline(int top, int left, int right, int bottom, DWORD colour[4]);
void draw3DBoxOutline(float centerx, float centery, float centerz, float depth, float width, float height, DWORD colour);
void draw3DBox(float centerx, float centery, float centerz, float depth, float width, float height, DWORD colour);
void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], float density);

void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], unsigned int tids[6], float density);
void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], unsigned int basetid, unsigned int tids[6], float density);

void getPointsOnFaceOfBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, float density, unsigned int face, VECTOR3 **points);
/**
	Tricked you huh? This function don't draw a box, but draws a line!
*/
void draw3DLine(float sx, float sy, float sz, float ex, float ey, float ez, DWORD colour);
//}
#endif // __draw_box_h__

