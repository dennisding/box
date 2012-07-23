#include "shader_helper.hpp"

#include "res_mgr/res_mgr.hpp"
#include "utils/log.hpp"

#include "d3dx11async.h"
#include "d3dcompiler.h"
#include <map>

typedef std::map< const void *, BinaryPtr > BinaryMap;

class ShaderInclude : public ID3DInclude
{
public:
	HRESULT __stdcall Open( D3D10_INCLUDE_TYPE include_type, LPCSTR name, LPCVOID parent, LPCVOID *data, UINT *bytes );
	HRESULT __stdcall Close( LPCVOID data );

private:
};

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

static ID3DBlob  *compile_shader( const std::string &name, BinaryPtr &bin, const std::string &profile )
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
//		D3D10_SHADER_OPTIMIZATION_LEVEL3,
		0,
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

ID3DBlob *compile_vertex_shader( const std::string &name )
{
	std::string full_name = "system/render/vs/" + name + ".vs";

	BinaryPtr bin = ResMgr::open_binary( full_name );
	if ( !bin ) {
		log_error( "[compile_vertex_shader] unable to open file '%s'", full_name.c_str() );
		return 0;
	}

	return compile_shader( full_name, bin, "vs_4_0" );
}

ID3DBlob *compile_pixel_shader( const std::string &name )
{
	std::string full_name = "system/render/ps/" + name + ".ps";

	BinaryPtr bin = ResMgr::open_binary( full_name );
	if ( !bin ) {
		log_error( "[compile_pixel_shader] unable to open file '%s'", full_name.c_str() );
		return 0;
	}

	return compile_shader( full_name, bin, "ps_4_0" );
}

static void extract_const( ConstBufferDescPtr &buffer_desc, ID3D11ShaderReflectionConstantBuffer *buffer )
{
	int index = 0;
	D3D11_SHADER_VARIABLE_DESC desc;
	ID3D11ShaderReflectionVariable *variable = 0;
	while ( true ) {
		variable = buffer->GetVariableByIndex( index );
		if ( variable->GetDesc( &desc ) != S_OK ) {
			break;
		}

		buffer_desc->add_const_desc( desc.Name, index, desc.StartOffset, desc.Size );
		++index;
	}
}

static ConstBufferDescPtr extract_const_buffer( ID3D11ShaderReflection *reflection )
{
	int index = 0;
	ID3D11ShaderReflectionConstantBuffer *buffer;
	
	D3D11_SHADER_BUFFER_DESC desc;
	ConstBufferDescPtr buffer_desc = new ConstBufferDesc;
	while ( true ) {
		buffer = reflection->GetConstantBufferByIndex( index );
		if ( buffer->GetDesc( &desc ) != S_OK ) {
			break;
		}

		extract_const( buffer_desc, buffer );
		++index;
	}

	if ( buffer_desc->empty() ) {
		return 0;
	}

	return buffer_desc;
}

ConstBufferDescPtr reflect_const_buffer_desc( ID3DBlob *byte_code )
{
	ID3D11ShaderReflection *reflection = 0;
	D3DReflect( byte_code->GetBufferPointer(), byte_code->GetBufferSize(), IID_ID3D11ShaderReflection, (void **)&reflection );

	ConstBufferDescPtr desc = extract_const_buffer( reflection );

	reflection->Release();
	
	return desc;
}
