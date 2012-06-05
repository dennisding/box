#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include "res_mgr/res_mgr.hpp"

template < class TypeName >
class Config
{
public:
	static bool read_bool( const std::string &name, bool default_value = false );
	static int read_int( const std::string &name, int default_value = 0 );
	static float read_float( const std::string &name, float default_value = 0 );
	static Point read_point( const std::string &name );
	static Vector read_vector( const std::string &name );
	static Matrix read_matrix( const std::string &name );
	static const std::string read_string( const std::string &name );
	static BinaryPtr read_binary( const std::string &name );

	static void init( SectionPtr &section );
	static void init( const std::string &name );

private:
	static SectionPtr section_;
};

template < typename Type > SectionPtr Config< Type >::section_;

#define AUTO_CONFIG( type_name ) class type_name : public Config< type_name > {}

AUTO_CONFIG( GameConfig );

// inline
template < typename Type >
inline bool Config<Type>::read_bool( const std::string &name, bool default_value )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return default_value;
	}
	return section->as_bool();
}

template < typename Type >
inline int Config<Type>::read_int( const std::string &name, int default_value )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return default_value;
	}
	return section->as_int();
}

template < typename Type >
inline float Config<Type>::read_float( const std::string &name, float default_value )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return default_value;
	}
	return section->as_float();
}

template < typename Type >
inline Point Config<Type>::read_point( const std::string &name )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return Point( 0, 0 );
	}
	return section->as_point();
}

template < typename Type >
inline Vector Config<Type>::read_vector( const std::string &mame )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return Vector( 0, 0, 0 );
	}
	return section->as_vector();
}

template < typename Type >
inline Matrix Config<Type>::read_matrix( const std::string &name )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		Matrix matrix;
		matrix.make_identify();
		return matrix;
	}
	return section->as_matrix();
}

template < typename Type >
inline const std::string Config<Type>::read_string( const std::string &name )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return "";
	}
	return section->as_string();
}

template < typename Type >
inline BinaryPtr Config<Type>::read_binary( const std::string &name )
{
	SectionPtr section = section_->read_section( name );
	if ( !section ) {
		return 0;
	}
	return section->as_binary();
}

template < typename Type >
inline void Config<Type>::init( const std::string &name )
{
	section_ = ResMgr::open( name );
	log_assert( section_, "invalid init file: %s", name.c_str() );
}

template < typename Type >
inline void Config<Type>::init( SectionPtr &section )
{
	section_ = section;
}

#endif
