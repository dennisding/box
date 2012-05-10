#ifndef _CVECTOR_HPP_
#define _CVECTOR_HPP_

#include <cstring>

class Vector
{
public:
	inline Vector () {}
	inline Vector( float av, float bv, float cv ) : a(av), b(bv), c(cv) {}

	inline const Vector operator + ( const Vector &v ) const {
		return Vector( a + v.a, b + v.b, c + v.c );
	}

	inline const Vector operator - ( const Vector &v ) const {
		return Vector( a - v.a, b - v.b, c - v.c );
	}

	inline const Vector operator * ( float s ) const {
		return Vector( a*s, b*s, c*s );
	}

	inline float dotProduct( const Vector &v ) const {
		return a*v.a + b*v.b + c*v.c;
	}

	inline const Vector crossProduct( const Vector &v ) const {
		return Vector(b*v.c - c*v.b, c*v.a - a*v.c, a*v.b - b*v.a );
	}

	inline float lengthSquare() const {
		return a*a + b*b + c*c;
	}

public:
	float a, b, c;

	static Vector zero;
};

inline bool operator < ( const Vector &v1, const Vector &v2 ) 
{
	return memcmp( &v1, &v2, sizeof( Vector ) ) < 0 ;
}

#endif

