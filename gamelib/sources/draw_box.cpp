#include "draw_box.h"
#include "utils.h"
#include "vector.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#include <GL/glext.h>
//extern "C"
//{
extern bool multitexture_availiable;
extern void APIENTRY glMultiTexCoord2dARB(int stream, double u, double v);
extern void APIENTRY glActiveTextureARB (GLenum p);
typedef struct LINEVERTEX3D { float x; float y; float z; DWORD color; } LINEVERTEX3D;
	// x, y, z, rhw, colour, tu,tv
typedef struct CONTROLVERTEX 
	{ 
		float x; float y; float z; float rhw; DWORD diffuse; float tu0; float tv0;
} CONTROLVERTEX;
#define ALPHA(x) ((x&0xFF000000)>>24)
#define RED(x)   ((x&0x00FF0000)>>16)
#define GREEN(x) ((x&0x0000FF00)>>8)
#define BLUE(x)  ((x&0x000000FF))
void draw3DBoxOutline(float centerx, float centery, float centerz, float depth, float width, float height, DWORD colour)
{
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - ddist;
	float maxy = centery + ddist;
	float minz = centerz - hdist;
	float maxz = centerz + hdist;
		
	LINEVERTEX3D Vertices[] =	{
		// x, y, z, colour
		{	minx,	miny,		minz,	colour },		// BL
		{	minx,	maxy,		minz,	colour },		// BR
		{	minx,	miny,		maxz,	colour },		// TL
		{	minx,	maxy,		maxz,	colour },		// TR
		//{	minx,	miny,		minz,	colour },		// BL

		{	maxx,	miny,		minz,	colour },		// BL
		{	maxx,	maxy,		minz,	colour },		// BR
		{	maxx,	miny,		maxz,	colour },		// TL
		{	maxx,	maxy,		maxz,	colour },		// TR
//		{	maxx,	miny,		minz,	colour },		// BL

		{	minx,	miny,		minz,	colour },		// BL
		{	maxx,	miny,		minz,	colour },		// BR
		{	minx,	miny,		maxz,	colour },		// TL
		{	maxx,	miny,		maxz,	colour },		// TR
	//	{	minx,	miny,		minz,	colour },		// BL

		{	minx,	maxy,		minz,	colour },		// BL
		{	maxx,	maxy,		minz,	colour },		// BR
		{	minx,	maxy,		maxz,	colour },		// TL
		{	maxx,	maxy,		maxz,	colour },		// TR
		//{	minx,	maxy,		minz,	colour },		// BL

		// Top Z
		{	minx,	miny,		maxz,	colour },		// BL
		{	maxx,	miny,		maxz,	colour },		// BR
		{	minx,	maxy,		maxz,	colour },		// TL
		{	maxx,	maxy,		maxz,	colour },		// TR
//		{	minx,	miny,		maxz,	colour },		// BL

		// Bottom Z
		{	minx,	miny,		minz,	colour },		// BL
		{	minx,	maxy,		minz,	colour },		// TL
		{	maxx,	maxy,		minz,	colour },		// TR
		{	maxx,	miny,		minz,	colour },		// BR
//		{	minx,	miny,		minz,	colour },		// BL
	};                                                          

//	glLoadIdentity();
//	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6*4; )
	{
		glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < 4; j++)
			{
				DWORD &c = Vertices[i].color;
				float c1[4] ={
					((float)RED(c)) / 255.0f,
				  ((float)GREEN(c)) / 255.0f,
				  ((float)BLUE(c)) / 255.0f,
				  ((float)ALPHA(c)) / 255.0f
				};
			
				glColor4fv(c1);  
				glVertex3fv((float *)&Vertices[i].x); 
				i++;
			}
	glEnd();
	}	

}
void draw3DBox(float centerx, float centery, float centerz, float depth, float width, float height, DWORD colour)
{
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - hdist;
	float maxy = centery + hdist;
	float minz = centerz - ddist;
	float maxz = centerz + ddist;
		
	LINEVERTEX3D Vertices[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z 1
	{	{	minx,	miny,	maxz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Far face - Facing z +1
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	maxy,	minz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Left	face -x
	{ {	minx,	miny,	minz,	colour },		// BL
		{	minx,	miny,	maxz,	colour },		// BR
		{	minx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Right Face +x
	{	{	maxx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	maxx,	maxy,	minz,	colour }},	// TL

		// Top Face +y
	{	{	minx,	maxy,	minz,	colour },		// BL
		{	maxx,	maxy,	minz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Bottom Face -y
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	miny,	maxz,	colour },		// TR
		{	minx,	miny,	maxz,	colour }},	// TL
	};
	float norms[][3] = {
		{ 0, 0, 1 },	// Near
		{ 0, 0, -1 }, // Far
		{ -1, 0, 0 }, // Left
		{ 1, 0, 0 }, // Right
		{ 0, 1, 0 }, // Top
		{ 0, -1, 0}, // Bottom
	};
	unsigned int size = 4;
	//for (int i = 0; i < 6*4; )

	float divisions = 2;
	DWORD &c = colour;//Vertices[j].color;
	float c1[4] ={
		((float)RED(c)) / 255.0f,
		((float)GREEN(c)) / 255.0f,
		((float)BLUE(c)) / 255.0f,
		((float)ALPHA(c)) / 255.0f
	};
//	glColor4fv(c1);  

	int dirs[6] = { 0, 0, 1, 1, 2, 2 };
	bool flip[6] = { 1, 0, 0, 1, 0, 1 };
	/*
		0 - x, y 
		1 - x, y
		2 - y, z
		3 - y, z
		4 - x, z
		5 - x, z
	*/

	float stepX = width / divisions;
	float stepY = height / divisions;
	float stepZ = depth / divisions;
	float x, y, z;
	for (unsigned int i = 0; i < 6; i++)
	{
		glBegin(GL_QUADS);
		glNormal3fv(norms[i]);
		
		if (dirs[i] == 0)
		{
			// Subdivide on X, Y plane
			z = Vertices[i][0].z;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY)
				{
					if (flip[i])
					{
						glTexCoord2d(0,0);	glVertex3f(x, y, z);
						glTexCoord2d(1,0);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(1,1);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(0,1);	glVertex3f(x, y+stepY, z);
					}
					else
					{
						glTexCoord2d(0,0);	glVertex3f(x, y, z);
						glTexCoord2d(0,1);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(1,1);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(1,0);	glVertex3f(x+stepX, y, z);
					}
				}
			}
		}
		else if (dirs[i] == 1)
		{
//			DebugText(EMSG, "Drawing Y,Z (1) plane!");
			// Subdivide on Y, Z plane
			x = Vertices[i][0].x;
			for (z = Vertices[i][0].z; z < Vertices[i][1].z; z += stepZ)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY)
				{
					if (flip[i])
					{
						glTexCoord2d(0,0);	glVertex3f(x, y, z);
						glTexCoord2d(0,1);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(1,1);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(1,0);	glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y, z+stepZ);
						glVertex3f(x, y+stepY, z+stepZ);
						glVertex3f(x, y+stepY, z);
					}
				}
			}
		}
/*		else if (dirs[i] == 2)
		{
			// Subdivide on X, Z plane
//			DebugText(EMSG, "Drawing X,Z (2) plane!");
			y = Vertices[i][0].y;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX)
			{
				for (z = Vertices[i][0].z; z < Vertices[i][2].z; z+= stepZ)
				{
					if (flip[i])
					{
						glVertex3f(x, y, z);
						glVertex3f(x+stepX, y, z);
						glVertex3f(x+stepX, y, z+stepZ);
						glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y, z+stepZ);
						glVertex3f(x+stepX, y, z+stepZ);
						glVertex3f(x+stepX, y, z);
					}

				}
			}	
		}
*/

	glEnd();

	}

}

