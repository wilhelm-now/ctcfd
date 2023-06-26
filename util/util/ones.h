#ifndef ONES_H
#define ONES_H

#include <util/typelist.h>
#include <util/number.h>


template<unsigned length>
struct ones;

template<>
struct ones<1>
{
	typedef typelist<number_t<1>, null_t> type;
};

template<unsigned length>
struct ones
{
	typedef typelist<number_t<1>, typename ones<length - 1>::type > type;
};

#endif // !ONES_H