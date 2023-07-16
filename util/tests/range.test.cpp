#include <util/is_same.h>
#include <util/valuelist.h>
#include <util/ct_assert.h>

#include <util/range.h>

int main()
{
	typedef NUMBERLIST_10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) expected_range_0_10_1;
	CT_ASSERT((is_same<expected_range_0_10_1, range<0, 10, 1>::type>::value));

	typedef NUMBERLIST_10(10, 9, 8, 7, 6, 5, 4, 3, 2, 1) expectd_range_10_0_negative_1;
	CT_ASSERT((is_same<expectd_range_10_0_negative_1, range<10, 0, -1>::type>::value));

	typedef NUMBERLIST_8(1, 3, 5, 7, 9, 11, 13, 15) expected_range_1_16_2;
	CT_ASSERT((is_same<expected_range_1_16_2, range<1, 16, 2>::type>::value));
}