#ifndef __vector_h_
#define __vector_h_
#ifndef M_PI
const float M_PI = 3.1415926535897932384626433832795f;
#endif

/** This is used to hold 3D points and vectors. Notice it's all inline for fastest performance. */
/*
struct CVector3
{
public:
	CVector3() 								{ x=y=z=0;}
	bool IsZero()							{ return (x==0 && y==0 && z==0); };
//	CVector3 operator=(CVector3 &other)		{ other.x = x; other.y = y; other.x = z; return this; }
	CVector3 operator=(const CVector3 &other){ x=other.x; y=other.y; z=other.z; return CVector3(x,y,z); }
	CVector3 (float X, float Y, float Z)	{ x = X; y = Y; z = Z;	}
	CVector3 operator+(CVector3 vVector)	{ return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);	}
	CVector3 operator+(float num)			{ return CVector3(x + num, y + num, z + num);	}
	CVector3 operator-(CVector3 vVector)	{ return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);	}
	CVector3 operator*(float num)			{ return CVector3(x * num, y * num, z * num);	}
	CVector3 operator/(float num)			{ return CVector3(x / num, y / num, z / num);	}
	CVector3 operator*(CVector3 other)		{ return CVector3(x * other.x, y * other.y, z * other.z);	}
	bool	 operator==(CVector3 other)		{ return ((x == other.x) && (y == other.y) && (z == other.z)); }
	bool	 operator!=(CVector3 other)		{ return ((x != other.x) || (y != other.y) || (z != other.z)); }
	float operator()(int a) 
	{ 
		if (a == 0)
			return x;
		else if (a == 1)
			return y;
		else if (a == 2)
			return z;
		else return 0;
		// return m[a][b]; 
	}
	float x, y, z;
};
struct CVector3f
{
public:
	CVector3() 								{ x=y=z=0;}
	bool IsZero()							{ return (x==0 && y==0 && z==0); };
//	CVector3 operator=(CVector3 &other)		{ other.x = x; other.y = y; other.x = z; return this; }
	CVector3 operator=(const CVector3 &other){ x=other.x; y=other.y; z=other.z; return CVector3(x,y,z); }
	CVector3 (float X, float Y, float Z)	{ x = X; y = Y; z = Z;	}
	CVector3 operator+(CVector3 vVector)	{ return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);	}
	CVector3 operator+(float num)			{ return CVector3(x + num, y + num, z + num);	}
	CVector3 operator-(CVector3 vVector)	{ return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);	}
	CVector3 operator*(float num)			{ return CVector3(x * num, y * num, z * num);	}
	CVector3 operator/(float num)			{ return CVector3(x / num, y / num, z / num);	}
	CVector3 operator*(CVector3 other)		{ return CVector3(x * other.x, y * other.y, z * other.z);	}
	bool	 operator==(CVector3 other)		{ return ((x == other.x) && (y == other.y) && (z == other.z)); }
	bool	 operator!=(CVector3 other)		{ return ((x != other.x) || (y != other.y) || (z != other.z)); }
	float operator()(int a) 
	{ 
		if (a == 0)
			return x;
		else if (a == 1)
			return y;
		else if (a == 2)
			return z;
		else return 0;
		// return m[a][b]; 
	}
	float x, y, z;
};
*/
struct CVector3
{
public:
	CVector3() 								{ x=y=z=0;}
	bool IsZero()							{ return (x==0 && y==0 && z==0); };
//	CVector3 operator=(CVector3 &other)		{ other.x = x; other.y = y; other.x = z; return this; }
	CVector3 operator=(const CVector3 &other){ x=other.x; y=other.y; z=other.z; return CVector3(x,y,z); }
	CVector3 (double X, double Y, double Z)	{ x = X; y = Y; z = Z;	}
	CVector3 operator+(CVector3 vVector)	{ return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);	}
	CVector3 operator-(CVector3 vVector)	{ return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);	}

	CVector3 operator+(float num)			{ return CVector3(x + num, y + num, z + num);	}
	CVector3 operator*(float num)			{ return CVector3(x * num, y * num, z * num);	}
	CVector3 operator/(float num)			{ return CVector3(x / num, y / num, z / num);	}

	CVector3 operator+(double num)			{ return CVector3(x + num, y + num, z + num);	}
	CVector3 operator*(double num)			{ return CVector3(x * num, y * num, z * num);	}
	CVector3 operator/(double num)			{ return CVector3(x / num, y / num, z / num);	}

	CVector3 operator+(int num)			{ return CVector3(x + num, y + num, z + num);	}
	CVector3 operator*(int num)			{ return CVector3(x * num, y * num, z * num);	}
	CVector3 operator/(int num)			{ return CVector3(x / num, y / num, z / num);	}

	CVector3 operator*(CVector3 other)		{ return CVector3(x * other.x, y * other.y, z * other.z);	}

	bool	 operator==(CVector3 other)		{ return ((x == other.x) && (y == other.y) && (z == other.z)); }
	bool	 operator!=(CVector3 other)		{ return ((x != other.x) || (y != other.y) || (z != other.z)); }
	double operator()(int a) 
	{ 
		if (a == 0)
			return x;
		else if (a == 1)
			return y;
		else if (a == 2)
			return z;
		else return 0;
		// return m[a][b]; 
	}
	double x, y, z;
};


/** This is used to hold 2D points and vectors. Notice it's all inline for fastest performance. */
struct CVector2
{
public:
	CVector2() 								{ x=y=0;}
	bool IsZero()							{ return (x==0 && y==0); };
	CVector2 (float X, float Y)				{	x = X; y = Y;}
	CVector2 operator+(CVector2 vVector)	{	return CVector2(vVector.x + x, vVector.y + y);	}
	CVector2 operator-(CVector2 vVector)	{	return CVector2(x - vVector.x, y - vVector.y);	}
	CVector2 operator*(CVector2 other)		{	return CVector2(x * other.x, y * other.y);	}
	CVector2 operator+(float num)			{	return CVector2(x + num, y + num);	}
	CVector2 operator*(float num)			{	return CVector2(x * num, y * num);	}
	float x, y;
};
#define VECTOR3 CVector3
#define VECTOR2 CVector2
VECTOR3 VecToAngles( const VECTOR3 value2);
void AngleVectors (VECTOR3 angles,VECTOR3 &forward, VECTOR3 &right, VECTOR3 &up);
/*
float Magnitude(VECTOR2 vNormal);
float Magnitude(VECTOR3 vNormal);
VECTOR3 Normalize(VECTOR3 &vNormal);
VECTOR2 Normalize(VECTOR2 &vNormal);
float Distance(VECTOR3 vPoint1, VECTOR3 vPoint2);
float Distance(VECTOR2 vPoint1, VECTOR2 vPoint2);
VECTOR3 CrossProduct(VECTOR3 a, VECTOR3 b);
float DotProduct(VECTOR3 a, VECTOR3 b);
*/
#endif // __vector_h_
