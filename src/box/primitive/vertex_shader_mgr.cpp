#include "vertex_shader_mgr.hpp"

#include "res_mgr/res_mgr.hpp"
#include "utils/log.hpp"

#include <map>

typedef std::map< std::string, VertexShaderPtr > VertexShaderMap;

static VertexShaderMap shaders_;

VertexShader::VertexShader( ID3D11VertexShader *shader, const std::string &name ) : shader_(shader), name_(name)
{

}

VertexShader::~VertexShader()
{
	shader_->Release();
}

void VertexShaderMgr::init()
{
	SectionPtr shader_info = ResMgr::open_section( "system/shader/vertex_shader.dog" );
}

void VertexShaderMgr::fini()
{
	shaders_.clear();
}

VertexShaderPtr VertexShaderMgr::get( const std::string &name )
{
	VertexShaderMap::iterator it = shaders_.find( name );
	if ( it == shaders_.end() ) {
		log_error( "unable to create the vertex shader: %s", name.c_str() );
		return 0;
	}
	return it->second;
}
