#include "log.hpp"

#include <cstdio>
#include <cstdarg>

void log_msg( const char *format, ... )
{
	va_list args;

	va_start( args, format );
	vprintf( format, args );
	va_end( args );
}