//***********************************************************************//
//
//		$Author:			Spikeles
//
//		$Program:			MapEdit
//
//		$Description: Frustum Code
//
//		$Credits:     http://www.markmorley.com/opengl/frustumculling.html
//
//		$Date:				25/3/2002
//
//***********************************************************************//
#include "frustum.h"
//#include <windows.h>
#include <math.h>
//#include <gl/gl.h>
//#include "memleakcheck.h"
//#include "profile.h"
//#include <math.h>								// Fast math routines
extern bool use_temporal_coherence;
bool use_sphere_test = false;
static unsigned char bit_translate_table[6]={32,16,8,4,2,1};
static unsigned char num_translate_table[32]={0,5,4,0,3,0,0,0,2,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const short unsigned int CUBE_INSIDE = 10;
const short unsigned int CUBE_INTERSECT = 11;

CFrustum::CFrustum()
{
	for (int i=0;i<6;i++)
		for (int a=0;a<4;a++)
		{
			oldfrustum[i][a]=1;
			frustum[i][a]=2;
		}
}
bool CFrustum::Changed()
{
	if (memcmp(oldfrustum,frustum,(sizeof(float)*(6*4)))==0)
		return false;
	return true;
}
// Supposed to be *really* fast square root function... 
/*float s_sqrt(double r)
{
	double x,y;
	double tempf;
	unsigned long *tfptr = ((unsigned long *)&tempf)+1;

	tempf = r;
	*tfptr=(0xbfcd4600-*tfptr)>>1;
	x=tempf;
	y=r*0.5;
	x*=1.5-x*x*y;
	x*=1.5-x*x*y;
	x*=1.5-x*x*y;
	x*=1.5-x*x*y;
	return x*r;
}
*/
// #define flip(x) 
/*inline float flip(float x)
{
	if (x<0) return (abs(x)); else if (x>0)  return (-(x));
}*/
void CFrustum::Extract(MATRIX &mproj, MATRIX &mmodl)
{
	//Profile Func("CFrustum::Extract");
	memcpy(oldfrustum,frustum,(sizeof(double)*(6*4)));
	double *proj = mproj.GetMatrix();
	double *modl = mmodl.GetMatrix();
	//float   clip[16];
	double   t;
	MATRIX m_clip = mproj * mmodl;
	double *clip = m_clip.GetMatrix();
	/* Combine the two matrices (multiply projection by modelview) */
	/*	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	*/	
	/* Extract the numbers for the RIGHT plane */
	frustum[0][0] = clip[ 3] - clip[ 0];
	frustum[0][1] = clip[ 7] - clip[ 4];
	frustum[0][2] = clip[11] - clip[ 8];
	frustum[0][3] = clip[15] - clip[12];

	/* Normalize the result */
	t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	/* Extract the numbers for the LEFT plane */
	frustum[1][0] = clip[ 3] + clip[ 0];
	frustum[1][1] = clip[ 7] + clip[ 4];
	frustum[1][2] = clip[11] + clip[ 8];
	frustum[1][3] = clip[15] + clip[12];

	/* Normalize the result */
	t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	/* Extract the BOTTOM plane */
	frustum[2][0] = clip[ 3] + clip[ 1];
	frustum[2][1] = clip[ 7] + clip[ 5];
	frustum[2][2] = clip[11] + clip[ 9];
	frustum[2][3] = clip[15] + clip[13];

	/* Normalize the result */
	t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	/* Extract the TOP plane */
	frustum[3][0] = clip[ 3] - clip[ 1];
	frustum[3][1] = clip[ 7] - clip[ 5];
	frustum[3][2] = clip[11] - clip[ 9];
	frustum[3][3] = clip[15] - clip[13];

	/* Normalize the result */
	t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	/* Extract the FAR plane */
	frustum[4][0] = clip[ 3] - clip[ 2];
	frustum[4][1] = clip[ 7] - clip[ 6];
	frustum[4][2] = clip[11] - clip[10];
	frustum[4][3] = clip[15] - clip[14];

	/* Normalize the result */
	t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	/* Extract the NEAR plane */
	frustum[5][0] = clip[ 3] + clip[ 2];
	frustum[5][1] = clip[ 7] + clip[ 6];
	frustum[5][2] = clip[11] + clip[10];
	frustum[5][3] = clip[15] + clip[14];

	/* Normalize the result */
	t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}

bool CFrustum::PointVisible( const float &x, const float &y, const float &z ) const
{
	register short unsigned int p;

	for( p = 0; p < 6; p++ )
	{
		if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
			return false;
	}
	return true;
}

short unsigned int CFrustum::CubeVisible( const float &x, const float &y, const float &z, const float &size, const unsigned char &prev_vis_test) const
{
//	Profile Func("CFrustum::CubeVisible");
	register short unsigned int p;
	short unsigned t;
	short unsigned pp[6]={0,1,2,3,4,5};
	// R , L , B , T , F , N	
	if (use_temporal_coherence)
	{
//		short unsigned int a = prev_vis_test-1;
/*		if (a>=1)
		{
			t = pp[0];
			pp[0] = pp[a];
			pp[a] = t;
		}
*/
		/*			if (a!=0 && a!=32) // If we failed last test
		{
		a = num_translate_table[a];
		t = pp[0];
		pp[0] = pp[a];
		pp[a] = t;
		}
		*/
	}

	//	/*	
	if (use_sphere_test)
	{
		//		for( p = 0; p < 6; p++ )
		//		{
		for( short unsigned int i = 0; i < 6; i++ )
		{
			p = pp[i];
			if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] < -size )
				return p+1;
		}
		return 0;
	}
	else
	{
		//*/		
		float plusz =  z + size;
		float minusz = z - size;	 
		float plusy = y + size;
		float minusy = y - size;
		float plusx = x + size;
		float minusx = x - size;
		float p0,p1,p2,p3;
		float p0xminusx, p0xplusx, p1xminusy, p1xplusy, p2xminusz, p2xplusz;
		float p2mzp3, p2pzp3;
		unsigned char c = 0 , c2 = 0, failed_planes = 0;

		for( short unsigned int i = 0; i < 6; i++ )
		{
			p = pp[i];
			c=0;
			p0 = frustum[p][0];
			p1 = frustum[p][1];
			p2 = frustum[p][2];
			p3 = frustum[p][3];
			p0xminusx = p0 * minusx;			p0xplusx = p0 * plusx;
			p1xminusy = p1 * minusy;			p1xplusy = p1 * plusy;
			p2xminusz = p2 * minusz;			p2xplusz = p2 * plusz;
			p2mzp3 = p2xminusz + p3;			p2pzp3 = p2xplusz + p3;
			//			if (p0xminusx<0 && p1xminusy<0 && p2mzp3<0 && p0xplusx <0 && p1xplusy && p2pzp3<0)
			//				return bit_translate_table[p];

			if( p0xminusx + p1xminusy + p2mzp3 > 0 )	c++;
			if( p0xminusx + p1xminusy + p2pzp3 > 0 )	c++;
			if( p0xminusx + p1xplusy  + p2pzp3 > 0 )	c++;
			if( p0xminusx + p1xplusy  + p2mzp3 > 0 )	c++;
			if( p0xplusx  + p1xminusy + p2mzp3 > 0 )	c++;
			if( p0xplusx  + p1xminusy + p2pzp3 > 0 )	c++;
			if( p0xplusx  + p1xplusy  + p2pzp3 > 0 )	c++;
			if( p0xplusx  + p1xplusy  + p2mzp3 > 0 )	c++;
			if( c == 0 )
				return 255;		// Points outside plane
			else if( c == 8 )
				c2++;			// All points in front of place
			else				// A point is missing out on the plane! 
				failed_planes = failed_planes | bit_translate_table[p];
		}
		if (c2 == 6)
			return 0;
			// 0 out, 10 total in, 11 intersect
		else return failed_planes;
		//return (c2 == 6) ? CUBE_INSIDE : CUBE_INTERSECT;
			//return p+1;//bit_translate_table[p];
			//next_plane:
			//c2 = c2 | bit_translate_table[p];
			//			;
		//}
		//return 0;
	}
}
