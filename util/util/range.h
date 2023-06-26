#ifndef RANGE_H
#define RANGE_H

#include <util/typelist.h>
#include <util/number.h>
#include <util/conditional.h>

namespace detail
{
	// default bool value top avoid having to specify when invoking
	template<int start, int stop, int step, bool valid = (start + step < stop)>
	struct range_impl
	{
		typedef typelist<number_t<start>,
			typename range_impl<start + step, stop, step, (start + step < stop)>::type >
			type;
	};

	template<int start, int stop, int step>
	struct range_impl<start, stop, step, false>
	{
		typedef null_t type;
	};
}

template<int start, int stop, int step>
struct range 
{
	typedef typename detail::range_impl<start, stop, step>::type type;
};

#endif // !RANGE_H