#ifndef FOR_I_H
#define FOR_I_H

#include <util/typelist.h>

namespace detail
{
	template<typename function, unsigned length, unsigned index, bool valid = index < length>
	struct for_i_impl
	{
		typedef typelist<typename function::template func<index>::type,
			typename for_i_impl<function, length, index + 1, index + 1 < length > ::type> type;
	};

	template<typename function, unsigned length, unsigned index>
	struct for_i_impl<function, length, index, false>
	{
		typedef null_t type;
	};
}

// Make a typelist from each index
// function has a member template meta-function called 'func' which takes an ordinary index and returns a type
// example: 
// struct plus_two 
// { template<unsigned index> 
//   struct func 
//   { typedef integral_constant<unsigned, index + 2> type; }; 
// };
template<unsigned length, typename function>
struct for_i : detail::for_i_impl<function, length, 0>
{
};

#endif // ! FOR_I_H