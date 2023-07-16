#include <util/is_same.h>

#include <util/ct_assert.h>

int main()
{
	CT_ASSERT((is_same<int, int>::value));
	CT_ASSERT(!(is_same<int, float>::value));
}