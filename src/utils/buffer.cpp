#include "buffer.hpp"

#define BUFFER_SIZE 4*1024

Buffer::Buffer() : index_( 0 )
{

}

void Buffer::write( void * const buffer, int size )
{
	write_at( index_, buffer, size );
	index_ += size;
}

void Buffer::write_at( int offset, void * const buffer, int size )
{

}

void Buffer::reset()
{
	index_ = 0;
}

BinaryPtr Buffer::to_binary()
{
	int buffer_size = index_;
	BinaryPtr bin = new Binary( buffer_size );
}