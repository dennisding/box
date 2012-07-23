#ifndef _BLEND_STATE_MGR_HPP_
#define _BLEND_STATE_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class BlendState : public Object
{
public:
	BlendState( ID3D11BlendState *blend_state );
	virtual ~BlendState();

private:
	ID3D11BlendState *blend_state_;
};

typedef SmartPtr< BlendState > BlendStatePtr;

class BlendStateMgr
{
public:
	static void init();
	static void fini();
	static BlendStatePtr get( const std::string &name );
};

// inline
inline BlendState::BlendState( ID3D11BlendState *blend_state ) : blend_state_( blend_state )
{

}

inline BlendState::~BlendState()
{
	blend_state_->Release();
}

#endif
