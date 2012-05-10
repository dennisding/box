#include <cctype>

#include "section_parser.hpp"

#include "utils/log.hpp"
#include "utils/buffer.hpp"

#define SECTION_TYPE_STRUCT 0
#define SECTION_TYPE_BOOL	1
#define SECTION_TYPE_INT	2
#define SECTION_TYPE_FLOAT	3
#define SECTION_TYPE_POINT	4
#define SECTION_TYPE_VECTOR	5
#define SECTION_TYPE_MATRIX	6
#define SECTION_TYPE_STRING	7
#define SECTION_TYPE_BINARY	8

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
		return sections_[ name ];
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

	virtual void write_section( const std::string &name, SectionPtr &section )
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
		do_encode();
		
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

class TokenParser
{
public:
	TokenParser( BinaryPtr &binary ) : binary_( binary )
	{
		line_ = 0;
		index_ = 4; // skip the #id
		buffer_ = binary->get_buffer();
		length_ = binary->get_length();
	}

	const std::string next_token()
	{
		skip_space();
		if ( index_ >= length_ ) {
			return "";
		}
		int begin = index_;
		if ( is_special_char( buffer_[begin] ) ) {
			++index_;
			return std::string( buffer_ + begin, index_ - begin );
		} else if ( buffer_[begin] == '#' ) {
			skip_line();
			return next_token();
		}

		while ( index_ < length_ ) {
			if ( is_special_char( buffer_[ index_ ] ) ) {
				break;
			}
			if ( isspace( (unsigned char)buffer_[ index_ ] )) {
				break;
			}
			++index_;
		}
		return std::string( buffer_ + begin, index_ - begin );
	}

public:
	int line_;

private:
	void skip_line()
	{
		while ( index_ < length_  && buffer_[ index_ ] != '\n' ) {
			++index_;
		}
		if ( index_ < length_ ) { // skip the '\n' character
			++index_;
		}
	}

	inline bool is_special_char( char c ) 
	{
		return c == '{' || c == '}' || c == '=';
	}

	void skip_space()
	{
		while ( index_ < length_ ) {
			if ( buffer_[ index_ ] == '\n' ) {
				++line_;
			}

			if ( !isspace( (unsigned char )buffer_[ index_ ] ) ) {
				return;
			}

			++index_;
		}
	}

private:
	BinaryPtr binary_;
	int length_;
	int index_;
	const char *buffer_;
};

class TextSectionParser
{
public:
	TextSectionParser( BinaryPtr &binary ) : token_parser_( binary )
	{
	}

	SectionPtr parse()
	{
		SectionPtr root( new StructSection );

		while ( true ) {
			std::string type_name = token_parser_.next_token();
			if ( type_name.empty() || type_name == "}" ) {
				return root;
			}
			std::string name = token_parser_.next_token();
			if ( name.empty() ) {
				syntax_erro( "name" );
				return 0;
			}

			std::string equal_sign = token_parser_.next_token();
			if ( equal_sign != "=" ) {
				syntax_erro( "equal sign =" );
				return 0;
			}

			SectionPtr section = parse_type( type_name );
			if ( !section ) {
				return 0;
			}
			root->write_section( name, section );
		}

	}

private:
	SectionPtr parse_type( const std::string &type_name )
	{
		if ( type_name == "int" ) {
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
		return 0;
	}

	SectionPtr parse_int_section()
	{
		std::string token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "int" );
			return 0;
		}
		int value = atol( token.c_str() );
		return new IntSection( value );
	}

	SectionPtr parse_float_section()
	{
		std::string token = token_parser_.next_token();
		if ( token.empty() ) {
			log_err( "section float error near line: %d", token_parser_.line_ );
			return 0;
		}
		float value = (float)atof( token.c_str() );
		return new FloatSection( value );
	}

	SectionPtr parse_point_section()
	{
		std::string token = token_parser_.next_token();
		if ( token != "{" ) {
			syntax_erro( "point {" );
			return 0;
		}
		token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "point number1" );
			return 0;
		}
		Point point;
		point.x_ = (float)atof( token.c_str() );

		token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "point number2" );
			return 0;
		}
		point.y_ = (float)atof( token.c_str() );

		token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "point }" );
			return 0;
		}
		return new PointSection( point );
	}

	SectionPtr parse_vector_section()
	{
		return 0;
	}

	SectionPtr parse_matrix_section()
	{
		return 0;
	}

	SectionPtr parse_string_section()
	{
		std::string token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "string" );
			return 0;
		}
		BinaryPtr binary = binary_decode( token.c_str(), (int)token.size() );
		return new StringSection( std::string( binary->get_buffer(), binary->get_length() ) );
	}

	SectionPtr parse_binary_section()
	{
		std::string token = token_parser_.next_token();
		if ( token.empty() ) {
			syntax_erro( "binary" );
			return 0;
		}
		return new BinarySection( binary_decode( token.c_str(), (int)token.size() ) );
	}

	SectionPtr parse_struct_section()
	{
		std::string token = token_parser_.next_token();
		if ( token != "{" ) {
			syntax_erro( "struct {" );
			return 0;
		}
		return parse();
	}

