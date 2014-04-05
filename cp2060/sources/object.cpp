#include "object.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stack>
#include "camera.h"
extern CCamera camera;
void Object::printTree(unsigned int indent)
{
	char msg[1000];
	int i = 0;
	for (i = 0; i < indent; i++)
	{
		if (!(i % 2))
			msg[i] = '|';
		else msg[i] = ' ';	
	}
	sprintf(msg+i,"+%s", getName().c_str());
	indent+=2;
	DebugText(EMSG, msg);
	for (i = 0; i < children.size(); i++)
		children[i]->printTree(indent);
}

Object::Object(Object *o)
{
	texture = o->texture;
	textureFile = o->textureFile;
	staticObject = o->staticObject;

	transparent = o->transparent;
	lighted = o->lighted;
	twoSided = o->twoSided;
	this->children = o->children;
	for (int i = 0 ; i < 3; i++)
	{
		limits[i] = o->limits[i];
		pivot[i] = o->pivot[i];
		scale[i] = o->scale[i];
		origin[i] = o->origin[i];
		accel[i] = o->accel[i];
		vel[i] = o->vel[i];
		fixedAngles[i] = o->fixedAngles[i];
		for (int j = 0; j < FRAMES; j++)
		{
			originOffsets[j][i] = o->originOffsets[j][i];
			angles[j][i] = o->angles[j][i];
		}
	}
	name = o->name;
	parent = o->parent;

	for (int i = 0; i < FRAMES; i++)
	{
		doneMatrix[i] = o->doneMatrix[i];
	
		drawMatrix[i] = o->drawMatrix[i];
		pivotMatrix[i] = o->pivotMatrix[i];
		childMatrix[i] = o->childMatrix[i];
	}
	doneWorldMatrix = o->doneWorldMatrix;
}
void Object::resetMatrices()
{
	for (int i = 0; i < FRAMES; i++)
	{
		doneMatrix[i] = false;
		drawMatrix[i].identity();
		pivotMatrix[i].identity();
		childMatrix[i].identity();
	}
	doneWorldMatrix = false;
	for (int i = 0; i < FRAMES; i++) doneMatrix[i] = false;
}
Object::Object(string newName) 
{
	lighted = true;
	texture = 0;
	transparent = 0.0;
	twoSided = false;
	name = newName;
	parent = NULL; 
	origin[0] = origin[1] = origin[2] = 0;
	scale[0] = scale[1] = scale[2] = 1;
	limits[0] = limits[1] = limits[2] = 0;
	fixedAngles[0] = fixedAngles[1] = fixedAngles[2] = 0.0;
	vel[0] = vel[1] = vel[2] = 0;
	accel[0] = accel[1] = accel[2] = 0;
	nullAngles();
	nullOriginOffsets();
	staticObject = true;

	resetMatrices();

	pivot[0] = pivot[1] = pivot[2] = 0;
//	for (unsigned int i = 0 ;i < MAX_PARAMS; i++)
//		param[i] = 0;
}

  void Object::nullAngles()
	{
		for (unsigned int i = 0; i < FRAMES; i++)
			angles[i][0] = angles[i][1] = angles[i][2] = 0;
	}
	/** Set the origin offsets to 0 */
	void Object::nullOriginOffsets()
	{
		for (unsigned int i = 0; i < FRAMES; i++)
			originOffsets[i][0] = originOffsets[i][1] = originOffsets[i][2] = 0;
	}
	/** Perfrom a recursive call to null all angles and origin offsets 
	 * on this nodes children */
	void Object::nullChildren()
	{
		resetMatrices();
		nullOriginOffsets();
		nullAngles();
		for (unsigned int i = 0; i < children.size(); i++)
		{
			children[i]->nullChildren();
		}
	}
	/* Desconstruct the object */
