#include "input_task.hpp"
#include "window.hpp"

#include "network/network.hpp"

void InputTask::init()
{
	GameWindow::init();
	Network::init();
}

void InputTask::fini()
{
	Network::fini();
	GameWindow::fini();
}

void InputTask::tick()
{
	GameWindow::tick();
	Network::tick();
}

void InputTask::draw()
{
	GameWindow::draw();
	Network::draw();
}
