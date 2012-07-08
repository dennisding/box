#ifndef _GLOBAL_CONST_MGR_HPP_
#define _GLOBAL_CONST_MGR_HPP_

#include "../primitive/const_buffer.hpp"

#include <string>

class GlobalConstSetter : public ConstSetter
{
public:
	GlobalConstSetter( int offset, int size );
	virtual void set_const( void *buffer, int offset = 0 );

public: // use by ConstMgr
	int offset_;
	int size_;
};

class GlobalConstMgr
{
public:
	static void init();
	static void fini();

	static void register_const( const std::string &name, int size );
	static void set_const( const std::string &name, void *buffer );
	static ConstSetterPtr get_const( const std::string &name );
};


#endif
