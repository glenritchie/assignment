#ifndef __matrix_h__
#define __matrix_h__
#include "vector.h"
#include <memory.h>
/** A matrix class used for matrix manipulation */
class CMatrix
{
public:
	CMatrix();
	CMatrix(CMatrix *other);
	inline CMatrix(double *mat) { memcpy(m, mat, sizeof(double)*16); }

	
	// Operators
	CMatrix operator+(CMatrix other);
	CMatrix operator-(CMatrix other);
	CMatrix operator*(CMatrix other);
	void 	operator*(float k);
	void set(double *mat) { memcpy(m, mat, sizeof(double)*16); }
//	inline float operator()(int a,int b) { return m[a][b]; }
	inline double &operator()(int a, int b) { return m[a][b]; }
	inline double getval(int a,int b) { return m[a][b]; }
	inline void setval(int a, int b, double val) { m[a][b] = val; }
	
	// Publics
	float 	det();
	void ToAngles(VECTOR3 &angles);
	void ToVectors(VECTOR3 &x_axis, VECTOR3 &y_axis, VECTOR3 &z_axis);
	
	CMatrix	adjoint();
	CMatrix transpose();
	CMatrix identity();
	CMatrix zero();
	CMatrix neg();
	CMatrix translate(float x, float y, float z);
	CMatrix translate(VECTOR3 v);
	CMatrix scale_xyz(float x, float y, float z);
	CMatrix setorigin(float x, float y, float z);
	CMatrix setorigin(VECTOR3 origin);
	CMatrix	rotate_x(float phi);
	CMatrix	rotate_y(float phi);
	CMatrix	rotate_z(float phi);
	CMatrix	rotate_axis(float axis_x,float axis_y,float axis_z,float axis_angle);
	CMatrix	rotate_angles(float pitch,float yaw, float roll);
	CMatrix LookAt(VECTOR3 pos, VECTOR3 tgt, VECTOR3 up);
	double *GetMatrix(){return *m; }
private:
	inline float det2x2(float a,float b, float c, float d) {  return((a)*(d)-(b)*(c)); }
	inline float det3x3(float a1,float a2,float a3,float b1,float b2,float b3,float c1,float c2,float c3)
	{ return(a1*det2x2(b2,b3,c2,c3)- b1*det2x2(a2,a3,c2,c3)+ c1*det2x2(a2,a3,b2,b3));}
	
	double m[4][4];
//	float m[4][4];
};

#define MATRIX CMatrix
//#define MATRIX CMatrix

#endif // __matrix_h__


