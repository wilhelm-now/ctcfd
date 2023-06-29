#ifndef DIFF_H
#define DIFF_H

#include <util/typelist.h>

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
	enum value { delta = tail::head::raw_value - head::raw_value };
public:
	typedef typelist<number_c<delta>, typename diff<tail>::type> type;
};

#endif // !DIFF_H
