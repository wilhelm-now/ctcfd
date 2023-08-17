#ifndef ONES_H
#define ONES_H

#include <util/for_i.h>
#include <util/number.h>

namespace detail
{
	struct one_func
	{
		template<unsigned>
		struct func
		{
			typedef NUMBER_MAKE(1.0) type;
		};
	};
}


template<unsigned length>
struct ones;
{
	typedef for_i<length, detail::one_func>::type type;
};

#endif // !ONES_H