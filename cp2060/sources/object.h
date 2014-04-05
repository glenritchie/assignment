#ifndef __object_h__
#define __object_h__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "matrix.h"
#include "utils.h"
#include "ctexture.h"

using namespace std;
const int FRAMES = 20;
/// Base object type
class Object
{
public:
		/** Construct a new object with the given type and name
	 * @param newName The name of this object
	 */
	Object(string newName); 
	Object(Object *o); 
	
	/** Set all the angles to 0 */
	void nullAngles();
	/** Set the origin offsets to 0 */
	void nullOriginOffsets();
	/** Perfrom a recursive call to null all angles and origin offsets 
	 * on this nodes children */
	void nullChildren();

	/* Desconstruct the object */
	virtual //~Object();
	~Object()
	{
		for (unsigned int i = 0; i < children.size(); i++)
			delete children[i];
		children.clear();	
	}
private:	
	/** Test this object to see if the given ray intersects this object. If so it returns the intersection point
	 * the distance to it and the normal of the face.<br>
	 * NOTE: This is the private interface for this object and should only be called by itself. To do an intersection
	 * test call rayCollide, it will test all children of this object
	 * @param point [in] The start point of the ray
	 * @param target [in] The end point of the ray
	 * @param out [out] The intersection point of the ray
	 * @param smallestDistance [out] The distance from the start of the ray to the intersection point
	 * @param faceNormal [out] The normal of the face that the ray collided with
	 * @return The object the face collided with
	 */
	virtual Object* subRayCollide(VECTOR3 point, VECTOR3 target, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal) 
	{ 
		return NULL; 
	}
	virtual Object* subSphereCollide( const VECTOR3 &center, const float &radius, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal) 
	{ 
		return NULL; 
	}
public:
	/** Test this object AND it's children. to see if the given ray intersects this object. If so it returns the intersection point
	 * the distance to it and the normal of the face.<br>
	 * NOTE: This is the public  interface for this object. This function calls subRayCollide to test it's children.
	 * @param point [in] The start point of the ray
	 * @param target [in] The end point of the ray
	 * @param out [out] The intersection point of the ray
	 * @param smallestDistance [out] The distance from the start of the ray to the intersection point
	 * @param faceNormal [out] The normal of the face that the ray collided with
	 * @return The object the face collided with
	 */
	Object *rayCollide(VECTOR3 point, VECTOR3 target, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal);

	Object *sphereCollide(const VECTOR3 &center, const float &radius, VECTOR3 &out, float &smallestDistance, VECTOR3 &faceNormal);

	void doLightMaps(VECTOR3 lights[], unsigned int lightCount);
	virtual void generateLightMaps(VECTOR3 lights[], unsigned int lightCount) { ; }
//	virtual void transformChanged() = 0;

	/** Add a child to this object 
	 * @param o The object to add
   */
	void addChild(Object *o) 
	{
		o->parent = this;
		children.push_back(o); 
	}

	/** Draw this object and it's children 
	 * @param frame The frame to draw
	*/
	void draw(const unsigned int frame);
	/** Find an object based on it's name below this node in the tree
	 * @param The name of the object we want to find
	 * @return The object
	 */
	Object *findObjectByName(const string &objectName);

	/** Set the scale of this object
	 * @param v x, y, z positions RELATIVE to the parent
	 */
	void setScale(float v[3]);
	
	/** Get the origin of this object relative to it's parent
	 * @param v x, y, z positions RELATIVE to the parent
	 */
	void Object::getOrigin(float v[3]);
	/** Set the origin of this object relative to it's parent
	 * @param v x, y, z positions RELATIVE to the parent
	 */
	void setOrigin(float v[3]);
	/** Set the angles of this object
	 * @param frame The frame to set
	 * @param a The x, y, z angles
	 */
	 void setAngles(const unsigned int &frame, float a[3]);
	 
	 /** Get the angles of this object
	 * @param frame The frame to get
	 * @param a The x, y, z angles to set
	 */
	 void getAngles(const unsigned int &frame, float a[3]);

