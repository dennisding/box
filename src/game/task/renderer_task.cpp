#include "renderer_task.hpp"

#include "box/renderer.hpp"

void RendererTask::init()
{
	Renderer::init();
}

void RendererTask::fini()
{
	Renderer::fini();
}

void RendererTask::tick()
{
	Renderer::tick();
}

void RendererTask::draw()
{
	Renderer::draw();
}
