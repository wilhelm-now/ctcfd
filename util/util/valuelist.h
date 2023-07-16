#ifndef VALUELIST_H
#define VALUELIST_H

// utility functions for typelists of values
#include <util/typelist.h>
#include <util/number.h>

#include <ostream>

#define NUMBERLIST_1(V1) typelist<NUMBER_MAKE(V1), null_t>
#define NUMBERLIST_2(V1, V2) typelist<NUMBER_MAKE(V1), NUMBERLIST_1(V2)>
#define NUMBERLIST_3(V1, V2, V3) typelist<NUMBER_MAKE(V1), NUMBERLIST_2(V2, V3)>
#define NUMBERLIST_4(V1, V2, V3, V4) typelist<NUMBER_MAKE(V1), NUMBERLIST_3(V2, V3, V4)>
#define NUMBERLIST_5(V1, V2, V3, V4, V5) typelist<NUMBER_MAKE(V1), NUMBERLIST_4(V2, V3, V4, V5)>
#define NUMBERLIST_6(V1, V2, V3, V4, V5, V6) typelist<NUMBER_MAKE(V1), NUMBERLIST_5(V2, V3, V4, V5, V6)>
#define NUMBERLIST_7(V1, V2, V3, V4, V5, V6, V7) typelist<NUMBER_MAKE(V1), NUMBERLIST_6(V1, V2, V3, V4, V5, V6, V7)>
#define NUMBERLIST_8(V1, V2, V3, V4, V5, V6, V7, V8) typelist<NUMBER_MAKE(V1), NUMBERLIST_7(V1, V2, V3, V4, V5, V6, V7, V8)>
#define NUMBERLIST_9(V1, V2, V3, V4, V5, V6, V7, V8, V9) typelist<NUMBER_MAKE(V1), NUMBERLIST_8(V1, V2, V3, V4, V5, V6, V7, V8, V9)>
#define NUMBERLIST_10(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10) typelist<NUMBER_MAKE(V1), NUMBERLIST_9(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10)>
#define NUMBERLIST_11(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11) typelist<NUMBER_MAKE(V1), NUMBERLIST_10(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11)>
#define NUMBERLIST_12(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12) typelist<NUMBER_MAKE(V1), NUMBERLIST_11(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12)>
#define NUMBERLIST_13(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13) typelist<NUMBER_MAKE(V1), NUMBERLIST_12(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13)>
#define NUMBERLIST_14(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14) typelist<NUMBER_MAKE(V1), NUMBERLIST_13(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14)>
#define NUMBERLIST_15(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15) typelist<NUMBER_MAKE(V1), NUMBERLIST_14(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15)>
#define NUMBERLIST_16(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16) typelist<NUMBER_MAKE(V1), NUMBERLIST_15(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16)>


template<typename values>
struct value_printer;

template<typename T, typename U>
struct value_printer<typelist<T, U> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get() << ", " << value_printer<U>();
	}
};

template<typename T>
struct value_printer<typelist<T, null_t> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get();
	}
};

namespace detail
{
	template<typename values>
	struct value_printer2d_impl;

	template<typename Head, typename Tail>
	struct value_printer2d_impl<typelist<Head, Tail> >
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<Head>() << "],\n" 
				<< value_printer2d_impl<Tail>();
		}
	};

	template<typename Head>
	struct value_printer2d_impl<typelist<Head, null_t> >
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<Head>() << ']';
		}
	};
}

template<typename values>
struct value_printer2d
{
	friend std::ostream& operator<<(std::ostream& os, value_printer2d)
	{
		return os << '[' << detail::value_printer2d_impl<values>() << ']';
	}
};


template<typename values>
std::ostream& print_values(std::ostream& os)
{
	return os << value_printer<values>();
}

#endif // !VALUELIST_H
