#include <util/for_i.h>
#include <util/valuelist.h>
#include <util/is_same.h>

#include <util/ct_assert.h>

struct identity_value
{
	template<unsigned idx>
	struct func
	{
		typedef NUMBER_MAKE(idx) type;
	};
};

struct test_2d
{
	template<unsigned i, unsigned j>
	struct func
	{
		typedef NUMBER_MAKE((i * 10) + j) type;
	};
};

int main()
{
	typedef NUMBERLIST_5(0, 1, 2, 3, 4) zero2four;
	CT_ASSERT((is_same<zero2four, for_i<5, identity_value>::type>::value));

	typedef NUMBERLIST_16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) to16;
	CT_ASSERT((is_same<to16, for_i<16, identity_value>::type>::value));


	typedef TYPELIST_3(
		NUMBERLIST_4(0, 1, 2, 3),
		NUMBERLIST_4(10, 11, 12, 13),
		NUMBERLIST_4(20, 21, 22, 23)
	) expected2d;

	CT_ASSERT((is_same<for_ij<3, 4, test_2d>::type, expected2d>::value));
}