#include "step11_params.h"

#include <util/point2d.h>
#include <util/typelist.h>
#include <util/zeros.h>

// Builds pressure forcing term to be used in poisson solver

template<typename px, typename nx, typename py, typename ny>
struct pressure_rhs_point
{
    typedef NUMBER_MAKE((RHO*((1.0/DT)*
    (
        (POINT_GET_U(nx) - POINT_GET_U(px))/(2.0*DX)
        + (POINT_GET_V(ny) - POINT_GET_V(py)/(2.0*DY))
    )
    - (((POINT_GET_U(nx) - POINT_GET_U(px))/(2.0*DX))*((POINT_GET_U(nx) - POINT_GET_U(px))/(2.0*DX)))
    - 2*((POINT_GET_U(ny) - POINT_GET_U(py))*(1/(2.0*DY))*(POINT_GET_V(nx) - POINT_GET_V(px))*(1/(2.0*DX)))
    - (((POINT_GET_V(ny) - POINT_GET_V(py))/(2.0*DY))*((POINT_GET_V(ny) - POINT_GET_V(py))/(2.0*DY)))
    ))) type;
};

template<typename prev_row, typename curr_row, typename next_row>
struct pressure_rhs_row;

// forcing 0 at y limit
template<
	TYPENAMES_2(pxpy, pxcy), // u[t, i-1, j-1], u[t, i-1, j]
	TYPENAMES_2(cxpy, cxcy), // u[t, i, j-1], u[t, i, j]
	TYPENAMES_2(nxpy, nxcy)  // u[t, i+1, j-1], u[[t, i+1, j]
>
struct pressure_rhs_row<
	TYPELIST_2(pxpy, pxcy),
	TYPELIST_2(cxpy, cxcy),
	TYPELIST_2(nxpy, nxcy)
>
{
  typedef TYPELIST_1(NUMBER_MAKE(0.0)) type;
};

template<
	TYPENAMES_3(pxpy, pxcy, pxny_tail), // u[t, i-1, j-1], u[t, i-1, j], u[t, i-1, j+1] with tail
	TYPENAMES_3(cxpy, cxcy, cxny_tail), // u[t, i, j-1], u[t, i, j], u[t, i, j+1] with tail 
	TYPENAMES_3(nxpy, nxcy, nxny_tail)  // u[t, i+1, j-1], u[[t, i+1, j], u[t, i+1, j+1] with tail
>
struct pressure_rhs_row<
	TAILED_TYPELIST_2(pxpy, pxcy, pxny_tail),
	TAILED_TYPELIST_2(cxpy, cxcy, cxny_tail),
	TAILED_TYPELIST_2(nxpy, nxcy, nxny_tail)
>
{
  typedef typelist<
    typename pressure_rhs_point<pxcy, nxcy, cxpy, typename cxny_tail::head>::type,
    typename pressure_rhs_row<
      typelist<pxcy, pxny_tail>,
      typelist<cxcy, cxny_tail>,
      typelist<nxcy, nxny_tail> >::type
    > type;
};


// pressure_rhs_impl allows pressure_rhs to apply near boundary conditions
template<typename values>
struct pressure_rhs_impl;

// forcing 0 at far x limit for all x values
template<typename previous_x, typename current_x>
struct pressure_rhs_impl<TYPELIST_2(previous_x, current_x)>
{
  typedef zeros<NX>::type type;
};


template<typename previous_x, typename current_x, typename next_x, typename tail_x>
struct pressure_rhs_impl<TAILED_TYPELIST_3(previous_x, current_x, next_x, tail_x)>
{
  typedef typelist<
    typelist<NUMBER_MAKE(0.0), // force 0 at near y limit
    typename pressure_rhs_row<previous_x, current_x, next_x>::type
	     >,
    typename pressure_rhs_impl<TAILED_TYPELIST_2(current_x, next_x, tail_x)>::type
    > type;
};


template<typename x0, typename x1, typename x2, typename x3>
struct pressure_rhs
{
  typedef typelist<zeros<NX>::type,
		   typename pressure_rhs_impl<TAILED_TYPELIST_3(x0, x1, x2, x3)>::type> type;
};


