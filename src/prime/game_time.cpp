#include "game_time.hpp"

#include <Windows.h>

int GameTime::logic_now_;
int GameTime::logic_frame_diff_;
int GameTime::render_now_;
int GameTime::render_frame_diff_;

static DWORD last_time_;

void GameTime::init()
{
	last_time_ = GetTickCount();
}

void GameTime::tick()
{
	DWORD now_time = GetTickCount();
	DWORD diff = now_time - last_time_;
	last_time_ = now_time;

	// set the logic time
	logic_now_ += diff;
	logic_frame_diff_ = diff;

	// set the render time
	render_now_ += diff;
	render_frame_diff_ = diff;
}