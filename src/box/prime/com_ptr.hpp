#ifndef _COM_PTR_HPP_
#define _COM_PTR_HPP_

template < typename Type >
class ComPtr
{
public:
	inline ComPtr() : obj_(0)
	{

	}

	inline ComPtr( Type *obj ) : obj_( obj )
	{

	}

	inline ~ComPtr()
	{
		if ( obj_ ) {
			obj_->Release();
		}
	}

	inline Type *operator->()
	{
		return obj_;
	}

	inline ComPtr( ComPtr &ptr ) : obj_( ptr.obj_ )
	{
		if ( obj_ ) {
			obj_->AddRef();
		}
	}

	inline void operator = ( ComPtr &ptr )
	{
		if ( obj_ ) {
			obj_->Release();
		}
		obj_ = ptr.obj_;

		if ( obj_ ) {
			obj_->AddRef();
		}
	}

	inline operator void *()
	{
		return obj_;
	}

private:
	Type *obj_;
};

#endif
