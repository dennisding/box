#include "vertex_shader_mgr.hpp"
#include "../framework/shader_helper.hpp"
#include "../framework/device.hpp"

#include "res_mgr/res_mgr.hpp"
#include "utils/log.hpp"

#include <map>

typedef std::map< std::string, VertexShaderPtr > VertexShaderMap;

static VertexShaderMap shaders_;

VertexShader::VertexShader( const std::string &name ) : name_( name )
{

}

bool VertexShader::init( D3DBlobPtr &byte_code )
{
	byte_code_ = byte_code;

	ID3D11VertexShader *shader = 0;
	Device::device_->CreateVertexShader( byte_code->GetBufferPointer(), byte_code->GetBufferSize(), 0, &shader );
	vertex_shader_ = D3D11VertexShaderPtr( shader );

	return true;
}

void VertexShaderMgr::init()
{
}

void VertexShaderMgr::fini()
{
	shaders_.clear();
}

VertexShaderPtr VertexShaderMgr::get( const std::string &name )
{
	VertexShaderMap::iterator it = shaders_.find( name );
	if ( it == shaders_.end() ) {
		return 0;
	}
	return it->second;
}

VertexShaderPtr VertexShaderMgr::get_or_create( const std::string &name )
{
	VertexShaderPtr shader = get( name );
	if ( shader ) {
		return shader;
	}

	D3DBlobPtr byte_code = compile_vertex_shader( name );
	if ( !byte_code ) {
		log_error( "[VertexShaderMgr::get_or_create] unabel to compile shader '%s'", name.c_str() );
		return 0;
	}

	shader = new VertexShader( name );
	if ( !shader->init( byte_code ) ) {
		log_error( "[VertexShaderMgr::get_or_create] unable to create vertex shader '%s'", name.c_str() );
		return 0;
	}

	shaders_.insert( std::make_pair( name, shader ) );
	return shader;
}
