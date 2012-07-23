#ifndef _EFFECT_DESC_MGR_HPP_
#define _EFFECT_DESC_MGR_HPP_

#include "effect.hpp"

#include "../prime/sampler_state_mgr.hpp"
#include "../prime/blend_state_mgr.hpp"
#include "../prime/input_layout_mgr.hpp"
#include "../prime/const_buffer_desc.hpp"
#include "../prime/depth_stencil_state_mgr.hpp"
#include "../prime/vertex_shader_mgr.hpp"
#include "../prime/pixel_shader_mgr.hpp"

#include "utils/smart_ptr.hpp"

#include <string>

class EffectDesc : public Object
{
public:
	VertexShaderPtr vertex_shader_;
	SamplerStatePtr vertex_shader_sampler_state_;

	PixelShaderPtr pixel_shader_;
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
