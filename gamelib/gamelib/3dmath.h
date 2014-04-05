#ifndef _3DMATH_H
#define _3DMATH_H
#include <math.h>
// #define PI 3.1415926535897932					// This is our famous PI

#define BEHIND		0
#define INTERSECTS	1
#define FRONT		2

#include "vector.h"

/** 
 * This returns the absolute value of "num"
 * @return float absolute value
 * @param num The number to find the absolute of
 */
double Absolute(double num);

/** 
 * This returns a perpendicular vector from 2 given vectors by taking the cross product.
 * @return The cross vector
 * @param vVector1 the first vector
 * @param vVector2 the second vector
 */
inline VECTOR3 Cross(VECTOR3 &vVector1, VECTOR3 &vVector2)
//VECTOR3 Cross(VECTOR3 &vVector1, VECTOR3 &vVector2)
{
	return VECTOR3(((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y)), 
		((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z)),
		((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x)));
}

/** 
 * This returns the magnitude of a normal (or any other vector)
 * @return The magnitude of the vector
 * @param vNormal the normal vector
 */
inline double Magnitude(const VECTOR3 &vNormal)
{
	return sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}

/** 
 * This returns a normalize vector (A vector exactly of length 1)
 * @return The normalized vector
 * @param vNormal the normal vector
 */
inline VECTOR3 Normalize(const VECTOR3 &vNormal)
{
	double magnitude = Magnitude(vNormal);				// Get the magnitude of our normal
	// Now that we have the magnitude, we can divide our normal by that magnitude.
	// That will make our normal a total length of 1.  This makes it easier to work with too.
	if (magnitude==0)
		return vNormal;
	return VECTOR3(
			vNormal.x / magnitude,
			vNormal.y / magnitude,
			vNormal.z / magnitude);
}
/** 
 * This returns the normal of a polygon (The direction the polygon is facing)
 * @return The normal of the polygon
 * @param vPolygon An array of 3d points that represent a polygon
 */
inline VECTOR3 Normal(VECTOR3 vPolygon[])
{
	VECTOR3 vVector1 = vPolygon[2] - vPolygon[0];
	VECTOR3 vVector2 = vPolygon[1] - vPolygon[0];
	VECTOR3 vNormal = Cross(vVector1, vVector2);		// Take the cross product of our 2 vectors to get a perpendicular vector
	// Now we have a normal, but it's at a strange length, so let's make it length 1.
	vNormal = Normalize(vNormal);						// Use our function we created to normalize the normal (Makes it a length of one)
	return vNormal;										// Return our normal at our desired length
}

/** 
 * This returns the distance between 2 3D points
 * @return The distance 
 * @param vPoint1 The first point in 3D
 * @param vPoint2 The second point in 3D
 */
inline double Distance(VECTOR3 &vPoint1, VECTOR3 &vPoint2)
{	return sqrt((vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );
}

/** 
 * This returns the distance between 2 2D points
 * @return The distance 
 * @param vPoint1 The first point in 2D
 * @param vPoint2 The second point in 2D
 */
inline float Distance(VECTOR2 &vPoint1, VECTOR2 &vPoint2)
{
	return sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) + (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y));
}

/** 
 * This returns the point on the line segment vA_vB that is closest to point vPoint
 * @return The closest point on the line 
 * @param vA The start of the line segment
 * @param vB The end of the line segment
 * @param vPoint The point we are trying to find
 */
VECTOR3 ClosestPointOnLine(VECTOR3 &vA, VECTOR3 &vB, VECTOR3 &vPoint);

/** 
 * This returns the distance the plane is from the origin (0, 0, 0)
 * It takes the normal to the plane, along with ANY point that lies on the plane (any corner)
 * @return returns the distance the plane is from the origin (0, 0, 0)
 * @param VNormal The normal of the plane
 * @param vPoint The point on the plane(a corner)
 */
