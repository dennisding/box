#include "game.hpp"

#include "utils/log.hpp"
#include "prime/config.hpp"
#include "prime/task_mgr.hpp"
#include "res_mgr/file_system.hpp"

bool Game::quit_ = false;

static TaskMgr task_mgr_;

static void init_settings()
{
	// init the res_mgr
	SectionPtr path_section = parse_section( read_native_file( "path.dog" ) );
	if ( path_section ) {
		ResMgr::init( path_section->read_string( "res_path" ) );
	} else {
		ResMgr::init( "../res/" );
	}

	// init game settings
	BinaryPtr bin = read_native_file( "settings.dog" );
	SectionPtr sec = parse_section( bin );
	if ( !sec ) {
		log_critical( "can not read game settings!" );
	}

	GameConfig::init( sec );
}

static void init_tasks()
{

}

void Game::init()
{
	init_settings();
	init_tasks();
}

void Game::fini()
{
}

void Game::run()
{
	Network::init();
	GameWindow::init();
	TaskMgr::init();

	while ( !quit_ ) {

		GameWindow::process_message();

		TaskMgr::tick();
		TaskMgr::draw();
	}

	TaskMgr::fini();
	GameWindow::fini();
	Network::fini();
}
