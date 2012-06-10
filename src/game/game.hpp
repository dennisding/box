#ifndef _GAME_HPP_
#define _GAME_HPP_

class Game
{
public:
	static void init();
	static void fini();
	static void run();

public:
	static bool quit_;
};

#endif
