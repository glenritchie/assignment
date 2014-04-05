//***********************************************************************//
//
//		$Author:		Spikeles
//
//		$Program:		MapEdit
//
//		$Description: Camera Class.. used for graphics independant movement
//
//		$Date:			25/3/2002
//
//***********************************************************************//
#include "camera.h"
#include "math.h"
//#include "mathlib.h"
//#include "memleakcheck.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>

CCamera::CCamera()
{
	ZeroOrigin();
	ZeroAngles();
	vmatrix.identity();
	pmatrix.identity();
	UpdateMatrix_Vectors();
	changed = true;
	setClassname("CCamera");

 	right = VECTOR3(1.0f, 0.0f, 0.0f);
 	up = 	VECTOR3(0.0f, 1.0f, 0.0f);
 	forward=VECTOR3(0.0f, 0.0f, 1.0f);
}
void CCamera::MoveBack(float speed)
{
	//origin=(forward*speed)+origin;
	changed = true;
	accel = accel - forward*speed;
	UpdateMatrix_Vectors();
}
void CCamera::MoveForward(float speed)
{
	//origin=(forward*-speed)+origin;
	changed = true;
	accel = accel + forward*speed;
	UpdateMatrix_Vectors();
}
void CCamera::MoveRight(float speed)
{
	changed = true;
	//origin=(right*-speed)+origin;
	accel = accel + right*-speed;
	UpdateMatrix_Vectors();
}
void CCamera::MoveLeft(float speed)
{
	changed = true;
	//origin=(right*speed)+origin;
	accel = accel + right*+speed;
	UpdateMatrix_Vectors();
}

// Overloaded functions
void CCamera::SetOrigin(float x,float y,float z)
{
	changed = true;
	origin.x=x;origin.y=y;origin.z=z;
	
	UpdateMatrix_Vectors();
}

void CCamera::SetAngles(CVector3 newangles)
{
	SetAngles(newangles.x,newangles.y,newangles.z);
}
void CCamera::SetAngles(float x, float y, float z)
{
	changed = true;
	// Check and fix rotation angles.
	if (z<0)    z=360;
	if (z>360)  z=0;

	angles.x=x;angles.y=y;angles.z=z;
	UpdateMatrix_Vectors();
}

void CCamera::AddAngles(CVector3 newangles)
{
	AddAngles(newangles.x,newangles.y,newangles.z);
}
void CCamera::AddAngles(float x, float y, float z)
{
	changed = true;
	SetAngles(x+angles.x,y+angles.y,z+angles.z);
}

void CCamera::Think()
{
	vel = vel + accel;
	origin = origin + vel;
	vel = vel - vel*0.5f; // Simulate drag
	accel.x = accel.y = accel.z = 0;

	changed = true;
	UpdateMatrix_Vectors();
}
void CCamera::MoveTo(float goalx, float goaly, float goalz)
{

}
void CCamera::lookAt(float eyex, float eyey, float eyez, float tx, float ty, float tz)
{
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, tx, ty, tz, 0, 1, 0);
	double m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	vmatrix = MATRIX(m);
	glPopMatrix();

	origin.x = eyex; origin.y = eyey; origin.z = eyez;
	vmatrix.ToVectors(right, up, forward);
	//up = VECTOR3(0, 1, 0);
}
void CCamera::UpdateMatrix_Vectors()
{

	//	vel = vel + accel;
//	origin = origin + vel;
//	vel = vel - (vel*0.10f); // Simulate drag
//	if (!changed)
//		return;
	// Find forward,right and up vectors
//	origin = VECTOR3(0,0,0);
	//CamAngleVectors(angles, forward, right, up);

	CMatrix a;
	CMatrix b;
	CMatrix c;
	CMatrix d;
	
	a.identity();
	b.identity();
	c.identity();
	d.identity();

	translation = origin;
	rotation = angles;
	// Translation and origin are DIFFERENT!!
	// Translation is the actual things we need to do to translate the camera to make it fit
	// in with the graphics api
	// For example.
	// If the origin of the camera is 0,0,10 then the translation is 0,0,-10
	
	CMatrix rot;
	
	//a.scale_xyz(1,1,-1);
	a.setorigin(-translation.x, -translation.y, -translation.z);
	b.rotate_axis(1.0f, 0.0f,	0.0f, angles.x);
	c.rotate_axis(0.0f,	1.0f, 	0.0f, angles.y);
	//d.rotate_axis(0.0f,	0.0f,	1.0f, -angles.z);
	//b.rotate_angles(angles.x, angles.y, angles.z);
	//c.rotate_y(-angles.y);
	//d.rotate_z(-angles.z);
	//b.rotate_angles(-rotation.x, -rotation.y, -rotation.z);
	rot = b * c;
	vmatrix = rot * a;
/*
    | A B C D |
M = | E F G H |
    | I J K L |
    | M N O P |
    X-axis = [ A E I ]
    Y-axis = [ B F J ]
    Z-axis = [ C G K ]
	Draw2DText(10, 20, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[0], m[4], m[8],  m[12]);
	Draw2DText(10, 30, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[1], m[5], m[9],  m[13]);
	Draw2DText(10, 40, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[2], m[6], m[10], m[14]);
	Draw2DText(10, 50, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[3], m[7], m[11], m[15]);
 */
//	CMatrix mat;
//	a.identity();
//	b.identity();
//	a.setorigin(origin.x, origin.y, origin.z);
//	b.rotate_angles(angles.x, angles.y, angles.z);
//	mat = b * a;

	//float *m = mat.GetMatrix();
	//forward = 
	vmatrix.ToVectors(right, up, forward);
	//mat.ToVectors(right, up, forward);
	//forward = forward * -1;
	
 
//  	vmatrix.identity();
	
	VECTOR3 Lookat = (forward * 2) + origin;
//	Lookat = Lookat * -1;
	//VECTOR3 Lookat(-10.0f,-10.0f,-10.0f);
//	Lookat = Normalize(Lookat);
//	Draw2DText(10, 140, 0xFFFFFFFF, "lookat: x:%.2f, y:%.2f, z:%.2f ", Lookat.x, Lookat.y, Lookat.z);
	CVector3 upDir(0.0f, 1.0f, 0.0f);
	vmatrix.LookAt(origin, Lookat, upDir);
		
//	float *m = vmatrix.GetMatrix();
//	Draw2DText(10, 200, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[0], m[4], m[8],  m[12]);
//	Draw2DText(10, 210, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[1], m[5], m[9],  m[13]);
//	Draw2DText(10, 220, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[2], m[6], m[10], m[14]);
//	Draw2DText(10, 230, 0xFFFFFFFF, "%.2f, %.2f, %.2f %.2f", m[3], m[7], m[11], m[15]);

	//vmatrix.rotate_angles(angles.x, angles.z, angles.y);
	//vmatrix.translate(-origin.x, -origin.y, -origin.z);
	Frustum.Extract(pmatrix, vmatrix);
}
void CCamera::updateFrustum()
{
	Frustum.Extract(pmatrix, vmatrix);
}
