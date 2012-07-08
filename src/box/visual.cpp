#include "visual.hpp"

#include "utils/log.hpp"
#include "res_mgr/res_mgr.hpp"

Visual::Visual() : loaded_( false )
{

}

Visual::~Visual()
{

}

void Visual::tick()
{
	if ( !loaded_ ) {
		return;
	}
}

void Visual::draw()
{
	if ( !loaded_ ) {
		return;
	}
}

void Visual::load( const std::string &name )
{
	SectionPtr section = ResMgr::open_section( name );
	if ( !section ) {
		log_error( "[Visual:load] unable to load visual '%s'", name.c_str() );
		return;
	}
}