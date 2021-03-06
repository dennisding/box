#include "section_parser.hpp"

#include "utils/log.hpp"
#include "utils/buffer.hpp"
#include "utils/scanner.hpp"

#include <cctype>

#define SECTION_TYPE_STRUCT 0
#define SECTION_TYPE_BOOL	1
#define SECTION_TYPE_INT	2
#define SECTION_TYPE_FLOAT	3
#define SECTION_TYPE_POINT	4
#define SECTION_TYPE_VECTOR	5
#define SECTION_TYPE_MATRIX	6
#define SECTION_TYPE_STRING	7
#define SECTION_TYPE_BINARY	8

struct BinarySectionHeader
{
	int id_;
	int offset_;
	int number_;
};

struct BinarySectionEntry
{
	int type_and_name_;
	int value_;

	inline int get_type()
	{
		return  (unsigned int)type_and_name_ >> 24;
	}

	inline void set_type( int type )
	{
		type_and_name_ &= 0x00FFFFFF;
		type_and_name_ = (type << 24 ) | type_and_name_;
	}

	int get_name_offset()
	{
		return 0x00FFFFFF & type_and_name_;
	}

	void set_name_offset( int offset )
	{
		type_and_name_ &= 0xFF000000;
		type_and_name_ |= offset;
	}
};

class IntSection : public Section
{
public:
	IntSection( int value ) : value_( value ) {}
	
	virtual int as_int()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_INT, 0, SectionPtr::raw_wrap( this ) ) );
	}

private:
	int value_;
};

class BoolSection : public Section
{
public:
	BoolSection( bool value ) : value_( value ) {}

	virtual bool as_bool()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_BOOL, 0, SectionPtr::raw_wrap( this ) ) );
	}

private:
	bool value_;
};

class FloatSection : public Section
{
public:
	FloatSection( float value ) : value_( value )
	{
	}

	virtual float as_float()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_FLOAT, 0, SectionPtr::raw_wrap( this ) ) );
	}

private:
	float value_;
};

class PointSection : public Section
{
public:
	PointSection( Point &point ) : value_( point ) {}

	virtual Point as_point()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_POINT, 0, SectionPtr::raw_wrap( this )) );
	}

private:
	Point value_;
};

class VectorSection : public Section
{
public:
	VectorSection( Vector &value ) : value_( value )
	{
	}

	virtual Vector as_vector()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_VECTOR, 0, SectionPtr::raw_wrap( this ) ) );
	}

private:
	Vector value_;
};

class MatrixSection : public Section
{
public:
	MatrixSection( Matrix &value ) : value_( value )
	{
	}

	virtual Matrix as_matrix()
	{
		return value_;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_MATRIX, 0, SectionPtr::raw_wrap( this ) ) );
	}

private:
	Matrix value_;
};

class StringSection : public Section
{
public:
	StringSection( const std::string &value ) : value_( value ) {}

	virtual const std::string as_string()
	{
		return value_;
	}

	virtual BinaryPtr as_binary()
	{
		BinaryPtr bin = new Binary((int)value_.size());
		memcpy( bin->get_buffer(), value_.c_str(), value_.size() );
		return bin;
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_STRING, 0, SectionPtr::raw_wrap( this )) );
	}

private:
	std::string value_;
};

class BinarySection : public Section
{
public:
	BinarySection( BinaryPtr &value ) : value_( value ) 
	{
	}

	virtual BinaryPtr as_binary()
	{
		return value_;
	}

	virtual const std::string as_string()
	{
		if ( value_->get_length() == 0 ) {
			return "";
		}
		return std::string( value_->get_buffer(), value_->get_length() );
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		infos.push_back( SectionInfo( SECTION_TYPE_BINARY, 0, SectionPtr::raw_wrap(this)) );
	}

private:
	BinaryPtr value_;
};

class StructSection : public Section
{
public:
	virtual SectionVector &read_section_vector( const std::string &name )
	{
		size_t pos = name.find( "/" );
		if ( pos == std::string::npos ) {
			SectionMap::iterator it = sections_.find( name );
			if ( it == sections_.end() ) {
				return Section::read_section_vector( name );
			}
			return it->second;
		}

		std::string child_name = name.substr( 0, pos );
		SectionMap::iterator it = sections_.find( child_name );
		if ( it == sections_.end() ) {
			return Section::read_section_vector( child_name );
		}

		return it->second[0]->read_section_vector( name.substr( pos + 1 ) );
	}

