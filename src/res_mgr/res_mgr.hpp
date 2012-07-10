#ifndef _RES_MGR_HPP_
#define _RES_MGR_HPP_

#include "section_parser.hpp"

class ResMgr
{
public:
	static void init( const std::string &name );
	static void fini();

	static SectionPtr open_section( const std::string &name );
	static BinaryPtr open_binary( const std::string &name );
};

#endif
