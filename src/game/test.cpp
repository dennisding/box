
#include "test.hpp"

#include "res_mgr/file_system.hpp"
#include "res_mgr/section_parser.hpp"

void test_section()
{
	BinaryPtr binary = native_file_read( "E:\\work\\cpp\\box\\dog_test.dog" );
	SectionPtr section = parse_section( binary );

	binary = dump_section_in_text( section );
	native_file_write( "e:\\out.dog", binary );
}

void do_test()
{
	test_section();
}