	virtual void write_int( const std::string &name, int value )
	{
		write_section( name, new IntSection( value ) );
	}

	virtual void write_float( const std::string &name, float value )
	{
		write_section( name, new FloatSection( value ) );
	}

	virtual void write_point( const std::string &name, Point point )
	{
		write_section( name, new PointSection( point ) );
	}

	virtual void write_vector( const std::string &name, Vector &vector )
	{
		write_section( name, new VectorSection( vector ) );
	}

	virtual void write_matrix( const std::string &name, Matrix &matrix )
	{
		write_section( name, new MatrixSection( matrix ) );
	}

	virtual void write_string( const std::string &name, const std::string &value )
	{
		write_section( name, new StringSection( value ) );
	}

	virtual void write_binary( const std::string &name, BinaryPtr &binary )
	{
		write_section( name, new BinarySection( binary ) );
	}

	virtual void write_section( const std::string &name, SectionPtr section )
	{
		SectionPtr child_section = section;
		size_t pos = name.find_last_of( '/' );
		size_t end_pos = name.size();
		while ( pos != std::string::npos ) {
			SectionPtr parent = new StructSection;
			parent->add_child( name.substr( pos + 1, end_pos - pos ), child_section );
			end_pos = pos;
			pos = name.find_last_of( '/', end_pos - 1 );
		}
		add_child( name.substr( 0, end_pos ), child_section );
	}

	virtual void add_child( const std::string &name, SectionPtr &section )
	{
		sections_[name].push_back( section );
	}

	virtual void query_info( SectionInfoVector &infos )
	{
		size_t index = infos.size();
		infos.push_back( SectionInfo( SECTION_TYPE_STRUCT, 0, SectionPtr::raw_wrap( this )) );
		int total = 0;
		for ( SectionMap::iterator mit = sections_.begin(); mit != sections_.end(); ++mit ) {
			total += mit->second.size();
			for ( SectionVector::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit ) {
				int child_index = infos.size();
				(*sit)->query_info( infos );
				infos[child_index].name_ = mit->first;
			}
		}
		infos[index].extra_info_ = total;
	}

private:
	SectionMap sections_;
};

class DataHandler
{
public:
	DataHandler( const char *data, int length ) : data_( data ), length_( length )
	{
	}

	inline BinaryPtr encode()
	{
		do_encode();

		return buffer_.to_binary();
	}

	inline BinaryPtr decode()
	{
		do_decode();
		
		return buffer_.to_binary();
	}

private:
	void do_encode()
	{
		buffer_.write_char( '\'' );
		for ( int index = 0; index < length_; ++index ) {
			encode_char( data_[index] );
		}
		buffer_.write_char( '\'' );
	}

	void do_decode()
	{
		for ( int index = 1; index < length_ - 1; ) {
			index = index + decode_char( data_[index], index );
		}
	}

	inline void encode_char( unsigned char c )
	{
		if ( c == '\\' ) {
			buffer_.write( "\\\\", 2 );
			return;
		}
		if ( c == '\'' ) {
			buffer_.write( "\\\'", 2 );
			return;
		}
		if ( !iscntrl( c ) ) {
			buffer_.write_char( c );
			return;
		}
		// control character encode
		buffer_.write_char( '\\' );
		buffer_.write_char( '0' + c/100 );
		c = c % 100;
		buffer_.write_char( '0' + c/10 );
		c = c % 10;
		buffer_.write_char( '0' + c );
	}

	inline int decode_char( char c, int index )
	{
		if ( c != '\\' ) {
			buffer_.write_char(c);
			return 1;
		}
		if ( index + 1 < length_ ) {
			if ( data_[index + 1 ] == '\\' || data_[ index + 1] == '\'' ) {
				buffer_.write_char( data_[ index + 1 ] );
				return 2;
			}
		}
		if ( index + 3 >= length_ ) {
			buffer_.write_char( c );
			return 1;
		}
		if ( !( isdigit( (unsigned char)data_[index+1]) && isdigit( (unsigned char)data_[index+2]) 
			&& (unsigned char)isdigit( data_[index+3]) ) ) {
				buffer_.write_char( c );
				return 1;
		}

		// control character decode
		int char_ord = data_[index + 1] - '0';
		char_ord = char_ord*10 + data_[index + 2] - '0';
		char_ord = char_ord*10 + data_[index + 3] - '0';
		buffer_.write_char( (char)char_ord );
		return 4;
	}

private:
	const char *data_;
	int length_;
	Buffer buffer_;
};

