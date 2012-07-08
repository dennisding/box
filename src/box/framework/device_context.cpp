#include "device_context.hpp"

#include "utils/log.hpp"

#include <list>

typedef std::list< RenderTargetPtr > RenderTargetList;
struct RenderBuffer
{
	RenderBuffer( RenderTargetPtr &render_target, DepthStencilPtr &depth_stencil ) :
			render_target_( render_target ), depth_stencil_( depth_stencil ) 
	{

	}

	RenderTargetPtr render_target_;
	DepthStencilPtr depth_stencil_;
};

typedef std::list< RenderBuffer > RenderBufferList;

static ID3D11DeviceContext *context_;
static RenderBufferList render_buffers_;

void DeviceContext::init( ID3D11DeviceContext *context )
{
	context_ = context;
}

void DeviceContext::fini()
{
	context_->Release();
	context_ = 0;
	render_buffers_.clear();
}

void DeviceContext::push_render_target( RenderTargetPtr &render_target, DepthStencilPtr depth_stencil )
{
	if ( !depth_stencil ) {
		depth_stencil = render_buffers_.rbegin()->depth_stencil_;
	}
	
	render_buffers_.push_back( RenderBuffer( render_target, depth_stencil ) );

	ID3D11RenderTargetView *view = render_target->get_view();
	context_->OMSetRenderTargets( 1, &view, depth_stencil->get_view() );
}

void DeviceContext::pop_render_target()
{
	if ( render_buffers_.empty() ) {
		log_critical( "pop from empty render target stack" );
	}

	render_buffers_.pop_back();

	RenderBuffer &buffer = *(render_buffers_.rbegin());
	ID3D11RenderTargetView *view = buffer.render_target_->get_view();
	context_->OMSetRenderTargets( 1, &view, buffer.depth_stencil_->get_view() );
}

void DeviceContext::clear_all()
{
	RenderBuffer &buffer = *render_buffers_.rbegin();
	float color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
	context_->ClearDepthStencilView( buffer.depth_stencil_->get_view(), 0, 0, 0 );
	context_->ClearRenderTargetView( buffer.render_target_->get_view(), color );
}
