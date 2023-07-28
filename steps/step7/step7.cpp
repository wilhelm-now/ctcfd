#include "step7_params.h"

#include <util/typelist.h>
#include <util/number.h>

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
	struct func
	{
		typedef NUMBER_MAKE(
			(0.5 <= DX * index_i && DX * index_i <= 1.0) && (0.5 <= DY * index_j && DY * index_j <= 1.0)
			? 2.0 : 1.0)
			type;
	};
};

// for just a point
template<TYPENAMES_5(px, nx, current, py, ny)> // previous and next x, current, previous and next y
struct compute_diffusion_point
{
	// u[t+1, i, j] = u[t, i, j] 
	//               + (NU*DT/(DX^2))*(u[t, i+1, j] - 2*u[t, i, j] + u[t, i-1, j])
	//               + (NU*DT/(DY^2))*(u[t, i, j+1] - 2*u[t, i, j] + u[t, i, j-1])
	typedef typename NUMBER_MAKE(
		NUMBER_GET_TYPE(current)
		+ (NU*DT/(DX*DX))*(NUMBER_GET_TYPE(nx) - 2*NUMBER_GET_TYPE(current) + NUMBER_GET_TYPE(px))
		+ (NU*DT/(DY*DY))*(NUMBER_GET_TYPE(ny) - 2*NUMBER_GET_TYPE(current) + NUMBER_GET_TYPE(py))
	) type;
};

// for just rows
template<typename previous_x_row, typename current_x_row, typename next_x_row>
struct compute_diffusion_row;

template<typename previous_x_values, typename current_x_previous_y, typename current_x, typename next_x_row>
struct compute_diffusion_row<
	previous_x_values, 
	typelist<current_x_previous_y, typelist<current_x, null_t> >,
	next_x_row>
{
	typedef null_t type; // end of domain at y limit
};

template<
	TYPENAMES_3(pxpy, pxcy, pxny_tail), // u[t, i-1, j-1], u[t, i-1, j], u[t, i-1, j+1] with tail
	TYPENAMES_3(cxpy, cxcy, cxny_tail), // u[t, i, j-1], u[t, i, j], u[t, i, j+1] with tail 
	TYPENAMES_3(nxpy, nxcy, nxny_tail)  // u[t, i+1, j-1], u[[t, i+1, j], u[t, i+1, j+1] with tail
>
struct compute_diffusion_row<
	TAILED_TYPELIST_2(pxpy, pxcy, pxny_tail),
	TAILED_TYPELIST_2(cxpy, cxcy, cxny_tail),
	TAILED_TYPELIST_2(nxpy, nxcy, nxny_tail)
>
{
	typedef typelist<
		typename compute_diffusion_point<pxcy, nxcy, cxcy, cxpy, cxny_tail::head>::type,
		typename compute_diffusion_row<
			typelist<pxcy, pxny_tail>,
			typelist<cxcy, cxny_tail>,
			typelist<nxcy, nxny_tail> >::type
	> type;
};

// computation for entire domain
template<typename values>
struct compute_diffusion;

template<typename previous_x, typename current_x>
struct compute_diffusion<typelist<previous_x, typelist<current_x, null_t> > >
{
	typedef null_t type; // end of domain at x limit
};

template<typename previous_x, typename current_x, typename next_x, typename x_tail>
struct compute_diffusion<TAILED_TYPELIST_3(previous_x, current_x, next_x, x_tail)>
{
	typedef typelist<
		typelist<NUMBER_MAKE(1), // boundary condition and rest of calculatin
		typename compute_diffusion_row<previous_x, current_x, next_x>::type
		>
		,
		typename compute_diffusion<typelist<current_x, typelist<next_x, x_tail> > >::type
	> type;
};

template<unsigned>
struct diffusion2d;

template<>
struct diffusion2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};

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