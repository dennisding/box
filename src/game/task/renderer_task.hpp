#ifndef _RENDERER_TASK_HPP_
#define _RENDERER_TASK_HPP_

#include "prime/task_mgr.hpp"

class RendererTask : public Task
{
public:
	RendererTask() : Task( "renderer" )
	{

	}

	virtual void init();
	virtual void fini();
	virtual void tick();
	virtual void draw();
};

#endif
