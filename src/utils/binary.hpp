#ifndef _BINARY_HPP_
#define _BINARY_HPP_

#include "smart_ptr.hpp"

class Binary : public Object
{
public:
	inline Binary( int length, char *data ) : length_( 0 ), buffer_( data )
	{

	}

	inline Binary( int length ) : length_( length ), buffer_( 0 )
	{
		if ( length ) {
			buffer_ = new char[ length ];
		}
	}

	inline ~Binary()
	{
		delete [] buffer_;
	}

	inline int get_length()
	{
		return length_;
	}

	inline char *get_buffer()
	{
		return buffer_;
	}

private:
	int length_;
	char *buffer_;
private:
	void operator=( const Binary &bin );
	Binary( const Binary &bin );
};

typedef SmartPtr< Binary > BinaryPtr;


#endif
