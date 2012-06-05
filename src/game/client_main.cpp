
#include <cstdio>

#include "test.hpp"
#include "game.hpp"

int main( int argc, char **argv )
{
	// do_test();
	// return
	Game::init();

	Game::run();

	Game::fini();
}