#include "matrix.h"
#include "3dmath.h"
#include <math.h>
#include <memory.h>
//#include "memleakcheck.h"
//extern double fsqrt(double r);
/****************************

  Lots of this code borrowed from LIB3DS source.

*****************************/

//const float M_PI = 3.1415926535897932384626433832795f;
//#include <d3dx8.h>
/*#define M_PI		(3.14159265358979323846264338327950288f)
#define rads        (M_PI/180)
#define __PI        (M_PI)
#define __DEG2RAD   (__PI/180)
#define deg2rad(x)  ((x)*__DEG2RAD)
*/
#define EPSILON (1e-8)

CMatrix::CMatrix(CMatrix *other)
{
	for (int j=0; j<4; j++)
		for (int i=0; i<4; i++)
			m[j][i]=other->m[j][i];
}

CMatrix::CMatrix()
{
	identity();
}
void CMatrix::ToVectors(VECTOR3 &x_axis, VECTOR3 &y_axis, VECTOR3 &z_axis)
{
	//	    |  0  1  2  3 |
	//  M = |  4  5  6  7 |
	//      |  8  9 10 11 |
	//      | 12 13 14 15 |
	//  Then the direction vector for each axis is as follows:
	//  X-axis = [  0  1  2 ]
	//  Y-axis = [  4  5  6 ]
	//  Z-axis = [  8  9 10 ]

	//  X-axis = [  0  4  8 ]
	//  Y-axis = [  1  5  9 ]
	//  Z-axis = [  2  6 10 ]
	
	// Find forward,right and up vectors from matrix
	//float *mat=GetMatrix();
	x_axis	= VECTOR3(m[0][0], m[1][0], m[2][0]);
	y_axis	= VECTOR3(m[0][1], m[1][1], m[2][1]);
	z_axis	= VECTOR3(m[0][2], m[1][2], m[2][2]);
/*
	x_axis	= VECTOR3(m[0][0], m[0][1], m[0][2]);
	y_axis	= VECTOR3(m[1][0], m[1][1], m[1][2]);
	z_axis	= VECTOR3(m[2][0], m[2][1], m[2][2]);
	*/
	x_axis = Normalize(x_axis);
	y_axis = Normalize(y_axis);
	z_axis = Normalize(z_axis);
}

void CMatrix::ToAngles(VECTOR3 &angles)
{
	CMatrix a;
	a=this;
	double *mat=a.GetMatrix();

	double angle_x, angle_y, angle_z;
	double C;
	double tr_x, tr_y;
	double D;
	
	//	    |  0  1  2  3 |
	//  M = |  4  5  6  7 |
	//      |  8  9 10 11 |
	//      | 12 13 14 15 |
	//  Then the direction vector for each axis is as follows:
	//  X-axis = [  0  4  8 ]
	//  Y-axis = [  1  5  9 ]
	//  Z-axis = [  2  6 10 ]
	//float RADIANS = M_PI/180;
	const double RADIANS = 180 / M_PI;
	angle_y = D =  asin( mat[2]);			// Calculate Y-axis angle 
    C           =  cos( angle_y );
    angle_y    *=  RADIANS;
	
    if ( fabs( C ) > 0.005 )				// Gimball lock? 
	{
		tr_x      =  mat[10] / C;           // No, so get X-axis angle 
		tr_y      = -mat[6]  / C;
		
		angle_x  = atan2( tr_y, tr_x ) * RADIANS;
		
		tr_x      =  mat[0] / C;            // Get Z-axis angle 
		tr_y      = -mat[1] / C;
		
		angle_z  = atan2( tr_y, tr_x ) * RADIANS;
	}
    else									// Gimball lock has occurred
	{
		angle_x  = 0;						// Set X-axis angle to zero 
		
		tr_x      =  mat[5];                // And calculate Z-axis angle 
		tr_y      =  mat[4];
		
		angle_z  = atan2( tr_y, tr_x ) * RADIANS;
	}
	
    // return only positive angles in [0,360] 
    if (angle_x < 0) angle_x += 360;
    if (angle_y < 0) angle_y += 360;
    if (angle_z < 0) angle_z += 360;
	angles.x=angle_x;
	angles.y=angle_y;
	angles.z=angle_z;
}

