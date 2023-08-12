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
	TYPENAMES_9(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_tail), // previous x, varying y
	TYPENAMES_9(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_tail)> // current x, varying y
struct compute_convection_row<
	TAILED_TYPELIST_8(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_tail),
	TAILED_TYPELIST_8(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_tail) >
{
	typedef 
	typelist<typename compute_convection_point<px_y1, cx_y0, cx_y1>::type,
		typelist<typename compute_convection_point<px_y2, cx_y1, cx_y2>::type,
			typelist<typename compute_convection_point<px_y3, cx_y2, cx_y3>::type,
				typelist<typename compute_convection_point<px_y4, cx_y3, cx_y4>::type,
					typelist<typename compute_convection_point<px_y5, cx_y4, cx_y5>::type,
						typelist<typename compute_convection_point<px_y6, cx_y5, cx_y6>::type,
							typelist<typename compute_convection_point<px_y7, cx_y6, cx_y7>::type,
				typename compute_convection_row<
							typelist<px_y7, px_tail>, 
							typelist<cx_y7, cx_tail> >::type
			> > > > > > > type;
};


template<
	TYPENAMES_8(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7), // previous x, varying y
	TYPENAMES_8(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7)> // current x, varying y
struct compute_convection_row<
	TYPELIST_8(px_y0, px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7),
	TYPELIST_8(cx_y0, cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7) >
{
	//typedef null_t type;
	typedef typelist<typename compute_convection_point<px_y1, cx_y0, cx_y1>::type,
				typelist<typename compute_convection_point<px_y2, cx_y1, cx_y2>::type,
					typelist<typename compute_convection_point<px_y3, cx_y2, cx_y3>::type,
						typelist<typename compute_convection_point<px_y4, cx_y3, cx_y4>::type,
							typelist<typename compute_convection_point<px_y5, cx_y4, cx_y5>::type,
								typelist<typename compute_convection_point<px_y6, cx_y5, cx_y6>::type,
									typelist<typename compute_convection_point<px_y7, cx_y6, cx_y7>::type,
		null_t> > > > > > > type;
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
		typelist<POINT_MAKE(1, 1), // boundary condition and rest of calculatin
		typename compute_convection_row<previous_x, current_x>::type
		>
		,
		typename compute_convection<typelist<current_x, next_x> >::type
	> type;
};

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