#ifndef _PIXEL_SHADER_MGR_HPP_
#define _PIXEL_SHADER_MGR_HPP_

#include "const_buffer_desc.hpp"

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class PixelShader : public Object
{
public:
	PixelShader( ID3DBlob *byte_code );
	virtual ~PixelShader();

private:
	ID3D11PixelShader *shader_;
	ConstBufferDescPtr const_buffer_desc_;
};

typedef SmartPtr< PixelShader > PixelShaderPtr;

class PixelShaderMgr
{
public:
	static void init();
	static void fini();
	static PixelShaderPtr get_or_create( const std::string &name );
};

// inline
inline PixelShader::~PixelShader()
{
	shader_->Release();
}


#endif