void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], float density)
{
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - hdist;
	float maxy = centery + hdist;
	float minz = centerz - ddist;
	float maxz = centerz + ddist;
	LINEVERTEX3D Vertices[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z 1
	{	{	minx,	miny,	maxz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},		// TL

		// Far face - Facing z +1
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	maxy,	minz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Left	face -x
	{ {	minx,	miny,	minz,	colour },		// BL
		{	minx,	miny,	maxz,	colour },		// BR
		{	minx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Right Face +x
	{	{	maxx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	maxx,	maxy,	minz,	colour }},	// TL

		// Top Face +y
	{	{	minx,	maxy,	minz,	colour },		// BL
		{	maxx,	maxy,	minz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Bottom Face -y
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	miny,	maxz,	colour },		// TR
		{	minx,	miny,	maxz,	colour }},	// TL
	};
	float norms[][3] = {
		{ 0, 0, 1 },	// Near
		{ 0, 0, -1 }, // Far
		{ -1, 0, 0 }, // Left
		{ 1, 0, 0 }, // Right
		{ 0, 1, 0 }, // Top
		{ 0, -1, 0}, // Bottom
	};
	unsigned int size = 4;
	//for (int i = 0; i < 6*4; )

	float divisions = density;
	DWORD &c = colour;//Vertices[j].color;
	float c1[4] ={
		((float)RED(c)) / 255.0f,
	  ((float)GREEN(c)) / 255.0f,
	  ((float)BLUE(c)) / 255.0f,
	  ((float)ALPHA(c)) / 255.0f
	};
//	glColor4fv(c1);  

	int dirs[6] = { 0, 0, 1, 1, 2, 2 };
	bool flip[6] = { 1, 0, 0, 1, 0, 1 };
	/*
		0 - x, y 
		1 - x, y
		2 - y, z
		3 - y, z
		4 - x, z
		5 - x, z
	*/
// tcoords[6][2]
	float stepX = width / divisions;
	float stepY = height / divisions;
	float stepZ = depth / divisions;
	float x, y, z;
	int e = 0;
	for (unsigned int i = 0; i < 6; i++)
//	unsigned int i = 3;
	{
	float sx = 0, sy = 0, sz = 0;
		
//		if (i == 3)
//			DebugText(EMSG, "tshift[%d]: %.2f, %.2f, %.2f", i, tshift[i][0], tshift[i][1], tshift[i][2]);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glScalef(tcoords[i][0], tcoords[i][1], 1);
		glTranslatef(tshift[i][0], tshift[i][1], tshift[i][2]);
//		glTranslatef(10, 10, 10);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
		glNormal3fv(norms[i]);
		

		if (dirs[i] == 0)
		{
			// Subdivide on X, Y plane
			z = Vertices[i][0].z;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx += stepX, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy += stepY)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy+stepY)) };
					
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
					}
				}
			}
		}
		else if (dirs[i] == 1)
		{
//			DebugText(EMSG, "Drawing Y,Z (1) plane!");
			// Subdivide on Y, Z plane
			x = Vertices[i][0].x;
			
			for (z = Vertices[i][0].z; z < Vertices[i][1].z; z += stepZ, sz+= stepZ, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy+=stepY)
				{
					double u[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz + stepZ)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy + stepY)) };
