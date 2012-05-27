#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <vector>

#include "binary.hpp"
#include "smart_ptr.hpp"

typedef std::vector< BinaryPtr > BinaryVector;

class Buffer : public Object
{
public:
	Buffer();
	void write( const void *buffer, int size );
	void write_char( char c );
	void write_at( int offset, const void *buffer, int size );
	void seek( int pos );

	BinaryPtr to_binary();
	int get_write_pos();

private:
	int write_pos_;
	int length_;
	BinaryVector buffers_;

private:
	Buffer( const Buffer & ); // not allow to copy
};

typedef SmartPtr< Buffer > BufferPtr;

/// inline
inline Buffer::Buffer() : write_pos_( 0 ), length_( 0 )
{
}

inline void Buffer::write( const void *buffer, int size )
{
	write_at( write_pos_, buffer, size );
}

inline void Buffer::write_char( char c )
{
	write_at( write_pos_, &c, sizeof( c ) );
}

inline void Buffer::seek( int pos )
{
	write_pos_ = pos;
}

inline void Buffer::get_write_pos()
{
	return write_pos_;
}

#endif
