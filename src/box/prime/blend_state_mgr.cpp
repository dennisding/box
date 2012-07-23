#include "blend_state_mgr.hpp"

#include "device.hpp"

#include <map>

typedef std::map< std::string, BlendStatePtr > BlendStateMap;

BlendStateMap blend_states_;

static D3D11_BLEND_DESC &get_default_desc()
{
	static D3D11_BLEND_DESC desc;
	memset( &desc, 0, sizeof( D3D11_BLEND_DESC ) );
	desc.AlphaToCoverageEnable = FALSE;
	desc.IndependentBlendEnable = FALSE;
	desc.RenderTarget[0].BlendEnable = FALSE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return desc;
}

static BlendStatePtr create_blend_state( D3D11_BLEND_DESC &desc )
{
	ID3D11BlendState *blend_state = 0;

	Device::device_->CreateBlendState( &desc, &blend_state );

	return new BlendState( blend_state );
}

void BlendStateMgr::init()
{
	blend_states_[ "default" ] = create_blend_state( get_default_desc() );
}

void BlendStateMgr::fini()
{
	blend_states_.clear();
}

BlendStatePtr BlendStateMgr::get( const std::string &name )
{
	BlendStateMap::iterator it = blend_states_.find( name );
	if ( it != blend_states_.end() ) {
		return it->second;
	}
	return 0;
}