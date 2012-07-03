#ifndef _PRIME_TASK_HPP_
#define _PRIME_TASK_HPP_

#include "prime/task_mgr.hpp"

class FirstTask : public Task
{
public:
	FirstTask() : Task( "first_task" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

class FinalTask : public Task
{
public:
	FinalTask() : Task( "final_task" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

#endif
