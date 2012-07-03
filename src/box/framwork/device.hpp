#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include "d3d11.h"

// use for create resource only
class Device
{
public:
	static void init( ID3D11Device *device );
	static void fini();

public:
	static ID3D11Device *device_;
};

#endif
