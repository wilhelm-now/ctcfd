#include "step6_params.h"

#include <util/typelist.h>
#include <util/number.h>
#include <util/point2d.h>

#include <util/for_i.h>
#include <util/conditional.h>

#include <util/ones.h>

#include <util/valuelist.h>

#include <iostream>


// Setting up initial conditions
// u = 1.0 everywhere except where 0.5 <= (x, y) <= 1.0
struct initial
{
	template<unsigned index_i, unsigned index_j>
	class func
	{
		typedef NUMBER_MAKE(
			(0.5 <= DX * index_i && DX * index_i <= 1.0) && (0.5 <= DY * index_j && DY * index_j <= 1.0)
			? 2.0 : 1.0)
			number;
	public:
		typedef point2d_c<number, number> type;
	};
};

// for just a point
template<
	typename previous_x, // u[t, i-1, j]
	typename previous_y, // u[t, i,   j-1]
	typename current     // u[t, i,   j]
>
struct compute_convection_point
{
	// u[t+1, i, j] = u[t, i, j] - u[t, i, j]*(DT/DX)*(u[t, i, j] - u[t, i-1, j]) - v[t, i, j]*(DT/DY)*(u[t, i, j] - u[t, i, j-1])
	// v[t+1, i, j] = v[t, i, j] - u[t, i, j]*(DT/DX)*(v[t, i, j] - v[t, i-1, j]) - v[t, i, j]*(DT/DY)*(v[t, i, j] - v[t, i, j-1])
	typedef POINT_MAKE(
		POINT_GET_U(current)
		- POINT_GET_U(current) * (DT / DX) * (POINT_GET_U(current) - POINT_GET_U(previous_x))
		- POINT_GET_V(current) * (DT / DY) * (POINT_GET_U(current) - POINT_GET_U(previous_y)),
		POINT_GET_V(current)
		- POINT_GET_U(current) * (DT / DX) * (POINT_GET_V(current) - POINT_GET_V(previous_x))
		- POINT_GET_V(current) * (DT / DY) * (POINT_GET_V(current) - POINT_GET_V(previous_y))
	) type;
};

// for just rows
template<typename previous_row, typename current_row>
struct compute_convection_row;

template<typename previous_x_values, typename previous_y>
struct compute_convection_row<previous_x_values, typelist<previous_y, null_t> >
{
	typedef null_t type; // end of domain at y limit
};

template<
	typename ignored_previous, // u[t, i-1, j-1]
	typename previous_x,       // u[t, i-1, j]
	typename next_previous_x,  // u[t, i-1, j + 1]
	typename previous_y,       // u[t,   i, j-1]
	typename current,          // u[t,   i, j]
	typename next              // u[t,   i, j + 1]
>
struct compute_convection_row<
	typelist<ignored_previous, typelist<previous_x, next_previous_x> >,
	typelist<previous_y, typelist<current, next> > >
{
	typedef typelist<typename compute_convection_point<previous_x, previous_y, current>::type,
		typename compute_convection_row<
		typelist<previous_x, next_previous_x>,
		typelist<current, next> >::type
	> type;
};

#ifdef CTCFD_FASTTRACKED

template<
	TYPENAMES_6(px_y0, px_y1, px_y2, px_y3, px_y4, px_tail), // previous x, varying y
	TYPENAMES_6(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_tail)> // current x, varying y
struct compute_convection_row<
	TAILED_TYPELIST_5(px_y0, px_y1, px_y2, px_y3, px_y4, px_tail),
	TAILED_TYPELIST_5(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_tail) >
{
	typedef 
	typelist<typename compute_convection_point<px_y1, cx_y0, cx_y1>::type,
	typelist<typename compute_convection_point<px_y2, cx_y1, cx_y2>::type,
	typelist<typename compute_convection_point<px_y3, cx_y2, cx_y3>::type,
	typelist<typename compute_convection_point<px_y4, cx_y3, cx_y4>::type,
	typename compute_convection_row<
		typelist<px_y4, px_tail>, 
		typelist<cx_y4, cx_tail> >::type
			> > > > type;
};


template<
	TYPENAMES_18(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_y8, px_y9, px_y10, px_y11, px_y12, px_y13, px_y14, px_y15, px_y16, px_tail), // previous x, varying y
	TYPENAMES_18(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_y8, cx_y9, cx_y10, cx_y11, cx_y12, cx_y13, cx_y14, cx_y15, cx_y16, cx_tail)> // current x, varying y
