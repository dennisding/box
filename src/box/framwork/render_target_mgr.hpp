#ifndef _RENDER_TARGET_MGR_HPP_
#define _RENDER_TARGET_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class RenderTarget : public Object
{
public:
	RenderTarget( const std::string &name, ID3D11RenderTargetView *view = 0 );
	~RenderTarget();

	ID3D11RenderTargetView *get_view();

private:
	std::string name_;
	ID3D11RenderTargetView *view_;
};

typedef SmartPtr< RenderTarget > RenderTargetPtr;

class RenderTargetMgr
{
public:
	static RenderTargetPtr create( const std::string &name, int width, int height );
	static RenderTargetPtr create_by_resource( const std::string &name, ID3D11Resource *resource );
};

// inline
inline ID3D11RenderTargetView *RenderTarget::get_view()
{
	return view_;
}

#endif
