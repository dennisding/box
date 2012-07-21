#ifndef _INDEX_BUFFER_MGR_HPP_
#define _INDEX_BUFFER_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class IndexBuffer : public Object
{
public:
	
};

typedef SmartPtr< IndexBuffer > IndexBufferPtr;

class IndexBufferMgr
{
public:
	IndexBufferPtr create( const std::string &ids, int size, void *src = 0 );
};

#endif
