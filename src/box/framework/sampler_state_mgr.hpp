#ifndef _SAMPLER_STATE_HPP_
#define _SAMPLER_STATE_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class SamplerState : public Object
{
public:

};

typedef SmartPtr< SamplerState > SamplerStatePtr;

class SamplerStateMgr
{
public:
	static SamplerStatePtr get( const std::string &name );
};

#endif
