
#include "matrix.hpp"


Matrix Matrix::identity;

namespace {
	class MakeIdentity
	{
	public:
		MakeIdentity( Matrix &matrix )
		{
			matrix.make_identify();
		}
	};

	MakeIdentity makeIdentityInstance( Matrix::identity );
}
