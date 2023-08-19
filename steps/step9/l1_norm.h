#ifndef L1_NORM_H
#define L1_NORM_H

#include <util/typelist.h>
#include <util/number.h>

#define CTCFD_ABS(x) ((x) >= 0 ? (x) : -(x))

template<typename values>
struct l1_norm;

template<>
struct l1_norm<null_t>
{
	enum {value = 0};
};

template<long long num, typename tail>
struct l1_norm<typelist<number_c<num>, tail> >
{
	enum { value = CTCFD_ABS(num) + l1_norm<tail>::value };
};

template<long long num, typename tail>
struct l1_norm<typelist<number_c<num>, tail> >
{
	enum { value = CTCFD_ABS(num) + l1_norm<tail>::value };
};

template<long long x1, long long x2, long long x3, long long x4, long long x5, long long x6, long long x7, long long x8, typename tail>
struct l1_norm<TAILED_TYPELIST_8(number_c<x1>, number_c<x2>, number_c<x3>, number_c<x4>, number_c<x5>, number_c<x6>, number_c<x7>, number_c<x8>, tail)>
{
	enum { value = CTCFD_ABS(x1) + CTCFD_ABS(x2) + CTCFD_ABS(x3) + CTCFD_ABS(x4) + CTCFD_ABS(x5) + CTCFD_ABS(x6) + CTCFD_ABS(x7) + CTCFD_ABS(x8) + l1_norm<tail>::value };
};


template<typename current_row, typename next_rows>
struct l1_norm<typelist<current_row, next_rows> >
{
	enum { value = l1_norm<current_row>::value + l1_norm<next_rows>::value };
};

template<typename values_current, typename values_prev>
struct l1_norm_convergence : NUMBER_MAKE((NUMBER_GET_RAW(l1_norm<values_current>::value) - NUMBER_GET_RAW(l1_norm<values_prev>::value)) / NUMBER_GET_RAW(l1_norm<values_prev>::value))
{
};

#endif // ! L1_NORM_H
