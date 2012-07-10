#include "shader_helper.hpp"
#include "device.hpp"

#include "res_mgr/res_mgr.hpp"
#include "utils/log.hpp"

#include "d3dcompiler.h"

class ShaderInclude : public ID3DInclude
{
public:
	HRESULT __stdcall Open( D3D10_INCLUDE_TYPE include_type, LPCSTR name, LPCVOID parent, LPCVOID *data, UINT *bytes );
	HRESULT __stdcall Close( LPCVOID data );

private:
	BinaryPtr binary_;
};

static ID3D11VertexShader *compile_shader( BinaryPtr &bin, const std::string &profile )
{
	return 0;
}

ID3D11VertexShader *create_vertex_shader( const std::string &name )
{
	std::string full_name = "system/render/ps/" + name + ".vs";

	// D3DX11CompileFromFile
	BinaryPtr bin = ResMgr::open_binary( full_name );
	if ( !bin ) {
		log_error( "[create_vertex_shader] unable to open file '%s'", full_name.c_str() );
		return 0;
	}

	return compile_shader( bin, "vs_4_0" );
}

ID3D11PixelShader *create_pixel_shader( const std::string &name )
{
	std::string full_name = "system/render/vs/" + name + ".ps";

	return 0;
}

HRESULT ShaderInclude::Open( D3D10_INCLUDE_TYPE include_type, LPCSTR name, LPCVOID parent, LPCVOID *data, UINT *bytes )
{
	std::string full_name = std::string( "system/render/include/" ) + name;
	binary_ = ResMgr::open_binary( full_name );
	if ( !binary_ ) {
		return S_FALSE;
	}

	*data = binary_->get_buffer();
	*bytes = binary_->get_length();

	return S_OK;
}

HRESULT ShaderInclude::Close( LPCVOID data )
{
	binary_ = 0;

	return S_OK;
}
