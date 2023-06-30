#ifndef DIFF_H
#define DIFF_H

#include <util/typelist.h>

// first derivate ignoring grid size: u[i+1] - u[i]
template<typename valuelist>
struct diff;

template<typename T>
struct diff<typelist<T, null_t> >
{
	typedef null_t type;
};

template<typename head, typename tail>
class diff<typelist<head, tail> >
{
	enum { delta = tail::head::raw_value - head::raw_value };
public:
	typedef typelist<number_c<delta>, typename diff<tail>::type> type;
};

// Second derivate ignoring grid size: u[i+1] - 2*u[i] + u[i]
template<typename valuelist>
struct second_diff;

template<typename prev, typename current>
struct second_diff<typelist<prev, typelist<current, null_t>> >
{
	typedef null_t type;
};

template<typename prev, typename current, typename next>
class second_diff<typelist<prev, typelist<current, next> > >
{
	enum { ddelta = next::head::raw_value - 2 * current::raw_value + prev::raw_value };
public:
	typedef typelist<number_c<ddelta>, typename second_diff<typename typelist<current, next> >::type> type;
};

#endif // !DIFF_H
