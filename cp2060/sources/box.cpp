#include "box.h"
#include "draw_box.h"
#include "utils.h"
#include "3dmath.h"
#include <math.h>
#include <time.h>
// #include "light.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "model.h"

/** The amount of ambient light applied to the entire scene */
unsigned int ambientLight = 5;
/** The maximum distance of all lights */
float maxDistance = 45;
/** A ptr to the world root node */
extern Object *world;
/** Are we doing some debug testing ?*/
bool debugRay = false;
/** Are we ray-tracing at the moment? */
bool beginLight = false;
/** Has the user forced the size of the lightmaps to a specific size? */
bool forceSize = false;
/** Does the user want to display the trace as it progresses */
bool showtrace = false;
/** This is the face ID on the object that was collided with - used for optimization*/
int lastFace = -1;
/** This is a special array that says whether the normals should be flipped. This SUCH AN 'UGLY' HACK, but i can't be bothered fixing it */
bool flip[6] = { 1, 0, 1, 0, 0, 1 };		
/** The number of faces tested */
int facesTested = 0;
/** The default light map size */
int defaultLightMapSize = 32;
/** The maximum possible size. A lightmap will be scaled to fit this size if it exceeds it */
int maxLightMapSize = 32;

Object* BoxObject::subSphereCollide( const VECTOR3 &center, const float &radius, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal)
{
	bool weAreClosest = false;
	// Test to see if this box collided with the given sphere

	if (doPoly)
		genFaces();

	const VECTOR3 &point = center;
	for (int i = 0; i < 6; i++)
	{
		float pDist;
		VECTOR3 intersection;
		
		// Use the 3dmath routine i borrowed to test one triangle of this face
		bool collide = false;
		collide = SpherePolygonCollision(vPoly1[i], (VECTOR3 &)center, 3, radius, pDist, intersection);
		// If we didn't collide then test the other triangle
		if (!collide)
			if (SpherePolygonCollision(vPoly2[i], (VECTOR3 &)center, 3, radius, pDist, intersection) == false)
				continue;
		// We got a hit! Check to see if this is the closest distance
		facesTested++;
		if ((pDist <= radius) && (pDist < smallestDistance))
		{
			weAreClosest = true;
			smallestDistance = pDist;
			faceNormal = norm[i];
			lastFace = i;
			out = intersection;
		}
	}
	if (weAreClosest)
		return this;
	return NULL;
}
void BoxObject::genFaces()
{
	doPoly = false;		// No need to do this again!
	float centerx = 0, centery = 0, centerz = 0;
	// Create a polygon out my planes
	typedef struct LINEVERTEX3D { double x; double y; double z; } LINEVERTEX3D;
	float hdist = height / 2;
	float wdist = width / 2;
	float ddist = depth / 2;
	float minx = centerx - wdist;
	float maxx = centerx + wdist;
	float miny = centery - hdist;
	float maxy = centery + hdist;
	float minz = centerz - ddist;
	float maxz = centerz + ddist;

	LINEVERTEX3D v[6][4] =	{
		// x, y, z, colour
		// Near Face - Facing z-1
		{	
			{	minx,	maxy,	maxz },		// TL
			{	maxx,	maxy,	maxz },		// TR
			{	maxx,	miny,	maxz },		// BR
			{	minx,	miny,	maxz },		// BL

		},	

		// Far face - Facing z+1
		{
			{	minx,	miny,	minz },		// BL
			{	maxx,	miny,	minz },		// BR
			{	maxx,	maxy,	minz },		// TR
			{	minx,	maxy,	minz },		// TL
		},	

		// Left	face -x
			{ 
				{	minx,	maxy,	minz },		// TL
				{	minx,	maxy,	maxz },		// TR
				{	minx,	miny,	maxz },		// BR
				{	minx,	miny,	minz }},		// BL

				// Right Face +x
				{	{	maxx,	miny,	minz },		// BL
				{	maxx,	miny,	maxz },		// BR
				{	maxx,	maxy,	maxz },		// TR
				{	maxx,	maxy,	minz }},	// TL

				// Top Face +y
				{	{	minx,	maxy,	minz },		// BL
				{	maxx,	maxy,	minz },		// BR
				{	maxx,	maxy,	maxz },		// TR
				{	minx,	maxy,	maxz }},	// TL

				// Bottom Face -y
				{	
					{	minx,	miny,	maxz },		// TL
					{	maxx,	miny,	maxz },		// TR
					{	maxx,	miny,	minz },		// BR
					{	minx,	miny,	minz }},	// BL

	};
	// Work out the bounding sphere by taking the largest distance from the center to each corner of the box
	radius = -1;
	VECTOR3 ce(0,0, 0);
	VECTOR3 corners[] = {
		VECTOR3(minx, miny, minz), VECTOR3 (minx, miny, maxz),
			VECTOR3(minx, maxy, minz), VECTOR3 (minx, maxy, maxz),
			VECTOR3(maxx, miny, minz), VECTOR3 (maxx, miny, maxz),
			VECTOR3(maxx, maxy, minz), VECTOR3 (maxx, maxy, maxz)
	};
	for (int i = 0; i < 8; i++)
	{
		double dist = Distance(ce, corners[i]);
		if (dist > radius)
			radius = dist;
	}

	// Get the matrix for this object into 'm'
	MATRIX m = findObjectWorldCoords(hx, hy, hz);

	for (int i = 0; i < 6; i++)
	{
		// Create a triangle from the 4 points
		vPoly1[i][0] = VECTOR3(v[i][0].x, v[i][0].y, v[i][0].z); 
		vPoly1[i][1] = VECTOR3(v[i][1].x, v[i][1].y, v[i][1].z); 
		vPoly1[i][2] = VECTOR3(v[i][2].x, v[i][2].y, v[i][2].z);

		// Create another triangle from the 4 points
		vPoly2[i][0] = VECTOR3(v[i][0].x, v[i][0].y, v[i][0].z);
		vPoly2[i][1] = VECTOR3(v[i][2].x, v[i][2].y, v[i][2].z); 
		vPoly2[i][2] = VECTOR3(v[i][3].x, v[i][3].y, v[i][3].z);

		// The above polys are in object co-ords. This is useless to us. We need em in world co-ords. So..
		// We take each point of the poly's and transform them by the matrix we got earlier in 'm'
		for (int p = 0; p < 3; p++)
		{
			double x1 = (vPoly1[i][p].x * m(0,0)) + (vPoly1[i][p].y * m(1,0)) + (vPoly1[i][p].z * m(2,0)) + m(3,0);
			double y1 = (vPoly1[i][p].x * m(0,1)) + (vPoly1[i][p].y * m(1,1)) + (vPoly1[i][p].z * m(2,1)) + m(3,1);
			double z1 = (vPoly1[i][p].x * m(0,2)) + (vPoly1[i][p].y * m(1,2)) + (vPoly1[i][p].z * m(2,2)) + m(3,2);
			vPoly1[i][p].x = x1;
			vPoly1[i][p].y = y1;
			vPoly1[i][p].z = z1;
		}
		for (int p = 0; p < 3; p++)
		{
			double x1 = (vPoly2[i][p].x * m(0,0)) + (vPoly2[i][p].y * m(1,0)) + (vPoly2[i][p].z * m(2,0)) + m(3,0);
			double y1 = (vPoly2[i][p].x * m(0,1)) + (vPoly2[i][p].y * m(1,1)) + (vPoly2[i][p].z * m(2,1)) + m(3,1);
			double z1 = (vPoly2[i][p].x * m(0,2)) + (vPoly2[i][p].y * m(1,2)) + (vPoly2[i][p].z * m(2,2)) + m(3,2);
			vPoly2[i][p].x = x1;
			vPoly2[i][p].y = y1;
			vPoly2[i][p].z = z1;
		}

		// Get the normals of this face and also work out the plane equationn for this face.
		VECTOR3 a = vPoly1[i][2] - vPoly1[i][0]; 
		VECTOR3 b = vPoly1[i][1] - vPoly1[i][0]; 
		norm[i] = Cross(a,b);
		planes[i][0] = norm[i].x; planes[i][1] = norm[i].y; planes[i][2] = norm[i].z;
		planes[i][3] = -Dot(norm[i], vPoly1[i][0]);
		norm[i] = Normalize(norm[i]);
	}
}

