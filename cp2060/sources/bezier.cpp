#ifdef WIN32
#include <windows.h>
#endif
#include "bezier.h"
#include "model.h"
#include <stdlib.h>
#include "utils.h"
#include <GL/gl.h>
#include <GL/glu.h>

BezierObject::BezierObject(Object *o) : Object(o)
{
	surfName = gluNewNurbsRenderer();
	density = 50;
}
BezierObject::~BezierObject()
{
	gluDeleteNurbsRenderer(surfName);
}
BezierObject::BezierObject(string newName) 
	: Object(newName)
{
	numControlPaths = 2;
	density = 50;
	//t = 0;
	// Do nothing.
}

void BezierObject::shapeDraw()
{

	int points = 6;
	int total = points * 3;
		
//	                          0.0, 1.0, 15, numControlPaths, &controlPoints[0][0][0]);
/*	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, numControlPaths, 
	                          0.0, 1.0, total, numControlPaths, &controlPoints[0][0][0]);
*/
//	glMapGrid2f(density, 0.0, 1.0, density, 0.0, 1.0);
//	glEvalMesh2(GL_LINE, 0, density, 0, density);
	
//  3, 3*DEGREE, &controlPoints[0][0][0], DEGREE, numControlPaths, GL_MAP2_TEXTURE_COORD_2);

	float knots[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };		
	int e = 0;
	e = glGetError();
	if (e != GL_NO_ERROR)
	{
		DebugText(EMSG, "before: nurbs: gl error: 0x%x", e);
	}

	
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	
	glFrontFace(GL_CW);
	extern bool wireframe;
	

/* Thank you Red BOOK for showing me this! */
	GLfloat texpts[2][2][2] = {
    {{1.0, 1.0}, {0.0, 1.0}},
		{{1.0, 0.0}, {0.0, 0.0}},
	};

		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
       0, 1, 4, 2, &texpts[0][0][0]);

	glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, DEGREE, 0.0, 1.0, 3*DEGREE,  numControlPaths, &controlPoints[0][0][0]);
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, DEGREE, 0.0, 1.0, 3*DEGREE,  numControlPaths, &controlPoints[0][0][0]);
	glMapGrid2f(density, 0.0, 1.0, density, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, density, 0, density);

/*	
	gluNurbsProperty(surfName, GLU_SAMPLING_TOLERANCE, density);

	if (wireframe)
		gluNurbsProperty(surfName, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	else
		gluNurbsProperty(surfName, GLU_DISPLAY_MODE, GLU_FILL);
			
	if (t != 0)
	{
		glEnable(GL_TEXTURE_2D);
		t->ActivateTexture(0);
	}
	else		
		glDisable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
//	glScalef(0.1, 0.23, 0.2);
//	glTranslatef(3.5, 2.0, 0);
	glMatrixMode(GL_MODELVIEW);
	
	gluBeginSurface(surfName);

	//	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
    //    0, 1, 4, 2, &texpts[0][0][0]);

	gluNurbsSurface(surfName, uKnotCount, uKnots, vKnotCount, vKnots, 
		                3, 3*DEGREE, &controlPoints[0][0][0], DEGREE, numControlPaths, GL_MAP2_TEXTURE_COORD_2);

//	gluNurbsSurface(surfName, uKnotCount, uKnots, vKnotCount, vKnots, 
//		                3, 3*DEGREE, &controlPoints[0][0][0], DEGREE, numControlPaths, GL_MAP2_TEXTURE_COORD_2);
	gluNurbsSurface(surfName, uKnotCount, uKnots, vKnotCount, vKnots, 
		                3, 3*DEGREE, &controlPoints[0][0][0], DEGREE, numControlPaths, GL_MAP2_NORMAL);
	gluNurbsSurface(surfName, uKnotCount, uKnots, vKnotCount, vKnots, 
		                3, 3*DEGREE, &controlPoints[0][0][0], DEGREE, numControlPaths, GL_MAP2_VERTEX_3);
	gluEndSurface(surfName);	

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
*/	
	//gluDeleteNurbsRenderer(surfName);
	e = glGetError();
	if (e != GL_NO_ERROR)
	{
		DebugText(EMSG, "nurbs: gl error: 0x%x", e);
	}
	if (selectedObject == this)
	{
		// Draw control points
		glDisable(GL_LIGHTING);
		glPointSize(5);
		glBegin(GL_POINTS);
		int k = 0;
		for (k = 0; k < numControlPaths; k++)
			for (int j = 0; j < DEGREE; j++)
			{
				glColor3f(0.0, 0.0, 1.0);
				glVertex3fv(controlPoints[k][j]);
			}
		glEnd();
		// Draw lines conecting the points
		for (k = 0; k < numControlPaths; k++)
		{
			glBegin(GL_LINE_STRIP);	
			for (int j = 0; j < DEGREE; j++)
				glVertex3fv(controlPoints[k][j]);
			glEnd();	
		}
	}
	glPopAttrib();


//	GLUquadric *q = gluNewQuadric();
//	gluSphere(q, 1, 20, 20);
//	gluDeleteQuadric(q);
}
void BezierObject::readControlPath(ifstream &file, unsigned int index, unsigned int &lineNumber, unsigned int &level)
{
	unsigned int cIndex = 0;
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of bezier path at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	do
	{
		file.clear();
		string text;
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("controlPoint") == 0)
		{
			if (cIndex >= DEGREE)
			{
				cout << "Too many control points! Max of " << DEGREE << endl;
				continue;
			}

			lineStream >> text >> this->controlPoints[index][cIndex][0] >>
													  this->controlPoints[index][cIndex][1] >>
													  this->controlPoints[index][cIndex][2];

			cIndex++;											
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
void BezierObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level) 
{  
	unsigned int controlPathIndex = 0;
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of bezier parms at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	do
	{
		file.clear();
		string text;
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("controlPath") == 0)
		{
		//	cout << "Reading controlPath " << controlPathIndex << endl;
			readControlPath(file, controlPathIndex, lineNumber, level);
			controlPathIndex++;
		}
		else if (line.find("density") == 0)
		{
			lineStream >> text >> this->density;
		}
		else if (line.find("uKnotCount") == 0)
		{
			lineStream >> text >> this->uKnotCount;
		}
		else if (line.find("vKnotCount") == 0)
		{
			lineStream >> text >> this->vKnotCount;
		}	
		else if (line.find("uKnots") == 0)
		{
			lineStream >> text;
			for (int i = 0; i < uKnotCount; i++)
				lineStream >> this->uKnots[i];
		}
		else if (line.find("vKnots") == 0)
		{
			lineStream >> text;
			for (int i = 0; i < vKnotCount; i++)
				lineStream >> this->vKnots[i];
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}

	}
	while (line.find("}") == -1);
	level--;
	file.clear();
	numControlPaths = controlPathIndex;
//	cout << "loadnumcon:" << numControlPaths << endl;	
}

