#include "depth_stencil_state_mgr.hpp"
#include "device.hpp"

#include <map>

typedef std::map< std::string, DepthStencilStatePtr > DepthStencilStateMap;

DepthStencilStateMap depth_stencil_states_;

static D3D11_DEPTH_STENCIL_DESC &get_default_desc()
{
	static D3D11_DEPTH_STENCIL_DESC desc;
	memset( &desc, 0, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	desc.DepthEnable = TRUE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = FALSE;

	return desc;
}

static DepthStencilStatePtr create_state( D3D11_DEPTH_STENCIL_DESC &desc )
{
	ID3D11DepthStencilState *state = 0;
	Device::device_->CreateDepthStencilState( &desc, &state );
	
	return new DepthStencilState( state );
}

void DepthStencilStateMgr::init()
{
	depth_stencil_states_[ "default" ] = create_state( get_default_desc() );
}

void DepthStencilStateMgr::fini()
{
	depth_stencil_states_.clear();
}

DepthStencilStatePtr DepthStencilStateMgr::get( const std::string &name )
{
	DepthStencilStateMap::iterator it = depth_stencil_states_.find( name );
	if ( it != depth_stencil_states_.end() ) {
		return it->second;
	}
	return 0;
}