Object *BoxObject::subRayCollide(VECTOR3 point, VECTOR3 target, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal)
{

	int start = 0;
	int end = 6;
	bool weAreClosest = false;
	
	if (/*beginLight &&*/ doPoly)
		genFaces();

	// If this object is not static AND we are testing a ray against it, then DO NOT test it
	if (!this->isStatic() && beginLight)
		return NULL;

	// Draw the bounding sphere
	// Comment out the 0 to make it work. disabled and only enable for debug use
	if (0 && debugRay)
	{
		glDisable(GL_LIGHTING);
		glColor3f(1,1,1);
		// Draw collide
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 6; i++)
		{
			glVertex3d(vPoly1[i][0].x, vPoly1[i][0].y, vPoly1[i][0].z);
			glVertex3d(vPoly1[i][1].x, vPoly1[i][1].y, vPoly1[i][1].z);
			glVertex3d(vPoly1[i][2].x, vPoly1[i][2].y, vPoly1[i][2].z);
			glVertex3d(vPoly2[i][0].x, vPoly2[i][0].y, vPoly2[i][0].z);
			glVertex3d(vPoly2[i][1].x, vPoly2[i][1].y, vPoly2[i][1].z);
			glVertex3d(vPoly2[i][2].x, vPoly2[i][2].y, vPoly2[i][2].z);
		}
		glEnd();	
	}
	
	// Make a ray vector
	VECTOR3 ray;
	ray = target - point;
	ray = Normalize(ray);
	// Perfom a sphere intersection test with the ray and this object
	// Equation from Page 603 - Computer Graphics with OpenGL - Hearn Baker - 3rd edition
	double &r = radius;
	VECTOR3 ap = VECTOR3(hx, hy, hz) - point;
	VECTOR3 b = ap - ray * (Dot(ray, ap));
	double mb = pow(Magnitude(b), 2);
	double r2 = pow(r, 2);
	double dis = r2 - mb;
	double c = sqrt(dis);
	double a = Dot(ray, ap);
	//	float s1 = a + c;
	//	float s2 = a - c;
	// If the discriminant is less than zero than the ray did not intesect
	// I think we then take the s(1 or 2) that is the smallest and 
	// plug that into the ray formula we get the intersection point
	// eg. VECTOR3 int = point + (ray * s);
	if (dis < 0)
		return NULL;


	VECTOR3 vLine[2];
	vLine[0] = point;
	vLine[1] = target;
	VECTOR3 ret;
	for (int i = start; i < end; i++)
	{
		// The next two tests make sure that both point and target are NOT on the same side of the plane
		// If they are then they don't intersect.
		
		// Test to see if the point we want is in form of the plane
		double pDist = planes[i][0] * point.x + planes[i][1] * point.y + 
			planes[i][2] * point.z + planes[i][3];
		if (pDist < 0.0)
			continue;
		// Test to see if the target point is further than the light range	
		double pDist2 = planes[i][0] * target.x + planes[i][1] * target.y + 
			planes[i][2] * target.z + planes[i][3];
		if (pDist2 > maxDistance)							
			continue;

		facesTested++;
		bool collision = false;
		VECTOR3 n1 = norm[i];
		VECTOR3 n2 = norm[i];
		// Does the object intersect the first poly
		int cf = -1;
		//ret = PointonTriangle(vPoly1[i], vLine, 3, norm[i]);
		ret = PointonTriangle(vPoly1[i], vLine, 3, n1);
		if (!ret.IsZero())
		{
			collision = true;
			cf = 0;
		}
		else
		{
			// It didn't intersect so we test the other poly
			//ret = PointonTriangle(vPoly2[i], vLine, 3, n2);
			ret = PointonTriangle(vPoly2[i], vLine, 3, n2);
			if (!ret.IsZero())
			{
				collision = true;
				cf = 1;
			}
		}
		// If there was a collsion
		if (collision)
		{
			// Find the distance
			double dist = Distance(ret, vLine[0]);
			if (dist < maxDistance)
			{
				if (dist < smallestDistance)
				{
					weAreClosest = true;
					smallestDistance = dist;
					faceNormal = norm[i];
					lastFace = i;
					out = ret;
				}
			}
		}
	}
	if (weAreClosest)
		return this;
	// If there was no collision OR we were not the closest object then return NULL for no collision.	
	return NULL;
}