BinaryPtr binary_encode( const char *data, int length )
{
	DataHandler handler( data, length );
	return handler.encode();
}

BinaryPtr binary_decode( const char *data, int length )
{
	DataHandler handler( data, length );
	return handler.decode();
}

class TextSectionParser
{
public:
	TextSectionParser( BinaryPtr &binary ) : scanner_( binary )
	{
	}

	SectionPtr parse()
	{
		SectionPtr root( new StructSection );

		while ( true ) {
			std::string type_name = scanner_.scan_text();
			if ( type_name.empty() || type_name == "}" ) {
				return root;
			}
			std::string name = scanner_.scan_text();
			if ( name.empty() ) {
				syntax_error( "name" );
				return 0;
			}

			if ( scanner_.scan_text() != "=" ) {
				syntax_error( "equal sign =" );
				return 0;
			}

			SectionPtr section = parse_type( type_name );
			if ( !section ) {
				return 0;
			}
			root->write_section( name, section );
		}

		return root;
	}

private:
	SectionPtr parse_type( const std::string &type_name )
	{
		if ( type_name == "bool" ) {
			return parse_bool_section();
		} else if ( type_name == "int" ) {
			return parse_int_section();
		} else if ( type_name == "float" ) {
			return parse_float_section();
		} else if ( type_name == "string" ) {
			return parse_string_section();
		} else if ( type_name == "point" ) {
			return parse_point_section();
		} else if ( type_name == "vector" ) {
			return parse_vector_section();
		} else if ( type_name == "matrix" ) {
			return parse_matrix_section();
		} else if ( type_name == "binary" ) {
			return parse_binary_section();
		} else if ( type_name == "struct") {
			return parse_struct_section();
		}
		syntax_error( "invalid type name" );
		return 0;
	}

	SectionPtr parse_bool_section()
	{
		if ( scanner_.scan() != TOKEN_IDENTIFY ) {
			syntax_error( "bool" );
			return 0;
		}
		if ( scanner_.token_text() == "true" ) {
			return new BoolSection( true );
		} else {
			return new BoolSection( false );
		}
		return 0;
	}

	SectionPtr parse_int_section()
	{
		if ( scanner_.scan() != TOKEN_INT ) {
			syntax_error( "int" );
			return 0;
		}
		int value = atol( scanner_.token_text().c_str() );
		return new IntSection( value );
	}

	SectionPtr parse_float_section()
	{
		if ( scanner_.scan() != TOKEN_FLOAT ) {
			syntax_error( "float" );
			return 0;
		}
		float value = (float)atof( scanner_.token_text().c_str() );
		return new FloatSection( value );
	}

	SectionPtr parse_point_section()
	{
		Point point;
		if ( !parse_float_array( &point.x_, 2, "point" ) ) {
			return 0;
		}

		return new PointSection( point );
	}

	SectionPtr parse_vector_section()
	{
		Vector vector;
		if ( !parse_float_array( &vector.x_, 3, "vector" ) ) {
			return 0;
		}

		return new VectorSection( vector );
	}

	SectionPtr parse_matrix_section()
	{
		Matrix matrix;
		if ( !parse_float_array( matrix.m_, 16, "matrix" ) ) {
			return 0;
		}

		return new MatrixSection( matrix );
	}

	bool parse_float_array( float *floats, int size, const std::string &tips_prefix )
	{
		if ( scanner_.scan_text() != "{" ) {
			syntax_error( tips_prefix + " {" );
			return false;
		}

		for ( int i = 0; i < size; ++i ) {
			ScanTokenType token = scanner_.scan();
			if ( (token != TOKEN_INT ) && ( token != TOKEN_FLOAT ) ) {
				syntax_error( tips_prefix + " number" );
				return false;
			}

			floats[ i ] = (float)atof( scanner_.token_text().c_str() );
		}

		if ( scanner_.scan_text() != "}" ) {
			syntax_error( tips_prefix + " }" );
			return false;
		}

		return true;
	}