struct compute_convection_row<
	TAILED_TYPELIST_17(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_y8, px_y9, px_y10, px_y11, px_y12, px_y13, px_y14, px_y15, px_y16, px_tail),
	TAILED_TYPELIST_17(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_y8, cx_y9, cx_y10, cx_y11, cx_y12, cx_y13, cx_y14, cx_y15, cx_y16, cx_tail) >
{
	typedef
		typelist<typename compute_convection_point<px_y1, cx_y0, cx_y1>::type,
		typelist<typename compute_convection_point<px_y2, cx_y1, cx_y2>::type,
		typelist<typename compute_convection_point<px_y3, cx_y2, cx_y3>::type,
		typelist<typename compute_convection_point<px_y4, cx_y3, cx_y4>::type,
		typelist<typename compute_convection_point<px_y5, cx_y4, cx_y5>::type,
		typelist<typename compute_convection_point<px_y6, cx_y5, cx_y6>::type,
		typelist<typename compute_convection_point<px_y7, cx_y6, cx_y7>::type,
		typelist<typename compute_convection_point<px_y8, cx_y7, cx_y8>::type,
		typelist<typename compute_convection_point<px_y9, cx_y8, cx_y9>::type,
		typelist<typename compute_convection_point<px_y10, cx_y9, cx_y10>::type,
		typelist<typename compute_convection_point<px_y11, cx_y10, cx_y11>::type,
		typelist<typename compute_convection_point<px_y12, cx_y11, cx_y12>::type,
		typelist<typename compute_convection_point<px_y13, cx_y12, cx_y13>::type,
		typelist<typename compute_convection_point<px_y14, cx_y13, cx_y14>::type,
		typelist<typename compute_convection_point<px_y15, cx_y14, cx_y15>::type,
		typelist<typename compute_convection_point<px_y16, cx_y15, cx_y16>::type,
		typename compute_convection_row<typelist<px_y16, px_tail>, typelist<cx_y16, cx_tail> >::type
		> > > > > > > > > > > > > > > > type;
};


#endif // CTCFD_FASTTRACKED

// computation for entire domain
template<typename values>
struct compute_convection;

template<typename previous_x>
struct compute_convection<typelist<previous_x, null_t> >
{
	typedef null_t type; // end of domain at x limit
};

template<typename previous_x, typename current_x, typename next_x>
struct compute_convection<typelist<previous_x, typelist<current_x, next_x> > >
{
	typedef typelist<
		typelist<POINT_MAKE(1, 1), // boundary condition and rest of calculation
		typename compute_convection_row<previous_x, current_x>::type
		>
		,
		typename compute_convection<typelist<current_x, next_x> >::type
	> type;
};

#ifdef CTCFD_FASTTRACKED
template<TYPENAMES_6(T1, T2, T3, T4, T5, tail)>
struct compute_convection<TAILED_TYPELIST_5(T1, T2, T3, T4, T5, tail)>
{
	typedef typelist<
		typelist<POINT_MAKE(1, 1), typename compute_convection_row<T1, T2>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T2, T3>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T3, T4>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T4, T5>::type>,
		typename compute_convection<typelist<T5, tail> >::type
		> > > >	type;
};

template<TYPENAMES_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, tail)>
struct compute_convection<TAILED_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, tail)>
{
	typedef typelist<
		typelist<POINT_MAKE(1, 1), typename compute_convection_row<T1, T2>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T2, T3>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T3, T4>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T4, T5>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T5, T6>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T6, T7>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T7, T8>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T8, T9>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T9, T10>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T10, T11>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T11, T12>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T12, T13>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T13, T14>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T14, T15>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T15, T16>::type>,
		typelist<typelist<POINT_MAKE(1, 1), typename compute_convection_row<T16, T17>::type>,
		typename compute_convection<typelist<T17, tail> >::type
		> > > >	> > > > > > > > > >	> > type;
};

#endif // CTCFD_FASTTRACKED

template<unsigned>
struct convection2d;

template<>
struct convection2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};

// Boundary conditions is u, v = 1 at each edge
// X boundary saved here
typedef convection2d<0>::type::head x_boundary;

template<unsigned timestep>
struct convection2d
{
	typedef typelist<x_boundary,
		typename compute_convection<typename convection2d<timestep - 1>::type>::type> type;
};

struct grid_x
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(index_i* DX) type;
	};
};

struct grid_y
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(index_j* DY) type;
	};
};

int main()
{
	std::cout << "{\"u0\": " << value_printer2d<convection2d<0>::type>()
		<< ",\n\"u" << NT << "\": " << value_printer2d<convection2d<NT>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}