#include <cstdio>
#include <vector>

#include "file_system.hpp"

#include "utils/smart_ptr.hpp"

class FileSystem : public Object
{
public:
	virtual ~FileSystem() {}
	virtual BinaryPtr read( const std::string &name ) = 0;
};

typedef SmartPtr< FileSystem > FileSystemPtr;
typedef std::vector< FileSystemPtr > FileSystemVector;

class WinFileSystem : public FileSystem
{
public:
	WinFileSystem( const std::string &root = "" ) : root_( root )
	{
		if ( root.empty() ) {
			return;
		}
		if ( root[ root.size() - 1 ] != '/' ) {
			root_.append( "/" );
		}
	}

	virtual BinaryPtr read( const std::string &name )
	{
		return native_file_read( root_ + name );
	}

private:
	std::string root_;
};

static FileSystemVector file_systems_;
static void init_file_systems()
{
	static bool inited = false;
	if ( inited ) {
		return;
	}

	file_systems_.push_back( new WinFileSystem() );
}

BinaryPtr FileSystemMgr::read( const std::string &name )
{
	init_file_systems();

	for ( FileSystemVector::iterator it = file_systems_.begin(); it != file_systems_.end(); ++it ) {
		BinaryPtr binary = (*it)->read( name );
		if ( binary ) {
			return binary;
		}
	}
	return 0;
}

BinaryPtr native_file_read( const std::string &name )
{
	FILE *file = fopen( name.c_str(), "rb" );
	if ( !file ) {
		return 0;
	}
	fseek( file, 0, SEEK_END );
	int size = ftell( file );
	BinaryPtr binary = new Binary( size );
	fseek( file, SEEK_SET, 0 );
	fread( binary->get_buffer(), 1, size, file );

	return binary;
}

void native_file_write( const std::string &name, BinaryPtr &binary )
{
	FILE *file = fopen( name.c_str(), "wb" );
	fwrite( binary->get_buffer(), 1, binary->get_length(), file );
	fclose( file );
}