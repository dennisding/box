
#include <cstdio>

#include "test.hpp"
#include "game.hpp"

int main( int argc, char **argv )
{
	Game::init();

	Game::run();

	Game::fini();
}