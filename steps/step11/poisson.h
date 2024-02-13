#include "step11_params.h"
#ifdef CTCFD_POISSON_SOR
#include "step11_sor_omega.h"
#endif


#include <util/for_i.h>
#include <util/number.h>
#include <util/zeros.h>
#include <util/conditional.h>
#include <util/valuelist.h>

#include <iostream>

// Poisson solver for step11 pressure
// dp/dy = 0 at y = LOWER_LIMIT = 0
// y = 0 at y = UPPER_LIMIT = 2
// dp/dx = 0 at x = LOWER_LIMIT, UPPER_LIMIT = 0, 2

#ifdef CTCFD_POISSON_JACOBI
#define CTCFD_PREVIOUS_VALUE(previous, computed) previous
#define CTCFD_PREVIOUS_ROW(previous_row, computed_row) previous_row
#elif defined(CTCFD_POISSON_GAUSS_SEIDEL)
#define CTCFD_PREVIOUS_VALUE(previous, computed) computed
#define CTCFD_PREVIOUS_ROW(previous_row, computed_row) computed_row
#elif defined(CTCFD_POISSON_SOR)
#define CTCFD_PREVIOUS_VALUE(previous, computed) NUMBER_MAKE((1 - CTCFD_POISSON_SOR_OMEGA)*NUMBER_GET_TYPE(previous) + CTCFD_POISSON_SOR_OMEGA*NUMBER_GET_TYPE(computed))
#define CTCFD_PREVIOUS_ROW(previous_row, computed_row) computed_row
#endif


template<TYPENAMES_5(px, nx, py, ny, b)> // previous x, next x, previous y, current y, source term at this location
struct compute_poisson_point
{
	typedef NUMBER_MAKE(
		(DY * DY * (NUMBER_GET_TYPE(nx) + NUMBER_GET_TYPE(px))
		 + DX * DX * (NUMBER_GET_TYPE(ny) + NUMBER_GET_TYPE(py))
		 - DX * DX * DY * DY * NUMBER_GET_TYPE(b)) 
		/ (2 * (DX * DX + DY * DY))) type;
};

template<typename previous_x_row, typename current_x_row, typename next_x_row, typename current_b_row>
class compute_poisson_row;

//Specialization to apply boundary condition on far y-limit.
template<
  TYPENAMES_3(pxpy, pxcy, pxny),
  TYPENAMES_3(cxpy, cxcy, cxny),
  TYPENAMES_3(nxpy, nxcy, nxny),
  TYPENAMES_3(cxpy_b, cxcy_b, cxny_b)>
class compute_poisson_row<
  TYPELIST_3(pxpy, pxcy, pxny),      
  TYPELIST_3(cxpy, cxcy, cxny),      
  TYPELIST_3(nxpy, nxcy, nxny),      
  TYPELIST_3(cxpy_b, cxcy_b, cxny_b)>
{
	typedef typename compute_poisson_point<pxcy, nxcy, cxpy, cxny, cxcy_b>::type computed;
public:
	typedef TYPELIST_2(computed, NUMBER_MAKE(0.0)) type; // pressure = 0 at far y limit
};

template<
	TYPENAMES_4(pxpy, pxcy, pxny, pxtaily), // p[i-1, j-i], p[i-1, j], p[i-1, j+1], tail at p[i-1, ...]
	TYPENAMES_4(cxpy, cxcy, cxny, cxtaily), // p[i, j-1],   p[i, j],   p[i, j+1],   tail at p[i, ...]
	TYPENAMES_4(nxpy, nxcy, nxny, nxtaily),  // p[i+1, j-1], p[i+1, j], p[i+1, j+1], tail at p[i+1, ...]
	TYPENAMES_4(cxpy_b, cxcy_b, cxny_b, cxtaily_b) // source terms
>
class compute_poisson_row<
	TAILED_TYPELIST_3(pxpy, pxcy, pxny, pxtaily),
	TAILED_TYPELIST_3(cxpy, cxcy, cxny, cxtaily),
	TAILED_TYPELIST_3(nxpy, nxcy, nxny, nxtaily),
	TAILED_TYPELIST_3(cxpy_b, cxcy_b, cxny_b, cxtaily_b)>
{
	typedef typename compute_poisson_point<pxcy, nxcy, cxpy, cxny, cxcy_b>::type computed;
public:
	typedef typelist<computed,
		typename compute_poisson_row<
		TAILED_TYPELIST_2(pxcy, pxny, pxtaily),
		TAILED_TYPELIST_2(CTCFD_PREVIOUS_VALUE(cxcy, computed), cxny, cxtaily),
		// finally tail
		TAILED_TYPELIST_2(nxcy, nxny, nxtaily),
		TAILED_TYPELIST_2(cxcy_b, cxny_b, cxtaily_b)>::type > type;
};


template<typename values, typename source_b>
class compute_poisson;

template<typename previous_x_row, typename current_x_row, typename edge_x_row, typename previous_source, typename current_source, typename edge_source>
class compute_poisson<TYPELIST_3(previous_x_row, current_x_row, edge_x_row), TYPELIST_3(previous_source, current_source, edge_source)>
{
	typedef typename compute_poisson_row<previous_x_row, current_x_row, edge_x_row, current_source>::type computed_row;
	typedef typelist<typename computed_row::head, computed_row> result; // Set dp/dy=0 at minimum y
public:
	typedef TYPELIST_2(result, result) type; // repeat result to get dp/dx = 0 at maximum x
};

template<typename previous_x_row, typename current_x_row, typename next_x_row, typename further_x, TYPENAMES_4(previous_source, current_source, next_source, further_source)>
class compute_poisson<TAILED_TYPELIST_3(previous_x_row, current_x_row, next_x_row, further_x), TAILED_TYPELIST_3(previous_source, current_source, next_source, further_source)>
{
	typedef typename compute_poisson_row<previous_x_row, current_x_row, next_x_row, current_source>::type computed_row;
	typedef typelist<typename computed_row::head, computed_row> result; // Set dp/dy=0 at minimum y
public:
	typedef typelist<result,
		typename compute_poisson<
		TAILED_TYPELIST_2(CTCFD_PREVIOUS_ROW(current_x_row, result), next_x_row, further_x),
		TAILED_TYPELIST_2(current_source, next_source, further_source)>::type>
		type;
};


template<unsigned iteration, typename initial, typename source_b>
struct poisson;

template<typename initial, typename source_b>
struct poisson<0, initial, source_b>
{
	typedef initial type; // done with iterations
};

template<unsigned iterations, typename initial, typename source_b>
struct poisson
{
private:
	typedef typename compute_poisson<initial, source_b>::type computed;
	// repeat first row for dp/dx = 0 at x = 0 (near limit)
	typedef typelist<typename computed::head, computed> intermediate;
public:
	// actual result
	typedef typename poisson<iterations - 1, intermediate, source_b>::type type;
};