	SectionPtr parse_string_section()
	{
		if ( scanner_.scan() != TOKEN_STRING ) {
			syntax_error( "string" );
			return 0;
		}
		std::string token = scanner_.token_text();
		BinaryPtr binary = binary_decode( token.c_str(), (int)token.size() );
		return new StringSection( std::string( binary->get_buffer(), binary->get_length() ) );
	}

	SectionPtr parse_binary_section()
	{
		if ( scanner_.scan() != TOKEN_STRING ) {
			syntax_error( "binary" );
			return 0;
		}
		const std::string token = scanner_.token_text();
		return new BinarySection( binary_decode( token.c_str(), (int)token.size() ) );
	}

	SectionPtr parse_struct_section()
	{
		if ( scanner_.scan_text() != "{" ) {
			syntax_error( "struct {" );
			return 0;
		}
		return parse();
	}

private:
	inline void syntax_error( const std::string &identify )
	{
		log_error( "section [%s] syntax error near line : %d ", identify.c_str(), scanner_.line_ );
	}

private:
	Scanner scanner_;
};

class BinarySectionParser
{
	typedef std::vector< BinarySectionEntry > SectionEntryVector;
public:
	BinarySectionParser( BinaryPtr &binary ) : binary_( binary ), entry_index_(0)
	{
	}

	SectionPtr parse()
	{
		BinarySectionHeader head;
		memcpy( &head, binary_->get_buffer(), sizeof( BinarySectionHeader ) );
		entrys_.resize( head.number_ );
		
		memcpy( &entrys_[0], binary_->get_buffer() + head.offset_, head.number_*sizeof( BinarySectionEntry ) );

		return parse_sections();
	}

private:

	SectionPtr parse_sections()
	{
		SectionPtr root = new StructSection;
		BinarySectionEntry &root_entry = entrys_[ entry_index_++ ];
		
		for ( int i = 0; i < root_entry.value_; ++i ) {
			BinarySectionEntry entry = entrys_[ entry_index_++ ];
			SectionPtr section;
			switch ( entry.get_type() ) {
			case SECTION_TYPE_STRUCT:
				--entry_index_;
				section = parse_sections();
				break;

			case SECTION_TYPE_BOOL:
				section = new BoolSection( entry.value_ != 0 );
				break;

			case SECTION_TYPE_INT:
				section = new IntSection( entry.value_ );
				break;

			case SECTION_TYPE_FLOAT:
				section = new FloatSection( *(float *)&entry.value_ );
				break;

			case SECTION_TYPE_POINT:
				{
					Point point;
					memcpy( &point, binary_->get_buffer() + entry.value_, sizeof(Point) );
					section = new PointSection( point );
				}
				break;

			case SECTION_TYPE_VECTOR:
				{
					Vector vector;
					memcpy( &vector, binary_->get_buffer() + entry.value_, sizeof(Vector) );
					section = new VectorSection( vector );
				}
				break;

			case SECTION_TYPE_MATRIX:
				{
					Matrix matrix;
					memcpy( &matrix, binary_->get_buffer() + entry.value_, sizeof( Matrix ) );
					section = new MatrixSection( matrix );
				}
				break;

			case SECTION_TYPE_STRING:
				section = new StringSection( extract_string( entry.value_ ) );
				break;

			case SECTION_TYPE_BINARY:
				section = new BinarySection( extract_binary( entry.value_ ) );
				break;
			}
			
			root->add_child( extract_string( entry.get_name_offset() ), section );
		}

		return root;
	}

	const std::string extract_string( int offset )
	{
		int size = *(int *)(binary_->get_buffer() + offset);
		return std::string( binary_->get_buffer() + offset + sizeof( int ), size );
	}

	BinaryPtr extract_binary( int offset )
	{
		int size = *(int *)( binary_->get_buffer() + offset );
		BinaryPtr bin = new Binary( size );
		memcpy( bin->get_buffer(), binary_->get_buffer() + offset + sizeof( int ), size );

		return bin;
	}

private:
	BinaryPtr binary_;
	SectionEntryVector entrys_;
	int entry_index_;
};

class SectionDumper
{
public:
	SectionDumper( SectionPtr &section ) : section_( section )
	{
	}

	BinaryPtr dump()
	{
		section_->query_info( section_infos_ );

		do_dump();

		return buffer_.to_binary();
	}

