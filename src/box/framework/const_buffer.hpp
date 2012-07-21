#ifndef _CONST_BUFFER_HPP_
#define _CONST_BUFFER_HPP_

#include "utils/smart_ptr.hpp"

#include <vector>

class Effect;

class ConstSetter : public Object
{
public:
	virtual ~ConstSetter() {}
	virtual void set_const( void *buffer, int offset = 0 ) = 0;
};

typedef SmartPtr< ConstSetter > ConstSetterPtr;
typedef std::vector< ConstSetterPtr > ConstSetterVector;

class MemoryConstSetter : public ConstSetter
{
public:
	MemoryConstSetter( void *buffer, int size );
	virtual void set_const( void *buffer, int offset = 0 );

private:
	void *buffer_; // manage by parent
	int size_;
};

class ConstBuffer : public Object
{
public:
	ConstBuffer( Effect *parent );
	void fill_buffer();

private:
	void gen_setters();

private:
	Effect *parent_;
	ConstSetterVector const_setters_;
};

typedef SmartPtr< ConstBuffer > ConstBufferPtr;

// inline function
inline MemoryConstSetter::MemoryConstSetter( void *buffer, int size ) : buffer_( buffer ), size_( size )
{

}

inline void MemoryConstSetter::set_const( void *buffer, int offset )
{
	memcpy( (char *)buffer + offset, buffer_, size_ );
}

inline ConstBuffer::ConstBuffer( Effect *parent ) : parent_( parent )
{
	gen_setters();
}

#endif
