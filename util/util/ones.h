#ifndef ONES_H
#define ONES_H

#include <util/for_i.h>
#include <util/number.h>

namespace detail
{
	struct one_func
	{
		template<unsigned = 0, unsigned = 0>
		struct func
		{
			typedef NUMBER_MAKE(1.0) type;
		};
	};
}


template<unsigned length>
struct ones
{
	typedef typename for_i<length, detail::one_func>::type type;
};

template<unsigned length_x, unsigned length_y>
struct ones2d
{
	typedef typename for_ij<length_x, length_y, detail::one_func>::type type;
};

#endif // !ONES_H