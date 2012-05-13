
#include "test.hpp"

#include "res_mgr/file_system.hpp"
#include "res_mgr/section_parser.hpp"

#include "utils/scanner.hpp"

void test_section()
{
	BinaryPtr binary = read_native_file( "..\\..\\res\\dog_test.dog" );
	SectionPtr section = parse_section( binary );

	binary = dump_section_in_text( section );
	write_native_file( "e:\\out.dog", binary );
}

void test_scan()
{
	BinaryPtr bin = read_native_file( "..\\..\\res\\dog_test.dog" );
	Scanner scanner( bin, 4 );
	TokenType token_type;
	while ( (token_type = scanner.scan() ) ) {
		printf( "%s\n", scanner.token_text().c_str() );
	}
}

void do_test()
{
	test_scan();
//	test_section();
}