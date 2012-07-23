#include "pixel_shader_mgr.hpp"
#include "shader_helper.hpp"
#include "device.hpp"

#include <map>

typedef std::map< std::string, PixelShaderPtr > PixelShaderMap;

static PixelShaderMap shaders_;

PixelShader::PixelShader( ID3DBlob *byte_code )
{
	Device::device_->CreatePixelShader( byte_code->GetBufferPointer(), byte_code->GetBufferSize(), 0, &shader_ );
	const_buffer_desc_ = reflect_const_buffer_desc( byte_code );
}

void PixelShaderMgr::init()
{

}

void PixelShaderMgr::fini()
{
	shaders_.clear();
}

PixelShaderPtr PixelShaderMgr::get_or_create( const std::string &name )
{
	PixelShaderMap::iterator it = shaders_.find( name );
	if ( it != shaders_.end() ) {
		return it->second;
	}

	ID3DBlob *byte_code = compile_pixel_shader( name );
	if ( !byte_code ) {
		return 0;
	}

	PixelShaderPtr shader = new PixelShader( byte_code );
	shaders_.insert( std::make_pair( name, shader ) );

	byte_code->Release();
	return shader;
}
