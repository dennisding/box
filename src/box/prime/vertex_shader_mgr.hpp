#ifndef _VERTEX_SHADER_MGR_HPP_
#define _VERTEX_SHADER_MGR_HPP_

#include "const_buffer_desc.hpp"
#include "input_layout_mgr.hpp"

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include "d3d11shader.h"
#include <string>

class VertexShader : public Object
{
public:
	VertexShader( const std::string &name, ID3DBlob *byte_code, const std::string &layout_name );
	virtual ~VertexShader();

	ConstBufferPtr create_const_buffer();
	InputLayoutPtr get_input_layout();

private:
	void setup_layout( ID3DBlob *byte_code, const std::string &layout_name );

private:
	std::string name_;
	ID3D11VertexShader *shader_;
	ConstBufferDescPtr const_buffer_desc_;
	InputLayoutPtr input_layout_;
};

typedef SmartPtr< VertexShader > VertexShaderPtr;

class VertexShaderMgr
{
public:
	static void init();
	static void fini();
	static VertexShaderPtr get_or_create( const std::string &name, const std::string &layout_name );
};

// inline
inline VertexShader::~VertexShader()
{
	shader_->Release();
}

inline ConstBufferPtr VertexShader::create_const_buffer()
{
	return const_buffer_desc_->create_buffer();
}

inline InputLayoutPtr VertexShader::get_input_layout()
{
	return input_layout_;
}

#endif
