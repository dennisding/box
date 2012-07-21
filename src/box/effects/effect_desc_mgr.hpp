#ifndef _EFFECT_DESC_MGR_HPP_
#define _EFFECT_DESC_MGR_HPP_

#include "effect.hpp"

#include "../framework/sampler_state_mgr.hpp"
#include "../framework/blend_state_mgr.hpp"
#include "../framework/input_layout_mgr.hpp"
#include "../framework/const_buffer_desc.hpp"
#include "../framework/depth_stencil_state_mgr.hpp"
#include "../framework/vertex_shader_mgr.hpp"
#include "../framework/pixel_shader_mgr.hpp"

#include "utils/smart_ptr.hpp"

#include <string>

class EffectDesc : public Object
{
public:
	InputLayoutPtr input_layout_;

	VertexShaderPtr vertex_shader_;
	ConstBufferDescPtr vertex_shader_const_desc_;
	SamplerStatePtr vertex_shader_sampler_state_;

	PixelShaderPtr pixel_shader_;
	ConstBufferDescPtr pixel_shader_const_desc_;
	SamplerStatePtr pixel_shader_sampler_state_;

	BlendStatePtr blend_state_;
	DepthStencilStatePtr depth_stencil_state_;
};

typedef SmartPtr< EffectDesc > EffectDescPtr;

class EffectDescMgr
{
public:
	static void init();
	static void fini();
	static EffectDescPtr get_desc( const std::string &name );
};

#endif
