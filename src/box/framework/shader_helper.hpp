#ifndef _SHADER_HELPER_HPP_
#define _SHADER_HELPER_HPP_

#include "d3d11.h"

#include <string>

ID3D11VertexShader *create_vertex_shader( const std::string &name );
ID3D11PixelShader *create_pixel_shader( const std::string &name );

#endif
