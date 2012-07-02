#ifndef _DEPTH_STENCIL_HPP_
#define _DEPTH_STENCIL_HPP_

#include "primitive/render_target_mgr.hpp"

#include "utils/smart_ptr.hpp"

#include "d3d11.h"

class DepthStencil : public Object
{
public:
	DepthStencil( ID3D11DepthStencilView *view );
	~DepthStencil();

	ID3D11DepthStencilView * get_view();

private:
	ID3D11DepthStencilView *view_;
};

typedef SmartPtr< DepthStencil > DepthStencilPtr;

DepthStencilPtr create_depth_stencil( int width, int height );
DepthStencilPtr create_depth_stencil_by_render_target( RenderTargetPtr &render_target );

// inline
inline DepthStencil::DepthStencil( ID3D11DepthStencilView *view ) : view_( view )
{

}

inline DepthStencil::~DepthStencil()
{
	view_->Release();
}

inline ID3D11DepthStencilView * DepthStencil::get_view()
{
	return view_;
}

#endif