//					DebugText(EMSG, "u[0]:%.2f, v[0]:%.2f: u[1]:%.2f, v[1]:%.2f, flip:%d", u[0], v[0], u[1], v[1], flip[i]);
//					DebugText(EMSG, "minz: %.2f, maxz: %.2f", minz, maxz);
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
					}
				}
			}
		}
		else if (dirs[i] == 2)
		{
			// Subdivide on X, Z plane
//			DebugText(EMSG, "Drawing X,Z (2) plane!");
			y = Vertices[i][0].y;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx+=stepX, sz = 0)
			{
				for (z = Vertices[i][0].z; z < Vertices[i][2].z; z+= stepZ, sz+=stepZ)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz+stepZ)) };
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x+stepX, y, z);
					}

				}
			}	
		}

	glEnd();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	}

}
/*
void draw2DBoxOutline(int top, int left, int right, int bottom, DWORD colour[4])
{
	CONTROLVERTEX Vertices[5] =	{
		// x, y, z, rhw, colour, tu,tv
		{	left,	bottom,		0.0f,	1.0f,	colour[0],	0.0f,	1.0f },		// BL
		{	left,	top,	  	0.0f,	1.0f,	colour[1],	0.0f,	0.0f },		// TL
		{	right,	top,		0.0f,	1.0f,	colour[3],	1.0f,	0.0f },		// TR
		{	right,	bottom,	0.0f,	1.0f,	colour[2],	1.0f,	1.0f },		// BR
		{	left,	bottom,		0.0f,	1.0f,	colour[0],	0.0f,	1.0f },		// BL
	};                                                          

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; )
	{
			DWORD &c = Vertices[i].diffuse;
			float c1[4] ={
				((float)RED(c)) / 255.0f,
			  ((float)GREEN(c)) / 255.0f,
			  ((float)BLUE(c)) / 255.0f,
			  ((float)ALPHA(c)) / 255.0f
			};
			
			glColor4fv(c1);  
			glTexCoord2dv((float*)&Vertices[i].tu0);
			glVertex3fv((float *)&Vertices[i].x); 
			i++;
	}	
	glEnd();
}
*/
/*
void draw2DBox(int top, int left, int right, int bottom, DWORD colour[4])
{
	CONTROLVERTEX Vertices[4] =	{
		// x, y, z, rhw, colour, tu,tv
		{	left,	bottom,		0.0f,	1.0f,	colour[0],	0.0f,	1.0f },		// BL
		{	right,	bottom, 0.0f,	1.0f,	colour[2],	1.0f,	1.0f },		// BR
		{	left,	top,			0.0f,	1.0f,	colour[1],	0.0f,	0.0f },		// TL
		{	right,	top,		0.0f,	1.0f,	colour[3],	1.0f,	0.0f },		// TR
	};                                                          
	
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; )
	{
			DWORD &c = Vertices[i].diffuse;
			float c1[4] ={
				((float)RED(c)) / 255.0,
			  ((float)GREEN(c)) / 255.0,
			  ((float)BLUE(c)) / 255.0,
			  ((float)ALPHA(c)) / 255.0
			};
			
			glColor4fv(c1);  
			glTexCoord2d(Vertices[i].tu0,Vertices[i].tv0);
			glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z); 
			i++;
	}	
	glEnd();
}
*/
void draw2DBox(float top, float left, float right, float bottom, DWORD colour[4])
{
	CONTROLVERTEX Vertices[4] =	{
		// x, y, z, rhw, colour, tu,tv
		{	left,	bottom,		0.0f,	1.0f,	colour[0],	0.0f,	1.0f },		// BL
		{	right,	bottom, 0.0f,	1.0f,	colour[2],	1.0f,	1.0f },		// BR
		{	left,	top,			0.0f,	1.0f,	colour[1],	0.0f,	0.0f },		// TL
		{	right,	top,		0.0f,	1.0f,	colour[3],	1.0f,	0.0f },		// TR
	};                                                          
	
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; )
	{
			DWORD &c = Vertices[i].diffuse;
			float c1[4] ={
				((float)RED(c)) / 255.0f,
			  ((float)GREEN(c)) / 255.0f,
			  ((float)BLUE(c)) / 255.0f,
			  ((float)ALPHA(c)) / 255.0f
			};
			
			glColor4fv(c1);  
			glTexCoord2d(Vertices[i].tu0,Vertices[i].tv0);
			glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z); 
			i++;
	}	
	glEnd();
}