/*
	Object::~Object()
	{
		for (unsigned int i = 0; i < children.size(); i++)
			delete children[i];
		children.clear();	
	}
*/	
	void Object::generateMatrix(const unsigned int frame)
	{
		if (frame > FRAMES) return;
		float x, y, z;	
		
		// Translate to where we want to be 
		//glTranslatef(origin[0], origin[1], origin[2]);
		// Translate offset
		x = interp(originOffsets[frame][0], originOffsets[(frame+1) % FRAMES][0]);
		y = interp(originOffsets[frame][1], originOffsets[(frame+1) % FRAMES][1]);
	 	z = interp(originOffsets[frame][2], originOffsets[(frame+1) % FRAMES][2]);
//		glTranslatef(x, y, z);

		// Move back FROM the pivot point
//		glTranslatef(pivot[0], pivot[1], pivot[2]);
		MATRIX t1, t2;
		MATRIX r1, r2;
		t1.translate(
					origin[0] + x + pivot[0], 
					origin[1] + y + pivot[1], 
					origin[2] + z + pivot[2]);
		// Interpolate angles
		x = interp(angles[frame][0], angles[(frame+1) % FRAMES][0]);
		y = interp(angles[frame][1], angles[(frame+1) % FRAMES][1]);
	 	z = interp(angles[frame][2], angles[(frame+1) % FRAMES][2]);
		// Perfom rotations backwards ( Z, Y, X )
		//glRotatef(z, 0, 0, 1);
		//glRotatef(y, 0, 1, 0);
		//glRotatef(x, 1, 0, 0);
		
		/**********
		Mon, 4th, Oct 04 
		DAMMIT! This causes NASTY gimbal lock problems!!! looking for an alternative.
		
		MATRIX ra, rb, rc;
		ra.rotate_x(x);
		rb.rotate_y(y);
		rc.rotate_z(z);
		r1 = ra * rb * rc;
		*/	
		MATRIX ra, rb, rc;
		ra.rotate_x(x);
		rb.rotate_y(y);
		rc.rotate_z(z);
		r1 = rc * rb * ra;
	
		
		pivotMatrix[frame] = t1 * r1;	
		//r1.rotate_angles(-x, y, z);

		// Translate TO pivot point
		//glTranslatef(-pivot[0], -pivot[1], -pivot[2]);
		t2.translate(-pivot[0], -pivot[1], -pivot[2]);
		
		//childDraw = t2 * r1 * t1;
		childMatrix[frame] = t1 * r1 * t2;
		// Rotate object to correct orientation		
		r2.rotate_angles(fixedAngles[0], fixedAngles[1], fixedAngles[2]);
//		r2.scale_xyz(scale[0], scale[1], scale[2]);

//		drawMatrix[frame] = childMatrix[frame];
		drawMatrix[frame] = childMatrix[frame] * r2;
//		glPushMatrix();
//		glRotatef(fixedAngles[2], 1, 0, 0);
//		glRotatef(fixedAngles[1], 0, 1, 0);
//		glRotatef(fixedAngles[0], 0, 0, 1);
//		glPushMatrix();
//			glScalef(scale[0], scale[1], scale[2]);
//				shapeDraw();
//			glPopMatrix();
//		glPopMatrix();
		
		
		// Draw our children
		for (unsigned int i = 0 ; i < children.size(); i++)
			children[i]->generateMatrix(frame);
		
		doneMatrix[frame] = true;
		//glPopMatrix();

	}
	void Object::drawPivot()
	{
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(-2, 0, 0);
			glVertex3f(2, 0, 0);
		
			glColor3f(0, 1, 0);
			glVertex3f(0, -2, 0);
			glVertex3f(0, 2, 0);

			glColor3f(0, 0, 1);
			glVertex3f(0, 0, -2);
			glVertex3f(0, 0, 2);
	
		glEnd();	
		glEnable(GL_LIGHTING);
		glPopAttrib();
	}

	
	/** Draw this object and it's children 
	 * @param frame The frame to draw
	*/
	void Object::draw(const unsigned int frame)
	{
		
		if (texture == 0 && textureFile.size() != 0)
		{
			//cout << "loading: " << textureFile << " for object: " << getName() << endl;
//			DebugText(EMSG, "loading: %s, for object: %s", textureFile.c_str(), getName().c_str());
			CResourceManager::getInstance().createResource(texture, textureFile.c_str());
			if (texture == 0)
			{
				DebugText(EMSG, "Unable to load texture!");
				//cout << "unable to load: " << textureFile << endl;
				glDisable(GL_TEXTURE_2D);
			}
		}
		if (texture != 0)
		{
			glEnable(GL_TEXTURE_2D);
			texture->ActivateTexture(0);
		}
		else
			glDisable(GL_TEXTURE_2D);



		if (frame > FRAMES) return;
		if (!doneMatrix[frame])
			generateMatrix(frame);
		
		glPushMatrix();
			if (this == selectedObject)
			{
				glPushMatrix();
					glMultMatrixd(pivotMatrix[frame].GetMatrix());
					drawPivot();
				glPopMatrix();
			}
			
			glPushAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT | GL_LIGHTING_BIT);
			if (twoSided)
			{
					glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, true);
					glDisable(GL_CULL_FACE);
			}
			if (!lighted)
				glDisable(GL_LIGHTING);
			if (transparent != 0)
			{
				glColor4f(1.0, 1.0, 1.0, transparent);
				//glDisable(GL_DEPTH_TEST);
			}
			//float a,b,c;
 			//findObjectWorldCoords(a,b,c);
 			//bool vis = camera.Frustum.PointVisible(a,b,c);
 			//if (vis)
			glMultMatrixd(drawMatrix[frame].GetMatrix());
 			shapeDraw();
			glPopAttrib();
		glPopMatrix();
		
		
		glPushMatrix();
			glMultMatrixd(childMatrix[frame].GetMatrix());
			for (unsigned int i = 0 ; i < children.size(); i++)
			{
				children[i]->draw(frame);
			}
		glPopMatrix();	
	}
	Object *Object::sphereCollide(const VECTOR3 &center, const float &radius, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal)
	{
		// Unfortunatly we must test every object in the scene ( though subRayCollide does it's own optimizations ) 
		// because although we might collide with the ground here, another object attached to the ground(as a child)
		// might also collide. So we must test all children as well.

		// Do collision against this object first
		Object *res = subSphereCollide(center, radius, out, smallestDistance, faceNormal);
		// For each child do the sphere test for it's children
		for (unsigned int i = 0; i < children.size(); i++)
		{
			Object *tempres = children[i]->sphereCollide(center, radius, out, smallestDistance, faceNormal);
			// If the object was not null then we collided and smallestDistance,out and faceNormal has been set
			// also record the object that it collided with
			if (tempres != NULL)
				res = tempres;	
		}	
		// Return the object that the sphere collided with
		return res;
	}

	Object *Object::rayCollide(VECTOR3 point, VECTOR3 target, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal)
	{
		// Unfortunatly we must test every object in the scene ( though subRayCollide does it's own optimizations ) 
		// because although we might collide with the ground here, another object attached to the ground(as a child)
		// might also collide. So we must test all children as well.

		// Do collision against this object first
		Object *res = subRayCollide(point, target, out, smallestDistance, faceNormal);
		// For each child do the ray test for it's children
		for (unsigned int i = 0; i < children.size(); i++)
		{
			Object *tempres = children[i]->rayCollide(point, target, out, smallestDistance, faceNormal);
			// If the object was not null then we collided and smallestDistance,out and faceNormal has been set
			// also record the object that it collided with
			if (tempres != NULL)
				res = tempres;	
		}	
		// Return the object that the ray collided with
		return res;
	}

	void Object::doLightMaps(VECTOR3 lights[], unsigned int lightCount)
	{
		// If this object is not static then do not generate a map for it or its children
		if (!this->isStatic())
			return;

		generateLightMaps(lights, lightCount);	
		for (unsigned int i = 0; i < children.size(); i++)
		{
			children[i]->doLightMaps(lights, lightCount);
		}
	}
	/** Draw this object and it's children 
	 * @param frame The frame to draw
	*/
