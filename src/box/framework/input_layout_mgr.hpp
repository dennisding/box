#ifndef _INPUT_LAYOUT_MGR_HPP_
#define _INPUT_LAYOUT_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <string>

class InputLayout : public Object
{
public:
};

typedef SmartPtr< InputLayout > InputLayoutPtr;

class InputLayoutMgr 
{
public:
	static void init();
	static void fini();
	static InputLayoutPtr get( const std::string &name );
};

#endif
