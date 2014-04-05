#include "vector.h"
#include <math.h>
 //const float M_PI = 3.1415926535897932384626433832795f;
// This stuff borrowed from Quake 2 code.
// FIXME

/*
              0(0,1)
              -
              |+Y
	          |
	          |
90(1,0) |-----+-----| -90(-1,0)
         +X   |   -X
	          |
              |-Y
		      -
			180 (0,-1)
*/


VECTOR3 VecToAngles( const VECTOR3 value2)
{
	VECTOR3 value1=value2;
	double	forward;
	double	yaw, pitch;

	if ( value1.y == 0 && value1.x == 0 ) {
		yaw = 0;
		if ( value1.z > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1.x ) {
			yaw = ( atan2 ( value1.y, value1.x ) * 180 / M_PI );
		}
		else if ( value1.y > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1.x*value1.x + value1.y*value1.y );
		pitch = ( atan2(value1.z, forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}
	//yaw+=90;
	return VECTOR3(pitch,0,yaw);
}

/*VECTOR3 VecToAngles( const VECTOR3 value2)
{
	VECTOR3 value1=value2;
	float	forward;
	float	yaw, pitch;

	if ( value1.y == 0 && value1.x == 0 ) {
		yaw = 0;
		if ( value1.z > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1.x ) {
			yaw = ( atan2 ( value1.z, value1.x ) * 180 / M_PI );
		}
		else if ( value1.y > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1.x*value1.x + value1.z*value1.z );
		pitch = ( atan2(value1.y, forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}
	return VECTOR3(pitch,0,yaw);
}
*/
/*
void AngleVectors (VECTOR3 angles,VECTOR3 &forward, VECTOR3 &right, VECTOR3 &up)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	angle = angles.x * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles.y * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	angle = angles.z * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);

	forward.x = cp*sy;
	forward.y = cp*cy;
	forward.z = sp;

	right.x = -(-1*sr*sp*sy+-1*cr*cy);
	right.y = -(-1*sr*sp*cy+-1*cr*-sy);
	right.z = -1*sr*cp;

	up.x = -(cr*sp*sy+-sr*cy);
	up.y = -(cr*sp*cy+-sr*-sy);
	up.z = cr*cp;
}

*/
// Simple maths function to convert some angles (x,y,z)
// to Forward, up and right pointing vectors. 
void AngleVectors (VECTOR3 angles,VECTOR3 &forward, VECTOR3 &right, VECTOR3 &up)
{
	double		angle;
	double		sr, sp, sy, cr, cp, cy;

	angle = angles.z * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	
	angle = angles.x * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	
	angle = angles.y * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	forward.x = cp*cy;
	forward.y = cp*sy;
	forward.z = -sp;

	right.x = (-1*sr*sp*cy+-1*cr*-sy);
	right.y = (-1*sr*sp*sy+-1*cr*cy);
	right.z = -1*sr*cp;
	
	up.x = (cr*sp*cy+-sr*-sy);
	up.y = (cr*sp*sy+-sr*cy);
	up.z = cr*cp;
}