CMatrix CMatrix::LookAt(VECTOR3 pos, VECTOR3 tgt, VECTOR3 up)
{
/*
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

 	xaxis.x           yaxis.x           zaxis.x          0
 	xaxis.y           yaxis.y           zaxis.y          0
 	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1
*/
/*
zaxis = normal(Eye - At)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

 xaxis.x           yaxis.x           zaxis.x          0
 xaxis.y           yaxis.y           zaxis.y          0
 xaxis.z           yaxis.z           zaxis.z          0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1
*/

	VECTOR3 z,x,y;
	z = pos - tgt;
	z = Normalize(z);
	
	x=Cross(up,z);
	x = Normalize(x);
	y=Cross(z,x);
	
	m[0][0]=x.x;
	m[1][0]=x.y;
	m[2][0]=x.z;
	m[3][0]=-Dot(x,pos);
	
	m[0][1]=y.x;
	m[1][1]=y.y;
	m[2][1]=y.z;
	m[3][1]=-Dot(y,pos);
	
	m[0][2]=z.x;
	m[1][2]=z.y;
	m[2][2]=z.z;
	m[3][2]=-Dot(z,pos);
	
	m[0][3]=0;
	m[1][3]=0;
	m[2][3]=0;
	m[3][3]=1.0f;
	
/*
	D3DXMATRIX a;
	D3DXVECTOR3 pos1=D3DXVECTOR3(pos.x,pos.y,pos.z);
	D3DXVECTOR3 tgt1=D3DXVECTOR3(tgt.x,tgt.y,tgt.z);
	D3DXVECTOR3 up1=D3DXVECTOR3(up.x,up.y,up.z);
	
	D3DXMatrixLookAtLH(&a,&pos1,&tgt1,&up1);
*/	
	/*	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	m[j][i]=matrix.m[j][i];
	*/
	return this;
}
CMatrix CMatrix::scale_xyz(float x, float y, float z)
{
  int i;

  for (i=0; i<4; i++) {
    m[0][i]*=x;
    m[1][i]*=y;
    m[2][i]*=z;
  }
  return this;
}


CMatrix CMatrix::rotate_angles(float pitch, float yaw, float roll)
{
	double A, B, C, D, E, F, AD, BD;
	
//	float *mat=this->GetMatrix();//[16];
//         | CE        -CF       D   0 |
//    M  = | BDE+AF    -BDF+AE  -BC  0 |
//         | -ADE+BF   ADF+BE    AC  0 |
//         | 0         0         0   1 |
	pitch=deg2rad(pitch);
	yaw=deg2rad(yaw);
	roll=deg2rad(roll);

	A       = cos(-pitch);
    B       = sin(-pitch);
    C       = cos(yaw);
    D       = sin(yaw);
    E       = cos(roll);
    F       = sin(roll);

    AD      =   A * D;
    BD      =   B * D;

    m[0][0]  =   C * E;
	m[0][1]  =  BD * E + A * F;
	m[0][2]  = -AD * E + B * F;

    m[1][0]  =  -C * F;
    m[1][1]  = -BD * F + A * E;
    m[1][2]  =  AD * F + B * E;

    m[2][0]  =   D;
    m[2][1]  =  -B * C;
    m[2][2]  =   A * C;

    //m[3][0]  =  mat[3][1] = mat[3] = mat[12] = mat[13] = mat[14] = 0;
    //m[15] =  1;

	return this;	
}

