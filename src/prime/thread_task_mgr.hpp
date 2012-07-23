#ifndef _THREAD_TASK_MGR_HPP_
#define _THREAD_TASK_MGR_HPP_

#include "thread.hpp"

#include "utils/smart_ptr.hpp"

#include <list>

#define THREAD_DEFAULT 0
#define THREAD_IO 1

class ThreadTask : public Object
{
public:
	virtual ~ThreadTask() {}
	virtual void run() = 0;
	virtual void on_finish() {} // call by logic thread
};

typedef SmartPtr< ThreadTask > ThreadTaskPtr;
typedef std::list< ThreadTaskPtr > ThreadTaskList;

class TaskThread : public Thread
{
public:
	TaskThread();
	virtual ~TaskThread();
	virtual void run();
	virtual void stop();
	void add_task( ThreadTaskPtr &task );
	void tick();

private:
	ThreadTaskPtr get_task();
	void set_to_finish( ThreadTaskPtr &task );

private:
	bool quit_;
	ThreadTaskList tasks_;
	ThreadTaskList finished_tasks_;
	Locker locker_;
};

typedef SmartPtr< TaskThread > TaskThreadPtr;

class ThreadTaskMgr
{
public:
	static void fini();
	static void add_task( ThreadTaskPtr &task );
	static void add_io_task( ThreadTaskPtr &task );
	static void tick();
};

#endif
