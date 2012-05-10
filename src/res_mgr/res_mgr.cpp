#include "res_mgr.hpp"
#include "file_system.hpp"
#include "section_parser.hpp"

BinaryPtr ResMgr::open_binary( const std::string &name )
{
	return FileSystemMgr::read( name );
}

SectionPtr ResMgr::open_section( const std::string &name )
{
	BinaryPtr binary = open_binary( name );
	return parse_section( binary );
}