CMatrix CMatrix::rotate_axis(float axis_x,float axis_y,float axis_z,float axis_angle)
{
	double s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,l, omega;
	double q[4];//={axis_x,axis_y,axis_z,axis_angle};
	CMatrix a(this);	// Copy this matrix into a
	CMatrix b;

	axis_angle = deg2rad(axis_angle);
	
	/*
	// Generate Quat
	l=sqrt(axis_x*axis_x + axis_y*axis_y + axis_z*axis_z);
	if (l<EPSILON) {
		q[0]=q[1]=q[2]=0.0f;
		q[3]=1.0f;
	}
	else {
		omega=-0.5*axis_angle;
		s=sin(omega)/l;
		q[0]=s*axis_x;
		q[1]=s*axis_y;
		q[2]=s*axis_z;
		q[3]=cos(omega);
	}
*/
	VECTOR3 axis= VECTOR3(axis_x,axis_y,axis_z);
	axis = Normalize(axis);
    double sin_a, cos_a;
	sin_a = sin( axis_angle / 2 );
    cos_a = cos( axis_angle / 2 );

    q[0]    = axis.x * sin_a;
    q[1]    = axis.y * sin_a;
    q[2]    = axis.z * sin_a;
    q[3]    = cos_a;

	double ll,lm;

	// Normalize
	ll=sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	if (fabs(ll)<EPSILON) {
	    q[0]=q[1]=q[2]=0.0f;
	    q[3]=1.0f; 
	}
	else {  
	    int i;
	    lm=1.0f/ll;
	    for (i=0; i<4; ++i) {
			q[i]=(float)(q[i]*lm);
		}
	}

	// Generate Matrix for Quat
	wx = q[3] * q[0];  wy = q[3] * q[1]; wz = q[3] * q[2];
	xx = q[0] * q[0];  xy = q[0] * q[1]; xz = q[0] * q[2];
	yy = q[1] * q[1];  yz = q[1] * q[2]; zz = q[2] * q[2];
	
	b.m[0][0]=1.0f - 2 * (yy + zz);
	b.m[1][0]=       2 * (xy - wz);
	b.m[2][0]=       2 * (xz + wy);
	
	b.m[0][1]=       2 * (xy + wz);
	b.m[1][1]=1.0f - 2 * (xx + zz);
	b.m[2][1]=       2 * (yz - wx);
	
	b.m[0][2]=       2 * (xz - wy);
	b.m[1][2]=       2 * (yz + wx);
	b.m[2][2]=1.0f - 2 * (xx + yy);
	
	b.m[3][0]=b.m[3][1]=b.m[3][2]=b.m[0][3]=b.m[1][3]=b.m[2][3]=0.0f;
	b.m[3][3]=1.0f;
/*
	l=q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
	if (fabs(l)<EPSILON) {
		s=1.0f;
	}
	else {
		s=2.0f/l;
	}
	
	xs = q[0] * s;   ys = q[1] * s;  zs = q[2] * s;
	wx = q[3] * xs;  wy = q[3] * ys; wz = q[3] * zs;
	xx = q[0] * xs;  xy = q[0] * ys; xz = q[0] * zs;
	yy = q[1] * ys;  yz = q[1] * zs; zz = q[2] * zs;
	
	b.m[0][0]=1.0f - (yy + zz);
	b.m[1][0]=xy - wz;
	b.m[2][0]=xz + wy;
	b.m[0][1]=xy + wz;
	b.m[1][1]=1.0f - (xx + zz);
	b.m[2][1]=yz - wx;
	b.m[0][2]=xz - wy;
	b.m[1][2]=yz + wx;
	b.m[2][2]=1.0f - (xx + yy);
	b.m[3][0]=b.m[3][1]=b.m[3][2]=b.m[0][3]=b.m[1][3]=b.m[2][3]=0.0f;
	b.m[3][3]=1.0f;
*/	
	//lib3ds_matrix_mul(m,a,b);
	CMatrix res=a*b;
	for (int j=0; j<4; j++)
		for (int i=0; i<4; i++)
			m[j][i]=res.m[j][i];
	return this;
}
CMatrix CMatrix::rotate_x(float phi)
{
	phi=deg2rad(phi);
	double SinPhi,CosPhi;
	double a1[4],a2[4];
	
	SinPhi=sin(phi);
	CosPhi=cos(phi);
	memcpy(a1,m[1],4*sizeof(double));
	memcpy(a2,m[2],4*sizeof(double));
	m[1][0]=CosPhi*a1[0]+SinPhi*a2[0];
	m[1][1]=CosPhi*a1[1]+SinPhi*a2[1];
	m[1][2]=CosPhi*a1[2]+SinPhi*a2[2];
	m[1][3]=CosPhi*a1[3]+SinPhi*a2[3];
	m[2][0]=-SinPhi*a1[0]+CosPhi*a2[0];
	m[2][1]=-SinPhi*a1[1]+CosPhi*a2[1];
	m[2][2]=-SinPhi*a1[2]+CosPhi*a2[2];
	m[2][3]=-SinPhi*a1[3]+CosPhi*a2[3];
	return this;
}

