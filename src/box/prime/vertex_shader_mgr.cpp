#include "vertex_shader_mgr.hpp"
#include "shader_helper.hpp"
#include "device.hpp"

#include "utils/log.hpp"

#include "d3dcompiler.h"
#include <map>

typedef std::map< std::string, VertexShaderPtr > VertexShaderMap;

static VertexShaderMap shaders_;

VertexShader::VertexShader( const std::string &name, ID3DBlob *byte_code, const std::string &layout_name ) 
	: name_( name ), shader_(0)
{
	Device::device_->CreateVertexShader( byte_code->GetBufferPointer(), byte_code->GetBufferSize(), 0, &shader_ );

	const_buffer_desc_ = reflect_const_buffer_desc( byte_code );

	setup_layout( byte_code, layout_name );
}

void VertexShader::setup_layout( ID3DBlob *byte_code, const std::string &layout_name )
{
	input_layout_ = InputLayoutMgr::get( layout_name );
	if ( !input_layout_ ) {
		input_layout_ = InputLayoutMgr::create( byte_code, layout_name );
	}
	
	if ( !input_layout_ ) {
		log_error( "[VertexShader::setup_layout] invalid input layout '%s'", layout_name.c_str() );
	}
}

void VertexShaderMgr::init()
{
}

void VertexShaderMgr::fini()
{
	shaders_.clear();
}

VertexShaderPtr VertexShaderMgr::get_or_create( const std::string &name, const std::string &layout_name )
{
	VertexShaderMap::iterator it = shaders_.find( name );
	if ( it != shaders_.end() ) {
		return it->second;
	}

	ID3DBlob *byte_code = compile_vertex_shader( name );
	if ( !byte_code ) {
		return 0;
	}

	VertexShaderPtr shader = new VertexShader( name, byte_code, layout_name );

	shaders_.insert( std::make_pair( name, shader ) );
	
	byte_code->Release();
	return shader;
}
