#ifndef _LOGIC_TASK_HPP_
#define _LOGIC_TASK_HPP_

#include "prime/task_mgr.hpp"

class LogicTask : public Task
{
public:
	LogicTask() : Task( "logic" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

#endif
