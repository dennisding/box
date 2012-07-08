#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include "utils/smart_ptr.hpp"

#include <windows.h>

class Locker
{
public:
	Locker();
	~Locker();
	void lock();
	bool try_lock();
	void unlock();

private:
	CRITICAL_SECTION critical_section_;

private:
	Locker( const Locker & );
};

class AutoLocker
{
public:
	AutoLocker( Locker &locker );
	~AutoLocker();
private:
	Locker *locker_;
};

#define AUTO_LOCK( locker ) AutoLocker _auto_locker_ins( locker )

class Thread : public Object
{
public:
	Thread();
	void start();
	void stop();

	virtual void run() = 0;

private:
	unsigned thread_id_;
};

typedef SmartPtr< Thread > ThreadPtr;

// inline function
inline Locker::Locker()
{
	InitializeCriticalSection( &critical_section_ );
}

inline Locker::~Locker()
{
	DeleteCriticalSection( &critical_section_ );
}

inline void Locker::lock()
{
	EnterCriticalSection( &critical_section_ );
}

inline bool Locker::try_lock()
{
	TryEnterCriticalSection( &critical_section_ );
}

inline void Locker::unlock()
{
	LeaveCriticalSection( &critical_section_ );
}

inline AutoLocker::AutoLocker( Locker &locker )
{
	locker.lock();
	locker_ = &locker;
}

inline AutoLocker::~AutoLocker()
{
	locker_->unlock();
}

#endif
