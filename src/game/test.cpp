
#include "test.hpp"

#include "res_mgr/file_system.hpp"
#include "prime/config.hpp"

void do_test()
{
	BinaryPtr bin = read_native_file( "config.dog" );
	SectionPtr section = parse_section( bin );
	GameConfig::init( section );

	const std::string path = GameConfig::read_string( "res_path" );
}