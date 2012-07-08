#ifndef _EFFECT_HPP_
#define _EFFECT_HPP_

#include "effect_desc_mgr.hpp"
//#include "../framework/const_mgr.hpp"

#include "utils/smart_ptr.hpp"

#include <string>

class ConstProviderBase : public Object
{
public:
	virtual ConstSetterPtr get_const_setter( const std::string &name ) = 0;
};

template < typename Type >
class ConstProvider : public ConstProviderBase
{
public:
	ConstProvider( Type *provider );
	virtual ConstSetterPtr get_const_setter( const std::string &name );

private:
	Type *provider_;
};

typedef SmartPtr< ConstProviderBase > ConstProviderPtr;

class Effect : public Object
{
public:
	Effect();
	~Effect();

	void init( const std::string &name, ConstProviderPtr &const_privider );

private:
	void create_self( EffectDescPtr &desc );

private:
};

// inline template
template < typename Type >
ConstProvider<Type>::ConstProvider( Type *provider ) : provider_( provider )
{

}

template < typename Type >
ConstSetterPtr ConstProvider<Type>::get_const_setter( const std::string &name )
{
	return provider_->get_const_setter();
}

#endif
