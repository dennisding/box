#include "effect_desc_mgr.hpp"

#include "utils/log.hpp"
#include "res_mgr/res_mgr.hpp"

#include <map>

typedef std::map< std::string, EffectDescPtr > EffectDescMap;

static EffectDescMap effect_descs_;

static const std::string read_effect_string( SectionPtr &info, const std::string &name )
{
	SectionPtr result = info->read_section( name );
	if ( !result ) {
		return "default";
	}
	
	return result->as_string();
}

static void create_effect( SectionPtr &info )
{
	EffectDescPtr effect = new EffectDesc;

	effect->input_layout_ = InputLayoutMgr::get( read_effect_string( info, "input_layout" ) );

	effect->vertex_shader_ = VertexShaderMgr::get( read_effect_string( info, "vertex_shader" ) );
	effect->vertex_shader_const_desc_ = effect->vertex_shader_->get_const_desc();
	effect->vertex_shader_sampler_state_ = SamplerStateMgr::get( read_effect_string( info, "vertex_shader_sampler_state" ) );

	effect->pixel_shader_ = PixelShaderMgr::get( read_effect_string( info, "pixel_shader" ) );
	effect->pixel_shader_const_desc_ = effect->pixel_shader_->get_const_desc();
	effect->pixel_shader_sampler_state_ = SamplerStateMgr::get( read_effect_string( info, "pixel_shader_sampler_state" ) );

	effect->blend_state_ = BlendStateMgr::get( read_effect_string( info, "blend_state" ) );
	effect->depth_stencil_state_ = DepthStencilStateMgr::get( read_effect_string( info, "depth_stencil_state" ));

	effect_descs_.insert( std::make_pair( info->read_string( "name" ), effect ) );
}

void EffectDescMgr::init()
{
	SectionPtr section = ResMgr::open_section( "system/render/effects.dog" );

	SectionVector &effects = section->read_section_vector( "effect" );
	for ( SectionVector::iterator it = effects.begin(); it != effects.end(); ++it ) {
		create_effect( *it );
	}
}

void EffectDescMgr::fini()
{
	effect_descs_.clear();
}


EffectDescPtr EffectDescMgr::get_desc( const std::string &name )
{
	EffectDescMap::iterator it = effect_descs_.find( name );
	if ( it == effect_descs_.end() ) {
		log_error( "[EffectDescMgr::get_desc] can not find the effect '%s'", name.c_str() );
		return 0;
	}
	return it->second;
}