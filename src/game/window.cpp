#include "window.hpp"
#include "game.hpp"

#include "prime/config.hpp"

#include <cstring>

HWND GameWindow::window_;

static int width_;
static int height_;

LRESULT CALLBACK win_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lpram )
{
	switch ( msg ) {
	case WM_CLOSE:
		Game::quit_ = true;
		break;

	default:
		return DefWindowProc( hwnd, msg, wparam, lpram );
	}

	return 0;
}

static void create_window()
{
	int desktop_width = GetSystemMetrics( SM_CXSCREEN );
	int desktop_height = GetSystemMetrics( SM_CYSCREEN );

	width_ = GameConfig::read_int( "window/width", 800 );
	height_ = GameConfig::read_int( "window/height", 600 );
	const std::string title = GameConfig::read_string( "window/title" );

	RECT rect = { 0, 0, width_, height_ };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, 0 );

	GameWindow::window_ = CreateWindowEx( WS_EX_CLIENTEDGE, 
		"game_win",
		title.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX,
		(desktop_width - width_)/2,
		(desktop_height - height_)/2,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		0,
		0
		);
}

static void register_class()
{
	WNDCLASSEX wc;
	memset( &wc, 0, sizeof(wc) );

	wc.cbSize = sizeof( wc );
	wc.lpfnWndProc = win_proc;
	wc.lpszClassName = "game_win";

	RegisterClassEx( &wc );
}

void GameWindow::init()
{
	register_class();
	create_window();

	ShowWindow( window_, SW_SHOW );
}

void GameWindow::fini()
{

}

void GameWindow::tick()
{
	MSG  msg;
	while ( PeekMessage( &msg, window_, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void GameWindow::draw()
{

}

int GameWindow::get_width()
{
	return width_;
}

int GameWindow::get_height()
{
	return height_;
}
