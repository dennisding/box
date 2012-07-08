#include "device.hpp"

ID3D11Device *Device::device_;

void Device::init( ID3D11Device *device )
{
	device_ = device;
}

void Device::fini()
{
	device_->Release();
	device_ = 0;
}
