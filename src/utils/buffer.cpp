#include "buffer.hpp"

#include <algorithm>

#define BUFFER_SIZE (4*1024)

void Buffer::write_at( int offset, const void *buffer, int size )
{
	int data_length = offset + size;
	// prepare the buffer
	for ( int buf_size = buffers_.size() * BUFFER_SIZE; buf_size < data_length; buf_size += BUFFER_SIZE ) {
		buffers_.push_back( new Binary( BUFFER_SIZE ) );
	}

	// write to buffer
	int writed = 0;
	while ( writed < size ) {
		int index = write_pos_ / BUFFER_SIZE;
		int offset = write_pos_ % BUFFER_SIZE;
		int available = (index + 1 ) * BUFFER_SIZE;
		available = std::min( available, size );
		memcpy( buffers_[index]->get_buffer(), buffer, available );

		write_pos_ += available;
		writed += available;
	}

	length_ = std::max( write_pos_, length_ );
}

BinaryPtr Buffer::to_binary()
{
	BinaryPtr bin = new Binary( length_ );

	int copyed = 0;
	while ( copyed < length_ ) {
		int copy_length = std::min( length_ - copyed, BUFFER_SIZE );
		memcpy( bin->get_buffer(), buffers_[copyed/BUFFER_SIZE]->get_buffer(), copy_length );
		copyed = copy_length;
	}

	return bin;
}

