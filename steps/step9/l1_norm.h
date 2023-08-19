#ifndef L1_NORM_H
#define L1_NORM_H

#include <util/typelist.h>
#include <util/number.h>

#define CTCFD_ABS(x) ((x) >= 0 ? (x) : -(x))

template<typename values>
struct l1_norm;

// base case
template<>
struct l1_norm<null_t>
{
	enum {value = 0};
};

// iteration over a row
template<number_rep num, typename tail>
struct l1_norm<typelist<number_c<num>, tail> >
{
	enum { value = CTCFD_ABS(num) + l1_norm<tail>::value };
};

// iteration over rows
template<typename current_row, typename next_rows>
struct l1_norm<typelist<current_row, next_rows> >
{
	enum { value = l1_norm<current_row>::value + l1_norm<next_rows>::value };
};


template<typename values_current, typename values_prev>
struct l1_norm_convergence : NUMBER_MAKE(CTCFD_ABS((NUMBER_GET_RAW(l1_norm<values_current>::value) - NUMBER_GET_RAW(l1_norm<values_prev>::value)) / NUMBER_GET_RAW(l1_norm<values_prev>::value)))
{
};

// Fast tracks for a single row below

template<number_rep x1, number_rep x2, number_rep x3, number_rep x4, typename tail>
struct l1_norm<TAILED_TYPELIST_4(number_c<x1>, number_c<x2>, number_c<x3>, number_c<x4>, tail)>
{
	enum { value = CTCFD_ABS(x1) + CTCFD_ABS(x2) + CTCFD_ABS(x3) + CTCFD_ABS(x4) + l1_norm<tail>::value };
};


template<number_rep x1, number_rep x2, number_rep x3, number_rep x4, number_rep x5, number_rep x6, number_rep x7, number_rep x8, typename tail>
struct l1_norm<TAILED_TYPELIST_8(number_c<x1>, number_c<x2>, number_c<x3>, number_c<x4>, number_c<x5>, number_c<x6>, number_c<x7>, number_c<x8>, tail)>
{
	enum { value = CTCFD_ABS(x1) + CTCFD_ABS(x2) + CTCFD_ABS(x3) + CTCFD_ABS(x4) + CTCFD_ABS(x5) + CTCFD_ABS(x6) + CTCFD_ABS(x7) + CTCFD_ABS(x8) + l1_norm<tail>::value };
};


template<number_rep x1, number_rep x2, number_rep x3, number_rep x4, number_rep x5, number_rep x6, number_rep x7, number_rep x8,
	number_rep x9, number_rep x10, number_rep x11, number_rep x12, number_rep x13, number_rep x14, number_rep x15, number_rep x16, typename tail>
struct l1_norm<TAILED_TYPELIST_16(number_c<x1>, number_c<x2>, number_c<x3>, number_c<x4>, number_c<x5>, number_c<x6>, number_c<x7>, number_c<x8>, number_c<x9>, number_c<x10>, number_c<x11>, number_c<x12>, number_c<x13>, number_c<x14>, number_c<x15>, number_c<x16>, tail)>
{
	enum { value = 
		CTCFD_ABS(x1) + CTCFD_ABS(x2) + CTCFD_ABS(x3) + CTCFD_ABS(x4) + CTCFD_ABS(x5) + CTCFD_ABS(x6) + CTCFD_ABS(x7) + 
		CTCFD_ABS(x8) + CTCFD_ABS(x9) + CTCFD_ABS(x10) + CTCFD_ABS(x11) + CTCFD_ABS(x12) + CTCFD_ABS(x13) + CTCFD_ABS(x14) 
		+ CTCFD_ABS(x15) + CTCFD_ABS(x16) + l1_norm<tail>::value };
};

// Fast tracks for multiple rows

template<TYPENAMES_9(T1A, T1B, T2A, T2B, T3A, T3B, T4A, T4B, tail)>
struct l1_norm<TAILED_TYPELIST_4(TAILED_TYPELIST_1(T1A, T1B), TAILED_TYPELIST_1(T2A, T2B), TAILED_TYPELIST_1(T3A, T3B), TAILED_TYPELIST_1(T4A, T4B), tail)>
{
	enum {value = 
		l1_norm<typelist<T1A, T1B> >::value + l1_norm<typelist<T2A, T2B> >::value
		+ l1_norm<typelist<T3A, T3B> >::value + l1_norm<typelist<T4A, T4B> >::value
		+ l1_norm<tail>::value};
};

#endif // ! L1_NORM_H
