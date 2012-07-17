#ifndef _CONST_BUFFER_DESC_HPP_
#define _CONST_BUFFER_DESC_HPP_

#include "../primitive/const_buffer.hpp"

#include "utils/smart_ptr.hpp"

#include <map>
#include <string>

class ConstVaraible 
{
public:
	ConstVaraible( const std::string &name, int offset );
	std::string name_;
	int offset_;
};

typedef std::map< std::string, ConstVaraible > ConstVariableMap;

class ConstBufferDesc : public Object
{
public:
	int get_size();

private:
	int size_;
};

typedef SmartPtr< ConstBufferDesc > ConstBufferDescPtr;

// inline
ConstVaraible( const std::string &name, int offset ) : name_( name ), offset_( offset )
{

}

#endif
