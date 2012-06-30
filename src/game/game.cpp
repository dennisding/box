#include "game.hpp"
#include "input_task.hpp"
#include "prime_task.hpp"
#include "gui_task.hpp"
#include "logic_task.hpp"
#include "renderer_task.hpp"

#include "utils/log.hpp"
#include "prime/config.hpp"
#include "prime/task_mgr.hpp"
#include "res_mgr/file_system.hpp"

#include <Windows.h>

bool Game::quit_ = false;

static TaskMgr task_mgr_;

static void init_settings()
{
	// init the res_mgr
	SectionPtr path_section = parse_section( read_native_file( "path.dog" ) );
	if ( path_section ) {
		ResMgr::init( path_section->read_string( "path" ) );
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
	TaskMgr::add_task( TaskPtr( new FirstTask ) );
	TaskMgr::add_task( TaskPtr( new InputTask ) );
	TaskMgr::add_task( TaskPtr( new GuiTask ) );
	TaskMgr::add_task( TaskPtr( new LogicTask ) );
	TaskMgr::add_task( TaskPtr( new RendererTask ) );
	TaskMgr::add_task( TaskPtr( new FinalTask ) );
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
	TaskMgr::init();

	while ( !quit_ ) {
		int begin_time = GetTickCount();

		TaskMgr::tick();
		TaskMgr::draw();

		int end_time = GetTickCount();
		int sleep_time = int(16.66666 - ( end_time - begin_time ));
		if ( sleep_time >= 0 ) {
			Sleep( sleep_time );
		}

	}

	TaskMgr::fini();
}
