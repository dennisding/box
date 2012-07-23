#include "swap_chain.hpp"
#include "device.hpp"
#include "device_context.hpp"
#include "depth_stencil.hpp"

#include "game/window.hpp"
#include "utils/log.hpp"

static IDXGISwapChain *swap_chain_;
static RenderTargetPtr render_target_;
static DepthStencilPtr depth_stencil_;

static void create_render_target()
{
	ID3D11Texture2D *buffer;
	swap_chain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void **)&buffer );

	render_target_ = RenderTargetMgr::create_by_resource( "back buffer", buffer );

	buffer->Release();
}

static IDXGIFactory *get_factory()
{
	IDXGIDevice *device = 0;
	IDXGIAdapter *adapter = 0;
	IDXGIFactory *factory = 0;

	Device::device_->QueryInterface( __uuidof( IDXGIDevice ), (void **)&device );
	device->GetParent( __uuidof( IDXGIAdapter ), (void **)&adapter );
	adapter->GetParent( __uuidof( IDXGIFactory ), (void **)&factory );

	device->Release();
	adapter->Release();
	return factory;
}

static IDXGISwapChain *create_swap_chain()
{
	IDXGISwapChain *swapchain = 0;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory( &desc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	desc.BufferCount = 1;
	desc.BufferDesc.Width = GameWindow::get_width();
	desc.BufferDesc.Height = GameWindow::get_height();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = GameWindow::window_;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = !GameWindow::is_fullscreen();

	IDXGIFactory *factory = get_factory();
	if ( factory->CreateSwapChain( Device::device_, &desc, &swapchain ) != S_OK ) {
		factory->Release();
		return 0;
	}

	factory->Release();
	return swapchain;
}

void SwapChain::init()
{
	swap_chain_ = create_swap_chain();
	if ( !swap_chain_ ) {
		log_critical( "unsable to create the swap chain" );
	}

	create_render_target();
	depth_stencil_ = create_depth_stencil_by_render_target( render_target_ );

	DeviceContext::push_render_target( render_target_, depth_stencil_ );
}

void SwapChain::fini()
{
	render_target_ = 0;
	depth_stencil_ = 0;
	swap_chain_->Release();
	swap_chain_ = 0;
}

void SwapChain::present()
{
	swap_chain_->Present( 1, 0 );
}