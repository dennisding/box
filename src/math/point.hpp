#ifndef _POINT_HPP_
#define _POINT_HPP_

class Point
{
public:
	Point() {}
	Point( float x, float y = 0 ) : x_(x), y_(y) {}

public:
	float x_;
	float y_;
};

#endif
