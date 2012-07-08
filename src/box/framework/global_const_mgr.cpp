#include "global_const_mgr.hpp"

#include "utils/log.hpp"
#include "utils/binary.hpp"

#include <vector>
#include <map>

typedef std::map< std::string, ConstSetterPtr > ShaderConstMap;

static ShaderConstMap const_map_;
static BinaryPtr const_buffer_;
static int buffer_length_;

GlobalConstSetter::GlobalConstSetter( int offset, int size ) : offset_( offset ), size_( size )
{

}

void GlobalConstSetter::set_const( void *buffer, int offset )
{
	memcpy( (char *)buffer + offset, const_buffer_->get_buffer() + offset_, size_ );
}

void GlobalConstMgr::init()
{
	const_buffer_ = new Binary( 4096 );
}

void GlobalConstMgr::fini()
{
	const_buffer_ = 0;
	const_map_.clear();
}

void GlobalConstMgr::register_const( const std::string &name, int size )
{
	int offset = buffer_length_;
	if ( offset + size > buffer_length_ ) {
		const_buffer_ = new Binary( const_buffer_->get_length() );
	}
	
	const_map_.insert( std::make_pair( name, new GlobalConstSetter( offset, size ) ) );
}

void GlobalConstMgr::set_const( const std::string &name, void *buffer )
{
	ShaderConstMap::iterator it = const_map_.find( name );
	if ( it == const_map_.end() ) {
		return;
	}

	memcpy( const_buffer_->get_buffer() + it->second.offset_, buffer, it->second.size_ );
}

ConstSetterPtr GlobalConstMgr::get_const( const std::string &name )
{
	ShaderConstMap::iterator it = const_map_.find( name );
	if ( it == const_map_.end() ) {
		log_error( "[ConstMgr::get_const] unregister const '%s'", name.c_str() );
		return new GlobalConstSetter( 0, 0 );
	}

	return it->second;
}
