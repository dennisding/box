#include "thread.hpp"

#include <process.h>

static void __cdecl thread_process( void *argc )
{
	Thread *thread = ( Thread *)argc;
	thread->run();
}

Thread::Thread() : thread_id_(0)
{

}

void Thread::start()
{
	thread_id_ = _beginthread( thread_process, 0, this );
}

void Thread::stop()
{
	_endthreadex( thread_id_ );
}