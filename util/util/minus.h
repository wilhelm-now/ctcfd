#ifndef MINUS_H
#define MINUS_H

#include <util/typelist.h>
#include <util/number.h>

// Gives a new typelist holding difference between values_a and values_b
// input typelists should have same shape
template<typename values_a, typename values_b>
struct minus;

// end of typelist
template<>
struct minus<null_t, null_t>
{
	typedef null_t type;
};

// 1-d iteration
template<number_rep a, typename tail_a, number_rep b, typename tail_b>
struct minus<TAILED_TYPELIST_1(number_c<a>, tail_a), TAILED_TYPELIST_1(number_c<b>, tail_b)>
{
	typedef typelist<number_c<a - b>, typename minus<tail_a, tail_b>::type> type;
};

// 2-d iteration
template<TYPENAMES_4(TA1, TA2, TB1, TB2)>
struct minus<TAILED_TYPELIST_1(TA1, TA2), TAILED_TYPELIST_1(TB1, TB2)>
{
	typedef typelist<typename minus<TA1, TB1>::type, typename minus<TA2, TB2>::type> type;
};


// fast tracks

template<NUMBER_NAMES_4(a1, a2, a3, a4), typename tail_a, NUMBER_NAMES_4(b1, b2, b3, b4), typename tail_b>
struct minus<TAILED_NUMBER_C_LIST_4(a1, a2, a3, a4, tail_a), TAILED_NUMBER_C_LIST_4(b1, b2, b3, b4, tail_b)>
{
private:
	typedef typename minus<tail_a, tail_b>::type tail;
public:
	typedef TAILED_NUMBER_C_LIST_4(a1 - b1, a2 - b2, a3 - b3, a4 - b4, tail) type;
};

template<
	NUMBER_NAMES_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16), typename tail_a,
	NUMBER_NAMES_16(b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16), typename tail_b>
struct minus<
	TAILED_NUMBER_C_LIST_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, tail_a),
	TAILED_NUMBER_C_LIST_16(b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, tail_b)>
{
private:
	typedef typename minus<tail_a, tail_b>::type tail;
public:
	typedef TAILED_NUMBER_C_LIST_16(a1 - b1, a2 - b2, a3 - b3, a4 - b4, a5 - b5, a6 - b6, a7 - b7, a8 - b8, a9 - b9, a10 - b10, a11 - b11, a12 - b12, a13 - b13, a14 - b14, a15 - b15, a16 - b16, tail) type;
};

template<TYPENAMES_5(A1, A2, A3, A4, tail_a), TYPENAMES_5(B1, B2, B3, B4, tail_b)>
class minus<TAILED_TYPELIST_4(A1, A2, A3, A4, tail_a), TAILED_TYPELIST_4(B1, B2, B3, B4, tail_b)>
{
	typedef typename minus<A1, B1>::type d1;
	typedef typename minus<A2, B2>::type d2;
	typedef typename minus<A3, B3>::type d3;
	typedef typename minus<A4, B4>::type d4;
	typedef typename minus<tail_a, tail_b>::type dtail;
public:
	typedef TAILED_TYPELIST_4(d1, d2, d3, d4, dtail) type;
};

template<
	TYPENAMES_17(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, tail_a), 
	TYPENAMES_17(B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, B16, tail_b)>
class minus<
	TAILED_TYPELIST_16(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, tail_a),
	TAILED_TYPELIST_16(B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, B16, tail_b)>
{
	typedef typename minus<A1, B1>::type d1;
	typedef typename minus<A2, B2>::type d2;
	typedef typename minus<A3, B3>::type d3;
	typedef typename minus<A4, B4>::type d4;
	typedef typename minus<A5, B5>::type d5;
	typedef typename minus<A6, B6>::type d6;
	typedef typename minus<A7, B7>::type d7;
	typedef typename minus<A8, B8>::type d8;
	typedef typename minus<A9, B9>::type d9;
	typedef typename minus<A10, B10>::type d10;
	typedef typename minus<A11, B11>::type d11;
	typedef typename minus<A12, B12>::type d12;
	typedef typename minus<A13, B13>::type d13;
	typedef typename minus<A14, B14>::type d14;
	typedef typename minus<A15, B15>::type d15;
	typedef typename minus<A16, B16>::type d16;
	typedef typename minus<tail_a, tail_b>::type dtail;
public:
	typedef TAILED_TYPELIST_16(d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, dtail) type;
};

#endif // !MINUS_H