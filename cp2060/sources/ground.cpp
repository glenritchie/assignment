#ifdef WIN32
#include <windows.h>
#endif

#include "ground.h"
#include "resource_manager.h"


#include <GL/gl.h>
#include <GL/glu.h>

#include "model.h"
#include "3dmath.h"
#include "utils.h"
GroundObject::GroundObject(string newName, float newWidth, float newHeight, 
						 float newDensity) : Object(newName)
{
	width = newWidth;
	height = newHeight;
	density = newDensity;
	groundDisplayList = 0;
	//groundTexture = 0;
}
GroundObject::GroundObject(Object *o) : Object(o)
{
	//groundTexture = 0;
	groundDisplayList = 0;
	GroundObject *g = CastTo<GroundObject>(o);
	if (g == NULL)
	{
		//groundTexture = 0;
		groundDisplayList = 0;
		return;
	}
	//this->groundTexture = g->groundTexture;
	this->groundDisplayList = g->groundDisplayList;
	this->width = g->width;
	this->height = g->height;
	this->density = g->density;
	//this->textureName = g->textureName;
	for (int x = 0; x < 100; x++)
		for (int y = 0; y < 100; y++)
			hmap[x][y] = g->hmap[x][y];

	//GroundObject(o->getName(), 0, 0, 0);
	
	// Do nothing
}
void GroundObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of ground object params at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	do
	{
		file.clear();
		string text;
//		text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("width") == 0)
		{
			lineStream >> text >> this->width;
		}
//		else if (line.find("texture") == 0)
//		{
//			lineStream >> text >> this->textureName;
//		}
		else if (line.find("height") == 0)
		{
			lineStream >> text >> this->height;
		}
		else if (line.find("density") == 0)
		{
			lineStream >> text >> this->density;
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	file.clear();
}

VECTOR3 GroundObject::getnorm(int x, int y)
{
	if (x < 0)		x = 0;
	if (x > 99)		x = 99;
	if (y < 0)		y = 0;
	if (y > 99)		y = 99;
	VECTOR3 n = hnormals[x][y];
	return n;
}

void GroundObject::create()
{
	
	char uniqueName[100];
	sprintf(uniqueName, "%s:%s", getName().c_str(), "groundList");
	
	//ResourceManager->CreateResource(groundTexture, textureFile.c_str());
	CResourceManager::getInstance().createResource(groundDisplayList, uniqueName);
	//ground = new DisplayList();
	groundDisplayList->record(true);

	const float groundWidth = width;
	const float groundHeight = height;
	const float stepX = density;
	const float stepY = density;

	ifstream file("height");
	for (int y = 0; y < 100; y++)
	{
		string line;
		getline(file, line);
		for (int x = 0; x < 100; x++)
			hmap[x][y] = line[x] - '0';
	}
//	DebugText(EMSG, "h: %d", hmap[0][0]);

	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
	glTranslatef(-groundWidth/2, 0, -groundHeight/2);
	//glDisable(GL_TEXTURE_2D);
	texture->ActivateTexture(0);	
	
	//glBegin(GL_QUADS);
	int gx =0;
	int gy = 0;
	//glNormal3f(0, 1.0f, 0);
	for (float x = 0; x < groundWidth; x+=stepX, gx++, gy = 0)
	{
		for (float y = 0; y < groundHeight; y+=stepY, gy++)
		{
//					float u[] = { 1.0 / (groundWidth / x), 1.0 / (groundWidth / (x+stepX)) };
//					float v[] = { 1.0 / (groundHeight / y), 1.0 / (groundHeight / (y+stepY)) };
					// float z = ((float)((float)hmap[(int)x][(int)y] - 5)/2);
					float z1 = getHeight((int)x, (int)y);
					float z2 = getHeight((int)x, (int)(y+stepY));
					float z3 = getHeight((int)(x+stepX), (int)(y+stepY));
					float z4 = getHeight((int)(x+stepX), (int)y);

					
					VECTOR3 a = VECTOR3(x+stepX, z4, y) - VECTOR3(x, z1, y);
					VECTOR3 b = VECTOR3(x, z2, y+stepY) - VECTOR3(x, z1, y);

					VECTOR3 norm = Normalize(Cross(a, b) * -1);
					hnormals[gx][gy] = norm;
//					glNormal3f(norm.x, norm.y, norm.z);
										
//					glTexCoord2f(u[0], v[0]); glVertex3f(x, z1, y);
//					glTexCoord2f(u[0], v[1]); glVertex3f(x, z2, y+stepY);
//					glTexCoord2f(u[1], v[1]); glVertex3f(x+stepX, z3, y+stepY);
//					glTexCoord2f(u[1], v[0]); glVertex3f(x+stepX, z4, y);
		}
	}
	glBegin(GL_QUADS);
	gx = gy = 0;
	for (int x = 0; x < groundWidth; x+=stepX, gx++, gy = 0)
	{
		for (int y = 0; y < groundHeight; y+=stepY, gy++)
		{

					float u[] = { 1.0 / (groundWidth / x), 1.0 / (groundWidth / (x+stepX)) };
					float v[] = { 1.0 / (groundHeight / y), 1.0 / (groundHeight / (y+stepY)) };
					// float z = ((float)((float)hmap[(int)x][(int)y] - 5)/2);
					float z1 = getHeight((int)x, (int)y);
					float z2 = getHeight((int)x, (int)(y+stepY));
					float z3 = getHeight((int)(x+stepX), (int)(y+stepY));
					float z4 = getHeight((int)(x+stepX), (int)y);
					
					VECTOR3 a = VECTOR3(x+stepX, z4, y) - VECTOR3(x, z1, y);
					VECTOR3 b = VECTOR3(x, z2, y+stepY) - VECTOR3(x, z1, y);

					VECTOR3 norm = Cross(a, b) * -1;
					norm = Normalize(norm);
					/*
						These are offsets into which quad. For example.
						BR. Bottom Right. To get the normal for the bottom right vertex we need:
							The normal for the quad to the right of me
							The normal for the quad to the below of me
							The normal for the quad 1 right and 1 below me.
							The normal for myself. 
						Once we have these we add em together, divide em by 4 (to get avg) and normalize it.
						That gives us the normal for the Bottom Right(BR) vertex. The offsets just describe
						what offset to use for each point.
					*/
					int off[4][4][2] = {
						{/*BL*/ {0, 0}, {-1, 0}, { 0, -1},   {-1, -1}},
						{/*TL*/ {0, 0}, {-1, 0}, {-1,  1},   { 0,  1}},
						{/*TR*/ {0, 0}, { 0, 1}, { 1,  1},   { 1,  0}},
						{/*BR*/ {0, 0}, { 0, -1}, { 1, -1},   { 1, 0}},
						};
					VECTOR3 n[4];
					for (int i = 0; i < 4; i++)
					{	
						VECTOR3 tmpn(0,0,0);
						for (int o = 0; o < 4; o++)
						{
							VECTOR3 tn = getnorm((int)gx + off[i][o][0], (int)gy + off[i][o][1]);
							tmpn = tmpn + tn;
						}
						n[i] = tmpn / 4;
						n[i] = Normalize(n[i]);

						//DebugText(EMSG, "vnormal[%s]: x: %.2f, y: %.2f, z: %.2f", tt,n[i].x, n[i].y, n[i].z);
					}					
/*					// Draw normals
					glPushAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT );
					glDisable(GL_LIGHTING);
					glDisable(GL_TEXTURE_2D);
					glBegin(GL_LINES);
					glColor3f(1,1,1);
					glVertex3f(x, z1, y);				glVertex3f(x+n[0].x, z1+n[0].y, y+n[0].z);
					glVertex3f(x, z2, y+stepY);			glVertex3f(x+n[1].x, z2+n[1].y, (y+stepY)+n[1].z);
					glVertex3f(x+stepX, z3, y+stepY);	glVertex3f((x+stepX)+n[2].x, z3+n[2].y, (y+stepY)+n[2].z);
					glVertex3f(x+stepX, z4, y);			glVertex3f((x+stepX)+n[3].x, z4+n[3].y, y+n[3].z);
					glColor3f(0,1,1);
					
					glVertex3f(x+(stepX/2), (z1+z2+z3+z4)/4, y+(stepY/2));
					glVertex3f(x+(stepX/2)+norm.x, (z1+z2+z3+z4)/4 + norm.y, y+(stepY/2) + norm.z);
					glEnd();
					
					glPopAttrib();
*/
					glNormal3f(n[0].x, n[0].y, n[0].z); glTexCoord2f(u[0], v[0]); glVertex3f(x, z1, y);
					glNormal3f(n[1].x, n[1].y, n[1].z); glTexCoord2f(u[0], v[1]); glVertex3f(x, z2, y+stepY);
					glNormal3f(n[2].x, n[2].y, n[2].z); glTexCoord2f(u[1], v[1]); glVertex3f(x+stepX, z3, y+stepY);
					glNormal3f(n[3].x, n[3].y, n[3].z); glTexCoord2f(u[1], v[0]); glVertex3f(x+stepX, z4, y);
		}
	}
	glEnd();


	glPopAttrib();	
	glPopMatrix();
	groundDisplayList->stop();

}
void GroundObject::shapeDraw()
{
	if (groundDisplayList != 0)
	{
		groundDisplayList->display();
		return;
	}
	create();
}
float GroundObject::getHeight(int x, int y)
{
	//if (x > 100) return 0;
//	if (y > 100) return 0;
	if (x >= 100) x = 99;
	if (y >= 100) y = 99;
int x1 = x + 1;
	int x2 = x - 1;
	int y1 = y + 1;
	int y2 = y - 1;
	if (x1 < 0 || x1 >= 100) x1 = x;
	if (x2 < 0 || x2 >= 100) x2 = x;
	if (y1 < 0 || y1 >= 100) y1 = y;
	if (y2 < 0 || y2 >= 100) y2 = y;
	
 	float z1 = (float)(hmap[x][y] + hmap[x][y1] + hmap[x][y2] + hmap[x2][y] + hmap[x1][y]) / 5;
//	DebugText(EMSG, "x: %d, y: %d: z:%.2f", x, y, z1);
	return z1 - 5;

}

