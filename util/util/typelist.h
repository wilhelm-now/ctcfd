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

	template<typename tlist, typename T>
	struct append;

	template<typename head, typename T>
	struct append<typelist<head, null_t>, T>
	{
		typedef typename TYPELIST_2(head, T) type;
	};

	template<typename head, typename tail, typename T>
	struct append<typelist<head, tail>, T>
	{
		typedef typename typelist<head, typename append<tail, T>::type> type;
	};
}

#endif // ! TYPELIST_H