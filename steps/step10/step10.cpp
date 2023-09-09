#include "step10_params.h"
#ifdef CTCFD_POISSON_SOR
#include "step10_sor_omega.h"
#endif


#include <util/for_i.h>
#include <util/number.h>
#include <util/zeros.h>
#include <util/conditional.h>
#include <util/valuelist.h>

#include <iostream>

typedef zeros<NY>::type boundary_near;

struct far_func
{
	template<unsigned index_j>
	struct func
	{
		// Special cases for dp/dy = 0 boundary condition
		// So use adjacent inner index for value instead of plain index_j*DY
		// First at y=0, then at y=limit, then normal.
		typedef NUMBER_MAKE((index_j == 0) ? 1 * DY : (index_j == (NY - 1)) ? (NY - 2) * DY : index_j * DY) type;
	};
};

typedef for_i<NY, far_func>::type boundary_far;

struct initial_func
{
	template<unsigned index_i>
	struct func
	{
		typedef typename conditional<index_i == (NX - 1), boundary_far, boundary_near /*zeros*/>::type type;
	};
};

template<TYPENAMES_4(px, nx, py, ny)> // previous x, next x, previous y, current y.
struct compute_poisson_point
{
	typedef NUMBER_MAKE(
		(DY* DY* (NUMBER_GET_TYPE(nx) + NUMBER_GET_TYPE(px)) +
			DX * DX * (NUMBER_GET_TYPE(ny) + NUMBER_GET_TYPE(py))) 
		/ (2 * (DX * DX + DY * DY))) type;
};

template<typename previous_x_row, typename current_x_row, typename next_x_row>
class compute_poisson_row;

// Specialization to apply boundary condition on far y-limit.
template<
	TYPENAMES_3(pxpy, pxcy, pxny), // p for previous, c for current, n for next. x for x, y for y.
	TYPENAMES_3(cxpy, cxcy, cxny), // pxpy = p[i-1, j-1], cxny = p[i, j+1]
	TYPENAMES_3(nxpy, nxcy, nxny)>
class compute_poisson_row<
	TYPELIST_3(pxpy, pxcy, pxny),
	TYPELIST_3(cxpy, cxcy, cxny),
	TYPELIST_3(nxpy, nxcy, nxny)>
{
	typedef typename compute_poisson_point<pxcy, nxcy, cxpy, cxny>::type computed;
#if defined(CTCFD_POISSON_SOR)
	typedef NUMBER_MAKE((1 - CTCFD_POISSON_SOR_OMEGA) * NUMBER_GET_TYPE(computed) + CTCFD_POISSON_SOR_OMEGA * NUMBER_GET_TYPE(computed)) sor_computed;
#endif

public:
	// Repeating computed enforces dp/dy = 0 at y=limit
	// Also stops recursion
#if defined(CTCFD_POISSON_SOR)
	typedef TYPELIST_2(sor_computed, sor_computed) type;
#else
	typedef TYPELIST_2(computed, computed) type;
#endif
};

template<
	TYPENAMES_4(pxpy, pxcy, pxny, pxtaily), // p[i-1, j-i], p[i-1, j], p[i-1, j+1], tail at p[i-1, ...]
	TYPENAMES_4(cxpy, cxcy, cxny, cxtaily), // p[i, j-1],   p[i, j],   p[i, j+1],   tail at p[i, ...]
	TYPENAMES_4(nxpy, nxcy, nxny, nxtaily)  // p[i+1, j-1], p[i+1, j], p[i+1, j+1], tail at p[i+1, ...]
>
class compute_poisson_row<
	TAILED_TYPELIST_3(pxpy, pxcy, pxny, pxtaily),
	TAILED_TYPELIST_3(cxpy, cxcy, cxny, cxtaily),
	TAILED_TYPELIST_3(nxpy, nxcy, nxny, nxtaily)>
{
	typedef typename compute_poisson_point<pxcy, nxcy, cxpy, cxny>::type computed;
public:
	typedef typelist<computed,
		typename compute_poisson_row<
		TAILED_TYPELIST_2(pxcy, pxny, pxtaily),
#if defined(CTCFD_POISSON_GAUSS_SEIDEL)
		// Mimic gauss-seidel iterations instead of jacobi iterations by using computed for following values
		TAILED_TYPELIST_2(computed, cxny, cxtaily),
#elif defined(CTCFD_POISSON_SOR)
		TAILED_TYPELIST_2(NUMBER_MAKE((1 - CTCFD_POISSON_SOR_OMEGA)*NUMBER_GET_TYPE(cxcy) + CTCFD_POISSON_SOR_OMEGA*NUMBER_GET_TYPE(computed)), cxny, cxtaily),
#else
		TAILED_TYPELIST_2(cxcy, cxny, cxtaily),
#endif
		// finally tail
		TAILED_TYPELIST_2(nxcy, nxny, nxtaily)>::type > type;
};

template<typename values>
class compute_poisson;

template<typename previous_x_row, typename current_x_row>
class compute_poisson<TYPELIST_2(previous_x_row, current_x_row)>
{
public:
	typedef TYPELIST_1(boundary_far) type;
};

template<typename previous_x_row, typename current_x_row, typename next_x_row, typename further_x>
class compute_poisson<TAILED_TYPELIST_3(previous_x_row, current_x_row, next_x_row, further_x)>
{
	typedef typename compute_poisson_row<previous_x_row, current_x_row, next_x_row>::type computed_row;
	typedef typelist<typename computed_row::head, computed_row> result; // repeat computed_row::head to enfore dp/dy = 0 at y=minimum
public:
	typedef typelist<result,
#if defined(CTCFD_POISSON_GAUSS_SEIDEL) || defined(CTCFD_POISSON_SOR)
		// Mimic gauss-seidel iterations instead of jacobi iterations by using computed for following values
		typename compute_poisson<TAILED_TYPELIST_2(result, next_x_row, further_x)>::type>
#else
		typename compute_poisson<TAILED_TYPELIST_2(current_x_row, next_x_row, further_x)>::type>
#endif
		type;
};

template<unsigned iteration>
struct poisson;

template<>
struct poisson<0>
{
	typedef for_i<NX, initial_func>::type type;
};

template<>
struct poisson<1>
{
	typedef typelist<boundary_near, compute_poisson<poisson<0>::type>::type> type;
};

template<unsigned iteration>
struct poisson
{
	typedef typelist<boundary_near, typename compute_poisson<typename poisson<iteration - 1>::type>::type> type;
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
#define PRESSURE_AT(iteration) << ",\n\"p" << iteration << "\": " << value_printer2d<poisson<iteration>::type>()
	std::cout << "{\"p0\":" << value_printer2d<poisson<0>::type>()
		PRESSURE_AT(NITERS)
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}
