#ifndef _SHADER_HELPER_HPP_
#define _SHADER_HELPER_HPP_

#include "const_buffer_desc.hpp"

#include "d3d11.h"
#include "d3dcompiler.h"
#include <string>

ID3DBlob *compile_vertex_shader( const std::string &name );
ID3DBlob *compile_pixel_shader( const std::string &name );

ID3D11ShaderReflection *reflect_shader( ID3DBlob *byte_code );

ConstBufferDescPtr reflect_const_buffer_desc( ID3DBlob *byte_code );

#endif
