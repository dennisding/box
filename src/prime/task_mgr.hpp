#ifndef _TASK_MGR_HPP_
#define _TASK_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include <vector>

class Task : public Object
{
public:
	Task( const std::string &name ) : name_( name )
	{

	}

	virtual ~Task() {}

	virtual void init() = 0;
	virtual void fini() = 0;
	virtual void tick() = 0;
	virtual void draw() = 0;

private:
	std::string name_;
};

typedef SmartPtr< Task > TaskPtr;
typedef std::vector< TaskPtr > TaskVector;

class TaskMgr
{
public:
	static void add_task( TaskPtr &task );
	
	static void init();
	static void fini();
	static void tick();
	static void draw();

private:
	static TaskVector tasks_; 
};

// inline
inline void TaskMgr::add_task( TaskPtr &task )
{
	tasks_.push_back( task );
}

inline void TaskMgr::init()
{
	for ( TaskVector::iterator it = tasks_.begin(); it != tasks_.end(); ++it ) {
		(*it)->init();
	}
}

inline void TaskMgr::fini()
{
	for ( TaskVector::iterator it = tasks_.begin(); it != tasks_.end(); ++it ) {
		(*it)->fini();
	}
}

inline void TaskMgr::tick()
{
	for ( TaskVector::iterator it = tasks_.begin(); it != tasks_.end(); ++it ) {
		(*it)->tick();
	}
}

inline void TaskMgr::draw()
{
	for ( TaskVector::iterator it = tasks_.begin(); it != tasks_.end(); ++it ) {
		(*it)->draw();
	}
}

#endif
