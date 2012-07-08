#ifndef _DEPTH_STENCIL_STATE_MGR_HPP_
#define _DEPTH_STENCIL_STATE_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class DepthStencilState : public Object
{
public:
};

typedef SmartPtr< DepthStencilState > DepthStencilStatePtr;

class DepthStencilStateMgr
{
public:
	static DepthStencilStatePtr get( const std::string &name );
};

#endif
