
#include "matrix.hpp"


Matrix Matrix::identity;

namespace {
	class MakeIdentity
	{
	public:
		MakeIdentity( Matrix &matrix )
		{
			matrix.makeIdentity();
		}
	};

	MakeIdentity makeIdentityInstance( Matrix::identity );
}
