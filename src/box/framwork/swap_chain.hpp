#ifndef _SWAP_CHAIN_HPP_
#define _SWAP_CHAIN_HPP_

#include "render_target_mgr.hpp"

#include "d3d11.h"

class SwapChain
{
public:
	static void init();
	static void fini();

	static void present();
};


#endif
