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

BinaryPtr native_file_read( const std::string &name );
void native_file_write( const std::string &name, BinaryPtr &binary );

#endif
