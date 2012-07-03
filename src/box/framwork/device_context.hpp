#ifndef _DEVICE_CONTEXT_HPP_
#define _DEVICE_CONTEXT_HPP_

#include "d3d11.h"

#include "../primitive/render_target_mgr.hpp"
#include "depth_stencil.hpp"

class DeviceContext
{
public:
	static void init( ID3D11DeviceContext *context );
	static void fini();

	static void push_render_target( RenderTargetPtr &render_target, DepthStencilPtr depth_stencil = 0 );
	static void pop_render_target();
	
	static void clear_all();
};

#endif
