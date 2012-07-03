#ifndef _SECTION_HPP_
#define _SECTION_HPP_

#include <map>
#include <vector>
#include <string>

#include "math/point.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "utils/binary.hpp"
#include "utils/smart_ptr.hpp"

class Section;
class SectionInfo;

typedef SmartPtr< Section > SectionPtr;
typedef std::vector< SectionPtr > SectionVector;
typedef std::map< std::string, SectionVector > SectionMap;
typedef std::vector< SectionInfo > SectionInfoVector;

class Section : public Object
{
public:
	Section() {}
	virtual ~Section() {}
	SectionPtr read_section( const std::string &name );
	virtual SectionVector & read_section_vector( const std::string &name );

	virtual int as_int();
	virtual bool as_bool();
	virtual float as_float();
	virtual Point as_point();
	virtual Vector as_vector();
	virtual Matrix as_matrix();
	virtual const std::string as_string();
	virtual BinaryPtr as_binary();

	int read_int( const std::string &name );
	bool read_bool( const std::string &name );
	float read_float( const std::string &name );
	Point read_point( const std::string &name );
	Vector read_vector( const std::string &name );
	Matrix read_matrix( const std::string &name );
	const std::string read_string( const std::string &name );
	BinaryPtr read_binary( const std::string &name );

	virtual void add_child( const std::string &name, SectionPtr &section ) {}
	virtual void write_section( const std::string &name, SectionPtr value ) {}
	virtual void write_int( const std::string &name, int value ) {}
	virtual void write_bool( const std::string &name, bool value ) {}
	virtual void write_float( const std::string &name, float value ) {}
	virtual void write_point( const std::string &name, Point point ) {}
	virtual void write_vector( const std::string &name, Vector &vector ) {}
	virtual void write_matrix( const std::string &name, Matrix &matrix ) {}
	virtual void write_string( const std::string &name, const std::string &value ) {}
	virtual void write_binary( const std::string &name, BinaryPtr &binary ) {}

	virtual void query_info( SectionInfoVector &infos ) = 0;

private:
	Section( const Section &section );
	void operator=( const Section &section );
};

class SectionInfo
{
public:
	SectionInfo( int type, int extra_info, SectionPtr section = 0 ) 
		: type_(type), extra_info_( extra_info ), section_( section )
	{

	}
	int type_;
	int extra_info_; // depends on type_, struct is the number of child and sub child
	std::string name_;
	SectionPtr section_;
};

// inline function
inline SectionPtr Section::read_section( const std::string &name )
{
	SectionVector sections = read_section_vector( name );
	if ( sections.empty() ) {
		return SectionPtr();
	}
	return sections[0];
}

inline SectionVector &Section::read_section_vector( const std::string &name )
{
	static SectionVector sections;
	return sections;
}

inline bool Section::as_bool()
{
	return false;
}

inline int Section::as_int()
{
	return 0;
}

inline float Section::as_float()
{
	return 0.f;
}

inline Point Section::as_point()
{
	return Point();
}

inline Vector Section::as_vector()
{
	return Vector();
}

inline Matrix Section::as_matrix()
{
	return Matrix();
}

inline const std::string Section::as_string()
{
	return "";
}

inline BinaryPtr Section::as_binary()
{
	return BinaryPtr();
}

inline int Section::read_int( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return 0;
	}
	return section->as_int();
}

inline float Section::read_float( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return 0;
	}
	return section->as_float();
}

inline Point Section::read_point( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return Point( 0, 0 );
	}
	return section->as_point();
}

inline Vector Section::read_vector( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return Vector::zero;
	}
	return section->as_vector();
}

inline Matrix Section::read_matrix( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return Matrix::identity;
	}
	return section->as_matrix();
}

inline const std::string Section::read_string( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return "";
	}
	return section->as_string();
}

inline BinaryPtr Section::read_binary( const std::string &name )
{
	SectionPtr section = read_section( name );
	if ( !section ) {
		return BinaryPtr();
	}
	return section->as_binary();
}

#endif