	/** Set the FIXED angles of this object. Fixed angles are rotations applied to the object
	 * so that it is oriented the correct way before drawing.
	 * @param frame The frame to set
	 * @param a The x, y, z angles
	 */
	 void setFixedAngles(float a[3]);
	/** Set the pivot of this object relative to it's parent
	 * @param p x, y, z pivot positions RELATIVE to the parent
	 */
	void setPivot(float p[3]);
	/** Set the limits on the angles
	 * @param l x, y, z Limits of the angles
	 */
	void setLimits(float l[3]);
	/** Set the oigin offsets RELATIVE to the center of this object
	 * @param frame The frame to set
	 * @param o The x, y, z position RELATIVE to the center of this object
	 */
	 void setOriginOffsets(const unsigned int &frame, float o[3]);
	/** Set a parameter for this object. The parameters are dependent on the object
	 * type.<br>
	 * For example with a Sphere the params are:<br>
	 * 0 - Radius<br>
	 * 1 - stacks<br>
	 * 2 - slices<br>
	 * Each object type has different params
	 */
//	void setParam(const unsigned int &paramID, const float &value);
	/** Get the objects parent.
	 * @return Return the parent node of this object or NULL if it's the root */
	Object *getParent() const { return parent; }
	/** Set the objects parent.
	 * @param newParent the parent node of this object or NULL if it's the root */
	void setParent(Object *newParent) { parent = newParent; }


	/** Get the number of children
	 * @return The number of children
	 */
	unsigned int getNumberOfChildren() { return children.size(); } 

	/** Get a child object
	 * @param i The index of the child object 
	 */
	Object *getChild(unsigned int i) 
	{
		if (i >= children.size()) return NULL;
		return children[i]; 
	}

	void setTextureFile(std::string newTextureFile)
	{
		textureFile = newTextureFile;
	}

	/**
	 * Read in from the given file stream the parameters for this object
	 * @param file The file to read from.
	*/
	virtual void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level) { ; }
	MATRIX Object::findObjectWorldCoords(float &x, float &y, float &z);

	/** Get the name of this node 
	 * @return The name of this node */
	string getName() const { return name; }
	/** Print this node and it's children for debugging purposes
	 * @param indent The current indent
	*/
	void printTree(unsigned int indent);
	/** Set this object to have two sides 
	 * @param value The value */
	void set2sided(bool value) { twoSided = value; }
	/** Set this object to be lighted or not lighted 
	 * @param value The value */
	void setLighted(bool value) { lighted = value; }
	/** Set this object to be transparent or not 
	 * @param value The value */
	void setTransparent(float value) { transparent = value; }
	/** Is this object transparent 
	 * @return True if this object is transparent */
	bool isTransparent() { return transparent != 0; }

	/** Set the static flag of this object.
	 * @param newStatic If tru then this object is static and should be ray-traced, else do NOT ray-trace it */
	void setStatic(bool newStatic) 
	{ 
		staticObject = newStatic; 
		for (int i = 0; i <children.size(); i++) children[i]->setStatic(newStatic);
	}

	/** Is this object static?
	 * @return True if the object is static
	 */
	bool isStatic() { return staticObject; }
	
	/** Generate the draw matrices for this frame for this object and it's children 
	 * @param frame The frame to generate the matrix for */
	void generateMatrix(const unsigned int frame);

	float accel[3];
	float vel[3];
		
protected:
	/** Draw the shape - this function is overridden by child objects */
	virtual void shapeDraw() { ; }

	CTexture *texture;
	std::string textureFile;

private:
	/** All objects are static unless specifically told they aren't. The static property is applied to all children down the tree from this node */
	bool staticObject;
	void resetMatrices();

	bool twoSided;
	float transparent;
	bool lighted;
	/** Draw a pivot point ( probably should be static )
	*/
	void drawPivot();
	/// STL vector holding this nodes children
	vector<Object *> children;
	
	/// Limits on the angles
	float limits[3];
	/// Object specific params
	//float param[MAX_PARAMS];
	/// Origin relative to parent (center of object)
	float originOffsets[FRAMES][3];
	/// Offset from the center of this object
	float pivot[3];
	float fixedAngles[3];
	/// Eular Angles (x, y, z) for each frame
	float angles[FRAMES][3];
	/// Scale values
	float scale[3];
    /// Origin relative to parent
	float origin[3];
	/// Parent object
	Object *parent;
	/// Name of this object
	string name;	

	MATRIX worldMatrix;
	MATRIX drawMatrix[FRAMES];
	MATRIX pivotMatrix[FRAMES];
	MATRIX childMatrix[FRAMES];
	bool doneWorldMatrix;
	bool doneMatrix[FRAMES];

};

extern Object *selectedObject;
extern float interp(float, float);
#endif

