#ifndef FOR_I_H
#define FOR_I_H

#include <util/typelist.h>
#include <util/conditional.h>

namespace detail
{
	template<template<unsigned> class function, unsigned length, unsigned index, bool valid = index < length>
	struct for_i_impl
	{
		typedef typename typelist<typename function<index>::type,
			typename for_i_impl<function, length, index + 1, index + 1 < length > ::type> type;
	};

	template<template<unsigned> class function, unsigned length, unsigned index>
	struct for_i_impl<function, length, index, false>
	{
		typedef null_t type;
	};
}

// Make a typelist from each index
// function takes an ordinary index and returns a type
template<unsigned length, template<unsigned> class function>
struct for_i : detail::for_i_impl<function, length, 0>
{
};

#endif // ! FOR_I_H