CMatrix CMatrix::rotate_y(float phi)
{
	phi=deg2rad(phi);
	double SinPhi,CosPhi;
	double a0[4],a2[4];
	
	SinPhi=sin(phi);
	CosPhi=cos(phi);
	memcpy(a0,m[0],4*sizeof(double));
	memcpy(a2,m[2],4*sizeof(double));
	m[0][0]=CosPhi*a0[0]-SinPhi*a2[0];
	m[0][1]=CosPhi*a0[1]-SinPhi*a2[1];
	m[0][2]=CosPhi*a0[2]-SinPhi*a2[2];
	m[0][3]=CosPhi*a0[3]-SinPhi*a2[3];
	m[2][0]=SinPhi*a0[0]+CosPhi*a2[0];
	m[2][1]=SinPhi*a0[1]+CosPhi*a2[1];
	m[2][2]=SinPhi*a0[2]+CosPhi*a2[2];
	m[2][3]=SinPhi*a0[3]+CosPhi*a2[3];
	return this;
}


/*!
* \ingroup matrix
*/
CMatrix CMatrix::rotate_z(float phi)
{
	phi=deg2rad(phi);
	double SinPhi,CosPhi;
	double a0[4],a1[4];
	
	SinPhi=sin(phi);
	CosPhi=cos(phi);
	memcpy(a0,m[0],4*sizeof(double));
	memcpy(a1,m[1],4*sizeof(double));
	m[0][0]=CosPhi*a0[0]+SinPhi*a1[0];
	m[0][1]=CosPhi*a0[1]+SinPhi*a1[1];
	m[0][2]=CosPhi*a0[2]+SinPhi*a1[2];
	m[0][3]=CosPhi*a0[3]+SinPhi*a1[3];
	m[1][0]=-SinPhi*a0[0]+CosPhi*a1[0];
	m[1][1]=-SinPhi*a0[1]+CosPhi*a1[1];
	m[1][2]=-SinPhi*a0[2]+CosPhi*a1[2];
	m[1][3]=-SinPhi*a0[3]+CosPhi*a1[3];
	return this;
}

CMatrix CMatrix::translate(VECTOR3 t)
{
	int i;
	
	for (i=0; i<3; i++) {
		m[3][i]+= m[0][i]*t.x + m[1][i]*t.y + m[2][i]*t.z;
	}
	return this;
}
CMatrix CMatrix::setorigin(float x, float y, float z)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return this;
}

CMatrix CMatrix::setorigin(VECTOR3 origin)
{
	m[3][0] = origin.x;
	m[3][1] = origin.y;
	m[3][2] = origin.z;
	return this;
}

CMatrix CMatrix::translate(float x, float y, float z)
{
	int i;
	
	for (i=0; i<3; i++) {
		m[3][i]+= m[0][i]*x + m[1][i]*y + m[2][i]*z;
	}
	return this;
}

