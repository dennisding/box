#ifndef _CVECTOR_HPP_
#define _CVECTOR_HPP_

#include <cstring>

class Vector
{
public:
	inline Vector () {}
	inline Vector( float av, float bv, float cv ) : x_(av), y_(bv), z_(cv) {}

	inline const Vector operator + ( const Vector &v ) const {
		return Vector( x_ + v.x_, y_ + v.y_, z_ + v.z_ );
	}

	inline const Vector operator - ( const Vector &v ) const {
		return Vector( x_ - v.x_, y_ - v.y_, z_ - v.z_ );
	}

	inline const Vector operator * ( float s ) const {
		return Vector( x_*s, y_*s, z_*s );
	}

	inline float dotProduct( const Vector &v ) const {
		return x_*v.x_ + y_*v.y_ + z_*v.z_;
	}

	inline const Vector crossProduct( const Vector &v ) const {
		return Vector(y_*v.z_ - z_*v.y_, z_*v.x_ - x_*v.z_, x_*v.y_ - y_*v.x_ );
	}

	inline float lengthSquare() const {
		return x_*x_ + y_*y_ + z_*z_;
	}

public:
	float x_, y_, z_;

	static Vector zero;
};

inline bool operator < ( const Vector &v1, const Vector &v2 ) 
{
	return memcmp( &v1, &v2, sizeof( Vector ) ) < 0 ;
}

#endif