private:
	inline void syntax_erro( const std::string &identify )
	{
		log_err( "section [%s] syntax error near line : %d ", identify.c_str(), token_parser_.line_ );
	}

private:
	TokenParser token_parser_;
};

typedef std::vector< SectionInfo > SectionInfoVector;

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
			DumpInfo dump_info = query_dump_info( info.type_ );
			
			buffer_.write( dump_info.type_name_.c_str(), dump_info.type_name_.size() );
			buffer_.write( " ", 1 );
			buffer_.write( info.name_.c_str(), info.name_.size() );
			buffer_.write( " = ", 3 );
			if ( dump_info.has_brace_ ) {
				buffer_.write( "{\n", 2 );
			}

			dump_section_content( info );

			if ( dump_info.has_brace_ ) {
				dump_indent();
				buffer_.write( "}", 1 );
			}
			buffer_.write( "\n", 1 );

		}
		indent_ -= 1;
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

	DumpInfo query_dump_info( int type )
	{
		switch( type ) {
		case SECTION_TYPE_INT:
			return DumpInfo( "int" );
		case SECTION_TYPE_FLOAT:
			return DumpInfo( "float" );
		case SECTION_TYPE_POINT:
			return DumpInfo( "point" );
		case SECTION_TYPE_STRING:
			return DumpInfo( "string" );
		case SECTION_TYPE_BINARY:
			return DumpInfo( "binary" );
		case SECTION_TYPE_STRUCT:
			return DumpInfo( "struct", true );
		}
		return DumpInfo( "error" );
	}

	void dump_section_content( SectionInfo &info )
	{
		switch( info.type_ ) {
		case SECTION_TYPE_INT:
			dump_int_value( info );
			break;
		case SECTION_TYPE_FLOAT:
			dump_float_value( info );
			break;
		case SECTION_TYPE_POINT:
			dump_point_value( info );
			break;
		case SECTION_TYPE_STRING:
			dump_string_value( info );
			break;
		case SECTION_TYPE_BINARY:
			dump_binary_value( info );
			break;
		case SECTION_TYPE_STRUCT:
			dump_struct_value( info );
			break;
		}
	}

	void dump_int_value( SectionInfo &info )
	{
		char buffer[32];
		int count = sprintf( buffer, "%d", info.section_->as_int() );
		buffer_.write( buffer, count );
	}

	void dump_float_value( SectionInfo &info )
	{
		char buffer[32];
		int count = sprintf( buffer, "%f", info.section_->as_float() );
		buffer_.write( buffer, count );
	}

	void dump_point_value( SectionInfo &info )
	{
		char buffer[32];
		buffer_.write( "{ ", 2 );
		Point point = info.section_->as_point();
		int count = sprintf( buffer, "%f", point.x_ );
		buffer_.write( buffer, count );
		buffer_.write( " ", 1 );
		count = sprintf( buffer, "%f", point.y_ );
		buffer_.write( buffer, count );
		buffer_.write( " }", 2 );
	}

	void dump_string_value( SectionInfo &info )
	{
		std::string value = info.section_->as_string();
		BinaryPtr binary = binary_encode( value.c_str(), (int)value.size() );
		buffer_.write( binary->get_buffer(), binary->get_length() );
	}

	void dump_binary_value( SectionInfo &info )
	{
		BinaryPtr value = info.section_->as_binary();
		BinaryPtr binary = binary_encode( value->get_buffer(), value->get_length() );
		buffer_.write( binary->get_buffer(), binary->get_length() );
	}

private:
	int indent_;
	int info_index_;
};

SectionPtr parse_text_section( BinaryPtr &binary )
{
	TextSectionParser parser( binary );
	return parser.parse();
}

SectionPtr parse_binary_section( BinaryPtr &binary )
{
	return SectionPtr();
}

SectionPtr parse_section( BinaryPtr &binary )
{
	if ( binary->get_length() < 4 ) {
		return 0;
	}
	if ( memcmp( binary->get_buffer(), "#dog", 4 ) == 0 ) {
		return parse_text_section( binary );
	}else if ( memcmp( binary->get_buffer(), "#doe", 4 ) == 0 ) {
		return parse_binary_section( binary );
	}
	return SectionPtr();
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

BinaryPtr test_encode( BinaryPtr &binary )
{
	BinaryPtr encode = binary_encode( binary->get_buffer(), binary->get_length() );
	return binary_decode( encode->get_buffer(), encode->get_length() );
}