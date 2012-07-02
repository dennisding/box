#include "buffer.hpp"

#include <algorithm>

#define BUFFER_SIZE (4*1024)

void Buffer::write_at( int offset, const void *buffer, int size )
{
	int data_length = offset + size;
	write_pos_ = offset;
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
		if ( size < available ) {
			available = size;
		}
		memcpy( buffers_[index]->get_buffer() + offset, buffer, available );

		write_pos_ += available;
		writed += available;
	}

	if ( length_ < write_pos_ ) {
		length_ = write_pos_;
	}
}

BinaryPtr Buffer::to_binary()
{
	BinaryPtr bin = new Binary( length_ );

	int copyed = 0;
	while ( copyed < length_ ) {
		int copy_length = length_ - copyed;
		if ( copy_length > BUFFER_SIZE ) {
			copy_length = BUFFER_SIZE;
		}
		memcpy( bin->get_buffer(), buffers_[copyed/BUFFER_SIZE]->get_buffer(), copy_length );
		copyed = copy_length;
	}

	return bin;
}

