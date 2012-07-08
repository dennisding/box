#ifndef _VISUAL_HPP_
#define _VISUAL_HPP_

#include "effects/effect.hpp"

#include "utils/smart_ptr.hpp"

// for test now
class Visual : public Object
{
public:
	Visual();
	~Visual();

	void tick();
	void draw();

	void load( const std::string &name );

private:
	bool loaded_;
	Effect effect_;
};

typedef SmartPtr< Visual > VisualPtr;

#endif