void draw3DLine(float sx, float sy, float sz, float ex, float ey, float ez, DWORD colour)
{
	glLoadIdentity();
	/*D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );	// Move object
*/
			DWORD &c = colour;
			float c1[4] ={
				((float)RED(c)) / 255.0f,
			  ((float)GREEN(c)) / 255.0f,
			  ((float)BLUE(c)) / 255.0f,
			  ((float)ALPHA(c)) / 255.0f
			};
			
			glColor4fv(c1);  
	
		glBegin(GL_LINES);
//		glColor4f(RED(colour), GREEN(colour), BLUE(colour), ALPHA(colour));  
		glVertex3f(sx, sy, sz);
		glVertex3f(ex, ey, ez);
	glEnd();	
	/*
	#define D3DFVF_LVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
	struct LVERTEX { float x, y, z; DWORD diffuse; };
	LVERTEX line[2] = {
		{ sx,	sy,	sz,	colour	}, 
		{ ex,	ey, ez,	colour	}
	};			
	SetVertexShader( D3DFVF_LVERTEX );
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 2, line, sizeof(LVERTEX));
*/
}
//}







void getPointsOnFaceOfBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, float density, unsigned int face, VECTOR3 **points)
{
	unsigned int colour = 0;
	double hdist = height / 2.0;
	double wdist = width / 2.0;
	double ddist = depth / 2.0;
	double minx = centerx - wdist;
	double maxx = centerx + wdist;
	double miny = centery - hdist;
	double maxy = centery + hdist;
	double minz = centerz - ddist;
	double maxz = centerz + ddist;
	LINEVERTEX3D Vertices[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z 1
	{	{	minx,	miny,	maxz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},		// TL

		// Far face - Facing z +1
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	maxy,	minz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Left	face -x
	{ {	minx,	miny,	minz,	colour },		// BL
		{	minx,	miny,	maxz,	colour },		// BR
		{	minx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Right Face +x
	{	{	maxx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	maxx,	maxy,	minz,	colour }},	// TL

		// Top Face +y
	{	{	minx,	maxy,	minz,	colour },		// BL
		{	maxx,	maxy,	minz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Bottom Face -y
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	miny,	maxz,	colour },		// TR
		{	minx,	miny,	maxz,	colour }},	// TL
	};

	double divisions = density;

	int dirs[6] = { 0, 0, 1, 1, 2, 2 };
	int a = 0, b = 0;
	double stepX = width / divisions;
	double stepY = height / divisions;
	double stepZ = depth / divisions;
	double x, y, z;
	
	unsigned int i = face;
	float sx = 0, sy = 0, sz = 0;
	
	if (dirs[i] == 0)
	{
		// Subdivide on X, Y plane
		z = Vertices[i][0].z;
		for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx += stepX, sy = 0, b = 0, a++)
		{
			for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy += stepY, b++)
			{
				points[a][b] = VECTOR3(x, y, z);
			}
		}
	}
	else if (dirs[i] == 1)
	{
		x = Vertices[i][0].x;
		for (z = Vertices[i][0].z; z < Vertices[i][1].z; z += stepZ, sz+= stepZ, sy = 0, b = 0, a++ )
		{
			for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy+=stepY, b++)
			{
				points[a][b] = VECTOR3(x, y, z);
			}
		}
	}
	else if (dirs[i] == 2)
	{
		y = Vertices[i][0].y;
		for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx+=stepX, sz = 0, b = 0, a++)
		{
			for (z = Vertices[i][0].z; z < Vertices[i][2].z; z+= stepZ, sz+=stepZ, b++)
			{
				points[a][b] = VECTOR3(x, y, z);
			}
		}
	}
}