CMatrix CMatrix::adjoint()
{
	float a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4;
	
	a1 = m[0][0];
	b1 = m[1][0];
	c1 = m[2][0];
	d1 = m[3][0];
	a2 = m[0][1];
	b2 = m[1][1];
	c2 = m[2][1];
	d2 = m[3][1];
	a3 = m[0][2];
	b3 = m[1][2];
	c3 = m[2][2];
	d3 = m[3][2];
	a4 = m[0][3];
	b4 = m[1][3];
	c4 = m[2][3];
	d4 = m[3][3];
	m[0][0]=  det3x3 (b2, b3, b4, c2, c3, c4, d2, d3, d4);
	m[0][1]= -det3x3 (a2, a3, a4, c2, c3, c4, d2, d3, d4);
	m[0][2]=  det3x3 (a2, a3, a4, b2, b3, b4, d2, d3, d4);
	m[0][3]= -det3x3 (a2, a3, a4, b2, b3, b4, c2, c3, c4);
	m[1][0]= -det3x3 (b1, b3, b4, c1, c3, c4, d1, d3, d4);
	m[1][1]=  det3x3 (a1, a3, a4, c1, c3, c4, d1, d3, d4);
	m[1][2]= -det3x3 (a1, a3, a4, b1, b3, b4, d1, d3, d4);
	m[1][3]=  det3x3 (a1, a3, a4, b1, b3, b4, c1, c3, c4);
	m[2][0]=  det3x3 (b1, b2, b4, c1, c2, c4, d1, d2, d4);
	m[2][1]= -det3x3 (a1, a2, a4, c1, c2, c4, d1, d2, d4);
	m[2][2]=  det3x3 (a1, a2, a4, b1, b2, b4, d1, d2, d4);
	m[2][3]= -det3x3 (a1, a2, a4, b1, b2, b4, c1, c2, c4);
	m[3][0]= -det3x3 (b1, b2, b3, c1, c2, c3, d1, d2, d3);
	m[3][1]=  det3x3 (a1, a2, a3, c1, c2, c3, d1, d2, d3);
	m[3][2]= -det3x3 (a1, a2, a3, b1, b2, b3, d1, d2, d3);
	m[3][3]=  det3x3 (a1, a2, a3, b1, b2, b3, c1, c2, c3);
	return this;
}
float CMatrix::det()
{
	double a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4;
	
	a1 = m[0][0];
	b1 = m[1][0];
	c1 = m[2][0];
	d1 = m[3][0];
	a2 = m[0][1];
	b2 = m[1][1];
	c2 = m[2][1];
	d2 = m[3][1];
	a3 = m[0][2];
	b3 = m[1][2];
	c3 = m[2][2];
	d3 = m[3][2];
	a4 = m[0][3];
	b4 = m[1][3];
	c4 = m[2][3];
	d4 = m[3][3];
    return(
		a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)-
		b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)+
		c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)-
		d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4)
		);
}

void CMatrix::operator*(float k)
{
	int i,j;
	
	for (j=0; j<4; j++)
		for (i=0; i<4; i++)
			m[j][i]*=k;
}

CMatrix CMatrix::operator*(CMatrix other)
{
	CMatrix newmat;
	int i,j,k;
	double ab;
	
	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			ab=0.0f;
			for (k=0; k<4; k++) 
				ab+=m[k][i]*other.m[j][k];
			newmat.m[j][i]=ab;
		}
	}
	return newmat;
}

CMatrix CMatrix::operator-(CMatrix other)
{
	CMatrix newmat;
	int i,j;
	
	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			newmat.m[j][i]=m[j][i]-other.m[j][i];
		}
	}
	return newmat;
}

CMatrix CMatrix::operator+(CMatrix other)
{
	CMatrix newmat;
	int i,j;
	
	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			newmat.m[j][i]=m[j][i]+other.m[j][i];
		}
	}
	return newmat;
}

CMatrix CMatrix::transpose()
{
	int i,j;
	double swp;
	
	for (j=0; j<4; j++) {
		for (i=j+1; i<4; i++) {
			swp=m[j][i];
			m[j][i]=m[i][j];
			m[i][j]=swp;
		}
	}
	return this;
}
CMatrix CMatrix::neg()
{
	int i,j;
	
	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			m[j][i]=-m[j][i];
		}
	}
	return this;
}
CMatrix CMatrix::zero()
{
	int i,j;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m[i][j]=0.0f;
		return this;
}

CMatrix CMatrix::identity()
{
	int i,j;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m[i][j]=0.0;
		for (i=0; i<4; i++) m[i][i]=1.0;
	return this;
}

