#ifndef NUMBER_H
#define NUMBER_H

#include <util/typelist.h>

#include <ostream>

#define NUMBER_SCALE (0x1000000ll) // 24 bits of decimals

typedef long long number_rep;

// Saves raw number in fixed point format
template<number_rep number>
struct number_c
{
	enum {raw_value = number};

	double get() const { return raw_value * 1.0 / NUMBER_SCALE; }
	
};

template<number_rep raw>
std::ostream& operator<<(std::ostream& os, number_c<raw> number)
{
	return os << number.get();
}


// save a number in type
#define NUMBER_MAKE(numb) number_c<static_cast<number_rep>((numb) * NUMBER_SCALE)>
// compile time get a number from the value
#define NUMBER_GET_RAW(raw_value) (raw_value * 1.0 / NUMBER_SCALE)
// compile time get a number from the type
#define NUMBER_GET_TYPE(typed_number) NUMBER_GET_RAW(typed_number::raw_value)



// helper to implement fast-tracks specializing number_c
#define NUMBER_NAMES_1(x1) number_rep x1
#define NUMBER_NAMES_2(x1, x2) number_rep x1, NUMBER_NAMES_1(x2)
#define NUMBER_NAMES_3(x1, x2, x3) number_rep x1, NUMBER_NAMES_2(x2, x3)
#define NUMBER_NAMES_4(x1, x2, x3, x4) number_rep x1, NUMBER_NAMES_3(x2, x3, x4)
#define NUMBER_NAMES_5(x1, x2, x3, x4, x5) number_rep x1, NUMBER_NAMES_4(x2, x3, x4, x5)
#define NUMBER_NAMES_6(x1, x2, x3, x4, x5, x6) number_rep x1, NUMBER_NAMES_5(x2, x3, x4, x5, x6)
#define NUMBER_NAMES_7(x1, x2, x3, x4, x5, x6, x7) number_rep x1, NUMBER_NAMES_6(x2, x3, x4, x5, x6, x7)
#define NUMBER_NAMES_8(x1, x2, x3, x4, x5, x6, x7, x8) number_rep x1, NUMBER_NAMES_7(x2, x3, x4, x5, x6, x7, x8)
#define NUMBER_NAMES_9(x1, x2, x3, x4, x5, x6, x7, x8, x9) number_rep x1, NUMBER_NAMES_8(x2, x3, x4, x5, x6, x7, x8, x9)
#define NUMBER_NAMES_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) number_rep x1, NUMBER_NAMES_9(x2, x3, x4, x5, x6, x7, x8, x9, x10)
#define NUMBER_NAMES_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) number_rep x1, NUMBER_NAMES_10(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)
#define NUMBER_NAMES_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) number_rep x1, NUMBER_NAMES_11(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)
#define NUMBER_NAMES_13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) number_rep x1, NUMBER_NAMES_12(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13)
#define NUMBER_NAMES_14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) number_rep x1, NUMBER_NAMES_13(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14)
#define NUMBER_NAMES_15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) number_rep x1, NUMBER_NAMES_14(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15)
#define NUMBER_NAMES_16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) number_rep x1, NUMBER_NAMES_15(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16)

#define TAILED_NUMBER_C_LIST_1(x1, tail) typelist<number_c<x1>, tail>
#define TAILED_NUMBER_C_LIST_2(x1, x2, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_1(x2, tail) >
#define TAILED_NUMBER_C_LIST_3(x1, x2, x3, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_2(x2, x3, tail) >
#define TAILED_NUMBER_C_LIST_4(x1, x2, x3, x4, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_3(x2, x3, x4, tail) >
#define TAILED_NUMBER_C_LIST_5(x1, x2, x3, x4, x5, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_4(x2, x3, x4, x5, tail) >
#define TAILED_NUMBER_C_LIST_6(x1, x2, x3, x4, x5, x6, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_5(x2, x3, x4, x5, x6, tail) >
#define TAILED_NUMBER_C_LIST_7(x1, x2, x3, x4, x5, x6, x7, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_6(x2, x3, x4, x5, x6, x7, tail) >
#define TAILED_NUMBER_C_LIST_8(x1, x2, x3, x4, x5, x6, x7, x8, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_7(x2, x3, x4, x5, x6, x7, x8, tail) >
#define TAILED_NUMBER_C_LIST_9(x1, x2, x3, x4, x5, x6, x7, x8, x9, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_8(x2, x3, x4, x5, x6, x7, x8, x9, tail) >
#define TAILED_NUMBER_C_LIST_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_9(x2, x3, x4, x5, x6, x7, x8, x9, x10, tail) >
#define TAILED_NUMBER_C_LIST_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_10(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, tail) >
#define TAILED_NUMBER_C_LIST_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_11(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, tail) >
#define TAILED_NUMBER_C_LIST_13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_12(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, tail) >
#define TAILED_NUMBER_C_LIST_14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_13(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, tail) >
#define TAILED_NUMBER_C_LIST_15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_14(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, tail) >
#define TAILED_NUMBER_C_LIST_16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, tail) typelist<number_c<x1>, TAILED_NUMBER_C_LIST_15(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, tail) >

#endif // !NUMBER_H
