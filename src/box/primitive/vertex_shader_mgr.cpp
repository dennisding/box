#include "vertex_shader_mgr.hpp"

#include "res_mgr/res_mgr.hpp"

#include <map>

typedef std::map< std::string, VertexShaderPtr > VertexShaderMap;

static VertexShaderMap shaders_;

VertexShader::VertexShader( ID3D11VertexShader *shader ) : shader_( shader )
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
