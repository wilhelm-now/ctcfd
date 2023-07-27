#include "step7_params.h"

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
struct compute_diffusion_point
{
	// u[t+1, i, j] = u[t, i, j] - u[t, i, j]*(DT/DX)*(u[t, i, j] - u[t, i-1, j]) - v[t, i, j]*(DT/DY)*(u[t, i, j] - u[t, i, j-1])
	// v[t+1, i, j] = v[t, i, j] - u[t, i, j]*(DT/DX)*(v[t, i, j] - v[t, i-1, j]) - v[t, i, j]*(DT/DY)*(v[t, i, j] - v[t, i, j-1])
	typedef typename POINT_MAKE(
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
struct compute_diffusion_row;

template<typename previous_x_values, typename previous_y>
struct compute_diffusion_row<previous_x_values, typelist<previous_y, null_t> >
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
struct compute_diffusion_row<
	typelist<ignored_previous, typelist<previous_x, next_previous_x> >,
	typelist<previous_y, typelist<current, next> > >
{
	typedef typelist<typename compute_diffusion_point<previous_x, previous_y, current>::type,
		typename compute_diffusion_row<
		typelist<previous_x, next_previous_x>,
		typelist<current, next> >::type
	> type;
};

// computation for entire domain
template<typename values>
struct compute_diffusion;

template<typename previous_x>
struct compute_diffusion<typelist<previous_x, null_t> >
{
	typedef null_t type; // end of domain at x limit
};

template<typename previous_x, typename current_x, typename next_x>
struct compute_diffusion<typelist<previous_x, typelist<current_x, next_x> > >
{
	typedef typelist<
		typelist<POINT_MAKE(1, 1), // boundary condition and rest of calculatin
		typename compute_diffusion_row<previous_x, current_x>::type
		>
		,
		typename compute_diffusion<typelist<current_x, next_x> >::type
	> type;
};

template<unsigned>
struct diffusion2d;

template<>
struct diffusion2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};

// Boundary conditions is u, v = 1 at each edge
// X boundary saved here
typedef diffusion2d<0>::type::head x_boundary;

template<unsigned timestep>
struct diffusion2d
{
	typedef typelist<x_boundary,
		typename compute_diffusion<typename diffusion2d<timestep - 1>::type>::type> type;
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
	std::cout << "{\"u0\": " << value_printer2d<diffusion2d<0>::type>()
		<< ",\n\"u" << NT << "\": " << value_printer2d<diffusion2d<NT>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}