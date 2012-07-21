#ifndef _PIXEL_SHADER_MGR_HPP_
#define _PIXEL_SHADER_MGR_HPP_

#include "const_buffer_desc.hpp"

#include "utils/smart_ptr.hpp"

#include <string>

class PixelShader : public Object
{
public:
	ConstBufferDescPtr get_const_desc() { return 0; }
};

typedef SmartPtr< PixelShader > PixelShaderPtr;

class PixelShaderMgr
{
public:
	static PixelShaderPtr get( const std::string &name ) { return 0; }
};

#endif
