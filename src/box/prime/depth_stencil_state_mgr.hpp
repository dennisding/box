#ifndef _DEPTH_STENCIL_STATE_MGR_HPP_
#define _DEPTH_STENCIL_STATE_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <d3d11.h>
#include <string>

class DepthStencilState : public Object
{
public:
	DepthStencilState( ID3D11DepthStencilState *depth_stencil_state );
	virtual ~DepthStencilState();

private:
	ID3D11DepthStencilState *depth_stencil_state_;
};

typedef SmartPtr< DepthStencilState > DepthStencilStatePtr;

class DepthStencilStateMgr
{
public:
	static void init();
	static void fini();
	static DepthStencilStatePtr get( const std::string &name );
};

// inline
inline DepthStencilState::DepthStencilState( ID3D11DepthStencilState *depth_stencil_state )
	: depth_stencil_state_( depth_stencil_state )
{

}

inline DepthStencilState::~DepthStencilState()
{
	depth_stencil_state_->Release();
}

#endif
