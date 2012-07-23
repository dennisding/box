#include "thread_task_mgr.hpp"

#include <vector>

typedef std::vector< TaskThreadPtr > TaskThreadVector;

class ThreadPool
{
public:
	ThreadPool( int thread_num );

	void fini();
	void tick();

	void add_task( ThreadTaskPtr &task );

private:
	int flag_;
	TaskThreadVector threads_;
};

static ThreadPool io_threads_( 2 );
static ThreadPool default_threads_(3);

TaskThread::TaskThread() : quit_( false )
{

}

TaskThread::~TaskThread()
{
	AUTO_LOCK( locker_ );
	tasks_.clear();
}

void TaskThread::run()
{
	while ( !quit_ ) {
		ThreadTaskPtr task = get_task();
		if ( task ) {
			task->run();
			set_to_finish( task );
		}
		Sleep( 1 );
	}
}

void TaskThread::stop()
{
	quit_ = true;
}

void TaskThread::add_task( ThreadTaskPtr &task )
{
	AUTO_LOCK( locker_ );
	tasks_.push_back( task );
}

ThreadTaskPtr TaskThread::get_task()
{
	AUTO_LOCK( locker_ );
	
	if ( tasks_.empty() ) {
		return 0;
	}

	ThreadTaskPtr task = tasks_.front();
	tasks_.pop_front();
	return task;
}

void TaskThread::set_to_finish( ThreadTaskPtr &task )
{
	AUTO_LOCK( locker_ );
	finished_tasks_.push_back( task );
}

void TaskThread::tick()
{
	ThreadTaskList tasks;
	locker_.lock();
	tasks.swap( finished_tasks_ );
	locker_.unlock();

	for ( ThreadTaskList::iterator it = tasks.begin(); it != tasks.end(); ++it ) {
		(*it)->on_finish();
	}
}

ThreadPool::ThreadPool( int thread_num ) : flag_( 0 )
{
	threads_.resize( thread_num );
	for ( int i = 0; i < thread_num; ++i ) {
		threads_[i] = new TaskThread();
		threads_[i]->start();
	}
}

void ThreadPool::fini()
{
	for ( TaskThreadVector::iterator it = threads_.begin(); it != threads_.end(); ++it ) {
		(*it)->stop();
	}
	threads_.clear();
}

void ThreadPool::tick()
{
	for ( TaskThreadVector::iterator it = threads_.begin(); it != threads_.end(); ++it ) {
		(*it)->tick();
	}
}

void ThreadPool::add_task( ThreadTaskPtr &task )
{
	flag_ %= threads_.size();
	threads_[ flag_ ]->add_task( task );
}

void ThreadTaskMgr::fini()
{
	default_threads_.fini();
	io_threads_.fini();
}

void ThreadTaskMgr::add_task( ThreadTaskPtr &task )
{
	default_threads_.add_task( task );
}

void ThreadTaskMgr::add_io_task( ThreadTaskPtr &task )
{
	io_threads_.add_task( task );
}

void ThreadTaskMgr::tick()
{
	io_threads_.tick();
	default_threads_.tick();
}