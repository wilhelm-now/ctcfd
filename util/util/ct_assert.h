#ifndef CT_ASSERT_H
#define CT_ASSERT_H

namespace detail
{
	template<bool> struct ct_error;
	template<> struct ct_error<true> {};
}

#define CT_ASSERT(condition) (detail::ct_error<((condition)) != 0>())

#endif // !CT_ASSERT_H