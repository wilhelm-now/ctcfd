#include <util/diff.h>
#include <util/range.h>
#include <util/is_same.h>

#include <util/ct_assert.h>

#include <util/valuelist.h>

int main()
{
	typedef range<0, 10, 2>::type values;
	typedef diff<values>::type diffed;

	typedef NUMBERLIST_4(2, 2, 2, 2) expected_diff;

	CT_ASSERT((is_same<expected_diff, diffed>::value));

	typedef NUMBERLIST_3(0, 0, 0) expected_second_diff;
	CT_ASSERT((is_same<expected_second_diff, second_diff<values>::type>::value));
}