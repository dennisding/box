#ifndef _GAME_TIME_HPP_
#define _GAME_TIME_HPP_

class GameTime
{
public:
	static void init();
	static void tick();

public:
	static int logic_now_;			// ��Ϸ���������ڵ��߼������ĺ�����
	static int logic_frame_diff_;
	static int render_now_;			// ��Ϸ���������ڵ���Ⱦ������
	static int render_frame_diff_;
};

#endif