#ifdef NOTHING 
	void Object::draw(const unsigned int frame)
	{
		if (frame > FRAMES) return;
	
		glPushMatrix();
		float x, y, z;	
		/*
			Too get an object to rotate correctly we must perfom the transformations
			in reverse. This is because OpenGL post-multiplies matrices.
			So to make an object rotate around a given pivot point we have to perform
			these operations...

			0. Make object rotate to fit the proper angle.
			1. Translate TO pivot point
			2. Perform rotations (X, Y, Z)
			3. Translate BACK from pivot point
			4. Add any extra origin required for the fram
			5. Move object to it's relative origin

			These are implemented using OpenGL operations but in REVERSE!

		*/
		
		// Translate to where we want to be 
		glTranslatef(origin[0], origin[1], origin[2]);
		// Translate offset
		x = interp(originOffsets[frame][0], originOffsets[(frame+1) % FRAMES][0]);
		y = interp(originOffsets[frame][1], originOffsets[(frame+1) % FRAMES][1]);
	 	z = interp(originOffsets[frame][2], originOffsets[(frame+1) % FRAMES][2]);
		glTranslatef(x, y, z);

		// Move back FROM the pivot point
		glTranslatef(pivot[0], pivot[1], pivot[2]);
		// Interpolate angles
		x = interp(angles[frame][0], angles[(frame+1) % FRAMES][0]);
		y = interp(angles[frame][1], angles[(frame+1) % FRAMES][1]);
	 	z = interp(angles[frame][2], angles[(frame+1) % FRAMES][2]);
		// Perfom rotations backwards ( Z, Y, X )
		glRotatef(z, 0, 0, 1);
		glRotatef(y, 0, 1, 0);
		glRotatef(x, 1, 0, 0);
		// If this is the currently selected object draw some axes in to help
		// us out! Note: This is REALLY HELPFULL!!!
		if (this == selectedObject)
		{
			glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
				glColor3f(1, 0, 0);
				glVertex3f(-2, 0, 0);
				glVertex3f(2, 0, 0);
			
				glColor3f(0, 1, 0);
				glVertex3f(0, -2, 0);
				glVertex3f(0, 2, 0);
	
				glColor3f(0, 0, 1);
				glVertex3f(0, 0, -2);
				glVertex3f(0, 0, 2);
		
			glEnd();	
			glEnable(GL_LIGHTING);
			glPopAttrib();
		}
		// Translate TO pivot point
		glTranslatef(-pivot[0], -pivot[1], -pivot[2]);
		
		// Rotate object to correct orientation		
		glPushMatrix();
			glRotatef(fixedAngles[2], 1, 0, 0);
			glRotatef(fixedAngles[1], 0, 1, 0);
			glRotatef(fixedAngles[0], 0, 0, 1);
			glPushMatrix();
				glScalef(scale[0], scale[1], scale[2]);
				shapeDraw();
			glPopMatrix();
		glPopMatrix();
		
		// Draw our children
		for (unsigned int i = 0 ; i < children.size(); i++)
			children[i]->draw(frame);
		
		glPopMatrix();
	}
