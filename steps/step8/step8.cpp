#include "step8_params.h"

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
template<TYPENAMES_5(px, nx, current, py, ny)> // previous and next x, current, previous and next y
class compute_burgers_point
{
	// u[t+1, i, j] = u[t, i, j] 
	//               - (DT/DX)*(u[t, i, j])*(u[t, i, j] - u[t, i-i, j])
	//               - (DT/DY)*(v[t, i, j])*(u[t, i, j] - u[t, i, j-1])
	//               + (NU*DT/(DX^2))*(u[t, i+1, j] - 2*u[t, i, j] + u[t, i-1, j])
	//               + (NU*DT/(DY^2))*(u[t, i, j+1] - 2*u[t, i, j] + u[t, i, j-1])
	// v[t+1, i, j] = v[t, i, j]
	//               - (DT/DX)*(u[t, i, j])*(v[t, i, j] - v[t, i-1, j])
	//               - (DT/DY)*(v[t, i, j])*(v[t, i, j] - v[t, i, j-1])
	//               + (NU*DT/(DX^2))*(v[t, i+1, j] - 2*v[t, i, j] + v[t, i-1, j])
	//               + (NU*DT/(DX^2))*(v[t, i, j+1] - 2*v[t, i, j] + v[t, i, j-1])
	typedef typename NUMBER_MAKE(
		POINT_GET_U(current)
		- (DT/DX)*(POINT_GET_U(current))*(POINT_GET_U(current) - POINT_GET_U(px))
		- (DT/DY)*(POINT_GET_V(current))*(POINT_GET_U(current) - POINT_GET_U(py))
		+ (NU*DT/(DX*DX))*(POINT_GET_U(nx) - 2* POINT_GET_U(current) + POINT_GET_U(px))
		+ (NU*DT/(DY*DY))*(POINT_GET_U(ny) - 2* POINT_GET_U(current) + POINT_GET_U(py))
	) u;
	typedef typename NUMBER_MAKE(
		POINT_GET_V(current)
		- (DT/DX)*(POINT_GET_U(current))*(POINT_GET_V(current) - POINT_GET_V(px))
		- (DT/DY)*(POINT_GET_V(current))*(POINT_GET_V(current) - POINT_GET_V(py))
		+ (NU*DT/(DX*DX))*(POINT_GET_V(nx) - 2*POINT_GET_V(current) + POINT_GET_V(px))
		+ (NU*DT/(DY*DY))*(POINT_GET_V(ny) - 2*POINT_GET_V(current) + POINT_GET_V(py))
	) v;
public:
	typedef point2d_c<u, v> type;
};

// for just rows
template<typename previous_x_row, typename current_x_row, typename next_x_row>
struct compute_burgers_row;

// Specialization to apply boundary condition on y limit
template<
	TYPENAMES_2(pxpy, pxcy), // u[t, i-1, j-1], u[t, i-1, j]
	TYPENAMES_2(cxpy, cxcy), // u[t, i, j-1], u[t, i, j]
	TYPENAMES_2(nxpy, nxcy)  // u[t, i+1, j-1], u[[t, i+1, j]
>
struct compute_burgers_row <
	TYPELIST_2(pxpy, pxcy),
	TYPELIST_2(cxpy, cxcy),
	TYPELIST_2(nxpy, nxcy)
>
{
	typedef TYPELIST_1(POINT_MAKE(1, 1)) type;
};

template<
	TYPENAMES_3(pxpy, pxcy, pxny_tail), // u[t, i-1, j-1], u[t, i-1, j], u[t, i-1, j+1] with tail
	TYPENAMES_3(cxpy, cxcy, cxny_tail), // u[t, i, j-1], u[t, i, j], u[t, i, j+1] with tail 
	TYPENAMES_3(nxpy, nxcy, nxny_tail)  // u[t, i+1, j-1], u[[t, i+1, j], u[t, i+1, j+1] with tail
>
struct compute_burgers_row<
	TAILED_TYPELIST_2(pxpy, pxcy, pxny_tail),
	TAILED_TYPELIST_2(cxpy, cxcy, cxny_tail),
	TAILED_TYPELIST_2(nxpy, nxcy, nxny_tail)
>
{
	typedef typelist<
		typename compute_burgers_point<pxcy, nxcy, cxcy, cxpy, typename cxny_tail::head>::type,
		typename compute_burgers_row<
			typelist<pxcy, pxny_tail>,
			typelist<cxcy, cxny_tail>,
			typelist<nxcy, nxny_tail> >::type
	> type;
};

// computation for entire domain
template<typename values>
struct compute_burgers;

template<typename previous_x, typename current_x>
struct compute_burgers<typelist<previous_x, typelist<current_x, null_t> > >
{
	typedef TYPELIST_1(current_x) type; // end of domain at x limit
};

template<typename previous_x, typename current_x, typename next_x, typename x_tail>
struct compute_burgers<TAILED_TYPELIST_3(previous_x, current_x, next_x, x_tail)>
{
	typedef typelist<
		typelist<POINT_MAKE(1, 1), // boundary condition and rest of calculatin
		typename compute_burgers_row<previous_x, current_x, next_x>::type
		>
		,
		typename compute_burgers<typelist<current_x, typelist<next_x, x_tail> > >::type
	> type;
};

template<unsigned>
struct burgers2d;

template<>
struct burgers2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};

// X boundary saved here
typedef burgers2d<0>::type::head x_boundary;

template<unsigned timestep>
struct burgers2d
{
	typedef typelist<x_boundary,
		typename compute_burgers<typename burgers2d<timestep - 1>::type>::type> type;
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
	std::cout << "{\"u0\": " << value_printer2d<burgers2d<0>::type>()
		<< ",\n\"u" << NT << "\": " << value_printer2d<burgers2d<NT>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}