	virtual void do_dump() = 0;

protected:
	SectionPtr section_;
	Buffer buffer_;
	SectionInfoVector section_infos_;
};

class DumpInfo
{
public:
	DumpInfo( const std::string &type_name, bool has_brace = false ) : type_name_( type_name ), has_brace_( has_brace )
	{
	}

	std::string type_name_;
	bool has_brace_;
};

class Dumper : public Object
{
public:
	Dumper( const std::string &type_name, SectionPtr &section ) : type_name_( type_name ), section_( section )
	{
	}

	inline void dump_basic_info( Buffer &buffer, SectionInfo &info )
	{
		// type name
		buffer.write( type_name_.c_str(), (int)type_name_.size() );
		buffer.write( " ", 1 );
		buffer.write(  info.name_.c_str(), (int)info.name_.size() );
		buffer.write( " = ", 3 );
	}

	virtual void dump_value( Buffer &buffer, int indent ) = 0;

private:
	const std::string type_name_;

protected:
	SectionPtr section_;
};

typedef SmartPtr< Dumper > DumperPtr;

class BoolDumper : public Dumper
{
public:
	BoolDumper( SectionPtr &section ) : Dumper( "bool", section )
	{
	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		if ( section_->as_bool() ) {
			buffer.write( "true", 4 );
		} else {
			buffer.write( "false", 5 );
		}
	}

};

class IntDumper : public Dumper
{
public:
	IntDumper( SectionPtr &section ) : Dumper( "int", section )
	{
	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		char tmp[32];
		int count = sprintf( tmp, "%d", section_->as_int() );
		buffer.write( tmp, count );
	}
};

class FloatDumper : public Dumper
{
public:
	FloatDumper( SectionPtr &section ) : Dumper( "float", section )
	{
	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		char tmp[32];
		int count = sprintf( tmp, "%g", section_->as_float() );
		buffer.write( tmp, count );
	}
};

class FloatArrayDumper : public Dumper
{
public:
	FloatArrayDumper( const std::string &name, SectionPtr &section ) : Dumper( name, section )
	{

	}

	inline void dump_indent( Buffer &buffer, int indent )
	{
		for ( int i = 0; i < indent; ++i ) {
			buffer.write( "\t", 1 );
		}
	}

	void dump_float_array( Buffer &buffer, float *floats, int num, int indent )
	{
		buffer.write( "{ ", 2 );
		char tmp[32];
		for ( int i = 0; i < num; ++i ) {
			if ( num > 4 && (i % 4 == 0) ) {
				buffer.write( "\n", 1 );
				dump_indent( buffer, indent + 1 );
			}
			int count = sprintf( tmp, "%g", floats[i] );
			buffer.write( tmp, count );
			buffer.write( " ", 1 );
		}

		if ( num > 4 ) {
			buffer.write( "\n", 1 );
			dump_indent( buffer, indent );
		}
		buffer.write( "}", 1 );
	}
};

class PointDumper : public FloatArrayDumper
{
public:
	PointDumper( SectionPtr &section ) : FloatArrayDumper( "point", section )
	{
	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		Point point = section_->as_point();
		dump_float_array( buffer, &point.x_, 2, indent );
	}
};

class VectorDumper : public FloatArrayDumper
{
public:
	VectorDumper( SectionPtr &section ) : FloatArrayDumper( "vector", section )
	{

	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		Vector vector = section_->as_vector();
		dump_float_array( buffer, &vector.x_, 3, indent );
	}

};

class MatrixDumper : public FloatArrayDumper
{
public:
	MatrixDumper( SectionPtr &section ) : FloatArrayDumper( "matrix", section )
	{

	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		Matrix matrix = section_->as_matrix();
		dump_float_array( buffer, matrix.m_, 16, indent );
	}
};

class StringDumper : public Dumper
{
public:
	StringDumper( SectionPtr &section ) : Dumper( "string", section )
	{

	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		const std::string &value = section_->as_string();
		BinaryPtr encoded = binary_encode( value.c_str(), (int)value.size() );
		buffer.write( encoded->get_buffer(), encoded->get_length() );
	}
};

class BinaryDumper : public Dumper
{
public:
	BinaryDumper( SectionPtr &section ) : Dumper( "binary", section )
	{

	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
		BinaryPtr value = section_->as_binary();
		BinaryPtr encoded = binary_encode( value->get_buffer(), value->get_length() );
		buffer.write( encoded->get_buffer(), encoded->get_length() );
	}
};

