#ifndef FOR_I_H
#define FOR_I_H

#include <util/typelist.h>

template<unsigned length, typename func>
struct for_i;

template<unsigned length_x, unsigned length_y, typename func>
struct for_ij;

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

	template<unsigned first_arg, class two_arg_function>
	struct bind_first
	{
		template<unsigned second_arg>
		struct func
		{
			typedef typename two_arg_function::template func<first_arg, second_arg>::type type;
		};
	};

	template<class function, unsigned length_i, unsigned length_j,
		unsigned index_i, bool valid = index_i < length_i>
	struct for_ij_impl
	{
		typedef typelist<
			typename for_i<length_j, bind_first<index_i, function> >::type,
			typename for_ij_impl<function, length_i, length_j, index_i + 1, index_i + 1 < length_i>::type> type;
	};

	template<class function, unsigned length_i, unsigned length_j,
		unsigned index_i>
	struct for_ij_impl<function, length_i, length_j, index_i, false>
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

// Like for_i but supplied meta-function takes two indices instead of one.
template<unsigned length_i, unsigned length_j, typename function>
struct for_ij : detail::for_ij_impl<function, length_i, length_j, 0>
{
};


#endif // ! FOR_I_H