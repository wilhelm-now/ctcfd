#include <util/is_same.h>
#include <util/valuelist.h>
#include <util/ct_assert.h>

#include <util/ones.h>

int main()
{
	CT_ASSERT((is_same<ones<5>::type, NUMBERLIST_5(1, 1, 1, 1, 1)>::value));
}