class StructDumper : public Dumper
{
public:
	StructDumper( SectionPtr &section ) : Dumper( "struct", section )
	{

	}

	virtual void dump_value( Buffer &buffer, int indent )
	{
	}
};

class TextSectionDumper : public SectionDumper
{
public:
	TextSectionDumper( SectionPtr &section ) : SectionDumper( section )
	{
	}

	virtual void do_dump()
	{
		indent_ = -1;
		info_index_ = 1; // skip the first section
		buffer_.write( "#dog\n", 5 );
		dump_struct_value( section_infos_[0] );
	}

private:
	void dump_struct_value( SectionInfo &root_info )
	{
		indent_ += 1;

		for ( int i = 0; i < root_info.extra_info_; ++i ) {
			dump_indent();
			SectionInfo &info = section_infos_[ info_index_++ ];
			DumperPtr dumper = query_dumper_by_type( info.type_, info.section_ );

			dumper->dump_basic_info( buffer_, info );

			if ( info.type_ == SECTION_TYPE_STRUCT ) {
				dump_struct_brace_value( info );
			} else {
				dumper->dump_value( buffer_, indent_ );
				buffer_.write( "\n", 1 );
			}
		}

		indent_ -= 1;
	}

	void dump_struct_brace_value( SectionInfo &root_info )
	{
		buffer_.write( "{\n", 2 );
		dump_struct_value( root_info );
		dump_indent();
		buffer_.write( "}\n", 2 );
	}

	void dump_indent( int indent = 0)
	{
		if ( !indent ) {
			indent = indent_;
		}
		for ( int i = 0; i < indent; ++i ) {
			buffer_.write( "\t", 1 );
		}
	}

	DumperPtr query_dumper_by_type( int type, SectionPtr &section )
	{
		switch( type ) {
		case SECTION_TYPE_BOOL:
			return new BoolDumper( section );
		case SECTION_TYPE_INT:
			return new IntDumper( section );
		case SECTION_TYPE_FLOAT:
			return new FloatDumper( section );
		case SECTION_TYPE_POINT:
			return new PointDumper( section );
		case SECTION_TYPE_VECTOR:
			return new VectorDumper( section );
		case SECTION_TYPE_MATRIX:
			return new MatrixDumper( section );
		case SECTION_TYPE_STRING:
			return new StringDumper( section );
		case SECTION_TYPE_BINARY:
			return new BinaryDumper( section );
		case SECTION_TYPE_STRUCT:
			return new StructDumper( section );
		}

		log_error( "invalid dumper type [%d]", type );
		return 0;
	}

private:
	int indent_;
	int info_index_;
};

class BinarySectionDumper : public SectionDumper
{
	typedef std::map< Point, int > PointMap;
	typedef std::map< Vector, int > VectorMap;
	typedef std::map< Matrix, int > MatrixMap;
	typedef std::map< std::string, int > StringMap;
public:
	BinarySectionDumper( SectionPtr &section ) : SectionDumper( section )
	{
	}

	virtual void do_dump()
	{
		gather_values();

		// dump binary section head
		buffer_.write( "#doe", 4 );
		
		int temp = (int)section_infos_.size();
		buffer_.write( &temp, sizeof( int ) ); // offset, temp offset, will replace at last
		buffer_.write( &temp, sizeof( int ) ); // info number

		dump_values();

		temp = buffer_.get_write_pos();
		dump_infos();
		buffer_.write_at( 4, &temp, sizeof( int ) ); // offset
	}

private:
	void gather_values()
	{
		for ( SectionInfoVector::iterator it = section_infos_.begin(); it != section_infos_.end(); ++it ) {
			strings_[ it->name_ ] = 0;

			switch ( it->type_ ) {
			case SECTION_TYPE_POINT:
				points_[ it->section_->as_point() ] = 0;
				break;
			case SECTION_TYPE_VECTOR:
				vectors_[ it->section_->as_vector() ] = 0;
				break;
			case SECTION_TYPE_MATRIX:
				matrixs_[ it->section_->as_matrix() ] = 0;
				break;
			case SECTION_TYPE_STRING:
				strings_[it->section_->as_string()] = 0;
				break;
			case SECTION_TYPE_BINARY:
				strings_[ it->section_->as_string() ] = 0;
				break;
			}
		}
	}

