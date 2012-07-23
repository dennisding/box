#ifndef _INPUT_LAYOUT_MGR_HPP_
#define _INPUT_LAYOUT_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class InputLayout : public Object
{
public:
	InputLayout( ID3D11InputLayout *layout );
	virtual ~InputLayout();

private:
	ID3D11InputLayout *layout_;
};

typedef SmartPtr< InputLayout > InputLayoutPtr;

class InputLayoutMgr 
{
public:
	static void init();
	static void fini();
	static InputLayoutPtr get( const std::string &name );
	static InputLayoutPtr create( ID3DBlob *byte_code, const std::string &name );
};

// inline
inline InputLayout::InputLayout( ID3D11InputLayout *layout ) : layout_( layout )
{

}

inline InputLayout::~InputLayout()
{
	layout_->Release();
}

#endif
