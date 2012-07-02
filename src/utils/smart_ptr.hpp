#ifndef _SMART_PTR_HPP_
#define _SMART_PTR_HPP_

#include <Windows.h>

class Object
{
public:
	inline Object() : _reference_counter_( 1 ) 
	{
	}

	inline virtual ~Object() 
	{
	}

	inline void _increase_ref()
	{
		InterlockedIncrement( &_reference_counter_ );
	}

	inline long _decrease_ref()
	{
		return InterlockedDecrement( &_reference_counter_ );
	}

private:
	volatile long _reference_counter_;

private:
	Object( const Object &obj ); // you are not allow to use copy constructor to create a new object
};

template < typename Type >
class SmartPtr
{
public:
	inline SmartPtr() : obj_( 0 )
	{
	}

	inline SmartPtr( Type *obj )  : obj_( obj )
	{
	}

	static inline SmartPtr raw_wrap( Type *obj )
	{
		if ( obj ) {
			obj->_increase_ref();
		}
		return SmartPtr( obj );
	}

	inline SmartPtr( const SmartPtr &ptr )
	{
		obj_ = const_cast< Type *>( ptr.obj_ );
		if ( obj_ ) {
			obj_->_increase_ref();
		}
	}
	
	inline ~SmartPtr() 
	{
		if ( obj_ && obj_->_decrease_ref() == 0 ) {
			delete obj_;
		}
	}

	inline void operator=( const SmartPtr &ptr ) {
		if ( obj_ && obj_->_decrease_ref() == 0 ) {
			delete obj_;
		}

		obj_ = const_cast< Type *>( ptr.obj_ );
		if ( obj_ ) {
			obj_->_increase_ref();
		}
	}

	inline Type &operator*()
	{
		return *obj_;
	}

	inline Type *operator->()
	{
		return obj_;
	}

	inline bool is_null() const
	{
		return obj_ == 0;
	}

	 inline operator void*()
	 {
		 return obj_;
	 }

	 inline Type *get_object() 
	 {
		 return obj_;
	 }

private:
	Type *obj_;
};

template < typename Type >
inline bool operator==( const SmartPtr< Type > &ptr1, const SmartPtr< Type > &ptr2 )
{
	return ptr1.obj_ == ptr2.obj_;
}

template < typename Type >
inline bool operator<( const SmartPtr< Type > &ptr1, const SmartPtr< Type > &ptr2 )
{
	return ptr1.obj_ < ptr2.obj_;
}

#endif
