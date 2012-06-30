#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "windows.h"

class GameWindow
{
public:
	static void init();
	static void fini();
	static void tick();
	static void draw();

	static int get_width();
	static int get_height();

public:
	static HWND window_;
};

#endif