#endif	
	
	MATRIX Object::findObjectWorldCoords(float &x, float &y, float &z)
	{
		if (1)//!doneWorldMatrix)
		{
//			if (!doneMatrix[0])
				generateMatrix(0);
//			DebugText(EMSG, "findObjectWorldCoords: %s", getName().c_str());
			MATRIX m;
			stack<MATRIX> mStack;
			// Go UP the tree making a matrix based on it
			Object *p = this;
			do
			{
				VECTOR3 allZeros = CVector3(p->fixedAngles[0], p->fixedAngles[1], p->fixedAngles[2]);
//				DebugText(EMSG, "pname: %s", p->getName().c_str(),
				if (p->fixedAngles[0] == 0)
					mStack.push(p->drawMatrix[0]);
				else if (p->fixedAngles[1] == 0)	
					mStack.push(p->drawMatrix[0]);
				else if (p->fixedAngles[2] == 0)
					mStack.push(p->drawMatrix[0]);

//				    p->fixedAngles[1] == 0 &&
//				    p->fixedAngles[2] == 0)
				else			
					mStack.push(p->childMatrix[0]);
				p = p->parent;
			}while (p != 0);
			// Make sure matrices are applied in the right order by
			// using a stack.
			do
			{
				m	= m * mStack.top();
				mStack.pop();
			}
			while (!mStack.empty());
			doneWorldMatrix = true;
			worldMatrix = m;
		}
		
		// 'm' should contain the matrix
		// Get x,y,z out of it!
		x = y = z = 0;
		float x1 = (x * worldMatrix(0,0)) + (y * worldMatrix(1,0)) + (z * worldMatrix(2,0)) + worldMatrix(3,0);
		float y1 = (x * worldMatrix(0,1)) + (y * worldMatrix(1,1)) + (z * worldMatrix(2,1)) + worldMatrix(3,1);
		float z1 = (x * worldMatrix(0,2)) + (y * worldMatrix(1,2)) + (z * worldMatrix(2,2)) + worldMatrix(3,2);
		x = x1;
		y = y1;
		z = z1;
		return worldMatrix;
	}
	
	/** Find an object based on it's name below this node in the tree
	 * @param The name of the object we want to find
	 * @return The object
	 */
	Object *Object::findObjectByName(const string &objectName)
	{
		if (getName() == objectName)
			return this;

		for (unsigned int i = 0; i < children.size(); i++)
		{
			Object *o = children[i]->findObjectByName(objectName);
			if (o != NULL)
				return o;
		}
		return NULL;
	}

	/** Set the origin of this object relative to it's parent
	 * @param v x, y, z positions RELATIVE to the parent
	 */
	void Object::setOrigin(float v[3])
	{
		origin[0] = v[0]; origin[1] = v[1]; origin[2] = v[2];
		resetMatrices();
	}
	void Object::getOrigin(float v[3])
	{
		v[0] =origin[0]; v[1] = origin[1]; v[2] = origin[2];
	}