void BoxObject::generateLightMaps(VECTOR3 lights[], unsigned int lightCount)
{
	
	extern void generateLightMapsDisplay();
	extern Object *rObject;
	extern int rayTracingLight;
	extern int rayTracingFace;
	extern float objectProgress;
	extern float faceProgress;
	extern int etw, eth;
	rObject = this;
	objectProgress = 0;
	faceProgress = 0;

	float hx, hy, hz;
	MATRIX m = findObjectWorldCoords(hx, hy, hz);
	int theight = defaultLightMapSize;
	int twidth = defaultLightMapSize;
	
	if (theight > maxLightMapSize) theight = maxLightMapSize;
	if (twidth > maxLightMapSize) twidth = maxLightMapSize;

	eth = theight;
	etw = twidth;
	DebugText(EMSG, "Doing %s: x:%d, y:%d", getName().c_str(), twidth, theight);


	clock_t startTime = clock();
	unsigned int bpp = 24;
	unsigned int bytesPerPixel = bpp/8;
	unsigned int imageSize	= twidth*theight*bytesPerPixel;

	float totalProgress = twidth * theight * lightCount * 6;
	float totalFaceProgress = twidth * theight * lightCount;
	float tProgress = 0;

	facesTested = 0;
	// For each face!
	for (int face = 0; face < 6; face ++)
	{
		float fProgress = 0;
		rayTracingFace = face;

		clock_t fstartTime = clock();
		//		unsigned char *baseTexture = NULL;
		VECTOR3 **p = (VECTOR3**)new VECTOR3[twidth];
		int i = 0;	
		for (i = 0; i < twidth; i++)
			p[i] = new VECTOR3[theight];

		if (imageData[face] == NULL)
		{
			imageData[face] = new GLubyte[imageSize];		
			for(i=0; i<(int)imageSize; i++)		// Loop Through The Image Data
				imageData[face][i] = ambientLight;
			extern unsigned char *rColor;
			rColor = imageData[face];
		}
		
		/*********************
		
			Get a matrix of points on the box. These points represent a grid that divides up the plane.
			We take a plane. We then divide it into equal divisions(twidth) and return them into p.
		 	This will give a whole heap of points (twidth*twidth) to be exact. Each one of these points will have a ray fired
			at it from each light. If it hits then the pixel that corresponds to the point in imageData ary will be increased by 
			a value that is determined by the distance of that point to the light. If the ray collided with an object then
			that pixel will not be set for that light and will stay the ambient color. This happens for each face and each light,
			so that each light will apply more light onto each point.
		
		*******************/
		getPointsOnFaceOfBox(0,0,0, depth, width, height, (float)twidth, face, p);
		// Transform all the co-ords into world-coords
		for (int y = 0; y < theight; y++)
		{
			for (int x = 0; x < twidth; x++)
			{
				VECTOR3 &point = p[x][y];

				double tx = (point.x * m(0,0)) + (point.y * m(1,0)) + (point.z * m(2,0)) + m(3,0);
				double ty = (point.x * m(0,1)) + (point.y * m(1,1)) + (point.z * m(2,1)) + m(3,1);
				double tz = (point.x * m(0,2)) + (point.y * m(1,2)) + (point.z * m(2,2)) + m(3,2);

				point.x = tx;
				point.y = ty;
				point.z = tz;
			}
		}

		int a = 0;
		VECTOR3 out;
		VECTOR3 faceNormal(0,0,0);
		Object *o = NULL;

		// Calc normal for this face
		VECTOR3 pNorm[3] = { p[0][0], p[1][1], p[1][0] };
		
		VECTOR3 p1 = pNorm[1] - pNorm[0]; 
		VECTOR3 p2 = pNorm[2] - pNorm[0]; 
		VECTOR3 n = Cross(p1, p2);
		// My evil hack!!
		if (flip[face]) n = n * -1;

		// Work out the plane for this face
		double plane[4];
		plane[0] = n.x; plane[1] = n.y; plane[2] = n.z;
		plane[3] = -Dot(n, pNorm[0]);

		// For each light do this face
		for (unsigned int l = 0; l < lightCount; l ++)
		{
			rayTracingLight = l;	// Progress msg variable
			
			// Total index into the imageData array
			a = 0;
			
			// Is this light on the front side of the plane.
			// Note there is no point lighting it if the light can't see it!
			double pDist = plane[0]*lights[l].x + plane[1]*lights[l].y +
				plane[2]*lights[l].z + plane[3];
			if (pDist < 0.0)
			{	
				// If we are showing the trace then make sure we keep the progress data up-to-date
				if (showtrace)
				{
					fProgress += theight * twidth;
					tProgress += theight * twidth;
					objectProgress = tProgress / totalProgress;
					faceProgress = fProgress / totalFaceProgress;
				}
				// Skip the rest of processing and go onto the next light
				continue;
			}
			// For each point in the lightmap (x,y)
		for (int y = 0; y < theight; y++)
			{
				for (int x = 0; x < twidth; x++, a+=3)	// Each pixel has 3 colours so 'a' is increased coz imageData is made of RGB colours
				{
					VECTOR3 &point = p[x][y];
					if (showtrace)	
					{
						// If we are showing the trace then make sure we keep the progress data up-to-date
						fProgress += 1;
						tProgress += 1;

						objectProgress = tProgress / totalProgress;
						faceProgress = fProgress / totalFaceProgress;
					}

					// Make a ray pointing from the light to the point
					VECTOR3 ray = point - lights[l];
					// Normalize the ray
					ray = Normalize(ray);
					// Don't actually try to get to the point! Try only to go as far as the light can reach 
					VECTOR3 tpoint = lights[l] + (ray * maxDistance);
					// Reset the last face
					lastFace = -1;	
					float smallestDistance = 99999;
					// Try the collision!
					o = world->rayCollide(lights[l], tpoint, out, smallestDistance, faceNormal);
					// If we hit an object and the face of that object that was hit was this face. Then light the pixel!
					if (o != NULL && lastFace == face)
					{
						// We got light! Set the texel to a color!
						// We are really looking for a collision with ourselves. So if it's not goto the next light
						if (o->getName() != getName())
						{
							continue;
						}

						// Work out a linear lightmap based on the distance to the light	
						int col = 0;
						float r = 0;
						if (smallestDistance > maxDistance)
							col = 0;
						else
						{
							r = (smallestDistance / maxDistance);
							col = 255 - int(255.0 * r);
						}
						// Get the current value in the light map ( red component.. but it dosn't matter which coz they get set the same )
						int val = imageData[face][a+1]; 
						val += col;
						if (val >= 255) val = 255;
						imageData[face][a+0] = val;
						imageData[face][a+1] = val;
						imageData[face][a+2] = val;					
					} // end if
				}	// end x
				// If we want to show the trace then display it on the screen
				if (showtrace)
					generateLightMapsDisplay();
			} // end y
		} // end l
		a = 0;

/*		for (int y = 0; y < theight; y++)
		{
			for (int x = 0; x < twidth; x++, a+=3)	// Each pixel has 3 colours so 'a' is increased coz imageData is made of RGB colours
			{
				cout << ((imageData[4][a] == 0) ? "0" : "1");
			}
			cout << endl;
		}
	*/
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		// Generate an id	
		if (tid[face] != 0)
			glDeleteTextures(1, &tid[face]);
		glGenTextures(1, &tid[face]);					// Generate OpenGL texture IDs

		glBindTexture(GL_TEXTURE_2D, tid[face]);

		// Make the light map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, twidth, theight, GL_RGB, GL_UNSIGNED_BYTE, imageData[face]);
		
		glPopAttrib();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);	

		clock_t fendTime = clock();
		clock_t diff = fendTime - fstartTime;
	}
	clock_t endTime = clock();
	clock_t diff = endTime - startTime;
	float secs = ((float)diff/(float)CLOCKS_PER_SEC);
	DebugText(EMSG, "'%s' took %.2f secs: ft:%d", getName().c_str(), secs, facesTested);
}

