#include <util/for_i.h>
#include <util/number.h>


namespace detail
{
	struct zero_func
	{
		template<unsigned>
		struct func
		{
			typedef NUMBER_MAKE(0) type;
		};
	};

	struct zero2d_func
	{
		template<unsigned, unsigned>
		struct func
		{
			typedef NUMBER_MAKE(0) type;
		};
	};
}

template<unsigned length>
struct zeros
{
	typedef typename for_i<length, detail::zero_func>::type type;
};

template<unsigned length_i, unsigned length_j>
struct zeros2d
{
	typedef typename for_ij<length_i, length_j, detail::zero2d_func>::type type;
};