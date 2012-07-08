#include "shader_helper.hpp"
#include "device.hpp"

ID3D11VertexShader *create_vertex_shader( const std::string &name )
{
	std::string full_name = "system/render/ps/" + name + ".vs";

	// D3DX11CompileFromFile
}

ID3D11PixelShader *create_pixel_shader( const std::string &name )
{
	std::string full_name = "system/render/vs/" + name + ".ps";
}
