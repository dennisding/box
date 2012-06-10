#ifndef _GAME_TIME_HPP_
#define _GAME_TIME_HPP_

class GameTime
{
public:
	static void init();
	static void tick();

public:
	static int logic_now_;			// 游戏启动到现在的逻辑经过的毫秒数
	static int logic_frame_diff_;
	static int render_now_;			// 游戏启动到现在的渲染毫秒数
	static int render_frame_diff_;
};

#endif
