#ifndef _VERTEX_BUFFER_MGR_HPP_
#define _VERTEX_BUFFER_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class VertexBuffer : public Object
{
public:
};

typedef SmartPtr< VertexBuffer > VertexBufferPtr;

class VertexBufferMgr
{
public:
	VertexBufferPtr create( const std::string &ids, int size, void *src = 0 );
};

#endif