double PlaneDistance(VECTOR3 &Normal, VECTOR3 &Point);

/** 
 * This takes a triangle (plane) and line and returns true if they intersected
 * @return returns true if the line and trianlge intersect
 * @param vPoly The polygon to test
 * @param vLine The line to test
 * @param vNormal The normal of the plane
 * @param originDistance The maximum distance from the origin we should test to
 */
bool IntersectedPlane(VECTOR3 vPoly[], VECTOR3 vLine[], VECTOR3 &vNormal, double &originDistance);

/** 
 * This returns the dot product between 2 vectors
 * @return returns the dot product between 2 vectors
 * @param vVector1 The first vector
 * @param vVector2 The second vector
*/
inline double Dot(const VECTOR3 &a, const VECTOR3 &b)
{ return (a.x*b.x + a.y*b.y + a.z*b.z); }
/** 
 * This returns the angle between 2 vectors
 * @return returns the angle between 2 vectors in degrees
 * @param Vector1 The first vector
 * @param Vector2 The second vector
 */
double AngleBetweenVectors(const VECTOR3 &Vector1, const VECTOR3 &Vector2);

/** 
 * This returns an intersection point of a polygon and a line (assuming intersects the plane)
 * @return intersection poin
 * @param vNormal The normal of the poly
 * @param vLine The line
 * @param distance The maximum distance to test
 */
VECTOR3 IntersectionPoint(VECTOR3 vNormal, VECTOR3 vLine[], double distance);

VECTOR3 PointonTriangle(VECTOR3 vPoly[], VECTOR3 vLine[], int verticeCount, VECTOR3 &vNormal);
//VECTOR3 PointonTriangle(VECTOR3 vPoly[], VECTOR3 vLine[], int verticeCount);

/// This returns true if the intersection point is inside of the polygon
bool InsidePolygon(VECTOR3 vIntersection, VECTOR3 Poly[], long verticeCount);

/// Use this function to test collision between a line and polygon
bool IntersectedPolygon(VECTOR3 vPoly[], VECTOR3 vLine[], int verticeCount);

/// This function classifies a sphere according to a plane. (BEHIND, in FRONT, or INTERSECTS)
int ClassifySphere(VECTOR3 &vCenter, 
				   VECTOR3 &vNormal, VECTOR3 &vPoint, float radius, float &distance);

/* This takes in the sphere center, radius, polygon vertices and vertex count.
 * This function is only called if the intersection point failed.  The sphere
 * could still possibly be intersecting the polygon, but on it's edges.
 */
bool EdgeSphereCollision(VECTOR3 &vCenter,
						 VECTOR3 vPolygon[], int vertexCount, float radius);

/** This returns true if the sphere is intersecting with the polygon.
*/
bool SpherePolygonCollision(VECTOR3 vPolygon[],
							VECTOR3 &vCenter, int vertexCount, float radius);
// Overloaded version modified by me to return the distance and intersection point
bool SpherePolygonCollision(VECTOR3 vPolygon[], 
							VECTOR3 &vCenter, int vertexCount, float radius, 
							float &distance, VECTOR3 &vPoistion);


/// This returns the offset the sphere needs to move in order to not intersect the plane
VECTOR3 GetCollisionOffset(VECTOR3 &vNormal, float radius, float distance);

///	This function returns an XYZ point along the curve, depending on t (0 to 1)
CVector3 PointOnCurve(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

void findPlane(float plane[4],  float v0[3], float v1[3], float v2[3]);
void shadowMatrix(float shadowMat[4][4],  float groundplane[4],  float lightpos[4]);
bool isPointInCube(CVector3 &cubeOrigin, const float &radius, const CVector3 &p );

//#define M_PI		3.14159265358979323846
#define rads        M_PI/180.0
#define __PI        (3.14159265358979323846264338327950288f)
#define __DEG2RAD    (__PI/180.0)
#define deg2rad(x)    ((x)*__DEG2RAD)

#endif