BoxObject::BoxObject(string newName, float newWidth, float newDepth, float newHeight) 
: width(newWidth), height(newHeight), depth(newDepth), Object(newName)
{
	// Do nothing.
}
void BoxObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{  
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of box parms at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	//cout << "Reading box types num:" << lineNumber << endl;
	string text;
	do
	{
		file.clear();
		string text;
		//text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("depth") == 0)
		{
			lineStream >> text >> this->depth;
		}
		else if (line.find("faceTexShift") == 0)
		{
			char ascFace[2] = {
				line[strlen("faceTexShift")],
					'\0'
			};
			int face = atoi(ascFace);
			lineStream >> text >> faceTexShift[face][0] >> faceTexShift[face][1] >> faceTexShift[face][2];
		}
		else if (line.find("faceTexScale") == 0)
		{
			char ascFace[2] = {
				line[strlen("faceTexScale")],
					'\0'
			};
			int face = atoi(ascFace);
			lineStream >> text >> faceTexScale[face][0] >> faceTexScale[face][1];
		}
		else if (line.find("width") == 0)
		{
			lineStream >> text >> this->width;
		}
		else if (line.find("density") == 0)
		{
			lineStream >> text >> this->density;
		}
		else if (line.find("height") == 0)
		{
			lineStream >> text >> this->height;
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	//cout << "End reading box types: level: " << level << endl;
	file.clear();
}
void BoxObject::shapeDraw()
{
	unsigned int colour = COLOR_RGBA(255, 255, 255, 255);
	DWORD colour2[4] = {
		COLOR_RGBA(255, 255, 255, 255),
			COLOR_RGBA(255, 255, 255, 255),
			COLOR_RGBA(255, 255, 255, 255),
			COLOR_RGBA(255, 255, 255, 255),
	};
	//glDisable(GL_LIGHTING);
	//DebugText(EMSG, "Drawing object: %s: face3: x:%.2f, y:%.2f,", getName().c_str(), faceTexShift[3][0], faceTexShift[3][1]);
	if (depth == 0)
		draw2DBox((height/2), -(width/2), (width/2),-(height/2), colour2);
	else if (height == 0)
		draw2DBox((depth/2), -(width/2), (width/2), -(depth/2), colour2);
	else if (width == 0)
		draw2DBox((height/2),-(depth/2),(depth/2),-(height/2), colour2);
	else 
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		unsigned int basetid = 0;

		if (texture)
			basetid = texture->getID();

		unsigned int tids[6] = { 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 6; i++)
			if (/*tid[i] == 0*/texture)
				tids[i] = texture->getID();
		//			else
		//		tids[i] = tid[i];
		extern bool multitexture_availiable;
		if (multitexture_availiable)
			draw3DBox(0,0,0, depth, width, height, colour, faceTexScale, faceTexShift, basetid, tid, density);
		else 		
			draw3DBox(0,0,0, depth, width, height, colour, faceTexScale, faceTexShift, tids, density);
		glPopAttrib();	
	}
}
BoxObject::BoxObject(Object *o) : Object(o) 
{
	for (int i = 0; i < 6; i++)
	{
		faceTexScale[i][0] = faceTexScale[i][1] = 1;
		faceTexShift[i][0] = faceTexShift[i][1] = 0;
		tid[i] = 0;
		imageData[i] = NULL;
		doneNorms[i] = false;
	}
	density = 1;
	doPoly = true;
	//	imageData = NULL;
}

