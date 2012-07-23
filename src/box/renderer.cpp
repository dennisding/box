#include "renderer.hpp"
#include "effects/effect_desc_mgr.hpp"
#include "prime/device.hpp"
#include "prime/depth_stencil_state_mgr.hpp"
#include "prime/blend_state_mgr.hpp"
#include "prime/global_const_mgr.hpp"
#include "prime/swap_chain.hpp"
#include "prime/device_context.hpp"
#include "prime/vertex_shader_mgr.hpp"
#include "prime/input_layout_mgr.hpp"

#include "game/window.hpp"
#include "utils/log.hpp"

#include "d3d11.h"

static void primitive_init()
{
	GlobalConstMgr::init();
	InputLayoutMgr::init();
	VertexShaderMgr::init();
	BlendStateMgr::init();
	DepthStencilStateMgr::init();

	EffectDescMgr::init();
}

static void primitive_fini()
{
	GlobalConstMgr::fini();
	InputLayoutMgr::fini();
	VertexShaderMgr::fini();
	BlendStateMgr::fini();
	DepthStencilStateMgr::fini();

	EffectDescMgr::fini();
}

void Renderer::init()
{
	// create the device and render context
	ID3D11Device *device = 0;
	ID3D11DeviceContext *context = 0;
	D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_11_0;

	HRESULT result = D3D11CreateDevice( 0, 
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		&feature,
		1,
		D3D11_SDK_VERSION,
		&device,
		0,
		&context 
		);
	if ( result != S_OK ) {
		log_critical( "unable to create d3d device!" );
	}

	Device::init( device );
	DeviceContext::init( context );
	SwapChain::init();
	
	primitive_init();
}

void Renderer::fini()
{
	primitive_fini();

	SwapChain::fini();
	DeviceContext::fini();
	Device::fini();
}

void Renderer::tick()
{
	DeviceContext::clear_all();

	SwapChain::present();
}

void Renderer::draw()
{

}

void Renderer::draw_item( VisibleItemPtr &item )
{

}
