#include "shader_helper.hpp"

#include "res_mgr/res_mgr.hpp"
#include "utils/log.hpp"

#include "d3dx11async.h"
#include <map>

typedef std::map< const void *, BinaryPtr > BinaryMap;

class ShaderInclude : public ID3DInclude
{
public:
	HRESULT __stdcall Open( D3D10_INCLUDE_TYPE include_type, LPCSTR name, LPCVOID parent, LPCVOID *data, UINT *bytes );
	HRESULT __stdcall Close( LPCVOID data );

private:
};

static D3DBlobPtr compile_shader( const std::string &name, BinaryPtr &bin, const std::string &profile )
{
	ID3D10Blob *blob = 0;
	ID3D10Blob *error = 0;
	ShaderInclude includer;

	HRESULT result = D3DX11CompileFromMemory( bin->get_buffer(),
		bin->get_length(),
		name.c_str(),
		0,
		&includer,
		"main",
		profile.c_str(),
		D3D10_SHADER_OPTIMIZATION_LEVEL3,
		0,
		0,
		&blob,
		&error,
		0 );
	
	if ( result == S_OK ) {
		return blob;
	}
	
	log_error( "%s", error->GetBufferPointer() );
	error->Release();
	return 0;
}

D3DBlobPtr compile_vertex_shader( const std::string &name )
{
	std::string full_name = "system/render/vs/" + name + ".vs";

	// D3DX11CompileFromFile
	BinaryPtr bin = ResMgr::open_binary( full_name );
	if ( !bin ) {
		log_error( "[create_vertex_shader] unable to open file '%s'", full_name.c_str() );
		return 0;
	}

	// return compile_shader( bin, "vs_4_0" );
	D3DBlobPtr byte_code = compile_shader( full_name, bin, "vs_4_0" );
	if ( !byte_code ) {
		log_error( "[create_vertex_shader] can not compile shader '%s'", full_name.c_str() );
		return 0;
	}

	return byte_code;
}

D3DBlobPtr compile_pixel_shader( const std::string &name )
{
	std::string full_name = "system/render/vs/" + name + ".ps";

	return 0;
}

HRESULT ShaderInclude::Open( D3D10_INCLUDE_TYPE include_type, LPCSTR name, LPCVOID parent, LPCVOID *data, UINT *bytes )
{
	std::string full_name = std::string( "system/render/include/" ) + name;
	BinaryPtr bin = ResMgr::open_binary( full_name );
	if ( !bin ) {
		return S_FALSE;
	}

	if ( bin->get_length() == 0 ) {
		*data = 0;
		*bytes = 0;
		return S_OK;
	}

	*data = bin->get_buffer();
	*bytes = bin->get_length();

	char *temp = new char[ bin->get_length() ];
	memcpy( temp, bin->get_buffer(), bin->get_length() );
	
	*data = temp;
	*bytes = bin->get_length();

	return S_OK;
}

HRESULT ShaderInclude::Close( LPCVOID data )
{
	delete []data;
	return S_OK;
}
