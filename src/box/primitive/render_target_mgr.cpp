#include "render_target_mgr.hpp"
#include "../framwork/device.hpp"

#include "utils/log.hpp"

RenderTarget::RenderTarget( const std::string &name, ID3D11RenderTargetView *view ) : name_( name ), view_( view )
{

}

RenderTarget::~RenderTarget()
{
	view_->Release();
}

RenderTargetPtr RenderTargetMgr::create( const std::string &name, int width, int height )
{
	log_critical( "not yet implement!" );
	return 0;
}

RenderTargetPtr RenderTargetMgr::create_by_resource( const std::string &name, ID3D11Resource *resource )
{
	ID3D11RenderTargetView *view;

	D3D11_RENDER_TARGET_VIEW_DESC desc;
	memset( &desc, 0, sizeof( D3D11_RENDER_TARGET_VIEW_DESC ) );
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	if ( Device::device_->CreateRenderTargetView( resource, &desc, &view ) != 0 ) {
		return 0;
	}
	
	return new RenderTarget( name, view );
}