#ifndef _FILE_SYSTEM_HPP_
#define _FILE_SYSTEM_HPP_

#include <string>

#include "utils/binary.hpp"
#include "utils/smart_ptr.hpp"

class FileSystemMgr
{
public:
	static BinaryPtr read( const std::string &name );
};

BinaryPtr read_native_file( const std::string &name );
void write_native_file( const std::string &name, BinaryPtr &binary );

#endif