void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], unsigned int tid[6], float density)
{
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - hdist;
	float maxy = centery + hdist;
	float minz = centerz - ddist;
	float maxz = centerz + ddist;
	LINEVERTEX3D Vertices[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z 1
	{	{	minx,	miny,	maxz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},		// TL

		// Far face - Facing z +1
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	maxy,	minz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Left	face -x
	{ {	minx,	miny,	minz,	colour },		// BL
		{	minx,	miny,	maxz,	colour },		// BR
		{	minx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Right Face +x
	{	{	maxx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	maxx,	maxy,	minz,	colour }},	// TL

		// Top Face +y
	{	{	minx,	maxy,	minz,	colour },		// BL
		{	maxx,	maxy,	minz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Bottom Face -y
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	miny,	maxz,	colour },		// TR
		{	minx,	miny,	maxz,	colour }},	// TL
	};
	float norms[][3] = {
		{ 0, 0, 1 },	// Near
		{ 0, 0, -1 }, // Far
		{ -1, 0, 0 }, // Left
		{ 1, 0, 0 }, // Right
		{ 0, 1, 0 }, // Top
		{ 0, -1, 0}, // Bottom
	};
	unsigned int size = 4;
	//for (int i = 0; i < 6*4; )

	float divisions = density;
	DWORD &c = colour;//Vertices[j].color;
	float c1[4] ={
		((float)RED(c)) / 255.0f,
	  ((float)GREEN(c)) / 255.0f,
	  ((float)BLUE(c)) / 255.0f,
	  ((float)ALPHA(c)) / 255.0f
	};
//	glColor4fv(c1);  

	int dirs[6] = { 0, 0, 1, 1, 2, 2 };
	bool flip[6] = { 1, 0, 0, 1, 0, 1 };
	/*
		0 - x, y 
		1 - x, y
		2 - y, z
		3 - y, z
		4 - x, z
		5 - x, z
	*/
// tcoords[6][2]
	float stepX = width / divisions;
	float stepY = height / divisions;
	float stepZ = depth / divisions;
	float x, y, z;
	int e = 0;
	int oldtid = -1;
	
	for (unsigned int i = 0; i < 6; i++)
//	unsigned int i = 3;
	{
		if (oldtid != tid[i])
		{
			glBindTexture(GL_TEXTURE_2D, tid[i]);
			oldtid = tid[i];
		}
		float sx = 0, sy = 0, sz = 0;
		
//		if (i == 3)
//			DebugText(EMSG, "tshift[%d]: %.2f, %.2f, %.2f", i, tshift[i][0], tshift[i][1], tshift[i][2]);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
	//	glScalef(tcoords[i][0], tcoords[i][1], 1);
	//	glTranslatef(tshift[i][0], tshift[i][1], tshift[i][2]);
//		glTranslatef(10, 10, 10);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
		glNormal3fv(norms[i]);
		

		if (dirs[i] == 0)
		{
			// Subdivide on X, Y plane
			z = Vertices[i][0].z;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx += stepX, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy += stepY)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy+stepY)) };
					
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
					}
				}
			}
		}
		else if (dirs[i] == 1)
		{
//			DebugText(EMSG, "Drawing Y,Z (1) plane!");
			// Subdivide on Y, Z plane
			x = Vertices[i][0].x;
			
			for (z = Vertices[i][0].z; z < Vertices[i][1].z; z += stepZ, sz+= stepZ, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy+=stepY)
				{
					double u[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz + stepZ)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy + stepY)) };
