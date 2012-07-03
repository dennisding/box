#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "visible_item.hpp"

class Renderer
{
public:
	static void init();
	static void fini();
	static void tick();
	static void draw();

	static void draw_item( VisibleItemPtr &item );
};

#endif