	void dump_values()
	{
		// dump point
		for ( PointMap::iterator it = points_.begin(); it != points_.end(); ++it ) {
			it->second = buffer_.get_write_pos();
			buffer_.write( &it->first, sizeof( Point ) );
		}

		// dump vector
		for ( VectorMap::iterator it = vectors_.begin(); it != vectors_.end(); ++it ) {
			it->second = buffer_.get_write_pos();
			buffer_.write( &it->first, sizeof( Vector ) );
		}

		// dump matrix
		for ( MatrixMap::iterator it = matrixs_.begin(); it != matrixs_.end(); ++it ) {
			it->second = buffer_.get_write_pos();
			buffer_.write( &it->first, sizeof( Matrix ) );
		}

		// dump string
		for ( StringMap::iterator it = strings_.begin(); it != strings_.end(); ++it ) {
			it->second = buffer_.get_write_pos();
			dump_string( it->first );
		}
	}

	void dump_string( const std::string &value )
	{
		// size
		int size = (int) value.size();
		buffer_.write( &size, sizeof( int ) );
		// content
		buffer_.write( value.c_str(), size );
		// appendent
		int appendent = 0;
		int append_size = 4 - size % 4;
		buffer_.write( &appendent, append_size );
	}

	void dump_infos()
	{
		for ( SectionInfoVector::iterator it = section_infos_.begin(); it != section_infos_.end(); ++it ) {
			BinarySectionEntry entry = get_section_entry( *it );
			buffer_.write( &entry, sizeof( BinarySectionEntry ) );
		}
	}

	BinarySectionEntry get_section_entry( SectionInfo &info )
	{
		BinarySectionEntry entry;
		entry.set_type( info.type_ );
		entry.set_name_offset( strings_[ info.name_ ] );

		switch ( info.type_ ) {
		case SECTION_TYPE_BOOL:
			entry.value_ = (int)info.section_->as_bool();
			break;

		case SECTION_TYPE_INT:
			entry.value_ = info.section_->as_int();
			break;

		case SECTION_TYPE_FLOAT:
			{
				float value = info.section_->as_float();
				memcpy( &entry.value_, &value, sizeof( float ) );
				break;
			}

		case SECTION_TYPE_POINT:
			entry.value_ = points_[ info.section_->as_point() ];
			break;

		case SECTION_TYPE_VECTOR:
			entry.value_ = vectors_[ info.section_->as_vector() ];
			break;

		case SECTION_TYPE_MATRIX:
			entry.value_ = matrixs_[ info.section_->as_matrix() ];
			break;

		case SECTION_TYPE_STRING:
		case SECTION_TYPE_BINARY:
			entry.value_ = strings_[ info.section_->as_string() ];
			break;

		default: // SECTION_TYPE_STRUCT
			entry.value_ = info.extra_info_;
		}
		return entry;
	}

private:
	PointMap points_;
	VectorMap vectors_;
	MatrixMap matrixs_;
	StringMap strings_;
};

SectionPtr parse_text_section( BinaryPtr &binary )
{
	TextSectionParser parser( binary );
	return parser.parse();
}

SectionPtr parse_binary_section( BinaryPtr &binary )
{
	BinarySectionParser parser( binary );
	return parser.parse();
}

SectionPtr parse_section( BinaryPtr &binary )
{
	if ( !binary || binary->get_length() < 4 ) {
		return 0;
	}

	if ( memcmp( binary->get_buffer(), "#dog", 4 ) == 0 ) {
		return parse_text_section( binary );
	}else if ( memcmp( binary->get_buffer(), "#doe", 4 ) == 0 ) {
		return parse_binary_section( binary );
	}

	return 0;
}

SectionPtr new_section()
{
	return new StructSection;
}

BinaryPtr dump_section_in_text( SectionPtr &section )
{
	TextSectionDumper dumper( section );
	return dumper.dump();
}

BinaryPtr dump_section_in_binary( SectionPtr &section )
{
	BinarySectionDumper dumper( section );
	BinaryPtr bin = dumper.dump();

	if ( bin->get_length() > 1024 * 1024 * 16 ) {
		log_error( "[dump_section_in_binary] binary section file can't not large than 16M" );
		return 0;
	}
	return bin;
}

