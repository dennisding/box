#ifndef _VERTEX_SHADER_MGR_HPP_
#define _VERTEX_SHADER_MGR_HPP_

#include "../framework/box_utils.hpp"
#include "../framework/const_buffer_desc.hpp"

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class VertexShader : public Object
{
public:
	VertexShader( const std::string &name );

	bool init( D3DBlobPtr &byte_code );
	
	ConstBufferDescPtr get_const_desc() { return 0; }

private:
	std::string name_;
	D3DBlobPtr byte_code_;
	D3D11VertexShaderPtr vertex_shader_;
};

typedef SmartPtr< VertexShader > VertexShaderPtr;

class VertexShaderMgr
{
public:
	static void init();
	static void fini();
	static VertexShaderPtr get( const std::string &name );
	static VertexShaderPtr get_or_create( const std::string &name );
};

#endif
