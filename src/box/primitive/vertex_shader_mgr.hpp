#ifndef _VERTEX_SHADER_MGR_HPP_
#define _VERTEX_SHADER_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include "d3d11.h"

class VertexShader : public Object
{
public:
	VertexShader( ID3D11VertexShader *shader );
	~VertexShader();

private:
	ID3D11VertexShader *shader_;
};

typedef SmartPtr< VertexShader > VertexShaderPtr;

class VertexShaderMgr
{
public:
	static void init();
	static void fini();
	static VertexShaderPtr get( const std::string &name );
};

#endif
