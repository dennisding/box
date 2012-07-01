#ifndef _INPUT_TASK_HPP_
#define _INPUT_TASK_HPP_

#include "prime/task_mgr.hpp"

class InputTask : public Task
{
public:
	InputTask() : Task( "input" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

#endif
