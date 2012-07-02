#include "depth_stencil.hpp"
#include "device.hpp"

static ID3D11Texture2D *create_texture( int width, int height )
{
	ID3D11Texture2D *texture = 0;

	D3D11_TEXTURE2D_DESC desc;
	memset( &desc, 0, sizeof( D3D11_TEXTURE2D_DESC ) );
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	Device::device_->CreateTexture2D( &desc, 0, &texture );

	return texture;
}

DepthStencilPtr create_depth_stencil( int width, int height )
{
	ID3D11Texture2D *texture = create_texture( width, height );
	ID3D11DepthStencilView *view = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	memset( &desc, 0, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	Device::device_->CreateDepthStencilView( texture, &desc, &view );

	texture->Release();

	return new DepthStencil( view );
}

DepthStencilPtr create_depth_stencil_by_render_target( RenderTargetPtr &render_target )
{
	ID3D11Texture2D *texture = 0;
	render_target->get_view()->GetResource( (ID3D11Resource **)&texture );

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc( &desc );
	texture->Release();

	return create_depth_stencil( desc.Width, desc.Height );
}
