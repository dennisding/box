#include "sampler_state_mgr.hpp"

#include <map>

typedef std::map< std::string, SamplerStatePtr > SamplerStateMap;

static SamplerStateMap samplers_;

void SamplerStateMgr::init()
{

}

void SamplerStateMgr::fini()
{
		
}

SamplerStatePtr SamplerStateMgr::get( const std::string &name )
{
	if ( name.empty() ) {
		return 0;
	}

	SamplerStateMap::iterator it = samplers_.find( name );
	if ( it == samplers_.end() ) {
		return 0;
	}

	return it->second;
}