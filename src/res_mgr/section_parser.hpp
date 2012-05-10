#ifndef _SECTION_PARSER_HPP_
#define _SECTION_PARSER_HPP_

#include "section.hpp"

SectionPtr parse_section( BinaryPtr &binary );
SectionPtr new_section();
BinaryPtr dump_section_in_text( SectionPtr &section );

BinaryPtr test_encode( BinaryPtr &binary );

#endif
