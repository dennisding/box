#include "scanner.hpp"

#include <cctype>

TokenType Scanner::scan()
{
	skip_space_and_comment();

	token_begin_ = index_;
	unsigned char ch = read_char();
	if ( ch == '\'' ) {
		return scan_string();
	} else if ( isdigit( ch ) ) {
		return scan_number();
	} else if ( isalpha( ch ) || ch == '_' ) {
		return scan_identify();
	} else if ( ch != 0 ) {
		return TOKEN_CHAR;
	}
	
	return TOKEN_UNKNOWN;
}

const std::string Scanner::scan_unknow_token()
{
	skip_space_and_comment();
	token_begin_ = index_;
	unsigned char ch = peek_char();
	
	while ( !iscntrl( ch ) && !isspace( ch ) ) {
		read_char();
		ch = peek_char();
	}

	return std::string( buffer_->get_buffer() + token_begin_, index_ - token_begin_ );
}

void Scanner::skip_space_and_comment()
{
	unsigned char ch;

	bool comment = false;
	while ( (ch = peek_char()) ) {
		if ( (ch == '#') || (ch == '/' && peek_next_char() == '/' ) ) {
			comment = true;
		}

		if ( !comment && !isspace( ch ) && !iscntrl( ch ) ) {
			break;
		}

		ch = read_char();

		if ( ch == '\n' ) {
			comment = false;
		}
	}
}

TokenType Scanner::scan_string()
{
	unsigned char ch = peek_char();
	bool escape = false;

	while ( ch ) {
		if ( ch == '\'' && !escape ) {
			read_char();
			break;
		}
		
		ch = read_char();
		if ( ch == '\\' ) {
			escape = true;
		} else {
			escape = false;
		}
		ch = peek_char();
	}

	return TOKEN_STRING;
}

TokenType Scanner::scan_number()
{
	TokenType token_type = TOKEN_INT;
	unsigned char ch = peek_char();
	while ( isdigit( ch ) || ch == '.' ) {
		if ( token_type == TOKEN_FLOAT && ch == '.' ) {
			return TOKEN_UNKNOWN;
		}
		if ( ch == '.' ) {
			token_type = TOKEN_FLOAT;
		}

		read_char();
		ch = peek_char();
	}

	return token_type;
}

TokenType Scanner::scan_identify()
{
	unsigned char ch = peek_char();
	while ( isalnum( ch ) || ch == '_' ) {
		read_char();
		ch = peek_char();
	}
	return TOKEN_IDENTIFY;
}
