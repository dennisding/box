#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <vector>

#include "binary.hpp"
#include "smart_ptr.hpp"

typedef std::vector< BinaryPtr > BinaryVector;

class Buffer : public Object
{
public:
	void write( void * const buffer, int size );
	void write_at( int offset, void * const buffer, int size );
	void reset();

	BinaryPtr to_binary();

private:
	int index_;
	BinaryVector buffers_;
	BinaryPtr current_buffer_;
};

typedef SmartPtr< Buffer > BufferPtr;

#endif
