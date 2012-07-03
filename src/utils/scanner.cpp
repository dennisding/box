#include "scanner.hpp"

#include <cctype>

ScanTokenType Scanner::scan()
{
	skip_space_and_comment();
	
	ScanTokenType token = TOKEN_UNKNOWN;
	int token_begin = index_;

	unsigned char ch = read_char();
	if ( ch == '\'' ) {
		token = scan_string();
	} else if ( isdigit( ch ) ) {
		token = scan_number();
	} else if ( isalpha( ch ) || ch == '_' ) {
		token = scan_identify();
	} else if ( ch != 0 ) {
		token = TOKEN_CHAR;
	}

	token_text_ = std::string( buffer_->get_buffer() + token_begin, index_ - token_begin );
	return token;
}

const std::string Scanner::scan_unknow_token()
{
	skip_space_and_comment();
	
	int token_begin = index_;
	unsigned char ch = peek_char();
	
	while ( !iscntrl( ch ) && !isspace( ch ) ) {
		read_char();
		ch = peek_char();
	}

	token_text_ = std::string( buffer_->get_buffer() + token_begin, index_ - token_begin );
	return token_text_;
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

ScanTokenType Scanner::scan_string()
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

ScanTokenType Scanner::scan_number()
{
	ScanTokenType token_type = TOKEN_INT;
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

ScanTokenType Scanner::scan_identify()
{
	unsigned char ch = peek_char();
	while ( isalnum( ch ) || ch == '_' ) {
		read_char();
		ch = peek_char();
	}
	return TOKEN_IDENTIFY;
}
