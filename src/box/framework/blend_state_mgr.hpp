#ifndef _BLEND_STATE_MGR_HPP_
#define _BLEND_STATE_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class BlendState : public Object
{
public:

};

typedef SmartPtr< BlendState > BlendStatePtr;

class BlendStateMgr
{
public:
	static BlendStatePtr get( const std::string &name );
};

#endif
