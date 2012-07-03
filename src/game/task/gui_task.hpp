#ifndef _GUI_TASK_HPP_
#define _GUI_TASK_HPP_

#include "prime/task_mgr.hpp"

class GuiTask : public Task
{
public:
	GuiTask() : Task( "gui" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

#endif
