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
#define NUMBERLIST_7(V1, V2, V3, V4, V5, V6, V7) typelist<NUMBER_MAKE(V1), NUMBERLIST_6(V2, V3, V4, V5, V6, V7)>
#define NUMBERLIST_8(V1, V2, V3, V4, V5, V6, V7, V8) typelist<NUMBER_MAKE(V1), NUMBERLIST_7(V2, V3, V4, V5, V6, V7, V8)>
#define NUMBERLIST_9(V1, V2, V3, V4, V5, V6, V7, V8, V9) typelist<NUMBER_MAKE(V1), NUMBERLIST_8(V2, V3, V4, V5, V6, V7, V8, V9)>
#define NUMBERLIST_10(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10) typelist<NUMBER_MAKE(V1), NUMBERLIST_9(V2, V3, V4, V5, V6, V7, V8, V9, V10)>
#define NUMBERLIST_11(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11) typelist<NUMBER_MAKE(V1), NUMBERLIST_10(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11)>
#define NUMBERLIST_12(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12) typelist<NUMBER_MAKE(V1), NUMBERLIST_11(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12)>
#define NUMBERLIST_13(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13) typelist<NUMBER_MAKE(V1), NUMBERLIST_12(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13)>
#define NUMBERLIST_14(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14) typelist<NUMBER_MAKE(V1), NUMBERLIST_13(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14)>
#define NUMBERLIST_15(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15) typelist<NUMBER_MAKE(V1), NUMBERLIST_14(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15)>
#define NUMBERLIST_16(V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16) typelist<NUMBER_MAKE(V1), NUMBERLIST_15(V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16)>


template<typename values>
struct value_printer;

template<typename T, typename U>
struct value_printer<typelist<T, U> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T() << ", " << value_printer<U>();
	}
};

template<typename T>
struct value_printer<typelist<T, null_t> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T();
	}
};

template<TYPENAMES_6(T1, T2, T3, T4, head, tail)>
struct value_printer<TAILED_TYPELIST_5(T1, T2, T3, T4, head, tail)>
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T1() << ", " << T2() << ", " << T3() << ", " << T4() << ", " << value_printer<TAILED_TYPELIST_1(head, tail)>();
	}
};

template<TYPENAMES_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, head, tail)>
struct value_printer<TAILED_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, head, tail)>
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		os << T1() << ", " << T2() << ", " << T3() << ", " << T4() << ", " << T5() << ", " << T6() << ", " << T7() << ", " << T8() << ", " << T9() << ", " << T10() << ", " << T11() << ", " << T12() << ", " << T13() << ", " << T14() << ", " << T15() << ", " << T16();
		return os << ", " << value_printer<TAILED_TYPELIST_1(head, tail)>();
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

	template<TYPENAMES_6(T1, T2, T3, T4, head, tail)>
	struct value_printer2d_impl<TAILED_TYPELIST_5(T1, T2, T3, T4, head, tail)>
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<T1>() << "],\n[" << value_printer<T2>() << "],\n[" << value_printer<T3>() << "],\n[" << value_printer<T4>() << "],\n" << value_printer2d_impl<TAILED_TYPELIST_1(head, tail)>();
		}
	};

	template<TYPENAMES_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, head, tail)>
	struct value_printer2d_impl<TAILED_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, head, tail)>
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<T1>() << "],\n[" << value_printer<T2>() << "],\n[" << value_printer<T3>() << "],\n[" << value_printer<T4>()
				<< "],\n[" << value_printer<T5>() << "],\n[" << value_printer<T6>() << "],\n[" << value_printer<T7>() << "],\n[" << value_printer<T8>()
				<< "],\n[" << value_printer<T9>() << "],\n[" << value_printer<T10>() << "],\n[" << value_printer<T11>() << "],\n[" << value_printer<T12>()
				<< "],\n[" << value_printer<T13>() << "],\n[" << value_printer<T14>() << "],\n[" << value_printer<T15>() << "],\n[" << value_printer<T16>()
				<< "],\n" << value_printer2d_impl<TAILED_TYPELIST_1(head, tail)>();
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
