#include "prime_task.hpp"
#include "../window.hpp"

#include "prime/game_time.hpp"
#include "prime/thread_task_mgr.hpp"
#include "network/network.hpp"

void FirstTask::init()
{
	GameTime::init();

	GameWindow::init();
	Network::init();
}

void FirstTask::fini()
{
	ThreadTaskMgr::fini();

	GameWindow::fini();
	Network::fini();
}

void FirstTask::tick()
{
	GameTime::tick();
	ThreadTaskMgr::tick();
	
	GameWindow::tick();
	Network::tick();
}

void FirstTask::draw()
{
	GameWindow::draw();
	Network::draw();
}

void FinalTask::init()
{

}

void FinalTask::fini()
{

}

void FinalTask::tick()
{

}

void FinalTask::draw()
{

}