/** Set the angles of this object
	 * @param frame The frame to set
	 * @param a The x, y, z angles
	 */
	 void Object::getAngles(const unsigned int &frame, float a[3])
	 {
			if (frame > FRAMES) return;
			a[0] = angles[frame][0];
			a[1] = angles[frame][1];
			a[2] = angles[frame][2];
	 }
	 void Object::setAngles(const unsigned int &frame, float a[3])
	 {
			if (frame > FRAMES) return;
			resetMatrices();
			angles[frame][0] = a[0];
			angles[frame][1] = a[1];
			angles[frame][2] = a[2];
	 }
	 void Object::setFixedAngles(float a[3])
	 {
		resetMatrices();
		fixedAngles[0] = a[0];
		fixedAngles[1] = a[1];
		fixedAngles[2] = a[2];
	 }
/** Set the pivot of this object relative to it's parent
	 * @param p x, y, z pivot positions RELATIVE to the parent
	 */
	void Object::setPivot(float p[3])
	{
		resetMatrices();
		pivot[0] = p[0]; pivot[1] = p[1]; pivot[2] = p[2];
	}
	
	void Object::setScale(float s[3])
	{
		resetMatrices();
		scale[0] = s[0]; scale[1] = s[1]; scale[2] = s[2];
	}
	/** Set the limits on the angles
	 * @param l x, y, z Limits of the angles
	 */
	void Object::setLimits(float l[3])
	{
		resetMatrices();
		limits[0] = l[0]; limits[1] = l[1]; limits[2] = l[2];
	}

	/** Set the oigin offsets RELATIVE to the center of this object
	 * @param frame The frame to set
	 * @param o The x, y, z position RELATIVE to the center of this object
	 */
	 void Object::setOriginOffsets(const unsigned int &frame, float o[3])
	 {
			if (frame > FRAMES) return;
			resetMatrices();
			originOffsets[frame][0] = o[0];
			originOffsets[frame][1] = o[1];
			originOffsets[frame][2] = o[2];
	 }

	/** Set a parameter for this object. The parameters are dependent on the object
	 * type.<br>
	 * For example with a Sphere the params are:<br>
	 * 0 - Radius<br>
	 * 1 - stacks<br>
	 * 2 - slices<br>
	 * Each object type has different params
	 */
//	void Object::setParam(const unsigned int &paramID, const float &value)
//	{
//		param[paramID] = value;
//	}


