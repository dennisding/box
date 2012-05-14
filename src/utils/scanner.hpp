#ifndef _SCANNER_HPP_
#define _SCANNER_HPP_

#include "binary.hpp"
#include "smart_ptr.hpp"

#include <string>

enum TokenType {
	TOKEN_UNKNOWN,
	TOKEN_CHAR,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STRING,
	TOKEN_IDENTIFY,
};

class Scanner : public Object
{
public:
	Scanner( BinaryPtr &bin, int start_pos = 0 );

	TokenType scan();
	const std::string &scan_text();
	const std::string &token_text();
	// token split by space
	const std::string scan_unknow_token();

public:
	int line_;

private:
	void skip_space_and_comment();
	unsigned char read_char();
	unsigned char peek_char();
	unsigned char peek_next_char();

	TokenType scan_string();
	TokenType scan_number();
	TokenType scan_identify();

private:
	BinaryPtr buffer_;
	int index_;

	std::string token_text_;
};

// inline
inline Scanner::Scanner( BinaryPtr &bin, int start_pos ) 
	: line_(0), buffer_( bin ), index_( start_pos )
{
}

inline const std::string &Scanner::scan_text()
{
	scan();
	return token_text_
}

inline const std::string &Scanner::token_text()
{
	return token_text_;
}

inline unsigned char Scanner::read_char()
{
	if ( buffer_->get_length() <= index_ ) { // end of buffer
		return 0;
	}
	unsigned char ch = buffer_->get_buffer()[ index_++ ];
	if ( ch == '\n' ) {
		line_++;
	}
	return ch;
}

inline unsigned char Scanner::peek_char()
{
	if ( buffer_->get_length() <= index_ ) { // end of buffer
		return 0;
	}
	return buffer_->get_buffer()[ index_ ];
}

inline unsigned char Scanner::peek_next_char()
{
	if ( buffer_->get_length() <= index_ + 1 ) { // end of buffer
		return 0;
	}
	return buffer_->get_buffer()[ index_ + 1];
}


#endif
