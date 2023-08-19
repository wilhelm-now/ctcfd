#include <util/minus.h>
#include <util/ct_assert.h>
#include <util/is_same.h>
#include <util/ones.h>
#include <util/for_i.h>


template<int offset = 0>
struct identity_plus
{
	template<unsigned x, unsigned y = 0>
	struct func
	{
		typedef NUMBER_MAKE(x + y + offset) type;
	};
};

struct identity : identity_plus<0>{};

int main()
{
	// one dimensional
	typedef for_i<20, identity>::type xs;
	typedef for_i<tl::length<xs>::value, identity_plus<1> >::type ys;

	typedef minus<ys, xs>::type subtracted;
	CT_ASSERT((is_same<ones<tl::length<xs>::value>::type, subtracted>::value));

	// two dimensional
	typedef for_ij<20, 30, identity>::type xs2d;
	typedef for_ij<20, 30, identity_plus<1> >::type ys2d;

	typedef minus<ys2d, xs2d>::type subtracted2d;
	CT_ASSERT((is_same<ones2d<20, 30>::type, subtracted2d>::value));
}