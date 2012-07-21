#ifndef _CONST_BUFFER_DESC_HPP_
#define _CONST_BUFFER_DESC_HPP_

#include "const_buffer.hpp"

#include "utils/smart_ptr.hpp"

#include <list>
#include <string>

class ConstDesc
{
public:
	ConstDesc( const std::string &name, int offset, int size );

public:
	std::string name_;
	int offset_;
	int size_;
};

typedef std::list< ConstDesc > ConstDescList;

class ConstBufferDesc : public Object
{
public:
	ConstBufferDesc();
	ConstBufferPtr create_buffer();

	void add_const_desc( const std::string &name, int offset, int size );

private:
	int size_;
	ConstDescList const_descs_;
};

typedef SmartPtr< ConstBufferDesc > ConstBufferDescPtr;

// inline
inline ConstDesc::ConstDesc( const std::string &name, int offset, int size ) :
	name_( name ), offset_( offset ), size_( size )
{

}

inline ConstBufferDesc::ConstBufferDesc() : size_(0)
{

}

inline void ConstBufferDesc::add_const_desc( const std::string &name, int offset, int size )
{
	int new_size = offset + size;
	if ( size_ < new_size ) {
		size_ = new_size;
	}
	const_descs_.push_back( ConstDesc( name, offset, size ) );
}

#endif
