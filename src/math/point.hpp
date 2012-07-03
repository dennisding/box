#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <cstring>

class Point
{
public:
	Point() {}
	Point( float x, float y = 0 ) : x_(x), y_(y) {}

public:
	float x_;
	float y_;
};

inline bool operator < ( const Point &p1, const Point &p2 )
{
	return memcmp( &p1, &p2, sizeof( Point ) ) < 0;
}

#endif
