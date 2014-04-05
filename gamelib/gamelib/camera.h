#include <math.h>
#include "vector.h"
#include "matrix.h"
//#include "f3.h"
#include "3dmath.h"
#include "frustum.h"
#include "base_entity.h"
//#include "matrix.h"

class CCamera : public CBaseEntity
{
public:
		CCamera();
		
		void Think();    											// Unit think function 	- abstract
		void MoveTo(float goalx, float goaly, float goalz);  		// Move to function 		-	abstract

		void UpdateMatrix_Vectors();

		void MoveForward(float speed);
		void MoveBack(float speed);
		void MoveRight(float speed);
		void MoveLeft(float speed);

		inline void ZeroOrigin(void){ origin.x=origin.y=origin.z=0; }
		inline void ZeroAngles(void){ angles.x=angles.y=angles.z=0; }

		// Origin
		CVector3 GetOrigin() { return origin; }
		CVector3 GetForwardVector() { return forward; }
		CVector3 GetUpVector() { return up; }
		CVector3 GetRightVector() { return right; }
		void SetOrigin(float x, float y, float z);
		inline void SetOrigin(CVector3 no) { SetOrigin(no.x,no.y,no.z); }

		// Angles
		CVector3 GetAngles() { return angles; }

		void SetAngles(float x, float y, float z);
		void SetAngles(CVector3 newangles);
		void lookAt(float eyex, float eyey, float eyez, float tx, float ty, float tz);

		// Adds a value ONTO the current angles
		void AddAngles(CVector3 newangles);
		void AddAngles(float x, float y, float z);
		void SetProjectionMatrix(MATRIX newpmatrix) { pmatrix=newpmatrix; }
		void SetFarClip(float newclip) { far_clip = newclip; }
		float GetFarClip() { return far_clip; }
		MATRIX* GetViewMatrix()						{ return &vmatrix; }

		CFrustum 	Frustum;


		void PreRender() { ; }
		void Render() { ; }
		void Collision(CBaseEntity*) { ; }
		void Cleanup() { ; }
		void getbbox(CVector3&, CVector3&) { ; }
		void subinit() { ; }
		void updateFrustum();
private:
		CVector3 translation;
		CVector3 rotation;
		float	far_clip;
		bool changed;
		//CVector3 origin;
		//CVector3 angles;

//		CVector3 forward;
	//	CVector3 up;
//		CVector3 right;
		CVector3 accel;
		CVector3 vel;
		MATRIX	vmatrix;
		MATRIX	pmatrix;
		float fov;
};

