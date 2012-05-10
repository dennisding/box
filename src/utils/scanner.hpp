#ifndef _SCANNER_HPP_
#define _SCANNER_HPP_

#include "binary.hpp"
#include "smart_ptr.hpp"

#include <string>

enum TokenType {
	TOKEN_UNKNOW,
	TOKEN_BOOL,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STRING,
};

class Scanner : public Object
{
public:
	Scanner( BinaryPtr &bin, int start_pos = 0 );

	TokenType next_token();
	const std::string &token_text();

private:
	BinaryPtr buffer_;
	int index_;
	std::string token_text_;
};

// inline
inline Scanner::Scanner( BinaryPtr &bin, int start_pos ) : buffer_( bin ), index_( start_pos )
{
}

inline const std::string &Scanner::token_text()
{
	return token_text_;
}

#endif