//					DebugText(EMSG, "u[0]:%.2f, v[0]:%.2f: u[1]:%.2f, v[1]:%.2f, flip:%d", u[0], v[0], u[1], v[1], flip[i]);
//					DebugText(EMSG, "minz: %.2f, maxz: %.2f", minz, maxz);
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
					}
				}
			}
		}
		else if (dirs[i] == 2)
		{
			// Subdivide on X, Z plane
//			DebugText(EMSG, "Drawing X,Z (2) plane!");
			y = Vertices[i][0].y;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx+=stepX, sz = 0)
			{
				for (z = Vertices[i][0].z; z < Vertices[i][2].z; z+= stepZ, sz+=stepZ)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz+stepZ)) };
					if (flip[i])
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y, z+stepZ);
					}
					else
					{
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
					}

				}
			}	
		}

	glEnd();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	}

}

void draw3DBox(float centerx, float centery, float centerz, 
			   float depth, float width, float height, DWORD colour,
			   float tcoords[6][2], float tshift[6][3], unsigned int basetid,
				 unsigned int tid[6], float density)
{
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - hdist;
	float maxy = centery + hdist;
	float minz = centerz - ddist;
	float maxz = centerz + ddist;
	LINEVERTEX3D Vertices[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z 1
	{	{	minx,	miny,	maxz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},		// TL

		// Far face - Facing z +1
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	maxy,	minz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Left	face -x
	{ {	minx,	miny,	minz,	colour },		// BL
		{	minx,	miny,	maxz,	colour },		// BR
		{	minx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	minz,	colour }},	// TL

		// Right Face +x
	{	{	maxx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	maxz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	maxx,	maxy,	minz,	colour }},	// TL

		// Top Face +y
	{	{	minx,	maxy,	minz,	colour },		// BL
		{	maxx,	maxy,	minz,	colour },		// BR
		{	maxx,	maxy,	maxz,	colour },		// TR
		{	minx,	maxy,	maxz,	colour }},	// TL

		// Bottom Face -y
	{	{	minx,	miny,	minz,	colour },		// BL
		{	maxx,	miny,	minz,	colour },		// BR
		{	maxx,	miny,	maxz,	colour },		// TR
		{	minx,	miny,	maxz,	colour }},	// TL
	};
	float norms[][3] = {
		{ 0, 0, 1 },	// Near
		{ 0, 0, -1 }, // Far
		{ -1, 0, 0 }, // Left
		{ 1, 0, 0 }, // Right
		{ 0, 1, 0 }, // Top
		{ 0, -1, 0}, // Bottom
	};
	unsigned int size = 4;
	//for (int i = 0; i < 6*4; )

	float divisions = density;
	DWORD &c = colour;//Vertices[j].color;
	float c1[4] ={
		((float)RED(c)) / 255.0f,
	  ((float)GREEN(c)) / 255.0f,
	  ((float)BLUE(c)) / 255.0f,
	  ((float)ALPHA(c)) / 255.0f
	};
//	glColor4fv(c1);  

	int dirs[6] = { 0, 0, 1, 1, 2, 2 };
	bool flip[6] = { 1, 0, 0, 1, 0, 1 };
	/*
		0 - x, y 
		1 - x, y
		2 - y, z
		3 - y, z
		4 - x, z
		5 - x, z
	*/
// tcoords[6][2]
	float stepX = width / divisions;
	float stepY = height / divisions;
	float stepZ = depth / divisions;
	float x, y, z;
	int e = 0;
	//int oldtid = -1;
	
	for (unsigned int i = 0; i < 6; i++)
	{
		//if (oldtid != tid[i])
		//{
		//	glBindTexture(GL_TEXTURE_2D, tid[i]);
		//	oldtid = tid[i];
		//}
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
//		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
		glBindTexture(GL_TEXTURE_2D, tid[i]);



		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, basetid);
		
		
		float sx = 0, sy = 0, sz = 0;
		
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glScalef(tcoords[i][0], tcoords[i][1], 1);
		glTranslatef(tshift[i][0], tshift[i][1], tshift[i][2]);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
		glNormal3fv(norms[i]);

//glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, &t0[0]);
//glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, &t1[0]);	

		if (dirs[i] == 0)
		{
			// Subdivide on X, Y plane
			z = Vertices[i][0].z;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx += stepX, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy += stepY)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy+stepY)) };
					
					if (flip[i])
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x+stepX, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x+stepX, y+stepY, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y+stepY, z);
/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
*/					}
					else
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y+stepY, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x+stepX, y+stepY, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x+stepX, y, z);
/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y+stepY, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
*/
}
				}
			}
		}
		else if (dirs[i] == 1)
		{
			// Subdivide on Y, Z plane
			x = Vertices[i][0].x;
			
			for (z = Vertices[i][0].z; z < Vertices[i][1].z; z += stepZ, sz+= stepZ, sy = 0)
			{
				for (y = Vertices[i][0].y; y < Vertices[i][2].y; y+= stepY, sy+=stepY)
				{
					double u[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz + stepZ)) };
					double v[] = { 1.0 / (height / sy), 1.0 / (height / (sy + stepY)) };
					if (flip[i])
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y+stepY, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x, y+stepY, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x, y, z+stepZ);
/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
						*/
					}
					else
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x, y, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x, y+stepY, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y+stepY, z);
						/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x, y+stepY, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y+stepY, z);
						*/
					}
				}
			}
		}
		else if (dirs[i] == 2)
		{
			// Subdivide on X, Z plane
//			DebugText(EMSG, "Drawing X,Z (2) plane!");
			y = Vertices[i][0].y;
			for (x = Vertices[i][0].x; x < Vertices[i][1].x; x += stepX, sx+=stepX, sz = 0)
			{
				for (z = Vertices[i][0].z; z < Vertices[i][2].z; z+= stepZ, sz+=stepZ)
				{
					double u[] = { 1.0 / (width / sx), 1.0 / (width / (sx+stepX)) };
					double v[] = { 1.0 / (depth / sz), 1.0 / (depth / (sz+stepZ)) };
					if (flip[i])
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x+stepX, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x+stepX, y, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y, z+stepZ);
						/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y, z+stepZ);
						*/
					}
					else
					{
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[0]);
						glVertex3f(x, y, z);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[0], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[0], v[1]);
						glVertex3f(x, y, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[1]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[1]);
						glVertex3f(x+stepX, y, z+stepZ);
						glMultiTexCoord2dARB(GL_TEXTURE0_ARB, u[1], v[0]); glMultiTexCoord2dARB(GL_TEXTURE1_ARB, u[1], v[0]);
						glVertex3f(x+stepX, y, z);
						/*
						glTexCoord2d(u[0],v[0]);	glVertex3f(x, y, z);
						glTexCoord2d(u[0],v[1]);	glVertex3f(x, y, z+stepZ);
						glTexCoord2d(u[1],v[1]);	glVertex3f(x+stepX, y, z+stepZ);
						glTexCoord2d(u[1],v[0]);	glVertex3f(x+stepX, y, z);
						*/
					}

				}
			}	
		}

	glEnd();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	}

}

