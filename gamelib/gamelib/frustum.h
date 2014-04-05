#ifndef __frustum_h__
#define __frustum_h__
//double fsqrt(double r);
#include "matrix.h"
class CFrustum
{
public:
	CFrustum() ;
	void Extract( MATRIX &mmprojw, MATRIX &mmodl) ;
	bool PointVisible( const float &x, const float &y, const float &z ) const;
	short unsigned int CubeVisible( const float &x, const float &y, const float &z, const float &size , const unsigned char &prev_vis_test = 0) const ;
	bool Changed(void) ;

private:
	double frustum[6][4];								// View Frustum
	double oldfrustum[6][4];							// View Frustum
};
#endif // __frustum_h__

