#ifndef _SHADER_HELPER_HPP_
#define _SHADER_HELPER_HPP_

#include "box_utils.hpp"

#include "d3d11.h"

#include <string>

D3DBlobPtr compile_vertex_shader( const std::string &name );
D3DBlobPtr compile_pixel_shader( const std::string &name );

#endif
