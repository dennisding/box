#ifndef _LOG_HPP_
#define _LOG_HPP_


void log_msg( const char *format, ... );

#define log_critical( format, ... ) log_msg( "critical: " format "\n", __VA_ARGS__ )
#define log_error( format, ... ) log_msg( "error: " format "\n", __VA_ARGS__ )
#define log_warn( format, ... ) log_msg( "warn: " format "\n", __VA_ARGS__ )
#define log_log( format, ... ) log_msg( "log: " format "\n", __VA_ARGS__ )
#define log_assert( con, format, ... ) if ( !(con) ) { log_msg( "log_assert:" #con format "\n", __VA_ARGS__ ); ((int *)0)=0; }

#endif
