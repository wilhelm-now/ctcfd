#ifndef FOR_I_H
#define FOR_I_H

#include <util/typelist.h>

template<unsigned length, typename func>
struct for_i;

template<unsigned length_x, unsigned length_y, typename func>
struct for_ij;

namespace detail
{
	// A for_i_track is an actual (fast) implementation of some loops
	template<unsigned>
	struct for_i_track
	{};

	template<typename function, unsigned length, unsigned index>
	class for_i_impl
	{
		enum {
			remaining = length - index
		};
		enum {
			track = remaining > 32 ? 4 : remaining > 16 ? 3 : remaining > 8 ? 2 : remaining >= 1 ? 1 : 0
		};
	public:
		typedef typename for_i_track<track>::template func<function, length, index>::type type;
	};

	template<>
	struct for_i_track<0>
	{
		template<typename function, unsigned length, unsigned index>
		struct func
		{
			typedef null_t type;
		};
	};

	template<>
	struct for_i_track<1>
	{
		template<typename function, unsigned length, unsigned index>
		struct func
		{
			typedef typelist<typename function::template func<index>::type,
				typename for_i_impl<function, length, index + 1>::type> type;
		};
	};

	// first fast-track of for-i loop.
	template<>
	struct for_i_track<2>
	{
		template<typename function, unsigned length, unsigned index>
		struct func
		{
			typedef
				typelist<typename function::template func<index + 0>::type,
				typelist<typename function::template func<index + 1>::type,
				typelist<typename function::template func<index + 2>::type,
				typelist<typename function::template func<index + 3>::type,
				typelist<typename function::template func<index + 4>::type,
				typelist<typename function::template func<index + 5>::type,
				typelist<typename function::template func<index + 6>::type,
				typelist<typename function::template func<index + 7>::type,
				typename for_i_impl<function, length, index + 8>::type
				> > > > > > > > type;
		};
	};

	template<>
	struct for_i_track<3>
	{
		template<typename function, unsigned length, unsigned index>
		struct func
		{
			typedef
				typelist<typename function::template func<index + 0>::type,
				typelist<typename function::template func<index + 1>::type,
				typelist<typename function::template func<index + 2>::type,
				typelist<typename function::template func<index + 3>::type,
				typelist<typename function::template func<index + 4>::type,
				typelist<typename function::template func<index + 5>::type,
				typelist<typename function::template func<index + 6>::type,
				typelist<typename function::template func<index + 7>::type,
				typelist<typename function::template func<index + 8>::type,
				typelist<typename function::template func<index + 9>::type,
				typelist<typename function::template func<index + 10>::type,
				typelist<typename function::template func<index + 11>::type,
				typelist<typename function::template func<index + 12>::type,
				typelist<typename function::template func<index + 13>::type,
				typelist<typename function::template func<index + 14>::type,
				typelist<typename function::template func<index + 15>::type,
				typename for_i_impl<function, length, index + 16>::type
				> > > > > > > > > > > > > > > > type;
		};
	};

	template<>
	struct for_i_track<4>
	{
		template<typename function, unsigned length, unsigned index>
		struct func
		{
			typedef
				typelist<typename function::template func<index + 0>::type,
				typelist<typename function::template func<index + 1>::type,
				typelist<typename function::template func<index + 2>::type,
				typelist<typename function::template func<index + 3>::type,
				typelist<typename function::template func<index + 4>::type,
				typelist<typename function::template func<index + 5>::type,
				typelist<typename function::template func<index + 6>::type,
				typelist<typename function::template func<index + 7>::type,
				typelist<typename function::template func<index + 8>::type,
				typelist<typename function::template func<index + 9>::type,
				typelist<typename function::template func<index + 10>::type,
				typelist<typename function::template func<index + 11>::type,
				typelist<typename function::template func<index + 12>::type,
				typelist<typename function::template func<index + 13>::type,
				typelist<typename function::template func<index + 14>::type,
				typelist<typename function::template func<index + 15>::type,
				typelist<typename function::template func<index + 16>::type,
				typelist<typename function::template func<index + 17>::type,
				typelist<typename function::template func<index + 18>::type,
				typelist<typename function::template func<index + 19>::type,
				typelist<typename function::template func<index + 20>::type,
				typelist<typename function::template func<index + 21>::type,
				typelist<typename function::template func<index + 22>::type,
				typelist<typename function::template func<index + 23>::type,
				typelist<typename function::template func<index + 24>::type,
				typelist<typename function::template func<index + 25>::type,
				typelist<typename function::template func<index + 26>::type,
				typelist<typename function::template func<index + 27>::type,
				typelist<typename function::template func<index + 28>::type,
				typelist<typename function::template func<index + 29>::type,
				typelist<typename function::template func<index + 30>::type,
				typelist<typename function::template func<index + 31>::type,
				typename for_i_impl<function, length, index + 32>::type
				> > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
		};
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