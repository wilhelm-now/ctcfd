#ifndef  TYPELIST_H
#define TYPELIST_H

// Simple definition of a typelist, from Modern C++ Design.

// end of typelist
class null_t {};

template<typename T, typename U>
struct typelist
{
	typedef T head;
	typedef U tail;
};

#define TYPELIST_1(T1) typelist<T1, null_t >
#define TYPELIST_2(T1, T2) typelist<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3) typelist<T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4) typelist<T1, TYPELIST_3(T2, T3, T4) >
#define TYPELIST_5(T1, T2, T3, T4, T5) typelist<T1, TYPELIST_4(T2, T3, T4, T5) >
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >
#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) typelist<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >
#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) typelist<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >
#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) typelist<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >
#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) typelist<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >
#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) typelist<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >
#define TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) typelist<T1, TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) >

#define TAILED_TYPELIST_1(T1, Tail) typelist<T1, Tail>
#define TAILED_TYPELIST_2(T1, T2, Tail) typelist<T1, TAILED_TYPELIST_1(T2, Tail) >
#define TAILED_TYPELIST_3(T1, T2, T3, Tail) typelist<T1, TAILED_TYPELIST_2(T2, T3, Tail) >
#define TAILED_TYPELIST_4(T1, T2, T3, T4, Tail) typelist<T1, TAILED_TYPELIST_3(T2, T3, T4, Tail) >
#define TAILED_TYPELIST_5(T1, T2, T3, T4, T5, Tail) typelist<T1, TAILED_TYPELIST_4(T2, T3, T4, T5, Tail) >
#define TAILED_TYPELIST_6(T1, T2, T3, T4, T5, T6, Tail) typelist<T1, TAILED_TYPELIST_5(T2, T3, T4, T5, T6, Tail) >
#define TAILED_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7, Tail) typelist<T1, TAILED_TYPELIST_6(T2, T3, T4, T5, T6, T7, Tail) >
#define TAILED_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8, Tail) typelist<T1, TAILED_TYPELIST_7(T2, T3, T4, T5, T6, T7, T8, Tail) >
#define TAILED_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9, Tail) typelist<T1, TAILED_TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9, Tail) >
#define TAILED_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, Tail) typelist<T1, TAILED_TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10, Tail) >
#define TAILED_TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, Tail) typelist<T1, TAILED_TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, Tail) >
#define TAILED_TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, Tail) typelist<T1, TAILED_TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, Tail) >
#define TAILED_TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, Tail) typelist<T1, TAILED_TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, Tail) >
#define TAILED_TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, Tail) typelist<T1, TAILED_TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, Tail) >
#define TAILED_TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, Tail) typelist<T1, TAILED_TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, Tail) >
#define TAILED_TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, Tail) typelist<T1, TAILED_TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, Tail) >
#define TAILED_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, Tail) typelist<T1, TAILED_TYPELIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, Tail) >
#define TAILED_TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, Tail) typelist<T1, TAILED_TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, Tail) >

#define TYPENAMES_1(T1) typename T1
#define TYPENAMES_2(T1, T2) typename T1, TYPENAMES_1(T2)
#define TYPENAMES_3(T1, T2, T3) typename T1, TYPENAMES_2(T2, T3)
#define TYPENAMES_4(T1, T2, T3, T4) typename T1, TYPENAMES_3(T2, T3, T4)
#define TYPENAMES_5(T1, T2, T3, T4, T5) typename T1, TYPENAMES_4(T2, T3, T4, T5)
#define TYPENAMES_6(T1, T2, T3, T4, T5, T6) typename T1, TYPENAMES_5(T2, T3, T4, T5, T6)
#define TYPENAMES_7(T1, T2, T3, T4, T5, T6, T7) typename T1, TYPENAMES_6(T2, T3, T4, T5, T6, T7)
#define TYPENAMES_8(T1, T2, T3, T4, T5, T6, T7, T8) typename T1, TYPENAMES_7(T2, T3, T4, T5, T6, T7, T8)
#define TYPENAMES_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) typename T1, TYPENAMES_8(T2, T3, T4, T5, T6, T7, T8, T9)
#define TYPENAMES_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) typename T1, TYPENAMES_9(T2, T3, T4, T5, T6, T7, T8, T9, T10)
#define TYPENAMES_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) typename T1, TYPENAMES_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)
#define TYPENAMES_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) typename T1, TYPENAMES_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)
#define TYPENAMES_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) typename T1, TYPENAMES_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)
#define TYPENAMES_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) typename T1, TYPENAMES_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)
#define TYPENAMES_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) typename T1, TYPENAMES_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)
#define TYPENAMES_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) typename T1, TYPENAMES_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)
#define TYPENAMES_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) typename T1, TYPENAMES_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)
#define TYPENAMES_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) typename T1, TYPENAMES_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)
#define TYPENAMES_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) typename T1, TYPENAMES_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)


namespace tl
{
	// Indexed access
	template<typename tlist, unsigned index>
	struct type_at;

	template<typename head, typename tail>
	struct type_at<typelist<head, tail>, 0>
	{
		typedef head type;
	};

	template<typename head, typename tail, unsigned index>
	struct type_at<typelist<head, tail>, index>
	{
		typedef typename type_at<tail, index - 1>::type type;
	};

	// length
	template<typename tlist>
	struct length;

	template<>
	struct length<null_t>
	{
		enum { value = 0 };
	};

	template<typename head, typename tail>
	struct length<typelist<head, tail> >
	{
		enum {value = 1 + length<tail>::value};
	};

	// Attempted fast-track overload, never checked if it's useful.
	template<typename T1, typename T2, typename T3, typename T4, typename tail>
	struct length<typelist<T1, typelist<T2, typelist<T3, typelist<T4, tail> > > > >
	{
		enum {value = 4 + length<tail>::value};
	};

	template<TYPENAMES_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, tail)>
	struct length<TAILED_TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, tail)>
	{
		enum {value = 16 + length<tail>::value};
	};

	template<typename tlist, typename T>
	struct append;

	template<typename head, typename T>
	struct append<typelist<head, null_t>, T>
	{
		typedef TYPELIST_2(head, T) type;
	};

	template<typename head, typename tail, typename T>
	struct append<typelist<head, tail>, T>
	{
		typedef typelist<head, typename append<tail, T>::type> type;
	};
}

#endif // ! TYPELIST_H